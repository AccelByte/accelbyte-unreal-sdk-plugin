// Copyright (c) 2024 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABChallenge.h"

using namespace AccelByte;

void UABChallenge::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABChallenge::GetChallenges(
	FDModelsGetChallengesResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	EAccelByteModelsChallengeSortBy SortBy, 
	EAccelByteModelsChallengeStatus Status, 
	int64 Offset, 
	int64 Limit)
{
	ApiClientPtr->Challenge.GetChallenges(
		THandler<FAccelByteModelsGetChallengesResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsGetChallengesResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		SortBy,
		Status,
		Offset,
		Limit
	);
}

void UABChallenge::GetScheduledChallengeGoals(
	FString const& ChallengeCode,
	FDModelsGetScheduledChallengeGoalsResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	TArray<FString> const& OptionalTags, 
	int64 Offset, 
	int64 Limit)
{
	ApiClientPtr->Challenge.GetScheduledChallengeGoals(
		ChallengeCode,
		THandler<FAccelByteModelsGetScheduledChallengeGoalsResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsGetScheduledChallengeGoalsResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		OptionalTags,
		Offset,
		Limit
	);
}

void UABChallenge::GetPreviousChallengeProgresses(
	FString const& ChallengeCode, 
	int32 Index,
	FDModelsChallengeProgressResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	TArray<FString> const& OptionalTags,
	FString const& GoalCode, 
	int64 Offset, 
	int64 Limit)
{
	ApiClientPtr->Challenge.GetPreviousChallengeProgresses(
		ChallengeCode,
		Index,
		THandler<FAccelByteModelsChallengeProgressResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsChallengeProgressResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		GoalCode,
		Offset,
		Limit,
		OptionalTags
	);
}

void UABChallenge::GetChallengeProgress(
	FString const& ChallengeCode, 
	FString const& GoalCode,
	FDModelsChallengeProgressResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	TArray<FString> const& OptionalTags,
	int64 Offset, 
	int64 Limit)
{
	ApiClientPtr->Challenge.GetChallengeProgress(
		ChallengeCode,
		GoalCode,
		THandler<FAccelByteModelsChallengeProgressResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsChallengeProgressResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Offset,
		Limit,
		OptionalTags
	);
}

void UABChallenge::GetRewards(
	FDModelsChallengeGetRewardStatusResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	EAccelByteModelsChallengeRewardStatus Status, 
	EAccelByteModelsChallengeSortBy SortBy, 
	int64 Offset, 
	int64 Limit)
{
	ApiClientPtr->Challenge.GetRewards(
		THandler<FAccelByteModelsChallengeGetRewardStatusResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsChallengeGetRewardStatusResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Status,
		SortBy,
		Offset,
		Limit
	);
}

void UABChallenge::ClaimReward(
	FAccelByteModelsChallengeRewardClaimRequest const& Request, 
	FDModelsChallengeRewardsResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Challenge.ClaimReward(
		Request,
		THandler<TArray<FAccelByteModelsChallengeReward>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsChallengeReward> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABChallenge::EvaluateChallengeProgress(
	FDHandler const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Challenge.EvaluateChallengeProgress(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}
