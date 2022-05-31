// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
{
public:
	Leaderboard(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Leaderboard();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
public:
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
	void GetRankings(FString const& LeaderboardCode, EAccelByteLeaderboardTimeFrame const& TimeFrame, uint32 Offset, uint32 Limit, THandler<FAccelByteModelsLeaderboardRankingResult> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get user's ranking from leaderboard
	*
	* @param UserId Specify user ID to get from the leaderboard ranking list.
	* @param LeaderboardCode Specify leaderboard code to get from the leaderboard.
	* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserRankingData.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserRanking(FString const& UserId, FString const& LeaderboardCode, THandler<FAccelByteModelsUserRankingData> const& OnSuccess, FErrorHandler const& OnError);

private:
	Leaderboard() = delete;
	Leaderboard(Leaderboard const&) = delete;
	Leaderboard(Leaderboard&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
