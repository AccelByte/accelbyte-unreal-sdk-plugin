// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteWebSocket.h"
#include "Core/AccelByteServerApiBase.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteAMS, Warning, All);

namespace AccelByte
{
class AccelByteWebSocket;
class ServerCredentials;
class ServerSettings;
class FHttpRetryScheduler;

namespace GameServerApi
{

/**
 * @brief API for communication from dedicated server to AMS.
 */
class ACCELBYTEUE4SDK_API ServerAMS : public FServerApiBase, public IWebsocketConfigurableReconnectStrategy
{
public:
	DECLARE_DELEGATE(FConnectSuccess);
	DECLARE_DELEGATE(FOnAMSDrainReceived);
	DECLARE_DELEGATE_OneParam(FConnectError, const FString& /* Message */);
	typedef AccelByteWebSocket::FConnectionCloseDelegate FConnectionClosed;
	typedef AccelByteWebSocket::FReconnectAttemptMulticastDelegate FReconnectAttempted;
	typedef AccelByteWebSocket::FMassiveOutageMulticastDelegate FMassiveOutage;

	ServerAMS(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef);

	~ServerAMS();

	/**
	 * @brief Connect to the AMS websocket.
	 */
	void Connect();

	/**
	 * @brief Disconnect from AMS if already connected.
	 */
	void Disconnect(bool bForceCleanup = false);

	/**
	 * @brief Check whether the websocket is currently connected to the AMS.
	 *
	 * @return true if it's connected, false otherwise.
	 */
	bool IsConnected() const;

	/**
	 * @brief set a delegate to be triggered when successfully connect to AMS.
	 *
	 * @param OnConnectSuccess delegate to set.
	 */
	void SetOnConnectSuccess(const FConnectSuccess& OnConnectSuccess);

	/**
	 * @brief set a delegate to be triggered when error connecting to AMS.
	 *
	 * @param OnConnectError delegate to set.
	 */
	void SetOnConnectError(const FConnectError& OnConnectError);

	/**
	 * @brief set a delegate to be triggered when AMS connection is closed.
	 *
	 * @param OnConnectionClosed delegate to set.
	 */
	void SetOnConnectionClosed(const FConnectionClosed& OnConnectionClosed);

	/**
	 *	Unbind delegates
	 */
	void UnbindDelegates();

	/**
	 * @brief send ready message to AMS.
	 */
	void SendReadyMessage();

	/**
	 * @brief set delegate to be called when drain message is received.
	 *
	 * @param OnAMSDrain delegate to set.
	 */
	void SetOnAMSDrainReceivedDelegate(FOnAMSDrainReceived OnAMSDrain);

	/**
	 * @brief set DS session timeout to the new number given.
	 * 
	 * @param NewTimeout new number for DS session timeout
	 */
	void SetDSTimeout(int32 NewTimeout);

	/**
	 * @brief reset DS session timeout to the one that already set in the fleet settings.
	 */
	void ResetDSTimeout();

	/**
	 * @brief Get a multicast delegate that will be triggered when an attempt to reconnect websocket has been done.
	 */
	FReconnectAttempted& OnReconnectAttemptedMulticastDelegate()
	{
		return ReconnectAttempted;
	}

	/**
	 * @brief Get a multicast delegate that will be triggered when connection is down & can't be reestablished. Longer than usual.
	 */
	FMassiveOutage& OnMassiveOutageMulticastDelegate()
	{
		return MassiveOutage;
	}

private:

	void CreateWebSocket();

	void OnConnected();

	void OnConnectionError(const FString& Error);

	void OnMessage(const FString& Message);

	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	void OnReconnectAttempt(FReconnectAttemptInfo const& ReconnectAttemptInfo);

	void OnMassiveOutage(FMassiveOutageInfo const& MassiveOutageInfo);

	void SendHeartbeat();

	bool PeriodicHeartbeat(float DeltaTime);

	TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> WebSocket = nullptr;
	
	FConnectSuccess OnConnectSuccessDelegate;
	FConnectError OnConnectErrorDelegate;
	FConnectionClosed OnConnectionClosedDelegate;
	FOnAMSDrainReceived OnAMSDrainReceivedDelegate;
	FTickerDelegate AMSHeartbeatTickDelegate;
	FReconnectAttempted ReconnectAttempted;
	FMassiveOutage MassiveOutage;
	
	FDelegateHandleAlias AMSHeartbeatTickDelegateHandle;
	
	FTimespan AMSHeartbeatInterval = FTimespan(0, 0, 15);

	FString AMSWatchdogUrl = TEXT("ws://localhost:5555/watchdog");
	
	bool bHeartbeatJobStarted = false;

	float PingDelay = 30.f;
	float InitialBackoffDelay = 1.f;
	float MaxBackoffDelay = 30.f;
};

}
}
