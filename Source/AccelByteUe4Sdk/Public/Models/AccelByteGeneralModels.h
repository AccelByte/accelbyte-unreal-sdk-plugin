// Copyright (c) 2019 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Runtime/Launch/Resources/Version.h"
#include "AccelByteGeneralModels.generated.h"

UENUM()
enum class EAccelByteTaskState
{
	/** Task has completed execution. */
	Completed,

	/** Task execution is still pending. */
	Pending,

	/** Task execution is paused because Bearer Auth Rejected. */
	Paused,

	/** Task is executing. */
	Running,

	/** Task execution failed. */
	Failed,

	/** Task has been canceled. */
	Cancelled,

	/** Task has been canceled. */
	Retrying,
};

UENUM(BlueprintType)
enum class EAccelBytePaginationType : uint8
{
	FIRST = 0,
	NEXT,
	PREVIOUS
};

UENUM(BlueprintType)
enum class EBanReason : uint8
{
	EMPTY,
	VIOLENCE,
	HARASSMENT,
	HATEFUL_CONDUCT,
	OFFENSIVE_USERNAME,
	IMPERSONATION,
	MALICIOUS_CONTENT,
	SEXUALLY_SUGGESTIVE,
	SEXUAL_VIOLENCE,
	EXTREME_VIOLENCE,
	UNDERAGE_USER,
	CHEATING,
	TOS_VIOLATION
};

UENUM(BlueprintType)
enum class EAccelByteCycle : uint8
{
	DAILY,
	WEEKLY,
	MONTHLY,
	QUARTERLY,
	YEARLY,
	ANNUALLY,
	SEASONAL
};

UENUM(BlueprintType)
enum class EAccelByteFileType : uint8
{
	NONE = 0,
	JPEG,
	JPG,
	PNG,
	BMP,
	GIF,
	MP3,
	BIN,
	WEBP
};

UENUM(BlueprintType)
enum class EAccelByteTTLConfigAction : uint8
{
	NONE = 0,
	DELETE_RECORD
};

UENUM(BlueprintType)
enum class EAccelByteGeneralSortBy : uint8
{
	NONE = 0,
	ASC,
	DESC
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaging
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString First{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString Last{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString Next{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString Previous{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Miscellaneous")
	FDateTime CurrentTime{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteLRUHttpStruct
{
	GENERATED_BODY()

	// UObject can't have a constructor with args
	bool Serialize(const FHttpRequestPtr& RequestPtr
		, double InExpireTime)
	{
		if (!RequestPtr.IsValid())
		{
			return false;
		}

		auto ResponsePtr = RequestPtr->GetResponse();

		if (!ResponsePtr.IsValid())
		{
			return false;
		}

		SetMember(RequestPtr->GetAllHeaders()
			, ResponsePtr->GetAllHeaders()
			, ResponsePtr->GetResponseCode()
			, RequestPtr->GetURL()
			, ResponsePtr->GetContent()
			, InExpireTime);
		return true;
	}
	
	void SetMember(const TArray<FString>& InRequestHeaders
		, const TArray<FString>& InResponseHeaders
		, int InResponseCode
		, const FString& InRequestURL
		, const TArray<uint8>& InResponsePayload
		, double InExpireTime)
	{
		RequestHeaders = InRequestHeaders;
		ResponseHeaders = InResponseHeaders;
		ResponseCode = InResponseCode;
		RequestURL = InRequestURL;
		ResponsePayload = InResponsePayload;
		ExpireTime = FString::SanitizeFloat(InExpireTime);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | LRUCache")
	TArray<FString> RequestHeaders{};
	
	/** @brief Header array in format "Name: Value". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | LRUCache")
	TArray<FString> ResponseHeaders{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | LRUCache")
	int ResponseCode = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | LRUCache")
	FString RequestURL {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | LRUCache")
	TArray<uint8> ResponsePayload{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | LRUCache")
	FString ExpireTime{};
};

struct FAccelByteHttpCacheItem
{
	/** @brief Platform time until cached response is stale (in seconds). */
	double ExpireTime{ 0.0f };

	/** @brief The raw request and response serialized. */
	FAccelByteLRUHttpStruct SerializableRequestAndResponse{};
};

struct FRequestBucket
{
	uint32 AvailableToken {0};
	double ResetTokenTime{ 0.0f };
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FTTLConfig
{
	GENERATED_BODY()

	/** @brief Action type after the time is expired. Example: Delete the record data after the desired time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | TTLConfig")
	EAccelByteTTLConfigAction Action{EAccelByteTTLConfigAction::NONE};
	
	/** @brief Expired time that indicates to take an action. Example: FDateTime::UtcNow() + FTimespan(0, 0, 5); */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | TTLConfig")
	FDateTime Expires_At{0};
};

class FAccelByteHttpResponseConstructable : public IHttpResponse
{
private:
	int32 ResponseCode{};

	TArray<uint8> RawContent{};

	TArray<FString> CombinedHeaders; // key: value

	const FString EffectiveURL = TEXT("");

	FString URL{};

public:
	
	virtual int32 GetResponseCode() const override
	{
		return ResponseCode;
	}

	void SetResponseCode(int32 InResponseCode)
	{
		ResponseCode = InResponseCode;
	}

	virtual FString GetContentAsString() const override
	{
		FUTF8ToTCHAR TCHARData(reinterpret_cast<const ANSICHAR*>(RawContent.GetData()), RawContent.Num());
		return FString(TCHARData.Length(), TCHARData.Get());
	}

	// Need to override this function for engines starting from UE 5.6.
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
	virtual FUtf8StringView GetContentAsUtf8StringView() const override
	{
		FUtf8StringView Data(reinterpret_cast<const UTF8CHAR*>(RawContent.GetData()), RawContent.Num());
		return Data;
	}
#endif

	void SetPayload(const TArray<uint8>& Input)
	{
		RawContent = Input;
	}

	// For engines below UE 5.6, we need to use FString GetURL() to return the URL, otherwise use FString& GetURL().
#if ENGINE_MAJOR_VERSION <= 5 && ENGINE_MINOR_VERSION < 6 || ENGINE_MAJOR_VERSION == 4
	virtual FString GetURL() const override
	{
		return URL;
	}
#else
	virtual const FString& GetURL() const override
	{
		return URL;
	}
#endif

	void SetURL(const FString& Input)
	{
		URL = Input;
	}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
	virtual const FString& GetEffectiveURL() const override
	{
		return EffectiveURL;
	}

	virtual EHttpRequestStatus::Type GetStatus() const override
	{
		return EHttpRequestStatus::Processing;
	}

	virtual EHttpFailureReason GetFailureReason() const override
	{
		return EHttpFailureReason::TimedOut;
	}
#endif

	virtual FString GetURLParameter(const FString& ParameterName) const override { return ""; }

	virtual FString GetHeader(const FString& HeaderName) const override
	{
		for (const auto& HeaderKeyValue : CombinedHeaders)
		{
			if (HeaderKeyValue.Contains(HeaderName))
			{
				FString Copy = HeaderKeyValue;
				FString Key = "";
				FString Value = "";
				Copy.Split(":", &Key, &Value);
				
				return Value.TrimStartAndEnd();
			}
		}

		return "";
	}

	virtual TArray<FString> GetAllHeaders() const override
	{
		return CombinedHeaders;
	}

	void SetHeaders(const TArray<FString>& Input) { CombinedHeaders = Input; }

	virtual FString GetContentType() const override { return GetHeader(TEXT("Content-Type")); }

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
	virtual uint64 GetContentLength() const override
#else
	virtual int32 GetContentLength() const override
#endif
	{
		return RawContent.Num();
	}

	virtual const TArray<uint8>& GetContent() const override
	{
		return RawContent;
	}
};

