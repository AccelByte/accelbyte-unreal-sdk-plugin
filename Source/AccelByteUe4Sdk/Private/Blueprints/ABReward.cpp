// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABReward.h"

using namespace AccelByte;

void UABReward::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABReward::GetRewardByRewardCode(
	FString const& RewardCode,
	FDModelsRewardInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto RewardPtr = ApiClientPtr->GetRewardApi().Pin();
	if (RewardPtr.IsValid())
	{
		RewardPtr->GetRewardByRewardCode(
		RewardCode,
		THandler<FAccelByteModelsRewardInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsRewardInfo const& Response)
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABReward::GetRewardByRewardId(
	FString const& RewardId,
	FDModelsRewardInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto RewardPtr = ApiClientPtr->GetRewardApi().Pin();
	if (RewardPtr.IsValid())
	{
		RewardPtr->GetRewardByRewardId(
		RewardId,
		THandler<FAccelByteModelsRewardInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsRewardInfo const& Response)
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABReward::QueryRewards(
	FString const& EventTopic,
	int32 Offset,
	int32 Limit,
	EAccelByteRewardListSortBy const& SortBy,
	FDModelsQueryReward const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto RewardPtr = ApiClientPtr->GetRewardApi().Pin();
	if (RewardPtr.IsValid())
	{
		RewardPtr->QueryRewards(
		EventTopic,
		Offset,
		Limit,
		SortBy,
		THandler<FAccelByteModelsQueryReward>::CreateLambda(
			[OnSuccess](FAccelByteModelsQueryReward const& Response)
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}