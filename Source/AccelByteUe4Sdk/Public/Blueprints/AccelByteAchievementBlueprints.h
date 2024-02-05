// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteAchievementModels.h"
#include "AccelByteAchievementBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsAchievement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryAchievementsSuccess, const FAccelByteModelsPaginatedPublicAchievement&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryAchievements(const FString& Language, const EAccelByteAchievementListSortBy& SortBy, const FQueryAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit, const FString& TagQuery, bool bGlobal);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAchievementSuccess, const FAccelByteModelsMultiLanguageAchievement&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void GetAchievement(const FString& AchievementCode, const FGetAchievementSuccess& OnSuccess, const FDErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryUserAchievementsSuccess, const FAccelByteModelsPaginatedUserAchievement&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy, const FQueryUserAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit, bool preferUnlocked, const FString& TagQuery);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void UnlockAchievement(const FString& AchievementCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryUserGlobalAchievementsSuccess, FAccelByteModelsPaginatedUserGlobalAchievement, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryGlobalAchievements(FString const& AchievementCode, EAccelByteGlobalAchievementStatus const& AchievementStatus, EAccelByteGlobalAchievementListSortBy const& SortBy, FQueryUserGlobalAchievementsSuccess const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset, int32 const& Limit);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryGlobalAchievementContributorsSuccess, FAccelByteModelsPaginatedGlobalAchievementContributors, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryGlobalAchievementContributors(FString const& AchievementCode, EAccelByteGlobalAchievementContributorsSortBy const& SortBy, FQueryGlobalAchievementContributorsSuccess const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset, int32 const& Limit);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryGlobalAchievementUserContributedSuccess, FAccelByteModelsPaginatedGlobalAchievementUserContributed, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api ")
	static void QueryGlobalAchievementUserContributed(FString const& AchievementCode, EAccelByteGlobalAchievementContributorsSortBy const& SortBy, FQueryGlobalAchievementUserContributedSuccess const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset, int32 const& Limit);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Achievement | Api")
	static void ClaimGlobalAchievements(FString const& AchievementCode, FDHandler const& OnSuccess, FDErrorHandler const& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetTagSuccess, const FAccelByteModelsPaginatedPublicTag&, Result);
	UFUNCTION(BlueprintCallable, Category="AccelByte | Achievement | Api")
	static void GetTags(const FString& Name, const EAccelByteAchievementListSortBy& SortBy,const FGetTagSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit);
};
