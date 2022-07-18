// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteError.h"
#include "ABReward.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsRewardInfo, FAccelByteModelsRewardInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsQueryReward, FAccelByteModelsQueryReward, Response);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABReward : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Reward | Api")
	void GetRewardByRewardCode(
		FString const& RewardCode,
		FDModelsRewardInfo const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Reward | Api")
	void GetRewardByRewardId(
		FString const& RewardId,
		FDModelsRewardInfo const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Reward | Api")
	void QueryRewards(
		FString const& EventTopic,
		int32 Offset,
		int32 Limit,
		EAccelByteRewardListSortBy const& SortBy,
		FDModelsQueryReward const& OnSuccess,
		FDErrorHandler const& OnError
	);

private:
	FApiClientPtr ApiClientPtr;
};
