// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteCredentials.h"
#include "Core/IWebSocketFactory.h"
#include "Core/AccelByteDefines.h"
#include "IWebSocket.h"

namespace AccelByte
{
	class Credentials;
	class Settings;

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

ENUM_CLASS_FLAGS(EWebSocketEvent);

class ACCELBYTEUE4SDK_API AccelByteWebSocket
{
	public:
	DECLARE_MULTICAST_DELEGATE(FConnectDelegate)
	DECLARE_MULTICAST_DELEGATE_OneParam(FMessageReceiveDelegate, const FString&)
	DECLARE_MULTICAST_DELEGATE_OneParam(FConnectionErrorDelegate, const FString&)
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FConnectionCloseDelegate, const int32, const FString&, const bool)
	
	AccelByteWebSocket(
		const Credentials& Credentials,
		float PingDelay = 30.f,
		float InitialBackoffDelay = 1.f,
		float MaxBackoffDelay = 30.f,
		float TotalTimeout = 60.f
	);
	
	~AccelByteWebSocket();

	FConnectDelegate& OnConnected();
	FMessageReceiveDelegate& OnMessageReceived();
	FConnectionErrorDelegate& OnConnectionError();
	FConnectionCloseDelegate& OnConnectionClosed();

	void Reconnect();

	FTickerDelegate TickerDelegate;
	FDelegateHandleAlias TickerDelegateHandle;

	static TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> Create(
		const FString& Url,
		const FString& Protocol,
		const Credentials& Credentials,
		const TMap<FString, FString>& UpgradeHeaders,
		const TSharedRef<IWebSocketFactory> WebSocketFactory,
		float PingDelay = 30.f,
		float InitialBackoffDelay = 1.f,
		float MaxBackoffDelay = 30.f,
		float TotalTimeout = 60.f
	);

	void UpdateUpgradeHeaders(const FString& Key, const FString& Value);
	
	void Connect();
	void Disconnect(bool ForceCleanup = false);
	bool IsConnected() const;
	void SendPing() const;
	void Send(const FString& Message) const;
	
private:
	bool bConnectTriggered {false};
	TQueue<FString> OnMessageQueue;
	TQueue<FConnectionClosedParams> OnConnectionClosedQueue;
	TQueue<FString> OnConnectionErrorQueue;
	bool bConnectedBroadcasted {false};
	
	FConnectDelegate ConnectDelegate;
	FMessageReceiveDelegate MessageReceiveDelegate;
	FConnectionErrorDelegate ConnectionErrorDelegate;
	FConnectionCloseDelegate ConnectionCloseDelegate;

	const float TickPeriod {0.5f};
	double TimeSinceLastPing {0.0f};
	float TimeSinceLastReconnect {0.0f};
	float TimeSinceConnectionLost {0.0f};
	int BackoffDelay {0};
	int InitialBackoffDelay {0};
	int32 RandomizedBackoffDelay {0};
	float PingDelay {0.0f};
	double TotalTimeout {0.0f};
	int MaxBackoffDelay {0};
	bool bWasWsConnectionError {false};
	bool bDisconnectOnNextTick {false};
	TSharedPtr<IWebSocketFactory> WebSocketFactory;

	const Credentials& Creds;
	
	FString Url;
	FString Protocol;
	TMap<FString, FString> UpgradeHeaders;

	EWebSocketState WsState;
	EWebSocketEvent WsEvents;

	void SetupWebSocket();
	bool Tick(float DeltaTime);
	void OnConnectionConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);
	void OnMessageReceived(const FString& Message);

	TSharedPtr<IWebSocket> WebSocket;
	
	bool StateTick(float DeltaTime);
	bool MessageTick(float DeltaTime);

	AccelByteWebSocket(AccelByteWebSocket const&) = delete; // Copy constructor
	AccelByteWebSocket(AccelByteWebSocket&&) = delete; // Move constructor
	AccelByteWebSocket& operator=(AccelByteWebSocket const&) = delete; // Copy assignment operator
	AccelByteWebSocket& operator=(AccelByteWebSocket &&) = delete; // Move assignment operator
};
}
