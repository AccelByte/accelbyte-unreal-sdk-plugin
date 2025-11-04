// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteAchievementModels.h"

namespace AccelByte
{

class Credentials;
class Settings;

namespace Api
{

/**
 * @brief Provide APIs to access Achievement service.
 */
class ACCELBYTEUE4SDK_API Achievement 
	: public FApiBase
	, public TSharedFromThis<Achievement, ESPMode::ThreadSafe>
{
public:
	Achievement(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	Achievement(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~Achievement();

	/**
	 * @brief Query all achievements in the related namespace.
	 *
	 * @param Language The language to display the appropriate achievement's name and description. If it is empty, it will use its 
	 *			default language. If the achievement does not have the expected language, it will use its default language.
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedPublicAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param TagQuery A query expression consists of tags to query the achievement from.
	 * @param bGlobal True if the configuration to display global achievements.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryAchievements(FString const& Language
		, EAccelByteAchievementListSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedPublicAchievement> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20
		, FString const& TagQuery = TEXT("")
		, bool bGlobal = false);

	/**
	 * @brief Get an specific achievement information.
	 *
	 * @param AchievementCode The code of the expected achievement.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsMultiLanguageAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAchievement(FString const& AchievementCode
		, THandler<FAccelByteModelsMultiLanguageAchievement> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief [Deprecated] Query user's achievements. Include achieved and in-progress.
	 *
	 * @param SortBy Sorting method for the achievements result. Only support achieveAt, createdAt
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param PreferUnlocked True if the configuration to display unlocked achievements first active, the list order should display 
	 *			unlocked achievements first on top of locked achievements, and false otherwise. Default value is true.
	 * @param TagQuery A query expression consists of tags to query the achievement from
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserAchievements(EAccelByteAchievementListSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedUserAchievement> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20
		, bool PreferUnlocked = true
		, FString const& TagQuery = TEXT(""));

	/**
	 * @brief Query user's achievements. Include achieved and in-progress.
	 *
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param PreferUnlocked True if the configuration to display unlocked achievements first active, the list order should display 
	 *			unlocked achievements first on top of locked achievements, and false otherwise. Default value is true.
	 * @param TagQuery A query expression consists of tags to query the achievement from.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserAchievements(EAccelByteGlobalAchievementListSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedUserAchievement> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20
		, bool PreferUnlocked = true
		, FString const& TagQuery = TEXT(""));

	/**
	 * @brief Query other user's achievements. Include achieved and in-progress.
	 *
	 * @param UserId Target user id to retrieve achievement data from
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param PreferUnlocked True if the configuration to display unlocked achievements first active, the list order should display 
	 *			unlocked achievements first on top of locked achievements, and false otherwise. Default value is true.
	 * @param TagQuery A query expression consists of tags to query the achievement from.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */

	FAccelByteTaskWPtr QueryUserAchievementsByUserId(FString const& UserId
		, EAccelByteAchievementListSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedUserAchievement> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20
		, bool PreferUnlocked = true
		, FString const& TagQuery = TEXT(""));

	/**
	 * @brief Unlock specific achievement.
	 *
	 * @param AchievementCode The achievement code which will be unlock.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UnlockAchievement(FString const& AchievementCode
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Unlock multiple achievement.
	 *
	 * @param AchievementsToUnlock Request of achievement that needs to be unlocked.
	 * @param UnlockResponses This will be called when the operation completed. In a form of array for each achievement that was requested.
	 * @param OnGeneralError This will be called when the entire operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkUnlockAchievement(FAccelByteModelsAchievementBulkUnlockRequest const& AchievementsToUnlock
		, THandler<TArray<FAccelByteModelsAchievementBulkUnlockRespone>> const& UnlockResponses
		, FErrorHandler const& OnGeneralError);

	/**
	 * @brief Get the progress list of global achievements. Include achieved and in-progress.
	 *
	 * @param AchievementCode The achievement code.
	 * @param AchievementStatus The status to display the appropriate achievements.
	 * @param SortBy Sorting method for the global achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserGlobalAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param TagQuery A query expression consists of tags to query the achievement from.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryGlobalAchievements(FString const& AchievementCode
		, EAccelByteGlobalAchievementStatus const& AchievementStatus
		, EAccelByteGlobalAchievementListSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedUserGlobalAchievement> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20
		, FString const& TagQuery = TEXT(""));

	/**
	 * @brief Get the list of contributors for a global achievement.
	 *
	 * @param AchievementCode The achievement code.
	 * @param SortBy Sorting method for the global achievements contributors result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedGlobalAchievementContributors&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryGlobalAchievementContributors(FString const& AchievementCode
		, EAccelByteGlobalAchievementContributorsSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedGlobalAchievementContributors> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20);

	/**
	 * @brief Get the list of global achievements that have been contributed by the user.
	 *
	 * @param AchievementCode The achievement code.
	 * @param SortBy Sorting method for the global achievements contributors result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedGlobalAchievementUserContributed&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryGlobalAchievementUserContributed(FString const& AchievementCode
		, EAccelByteGlobalAchievementContributorsSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedGlobalAchievementUserContributed> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20);

	/**
	 * @brief Claim specific global achievement.
	 *
	 * @param AchievementCode The global achievement code which will be claimed.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ClaimGlobalAchievements(FString const& AchievementCode
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get available public achievement tag.
	 *
	 * @param Name The name of the public tag.
	 * @param SortBy Sorting method for the public tag result.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetTags(FString const& Name, EAccelByteAchievementListSortBy const& SortBy,THandler<FAccelByteModelsPaginatedPublicTag> const& OnSuccess, FErrorHandler const& OnError,int32 const& Offset = 0, int32 const& Limit = 20);

private:
	Achievement() = delete;
	Achievement(Achievement const&) = delete;
	Achievement(Achievement&&) = delete;

	static FString ConvertAchievementSortByToString(EAccelByteAchievementListSortBy const& SortBy);
	static FString ConvertGlobalAchievementSortByToString(EAccelByteGlobalAchievementListSortBy const& SortBy);
	static FString ConvertGlobalAchievementTypeToString(EAccelByteGlobalAchievementStatus const& Status);
	static FString ConvertGlobalAchievementContributosSortByToString(EAccelByteGlobalAchievementContributorsSortBy const& SortBy);
};

using AchievementRef = TSharedRef<Achievement, ESPMode::ThreadSafe>;
using AchievementPtr = TSharedPtr<Achievement, ESPMode::ThreadSafe>;
using AchievementWPtr = TWeakPtr<Achievement, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
