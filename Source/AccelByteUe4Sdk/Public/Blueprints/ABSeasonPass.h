// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteMultiRegistry.h"
#include "Models/AccelByteSeasonPassModels.h"
#include "ABSeasonPass.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSeasonInfo, FAccelByteModelsSeasonInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserSeasonInfo, FAccelByteModelsUserSeasonInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSeasonClaimRewardResponse, FAccelByteModelsSeasonClaimRewardResponse, Response);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABSeasonPass : public UObject
{
	GENERATED_BODY()

public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SeasonPass | Api")
	void GetCurrentSeason(
		FString const& Language,
		FDModelsSeasonInfo const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SeasonPass | Api")
	void GetUserSeason(
		FString const& SeasonId,
		FDModelsUserSeasonInfo const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SeasonPass | Api")
	void GetCurrentUserSeason(
		FDModelsUserSeasonInfo const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SeasonPass | Api")
	void ClaimRewards(
		FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest,
		FDModelsSeasonClaimRewardResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | SeasonPass | Api")
	void BulkClaimRewards(
		FDModelsSeasonClaimRewardResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

private:
	FApiClientPtr ApiClientPtr;
};
