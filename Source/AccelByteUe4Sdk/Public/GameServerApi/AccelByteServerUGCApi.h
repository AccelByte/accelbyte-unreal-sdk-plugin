// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteUGCModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{
#ifndef MAX_BULK_CONTENT_IDS_COUNT
#define MAX_BULK_CONTENT_IDS_COUNT 100
#endif // !MAX_BULK_CONTENT_IDS_COUNT
#ifndef MAX_BULK_CONTENT_SHARECODES_COUNT
#define MAX_BULK_CONTENT_SHARECODES_COUNT 100
#endif // !MAX_BULK_CONTENT_SHARECODES_COUNT

/**
 * @brief Provide APIs to access UGC service.
 */
class ACCELBYTEUE4SDK_API ServerUGC : public FServerApiBase
{
public:
	ServerUGC(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler & InHttpRef);
	~ServerUGC();

	/**
	 * @brief Search all available contents based on the given filter.
	 *
	 * @param Request Filter request to specify the search result.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 20
	 * @param Offset The offset number to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Search contents specific to a channel by channel id.
	 *
	 * @param ChannelId Channel Id.
	 * @param Request Filter request to specify the search result.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 20
	 * @param Offset The offset number to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SearchContentsSpecificToChannel(FString const& ChannelId
		, FAccelByteModelsUGCSearchContentsRequest const& Request
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Modify existing content to update some information by share code.
	 *
	 * @param UserId The user id who modify the Content.
	 * @param ChannelId The id of the content's channel.
	 * @param ShareCode The share code of the content that will be fetched.
	 * @param ModifyRequest Detail information for the content request that will be modified.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ModifyContentByShareCode(FString const& UserId
		, FString const& ChannelId
		, FString const& ShareCode
		, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
		, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a content based on the its channel id and share code.
	 *
	 * @param UserId The user id who delete the Content.
	 * @param ChannelId The id of the content's channel.
	 * @param ShareCode The share code of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteContentByShareCode(FString const& UserId
		, FString const& ChannelId
		, FString const& ShareCode
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a content information by its content id. Can be used without logged in.
	 *
	 * @param ContentId The id of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetContentByContentId(FString const& ContentId
		, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a content information by its share code. Can be used without logged in.
	 *
	 * @param ShareCode The share code of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetContentByShareCode(FString const& ShareCode
		, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Search specific contents based on the given filter. Can be used without logged in.
	 *
	 * @param Name Content Name.
	 * @param Creator Creator Name.
	 * @param Type Content Type.
	 * @param Subtype Content Subtype.
	 * @param Tags Content Tags.
	 * @param IsOfficial Filter only official contents
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy Sorting criteria, name,download,like,date. default=date.
	 * @param OrderBy Sorting order: asc, desc. default=desc
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicSearchContents(FString const& Name
		, FString const& Creator
		, FString const& Type
		, FString const& Subtype
		, TArray<FString> const& Tags
		, bool IsOfficial
		, FString const& UserId
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUgcSortBy SortBy = EAccelByteUgcSortBy::DATE
		, EAccelByteUgcOrderBy OrderBy = EAccelByteUgcOrderBy::DESC
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Search specific contents based on the given filter. Can be used without logged in.
	 *
	 * @param Request Filter request to specify the search result.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 20
	 * @param Offset The offset number to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicSearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Get contents by content Ids. Can be used without logged in.
	 *
	 * @param ContentIds Content Ids Array  
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetContentBulk(TArray<FString> const& ContentIds
		, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's generated contents. Can be used without logged in.
	 *
	 * @param UserId User Id 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetUserContent(FString const& UserId
		, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 1000
		, int32 Offset = 0);

	/**
	 * @brief Search contents specific to a channel. Can be used without logged in.
	 *
	 * @param ChannelId Channel Id.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCSearchContentsPagingResponseV2.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 20
	 * @param Offset The offset number to retrieve. Default value : 0
	 * @param SortBy Sorting criteria: created time with asc or desc. default = created time and desc.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicSearchContentsSpecificToChannelV2(FString const& ChannelId
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteUGCContentUtilitiesSortByV2 SortBy = EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_DESC);

	/**
	 * @brief Get all contents in current namespace. Can be used without logged in.
	 *
	 * @param Filter To filter the returned UGC contets.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCSearchContentsPagingResponseV2.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 20
	 * @param Offset The offset number to retrieve. Default value : 0
	 * @param SortBy Sorting criteria: name, download, like, created time with asc or desc. default = created time and desc.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicSearchContentsV2(FAccelByteModelsUGCFilterRequestV2 const& Filter
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteUGCContentSortByV2 SortBy = EAccelByteUGCContentSortByV2::CREATED_TIME_DESC);

	/**
	 * @brief Get contents by content Ids. Can be used without logged in.
	 *
	 * @param ContentIds Content Ids Array
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsUGCContentResponseV2>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetContentBulkByIdsV2(TArray<FString> const& ContentIds
		, THandler<TArray<FAccelByteModelsUGCContentResponseV2>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a content information by its share code. Can be used without logged in.
	 *
	 * @param ShareCode The share code of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponseV2.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetContentByShareCodeV2(FString const& ShareCode
		, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a content information by its content id. Can be used without logged in.
	 *
	 * @param ContentId The id of the content that will be fetched.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCContentResponseV2.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetContentByContentIdV2(FString const& ContentId
		, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's generated contents. Can be used without logged in.
	 *
	 * @param UserId User Id
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUGCSearchContentsPagingResponseV2.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Number of content per page. Default value : 20
	 * @param Offset The offset number to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PublicGetUserContentsV2(FString const& UserId
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteUGCContentUtilitiesSortByV2 SortBy = EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_DESC);

private:
	ServerUGC() = delete;
	ServerUGC(ServerUGC const&) = delete;
	ServerUGC(ServerUGC&&) = delete;

	void InternalSearchContents(FString const& Name
		, FString const& Creator
		, FString const& Type
		, FString const& Subtype
		, TArray<FString> const& Tags
		, bool IsOfficial
		, FString const& UserId
		, EAccelByteUgcSortBy SortBy
		, EAccelByteUgcOrderBy OrderBy
		, int32 Limit
		, int32 Offset
		, FString& OutUrl
		, TMultiMap<FString, FString>& OutQueryParams);

	void InternalSearchContentsV2(FAccelByteModelsUGCFilterRequestV2 const& Filter
		, int32 Limit
		, int32 Offset
		, EAccelByteUGCContentSortByV2 SortBy
		, FString& OutUrl
		, TMap<FString, FString>& OutQueryParams);

	const FString GetNamespace();
	const TMap<FString, FString> GetDefaultHeaders();
};

} // Namespace GameServerApi
} // Namespace AccelByte
