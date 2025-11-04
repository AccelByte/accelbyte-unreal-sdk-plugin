// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteLeaderboardModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Leaderboard API to obtain player's ranking in a namespace.
 */
class ACCELBYTEUE4SDK_API Leaderboard 
	: public FApiBase
	, public TSharedFromThis<Leaderboard, ESPMode::ThreadSafe>
{
	static constexpr int32 LeaderboardUserIdsURLLimit = 20;

public:
	Leaderboard(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	Leaderboard(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~Leaderboard();

	/**
	 * @brief Get leaderboard rankings in a specified timeframe.
	 *
	 * @param LeaderboardCode Specify leaderboard code to get from the leaderboard.
	 * @param TimeFrame Specify the time frame of leaderboard.
	 * @param Offset Starting index of leaderboard rank. First index is 0.
	 * @param Limit Ranking displayed for each page.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsLeaderboardRankingResult.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetRankings(FString const& LeaderboardCode
		, EAccelByteLeaderboardTimeFrame const& TimeFrame
		, uint32 Offset
		, uint32 Limit
		, THandler<FAccelByteModelsLeaderboardRankingResult> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's ranking from leaderboard
	 *
	 * @param UserId Specify user ID to get from the leaderboard ranking list.
	 * @param LeaderboardCode Specify leaderboard code to get from the leaderboard.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserRankingData.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserRanking(FString const& UserId
		, FString const& LeaderboardCode
		, THandler<FAccelByteModelsUserRankingData> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief List all leaderboards in the current namespace
	 * 
	 * @param Offset The offset of leaderboard data result
	 * @param Limit The limit of leaderboard data result
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsPaginatedLeaderboardData
	 * @param OnError This will be called when the operation failed.
	 */
	void GetLeaderboards(uint32 Offset
		, uint32 Limit
		, THandler<FAccelByteModelsPaginatedLeaderboardData> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get the list of user ranking data in a leaderboard in a specific cycle
	 * 
	 * @param LeaderboardCode 
	 * @param CycleId 
	 * @param Offset The offset of leaderboard data result
	 * @param Limit The limit of leaderboard data result
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsLeaderboardRankingResultV3
	 * @param OnError This will be called when the operation failed
	 */
	void GetRankingByCycle(FString const& LeaderboardCode
		, FString const& CycleId
		, uint32 Offset
		, uint32 Limit
		, THandler<FAccelByteModelsLeaderboardRankingResultV3> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief List all leaderboards in the current namespace
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsPaginatedLeaderboardData
	 * @param OnError This will be called when the operation failed.
	 * @param Limit The limit of leaderboard data result
	 * @param Offset The offset of leaderboard data result
	 */
	void GetLeaderboardsV3(THandler<FAccelByteModelsPaginatedLeaderboardData> const& OnSuccess
		, FErrorHandler const& OnError
		, uint32 Limit = 20
		, uint32 Offset = 0);

	/**
	 * @brief Retrieve Leaderboard data for specific LeaderboardCode
	 *
	 * @param LeaderboardCode specified Leaderboard code to query
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsPaginatedLeaderboardData
	 * @param OnError This will be called when the operation failed.
	 */
	void GetLeaderboardV3(FString const& LeaderboardCode
		, THandler<FAccelByteModelsBaseLeaderboardData> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get the list of all time user ranking data in a leaderboard
	 *
	 * @param LeaderboardCode The leaderboard code
	 * @param Offset The offset of leaderboard data result
	 * @param Limit The limit of leaderboard data result
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsLeaderboardRankingResultV3
	 * @param OnError This will be called when the operation failed
	 */
	void GetRankingsV3(FString const& LeaderboardCode
		, uint32 Offset
		, uint32 Limit
		, THandler<FAccelByteModelsLeaderboardRankingResultV3> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get the list of user ranking data in a leaderboard in a specific cycle
	 *
	 * @param LeaderboardCode
	 * @param CycleId
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsLeaderboardRankingResultV3
	 * @param OnError This will be called when the operation failed
	 * @param Limit The limit of leaderboard data result
	 * @param Offset The offset of leaderboard data result
	 */
	void GetRankingByCycleV3(FString const& LeaderboardCode
		, FString const& CycleId
		, THandler<FAccelByteModelsLeaderboardRankingResultV3> const& OnSuccess
		, FErrorHandler const& OnError
		, uint32 Limit = 20
		, uint32 Offset = 0);

	/**
	 * @brief Get all time and cycles user ranking data in a leaderboard. 
	 * 
	 * @param UserId 
	 * @param LeaderboardCode 
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserRankingDataV3
	 * @param OnError This will be called when the operation failed
	 */
	void GetUserRankingV3(FString const& UserId
		, FString const& LeaderboardCode
		, THandler<FAccelByteModelsUserRankingDataV3> OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve bulk user ranking data for all time periods and cycles in a leaderboard.
	 *
	 * @param UserIds List of UserIds.
	 * @param LeaderboardCode The leaderboard code
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsBulkUserRankingDataV3
	 * @param OnError This will be called when the operation failed
	 */
	void GetBulkUserRankingV3(TArray<FString> const& UserIds
		, FString const& LeaderboardCode
		, THandler<FAccelByteModelsBulkUserRankingDataV3> OnSuccess
		, FErrorHandler const& OnError);

private:
	Leaderboard() = delete;
	Leaderboard(Leaderboard const&) = delete;
	Leaderboard(Leaderboard&&) = delete;
};

using LeaderboardRef = TSharedRef<Leaderboard, ESPMode::ThreadSafe>;
using LeaderboardPtr = TSharedPtr<Leaderboard, ESPMode::ThreadSafe>;
using LeaderboardWPtr = TWeakPtr<Leaderboard, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
