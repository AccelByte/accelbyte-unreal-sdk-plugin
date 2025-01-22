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
typedef TSharedPtr< FAccelByteModelsTelemetryBody, ESPMode::ThreadSafe> TelemetryBodyPtr;

/**
 * @brief Send telemetry data securely and the user should be logged in first.
 */
class ACCELBYTEUE4SDK_API GameTelemetry : public FApiBase, public TSharedFromThis<GameTelemetry, ESPMode::ThreadSafe>
{
public:
	GameTelemetry(Credentials& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, bool bInCacheEvent = true, bool bInRetryOnFailed = false);
	virtual ~GameTelemetry();

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

	/**
	 * @brief Set a flag to cache events.
	 * 
	 * @param bShouldCache boolean flag to cache events
	 */
	void ShouldCacheEnabled(bool bShouldCache);

	/**
	 * @brief Check the flag value to cache events
	 */
	bool IsCacheEnabled() const;

private:
	void SendProtectedEvents(TArray<TelemetryBodyPtr> const& Events
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	bool PeriodicTelemetry(float DeltaTime);
	
	void LoadCachedEvents();
	
	void AppendEventToCache(TelemetryBodyPtr Telemetry);
	
	void OnLoginSuccess(FOauth2Token const& Response);

	void OnLogoutSuccess();
	
	void RemoveEventsFromCache(TArray<TelemetryBodyPtr> const& Events);
	
	bool JobArrayQueueAsJsonString(FString& OutJsonString);

	const FString GetEventNamespace();
	
protected:
	bool EventsJsonToArray(FString& InJsonString
		, TArray<TelemetryBodyPtr>& OutArray);
	
	virtual FString GetTelemetryKey();

private:
	GameTelemetry() = delete;
	GameTelemetry(GameTelemetry const&) = delete;
	GameTelemetry(GameTelemetry&&) = delete;

	TSharedRef<Credentials, ESPMode::ThreadSafe> CredentialsRef;

	FTimespan TelemetryInterval = FTimespan(0, 1, 0);
	TSet<FString> ImmediateEvents;
	TSet<FString> CriticalEvents;
	
	bool bCacheUpdated = false;
	TQueue<TTuple<TelemetryBodyPtr, FVoidHandler, FErrorHandler>> JobQueue{};
	FString EventPtrArrayAccumulation{};//Contains EventPtrArray that will be written to cache, update by EventPtrArrayAccumulation
	TArray<TelemetryBodyPtr> EventPtrArray;
	mutable FCriticalSection EventPtrArrayLock;

	bool bTelemetryJobStarted = false;
	FTimespan const MINIMUM_INTERVAL_TELEMETRY = FTimespan(0, 0, 5);
	FTickerDelegate GameTelemetryTickDelegate;
	FDelegateHandleAlias GameTelemetryTickDelegateHandle;
	FDelegateHandle GameTelemetryLoginSuccess;
	FDelegateHandle GameTelemetryLogoutSuccess;

	bool ShuttingDown;
	bool bCacheEvent;
	bool bRetryOnFailed;

	FString EventNamespace;
};

typedef TSharedRef<GameTelemetry, ESPMode::ThreadSafe> GameTelemetryRef;
typedef TSharedPtr<GameTelemetry, ESPMode::ThreadSafe> GameTelemetryPtr;
typedef TWeakPtr<GameTelemetry, ESPMode::ThreadSafe> GameTelemetryWPtr;

} // Namespace Api
} // Namespace AccelByte
