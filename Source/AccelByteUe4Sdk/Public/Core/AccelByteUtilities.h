// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteIdValidator.h"
#include "JsonObjectConverter.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Math/NumericLimits.h"
#include "Models/AccelByteChallengeModels.h"
#include "AccelByteUtilities.generated.h"

// AccelByte IDs have a max length of 32, as they are UUIDs that are striped of their hyphens
#define ACCELBYTE_ID_LENGTH 32
#define ACCELBYTE_ID_LENGTH_WITH_HYPENS 36

using AccelByte::THandler;
using AccelByte::FErrorHandler;

static constexpr int32 UserIdsURLLimit = 40;

static constexpr int32 MaximumQueryLimit = 100;

enum class EAccelBytePlatformType : uint8;
enum class EAccelByteDevModeDeviceIdMethod : uint8;
enum class EJwtResult
{
	Ok,
	MalformedJwt,
	SignatureMismatch,
	AlgorithmMismatch,
	MalformedPublicKey
};

UENUM(BlueprintType)
enum class EAccelByteDevModeDeviceIdMethod : uint8
{
	UNSPECIFIED = 0,
	COMMANDLINE = 1,
	PICK_RANDOM = 2,
	RANDOMIZE = 3,
	PERSISTENT = 4
};

/**
 * @brief RSA public key with parameters encoded in Base64Url. Only supports 2048 bits modulus and 24 bits exponent.
 */
class ACCELBYTEUE4SDK_API FRsaPublicKey
{
public:
	
	/**
	 * @brief Construct FRsaPublicKey with modulus and exponent.
	 * 
	 * @param ModulusB64Url RSA modulus (n) in Base64URL format.
	 * @param ExponentB64Url RSA exponent (e) in Base64URL format.
	 */
	FRsaPublicKey(FString ModulusB64Url, FString ExponentB64Url); 

	/**
	 * @brief Check if this RSA public key is valid.
	 *
	 * @return Valid RSA Public key or not.
	 */
	bool IsValid() const;

	
	/**
	 * @brief Convert RSA public key to armored PEM format.
	 * 
	 * @return PEM format armored with "-----BEGIN PUBLIC KEY-----" and "-----END PUBLIC KEY-----"
	 */
	FString ToPem() const;

private:
	FString const ModulusB64Url;
	FString const ExponentB64Url;
};


/**
 * @brief Provide access to verify JWT and extract its content.
 */
class ACCELBYTEUE4SDK_API FJwt
{
public:
	/**
	 * @brief Construct FJwt from JWT string.
	 * 
	 * @param JwtString JWT encoded as dot separated Base64Url string.
	 */
	explicit FJwt(FString JwtString);

	/**
	 * @brief Verify this JWT using RSA public key.
	 * 
	 * @param Key RSA public key.
	 * 
	 * @return EJwtResult::Ok if signature match.
	 */
	EJwtResult VerifyWith(FRsaPublicKey Key) const;

	/**
	 * @brief Get header content from JWT. Content could be any valid JSON having at least "alg" field.
	 * 
	 * @return JWT header.
	 */
	TSharedPtr<FJsonObject> const& Header() const;

	/**
	 * @brief Get payload content from JWT. Content could be any valid JSON.
	 * 
	 * @return JWT payload.
	 */
	TSharedPtr<FJsonObject> const& Payload() const;

	/**
	 * @brief Check if this JWT format is correct and both and payload are valid JSON encoded as Base64URL.
	 * 
	 * @return true if  this JWT is valid, and
	 * @return false if otherwise.
	 */
	bool IsValid() const;

private:
	FString const JwtString;
	int32 const HeaderEnd;
	int32 const PayloadEnd;
	TSharedPtr<FJsonObject> const HeaderJsonPtr;
	TSharedPtr<FJsonObject> const PayloadJsonPtr;
};

/**
 * Enum used to determine whether the server management from this instance
 */
enum class EAccelByteCurrentServerManagementType
{
	NOT_A_SERVER,
	LOCAL_SERVER,
	ONLINE_ARMADA,
	ONLINE_AMS
};

class ACCELBYTEUE4SDK_API FAccelByteUtilities
{
public:	
	static constexpr uint8 FieldRemovalFlagObjects = 1 << 0;
	static constexpr uint8 FieldRemovalFlagArrays  = 1 << 1;
	static constexpr uint8 FieldRemovalFlagStrings = 1 << 2;
	static constexpr uint8 FieldRemovalFlagDates   = 1 << 3;
	static constexpr uint8 FieldRemovalFlagNumbers = 1 << 4;
	static constexpr uint8 FieldRemovalFlagNull    = 1 << 5;
	static constexpr uint8 FieldRemovalFlagNested  = 1 << 6;
	static constexpr uint8 FieldRemovalFlagNumbersZeroValues = 1 << 7;
	static constexpr uint8 FieldRemovalFlagAll     = 0xFF & ~FieldRemovalFlagNumbersZeroValues;

	
	static bool IsRunningDevMode();
	
	//Obsolete the general purpose file cache file
	static FString AccelByteStorageFile();
	//Filename to cache CriticalEvent telemetries
	static FString GetCacheFilenameTelemetry();
	//Filename to cache cache (DeviceID & refresh token)
	static FString GetCacheFilenameGeneralPurpose();

	template<typename CharType = TCHAR, template<typename> class PrintPolicy = TPrettyJsonPrintPolicy, typename InStructType>
	static bool TArrayUStructToJsonString(TArray<InStructType> const& InArray
		, FString& OutJsonString
		, int64 CheckFlags = 0
		, int64 SkipFlags = 0
		, int32 Indent = 0)
	{
		const UStruct* StructDefinition = InStructType::StaticStruct();
		TArray< TSharedPtr<FJsonValue> > JsonArray;

		for (auto& Item : InArray)
		{
			const void* Struct = &Item;
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			if (FJsonObjectConverter::UStructToJsonObject(StructDefinition, Struct, JsonObject.ToSharedRef(), CheckFlags, SkipFlags, nullptr))
			{
				TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObject));
				JsonArray.Add(JsonValue);
			}
		}

		TSharedRef<TJsonWriter<CharType, PrintPolicy<CharType>>> JsonWriter = TJsonWriterFactory<CharType, PrintPolicy<CharType>>::Create(&OutJsonString, Indent);
		if (FJsonSerializer::Serialize(JsonArray, JsonWriter))
		{
			JsonWriter->Close();
			return true;
		}
		else
		{
			UE_LOG(LogJson, Warning, TEXT("UStructToFormattedObjectString - Unable to write out json"));
			JsonWriter->Close();
		}

		return false;
	}

	static void RemoveEmptyStrings(TSharedPtr<FJsonObject> Json);

	/**
	 * @brief Convert an UEnum value into a String enum value.
	 *
	 * @param Value UEnum value.
	 * 
	 * @return String value of specified UEnum. 
	 */
	template<typename TEnum>
	static FString GetUEnumValueAsString(TEnum Value)
	{
		FString ValueString = UEnum::GetValueAsString(Value);
		FString Delimiter = TEXT("::");

		TArray<FString> ParsedStrings;

		ValueString.ParseIntoArray(ParsedStrings, *Delimiter);

		return ParsedStrings.Last();
	}

	/**
	 * @brief Convert a String enum value into a UEnum value.
	 *
	 * @param ValueString String enum value.
	 *
	 * @return UEnum value of specified String enum.
	 */
	template<typename TEnum>
	static TEnum GetUEnumValueFromString(FString const& ValueString)
	{
		UEnum* EnumPtr = StaticEnum<TEnum>();
		int64 ValueInt = 0;

		if (EnumPtr != nullptr)
		{
			ValueInt = EnumPtr->GetValueByNameString(ValueString);

			if (ValueInt == INDEX_NONE)
			{
				ValueInt = 0;
			}
		}

		return static_cast<TEnum>(ValueInt);
	}

	/**
	 * @brief Retrieve Platform name from the specified PlatformType.
	 *
	 * @param Platform Platform type enumeration.
	 */
	static FString GetPlatformString(EAccelBytePlatformType Platform);

	static FString GetAuthenticatorString(EAccelByteLoginAuthFactorType Authenticator);
	
	static FString CreateQueryParams(TMap<FString, FString> Map
		, FString SuffixChar = TEXT("?"));
 
	static FString CreateQueryParamValueUrlEncodedFromArray(TArray<FString> const& Array
		, FString const& Delimiter = TEXT(","));
	
	/**
	 * @brief Remove fields which have empty values according to the given flags. Defaults to removing empty objects/arrays, blank
	 * strings, and looping recursively on object and array field values.
	 * Possible Removal flags are as follow:
	 *	 - FieldRemovalFlagObjects: for empty JSON object value
	 *	 - FieldRemovalFlagArrays: for empty JSON array value
	 *	 - FieldRemovalFlagStrings: for empty string value
	 *	 - FieldRemovalFlagDates: for empty date value
	 *	 - FieldRemovalFlagNumbers: for empty number value
	 *	 - FieldRemovalFlagNull: for null value
	 *	 - FieldRemovalFlagNested: for empty nested value
	 *
	 * @param JsonObjectPtr JSON object which will be modified.
	 * @param Flags Removal flags for the empty value using bit flag.
	 * @param ExcludedFieldNames Specified field names that will be excluded in the omission process.
	 */
	static void RemoveEmptyFieldsFromJson(TSharedPtr<FJsonObject> const& JsonObjectPtr
		, uint8 const Flags = FieldRemovalFlagObjects | FieldRemovalFlagStrings | FieldRemovalFlagArrays | FieldRemovalFlagNested
		, TArray<FString> const& ExcludedFieldNames = {});

	/**
	 * @brief Convert array of UStructs into JSON array string.
	 *
	 * @param Objects List of UStruct objects.
	 * @param OutString Output string.
	 */
	template<typename ObjectType> 
	static bool UStructArrayToJsonObjectString(TArray<ObjectType> const& Objects
		, FString& OutString)
	{
		OutString.Append(TEXT("["));	
		FString JsonArrayString = TEXT("");
		for (auto& Item : Objects)
		{
			FString Delimiter =	JsonArrayString.IsEmpty() ? TEXT("") : TEXT(",");
			JsonArrayString.Append(Delimiter);
			FString JsonObjectString = TEXT("");
			if (!FJsonObjectConverter::UStructToJsonObjectString(Item, JsonObjectString))
			{
				return false;
			}
			JsonArrayString.Append(JsonObjectString); 
		}
		OutString.Append(JsonArrayString);
		OutString.Append(TEXT("]")); 
		return true;
	}

	/**
	 * @brief Try to get the DeviceID from current Platform, if not found from the current Platform then 
	 * the first fallback option will get the DeviceID from a cached file and if not found in the cached file then 
	 * the second fallback option will try to get the MAC Address value and if MAC Address not found then
	 * the last fallback option is generating random UUID and then cached in a file.
	 *
	 * NOTE:
	 * DeviceID for shipping build and Dedicated Server are always encoded, and other thant those it can use development
	 * mode DeviceID that is configurable from the .ini file.
	 *
	 * @param bIsDeviceIdRequireEncode Flag to encode DeviceID. (default: true)
	 * 
	 * @return String of DeviceID.
	 */
	static FString GetDeviceId(bool bIsDeviceIdRequireEncode = true);

	/**
	 * @brief Encode HMAC the message using built in function from UnrealEngine and then Base64 the result.
	 * 
	 * @param Message Specified text.
	 * @param Key HMAC Buffer Key.
	 * 
	 * @return Encoded string result.
	 */
	static FString EncodeHMACBase64(FString const& Message
		, FString const& Key);

	/**
	 * @brief Obtain an HMAC Buffer encoded MAC Address, using PublisherNamespace as key.
	 *
	 * @param bEncoded Flag to encode the return value. (default: true)
	 * @return Either plain MAC Address or encoded one.
	 */
	static FString GetMacAddress(bool bEncoded = true);

	/**
	 * @brief Get current Platform name.
	 *
	 * @return Current Platform name.
	 */
	static FString GetPlatformName();

	/**
	 * @brief Execute XOR operation between two string values.
	 *
	 * @param Input Specified input string.
	 * @param Key Specified key string.
	 * 
	 * @return String result from XOR operation.
	 */
	static FString XOR(FString const& Input
		, FString const& Key);

	/**
	 * @brief Retrieve AuthTrustId that was cached in a filed.
	 *
	 * @return AuthTrustId string.
	 */
	static FString GetAuthTrustId();

	/**
	 * @brief Store AuthTrustId value to a file.
	 *
	 * @param AuthTrustId AuthTrustId.
	 */
	static void SetAuthTrustId(FString const& AuthTrustId);

	/**
	 * @brief Retrieve Authorization Code provided by AccelByte Launcher. 
	 *
	 * @return Authorization code either from command line argument or environment variable.
	 */
	static FString GetAuthorizationCode();

	/**
	 * @brief Retrieve an information if get authorization code is using exchange method provided by AccelByte Launcher. 
	 *
	 * @return Whether the current exchange method using exchange code or authorization code from command line argument.
	 */
	static bool IsUsingExchangeCode();

	/**
	 * @brief Get game client flight id.
	 *
	 * @return Flight id of game client.
	 */
	static FString GetFlightId();

	/**
	 * @brief To be called from game server to decide the type of the game server management.
	 */
	static EAccelByteCurrentServerManagementType GetCurrentServerManagementType();

	/**
	 * @brief Get the key to DeviceID cache entry
	 */
	static FString AccelByteStoredKeyDeviceId() { return FString(TEXT("DeviceId")); }

	/**
	 * @brief Parse command line to obtain an argument that:
	 *  * intialized by dash (-)
	 *  * contain a value that following equal sign (=)
	 *  Assuming the switch is case insesitive
	 *  Example: -serverip=127.0.0.1
	 *     key or switch is the serverip
	 *     value is 127.0.0.1
	 *     
	 *  @param Key The key of switch argument.
	 *  @param Value The operation result will be returned through this parameter.
	 * 
	 *  @return True if value from the second parameter is returned and
	 *  @return False if no value/args can be found
	 */
	static bool GetValueFromCommandLineSwitch(const FString& Key, FString& Value);
	static bool GetValueFromCommandLineSwitch(const FString& Key, int& Value);
	static bool GetValueFromCommandLineSwitch(const FString& Key, bool& Value);
	static bool GetAccelByteConfigFromCommandLineSwitch(const FString& Key, FString& Value);
	static bool GetAccelByteConfigFromCommandLineSwitch(const FString& Key, int& Value);
	static bool GetAccelByteConfigFromCommandLineSwitch(const FString& Key, bool& Value);
	static bool LoadABConfigFallback(const FString& Section, const FString& Key, FString& Value, const FString& DefaultSection = TEXT(""));
	static bool LoadABConfigFallback(const FString& Section, const FString& Key, bool& Value, const FString& DefaultSection = TEXT(""));
	static bool LoadABConfigFallback(const FString& Section, const FString& Key, int& Value, const FString& DefaultSection = TEXT(""));
	static FString ConvertItemSortByToString(EAccelByteItemListSortBy const& SortBy);
	static FString ConvertChallengeSortByToString(EAccelByteModelsChallengeSortBy const& SortBy);
	static bool ReplaceDecimalSeparator(FString& NumberStr, const TCHAR* From, const TCHAR* To);
	static bool IsNumericString(const FString& String);
	static bool IsLanguageUseCommaDecimalSeparator();
	static bool IsAccelByteIDValid(FString const& AccelByteId, EAccelByteIdHypensRule HypenRule = EAccelByteIdHypensRule::NO_RULE);
	static void AppendModulesVersionToMap(TMap<FString, FString>& Headers);
	static FString GetContentType(EAccelByteFileType const& FileType);
	static const FString GenerateHashString(const FString& Message);
	static const FString GenerateTOTP(const FString& SecretKey, int CodeLength = 6, int TimeStep = 30);
	static bool ValidateTOTP(const FString& ServerSecretKey, const FString& TOTP, const FString& UserId);
	static bool IsValidEmail(const FString& Email);

	static FDateTime GetCurrentServerTime();

	/**
	 * Split an array into multiple arrays with a maximum element number.
	 * @param InArray Array to split.
	 * @param Num Number of maximum elements per array.
	 * @param OutArrays Result with array split with maximum element number.
	 */
	static bool SplitArraysToNum(const TArray<FString>& InArray, const int32 Num, TArray<TArray<FString>>& OutArrays);

	/**
	 * @brief Convert an AccelByte general sort enum into a string value.
	 *
	 * @param SortBy The AccelByte enum value to be converted.
	 *
	 * @return The string representation of the given enum value.
	 */
	static FString ConvertAccelByteGeneralSortByToString(EAccelByteGeneralSortBy SortBy);

private:
	static bool FindAccelByteKeyFromTokens(const FString& AccelByteKey, FString& Value);
	static FString GenerateTOTP(int64 CurrentTime, const FString& SecretKey, int32 CodeLength, int32 TimeStep);
	static TArray<FString> GenerateAcceptableTOTP(const FString& ServerSecretKey, const FString& UserId);

//To allow override for testing using mock class
protected:
#pragma region DEVICE_ID
	/**
	 * @brief Create a randomized 30 digit alpha-numeric DeviceID.
	 *
	 * @return Random string of DeviceID. 
	 */
	static FString RandomizeDeviceId(int64 Seed = FDateTime::UtcNow().ToUnixTimestamp());

	/**
	 * @brief Try to obtain DeviceID from a cached value if not found using the Default parameter and also store it into the cache
	 *
	 * @param Default 
	 */
	static FString GetOrSetIfDeviceIdNotFound(FString const& Default = "");

	/**
	 * @brief Get the development mode DeviceID override method from "DefaultEngine.ini"
	 *  	Section:	[AccelByte.Dev]
	 *  	Key:		DeviceIdOverrideMethod=(STRING)
	 *
	 * @return DeviceID override method.
	 */
	static EAccelByteDevModeDeviceIdMethod GetCurrentDeviceIdOverrideMethod();

	/**
	 * @brief Obtain DeviceID for development mode
	 * GAME CLIENT NON SHIPPING BUILD ONLY!
	 * Can be controlled using either command-line args or configuration file.
	 * 
	 * IF you want to FORCEFULLY override the device ID:
	 * 	Please use Command-line args.
	 * 	Example:
	 * 	"PackagedGameClient.exe -deviceid YOUR_DEVICE_ID ......"
	 * 
	 * ELSE use the "DefaultEngine.ini" file to control the device ID.
	 * First of all, select the override method using this field in configuration:
	 * ===>Section:	[AccelByte.Dev]
	 * 	Key:		DeviceIdOverrideMethod=(STRING)
	 * 	Value:
	 * 	Supported string as enumerator = {COMMANDLINE, PICK_RANDOM, RANDOMIZE, PERSISTENT}
	 * 	Example
	 * 			[AccelByte.Dev]
	 * 			DeviceIdOverrideMethod=RANDOMIZE
	 * 	IF this field is wrong or left empty:
	 * 		The override behavior will be done by the following method order [COMMANDLINE, PICK_RANDOM, RANDOMIZE, PERSISTENT]
	 * 
	 * Then specify the key/value for the list of PICK_RANDOM override method
	 * ===>Section:	[AccelByte.Dev]
	 * 	Key:
	 * 		[_] DeviceId=(STRING[])
	 * 			Example
	 * 				[AccelByte.Dev]
	 * 				+DeviceId=aaasdfsadfasd
	 * 				+DeviceId=sdahssdghsgfd
	 * 				+DeviceId=safvcgdsfgsgf
	 * 				+DeviceId=gsdgdsfgfafsf
	 * 
	 * COMPILED EXAMPLE from DefaultEngine.ini
	 * [AccelByte.Dev]
	 * DeviceIdOverrideMethod=[COMMANDLINE, or PICK_RANDOM, or RANDOMIZE, or PERSISTENT]
	 * +DeviceId=aaasdfsadfasd
	 * +DeviceId=sdahssdghsgfd
	 * +DeviceId=safvcgdsfgsgf
	 * +DeviceId=gsdgdsfgfafsf
	 *
	 * To deny all override and pretend to be shipping build:
	 * ===>Section:	[AccelByte.Dev]<summary>
	 * 	Key:		DiscardOverride=(BOOL)
	 * 		Example
	 * 			[AccelByte.Dev]
	 * 			DiscardOverride=true
	 *
	 *  @param Default The default value if there is no override found
	 *
	 *  @return String of DeviceID.
	 */
	static FString GetDevModeDeviceId(FString const& Default);

	static FString AccelByteStoredSectionIdentifiers() { return FApp::GetProjectName() / FString(TEXT("Identifiers")); }
#pragma endregion
	
	static FString AccelByteStored() 
	{ 
		FString Result = TEXT("AccelByteStored");
#ifdef WITH_AUTOMATION_TESTS
		Result = FString::Printf(TEXT("%s-%s"), *Result, *GetFlightId());
#endif
		return Result; 
	} 
	static FString AccelByteStoredSectionIAM() { return FString(TEXT("IAM")); }
	static FString AccelByteStoredKeyAuthTrustId() { return FString(TEXT("auth-trust-id")); }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPubIp
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Web | Models | Ip")
		FString Ip {};
};

/**
 * @brief AccelByteNetUtilities 
 */
class ACCELBYTEUE4SDK_API FAccelByteNetUtilities
{
public:
	/**
	 * @brief Get Public IP using api.ipify.org
	 *
	 * @param OnSuccess Callback function for successful delegate.
	 * @param OnError Callback function for error delegate.
	 */
	static void GetPublicIP(THandler<FAccelByteModelsPubIp> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Download  acontent from specified URL with several callback functions including OnProgress, OnDownloaded, and OnError.
	 *
	 * @param Url Specified URL to download the content.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnDownloaded Callback function for successful download delegate.
	 * @param OnError Callback function for error delegate.
	 */
	static void DownloadFrom(FString const& Url
		, FHttpRequestProgressDelegate const& OnProgress
		, const THandler<TArray<uint8>>& OnDownloaded
		, const FErrorHandler& OnError);

	/**
	 * @brief Upload a content using specified URL with several callback functions including OnProgress, OnSuccess, and OnError.
	 *
	 * @param Url Specified URL to upload the content.
	 * @param DataUpload Content to upload.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnSuccess Callback function for successful upload delegate.
	 * @param OnError Callback function for error delegate.
	 * @param ContentType Specified content-type header which determine the type of uploaded content (default: application/octet-stream)
	 */
	static void UploadTo(FString const& Url
		, TArray<uint8> const& DataUpload
		, FHttpRequestProgressDelegate const& OnProgress
		, AccelByte::FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& ContentType = TEXT("application/octet-stream"));

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
	/**
	 * @brief Download  acontent from specified URL with several callback functions including OnProgress, OnDownloaded, and OnError.
	 *
	 * @param Url Specified URL to download the content.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnDownloaded Callback function for successful download delegate.
	 * @param OnError Callback function for error delegate.
	 */
	static void DownloadFrom(FString const& Url
		, FHttpRequestProgressDelegate64 const& OnProgress
		, const THandler<TArray<uint8>>& OnDownloaded
		, const FErrorHandler& OnError);

	/**
	 * @brief Upload a content using specified URL with several callback functions including OnProgress, OnSuccess, and OnError.
	 *
	 * @param Url Specified URL to upload the content.
	 * @param DataUpload Content to upload.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnSuccess Callback function for successful upload delegate.
	 * @param OnError Callback function for error delegate.
	 * @param ContentType Specified content-type header which determine the type of uploaded content (default: application/octet-stream)
	 */
	static void UploadTo(FString const& Url
		, TArray<uint8> const& DataUpload
		, FHttpRequestProgressDelegate64 const& OnProgress
		, AccelByte::FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& ContentType = TEXT("application/octet-stream"));
#endif

	/**
	 * @brief Check whether specified URL is a valid URL format or not.
	 *
	 * @param Url Specified URL.
	 * 
	 * @return true if the URL is valid, and
	 * @return false if it is invalid.
	 */
	static bool IsValidUrl(FString const& Url);
};
