// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteAchievementModels.h"
#include "AccelByteAchievementBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsAchievement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryAchievementsSuccess, const FAccelByteModelsPaginatedPublicAchievement&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryAchievements(const FString& Language, const EAccelByteAchievementListSortBy& SortBy, const FQueryAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAchievementSuccess, const FAccelByteModelsMultiLanguageAchievement&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void GetAchievement(const FString& AchievementCode, const FGetAchievementSuccess& OnSuccess, const FDErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryUserAchievementsSuccess, const FAccelByteModelsPaginatedUserAchievement&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy, const FQueryUserAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void UnlockAchievement(const FString& AchievementCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError);
};
