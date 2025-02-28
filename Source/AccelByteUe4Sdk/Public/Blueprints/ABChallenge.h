// Copyright (c) 2024 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteChallengeModels.h"
#include "ABChallenge.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsGetChallengesResponse, FAccelByteModelsGetChallengesResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsGetScheduledChallengeGoalsResponse, FAccelByteModelsGetScheduledChallengeGoalsResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsChallengeProgressResponse, FAccelByteModelsChallengeProgressResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsChallengeGetRewardStatusResponse, FAccelByteModelsChallengeGetRewardStatusResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsChallengeRewardsResponse, TArray<FAccelByteModelsChallengeReward>, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABChallenge : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

#pragma region V1 Challenge

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void GetChallenges(
		FDModelsGetChallengesResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC,
		EAccelByteModelsChallengeStatus Status = EAccelByteModelsChallengeStatus::NONE,
		int64 Offset = 0,
		int64 Limit = 20
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void GetChallengeList(
		FDModelsGetChallengesResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		TArray<FString> const& OptionalTags, // Note: This is supposed to be optional, but the blueprint can't parse an array when a default value is provided.
		EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC,
		EAccelByteModelsChallengeStatus Status = EAccelByteModelsChallengeStatus::NONE,
		int64 Offset = 0,
		int64 Limit = 20,
		FString Keyword = TEXT("")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void GetScheduledChallengeGoals(
		FString const& ChallengeCode,
		FDModelsGetScheduledChallengeGoalsResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		TArray<FString> const& OptionalTags, // Note: This is supposed to be optional, but the blueprint can't parse an array when a default value is provided.
		int64 Offset = 0,
		int64 Limit = 20
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void GetPreviousChallengeProgresses(
		FString const& ChallengeCode,
		int32 Index,
		FDModelsChallengeProgressResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		TArray<FString> const& OptionalTags, // Note: This is supposed to be optional, but the blueprint can't parse an array when a default value is provided.
		FString const& GoalCode = TEXT(""),
		int64 Offset = 0,
		int64 Limit = 20
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void GetChallengeProgress(
		FString const& ChallengeCode,
		FString const& GoalCode,
		FDModelsChallengeProgressResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		TArray<FString> const& OptionalTags, // Note: This is supposed to be optional, but the blueprint can't parse an array when a default value is provided.
		int64 Offset = 0,
		int64 Limit = 20
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void GetRewards(
		FDModelsChallengeGetRewardStatusResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteModelsChallengeRewardStatus Status = EAccelByteModelsChallengeRewardStatus::NONE,
		EAccelByteModelsChallengeSortBy SortBy = EAccelByteModelsChallengeSortBy::UPDATED_AT_DESC,
		int64 Offset = 0,
		int64 Limit = 20
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void ClaimReward(
		FAccelByteModelsChallengeRewardClaimRequest const& Request,
		FDModelsChallengeRewardsResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Challenge | Api")
	void EvaluateChallengeProgress(
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError
	);

#pragma endregion

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};