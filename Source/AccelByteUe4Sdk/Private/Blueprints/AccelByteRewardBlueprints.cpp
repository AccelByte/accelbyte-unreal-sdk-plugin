// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteRewardBlueprints.h"

#include "Api/AccelByteRewardApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Reward;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsReward::GetRewardByRewardCode(const FString& RewardCode, const FGetRewardByRewardCodeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Reward.GetRewardByRewardCode(RewardCode, THandler<FAccelByteModelsRewardInfo>::CreateLambda([OnSuccess](const FAccelByteModelsRewardInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsReward::GetRewardByRewardId(const FString& RewardId, const FGetRewardByRewardIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Reward.GetRewardByRewardId(RewardId, THandler<FAccelByteModelsRewardInfo>::CreateLambda([OnSuccess](const FAccelByteModelsRewardInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsReward::QueryRewards(const FString& EventTopic, int32 Offset, int32 Limit, const EAccelByteRewardListSortBy& SortBy, const FQueryRewardsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Reward.QueryRewards(EventTopic, Offset, Limit, SortBy, THandler<FAccelByteModelsQueryReward>::CreateLambda([OnSuccess](const FAccelByteModelsQueryReward& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
