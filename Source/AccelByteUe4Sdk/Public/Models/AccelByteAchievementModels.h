// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteAchievementModels.generated.h"

/** @brief Enumeration for Achievement sorting */
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

/** @brief Data Model for Achievement's Icon */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementIcon
{
	GENERATED_BODY()
	
	/** @brief Url where the Icon is located */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | AchievementIcon")
	FString Url{};
	
	/** @brief Slug information of the Icon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | AchievementIcon")
	FString Slug{};
};

/** @brief Data Model for Achievement's public information */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicAchievement
{
	GENERATED_BODY()
	
	/** @brief Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString AchievementCode{};

	/** @brief Game Namespace where the Achievement is registered */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString Namespace{};

	/** @brief The name of the Achievement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString Name{};

	/** @brief The description of the Achievement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString Description{};

	/** @brief List of Achievement icons for locked state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	TArray<FAccelByteModelsAchievementIcon> LockedIcons{};

	/** @brief List of Achievement icons for unlocked state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	TArray<FAccelByteModelsAchievementIcon> UnlockedIcons{};

	/** @brief A flag to indicate the Achievement is hidden */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	bool Hidden{};

	/** @brief Order of the Achievement in the list */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	int32 ListOrder{};

	/** @brief Achievement Tags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	TArray<FString> Tags{};

	/** @brief A flag that indicates Achievement is incremental or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	bool Incremental{};

	/** @brief Achievement goal value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	float GoalValue{};

	/** @brief Statistic code related to track the Achievement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString StatCode{};

	/** @brief Achievement creation time (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString CreatedAt{};

	/** @brief Last updated time (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FString UpdatedAt{};
};

/** @brief Pagniated Data Model for Achievement's public information */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedPublicAchievement
{
	GENERATED_BODY()

	/** @brief List of Achievement data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedPublicAchievement")
	TArray<FAccelByteModelsPublicAchievement> Data{};

	/** @brief Paging data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedPublicAchievement")
	FAccelByteModelsPaging Paging{};
};

/** @brief Data Model for Achievement's localization */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMultiLanguageAchievement
{
	GENERATED_BODY()

	/** @brief Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	FString AchievementCode{};

	/** @brief Game Namespace where the Achievement is registered */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	FString Namespace{};

	/**
	 * @brief List of the localized name of the Achievement using KV-pairs.
	 *        The Key is the language code in ISO 639-1 and the Value is
	 *        the localized name
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	TMap<FString, FString> Name{};

	/**
	 * @brief List of the localized description of the Achievement using KV-pairs.
	 *        The Key is the language code in ISO 639-1 and the Value is
	 *        the localized description
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	TMap<FString, FString> Description{};

	/** @brief List of Achievement icons for locked state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	TArray<FAccelByteModelsAchievementIcon> LockedIcons{};

	/** @brief List of Achievement icons for unlocked state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	TArray<FAccelByteModelsAchievementIcon> UnlockedIcons{};

	/** @brief A flag to indicate the Achievement is hidden */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	bool Hidden{};
	
	/** @brief Order of the Achievement in the list */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	int32 ListOrder{};

	/** @brief Achievement Tags */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	TArray<FString> Tags{};

	/** @brief A flag that indicates Achievement is incremental or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	bool Incremental{};

	/** @brief Achievement goal value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	float GoalValue{};

	/** @brief Statistic code related to track the Achievement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	FString StatCode{};

	/** @brief Achievement creation time (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	FString CreatedAt{};

	/** @brief Last updated time (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	FString UpdatedAt{};
};

/** @brief Data Model for Achievement collection information */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCountInfo
{
	GENERATED_BODY()
	/** @brief Total number of Achievements available */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | CountInfo")
	int32 NumberOfAchievements{};

	/** @brief Total number of Achievements which are hidden */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | CountInfo")
	int32 NumberOfHiddenAchievements{};

	/** @brief Total number of Achievements which are visible */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | CountInfo")
	int32 NumberOfVisibleAchievements{};
};

/** @brief Data Model for User Achievement progression */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserAchievement
{
	GENERATED_BODY()

	/** @brief Unique Id for User Achievement's identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
	FString Id{};

	/**
     * @brief List of the localized name of the Achievement using KV-pairs.
     *        The Key is the language code in ISO 639-1 and the Value is
     *        the localized name
     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
	TMap<FString, FString> Name{};

	/** @brief Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
	FString AchievementCode{};

	/** @brief The time when user achieved the Achievement (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
	FString AchievedAt{};

	/** @brief The last updated value of the Achievement  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
	float LatestValue{};

	/** @brief User Achievement's status. (1: In-Progress, 2: Unlocked) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserAchievement")
	int32 Status{};
};

/** @brief Data Model for List of User Achievements progression */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedUserAchievement
{
	GENERATED_BODY()

	/** @brief The total number of Achievements for the user */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserAchievement")
	FAccelByteModelsCountInfo CountInfo{};

	/** @brief List of User Achievements data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserAchievement")
	TArray<FAccelByteModelsUserAchievement> Data{};

	/** @brief Paging data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserAchievement")
	FAccelByteModelsPaging Paging{};
};
