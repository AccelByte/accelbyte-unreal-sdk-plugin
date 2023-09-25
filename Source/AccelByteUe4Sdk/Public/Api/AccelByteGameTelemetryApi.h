// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"
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
class ACCELBYTEUE4SDK_API GameTelemetry : public FApiBase
{
public:
	GameTelemetry(Credentials& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, bool bInCacheEvent = true);
	~GameTelemetry();

	/**
	 * @brief Set the interval of sending telemetry event to the backend.
	 * By default it sends the queued events once a minute.
	 * Should not be less than 5 seconds.
	 *
	 * @param Interval The interval between telemetry event
	 */
	void SetBatchFrequency(FTimespan Interval);

	/**
	 * @brief Set list of event that need to be backed up on disc before sending in order to be able to recover in case of failure.
	 *
	 * @param EventNames FString Array of payload EventName.
	 */
	void SetCriticalEventList(TArray<FString> const& EventNames);

	/**
	 * @brief Set list of event that need to be sent immediately without the needs to jobQueue it.
	 *
	 * @param EventNames FString Array of payload EventName.
	 */
	void SetImmediateEventList(TArray<FString> const& EventNames);

	/**
	 * @brief Send/enqueue a single authorized telemetry data.
	 * Server should be logged in. See DedicatedServer::LoginWithClientCredentials()
	 *
	 * @param TelemetryBody Telemetry request with arbitrary payload.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void Send(FAccelByteModelsTelemetryBody TelemetryBody
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Flush pending telemetry events
	 */
	void Flush();

	/**
	 * @brief Startup module
	 */
	void Startup();

	/**
	 * @brief Shutdown module
	 */
	void Shutdown();

	/**
	 * @brief Check the cache has been updated or not
	*/
	bool IsCacheUpdated() { return bCacheUpdated; }

private:
	void SendProtectedEvents(TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> const& Events
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	bool PeriodicTelemetry(float DeltaTime);
	
	void LoadCachedEvents();
	
	void AppendEventToCache(TSharedPtr<FAccelByteModelsTelemetryBody> Telemetry);
	
	void OnLoginSuccess(FOauth2Token const& Response);
	
	void RemoveEventsFromCache();
	
	bool JobArrayQueueAsJsonString(FString& OutJsonString);
	
protected:
	bool EventsJsonToArray(FString& InJsonString
		, TArray<TSharedPtr<FAccelByteModelsTelemetryBody>>& OutArray);
	
	FString GetTelemetryKey();

private:
	GameTelemetry() = delete;
	GameTelemetry(GameTelemetry const&) = delete;
	GameTelemetry(GameTelemetry&&) = delete;

	Credentials& CredentialsRef;

	FTimespan TelemetryInterval = FTimespan(0, 1, 0);
	TSet<FString> ImmediateEvents;
	TSet<FString> CriticalEvents;
	bool bCacheUpdated = false;
	TQueue<TTuple<TSharedPtr<FAccelByteModelsTelemetryBody>, FVoidHandler, FErrorHandler>> JobQueue;
	TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> EventPtrArray;
	bool bTelemetryJobStarted = false;
	FTimespan const MINIMUM_INTERVAL_TELEMETRY = FTimespan(0, 0, 5);
	FTickerDelegate GameTelemetryTickDelegate;
	FDelegateHandleAlias GameTelemetryTickDelegateHandle;
	FDelegateHandle GameTelemetryLoginSuccess;

	bool ShuttingDown;
	bool bCacheEvent;
};

} // Namespace Api
} // Namespace AccelByte
