﻿// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteUGCModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteUgcSortBy : uint8
{
	NAME = 0,
	DOWNLOAD,
	LIKE,
	DATE,
	UPDATED_TIME
};

UENUM(BlueprintType)
enum class EAccelByteLikedContentSortBy : uint8
{
	NAME = 0,
	DOWNLOAD,
	LIKE,
	DATE
};

UENUM(BlueprintType)
enum class EAccelByteUgcOrderBy : uint8
{
	ASC = 0,
	DESC
};

UENUM(BlueprintType)
enum class EAccelByteUGCContentSortByV2 : uint8
{
	NAME = 0,
	NAME_ASC,
	NAME_DESC,
	DOWNLOAD,
	DOWNLOAD_ASC,
	DOWNLOAD_DESC,
	LIKE,
	LIKE_ASC,
	LIKE_DESC,
	CREATED_TIME,
	CREATED_TIME_ASC,
	CREATED_TIME_DESC
};

UENUM(BlueprintType)
enum class EAccelByteUGCContentUtilitiesSortByV2 : uint8
{
	NONE,
	CREATED_TIME,
	CREATED_TIME_ASC,
	CREATED_TIME_DESC
};

UENUM(BlueprintType)
enum class EAccelByteStagingContentUtilitiesSortBy : uint8
{
	NONE,
	CREATED_TIME,
	CREATED_TIME_ASC,
	CREATED_TIME_DESC,
	UPDATED_TIME,
	UPDATED_TIME_ASC,
	UPDATED_TIME_DESC
};

UENUM(BlueprintType)
enum class EStagingContentResponseStatus : uint8
{
	PENDING = 0,
	REJECTED,
	APPROVED
};

UENUM(BlueprintType)
enum class EStagingContentRequestStatus : uint8
{
	PENDING = 0,
	REJECTED
};

UENUM(BlueprintType)
enum class EContentStatus : uint8
{
	UNDER_REVIEW = 0,
	PUBLISHED
};

class ACCELBYTEUE4SDK_API FAccelByteUGCUtilities
{
public:
	static FString ConvertLikedContentSortByToString(const EAccelByteLikedContentSortBy& SortBy)
	{
		switch (SortBy)
		{
		case EAccelByteLikedContentSortBy::NAME:
			return TEXT("name");
		case EAccelByteLikedContentSortBy::DATE:
			return TEXT("date");
		case EAccelByteLikedContentSortBy::DOWNLOAD:
			return TEXT("download");
		case EAccelByteLikedContentSortBy::LIKE:
			return TEXT("like");
		default:
			return TEXT("");
		}
	}

	static FString ConvertUGCSortByToString(const EAccelByteUgcSortBy& SortBy)
	{
		switch (SortBy)
		{
		case EAccelByteUgcSortBy::NAME:
			return TEXT("name");
		case EAccelByteUgcSortBy::DATE:
			return TEXT("date");
		case EAccelByteUgcSortBy::DOWNLOAD:
			return TEXT("download");
		case EAccelByteUgcSortBy::LIKE:
			return TEXT("like");
		case EAccelByteUgcSortBy::UPDATED_TIME:
			return TEXT("updatedTime");
		default:
			return TEXT("");
		}
	}

	static FString ConvertUGCOrderByToString(const EAccelByteUgcOrderBy& OrderBy)
	{
		switch (OrderBy)
		{
		case EAccelByteUgcOrderBy::ASC:
			return TEXT("asc");
		case EAccelByteUgcOrderBy::DESC:
			return TEXT("desc");
		default:
			return TEXT("");
		}
	}

	static FString ConvertGetUGContentsSortByToString(const EAccelByteUGCContentSortByV2& SortBy)
	{
		switch (SortBy)
		{
		case EAccelByteUGCContentSortByV2::NAME:
			return TEXT("name");
		case EAccelByteUGCContentSortByV2::NAME_ASC:
			return TEXT("name:asc");
		case EAccelByteUGCContentSortByV2::NAME_DESC:
			return TEXT("name:desc");
		case EAccelByteUGCContentSortByV2::DOWNLOAD:
			return TEXT("download");
		case EAccelByteUGCContentSortByV2::DOWNLOAD_ASC:
			return TEXT("download:asc");
		case EAccelByteUGCContentSortByV2::DOWNLOAD_DESC:
			return TEXT("download:desc");
		case EAccelByteUGCContentSortByV2::LIKE:
			return TEXT("like");
		case EAccelByteUGCContentSortByV2::LIKE_ASC:
			return TEXT("like:asc");
		case EAccelByteUGCContentSortByV2::LIKE_DESC:
			return TEXT("like:desc");
		case EAccelByteUGCContentSortByV2::CREATED_TIME:
			return TEXT("createdTime");
		case EAccelByteUGCContentSortByV2::CREATED_TIME_ASC:
			return TEXT("createdTime:asc");
		case EAccelByteUGCContentSortByV2::CREATED_TIME_DESC:
			return TEXT("createdTime:desc");
		default:
			return TEXT("");
		}
	}

	static FString ConvertUGCUtilitiesSortByToString(const EAccelByteUGCContentUtilitiesSortByV2& SortBy)
	{
		switch (SortBy)
		{
		case EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME:
			return TEXT("createdTime");
		case EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_ASC:
			return TEXT("createdTime:asc");
		case EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_DESC:
			return TEXT("createdTime:desc");
		default:
			return TEXT("");
		}
	}

	static FString ConvertStagingContentSortByToString(const EAccelByteStagingContentUtilitiesSortBy& SortBy)
	{
		switch (SortBy)
		{
		case EAccelByteStagingContentUtilitiesSortBy::CREATED_TIME:
			return TEXT("createdTime");
		case EAccelByteStagingContentUtilitiesSortBy::CREATED_TIME_ASC:
			return TEXT("createdTime:asc");
		case EAccelByteStagingContentUtilitiesSortBy::CREATED_TIME_DESC:
			return TEXT("createdTime:desc");
		case EAccelByteStagingContentUtilitiesSortBy::UPDATED_TIME:
			return TEXT("updatedTime");
		case EAccelByteStagingContentUtilitiesSortBy::UPDATED_TIME_ASC:
			return TEXT("updatedTime:asc");
		case EAccelByteStagingContentUtilitiesSortBy::UPDATED_TIME_DESC:
			return TEXT("updatedTime:desc");
		default:
			return TEXT("");
		}
	}
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCPayloadUrl
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString Source{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString Url{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCScreenshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString ScreenshotId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString Source{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Payload")
	FString Url{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUploadScreenshotV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadScreenshotV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadScreenshotV2")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadScreenshotV2")
	FString FileExtension{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateScreenshotV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateScreenshotV2")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateScreenshotV2")
	FString ScreenshotId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCCreatorState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | State")
	bool State{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | State")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCTagResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Tag")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Tag")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Tag")
	FString Tag{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCChannelResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCChannelResponse")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCChannelResponse")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCChannelResponse")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCChannelResponse")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCTypeResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCTypeResponse")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCTypeResponse")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCTypeResponse")
	TArray<FString> Subtype{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCTypeResponse")
	FString Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCPreviewMetadata
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | PreviewMetadata")
	FString PreviewContentType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | PreviewMetadata")
	FString PreviewFileExtension{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCModifyContentShareCodeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyContentShareCodeRequest")
	FString ShareCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FString ContentType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FString FileExtension{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FString Preview{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FString Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FString SubType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FJsonObjectWrapper CustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Request")
	FAccelByteModelsUGCPreviewMetadata PreviewMetadata{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateRequest : public FAccelByteModelsUGCRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateRequest")
	bool UpdateContentFile{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString ChannelId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString ContentType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FDateTime CreatedTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString CreatorName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString FileExtension{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	bool IsOfficial{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	TArray<FAccelByteModelsUGCPayloadUrl> PayloadUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString Preview{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString ShareCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString SubType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	FJsonObjectWrapper CustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Response")
	EContentStatus ContentStatus{EContentStatus::UNDER_REVIEW};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCContentResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString ChannelId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FDateTime CreatedTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FAccelByteModelsUGCCreatorState CreatorFollowState{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString CreatorName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	int32 DownloadCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString FileExtension{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	TArray<FString> Groups{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	bool IsHidden{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	bool IsOfficial{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	int32 LikeCount{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FAccelByteModelsUGCCreatorState LikeState{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString Payload{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	TArray<FAccelByteModelsUGCPayloadUrl> PayloadUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	TArray<FAccelByteModelsUGCPayloadUrl> PreviewURL{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	TArray<FAccelByteModelsUGCScreenshot> Screenshots{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString ShareCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString SubType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FDateTime UpdatedTime{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentResponse")
	FJsonObjectWrapper CustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCTagsPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Tags Paging")
	TArray<FAccelByteModelsUGCTagResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Tags Paging")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCTypesPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Types Paging")
	TArray<FAccelByteModelsUGCTypeResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Types Paging")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCChannelsPagingResponse {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Channels Paging")
	TArray<FAccelByteModelsUGCChannelResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Channels Paging")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCPreview
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Preview")
	FString Preview{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserIdState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UserIdState")
	bool State{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UserIdState")
	FString UserId{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelSpayLoadURL
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SpayLoadURL")
	FString Source{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SpayLoadURL")
	FString Url{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelScreenshots
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Screenshots")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Screenshots")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Screenshots")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Screenshots")
	FString ScreenshotId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Screenshots")
	FString Source{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Screenshots")
	FString Url{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCSearchContentsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	FString Creator{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	FString Subtype{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	bool bIsOfficial{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	bool bIsHidden{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	EAccelByteUgcSortBy SortBy{EAccelByteUgcSortBy::DATE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsRequest")
	EAccelByteUgcOrderBy OrderBy{EAccelByteUgcOrderBy::DESC};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCSearchContentsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString ChannelId{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString CreatedTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FAccelByteModelsUserIdState CreatorFollowState{};			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString CreatorName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	int DownloadCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString FileExtension{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	TArray<FString> Groups{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	bool IsHidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	bool IsOfficial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	int LikeCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FAccelByteModelsUserIdState LikeState{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString Payload{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	TArray<FAccelByteModelSpayLoadURL> PayloadURL{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	TArray<FAccelByteModelSpayLoadURL> PreviewURL{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	TArray<FAccelByteModelScreenshots> Screenshots{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString ShareCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString UpdatedTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponse")
	FJsonObjectWrapper CustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCSearchContentsPagingResponse {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsPagingResponse")
	TArray<FAccelByteModelsUGCSearchContentsResponse> Data{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsPagingResponse")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateLikeStatusToContentResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateLikeStatusToContentResponse")
	FString ContentId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateLikeStatusToContentResponse")
	bool LikeStatus{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetListFollowersResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersResponse")
	int32 FollowCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersResponse")
	int32 FollowingCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersResponse")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersResponse")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersResponse")
	int32 TotalLikedContent{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetListFollowersPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersPagingResponse")
	TArray<FAccelByteModelsUGCGetListFollowersResponse> Data{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetListFollowersPagingResponse")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateFollowStatusToUserResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateFollowStatusToUserResponse")
	bool FollowStatus{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateFollowStatusToUserResponse")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetContentBulkRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | LinkPlatformAccountRequest")
	TArray<FString> ContentIds {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCBulkGetContentByShareCodeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | BulkGetContentByShareCodeRequest")
	TArray<FString> ShareCodes {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCContentPageResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentPageResponse")
	TArray<FAccelByteModelsUGCContentResponse> Data {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentPageResponse")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FContentRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentRequest")
	FString ContentType {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentRequest")
	FString Description {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ContentRequest")
	FString FileExtension {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCScreenshotRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ScreenshotRequest")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ScreenshotRequest")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ScreenshotRequest")
	FString FileExtension{};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCScreenshotsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ScreenshotsRequest")
	TArray<FAccelByteModelsUGCScreenshot> Screenshots {};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUploadScreenshotsRequestV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ScreenshotsRequestV2")
	TArray<FAccelByteModelsUGCUploadScreenshotV2> Screenshots {};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateScreenshotsV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ScreenshotsRequestV2")
	TArray<FAccelByteModelsUGCUpdateScreenshotV2> Screenshots {};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCFilterRequestV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | FilterRequest")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | FilterRequest")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | FilterRequest")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | FilterRequest")
	TArray<FString> Tags{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUploadContentURLRequestV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadContentRequestV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadContentRequestV2")
	FString FileExtension{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUploadContentURLResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadContentRequestV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadContentRequestV2")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadContentRequestV2")
	FString Url{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UploadContentRequestV2")
	FString FileLocation{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateContentScreenshotResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentScreenshotResponse")
	TArray<FAccelByteModelScreenshots> Screenshots {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetFollowedContentResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GetFollowedContentResponse")
	TArray<FAccelByteModelScreenshots> Screenshots {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCFollowedUsersResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | FollowedUsersResponse")
	TArray<FAccelByteModelsUGCGetListFollowersResponse> Data {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | FollowedUsersResponse")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGroups
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Groups")
	TArray<FString> Contents {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Groups")
	FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Groups")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Groups")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Groups")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | Groups")
	FString UserId{};
};
	
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetGroupsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GroupsResponse")
	TArray<FAccelByteModelsUGCGroups> Data {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | GroupsResponse")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCContentResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString ChannelId{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString CreatedTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FAccelByteModelsUserIdState CreatorFollowState{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	int DownloadCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString FileExtension{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	TArray<FString> Groups{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	bool IsHidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	bool IsOfficial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	int LikeCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FAccelByteModelsUserIdState LikeState{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	TArray<FAccelByteModelSpayLoadURL> PayloadURL{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	TArray<FAccelByteModelScreenshots> Screenshots{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString ShareCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FDateTime UpdatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsResponseV2")
	FJsonObjectWrapper CustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCSearchContentsPagingResponseV2 {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsPagingResponseV2")
	TArray<FAccelByteModelsUGCContentResponseV2> Data{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | SearchContentsPagingResponseV2")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreateUGCRequestV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCRequestV2")
	FJsonObjectWrapper CustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCCreateUGCResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString ChannelId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FDateTime CreatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString FileLocation{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	bool IsHidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	bool IsOfficial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString ParentNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	TArray<FAccelByteModelsUGCPayloadUrl> PayloadUrl{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString ShareCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FDateTime UpdatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	FJsonObjectWrapper CustomAttributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | CreateUGCResponseV2")
	EContentStatus ContentStatus{EContentStatus::UNDER_REVIEW};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsModifyUGCRequestV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCRequestV2")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCRequestV2")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCRequestV2")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCRequestV2")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCRequestV2")
	FJsonObjectWrapper CustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCModifyUGCResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString ChannelId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FDateTime CreatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	bool IsHidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	bool IsOfficial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString ParentNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	TArray<FAccelByteModelsUGCPayloadUrl> PayloadUrl{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString ShareCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FDateTime UpdatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | ModifyUGCResponseV2")
	FJsonObjectWrapper CustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCAddDownloadContentCountResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | AddDownloadContentCountResponseV2")
	FString ContentId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCContentDownloaderV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentDownloaderV2")
	FString ContentId {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentDownloaderV2")
	FDateTime CreatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentDownloaderV2")
	FString DownloadedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentDownloaderV2")
	FString Namespace{};
};
	
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetPaginatedContentDownloaderResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | PaginatedContentDownloaderResponseV2")
	TArray<FAccelByteModelsUGCContentDownloaderV2> Data {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | PaginatedContentDownloaderResponseV2")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCContentLikerV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentLikerV2")
	FString ContentId {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentLikerV2")
	FDateTime CreatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentLikerV2")
	FString LikedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UGCContentLikerV2")
	FString Namespace{};
};
	
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCGetPaginatedContentLikerResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | PaginatedContentLikerResponseV2")
	TArray<FAccelByteModelsUGCContentLikerV2> Data {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | PaginatedContentLikerResponseV2")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateContentFileLocationRequestV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString FileLocation{};
};
	
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCUpdateContentFileLocationResponseV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString ChannelId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FDateTime CreatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FJsonObjectWrapper CustomAttributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString FileLocation{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	bool IsHidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	bool IsOfficial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString ParentNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	TArray<FAccelByteModelsUGCPayloadUrl> PayloadUrl{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString ShareCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FDateTime UpdatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | UpdateContentFileLocationV2")
	EContentStatus ContentStatus{EContentStatus::UNDER_REVIEW};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCStagingContentResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString ChannelId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FDateTime CreatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FJsonObjectWrapper CustomAttributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString FileExtension{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString Note{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString PayloadURL{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	TArray<FAccelByteModelsUGCScreenshot> Screenshots{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	EStagingContentResponseStatus Status{EStagingContentResponseStatus::PENDING};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString SubType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FDateTime UpdatedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUGCPaginatedListStagingContentResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	TArray<FAccelByteModelsUGCStagingContentResponse> Data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | UGC | StagingContentResponse")
	FAccelByteModelsPaging Paging{};
};

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCResponse, FAccelByteModelsUGCResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCContentResponse, FAccelByteModelsUGCContentResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCContentPagingResponse, FAccelByteModelsUGCContentPageResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCBulkContentResponse, TArray<FAccelByteModelsUGCContentResponse>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCPreviewResponse, FAccelByteModelsUGCPreview, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayUint8, TArray<uint8>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCTagsPagingResponse, FAccelByteModelsUGCTagsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCTypesPagingResponse, FAccelByteModelsUGCTypesPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCChannelResponse, FAccelByteModelsUGCChannelResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCChannelsPagingResponse, FAccelByteModelsUGCChannelsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCSearchContentsPagingResponse, FAccelByteModelsUGCSearchContentsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUpdateLikeStatusToContentResponse, FAccelByteModelsUGCUpdateLikeStatusToContentResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCGetListFollowersPagingResponse, FAccelByteModelsUGCGetListFollowersPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUpdateFollowStatusToUserResponse, FAccelByteModelsUGCUpdateFollowStatusToUserResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCSearchContentsPagingResponseV2, FAccelByteModelsUGCSearchContentsPagingResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCContentResponseV2, FAccelByteModelsUGCContentResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCBulkContentResponseV2, TArray<FAccelByteModelsUGCContentResponseV2>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCCreateUGCResponseV2, FAccelByteModelsUGCCreateUGCResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCModifyUGCResponseV2, FAccelByteModelsUGCModifyUGCResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUploadContentResponse, FAccelByteModelsUGCUploadContentURLResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCContentPageResponseV2, FAccelByteModelsUGCSearchContentsPagingResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUpdateScreenshotsV2, FAccelByteModelsUGCUpdateScreenshotsV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUpdateContentScreenshotResponse, FAccelByteModelsUGCUpdateContentScreenshotResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCAddDownloadContentCountResponse, FAccelByteModelsUGCAddDownloadContentCountResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCGetPaginatedContentDownloaderResponse, FAccelByteModelsUGCGetPaginatedContentDownloaderResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCGetPaginatedContentLikerResponse, FAccelByteModelsUGCGetPaginatedContentLikerResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUpdateContentFileLocation, FAccelByteModelsUGCUpdateContentFileLocationResponseV2, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCPaginatedListStagingContent, FAccelByteModelsUGCPaginatedListStagingContentResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCStagingContent, FAccelByteModelsUGCStagingContentResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCSearchContentsResponse, FAccelByteModelsUGCSearchContentsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCServerResponse, FAccelByteModelsUGCResponse, Response);

#pragma endregion