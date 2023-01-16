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
class ACCELBYTEUE4SDK_API Achievement : public FApiBase
{
public:
	Achievement(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Achievement();

	/**
	 * @brief Query all achievements in the related namespace.
	 *
	 * @param Language The language to display the appropriate achievement's name and description. If it is empty, it will use the its default language.
				If the achievement does not have the expected language, it will use its default language.
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedPublicAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param TagQuery A query expression consists of tags to query the achievement from
	 */
	void QueryAchievements(FString const& Language
		, EAccelByteAchievementListSortBy const& SortBy
		, THandler<FAccelByteModelsPaginatedPublicAchievement> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20
		, FString const& TagQuery = TEXT(""));

	/**
	 * @brief Get an specific achievement information.
	 *
	 * @param AchievementCode The code of the expected achievement.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsMultiLanguageAchievement&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetAchievement(FString const& AchievementCode
		, THandler<FAccelByteModelsMultiLanguageAchievement> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query user's achievements. Include achieved and in-progress.
	 *
	 * @param SortBy Sorting method for the achievements result.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserAchievement&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	 * @param PreferUnlocked True if the configuration to display unlocked achievements first active, the list order should display unlocked achievements first on top of locked achievements, and false otherwise. Default value is true.
	 * @param TagQuery A query expression consists of tags to query the achievement from
	 */
	void QueryUserAchievements(EAccelByteAchievementListSortBy const& SortBy
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
	 */
	void UnlockAchievement(FString const& AchievementCode
		, FVoidHandler const OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get public tag data.
	 *
	 * @param Name The name of the public tag.
	 * @param SortBy Sorting method for the public tag result.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the achievements result. Default value is 0.
	 * @param Limit The limit of the achievements result. Default value is 20.
	*/
	void GetTags(FString const& Name,EAccelByteAchievementListSortBy const& SortBy,THandler<FAccelByteModelsPaginatedPublicTag> const& OnSuccess, FErrorHandler const& OnError,int32 const& Offset = 0, int32 const& Limit = 20);

private:
	Achievement() = delete;
	Achievement(Achievement const&) = delete;
	Achievement(Achievement&&) = delete;

	static FString ConvertAchievementSortByToString(EAccelByteAchievementListSortBy const& SortBy);
};

} // Namespace Api
} // Namespace AccelByte
