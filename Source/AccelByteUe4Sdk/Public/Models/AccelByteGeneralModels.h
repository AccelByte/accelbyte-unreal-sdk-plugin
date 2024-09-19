// Copyright (c) 2019 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "AccelByteGeneralModels.generated.h"

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
	void SetMember(const TArray<FString>& RequestHeaders_, const TArray<FString>& ResponseHeaders_, int ResponseCode_, const FString& RequestURL_, const TArray<uint8>& ResponsePayload_, double ExpireTime_)
	{
		RequestHeaders = RequestHeaders_;
		ResponseHeaders = ResponseHeaders_;
		ResponseCode = ResponseCode_;
		RequestURL = RequestURL_;
		ResponsePayload = ResponsePayload_;
		ExpireTime = FString::SanitizeFloat( ExpireTime_);
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

	/** @brief Completed request with valid response. */
	FHttpRequestPtr Request{nullptr};

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
