// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteUtilities.h"
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteIdValidator.h"
#include "Models/AccelByteUserModels.h"
#include "Misc/CommandLine.h"
#include "Misc/CString.h"
#include <memory>
#include "Kismet/GameplayStatics.h"
#include "Misc/SecureHash.h"
#include "Misc/Base64.h"
#include "Interfaces/IPluginManager.h"

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
	/** @brief Creates our OpenSSL Message Digest Context on construction */
	FScopedEVPMDContext() :
		Context(EVP_MD_CTX_create())
	{
	}

	/** @brief Disable copying/assigning */
	FScopedEVPMDContext(FScopedEVPMDContext&) = delete;
	FScopedEVPMDContext& operator=(FScopedEVPMDContext&) = delete;

	/** @brief Free our OpenSSL Message Digest Context */
	~FScopedEVPMDContext()
	{
		EVP_MD_CTX_destroy(Context);
	}

	/** @brief Get our OpenSSL Message Digest Context */
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


FRsaPublicKey::FRsaPublicKey(FString ModulusB64Url
	, FString ExponentB64Url) 
	: ModulusB64Url{MoveTemp(ModulusB64Url)}
	, ExponentB64Url{MoveTemp(ExponentB64Url)}
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

TSharedPtr<FJsonObject> ExtractJson(FString const& JsonB64Url
	, int32 Start
	, int32 Count)
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
	: JwtString{MoveTemp(JwtString)}
	, HeaderEnd{GetHeaderEnd(this->JwtString)}
	, PayloadEnd{GetPayloadEnd(this->JwtString)}
	, HeaderJsonPtr{ExtractJson(this->JwtString, 0, HeaderEnd)}
	, PayloadJsonPtr{ExtractJson(this->JwtString, HeaderEnd + 1, PayloadEnd - HeaderEnd - 1)}
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
	return HeaderEnd != INDEX_NONE
		&& PayloadEnd != INDEX_NONE
		&& HeaderEnd != PayloadEnd
		&& HeaderJsonPtr != nullptr
		&& PayloadJsonPtr != nullptr
		&& JwtString.Len() - PayloadEnd - 1 == RS256_SIGNATURE_LENGTH;
}

bool FAccelByteUtilities::IsRunningDevMode()
{
#if UE_BUILD_SHIPPING
	return false;
#endif

	return true;
}

FString FAccelByteUtilities::AccelByteStorageFile()
{
	FString StorageFilename = TEXT("AccelByteData");

	if (IsRunningDevMode())
	{
		FString TempFilename;  
		if (GConfig->GetString(TEXT("AccelByte.Dev")
			, TEXT("BinaryFilename")
			, TempFilename
			, GEngineIni))
		{
			StorageFilename = TempFilename;
		}
	}

	return StorageFilename;
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
	case EAccelBytePlatformType::PS4Web:
		return "ps4web";
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

FString FAccelByteUtilities::CreateQueryParams(TMap<FString, FString> Map
	, FString SuffixChar)
{
	FString Query = TEXT("");
	for (auto kvp : Map)
	{
		FString Param = kvp.Key;
		FString Value = kvp.Value;
		if (!Param.IsEmpty() && !Value.IsEmpty())
		{			
			Query.Append(Query.IsEmpty() ? SuffixChar : TEXT("&"));
			Query.Append(FString::Printf(TEXT("%s=%s"), *Param, *Value));
		}	
	}
	return Query;
}

FString FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(TArray<FString> const& Array
	, FString const& Delimiter)
{
	FString QueryParamValue = TEXT("");
	if (Array.Num() > 0)
	{ 
		for (int i = 0; i < Array.Num(); i++)
		{
			const FString& UrlEncodedStringValue = FGenericPlatformHttp::UrlEncode(Array[i]);
			FString ItemAppended = FString::Printf(TEXT("%s%s"), *Delimiter, *UrlEncodedStringValue);
			QueryParamValue.Append( i == 0 ? Array[i] : ItemAppended);
		}
	}
	return QueryParamValue;	
}

void FAccelByteUtilities::RemoveEmptyFieldsFromJson(TSharedPtr<FJsonObject> const& JsonObjectPtr
	, uint8 const Flags
	, TArray<FString> const& ExcludedFieldNames)
{
// Macro to make field removal flag bit logic more readable
#define HAS_FIELD_REMOVAL_FLAG(FlagName) ((Flags & FieldRemovalFlag##FlagName) != 0)

	TArray<FString> FieldsToRemove;
	TMap<FString, TSharedPtr<FJsonValue>> FieldsToReplace;

	// Remove empty field so it doesn't get updated in BE
	for(auto& KeyValuePair : JsonObjectPtr->Values)
	{
		if(!KeyValuePair.Value.IsValid() || ExcludedFieldNames.Contains(KeyValuePair.Key))
		{
			continue;
		}

		bool bRemoveField = false;
		switch (KeyValuePair.Value->Type)
		{
		case EJson::Array:
		{
			TArray<TSharedPtr<FJsonValue>> OriginalArray = KeyValuePair.Value->AsArray();
			bRemoveField = HAS_FIELD_REMOVAL_FLAG(Arrays) && OriginalArray.Num() == 0;

			if(bRemoveField || !HAS_FIELD_REMOVAL_FLAG(Nested))
			{
				break;
			}

			// If the array is not empty, we want to look for nested objects which might have empty values inside them
			TArray<TSharedPtr<FJsonValue>> NewArray;
			for(const auto& NestedValue : OriginalArray)
			{
				// Skipping over any array item that is not an object
				if(NestedValue->Type != EJson::Object)
				{
					NewArray.Add(NestedValue);
					continue;
				}

				TSharedPtr<FJsonObject> NestedObject = NestedValue->AsObject();
				RemoveEmptyFieldsFromJson(NestedObject, Flags, ExcludedFieldNames);

				if(HAS_FIELD_REMOVAL_FLAG(Objects) && NestedObject->Values.Num() == 0)
				{
					continue;
				}
				NewArray.Add(MakeShared<FJsonValueObject>(NestedObject));
			}

			if(HAS_FIELD_REMOVAL_FLAG(Arrays) && NewArray.Num() == 0)
			{
				bRemoveField = true;
				break;
			}
			FieldsToReplace.Add(KeyValuePair.Key, MakeShared<FJsonValueArray>(NewArray));

			break;
		}
		case EJson::Object:
		{
			TSharedPtr<FJsonObject> Object = KeyValuePair.Value->AsObject();
			TArray<FString> Keys;
			bRemoveField = HAS_FIELD_REMOVAL_FLAG(Objects) && Object->Values.Num() == 0;

			if(bRemoveField || !HAS_FIELD_REMOVAL_FLAG(Nested))
			{
				break;
			}

			RemoveEmptyFieldsFromJson(Object, Flags, ExcludedFieldNames);
			if(HAS_FIELD_REMOVAL_FLAG(Objects) && Object->Values.Num() == 0)
			{
				bRemoveField = true;
				break;
			}
			FieldsToReplace.Add(KeyValuePair.Key, MakeShared<FJsonValueObject>(Object));

			break;
		}
		case EJson::String:
		{
			const FString Value = KeyValuePair.Value->AsString();

			// Removing string fields if the dates flag is set and the string is equal to an uninitialized datetime (aka 0 ticks)
			bRemoveField =
				(HAS_FIELD_REMOVAL_FLAG(Strings) && Value.IsEmpty()) ||
				(HAS_FIELD_REMOVAL_FLAG(Dates) && Value.Equals(FDateTime(0).ToString()));

			break;
		}
		case EJson::Number:
		{
			bool removeZeroValues = HAS_FIELD_REMOVAL_FLAG(NumbersZeroValues) != 0;
			int32 integerValue = static_cast<int32>(FMath::Floor(KeyValuePair.Value->AsNumber()));

			if (HAS_FIELD_REMOVAL_FLAG(Numbers) && integerValue == TNumericLimits<int32>::Min())
			{
				// Remove fields with a value of TNumericLimits<int32>::Min() if FieldRemovalFlagAll is set
				bRemoveField = true;
			}
			else if (removeZeroValues && integerValue == 0)
			{
				// Remove fields with a value of 0 if FieldRemovalFlagNumbersZeroValues is set
				bRemoveField = true;
			}
			break;
		}
		case EJson::Null:
		{
			bRemoveField = HAS_FIELD_REMOVAL_FLAG(Null);
			break;
		}

		// Redundant technically, but for readability explicitly says that we don't care about the other members of EJson
		default: break;
		}

		if(bRemoveField)
		{
			FieldsToRemove.Add(KeyValuePair.Key);
		}
	}

	for(const FString& Key : FieldsToRemove)
	{
		JsonObjectPtr->RemoveField(Key);
	}

	for(const auto& KeyValuePair : FieldsToReplace)
	{
		JsonObjectPtr->SetField(KeyValuePair.Key, KeyValuePair.Value);
	}

#undef HAS_FIELD_REMOVAL_FLAG
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

FString FAccelByteUtilities::EncodeHMACBase64(FString const& Message
	, FString const& Key)
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

FString FAccelByteUtilities::GetOrSetIfDeviceIdNotFound(FString const& Default)
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
	
	return GetUEnumValueFromString<EAccelByteDevModeDeviceIdMethod>(DeviceIdOverrideMethod);
}

FString FAccelByteUtilities::GetDeviceId(bool bIsDeviceIdRequireEncode)
{
	FString Output = FString();

	FString PlatformDeviceId = FPlatformMisc::GetDeviceId();
	if (PlatformDeviceId.IsEmpty())
	{
		bool bIsCached = false;
		IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->GetItem(AccelByteStoredKeyDeviceId()
			, THandler<TPair<FString, FString>>::CreateLambda(
				[&Output, &bIsCached](TPair<FString, FString> SavedDeviceId)
				{
					if (SavedDeviceId.Key.IsEmpty() || SavedDeviceId.Value.IsEmpty())
					{
						return;
					}
					Output = SavedDeviceId.Value;
					bIsCached = true;

				})
			, AccelByteStorageFile());
		if (!bIsCached)
		{
			FString PlainMacAddress = GetMacAddress(false);
			if (PlainMacAddress.IsEmpty())
			{
				Output = FGuid::NewGuid().ToString();
			}
			else
			{
				Output = PlainMacAddress;
			}
			IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->SaveItem(AccelByteStoredKeyDeviceId()
				, Output
				, THandler<bool>::CreateLambda([](bool bIsSuccess){})
				, AccelByteStorageFile());
		}
	}
	else //IF Platform-specific DeviceID available
	{
		Output = PlatformDeviceId;
	}
	
	bool bIsClientDevMode = IsRunningDevMode() && !IsRunningDedicatedServer();

	if (bIsClientDevMode)
	{
		Output = GetDevModeDeviceId(Output);
	}

	if (bIsDeviceIdRequireEncode || !bIsClientDevMode)
	{
		Output = EncodeHMACBase64(Output, FRegistry::Settings.PublisherNamespace);
	}
	return Output;
}

FString FAccelByteUtilities::GetDevModeDeviceId(FString const& DefaultDeviceId)
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
		*AccelByteStoredKeyDeviceId(),
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

FString FAccelByteUtilities::XOR(FString const& Input
	, FString const& Key)
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

bool FAccelByteUtilities::GetValueFromCommandLineSwitch(FString const& Key
	, FString& Value)
{
	const auto CommandParams = FCommandLine::Get();
	TArray<FString> Tokens;
	TArray<FString> Switches;
	FCommandLine::Parse(CommandParams, Tokens, Switches);
	for (auto Param : Switches)
	{
		if (Param.StartsWith(Key))
		{
			TArray<FString> ArraySplit;
			Param.ParseIntoArray(ArraySplit, TEXT("="), true);
			if (ArraySplit.Num() == 2 && ArraySplit[0] == Key)
			{
				Value = ArraySplit[1];
				return true;
			}
		}
	}
	return false;
}

bool FAccelByteUtilities::GetValueFromCommandLineSwitch(FString const& Key
	, int & Value)
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

bool FAccelByteUtilities::GetValueFromCommandLineSwitch(FString const& Key
	, bool & Value)
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

void FAccelByteUtilities::SetAuthTrustId(FString const& AuthTrustId)
{ 
	FPlatformMisc::SetStoredValue(AccelByteStored(), AccelByteStoredSectionIAM(), AccelByteStoredKeyAuthTrustId(), AuthTrustId);
}

const FString AuthorizationCodeArgument = TEXT("--AB_AUTH_CODE=");

FString FAccelByteUtilities::GetAuthorizationCode()
{
	FString AuthorizationCode{};
	FParse::Value(FCommandLine::Get(), *AuthorizationCodeArgument, AuthorizationCode);
	if (AuthorizationCode.IsEmpty())
	{
		AuthorizationCode = Environment::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), 1000);
	}
	return AuthorizationCode;
}

FString FAccelByteUtilities::GetFlightId()
{
	static FString FlightId{};

	if (FlightId.IsEmpty())
	{
		FlightId = FGuid::NewGuid().ToString().ToLower();
	}
	return FlightId;
}

void FAccelByteNetUtilities::GetPublicIP(THandler<FAccelByteModelsPubIp> const& OnSuccess
	, FErrorHandler const& OnError)
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

void FAccelByteNetUtilities::DownloadFrom(FString const& Url
	, FHttpRequestProgressDelegate const& OnProgress
	, THandler<TArray<uint8>> const& OnDownloaded
	, FErrorHandler const& OnError)
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

void FAccelByteNetUtilities::UploadTo(FString const& Url
	, TArray<uint8> const& DataUpload
	, FHttpRequestProgressDelegate const& OnProgress
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& ContentType)
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

#define REGEX_BASE_URL_WITH_DOMAIN "https?:\\/\\/(?:www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,128}\\.[a-zA-Z0-9()]{1,6}"
#define REGEX_BASE_URL_WITHOUT_DOMAIN "(?:(https?:\\/\\/)?((?:[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)|localhost))"
#define REGEX_OPTIONAL_PORT "(:(?:[1-9]{1}[0-9]{1,4}|[0-9]{1}))?"
#define REGEX_PATH_AND_PARAM "(\\/?|((\\/)[^\\/\\\\]{1,512}(?:\\7[^\\/\\\\]{1,512})*(\\/?)))"
#define REGEX_URL "^(" REGEX_BASE_URL_WITH_DOMAIN "|" REGEX_BASE_URL_WITHOUT_DOMAIN ")" REGEX_OPTIONAL_PORT REGEX_PATH_AND_PARAM "$"

bool FAccelByteNetUtilities::IsValidUrl(FString const& Url)
{
	FRegexPattern UrlRegex(TEXT(REGEX_URL));
	FRegexMatcher Matcher(UrlRegex, Url);
	return Matcher.FindNext();
}

FString FAccelByteUtilities::ConvertItemSortByToString(EAccelByteItemListSortBy const& SortBy)
{ 
	switch (SortBy)
	{
	case EAccelByteItemListSortBy::NAME: 
		return TEXT("name");
	case EAccelByteItemListSortBy::NAME_DESC:
		return TEXT("name:desc");
	case EAccelByteItemListSortBy::CREATEDAT:
		return TEXT("createdAt");
	case EAccelByteItemListSortBy::CREATEDAT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteItemListSortBy::CREATEDAT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteItemListSortBy::UPDATEDAT:
		return TEXT("updatedAt");
	case EAccelByteItemListSortBy::UPDATEDAT_ASC:
		return TEXT("updatedAt:asc");
	case EAccelByteItemListSortBy::UPDATEDAT_DESC:
		return TEXT("updatedAt:desc");
	case EAccelByteItemListSortBy::DISPLAYORDER:
		return TEXT("displayOrder");
	case EAccelByteItemListSortBy::DISPLAYORDER_ASC:
		return TEXT("displayOrder:asc");
	case EAccelByteItemListSortBy::DISPLAYORDER_DESC:
		return TEXT("displayOrder:desc");
	}
	return TEXT("");
}

bool FAccelByteUtilities::ReplaceDecimalSeparator(FString& NumberStr, const TCHAR* From, const TCHAR* To)
{
	if (FAccelByteUtilities::IsNumericString(NumberStr))
	{
		NumberStr = NumberStr.Replace(From, To);
		return true;
	}
	return false;
}

bool FAccelByteUtilities::IsNumericString(FString const& String)
{
	if (String.IsEmpty())
	{
		return false;
	}

	const TCHAR* Str = String.GetCharArray().GetData();
	if (*Str == '-' || *Str == '+')
	{
		Str++;
	}

	bool bHasDecimalSeparator = false;
	TCHAR DecimalSeparator = '.';
	if (IsLanguageUseCommaDecimalSeparator())
	{
		DecimalSeparator = ',';
	}

	while (*Str != '\0')
	{
		if (*Str == DecimalSeparator)
		{
			if (bHasDecimalSeparator)
			{
				return false;
			}
			bHasDecimalSeparator = true;
		}
		else if (!FChar::IsDigit(*Str))
		{
			return false;
		}

		++Str;
	}

	return true;
}

bool FAccelByteUtilities::IsLanguageUseCommaDecimalSeparator()
{
	double CheckValue = 0.1f;
	FString StringValue = FString::Printf(TEXT("%g"), CheckValue);
	if (StringValue.Contains(TEXT(",")))
	{
		return true;
	}
	return false;
}

bool FAccelByteUtilities::IsAccelByteIDValid(FString const& AccelByteId, EAccelByteIdHypensRule HypenRule)
{
	return FAccelByteIdValidator::IsAccelByteIdValid(AccelByteId);
}

void FAccelByteUtilities::AppendModulesVersionToMap(TMap<FString, FString>& Headers)
{
	// Append AccelByteUe4SdkPlugin Version 
	const FString AccelByteUe4SdkPluginName = TEXT("AccelByteUe4Sdk");
	TSharedPtr<IPlugin> const AccelByteUe4SdkPlugin = IPluginManager::Get().FindPlugin(AccelByteUe4SdkPluginName);
	if (AccelByteUe4SdkPlugin.IsValid())
	{
		const FPluginDescriptor& Descriptor = AccelByteUe4SdkPlugin->GetDescriptor();
		const FString AccelByteUe4SdkPluginVersion = Descriptor.VersionName;
		Headers.Add(TEXT("AccelByte-SDK-Version"), AccelByteUe4SdkPluginVersion);
	}

	// Append OnlineSubsystemAccelBytePlugin Version
	const FString OnlineSubsystemAccelBytePluginName = TEXT("OnlineSubsystemAccelByte");
	TSharedPtr<IPlugin> const OnlineSubsystemAccelBytePlugin = IPluginManager::Get().FindPlugin(OnlineSubsystemAccelBytePluginName);
	if (OnlineSubsystemAccelBytePlugin.IsValid())
	{
		const FPluginDescriptor& Descriptor = OnlineSubsystemAccelBytePlugin->GetDescriptor();
		const FString OnlineSubsystemAccelBytePluginVersion = Descriptor.VersionName;
		Headers.Add(TEXT("AccelByte-OSS-Version"), OnlineSubsystemAccelBytePluginVersion);
	}

	// Append Game Project Version
	FString ProjectVersion = TEXT("");
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"), ProjectVersion, GGameIni);
	Headers.Add(TEXT("Game-Client-Version"), ProjectVersion);
}

EAccelByteCurrentServerManagementType FAccelByteUtilities::GetCurrentServerManagementType()
{
	//Sort this decision based on priority
	if (!IsRunningDedicatedServer())
	{
		return EAccelByteCurrentServerManagementType::NOT_A_SERVER;
	}

	if (!FRegistry::ServerSettings.DSId.IsEmpty())
	{
		return EAccelByteCurrentServerManagementType::ONLINE_AMS;
	}

	FString PodNameValue = FPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"));
	if (!PodNameValue.IsEmpty())
	{
		return EAccelByteCurrentServerManagementType::ONLINE_ARMADA;
	}
	else
	{
		return EAccelByteCurrentServerManagementType::LOCAL_SERVER;
	}
}

// Helper function to get content type based on EAccelByteFileType enum
FString FAccelByteUtilities::GetContentType(EAccelByteFileType const& FileType)
{
	switch (FileType)
	{
	case EAccelByteFileType::JPEG:
	case EAccelByteFileType::JPG:
		return "image/jpeg";
	case EAccelByteFileType::PNG:
		return "image/png";
	case EAccelByteFileType::BMP:
		return "image/bmp";
	case EAccelByteFileType::GIF:
		return "image/gif";
	case EAccelByteFileType::MP3:
		return "audio/mpeg";
	case EAccelByteFileType::BIN:
		return "application/octet-stream";
	case EAccelByteFileType::WEBP:
		return "image/webp";
	default:
		return ""; // Handle NONE or other cases accordingly
	}
}