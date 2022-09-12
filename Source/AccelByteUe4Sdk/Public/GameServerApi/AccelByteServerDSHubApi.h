// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteDSHubModels.h"
#include "Models/AccelByteMatchmakingModels.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteDSHub, Warning, All);

namespace AccelByte
{
class AccelByteWebSocket;
class ServerCredentials;
class ServerSettings;
class FHttpRetryScheduler;

namespace GameServerApi
{

DECLARE_DELEGATE(FConnectSuccess);
DECLARE_DELEGATE_OneParam(FConnectError, const FString& /* Message */)
DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);
	
DECLARE_DELEGATE_OneParam(FOnServerClaimedNotification, const FAccelByteModelsServerClaimedNotification& /*Notification*/);
DECLARE_DELEGATE_OneParam(FOnV2BackfillProposalNotification, const FAccelByteModelsV2MatchmakingBackfillProposalNotif& /*Notification*/);

/**
 * @brief DS Hub API for communication from dedicated server to backend.
 * Unlike other servers which use HTTP, DS hub server uses WebSocket (RFC 6455).
 */
class ACCELBYTEUE4SDK_API ServerDSHub
{
public:
	ServerDSHub(ServerCredentials& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f);

	~ServerDSHub();

	/**
	 * @brief Connect to the DSHub websocket. Must be logged in with server client credentials to continue.
	 */
	void Connect(const FString& InBoundServerName);

	/**
	 * @brief Disconnect from DSHub if already connected.
	 */
	void Disconnect(bool bForceCleanup = false);

	/**
	 * @brief Check whether the websocket is currently connected to the DSHub server.
	 *
	 * @return true if it's connected, false otherwise.
	 */
	bool IsConnected() const;

	void SetOnConnectSuccess(const FConnectSuccess& OnConnectSuccess);

	void SetOnConnectError(const FConnectError& OnConnectError);

	void SetOnConnectionClosed(const FConnectionClosed& OnConnectionClosed);

	/**
	 * Bind a delegate to listen to server claim notifications from DS hub
	 */
	void SetOnServerClaimedNotificationDelegate(const FOnServerClaimedNotification& InDelegate);

	/**
	 * Bind a delegate to listen to backfill proposal notifications from DS hub
	 */
	void SetOnV2BackfillProposalNotificationDelegate(const FOnV2BackfillProposalNotification& InDelegate);

	/**
	 *	Unbind notification delegates
	 */
	void UnbindDelegates();

private:
	ServerCredentials& CredentialsRef;
	ServerSettings const& SettingsRef;
	FHttpRetryScheduler& HttpRef;

	float PingDelay{};
	float InitialBackoffDelay{};
	float MaxBackoffDelay{};
	float TotalTimeout{};

	/**
	 * Header key for specifying the server name to connect to the DS hub socket with.
	 */
	const FString DSHubServerNameHeader = TEXT("X-Ab-ServerID");
	
	/**
	 * Server name that is associated with this DS hub connection. Set on connection.
	 */
	FString BoundServerName = TEXT("");

	/**
	 * Websocket instance that is connected to the DSHub server.
	 */
	TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> WebSocket = nullptr;

	/**
	 * Delegate fired when successfully connected to DS Hub.
	 */
	FConnectSuccess OnConnectSuccessDelegate{};

	/**
	 * Delegate fired when failed connecting to DS Hub.
	 */
	FConnectError OnConnectErrorDelegate{};

	/**
	 * Delegate fired when connection to DS Hub is closed (either by server or client).
	 */
	FConnectionClosed OnConnectionClosedDelegate{};

	/**
	 * Delegate fired when the server associated with this DS hub connection is claimed by a session.
	 */
	FOnServerClaimedNotification OnServerClaimedNotification{};

	/**
	 * Delegate fired when the server associated with this DS hub connection receives a backfill proposal from matchmaking V2.
	 */
	FOnV2BackfillProposalNotification OnV2BackfillProposalNotification{};

	/**
	 * Create an instance of our websocket to connect to the DSHub server.
	 */
	void CreateWebSocket();

	/**
	 * Delegate handler for when our DSHub websocket successfully connects
	 */
	void OnConnected();

	/**
	 * Delegate handler for when our DSHub websocket fails to connect
	 */
	void OnConnectionError(const FString& Error);

	/**
	 * Delegate handler for when we receive a message from the DSHub
	 */
	void OnMessage(const FString& Message);

	/**
	 * Delegate fired when our websocket closes
	 */
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

};

}
}
