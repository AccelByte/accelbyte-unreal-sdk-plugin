// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteAchievementModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteAchievementListSortBy : uint8
{
	NONE = 0,
	LISTORDER,
	LISTORDER_ASC,
	LISTORDER_DESC,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	UPDATED_AT,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementIcon
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | AchievementIcon")
		FString Url;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | AchievementIcon")
		FString Slug;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicAchievement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		TArray<FAccelByteModelsAchievementIcon> LockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		TArray<FAccelByteModelsAchievementIcon> UnlockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		bool Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		int32 ListOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		bool Incremental;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		int32 GoalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString StatCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedPublicAchievement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedPublicAchievement")
		TArray<FAccelByteModelsPublicAchievement> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedPublicAchievement")
		FAccelByteModelsPaging Paging;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMultiLanguageAchievement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		TMap<FString, FString> Name; // "Language" : "Name"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		TMap<FString, FString> Description; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		TArray<FAccelByteModelsAchievementIcon> LockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		TArray<FAccelByteModelsAchievementIcon> UnlockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		bool Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		int32 ListOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		bool Incremental;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		int32 GoalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		FString StatCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCountInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | CountInfo")
		int32 NumberOfAchievements;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | CountInfo")
		int32 NumberOfHiddenAchievements;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | CountInfo")
		int32 NumberOfVisibleAchievements;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserAchievement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
		FString AchievedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
		int32 LatestValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
		int32 Status; // 1: In-Progress, 2: Unlocked
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedUserAchievement
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserAchievement")
		FAccelByteModelsCountInfo CountInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserAchievement")
		TArray<FAccelByteModelsUserAchievement> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserAchievement")
		FAccelByteModelsPaging Paging;
};
