// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "JsonObjectConverter.h"
#include "Models/AccelByteEcommerceModels.h"

#include "AccelByteUtilities.generated.h"

using AccelByte::THandler;
using AccelByte::FErrorHandler;

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
 * @brief RSA public key with parameters encoded in Base64Url. Only supports 2048 bits modulus and 24 bits exponent
 */
class ACCELBYTEUE4SDK_API FRsaPublicKey
{
public:
	
	/**
	 * @brief Construct FRsaPublicKey with modulus and exponent
	 * @param ModulusB64Url RSA modulus (n) in Base64URL format
	 * @param ExponentB64Url RSA exponent (e) in Base64URL format
	 */
	FRsaPublicKey(FString ModulusB64Url, FString ExponentB64Url); 

	/**
	 * @brief Check if this RSA public key is valid.    
	 */
	bool IsValid() const;

	
	/**
	 * @brief Convert RSA public key to armored PEM format
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
	 * @brief Construct FJwt from JWT string
	 * @param JwtString JWT encoded as dot separated Base64Url string
	 */
	explicit FJwt(FString JwtString);

	/**
	 * @brief Verify this JWT using RSA public key
	 * @param Key RSA public key
	 * @return EJwtResult::Ok if signature match
	 */
	EJwtResult VerifyWith(FRsaPublicKey Key) const;

	/**
	 * @brief Get header content from JWT. Content could be any valid JSON having at least "alg" field
	 * @return JWT header
	 */
	TSharedPtr<FJsonObject> const& Header() const;

	/**
	 * @brief Get payload content from JWT. Content could be any valid JSON
	 * @return JWT payload
	 */
	TSharedPtr<FJsonObject> const& Payload() const;

	/**
	 * @brief Check if this JWT format is correct and both and payload are valid JSON encoded as Base64URL
	 * @return true if  this JWT is valid, false otherwise
	 */
	bool IsValid() const;

private:
	FString const JwtString;
	int32 const HeaderEnd;
	int32 const PayloadEnd;
	TSharedPtr<FJsonObject> const HeaderJsonPtr;
	TSharedPtr<FJsonObject> const PayloadJsonPtr;
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
	static constexpr uint8 FieldRemovalFlagAll     = 0xFF;

	template<typename CharType = TCHAR, template<typename> class PrintPolicy = TPrettyJsonPrintPolicy, typename InStructType>
	static bool TArrayUStructToJsonString(const TArray<InStructType>& InArray, FString& OutJsonString, int64 CheckFlags = 0, int64 SkipFlags = 0, int32 Indent = 0)
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

	template<typename TEnum>
	static FString GetUEnumValueAsString(TEnum Value)
	{
		FString ValueString = UEnum::GetValueAsString(Value);
		FString Delimiter = TEXT("::");

		TArray<FString> ParsedStrings;

		ValueString.ParseIntoArray(ParsedStrings, *Delimiter);

		return ParsedStrings.Last();
	}

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

	static FString GetPlatformString(EAccelBytePlatformType Platform);

	static FString GetAuthenticatorString(EAccelByteLoginAuthFactorType Authenticator);
	
	static FString CreateQueryParams(TMap<FString, FString> Map, FString SuffixChar = TEXT("?"))
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
 
	static FString CreateQueryParamValueUrlEncodedFromArray(const TArray<FString>& Array, const FString& Delimiter = TEXT(","))
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

	/**
	 * Remove fields which have empty values according to the given flags. Defaults to removing empty objects/arrays, blank
	 * strings, and recursing on object and array field values.
	 */
	static void RemoveEmptyFieldsFromJson(
		const TSharedPtr<FJsonObject>& JsonObjectPtr,
		const uint8 Flags = FieldRemovalFlagObjects | FieldRemovalFlagStrings | FieldRemovalFlagArrays | FieldRemovalFlagNested,
		const TArray<FString>& ExcludedFieldNames = {})
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
				bRemoveField = HAS_FIELD_REMOVAL_FLAG(Numbers) && KeyValuePair.Value->AsNumber() == 0;
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
	
	template<typename ObjectType> 
	static bool UStructArrayToJsonObjectString(TArray<ObjectType> Objects, FString& OutString)
	{
		OutString.Append(TEXT("["));	
		FString JsonArrayString = TEXT("");
		for (auto Item : Objects)
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

	/// <summary>
	/// Try to Get the DeviceID from current platform
	/// Fallback if not found, using MacAddress
	/// Fallback if not found, using a randomized deviceID
	/// NOTE: 
	/// DeviceID for shipping build & dedicated server are always encoded
	/// Other than those, it can use the development mode deviceID that is configurable
	/// </summary>
	/// <param name="bIsDeviceIdRequireEncode"></param>
	/// <returns>Return either plain DeviceID or encoded</returns>
	static FString GetDeviceId(bool bIsDeviceIdRequireEncode = true);

	/// <summary>
	/// Encode HMAC the message using built in function from UnrealEngine and then Base64 the result
	/// </summary>
	/// <param name="Message">Targeted text</param>
	/// <param name="Key">HMAC Buffer Key</param>
	/// <returns>Encode result</returns>
	static FString EncodeHMACBase64(const FString& Message, const FString& Key);

	/// <summary>
	/// Obtain an HMAC Buffer encoded MacAddress, using PublisherNamespace as key.
	/// </summary>
	/// <param name="bEncoded">Is the returned value is encoded</param>
	/// <returns>Return either plain MacAddress or encoded</returns>
	static FString GetMacAddress(bool bEncoded = true);

	static FString GetPlatformName();

	static FString XOR(const FString& Input, const FString& Key);

	static FString GetAuthTrustId();

	static void SetAuthTrustId(const FString& AuthTrustId);

	/// Parse command line to obtain an argument that:
	/// * intialized by dash (-)
	/// * contain a value that following equal sign (=)
	/// Assuming the switch is case insesitive
	/// Example: -serverip=127.0.0.1
	///    key or switch is the serverip
	///    value is 127.0.0.1
	///
	/// @param Key The key of switch argument.
	/// @param Value The operation result will be returned through this parameter.
	///
	/// @return True if value from the second parameter is returned and
	/// @return False if no value/args can be found
	static bool GetValueFromCommandLineSwitch(const FString& Key, FString& Value);
	static bool GetValueFromCommandLineSwitch(const FString& Key, int& Value);
	static bool GetValueFromCommandLineSwitch(const FString& Key, bool& Value);
	static FString ConvertItemSortByToString(EAccelByteItemListSortBy const& SortBy);

//To allow override for testing using mock class
protected:
#pragma region DEVICE_ID
	/// <summary>
	/// Create a randomized 30 digit alpha-numberic DeviceID
	/// </summary>
	/// <returns></returns>
	static FString RandomizeDeviceId();

	/// <summary>
	/// Try to obtain DeviceID from a cached value
	/// If not found using the Default parameter and also store it into the cache
	/// </summary>
	/// <param name="Default"></param>
	/// <returns></returns>
	static FString GetOrSetIfDeviceIdNotFound(FString Default = "");

	/// <summary>
	/// Get the development mode DeviceID override method from "DefaultEngine.ini"
	/// 	Section:	[AccelByte.Dev]
	/// 	Key:		DeviceIdOverrideMethod=(STRING)
	/// </summary>
	/// <returns>Parsing result</returns>
	static EAccelByteDevModeDeviceIdMethod GetCurrentDeviceIdOverrideMethod();

	/// <summary>
	/// Obtain DeviceID for development mode
	/// GAME CLIENT NON SHIPPING BUILD ONLY!
	/// Can be controlled using either command-line args or configuration file.
	/// 
	/// IF you want to FORCEFULLY override the device ID:
	///		Please use Command-line args.
	///		Example:
	/// 	"PackagedGameClient.exe -deviceid YOUR_DEVICE_ID ......"
	/// 
	/// ELSE use the "DefaultEngine.ini" file to control the device ID.
	/// First of all, select the override method using this field in configuration:
	/// ===>Section:	[AccelByte.Dev]
	/// 	Key:		DeviceIdOverrideMethod=(STRING)
	///		Value:
	/// 	Supported string as enumerator = {COMMANDLINE, PICK_RANDOM, RANDOMIZE, PERSISTENT}
	/// 	Example
	/// 			[AccelByte.Dev]
	/// 			DeviceIdOverrideMethod=RANDOMIZE
	///		IF this field is wrong or left empty:
	///			The override behavior will be done by the following method order [COMMANDLINE, PICK_RANDOM, RANDOMIZE, PERSISTENT]
	/// 
	/// Then specify the key/value for the list of PICK_RANDOM override method
	/// ===>Section:	[AccelByte.Dev]
	/// 	Key:
	/// 		[_] DeviceId=(STRING[])
	/// 			Example
	/// 				[AccelByte.Dev]
	/// 				+DeviceId=aaasdfsadfasd
	/// 				+DeviceId=sdahssdghsgfd
	/// 				+DeviceId=safvcgdsfgsgf
	/// 				+DeviceId=gsdgdsfgfafsf
	/// 
	/// COMPILED EXAMPLE from DefaultEngine.ini
	/// [AccelByte.Dev]
	/// DeviceIdOverrideMethod=[COMMANDLINE, or PICK_RANDOM, or RANDOMIZE, or PERSISTENT]
	/// +DeviceId=aaasdfsadfasd
	/// +DeviceId=sdahssdghsgfd
	/// +DeviceId=safvcgdsfgsgf
	/// +DeviceId=gsdgdsfgfafsf
	/// 
	/// To deny all override and pretend to be shipping build:
	/// ===>Section:	[AccelByte.Dev]
	/// 	Key:		DiscardOverride=(BOOL)
	/// 		Example
	/// 			[AccelByte.Dev]
	/// 			DiscardOverride=true
	/// 
	/// </summary>
	/// <param name="Default">The default value if there is no override found</param>
	/// <returns></returns>
	static FString GetDevModeDeviceId(FString Default);

	static FString AccelByteStoredSectionIdentifiers() { return FApp::GetProjectName() / FString(TEXT("Identifiers")); }
	static FString AccelByteStoredKeyDeviceId() { return FString(TEXT("deviceId")); }
#pragma endregion

	static FString AccelByteStored() { return FString(TEXT("AccelByteStored")); } 
	static FString AccelByteStoredSectionIAM() { return FString(TEXT("IAM")); }
	static FString AccelByteStoredKeyAuthTrustId() { return FString(TEXT("auth-trust-id")); }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPubIp
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Web | Models | Ip")
		FString Ip;
};

class ACCELBYTEUE4SDK_API FAccelByteNetUtilities
{
public:
	/*
	* @brief Get Public IP using api.ipify.org
	*/
	static void GetPublicIP(const THandler<FAccelByteModelsPubIp>& OnSuccess, const FErrorHandler& OnError);
	static void DownloadFrom(const FString& Url, const FHttpRequestProgressDelegate& OnProgress, const THandler<TArray<uint8>>& OnDownloaded, const FErrorHandler& OnError);
	static void UploadTo(const FString& Url, const TArray<uint8>& DataUpload, const FHttpRequestProgressDelegate& OnProgress,
		const AccelByte::FVoidHandler& OnSuccess, const FErrorHandler& OnError, FString ContentType = TEXT("application/octet-stream"));
};
