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

class ACCELBYTEUE4SDK_API Configurations : public FApiBase, public TSharedFromThis<Configurations, ESPMode::ThreadSafe>
{
public:
	Configurations(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~Configurations();

	/**
	 * @brief Get all public configurations for current namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAll(THandler<TArray<FAccelByteModelsConfiguration>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a public configuration based on the key for current namespace.
	 *
	 * @param Key The configuration Key.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr Get(FString const& Key
		, THandler<FAccelByteModelsConfiguration> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	Configurations() = delete;
	Configurations(Configurations const&) = delete;
	Configurations(Configurations&&) = delete;
};

typedef TSharedRef<Configurations, ESPMode::ThreadSafe> ConfigurationsRef;
typedef TSharedPtr<Configurations, ESPMode::ThreadSafe> ConfigurationsPtr;
typedef TWeakPtr<Configurations, ESPMode::ThreadSafe> ConfigurationsWPtr;

}
}
