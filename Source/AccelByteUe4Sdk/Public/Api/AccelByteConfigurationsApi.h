// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Models/AccelByteConfigurationsModels.h"

namespace AccelByte
{
	class Credentials;
	class Settings;

	namespace Api
	{
		class ACCELBYTEUE4SDK_API Configurations : public FApiBase
		{
		public:
			Configurations(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
			~Configurations();

			/**
			 * @brief Get all public configurations for current namespace.
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetAll(const THandler<TArray<FAccelByteModelsConfiguration>>& OnSuccess
				, const FErrorHandler& OnError);

			/**
			 * @brief Get a public configuration based on the key for current namespace.
			 *
			 * @param Key The configuration Key.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void Get(const FString& Key
				, const THandler<FAccelByteModelsConfiguration>& OnSuccess
				, const FErrorHandler& OnError);

		private:
			Configurations() = delete;
			Configurations(Configurations const&) = delete;
			Configurations(Configurations&&) = delete;
		};
	}
}
