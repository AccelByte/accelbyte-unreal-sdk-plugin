// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteUGCModels.h"
#include "ABUGC.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCResponse, FAccelByteModelsUGCResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCContentResponse, FAccelByteModelsUGCContentResponse, Response);

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

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABUGC : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateContentByStruct(
		FString const& ChannelId,
		FAccelByteModelsUGCRequest const& UGCRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateContent(
		FString const& ChannelId,
		FString const& Name,
		FString const& Type,
		FString const& SubType,
		TArray<FString> const& Tags,
		TArray<uint8> const& Preview,
		FString const& FileExtension,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		FString ContentType = TEXT("application/octet-stream")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentByUpdateStruct(
		FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsUGCUpdateRequest const& UGCRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentByStruct(
		FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsUGCRequest const& UGCRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContent(
		FString const& ChannelId,
		FString const& ContentId,
		FString const& Name,
		FString const& Type,
		FString const& SubType,
		TArray<FString> const& Tags,
		TArray<uint8> const& Preview,
		FString const& FileExtension,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		FString ContentType = TEXT("application/octet-stream")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteContent(
		FString const& ChannelId,
		FString const& ContentId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByContentId(
		FString const& ContentId,
		FDModelsUGCContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByShareCode(
		FString const& ShareCode,
		FDModelsUGCContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentPreviewAsStruct(
		FString const& ContentId,
		FDModelsUGCPreviewResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentPreviewAsIntArray(
		FString const& ContentId,
		FDArrayUint8 const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetTags(
		FDModelsUGCTagsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetTypes(
		FDModelsUGCTypesPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateChannel(
		FString const& ChannelName,
		FDModelsUGCChannelResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetChannels(
		FDModelsUGCChannelsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0,
		FString const& ChannelName = ""
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetChannelsByUserId(
		const FString& UserId,
		FDModelsUGCChannelsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteChannel(
		FString const& ChannelId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void SearchContents(const FString& Name,
		const FString& Creator,
		const FString& Type,
		const FString& Subtype,
		const TArray<FString>& Tags,
		bool IsOfficial,
		const FString& UserId,
		FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteUgcSortBy SortBy = EAccelByteUgcSortBy::DATE,
		EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC,
		int32 Limit = 1000,
		int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateLikeStatusToContent(const FString& ContentId,
		bool bLikeStatus,
		FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetListFollowers(const FString& UserId,
		FDModelsUGCGetListFollowersPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateFollowStatusToUser(const FString& UserId,
		bool bFollowStatus, 
		FDModelsUGCUpdateFollowStatusToUserResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void SearchContentsSpecificToChannel(const FString& ChannelId,
		const FString& Name,
		const FString& Creator,
		const FString& Type,
		const FString& Subtype,
		const TArray<FString>& Tags,
		bool IsOfficial,
		const FString& UserId,
		FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteUgcSortBy SortBy = EAccelByteUgcSortBy::DATE,
		EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC,
		int32 Limit = 1000,
		int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void SearchContentsSpecificToChannelV2(const FString& ChannelId,
		FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0,
		EAccelByteUGCContentSortByV2 SortBy = EAccelByteUGCContentSortByV2::CREATED_TIME_ASC);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void SearchContentsV2(const FAccelByteModelsUGCFilterRequestV2& Filter,
		FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000, 
		int32 Offset = 0, 
		EAccelByteUGCContentSortByV2 SortBy = EAccelByteUGCContentSortByV2::CREATED_TIME_ASC);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentBulkByIdsV2(const TArray<FString>& ContentIds, 
		FDModelsUGCBulkContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByShareCodeV2(FString const& ShareCode,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByContentIdV2(FString const& ContentId,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateContentV2(FString const& ChannelId,
		FAccelByteModelsCreateUGCRequestV2 const& CreateRequest,
		FDModelsUGCCreateUGCResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteContentV2(
		FString const& ChannelId,
		FString const& ContentId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentV2(FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsModifyUGCRequestV2 const& ModifyRequest,
		FDModelsUGCModifyUGCResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GenerateUploadContentURLV2(FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsUploadContentURLRequestV2 const& UploadRequest,
		FDModelsUGCUploadContentResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateContentFileLocationV2(FString const& ChannelId,
		FString const& ContentId,
		FString const& FileExtension,
		FString const& S3Key,
		FDModelsUGCUpdateContentFileLocation const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetUserContentsV2(const FString& UserId
		, FDModelsUGCContentPageResponseV2 const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateContentScreenshotV2(const FString& ContentId
		, FAccelByteModelsUGCUpdateScreenshotsV2 ScreenshotsRequest
		, FDModelsUGCUpdateScreenshotsV2 const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UploadContentScreenshotV2(const FString& ContentId
		, FAccelByteModelsUGCUploadScreenshotsRequestV2 ScreenshotsRequest
		, FDModelsUGCUpdateContentScreenshotResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteContentScreenshotV2(const FString& ContentId
		, const FString& ScreenshotId
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void AddDownloadContentCountV2(FString const& ContentId,
		FDModelsUGCAddDownloadContentCountResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetListContentDownloaderV2(FString const& ContentId,
		FDModelsUGCGetPaginatedContentDownloaderResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		FString const& UserId = TEXT(""),
		int32 Limit = 1000,
		int32 Offset = 0,
		EAccelByteUGCContentUtilitiesSortByV2 SortBy = EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_ASC);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetListContentLikerV2(FString const& ContentId,
		FDModelsUGCGetPaginatedContentLikerResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateLikeStatusToContentV2(const FString& ContentId,
		bool bLikeStatus,
		FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess,
		FDErrorHandler const& OnError);
		
private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
