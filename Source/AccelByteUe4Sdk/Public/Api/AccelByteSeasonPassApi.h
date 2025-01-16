// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSeasonPassModels.h"


namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{

class ServerSeasonPass;
/**
 * @brief Season Pass APIs to access Season Pass service. User can obtaining season and claiming rewards.
 */
class ACCELBYTEUE4SDK_API SeasonPass : public FApiBase, public TSharedFromThis<SeasonPass, ESPMode::ThreadSafe>
{
public:
	SeasonPass(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~SeasonPass();

	/**
	 * @brief Get current active season.
	 *
	 * @param Language The language of the Season.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCurrentSeason(FString const& Language
		, THandler<FAccelByteModelsSeasonInfo> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Get user season data by SeasonId
	 *
	 * @param SeasonId The Id of the Season.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserSeason(FString const& SeasonId
		, THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Get current active user season data.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Claim Season Rewards.
	 *
	 * @param RewardRequest Detail information for the Reward Request.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonClaimRewardResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ClaimRewards(FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest
		, THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Bulk claim season rewards.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonClaimRewardResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkClaimRewards(THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	/**
	 * @brief Utility to convert Claim Reward Json Object to TMap<int32, TMap<FString, TArray<FString>>>
	 *
	 * @param JsonObject Input of JsonObject to be converted
	 * @return TMap<FString, TArray<FString>> TMap as Passes Rewards (Key as Pass Code, Value as Array of Reward Codes)
	 */
	static TMap<FString, TArray<FString>> FJsonObjectToPassRewards(TSharedPtr<FJsonObject> const& JsonObject);

	SeasonPass() = delete;
	SeasonPass(SeasonPass const&) = delete;
	SeasonPass(SeasonPass&&) = delete;
};

typedef TSharedRef<SeasonPass, ESPMode::ThreadSafe> SeasonPassRef;
typedef TSharedPtr<SeasonPass, ESPMode::ThreadSafe> SeasonPassPtr;
typedef TWeakPtr<SeasonPass, ESPMode::ThreadSafe> SeasonPassWPtr;

};
}
