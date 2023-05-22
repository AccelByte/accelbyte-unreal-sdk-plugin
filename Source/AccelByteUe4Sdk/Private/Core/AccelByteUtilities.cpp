// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteUserModels.h"
#include "Misc/CommandLine.h"
#include "Misc/CString.h"
#include <memory>
#include "Kismet/GameplayStatics.h"
#include "Misc/SecureHash.h"
#include "Misc/Base64.h"

using namespace AccelByte;

#if !PLATFORM_SWITCH
// enclosing with namespace because of collision with Unreal types
namespace openssl
{
THIRD_PARTY_INCLUDES_START
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
THIRD_PARTY_INCLUDES_END
}

using namespace openssl;

class FScopedEVPMDContext
{
public:
	/**
	 * Creates our OpenSSL Message Digest Context on construction
	 */
	FScopedEVPMDContext() :
		Context(EVP_MD_CTX_create())
	{
	}

	/** Disable copying/assigning */
	FScopedEVPMDContext(FScopedEVPMDContext&) = delete;
	FScopedEVPMDContext& operator=(FScopedEVPMDContext&) = delete;

	/**
	 * Free our OpenSSL Message Digest Context
	 */
	~FScopedEVPMDContext()
	{
		EVP_MD_CTX_destroy(Context);
	}

	/**
	 * Get our OpenSSL Message Digest Context
	 */
	EVP_MD_CTX* Get() const { return Context; }

private:
	EVP_MD_CTX* Context;
};
#endif


int32 constexpr RS256_SIGNATURE_LENGTH = 342;
int32 constexpr RSA_MODULUS_BASE64_LENGTH = 342;
int32 constexpr RSA_EXPONENT_BASE64_LENGTH = 4;


void UnescapeB64Url(FString& B64UrlString)
{
	B64UrlString.ReplaceCharInline('-', '+');
	B64UrlString.ReplaceCharInline('_', '/');	
}


FRsaPublicKey::FRsaPublicKey(FString ModulusB64Url, FString ExponentB64Url) :
	ModulusB64Url{MoveTemp(ModulusB64Url)},
	ExponentB64Url{MoveTemp(ExponentB64Url)}
{}

bool FRsaPublicKey::IsValid() const
{
	return ModulusB64Url.Len() == RSA_MODULUS_BASE64_LENGTH && ExponentB64Url.Len() == RSA_EXPONENT_BASE64_LENGTH;
}

FString FRsaPublicKey::ToPem() const
{
	FString UnarmoredPem = FString::Printf(
		TEXT("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA%sID%s"),
		*ModulusB64Url,
		*ExponentB64Url);

	UnescapeB64Url(UnarmoredPem);

	return FString::Printf(TEXT("-----BEGIN PUBLIC KEY-----\n%s\n-----END PUBLIC KEY-----"), *UnarmoredPem);
}

int32 GetHeaderEnd(FString const& JwtString)
{
	int32 HeaderEnd;
	JwtString.FindChar('.', HeaderEnd);

	return HeaderEnd;
}


int32 GetPayloadEnd(FString const& JwtString)
{
	int32 PayloadEnd;
	JwtString.FindLastChar('.', PayloadEnd);

	return PayloadEnd;
}

TSharedPtr<FJsonObject> ExtractJson(FString const& JsonB64Url, int32 Start, int32 Count)
{
	if (JsonB64Url.IsEmpty())
	{
		return nullptr;
	}
	
	FString JsonStr = JsonB64Url.Mid(Start, Count);
	UnescapeB64Url(JsonStr);
	FBase64::Decode(JsonStr, JsonStr);

	TSharedPtr<FJsonObject> JsonObjectPtr{ MakeShared<FJsonObject>() };

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonStr), JsonObjectPtr))
	{
		return nullptr;
	}	

	return JsonObjectPtr;
}

FJwt::FJwt(FString JwtString)
	:
	JwtString{MoveTemp(JwtString)},
	HeaderEnd{GetHeaderEnd(this->JwtString)},
	PayloadEnd{GetPayloadEnd(this->JwtString)},
	HeaderJsonPtr{ExtractJson(this->JwtString, 0, HeaderEnd)},
	PayloadJsonPtr{ExtractJson(this->JwtString, HeaderEnd + 1, PayloadEnd - HeaderEnd - 1)}
{
}

EJwtResult FJwt::VerifyWith(FRsaPublicKey Key) const
{
#if !PLATFORM_SWITCH
	if (!IsValid())
	{
		return EJwtResult::MalformedJwt;
	}

	if (!Key.IsValid())
	{
		return EJwtResult::MalformedPublicKey;
	}

	if (!HeaderJsonPtr->HasField("alg") || HeaderJsonPtr->GetStringField("alg") != TEXT("RS256"))
	{
		return EJwtResult::AlgorithmMismatch;
	}

	std::unique_ptr<BIO, decltype(&BIO_free_all)> const BIOPublicKeyPtr{BIO_new(BIO_s_mem()), BIO_free_all};

	if (!BIOPublicKeyPtr)
	{
		return EJwtResult::SignatureMismatch;
	}

	FString const PemPublicKey = Key.ToPem();
	int const KeyLength = PemPublicKey.Len();

	if (BIO_write(BIOPublicKeyPtr.get(), StringCast<ANSICHAR>(*PemPublicKey).Get(), KeyLength) != KeyLength)
	{
		return EJwtResult::SignatureMismatch;
	}

	FString const Password{};
	std::shared_ptr<EVP_PKEY> const PublicKeyPtr{
		PEM_read_bio_PUBKEY(BIOPublicKeyPtr.get(), nullptr, nullptr, (void*)StringCast<ANSICHAR>(*Password).Get()),
		EVP_PKEY_free};

	if (!PublicKeyPtr)
	{
		return EJwtResult::SignatureMismatch;
	}
	
	FScopedEVPMDContext MDContext;

	if (MDContext.Get() == nullptr)
	{
		return EJwtResult::SignatureMismatch;
	}

	if (!EVP_VerifyInit(MDContext.Get(), EVP_sha256()))
	{
		return EJwtResult::SignatureMismatch;
	}

	if (!EVP_VerifyUpdate(MDContext.Get(), TCHAR_TO_ANSI(*JwtString), PayloadEnd))
	{
		return EJwtResult::SignatureMismatch;
	}

	FString SignatureB64 = JwtString.Right(JwtString.Len() - PayloadEnd - 1);
	UnescapeB64Url(SignatureB64);
	TArray<uint8> SignatureBytes{};
	FBase64::Decode(SignatureB64, SignatureBytes);

	auto const VerifyResult = EVP_VerifyFinal(
		MDContext.Get(),
		SignatureBytes.GetData(),
		SignatureBytes.Num(),
		PublicKeyPtr.get());

	if (VerifyResult != 1)
	{
		return EJwtResult::SignatureMismatch;
	}
	return EJwtResult::Ok;
#else
	FReport::Log(TEXT("VerifyWith is disabled on nintendo switch!"));
	return EJwtResult::MalformedJwt;
#endif
}

TSharedPtr<FJsonObject> const& FJwt::Header() const
{
	return HeaderJsonPtr;
}

TSharedPtr<FJsonObject> const& FJwt::Payload() const
{
	return PayloadJsonPtr;
}

bool FJwt::IsValid() const
{
	return
		HeaderEnd != INDEX_NONE
		&& PayloadEnd != INDEX_NONE
		&& HeaderEnd != PayloadEnd
		&& HeaderJsonPtr != nullptr
		&& PayloadJsonPtr != nullptr
		&& JwtString.Len() - PayloadEnd - 1 == RS256_SIGNATURE_LENGTH;
}

void FAccelByteUtilities::RemoveEmptyStrings(TSharedPtr<FJsonObject> JsonPtr)
{
	TArray<FString> KeysToRemove;
	for (auto& Item : JsonPtr->Values)
	{
		if (Item.Value->Type == EJson::String && Item.Value->AsString() == "")
		{
			KeysToRemove.Add(Item.Key);
		}
		else if (Item.Value->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> const Child = Item.Value->AsObject();
			RemoveEmptyStrings(Child);
		}
	}

	for (FString const& Key : KeysToRemove)
	{
		JsonPtr->RemoveField(Key);
	}
}

FString FAccelByteUtilities::GetPlatformString(EAccelBytePlatformType Platform)
{
	switch (Platform)
	{
	case EAccelBytePlatformType::Steam:
		return "steam";
	case EAccelBytePlatformType::PS4:
		return "ps4";
	case EAccelBytePlatformType::PS4CrossGen:
	case EAccelBytePlatformType::PS5:
		return "ps5";
	case EAccelBytePlatformType::Live:
		return "live";
	case EAccelBytePlatformType::Google:
		return "google";
	case EAccelBytePlatformType::Apple:
		return "apple";	
	case EAccelBytePlatformType::Facebook:
		return "facebook";
	case EAccelBytePlatformType::Android:
		return "android";
	case EAccelBytePlatformType::iOS:
		return "ios";
	case EAccelBytePlatformType::Device:
		return "device";
	case EAccelBytePlatformType::Twitch:
		return "twitch";
	case EAccelBytePlatformType::Oculus:
		return "oculus";
	case EAccelBytePlatformType::Twitter:
		return "twitter";
	case EAccelBytePlatformType::EpicGames:
		return "epicgames"; 
	case EAccelBytePlatformType::AwsCognito:
		return "awscognito";
	case EAccelBytePlatformType::Discord:
		return "discord";
	case EAccelBytePlatformType::Nintendo:
		return "nintendo";	
	case EAccelBytePlatformType::SnapChat:
		return "snapchat";
	case EAccelBytePlatformType::EAOrigin:
		return "eaorigin";
	default:
		return "unknown";
	}
}

FString FAccelByteUtilities::GetAuthenticatorString(EAccelByteLoginAuthFactorType Authenticator)
{
	switch (Authenticator)
	{
	case EAccelByteLoginAuthFactorType::Authenticator:
		return "authenticator";
	case EAccelByteLoginAuthFactorType::BackupCode:
		return "backupCode";
	default:
		return "unknown";
	}
}

FString FAccelByteUtilities::RandomizeDeviceId()
{
	FString NewDeviceId;
	TCHAR valid[] = { '0', '1', '2', '3', '4', '5', '6','7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
					'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

	const int VALID_CHARACTER_NUMBER = 35;
	const int DEVICE_ID_LENGTH = 30;

	for (int i = 0; i < DEVICE_ID_LENGTH; i++)
	{
		NewDeviceId = NewDeviceId + valid[FMath::RandRange(0, VALID_CHARACTER_NUMBER)];
	}
	return NewDeviceId;
}

FString FAccelByteUtilities::EncodeHMACBase64(const FString& Message, const FString& Key)
{
	const int BYTE_BUFFER_COUNT = 20; // Based on the inline documentation FSHA1::HMACBuffer
	uint8 EncodeOutput[BYTE_BUFFER_COUNT];

	TArray<uint8> MessageBytes = FAccelByteArrayByteFStringConverter::FStringToBytes(Message);
	TArray<uint8> KeyBytes = FAccelByteArrayByteFStringConverter::FStringToBytes(Key);
	FSHA1::HMACBuffer(KeyBytes.GetData(), KeyBytes.Num(), MessageBytes.GetData(), MessageBytes.Num(), EncodeOutput);
	
	TArray<uint8> TArrayEncodedOutput(&EncodeOutput[0], BYTE_BUFFER_COUNT);
	//Base64 to avoid unwanted behaviour
	return FBase64::Encode(TArrayEncodedOutput);
}

FString FAccelByteUtilities::GetOrSetIfDeviceIdNotFound(const FString Default)
{
	static FString StaticDeviceId;

	// Check to see if we already have a valid machine ID to use
	if (StaticDeviceId.IsEmpty() && !FPlatformMisc::GetStoredValue(AccelByteStored(),
		AccelByteStoredSectionIdentifiers(), AccelByteStoredKeyDeviceId(), StaticDeviceId))
	{
		// No valid device ID, use the default
		StaticDeviceId = Default;

		if (!FPlatformMisc::SetStoredValue(AccelByteStored(), AccelByteStoredSectionIdentifiers(),
			AccelByteStoredKeyDeviceId(), StaticDeviceId))
		{
			// Failed to persist the machine ID - use the default
			StaticDeviceId = Default;
		}
	}

	return StaticDeviceId;
}

EAccelByteDevModeDeviceIdMethod FAccelByteUtilities::GetCurrentDeviceIdOverrideMethod()
{
	FString DeviceIdOverrideMethod = FString();
	if (!GConfig->GetString(
		TEXT("AccelByte.Dev"),
		TEXT("DeviceIdOverrideMethod"),
		DeviceIdOverrideMethod,
		GEngineIni)
		|| DeviceIdOverrideMethod.IsEmpty())
	{
		return EAccelByteDevModeDeviceIdMethod::UNSPECIFIED;
	}
	
	return FAccelByteUtilities::GetUEnumValueFromString<EAccelByteDevModeDeviceIdMethod>(DeviceIdOverrideMethod);
}

FString FAccelByteUtilities::GetDeviceId(bool bIsDeviceIdRequireEncode)
{
	FString Output = FString();

	FString PlatformDeviceId = FPlatformMisc::GetDeviceId();
	if (PlatformDeviceId.IsEmpty())
	{
		//Decision to encode at the end of this function
		FString PlainMacAddress = FAccelByteUtilities::GetMacAddress(false);

		if (PlainMacAddress.IsEmpty())
		{
			Output = RandomizeDeviceId();
		}
		else
		{
			Output = PlainMacAddress;
		}
	}
	else //IF Platform-specific DeviceID available
	{
		Output = PlatformDeviceId;
	}

	bool bIsShippingBuild = false;
#if UE_BUILD_SHIPPING
	bIsShippingBuild = true;
#endif
	bool bIsDevMode = !bIsShippingBuild && !IsRunningDedicatedServer();

	if (bIsDevMode)
	{
		Output = GetDevModeDeviceId(Output);
	}

	if (bIsDeviceIdRequireEncode || !bIsDevMode)
	{
		Output = EncodeHMACBase64(Output, FRegistry::Settings.PublisherNamespace);
	}
	return Output;
}

FString FAccelByteUtilities::GetDevModeDeviceId(FString DefaultDeviceId)
{
#pragma region RETRIEVE_OVERRIDE_CONDITION
	bool bUsePersistentDeviceId = false;
	bool bIsRandomizeDeviceId = false;
	TArray<FString> DeviceIdsFromIni = TArray<FString>();
	FString CommandLineDeviceId = FString();
	bool bIsCommandLineOverride = false;
	bool bIsDiscardOverride = false;

	bUsePersistentDeviceId = 
		GConfig->GetBool(
			TEXT("AccelByte.Dev"),
			TEXT("UsePersistentDeviceId"),
			bUsePersistentDeviceId,
			GEngineIni)
		&& bUsePersistentDeviceId;

	bIsRandomizeDeviceId =
		GConfig->GetBool(
			TEXT("AccelByte.Dev"),
			TEXT("RandomizeDeviceId"),
			bIsRandomizeDeviceId,
			GEngineIni)
		&& bIsRandomizeDeviceId;

	GConfig->GetArray(
		TEXT("AccelByte.Dev"),
		TEXT("DeviceId"),
		DeviceIdsFromIni,
		GEngineIni);

	bIsCommandLineOverride = 
		FParse::Value(FCommandLine::Get(), TEXT("deviceid"), CommandLineDeviceId)
		&& !CommandLineDeviceId.IsEmpty();

	bIsDiscardOverride =
		GConfig->GetBool(
			TEXT("AccelByte.Dev"),
			TEXT("DiscardOverride"),
			bIsDiscardOverride,
			GEngineIni)
		&& bIsDiscardOverride;
#pragma endregion 

	//Early return, high priority discard Override configuration
	if (bIsDiscardOverride)
	{
		return DefaultDeviceId;
	}

	//File-based configuration override method
	switch (GetCurrentDeviceIdOverrideMethod())
	{
	case EAccelByteDevModeDeviceIdMethod::COMMANDLINE:
		if (bIsCommandLineOverride)
		{
			return CommandLineDeviceId;
		}
		break;
	case EAccelByteDevModeDeviceIdMethod::PICK_RANDOM:
		if (DeviceIdsFromIni.Num() > 0)
		{
			int TheChosenIndex = FMath::Rand() % DeviceIdsFromIni.Num();
			return DeviceIdsFromIni[TheChosenIndex];
		}
		break;
	case EAccelByteDevModeDeviceIdMethod::RANDOMIZE:
		return RandomizeDeviceId();
	case EAccelByteDevModeDeviceIdMethod::PERSISTENT:
		return GetOrSetIfDeviceIdNotFound(DefaultDeviceId);
	case EAccelByteDevModeDeviceIdMethod::UNSPECIFIED:
	default:
		break;
	}

#pragma region IF_OVERRIDE_METHOD_UNSPECIFIED
	if (bIsCommandLineOverride)
	{
		return CommandLineDeviceId;
	}
	// Failed to get the commandline, then look for randomized
	if (bIsRandomizeDeviceId)
	{
		if (DeviceIdsFromIni.Num() == 0)
		{
			return RandomizeDeviceId();
		}

		int TheChosenIndex = FMath::Rand() % DeviceIdsFromIni.Num();
		return DeviceIdsFromIni[TheChosenIndex];
	}

	// Persist and use whatever deviceId we have.
	if (bUsePersistentDeviceId)
	{
		return GetOrSetIfDeviceIdNotFound(DefaultDeviceId);
	}
#pragma endregion

	return DefaultDeviceId;
}

FString FAccelByteUtilities::GetMacAddress(bool bEncoded)
{
	FString MacAddressString = TEXT("");
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	for (TArray<uint8>::TConstIterator it(MacAddr); it; ++it)
	{
		MacAddressString += FString::Printf(TEXT("%02x"), *it);
	}

	return (bEncoded && !MacAddressString.IsEmpty()) ? EncodeHMACBase64(MacAddressString, FRegistry::Settings.PublisherNamespace) : MacAddressString;
}

FString FAccelByteUtilities::GetPlatformName()
{
	FString PlatformName = TEXT("");
	PlatformName = UGameplayStatics::GetPlatformName();
	return PlatformName;
}

FString FAccelByteUtilities::XOR(const FString& Input, const FString& Key)
{
	TArray<uint8> AccumulateResult;

	TArray<uint8> ByteArray;
	ByteArray.AddUninitialized(Input.Len());
	StringToBytes(Input, ByteArray.GetData(), Input.Len());

	TArray<uint8> ByteArrayKey;
	ByteArrayKey.AddUninitialized(Key.Len());
	StringToBytes(Key, ByteArrayKey.GetData(), Key.Len());

	for (int i = 0; i < Input.Len(); i++)
	{
		AccumulateResult.Add((TCHAR)(ByteArray[i] ^ ByteArrayKey[i % ByteArrayKey.Num()]));
	}

	FString Output = BytesToString(AccumulateResult.GetData(), AccumulateResult.Num());
	return Output;
}

FString FAccelByteUtilities::GetAuthTrustId()
{ 
	FString AuthTrustId{};
	FPlatformMisc::GetStoredValue(AccelByteStored(), AccelByteStoredSectionIAM(), AccelByteStoredKeyAuthTrustId(), AuthTrustId);
	return AuthTrustId;
}

bool FAccelByteUtilities::GetValueFromCommandLineSwitch(const FString& Key, FString& Value)
{
	const auto CommandParams = FCommandLine::Get();
	TArray<FString> Tokens;
	TArray<FString> Switches;
	FCommandLine::Parse(CommandParams, Tokens, Switches);
	for (auto Param : Switches)
	{
		if (Param.Contains(Key))
		{
			TArray<FString> ArraySplit;
			Param.ParseIntoArray(ArraySplit, TEXT("="), 1);
			if (ArraySplit.Num() == 2 && ArraySplit[1].IsEmpty() == false)
			{
				Value = ArraySplit[1];
				return true;
			}
		}
	}
	return false;
}

bool FAccelByteUtilities::GetValueFromCommandLineSwitch(const FString& Key, int& Value)
{
	FString TempResult = "";
	bool bIsSuccess = GetValueFromCommandLineSwitch(Key, TempResult);
	if (bIsSuccess == false)
	{
		return false;
	}

	Value = FCString::Atoi(*TempResult);
	return true;
}

bool FAccelByteUtilities::GetValueFromCommandLineSwitch(const FString& Key, bool& Value)
{
	FString TempResult = "";
	bool bIsSuccess = GetValueFromCommandLineSwitch(Key, TempResult);
	if (bIsSuccess == false)
	{
		return false;
	}

	Value = TempResult.ToBool();
	return true;
}

void FAccelByteUtilities::SetAuthTrustId(const FString& AuthTrustId)
{ 
	FPlatformMisc::SetStoredValue(AccelByteStored(), AccelByteStoredSectionIAM(), AccelByteStoredKeyAuthTrustId(), AuthTrustId);
}

void FAccelByteNetUtilities::GetPublicIP(const THandler<FAccelByteModelsPubIp>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("ipify support is deprecated. Please use ISocketSubsystem to get public IP address."));
	
	FString Url = FString::Printf(TEXT("https://api.ipify.org?format=json"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void FAccelByteNetUtilities::DownloadFrom(const FString& Url, const FHttpRequestProgressDelegate& OnProgress, const THandler<TArray<uint8>>& OnDownloaded, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Verb = TEXT("GET");
	FString Accept = TEXT("application/octet-stream");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->OnRequestProgress() = OnProgress;
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnDownloaded, OnError), FPlatformTime::Seconds());
}

void FAccelByteNetUtilities::UploadTo(const FString& Url, const TArray<uint8>& DataUpload, const FHttpRequestProgressDelegate& OnProgress,
	const AccelByte::FVoidHandler& OnSuccess, const FErrorHandler& OnError, FString ContentType)
{
	FReport::Log(FString(__FUNCTION__));

	FString Verb = TEXT("PUT");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->OnRequestProgress() = OnProgress;
	Request->SetHeader(TEXT("Content-Type"), *ContentType);
	Request->SetContent(DataUpload);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	
}

FString FAccelByteUtilities::ConvertItemSortByToString(EAccelByteItemListSortBy const& SortBy)
{ 
	switch (SortBy)
	{
	case EAccelByteItemListSortBy::NAME:
		return FGenericPlatformHttp::UrlEncode(TEXT("name"));
	case EAccelByteItemListSortBy::NAME_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("name:desc"));
	case EAccelByteItemListSortBy::CREATEDAT:
		return FGenericPlatformHttp::UrlEncode(TEXT("createdAt"));
	case EAccelByteItemListSortBy::CREATEDAT_ASC:
		return FGenericPlatformHttp::UrlEncode(TEXT("createdAt:asc"));
	case EAccelByteItemListSortBy::CREATEDAT_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("createdAt:desc"));
	case EAccelByteItemListSortBy::UPDATEDAT:
		return FGenericPlatformHttp::UrlEncode(TEXT("updatedAt"));
	case EAccelByteItemListSortBy::UPDATEDAT_ASC:
		return FGenericPlatformHttp::UrlEncode(TEXT("updatedAt:asc"));
	case EAccelByteItemListSortBy::UPDATEDAT_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("updatedAt:desc"));
	case EAccelByteItemListSortBy::DISPLAYORDER:
		return FGenericPlatformHttp::UrlEncode(TEXT("displayOrder"));
	case EAccelByteItemListSortBy::DISPLAYORDER_ASC:
		return FGenericPlatformHttp::UrlEncode(TEXT("displayOrder:asc"));
	case EAccelByteItemListSortBy::DISPLAYORDER_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("displayOrder:desc"));
	}
	return TEXT("");
}
