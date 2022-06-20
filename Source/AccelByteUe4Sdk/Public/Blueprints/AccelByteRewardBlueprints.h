// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteRewardBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsReward : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetRewardByRewardCodeSuccess, const FAccelByteModelsRewardInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Reward | Api")
	static void GetRewardByRewardCode(const FString& RewardCode, const FGetRewardByRewardCodeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetRewardByRewardIdSuccess, const FAccelByteModelsRewardInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Reward | Api")
	static void GetRewardByRewardId(const FString& RewardId, const FGetRewardByRewardIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryRewardsSuccess, const FAccelByteModelsQueryReward&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Reward | Api")
	static void QueryRewards(const FString& EventTopic, int32 Offset, int32 Limit, const EAccelByteRewardListSortBy& SortBy, const FQueryRewardsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};
