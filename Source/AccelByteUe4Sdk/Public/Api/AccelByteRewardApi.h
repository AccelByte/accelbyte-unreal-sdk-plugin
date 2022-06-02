// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Reward API for getting information about the reward
 */	
class ACCELBYTEUE4SDK_API Reward
{
public:
	Reward(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Reward();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
public:
	/**
	 * @brief Get Reward from a specific Reward Code
	 *
	 * @param RewardCode The RewardCode.
	 * @param OnSuccess This will be called when operation succeeded. The result is const FAccelByteModelsRewardInfoByCode&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetRewardByRewardCode(FString const& RewardCode, THandler<FAccelByteModelsRewardInfo> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get Reward from a specific Reward Id
	 *
	 * @param RewardId The RewardId.
	 * @param OnSuccess This will be called when operation suceeded. The result is const FAccelByteModelsRewardInfoByCode&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetRewardByRewardId(FString const& RewardId, THandler<FAccelByteModelsRewardInfo> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Query all Reward from a specific Event Topic
	 *
	 * @param  Offset Offset of the data limit.
	 * @param Limit Limit of the data that gonna get.
	 * @param SortBy SortBy namespace, namespace:asc, namespace:desc, rewardcode, rewardcode:asc, rewardcode:desc
	 */
	void QueryRewards(FString const& EventTopic, int32 Offset, int32 Limit, EAccelByteRewardListSortBy const& SortBy,THandler<FAccelByteModelsQueryReward> const& OnSuccess, FErrorHandler const& OnError);
	
private:
	Reward() = delete;
	Reward(Reward const&) = delete;
	Reward(Reward&&) = delete;
	
	FString ConvertRewardSortByToString(EAccelByteRewardListSortBy const& SortBy);
};
	
} // Namespace Api
} // Namespace AccelByte
