// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteChallengeModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{
	
/**
 * @brief Server Challenge APIs to manage user challenge progress
 */
class ACCELBYTEUE4SDK_API ServerChallenge : public FServerApiBase
{
public:
	ServerChallenge(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerChallenge();

	/**
	 * @brief Send a request to get current progress for a specific challenge from a specific user
	 *
	 * @param ChallengeCode String code for the challenge to check progress for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Offset Number of goals to skip when returning goal progress list, defaults to 0
	 * @param Limit Number of goals to include when returning goal progress list, defaults to 20
	 * @param GoalCode String code for the specific challenge goal to get progress for
	 * @param Tags Array of tag strings used to filter resulting challenge progress list
	 * @param DateTime Filter the result of query to progression before the specified date time, by default no specified dateTime. WARNING: cannot use DateTime from future, only past.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserChallengeProgress(const FString& UserId
		, FString const& ChallengeCode
		, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, uint64 Offset = 0
		, uint64 Limit = 20
		, FString const& GoalCode = TEXT("")
		, TArray<FString> const& Tags = {}
		, TOptional<FDateTime> DateTime = {});

	/**
	 * @brief Send a request to attempt to evaluate many user's challenge progress
	 *
	 * @param Request Structure containing the IDs of the users to evaluate challenge progress for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr EvaluateChallengeProgress(FAccelByteModelsChallengeServerEvaluateProgressRequest const& Request
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send a request to attempt to evaluate many user's challenge progress
	 *
	 * @param Request Structure containing the IDs of the users to evaluate challenge progress for
	 * @param OptionalParameter Struct containing optional parameters to specify the evaluation
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr EvaluateChallengeProgress(FAccelByteModelsChallengeServerEvaluateProgressRequest const& Request
		, FAccelByteModelsChallengeEvaluateProgressOptionalParameter const& OptionalParameter
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send a request to get status of all challenge rewards
	 * @param UserId UserId to lookup the reward
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * @param Status Determines what specific rewards should be included in the response, defaults to EAccelByteModelsChallengeRewardStatus::NONE to show all
	 * @param SortBy Determines the order in which the rewards in the response are returned
	 * @param Offset Number of rewards to skip when returning reward list, defaults to 0
	 * @param Limit Number of rewards to include when returning reward list, defaults to 20
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserRewards(const FString& UserId
		, THandler<FAccelByteModelsChallengeGetRewardStatusResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteModelsChallengeRewardStatus Status = EAccelByteModelsChallengeRewardStatus::NONE
		, EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC
		, uint64 Offset = 0
		, uint64 Limit = 20);

	/**
	 * @brief Send a request to claim rewards for a user
	 *
	 * @param Request Structure containing UserID and IDs of rewards to claim
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ClaimReward(FAccelByteModelsChallengeRewardClaimUserRequest const& Request
		, THandler<TArray<FAccelByteModelsChallengeReward>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Send a request to claim rewards for multiple users
	 *
	 * @param Request Array of rewards to claim for each user
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ClaimReward(TArray<FAccelByteModelsChallengeRewardClaimUserRequest> const& Request
		, THandler<TArray<FAccelByteModelsChallengeRewardClaimUserResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerChallenge() = delete;
	ServerChallenge(const ServerChallenge&) = delete;
	ServerChallenge(ServerChallenge&&) = delete;
};
}
}
