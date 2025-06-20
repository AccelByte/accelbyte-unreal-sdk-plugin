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
class ACCELBYTEUE4SDK_API Challenge : public FApiBase, public TSharedFromThis<Challenge, ESPMode::ThreadSafe>
{
public:
	Challenge(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);

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
	 * @param Keyword String challenge code for the specific challenge list
	 * @param Tags Array of tag strings used to filter resulting challenge list
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetChallenges(THandler<FAccelByteModelsGetChallengesResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC
		, EAccelByteModelsChallengeStatus Status = EAccelByteModelsChallengeStatus::NONE
		, uint64 Offset = 0
		, uint64 Limit = 20
		, FString Keyword = TEXT("")
		, TArray<FString> const& Tags = {});

	/**
	 * @brief Send a request to get previous challenge progresses
	 *
	 * @param ChallengeCode String code for the challenge to check progress for
	 * @param Index A positive integer indicating the index of the rotation. 
	    The rotations are sorted by their start time in descending order. 
		A value of 0 refers to the current or most recent active rotation. 
		Incrementing the value allows you to reference previous rotations.
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param GoalCode String code for the specific challenge goal to get progress for
	 * @param Offset Number of goals to skip when returning goal progress list, defaults to 0
	 * @param Limit Number of goals to include when returning goal progress list, defaults to 20
	 * @param Tags Array of tag strings used to filter resulting challenge progress list
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPreviousChallengeProgresses(FString const& ChallengeCode
		, int32 Index
		, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& GoalCode = TEXT("")
		, uint64 Offset = 0
		, uint64 Limit = 20
		, TArray<FString> const& Tags = {});

	/**
	 * @brief Send a request to get current progress for a specific challenge
	 *
	 * @param ChallengeCode String code for the challenge to check progress for
	 * @param GoalCode String code for the specific challenge goal to get progress for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Offset Number of goals to skip when returning goal progress list, defaults to 0
	 * @param Limit Number of goals to include when returning goal progress list, defaults to 20
	 * @param Tags Array of tag strings used to filter resulting challenge progress list
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetChallengeProgress(FString const& ChallengeCode
		, FString const& GoalCode
		, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, uint64 Offset = 0
		, uint64 Limit = 20
		, TArray<FString> const& Tags = {});

	/**
	 * @brief Send a request to get current progress for a specific challenge
	 *
	 * @param ChallengeCode String code for the challenge to check progress for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Offset Number of goals to skip when returning goal progress list, defaults to 0
	 * @param Limit Number of goals to include when returning goal progress list, defaults to 20
	 * @param GoalCode String code for the specific challenge goal to get progress for
	 * @param Tags Array of tag strings used to filter resulting challenge progress list
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetChallengeProgress(FString const& ChallengeCode
		, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, uint64 Offset = 0
		, uint64 Limit = 20
		, FString const& GoalCode = TEXT("")
		, TArray<FString> const& Tags = {});

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

	/**
	 * @brief Send a request to query the List schedules of given challenge
	 * 
	 * @param ChallengeCode String code for the challenge to check the schedules for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param DateTime Optional parameter to query schedules prior to specific date time
	 * @param Offset Number of rewards to skip when returning reward list, defaults to 0
	 * @param Limit Number of rewards to include when returning reward list, defaults to 20
	 */
	FAccelByteTaskWPtr ListSchedules(FString ChallengeCode
		, THandler<FAccelByteModelsChallengeListSchedulesResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FDateTime const& DateTime = FDateTime::MinValue()
		, uint64 Offset = 0
		, uint64 Limit = 20);

	/**
	 * @brief Send a request to query the List schedules of given goal in a challenge
	 *
	 * @param ChallengeCode String code for the challenge to check the schedules for
	 * @param GoalCode String code for the specific challenge goal to check the schedules for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Offset Number of rewards to skip when returning reward list, defaults to 0
	 * @param Limit Number of rewards to include when returning reward list, defaults to 20
	 */
	FAccelByteTaskWPtr ListScheduleByGoal(FString ChallengeCode
		, FString GoalCode
		, THandler<FAccelByteModelsChallengeListScheduleByGoalResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, uint64 Offset = 0
		, uint64 Limit = 20);

private:
	Challenge() = delete;
	Challenge(Challenge const&) = delete;
	Challenge(Challenge&&) = delete;

	static FString CreateTagsString(const TArray<FString>& Tags);
};
	
typedef TSharedRef<Challenge, ESPMode::ThreadSafe> ChallengeRef;
typedef TSharedPtr<Challenge, ESPMode::ThreadSafe> ChallengePtr;
typedef TWeakPtr<Challenge, ESPMode::ThreadSafe> ChallengeWPtr;

} // Namespace Api
} // Namespace AccelByte
