// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteAchievementModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Provide APIs to access Achievement service.
 */
class ACCELBYTEUE4SDK_API ServerAchievement : public FServerApiBase
{
public:
	ServerAchievement(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler & InHttpRef);
	~ServerAchievement();

	/**
	 * @brief Unlock specific achievement.
	 *
	 * @param UserId The id of the user who will receive achievement.
	 * @param AchievementCode The achievement code which will be unlock.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UnlockAchievement(FString const& UserId
		, FString const& AchievementCode
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create events to sync to PSN, used to unlock trophies through game server
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkCreatePSNEvent(FAccelByteModelsAchievementBulkCreatePSNEventRequest const& Request
		, THandler<FAccelByteModelsAchievementBulkCreatePSNEventResponse> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerAchievement() = delete;
	ServerAchievement(ServerAchievement const&) = delete;
	ServerAchievement(ServerAchievement&&) = delete;
};

} // Namespace GameServerApi
} // Namespace AccelByte
