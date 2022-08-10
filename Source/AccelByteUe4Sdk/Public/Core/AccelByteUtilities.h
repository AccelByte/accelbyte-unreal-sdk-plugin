// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "JsonObjectConverter.h"

#include "AccelByteUtilities.generated.h"

using AccelByte::THandler;
using AccelByte::FErrorHandler;

enum class EAccelBytePlatformType : uint8;

enum class EJwtResult
{
	Ok,
	MalformedJwt,
	SignatureMismatch,
	AlgorithmMismatch,
	MalformedPublicKey
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
	
	static FString CreateQueryParams(TMap<FString, FString> Map)
	{
		FString Query = TEXT("");
		for (auto kvp : Map)
		{
			AppendQueryParam(Query, kvp.Key, kvp.Value);
		}
		return Query;
	}

	static FString CreateQueryParamValueFromArray(TArray<FString> Array)
	{
		FString QueryParamValue = TEXT("");
		if (Array.Num() > 0)
		{ 
			for (int i = 0; i < Array.Num(); i++)
			{
				FString ItemAppended = FString::Printf(TEXT(",%s"), *Array[i]);
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
		uint8 Flags = FieldRemovalFlagObjects | FieldRemovalFlagStrings | FieldRemovalFlagArrays | FieldRemovalFlagNested)
	{
		TArray<FString> FieldsToRemove;
		TMap<FString, TSharedPtr<FJsonValue>> FieldsToReplace;

		// Remove empty field so it doesn't get updated in BE
		for(auto& KeyValuePair : JsonObjectPtr->Values)
		{
			if(!KeyValuePair.Value.IsValid())
			{
				continue;
			}

			bool bRemoveField = false;
			switch (KeyValuePair.Value->Type)
			{
			case EJson::Array:
			{
				TArray<TSharedPtr<FJsonValue>> OriginalArray = KeyValuePair.Value->AsArray();
				bRemoveField = (Flags & FieldRemovalFlagArrays) != 0 && OriginalArray.Num() == 0;

				if(bRemoveField || (Flags & FieldRemovalFlagNested) == 0)
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
					RemoveEmptyFieldsFromJson(NestedObject, Flags);
					NewArray.Add(MakeShared<FJsonValueObject>(NestedObject));
				}

				if((Flags & FieldRemovalFlagArrays) != 0 && NewArray.Num() == 0)
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
				bRemoveField = (Flags & FieldRemovalFlagObjects) != 0 && Object->Values.Num() == 0;

				if(bRemoveField || (Flags & FieldRemovalFlagNested) == 0)
				{
					break;
				}

				RemoveEmptyFieldsFromJson(Object, Flags);
				if((Flags & FieldRemovalFlagObjects) != 0 && Object->Values.Num() == 0)
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
					((Flags & FieldRemovalFlagStrings) != 0 && Value.IsEmpty()) ||
					((Flags & FieldRemovalFlagDates) != 0 && Value.Equals(FDateTime(0).ToString()));

				break;
			}
			case EJson::Number:
			{
				bRemoveField = (Flags & FieldRemovalFlagNumbers) != 0 && KeyValuePair.Value->AsNumber() == 0;
				break;
			}
			case EJson::Null:
			{
				bRemoveField = (Flags & FieldRemovalFlagNull) != 0;
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
	}

private:
	static void AppendQueryParam(FString& Query, FString const& Param, FString const& Value)
	{
		if (!Param.IsEmpty() && !Value.IsEmpty())
		{			
			Query.Append(Query.IsEmpty() ? TEXT("?") : TEXT("&"));
			Query.Append(FString::Printf(TEXT("%s=%s"), *Param, *Value));
		}		
	}
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
