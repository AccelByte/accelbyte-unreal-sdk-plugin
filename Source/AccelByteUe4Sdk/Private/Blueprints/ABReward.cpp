// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABReward.h"

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
	ApiClientPtr->Reward.GetRewardByRewardCode(
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

void UABReward::GetRewardByRewardId(
	FString const& RewardId,
	FDModelsRewardInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->Reward.GetRewardByRewardId(
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

void UABReward::QueryRewards(
	FString const& EventTopic,
	int32 Offset,
	int32 Limit,
	EAccelByteRewardListSortBy const& SortBy,
	FDModelsQueryReward const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->Reward.QueryRewards(
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