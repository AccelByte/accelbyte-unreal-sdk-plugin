// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

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
{
public:
	Achievement(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Achievement();

	/**
	 * @brief Query all achievements in the related namespace.
	 *
	 * @param Language The language to display the appropiate achievement's name and description. If it is empty, it will use the its default language.
				If the achievement does not have the expected language, it will use its dafault languge.
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedPublicAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	*/
	void QueryAchievements(FString const& Language, EAccelByteAchievementListSortBy const& SortBy, THandler<FAccelByteModelsPaginatedPublicAchievement> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset = 0, int32 const& Limit = 20);

	/**
	 * @brief Get an specific achievement information.
	 *
	 * @param AchievementCode The code of the expected achievement.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsMultiLanguageAchievement&.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetAchievement(FString const& AchievementCode, THandler<FAccelByteModelsMultiLanguageAchievement> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Query user's achievements. Include achieved and in-progress.
	 *
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param PreferUnlocked True if the configuration to display unlocked achievements first active, the list order should display unlocked achievements first on top of locked achievements, and false otherwise. Default value is true.
	*/
	void QueryUserAchievements(EAccelByteAchievementListSortBy const& SortBy, THandler<FAccelByteModelsPaginatedUserAchievement> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset = 0, int32 const& Limit = 20, bool PreferUnlocked = true);

	/**
	 * @brief Unlock specific achievement.
	 *
	 * @param AchievementCode The achievement code which will be unlock.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
	void UnlockAchievement(FString const& AchievementCode, FVoidHandler const OnSuccess, FErrorHandler const& OnError);

private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

	Achievement() = delete;
	Achievement(Achievement const&) = delete;
	Achievement(Achievement&&) = delete;

	FString ConvertAchievementSortByToString(EAccelByteAchievementListSortBy const& SortBy);
};

} // Namespace Api
} // Namespace AccelByte
