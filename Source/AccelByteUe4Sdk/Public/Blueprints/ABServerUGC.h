// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteUGCModels.h"
#include "ABServerUGC.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UABServerUGC final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
		, FDModelsUGCSearchContentsResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void SearchContentsSpecificToChannel(FString const& ChannelId
		, FAccelByteModelsUGCSearchContentsRequest const& Request
		, FDModelsUGCSearchContentsResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void ModifyContentByShareCode(FString const& UserId
		, FString const& ChannelId
		, FString const& ShareCode
		, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
		, FDModelsUGCServerResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void DeleteContentByShareCode(FString const& UserId
		, FString const& ChannelId
		, FString const& ShareCode
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetContentByContentId(
		FString const& ContentId,
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
	void PublicGetContentBulk(
		TArray<FString> const& ContentIds,
		FDModelsUGCBulkContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
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
	void PublicSearchContentsSpecificToChannelV2(const FString& ChannelId,
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
	void PublicGetContentBulkByIdsV2(const TArray<FString>& ContentIds, 
		FDModelsUGCBulkContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetContentByShareCodeV2(FString const& ShareCode,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetContentByContentIdV2(FString const& ContentId,
		FDModelsUGCContentResponseV2 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void PublicGetUserContentsV2(const FString& UserId
		, FDModelsUGCContentPageResponseV2 const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
