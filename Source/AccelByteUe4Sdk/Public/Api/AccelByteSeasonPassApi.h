// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSeasonPassModels.h"


namespace AccelByte
{
	class Credentials;
	class Settings;

	namespace Api
	{
		class ServerSeasonPass;
		/**
		* @brief Season Pass APIs to access Season Pass service. User can obtaining season and claiming rewards.
		*/
		class ACCELBYTEUE4SDK_API SeasonPass
		{
		public:
			SeasonPass(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
			~SeasonPass();
		private:
			FHttpRetryScheduler& HttpRef;
			Credentials const& CredentialsRef;
			Settings const& SettingsRef;
		public:
			/** @brief Get current active season.
			 *
			 * @param Language The language of the Season.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetCurrentSeason(FString const& Language, THandler<FAccelByteModelsSeasonInfo> const& OnSuccess, FErrorHandler const& OnError);
			
			/** @brief Get user season data by SeasonId
			*
			* @param SeasonId The Id of the Season.
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			* @param OnError This will be called when the operation failed.
			*/
			void GetUserSeason(FString const& SeasonId, THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess, FErrorHandler const& OnError);
			
			/** @brief Get current active user season data.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			* @param OnError This will be called when the operation failed.
			*/
			void GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess, FErrorHandler const& OnError);
			
			/** @brief Claim Season Rewards.
			*
			* @param RewardRequest Detail information for the Reward Request.
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonClaimRewardResponse.
			* @param OnError This will be called when the operation failed.
			*/
			void ClaimRewards(FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest, THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess, FErrorHandler const& OnError);
			
			/** @brief Bulk claim season rewards.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonClaimRewardResponse.
			* @param OnError This will be called when the operation failed.
			*/
			void BulkClaimRewards(THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess, FErrorHandler const& OnError);

		private:
			/** @brief Utility to convert Claim Reward Json Object to TMap<int32, TMap<FString, TArray<FString>>>
			 *
			 * @param JsonObject Input of JsonObject to be converted
			 * @return TMap<FString, TArray<FString>> TMap as Passes Rewards (Key as Pass Code, Value as Array of Reward Codes)
			 */
			static TMap<FString, TArray<FString>> FJsonObjectToPassRewards(TSharedPtr<FJsonObject> JsonObject);

			SeasonPass() = delete;
			SeasonPass(SeasonPass const&) = delete;
			SeasonPass(SeasonPass&&) = delete;
		};
	};
}
