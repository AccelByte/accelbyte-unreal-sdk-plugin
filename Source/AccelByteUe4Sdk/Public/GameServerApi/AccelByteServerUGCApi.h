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
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */
	void SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
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
	 * @param Limit Number of content per page. Default value : 1000
	 * @param Offset The offset number to retrieve. Default value : 0
	 */
	void SearchContentsSpecificToChannel(FString const& ChannelId
		, FAccelByteModelsUGCSearchContentsRequest const& Request
		, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

private:
	ServerUGC() = delete;
	ServerUGC(ServerUGC const&) = delete;
	ServerUGC(ServerUGC&&) = delete;

	static FString ConvertUGCSortByToString(const EAccelByteUgcSortBy& SortBy);
	static FString ConvertUGCOrderByToString(const EAccelByteUgcOrderBy& OrderBy);
};

} // Namespace GameServerApi
} // Namespace AccelByte
