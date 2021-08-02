// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteSeasonPassModels.h"

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
			ServerSeasonPass(const ServerCredentials& Credentials, const ServerSettings& Settings);
			~ServerSeasonPass();
		private:
			const ServerCredentials& Credentials;
			const ServerSettings& Settings;
		public:
			/** @brief Grant exp to user by UserId. After reaching to the next level, currentExp will be reset to 0, remainder will be added.
			 *
			 * @param UserId The User ID will be granted the exp
			 * @param Exp Total of the exp will be granted to user.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserSeasonInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void GrantExpToUser(const FString& UserId, int32 Exp, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError);
		private:
			ServerSeasonPass() = delete;
			ServerSeasonPass(const ServerSeasonPass&) = delete;
			ServerSeasonPass(ServerSeasonPass&&) = delete;
		};
	}
}
