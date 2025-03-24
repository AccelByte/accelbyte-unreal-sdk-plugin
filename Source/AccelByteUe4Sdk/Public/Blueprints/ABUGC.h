// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteUGCModels.h"
#include "ABUGC.generated.h"

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
	void PublicGetContentByContentId(
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
	void PublicGetContentByShareCode(
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
	void GetContentBulk(
		TArray<FString> const& ContentIds,
		FDModelsUGCBulkContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetContentBulk(
		TArray<FString> const& ContentIds,
		FDModelsUGCBulkContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetUserContent(
		FString const& UserId,
		FDModelsUGCContentPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetUserContent(
		FString const& UserId,
		FDModelsUGCContentPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
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
	void SearchAllContents(FAccelByteModelsUGCSearchContentsRequest const& Request
		, FDModelsUGCSearchContentsPagingResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicSearchContents(const FString& Name,
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
	void PublicSearchAllContents(FAccelByteModelsUGCSearchContentsRequest const& Request
		, FDModelsUGCSearchContentsPagingResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

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
	void SearchContentsByChannelId(FString const& ChannelId,
		FAccelByteModelsUGCSearchContentsRequest const& Request,
		FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 20,
		int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void SearchContentsSpecificToChannelV2(const FString& ChannelId,
		FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0,
		EAccelByteUGCContentSortByV2 SortBy = EAccelByteUGCContentSortByV2::CREATED_TIME_ASC);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicSearchContentsSpecificToChannelV2(const FString& ChannelId,
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
	void PublicSearchContentsV2(const FAccelByteModelsUGCFilterRequestV2& Filter,
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
	void PublicGetContentBulkByIdsV2(const TArray<FString>& ContentIds, 
		FDModelsUGCBulkContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByShareCodeV2(FString const& ShareCode,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetContentByShareCodeV2(FString const& ShareCode,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByContentIdV2(FString const& ContentId,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetContentByContentIdV2(FString const& ContentId,
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
	void PublicGetUserContentsV2(const FString& UserId
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

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentByShareCode(FString const& ChannelId,
		FString const& ShareCode,
		FAccelByteModelsUGCUpdateRequest const& ModifyRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteContentByShareCode(FString const& ChannelId,
		FString const& ShareCode,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void BulkGetContentByShareCode(const TArray<FString>& ShareCodes, 
		FDModelsUGCBulkContentResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void BulkGetContentByShareCodeV2(const TArray<FString>& ShareCodes, 
		FDModelsUGCBulkContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentShareCode(FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentShareCodeV2(FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest,
		FDModelsUGCCreateUGCResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetStagingContents(EStagingContentRequestStatus Status,
		FDModelsUGCPaginatedListStagingContent const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 20,
		int32 Offset = 0,
		EAccelByteStagingContentUtilitiesSortBy SortBy = EAccelByteStagingContentUtilitiesSortBy::NONE);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetStagingContentById(FString const& ContentId,
		FDModelsUGCStagingContent const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateStagingContent(FString const& ContentId,
		FAccelByteModelsUGCUpdateContentFileLocationRequestV2 const& UpdateRequest,
		FDModelsUGCStagingContent const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteStagingContent(FString const& ContentId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
