// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteServerApiBase.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteWatchdog, Warning, All);

namespace AccelByte
{
class AccelByteWebSocket;
class ServerCredentials;
class ServerSettings;
class FHttpRetryScheduler;

namespace GameServerApi
{

/**
 * @brief Watchdog API for communication from dedicated server to backend.
 */
class ACCELBYTEUE4SDK_API ServerWatchdog : public FServerApiBase
{
public:
	DECLARE_DELEGATE(FConnectSuccess);
	DECLARE_DELEGATE(FOnWatchdogDrainReceived);
	DECLARE_DELEGATE_OneParam(FConnectError, const FString& /* Message */);
	DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);

	ServerWatchdog(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef);

	~ServerWatchdog();

	/**
	 * @brief Connect to the Watchdog websocket.
	 */
	void Connect();

	/**
	 * @brief Disconnect from Watchdog if already connected.
	 */
	void Disconnect(bool bForceCleanup = false);

	/**
	 * @brief Check whether the websocket is currently connected to the Watchdog.
	 *
	 * @return true if it's connected, false otherwise.
	 */
	bool IsConnected() const;

	/**
	 * @brief set a delegate to be triggered when successfully connect to Watchdog.
	 *
	 * @param OnConnectSuccess delegate to set.
	 */
	void SetOnConnectSuccess(const FConnectSuccess& OnConnectSuccess);

	/**
	 * @brief set a delegate to be triggered when error connecting to Watchdog.
	 *
	 * @param OnConnectError delegate to set.
	 */
	void SetOnConnectError(const FConnectError& OnConnectError);

	/**
	 * @brief set a delegate to be triggered when Watchdog connection is closed.
	 *
	 * @param OnConnectionClosed delegate to set.
	 */
	void SetOnConnectionClosed(const FConnectionClosed& OnConnectionClosed);

	/**
	 *	Unbind delegates
	 */
	void UnbindDelegates();

	/**
	 * @brief send ready message to Watchdog.
	 */
	void SendReadyMessage();

	/**
	 * @brief set delegate to be called when drain message is received.
	 *
	 * @param OnWatchdogDrain delegate to set.
	 */
	void SetOnWatchdogDrainReceivedDelegate(FOnWatchdogDrainReceived OnWatchdogDrain);


private:

	void CreateWebSocket();

	void OnConnected();

	void OnConnectionError(const FString& Error);

	void OnMessage(const FString& Message);

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	void SendHeartbeat();

	bool PeriodicHeartbeat(float DeltaTime);

	TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> WebSocket = nullptr;
	
	FConnectSuccess OnConnectSuccessDelegate;
	FConnectError OnConnectErrorDelegate;
	FConnectionClosed OnConnectionClosedDelegate;
	FOnWatchdogDrainReceived OnWatchdogDrainReceivedDelegate;
	FTickerDelegate WatchdogHeartbeatTickDelegate;
	
	FDelegateHandleAlias WatchdogHeartbeatTickDelegateHandle;
	
	FTimespan WatchdogHeartbeatInterval = FTimespan(0, 0, 15);
	
	bool bHeartbeatJobStarted = false;

};

}
}
