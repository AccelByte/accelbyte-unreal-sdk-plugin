// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
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
			SeasonPass(const Credentials& Credentials, const Settings& Settings);
			~SeasonPass();
		private:
			const Credentials& Credentials;
			const Settings& Settings;
		public:
			/** @brief Get current active season.
			 *
			 * @param Language The language of the Season.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetCurrentSeason(const FString& Language, const THandler<FAccelByteModelsSeasonInfo>& OnSuccess, const FErrorHandler& OnError);
			
			/** @brief Get user season data by SeasonId
			*
			* @param SeasonId The Id of the Season.
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			* @param OnError This will be called when the operation failed.
			*/
			void GetUserSeason(const FString& SeasonId, const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess, const FErrorHandler& OnError);
			
			/** @brief Get current active user season data.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			* @param OnError This will be called when the operation failed.
			*/
			void GetCurrentUserSeason(const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess, const FErrorHandler& OnError);
			
			/** @brief Claim Season Rewards.
			*
			* @param RewardRequest Detail information for the Reward Request.
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonClaimRewardResponse.
			* @param OnError This will be called when the operation failed.
			*/
			void ClaimRewards(const FAccelByteModelsSeasonClaimRewardRequest& RewardRequest, const THandler<FAccelByteModelsSeasonClaimRewardResponse>& OnSuccess, const FErrorHandler& OnError);
			
			/** @brief Bulk claim season rewards.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSeasonClaimRewardResponse.
			* @param OnError This will be called when the operation failed.
			*/
			void BulkClaimRewards(const THandler<FAccelByteModelsSeasonClaimRewardResponse>& OnSuccess, const FErrorHandler& OnError);

		private:
			/** @brief Utility to convert Claim Reward Json Object to TMap<int32, TMap<FString, TArray<FString>>>
			 *
			 * @param JsonObject Input of JsonObject to be converted
			 * @return TMap<FString, TArray<FString>> TMap as Passes Rewards (Key as Pass Code, Value as Array of Reward Codes)
			 */
			TMap<FString, TArray<FString>> FJsonObjectToPassRewards(TSharedPtr<FJsonObject> JsonObject);

			SeasonPass() = delete;
			SeasonPass(const SeasonPass&) = delete;
			SeasonPass(SeasonPass&&) = delete;
		};
	};
}
