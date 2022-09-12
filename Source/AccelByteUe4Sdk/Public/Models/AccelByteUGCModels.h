// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteUGCModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteUgcSortBy : uint8
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
	bool  LikeStatus{};	
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
	FDateTime CreatedAt{};
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

