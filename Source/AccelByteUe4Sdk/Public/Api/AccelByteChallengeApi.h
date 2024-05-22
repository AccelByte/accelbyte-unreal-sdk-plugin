// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteApiBase.h"
#include "Models/AccelByteChallengeModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Interface for interacting with the AccelByte challenge service
 */	
class ACCELBYTEUE4SDK_API Challenge : public FApiBase
{
public:
	Challenge(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef);

	~Challenge();

	/**
	 * @brief Send a request to get all challenges.
	 *
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param SortBy Determines the returned sort order of the challenges, defaults to EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC
	 * @param Status Determines how the returned challenge list should be filtered, defaults to EAccelByteModelsChallengeStatus::NONE
	 * @param Offset Number of challenges to skip when returning the challenge list, defaults to 0
	 * @param Limit Number of challenges that should be included in the challenge list, defaults to 20
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetChallenges(THandler<FAccelByteModelsGetChallengesResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC
		, EAccelByteModelsChallengeStatus Status = EAccelByteModelsChallengeStatus::NONE
		, uint64 Offset = 0
		, uint64 Limit = 20);

	/**
	 * @brief Send a request to get all scheduled goals for a specific challenge
	 *
	 * @param ChallengeCode String code for the challenge to check scheduled goals for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate excecuted when request fails
	 * @param Tags Array of tag strings used to filter resulting goals list
	 * @param Offset Number of scheduled goals to skip when returning goals list, defaults to 0
	 * @param Limit Number of scheduled goals that should be included in the goals list, defaults to 20
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetScheduledChallengeGoals(FString const& ChallengeCode
		, THandler<FAccelByteModelsGetScheduledChallengeGoalsResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, TArray<FString> Tags = {}
		, uint64 Offset = 0
		, uint64 Limit = 20);

	/**
	 * @brief Send a request to get current progress for a specific challenge
	 *
	 * @param ChallengeCode String code for the challenge to check progress for
	 * @param GoalCode String code for the specific challenge goal to get progress for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Offset Number of goals to skip when returning goal progress list, defaults to 0
	 * @param Limit Number of goals to include when returning goal progress list, defaults to 20
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetChallengeProgress(FString const& ChallengeCode
		, FString const& GoalCode
		, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, uint64 Offset = 0
		, uint64 Limit = 20);

	/**
	 * @brief Send a request to get status of all challenge rewards
	 *
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Status Determines what specific rewards should be included in the response, defaults to EAccelByteModelsChallengeRewardStatus::NONE to show all
	 * @param SortBy Determines the order in which the rewards in the response are returned
	 * @param Offset Number of rewards to skip when returning reward list, defaults to 0
	 * @param Limit Number of rewards to include when returning reward list, defaults to 20
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetRewards(THandler<FAccelByteModelsChallengeGetRewardStatusResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteModelsChallengeRewardStatus Status = EAccelByteModelsChallengeRewardStatus::NONE
		, EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC
		, uint64 Offset = 0
		, uint64 Limit = 20);

	/**
	 * @brief Send a request to claim a single reward
	 *
	 * @param Request Structure containing IDs of rewards to claim
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ClaimReward(FAccelByteModelsChallengeRewardClaimRequest const& Request
		, THandler<TArray<FAccelByteModelsChallengeReward>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Send a request to attempt to evaluate current progress toward all active challenges
	 *
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr EvaluateChallengeProgress(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	Challenge() = delete;
	Challenge(Challenge const&) = delete;
	Challenge(Challenge&&) = delete;

};
	
} // Namespace Api
} // Namespace AccelByte
