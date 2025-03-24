// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "JsonObjectWrapper.h"
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

/** @brief Enumeration for Global Achievement sorting */
UENUM(BlueprintType)
enum class EAccelByteGlobalAchievementListSortBy : uint8
{
	NONE = 0,
	ACHIEVED_AT,
	ACHIEVED_AT_ASC,
	ACHIEVED_AT_DESC,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC
};

/** @brief Enumeration for Global Achievement status */
UENUM(BlueprintType)
enum class EAccelByteGlobalAchievementStatus : uint8
{
	NONE = 0,
	IN_PROGRESS,
	UNLOCKED
};

/** @brief Enumeration for Global Achievement contributors sorting */
UENUM(BlueprintType)
enum class EAccelByteGlobalAchievementContributorsSortBy : uint8
{
	NONE = 0,
	CONTRIBUTED_VALUE,
	CONTRIBUTED_VALUE_ASC,
	CONTRIBUTED_VALUE_DESC
};


/** @brief Enumeration to convert Achievement Status sorting */
UENUM(BlueprintType)
enum class ConvertAchievementStatus : uint8
{
	NONE = 0,
	IN_PROGRESS = 1,
	UNLOCKED = 2
};

#pragma region BULK_OPERATION

/** @brief Request to unlock multiple achievement in bulk operation */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementBulkUnlockRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockRequest ")
	TArray<FString> AchievementCodes{};
};

/** @brief Error information from specific achievement when performing bulk operation */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementBulkUnlockErrorDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockResponseErrorDetail ")
	int32 ErrorCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockResponseErrorDetail ")
	FString ErrorMessage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockResponseErrorDetail ")
	FString ThirdPartyReferenceId{};
};

/** @brief Response from bulk operation to unlock multiple achievement */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementBulkUnlockRespone
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockResponse ")
	FString AchievementCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockResponse ")
	FAccelByteModelsAchievementBulkUnlockErrorDetail ErrorDetail{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkUnlockResponse ")
	bool Success{ false };
};

#pragma endregion

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

	/** @brief A flag that indicates Achievement is global or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	bool Global{};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicAchievement")
	FJsonObjectWrapper CustomAttributes{};
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

	/** @brief A flag that indicates Achievement is global or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	bool Global{};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | MultiLanguageAchievemen")
	FJsonObjectWrapper CustomAttributes{};
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

/** @brief Data Model for User Global Achievement progression */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserGlobalAchievement
{
	GENERATED_BODY()

	/** @brief Unique Id for User Achievement's identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	FString Id{};

	/**
	 * @brief List of the localized name of the Achievement using KV-pairs.
	 *        The Key is the language code in ISO 639-1 and the Value is
	 *        the localized name
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	TMap<FString, FString> Name{};

	/** @brief Global Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	FString AchievementCode{};

	/** @brief Game Namespace where the Global Achievement is registered */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	FString Namespace{};

	/** @brief User Global Achievement's status. (1: In-Progress, 2: Unlocked) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	ConvertAchievementStatus Status{ConvertAchievementStatus::NONE};

	/** @brief The last updated value of the Global Achievement  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	float LatestValue{};

	/** @brief The time when user achieved the Global Achievement (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	FDateTime AchievedAt{0};

	/** @brief The time when the Global Achievement created (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	FDateTime CreatedAt{0};

	/** @brief The time when the Global Achievement updated (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	FDateTime UpdatedAt{0};
};

/** @brief Data Model for List of User Global Achievements progression */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedUserGlobalAchievement
{
	GENERATED_BODY()

	/** @brief List of User Global Achievements data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserGlobalAchievement")
	TArray<FAccelByteModelsUserGlobalAchievement> Data{};

	/** @brief Paging data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedUserGlobalAchievement")
	FAccelByteModelsPaging Paging{};
};

/** @brief Data Model for List of Global Achievement Contributors*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalAchievementContributors
{
	GENERATED_BODY()

	/** @brief Unique Id for User Achievement's identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	FString Id{};

	/** @brief Namespace of the Global Achievement belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	FString Namespace{};

	/** @brief Global Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	FString AchievementCode{};

	/** @brief Global Achievement user id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	FString UserId{};

	/** @brief Value contributed to Global Achievement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	int32 ContributedValue{};

	/** @brief The time when the Global Achievement created (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	FDateTime CreatedAt{0};

	/** @brief The time when the Global Achievement updated (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributors")
	FDateTime UpdatedAt{0};
};

/** @brief Data Model for List of Global Achievements Contributors*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedGlobalAchievementContributors
{
	GENERATED_BODY()

	/** @brief List of Global Achievements Contributors data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedGlobalAchievementContributors")
	TArray<FAccelByteModelsGlobalAchievementContributors> Data{};

	/** @brief Paging data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedGlobalAchievementContributors")
	FAccelByteModelsPaging Paging{};
};

/** @brief Data Model for List of Global Achievement Contributed by User*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalAchievementContributed
{
	GENERATED_BODY()

	/** @brief Unique Id for User Achievement's identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributed")
	FString Id{};

	/**
	 * @brief List of the localized name of the Achievement using KV-pairs.
	 *        The Key is the language code in ISO 639-1 and the Value is
	 *        the localized name
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserGlobalAchievement")
	TMap<FString, FString> Name{};

	/** @brief Namespace of the Global Achievement belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributed")
	FString Namespace{};

	/** @brief Global Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributed")
	FString AchievementCode{};

	/** @brief Global Achievement user id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributed")
	FString UserId{};

	/** @brief Value contributed to Global Achievement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributed")
	int32 ContributedValue{};

	/** @brief A flag that indicates the Global Achievement can be claimed or not */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementContributed")
	bool CanClaimReward{};
};

/** @brief Data Model for List of Global Achievements Contributors*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedGlobalAchievementUserContributed
{
	GENERATED_BODY()

	/** @brief List of Global Achievements Contributed data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementUserContributed")
	TArray<FAccelByteModelsGlobalAchievementContributed> Data{};

	/** @brief Paging data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | GlobalAchievementUserContributed")
	FAccelByteModelsPaging Paging{};
};

/** @brief Data Model for List of Claimed Global Achievements*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsClaimedGlobalAchievements
{
	GENERATED_BODY()

	/** @brief Global Achievement code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ClaimedGlobalAchievements")
	FString AchievementCode{};

	/** @brief The time when user achieved the Global Achievement (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ClaimedGlobalAchievements")
	FDateTime AchievedAt{0};
};

/** @brief Data Model for Achievement Tag */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicTag
{
	GENERATED_BODY()

	/** @brief Name of Public Tag data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicTag")
	FString Name{};
	
	/** @brief Namespace of the Public Tag belongs to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicTag")
	FString Namespace{};

	/** @brief Public Tag creation time (ISO 8601) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PublicTag")
	FString CreatedAt{};
};

/** @brief Data Model for List of Public Tags */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedPublicTag
{
	GENERATED_BODY()

	/** @brief List of Public Tag data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedPublicTag")
	TArray<FAccelByteModelsPublicTag> Data{};

	/** @brief Paging data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PaginatedPublicTag")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementPSNEventData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventData")
	FString EventName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventData")
	FJsonObjectWrapper Properties{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementPSNEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEvent")
	FString Version{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEvent")
	FString EventId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEvent")
	FString DateTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEvent")
	FAccelByteModelsAchievementPSNEventData EventData{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementPSNEventCreate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventCreate")
	FString ThirdPartyUserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventCreate")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventCreate")
	TArray<FAccelByteModelsAchievementPSNEvent> Events{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementBulkCreatePSNEventRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkCreatePSNEventRequest")
	TArray<FAccelByteModelsAchievementPSNEventCreate> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementPSNEventStatusError
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventStatusError")
	int64 ErrorCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventStatusError")
	FString ErrorMessage{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementPSNEventStatus
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventStatus")
	bool IsSuccess{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventStatus")
	FString ThirdPartyUserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PSNEventStatus")
	FAccelByteModelsAchievementPSNEventStatusError ErrorDetail{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementBulkCreatePSNEventResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | BulkCreatePSNEventResponse")
	TArray<FAccelByteModelsAchievementPSNEventStatus> Data{};
};
