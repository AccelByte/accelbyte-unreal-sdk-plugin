// Copyright (c) 2021 - 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteCredentials.h"
#include "Core/IWebSocketFactory.h"
#include "Core/AccelByteDefines.h"
#include "Core/AccelByteWebSocketErrorTypes.h"
#include "AccelByteWebSocketReconnection.h"
#include "IWebSocket.h"

namespace AccelByte
{
	class Credentials;
	class Settings;
	class ServerCredentials;

enum class EWebSocketState
{
	Closed = 0,
	Connecting = 1,
	Connected = 2,
	Closing = 3,
	Reconnecting = 4,
	WaitingReconnect = 5
};

enum class EWebSocketEvent : uint8
{
	None = 0,
	Connect = 1,
	Connected = 2,
	Close = 4,
	Closed = 8,
	ConnectionError = 16
};

struct FConnectionClosedParams
{
	int32 Code;
	FString Reason;
	bool WasClean;	
};

struct FReconnectAttemptInfo
{
	uint32 AttemptCount = 0;
	FTimespan NextRetryAttemptIn{ 0 };
};

struct FMassiveOutageInfo
{
	FTimespan TotalTimeoutDuration{ 0 };
};

ENUM_CLASS_FLAGS(EWebSocketEvent);

class ACCELBYTEUE4SDK_API AccelByteWebSocket :
    public TSharedFromThis<AccelByteWebSocket, ESPMode::ThreadSafe> {
public:
	// To be used by Lobby, Chat, and DSM. Using typedef is possible too.
	DECLARE_DELEGATE_ThreeParams(FConnectionCloseDelegate, int32 /* StatusCode */, FString const& /* Reason */, bool /* WasClean */);

	// To be used as multicast delegate (member of this WebSocket class)
	DECLARE_MULTICAST_DELEGATE(FConnectDelegate)
	DECLARE_MULTICAST_DELEGATE_OneParam(FMessageReceiveDelegate, const FString&)
	DECLARE_MULTICAST_DELEGATE_OneParam(FConnectionErrorDelegate, const FString&)
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FConnectionCloseMulticastDelegate, const int32, const FString&, const bool)
	DECLARE_MULTICAST_DELEGATE_OneParam(FReconnectAttemptMulticastDelegate, const FReconnectAttemptInfo&)
	DECLARE_MULTICAST_DELEGATE_OneParam(FMassiveOutageMulticastDelegate, const FMassiveOutageInfo&)
	
	AccelByteWebSocket(
		const Credentials& Credentials,
		IWebsocketConfigurableReconnectStrategy& ParentReconnectionStrategyRef,
		float PingDelay = 30.f
	);
	
	AccelByteWebSocket(
		const ServerCredentials& Credentials,
		IWebsocketConfigurableReconnectStrategy& ParentReconnectionStrategyRef,
		float PingDelay = 30.f
	);

	~AccelByteWebSocket();

	FConnectDelegate& OnConnected();
	FMessageReceiveDelegate& OnMessageReceived();
	FConnectionErrorDelegate& OnConnectionError();
	FConnectionCloseMulticastDelegate& OnConnectionClosed();
	FReconnectAttemptMulticastDelegate& OnReconnectAttempt();
	FMassiveOutageMulticastDelegate& OnMassiveOutage();

	void Reconnect();

	FTickerDelegate TickerDelegate;
	FDelegateHandleAlias TickerDelegateHandle;

	static TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> Create(
		const FString& Url,
		const FString& Protocol,
		const Credentials& Credentials,
		const TMap<FString, FString>& UpgradeHeaders,
		const TSharedRef<IWebSocketFactory> WebSocketFactory,
		IWebsocketConfigurableReconnectStrategy& InParentReconnectionStrategyRef,
		float PingDelay = 30.f
	);

	/**
	 * @brief Create a websocket using server credentials. Used to create a socket for DSHub.
	 */
	static TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> Create(
		const FString& Url,
		const FString& Protocol,
		const ServerCredentials& Credentials,
		const TMap<FString, FString>& UpgradeHeaders,
		const TSharedRef<IWebSocketFactory> WebSocketFactory,
		IWebsocketConfigurableReconnectStrategy& InParentReconnectionStrategyRef,
		float PingDelay = 30.f
	);

	void UpdateUpgradeHeaders(const FString& Key, const FString& Value);
	
	void Connect(bool ForceConnect = false);
	void Disconnect(bool ForceCleanup = false);
	bool IsConnected() const;
	bool IsReconnecting() const;
	void SendPing() const;
	void Send(const FString& Message) const;

	EWebSocketState GetState() const;

private:
	bool bConnectTriggered {false};
	TQueue<FString> OnMessageQueue;
	TQueue<FConnectionClosedParams> OnConnectionClosedQueue;
	TQueue<FString> OnConnectionErrorQueue;
	TQueue<FReconnectAttemptInfo> OnReconnectingAttemptQueue;
	TQueue<FMassiveOutageInfo> OnMassiveOutageQueue;
	bool bConnectedBroadcasted {false};

	FConnectDelegate ConnectDelegate;
	FMessageReceiveDelegate MessageReceiveDelegate;
	FConnectionErrorDelegate ConnectionErrorDelegate;
	FConnectionCloseMulticastDelegate ConnectionCloseDelegate;
	FReconnectAttemptMulticastDelegate ReconnectAttemptDelegate;
	FMassiveOutageMulticastDelegate MassiveOutageDelegate;

#pragma region RECONNECTION_RELATED_MEMBERS
	uint32 ReconnectingAttemptCount = 0;
	int32 LatestWebsocketDisonnectionCode = 0;
	FReconnectionStrategy& GetCurrentReconnectionStrategy();
	uint32 MassiveOutageReminderCounter = 0; //MassiveOutage delegate shoulde be fired once every that duration recurring. i.e. each 5 minutes
	IWebsocketConfigurableReconnectStrategy& ParentReconnectionStrategyRef;
#pragma endregion RECONNECTION_RELATED_MEMBERS

	const float TickPeriod {0.5f};
	double TimeSinceLastPing {0.0f};
	float TimeSinceLastReconnect {0.0f};
	float TimeSinceConnectionLost {0.0f};
	int BackoffDelay {0};
	int32 RandomizedBackoffDelay {0};
	float PingDelay {0.0f};
	bool bWasWsConnectionError {false};
	bool bDisconnectOnNextTick {false};
	TSharedPtr<IWebSocketFactory> WebSocketFactory;

	/**
	 * @brief Pointer to a client or server credentials instance for authenticating this websocket.
	 * 
	 * NOTE: Do not delete this pointer!
	 */
	TWeakPtr<const BaseCredentials, ESPMode::ThreadSafe> CredentialsWPtr = nullptr;

	FString Url;
	FString Protocol;
	TMap<FString, FString> UpgradeHeaders;

	EWebSocketState WsState = EWebSocketState::Closed;
	EWebSocketEvent WsEvents = EWebSocketEvent::None;

	void SetupWebSocket();
	bool Tick(float DeltaTime);
	void OnConnectionConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);
	void OnMessageReceived(const FString& Message);

	TSharedPtr<IWebSocket> WebSocket;
	
	bool StateTick(float DeltaTime);
	bool MessageTick(float DeltaTime);

	void TeardownTicker();
	void TeardownWebsocket();

	AccelByteWebSocket(AccelByteWebSocket const&) = delete; // Copy constructor
	AccelByteWebSocket(AccelByteWebSocket&&) = delete; // Move constructor
	AccelByteWebSocket& operator=(AccelByteWebSocket const&) = delete; // Copy assignment operator
	AccelByteWebSocket& operator=(AccelByteWebSocket &&) = delete; // Move assignment operator
};
}
