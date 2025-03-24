// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelBytePresenceBroadcastEventModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{
/**
 * @brief Send presence event securely and the user should be logged in first.
 */
class ACCELBYTEUE4SDK_API PresenceBroadcastEvent : public FApiBase, public TSharedFromThis<PresenceBroadcastEvent, ESPMode::ThreadSafe>
{
public:
	PresenceBroadcastEvent(Credentials& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	
	~PresenceBroadcastEvent();

	/**
	 * @brief Get current flight Id.
	 * 
	 * @return Current flight Id
	 */
	FString GetFlightId();

	/**
	 * @brief Check whether game platform info is already sent.
	 *
	 * @return true if it's already send game platform info, false otherwise.
	 */
	bool IsGamePlatformSent();

	/**
	 * @brief Set game state.
	 * It will send presence event immediately.
	 *
	 * @param State Current game state
	 * @param OnSuccess Callback for successful send game state
	 * @param OnError Callback for failed send game state
	 * @param Description Description for current game state
	 */
	void SetGameState(EAccelByteGameState State
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Description = "");

	/**
	 * @brief Set the interval of sending presence event.
	 * By default it sends the queued events once in a ten minute.
	 * Should not be less than 5 seconds.
	 *
	 * @param Interval The interval between telemetry event
	 */
	void SetHeartbeatInterval(FTimespan const& Interval);

	/**
	 * @brief Set a delegate to be triggered when error sending presence event.
	 *
	 * @param OnError delegate to set.
	 */
	void SetOnSendPresenceHeartbeatError(FErrorHandler const& OnError);

	/**
	 * @brief Set a delegate to be triggered when succeeded sending presence event.
	 *
	 * @param OnSuccess delegate to set.
	 */
	void SetOnSendPresenceHeartbeatSuccess(FVoidHandler const& OnSuccess);

	/**
	 * @brief Send platform info event.
	 * 
	 * @param OnSuccess Callback for successful send game platform
	 * @param OnError Callback for failed send game platform
	 */
	void SendPlatform(FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Add presence information
	 *
	 * @param Key Presence event key
	 * @param Value Presence event value
	 */
	void AddPresenceData(FString const& Key, FString const& Value);

	/**
	 * @brief Remove presence information
	 *
	 * @param Key Presence event key
	 */
	void RemovePresenceData(FString const& Key);

	/**
	 * @brief Startup module
	 */
	void Startup();

	/**
	 * @brief Start heartbeat. This funtion is DEPRECATED, will be removed soon.
	 */
	void StartHeartbeat();

	/**
	 * @brief Stop heartbeat. This funtion is DEPRECATED, will be removed soon.
	 */
	void StopHeartbeat();

	/**
	 * @brief Shutdown module
	 */
	void Shutdown();

private:
	PresenceBroadcastEvent() = delete;
	PresenceBroadcastEvent(PresenceBroadcastEvent const&) = delete;
	PresenceBroadcastEvent(PresenceBroadcastEvent&&) = delete;
	TSharedRef<Credentials, ESPMode::ThreadSafe> CredentialsRef;

	bool PeriodicHeartbeat(float DeltaTime);
	void OnLoginSuccess(FOauth2Token const& Response);
	void SendPresenceBroadcastEvent(FAccelBytePresenceBroadcastEventPayload const& Events
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	FTimespan BroadcastInterval = FTimespan(0, 10, 0);
	const FTimespan MinimumBroadcastInterval = FTimespan(0, 0, 5);
	FTickerDelegate PresenceBroadcastEventHeartbeatTickDelegate;
	FDelegateHandleAlias PresenceBroadcastEventHeartbeatTickDelegateHandle;

	FVoidHandler OnSendPresenceHeartbeatSuccess;
	FErrorHandler OnSendPresenceHeartbeatError;

	FDelegateHandle PresenceBroadcastLoginSuccess;

	FAccelBytePresenceBroadcastEventPayload PresencePayload;
	TSharedPtr<FJsonObject> AdditionalPayload;

	FString FlightId{};
	const FString EventName       = TEXT("enhanced_presence");

	EAccelByteGameState GameState = EAccelByteGameState::OUT_OF_GAMEPLAY;

	bool bIsGamePlatformSent = false;
	bool bIsShuttingDown = false;

};

typedef TSharedRef<PresenceBroadcastEvent, ESPMode::ThreadSafe> PresenceBroadcastEventRef;
typedef TSharedPtr<PresenceBroadcastEvent, ESPMode::ThreadSafe> PresenceBroadcastEventPtr;
typedef TWeakPtr<PresenceBroadcastEvent, ESPMode::ThreadSafe> PresenceBroadcastEventWPtr;

} // Namespace Api
} // Namespace AccelByte