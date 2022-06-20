// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteSeasonPassModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
	class ServerCredentials;
	class ServerSettings;

	namespace GameServerApi
	{
		/**
		* @brief Server Season Pass APIs to manage user season pass related like granting exp progression.
		*/
		class ACCELBYTEUE4SDK_API ServerSeasonPass
		{
		public:
			ServerSeasonPass(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
			~ServerSeasonPass();
		private:
			ServerCredentials const& CredentialsRef;
			ServerSettings const& SettingsRef;
			FHttpRetryScheduler& HttpRef;
		public:
			/** @brief Grant exp to user by UserId. After reaching to the next level, currentExp will be reset to 0, remainder will be added.
			 *
			 * @param UserId The User ID will be granted the exp
			 * @param Exp Total of the exp will be granted to user.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			 * @param OnError This will be called when the operation failed.
			 * @param Source Granted exp source, default value SWEAT
			 * @param Tags Grant reason, default value Empty 
			 */
			void GrantExpToUser(const FString& UserId, int32 Exp, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError,
				EAccelByteSeasonPassSource Source = EAccelByteSeasonPassSource::SWEAT, const TArray<FString>& Tags = {});

			/** @brief Grant tier to user by UserId. It will auto enroll if there's no user season but active published season exist,
			 * season only located in non-publisher namespace, otherwise ignore.
			 *
			 * @param UserId The User ID will be granted the exp
			 * @param Count Total of the exp will be granted to user.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			 * @param OnError This will be called when the operation failed.
			 * @param Source Granted tier source, default value SWEAT
			 * @param Tags Grant reason, default value Empty 
			 */
			void GrantTierToUser(const FString& UserId, int32 Count, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError,
				EAccelByteSeasonPassSource Source = EAccelByteSeasonPassSource::SWEAT, const TArray<FString>& Tags = {});
			
			/** @brief Get current user season progression, season only located in non-publisher namespace.
			*
			* @param UserId The User ID to check user season progression
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfoWithoutReward.
			* @param OnError This will be called when the operation failed.
			*/
			void GetCurrentUserSeasonProgression(const FString& UserId, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError);

			/** @brief Get user season data by SeasonId, season only located in non-publisher namespace.
			*
			* @param UserId The Id of the User.
			* @param SeasonId The Id of the Season.
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			* @param OnError This will be called when the operation failed.
			*/
			void GetUserSeasonData(const FString& UserId, FString const& SeasonId, const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess, const FErrorHandler& OnError);

			/** @brief Get current user season history
			*
			* @param UserId The User ID to check user season progression
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonExpHistory.
			* @param OnError This will be called when the operation failed.
			*/
			void GetCurrentUserSeasonHistory(const FString& UserId, const THandler<FAccelByteModelsUserSeasonExpHistory>& OnSuccess, const FErrorHandler& OnError);

			/** @brief Get user exp acquisition history's tag list
			*
			* @param UserId The User ID to check user season progression
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonExpHistory.
			* @param OnError This will be called when the operation failed.
			*/			
			void QueryUserSeasonExp(const FString& UserId, const THandler<FAccelByteModelsQueryUserSeasonExp>& OnSuccess, const FErrorHandler& OnError);

		private:
			ServerSeasonPass() = delete;
			ServerSeasonPass(const ServerSeasonPass&) = delete;
			ServerSeasonPass(ServerSeasonPass&&) = delete;
		};
	}
}
