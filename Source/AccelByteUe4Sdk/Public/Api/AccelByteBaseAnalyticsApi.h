// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Models/AccelByteGameTelemetryModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Send telemetry data securely and the user should be logged in first.
 */
class ACCELBYTEUE4SDK_API BaseAnalytics : public GameTelemetry
{
protected:
	BaseAnalytics(Credentials& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, FString const& InEventName, bool bInCacheEvent, bool bInRetryOnFailed = false);

	BaseAnalytics(Credentials& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient, FString const& InEventName, bool bInCacheEvent, bool bInRetryOnFailed = false);

	void SendEventData(const TSharedPtr<FJsonObject>& Payload,
		FVoidHandler const& OnSuccess,
		FErrorHandler const& OnError,
		FDateTime const& ClientTimestamp = FDateTime::UtcNow());
	
	/**
	 * @brief Send/enqueue a single authorized telemetry data of an event.
	 * User should be logged in.
	 *
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelBytePredefinedEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendEventData(const TSharedRef<T>& Payload, 
						FVoidHandler const& OnSuccess, 
						FErrorHandler const& OnError, 
						FDateTime const& ClientTimestamp = FDateTime::UtcNow())
	{
		const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Payload.Get());
		SendEventData(JsonObject, OnSuccess, OnError, ClientTimestamp);
	}

	void SendEventData(FAccelByteModelsTelemetryBody Payload, 
						FVoidHandler const& OnSuccess, 
						FErrorHandler const& OnError, 
						FDateTime const& ClientTimestamp = FDateTime::UtcNow());


	const FString EventName;
	using FApiBase::CredentialsRef;
	using GameTelemetry::Flush;
	using GameTelemetry::SetCriticalEventList;
	using GameTelemetry::SetImmediateEventList;

private:
	using GameTelemetry::Send;
	BaseAnalytics() = delete;
	BaseAnalytics(BaseAnalytics const&) = delete;
	BaseAnalytics(BaseAnalytics&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
