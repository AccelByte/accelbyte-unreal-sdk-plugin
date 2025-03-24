// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteSeasonPassModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{
	
/**
 * @brief Server Season Pass APIs to manage user season pass related like granting exp progression.
 */
class ACCELBYTEUE4SDK_API ServerSeasonPass : public FServerApiBase
{
public:
	ServerSeasonPass(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerSeasonPass();

	/**
	 * @brief Grant exp to user by UserId. After reaching to the next level, currentExp will be reset to 0, remainder will be added.
	 *
	 * @param UserId The User ID will be granted the exp
	 * @param Exp Total of the exp will be granted to user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param Source Granted exp source, default value SWEAT
	 * @param Tags Grant reason, default value Empty 
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GrantExpToUser(FString const& UserId
		, int32 Exp
		, THandler<FAccelByteModelsUserSeasonInfoWithoutReward> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteSeasonPassSource Source = EAccelByteSeasonPassSource::SWEAT
		, TArray<FString> const& Tags = {});

	/**
	 * @brief Grant tier to user by UserId. It will auto enroll if there's no user season but active published season exist,
	 * season only located in non-publisher namespace, otherwise ignore.
	 *
	 * @param UserId The User ID will be granted the exp
	 * @param Count Total of the exp will be granted to user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param Source Granted tier source, default value SWEAT
	 * @param Tags Grant reason, default value Empty
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GrantTierToUser(FString const& UserId
		, int32 Count
		, THandler<FAccelByteModelsUserSeasonInfoWithoutReward> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteSeasonPassSource Source = EAccelByteSeasonPassSource::SWEAT
		, TArray<FString> const& Tags = {});
	
	/**
	 * @brief Get current user season progression, season only located in non-publisher namespace.
	 *
	 * @param UserId The User ID to check user season progression
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfoWithoutReward.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCurrentUserSeasonProgression(FString const& UserId
		, THandler<FAccelByteModelsUserSeasonInfoWithoutReward> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user season data by SeasonId, season only located in non-publisher namespace.
	 *
	 * @param UserId The Id of the User.
	 * @param SeasonId The Id of the Season.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserSeasonData(FString const& UserId
		, FString const& SeasonId
		, THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get current user season history
	 *
	 * @param UserId The User ID to check user season progression
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonExpHistory.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCurrentUserSeasonHistory(FString const& UserId
		, THandler<FAccelByteModelsUserSeasonExpHistory> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user exp acquisition history's tag list
	 *
	 * @param UserId The User ID to check user season progression
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonExpHistory.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserSeasonExp(FString const& UserId
		, THandler<FAccelByteModelsQueryUserSeasonExp> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get current user session progression 
	 *
	 * @param UserIds The User IDs to check user season progression
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsUserSeasonInfo>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetUserSessionProgression(TArray<FString> const& UserIds
		, THandler<TArray<FAccelByteModelsUserSeasonInfo>> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerSeasonPass() = delete;
	ServerSeasonPass(const ServerSeasonPass&) = delete;
	ServerSeasonPass(ServerSeasonPass&&) = delete;
};

} // Namespace GameServerApi
} // Namespace AccelByte
