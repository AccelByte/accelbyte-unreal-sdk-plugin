// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteAchievementModels.h"
#include "ABAchievement.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedPublicAchievementResponse, FAccelByteModelsPaginatedPublicAchievement, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsMultiLanguageAchievementResponse, FAccelByteModelsMultiLanguageAchievement, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedUserAchievementResponse, FAccelByteModelsPaginatedUserAchievement, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABAchievement : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api")
	void QueryAchievements(
		FString const& Language,
		EAccelByteAchievementListSortBy const& SortBy,
		FDModelsPaginatedPublicAchievementResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 const& Offset = 0,
		int32 const& Limit = 20
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api")
	void GetAchievement(
		FString const& AchievementCode,
		FDModelsMultiLanguageAchievementResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api")
	void QueryUserAchievements(
		EAccelByteAchievementListSortBy const& SortBy,
		FDModelsPaginatedUserAchievementResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 const& Offset = 0,
		int32 const& Limit = 20,
		bool PreferUnlocked = true
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api")
	void UnlockAchievement(
		FString const& AchievementCode,
		FDHandler const OnSuccess,
		FDErrorHandler const& OnError
	);

private:
	FApiClientPtr ApiClientPtr;
};
