// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSessionModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{
/**
 * @brief Provide APIs to access SessionHistory service.
 */
class ACCELBYTEUE4SDK_API SessionHistory : public FApiBase, public TSharedFromThis<SessionHistory, ESPMode::ThreadSafe>
{
public:
	SessionHistory(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~SessionHistory();

#pragma region SessionHistory

	/**
	 * @brief Retrieve game session history for the current user.
	 *
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsGameSessionHistoriesResult.
	 * @param OnError A delegate executed when the request fails.
	 * @param SortBy The criteria for sorting the list of game sessions. Possible values: "Ascending" & "Descending". Default: "Descending".
	 * @param Offset The page number to retrieve. Default: 0.
	 * @param Limit The number of items per page to retrieve. Default: 20.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryGameSessionHistory(THandler<FAccelByteModelsGameSessionHistoriesResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteGeneralSortBy const& SortBy = EAccelByteGeneralSortBy::DESC
		, int32 Offset = 0
		, int32 Limit = 20);

#pragma endregion

private:
	SessionHistory() = delete;
	SessionHistory(SessionHistory const&) = delete;
	SessionHistory(SessionHistory&&) = delete;
};

typedef TSharedRef<SessionHistory, ESPMode::ThreadSafe> SessionHistoryRef;
typedef TSharedPtr<SessionHistory, ESPMode::ThreadSafe> SessionHistoryPtr;
typedef TWeakPtr<SessionHistory, ESPMode::ThreadSafe> SessionHistoryWPtr;

} // Namespace Api
} // Namespace AccelByte