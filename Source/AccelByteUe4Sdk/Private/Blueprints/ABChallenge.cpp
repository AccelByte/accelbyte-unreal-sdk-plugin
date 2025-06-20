// Copyright (c) 2024 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABChallenge.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteChallenge, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteChallenge);

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
	const auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->GetChallenges(
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
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
}

void UABChallenge::GetChallengeList(
	FDModelsGetChallengesResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	TArray<FString> const& OptionalTags,
	EAccelByteModelsChallengeSortBy SortBy,
	EAccelByteModelsChallengeStatus Status,
	int64 Offset,
	int64 Limit,
	FString Keyword)
{
	auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->GetChallenges(
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
			Limit,
			Keyword,
			OptionalTags
		);
	}
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
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
	const auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->GetPreviousChallengeProgresses(
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
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
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
	const auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->GetChallengeProgress(
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
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
}

void UABChallenge::GetRewards(
	FDModelsChallengeGetRewardStatusResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteModelsChallengeRewardStatus Status,
	EAccelByteModelsChallengeSortBy SortBy,
	int64 Offset,
	int64 Limit)
{
	const auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->GetRewards(
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
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
}

void UABChallenge::ClaimReward(
	FAccelByteModelsChallengeRewardClaimRequest const& Request,
	FDModelsChallengeRewardsResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->ClaimReward(
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
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
}

void UABChallenge::EvaluateChallengeProgress(
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto ChallengePtr = ApiClientPtr->GetChallengeApi().Pin();
	if (ChallengePtr.IsValid())
	{
		ChallengePtr->EvaluateChallengeProgress(
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
	else
	{
		UE_LOG(LogAccelByteChallenge, Warning, TEXT("Invalid Challenge API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Challenge API already destroyed!"));
	}
}
