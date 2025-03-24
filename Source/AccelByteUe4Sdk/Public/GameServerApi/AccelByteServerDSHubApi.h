// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteServerApiBase.h"
#include "Core/AccelByteWebSocket.h"
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
DECLARE_DELEGATE_OneParam(FConnectError, FString const& /* Message */);
typedef AccelByteWebSocket::FConnectionCloseDelegate FConnectionClosed;
typedef AccelByteWebSocket::FReconnectAttemptMulticastDelegate FReconnectAttempted;
typedef AccelByteWebSocket::FMassiveOutageMulticastDelegate FMassiveOutage;
	
DECLARE_DELEGATE_OneParam(FOnServerClaimedNotification, FAccelByteModelsServerClaimedNotification const& /*Notification*/);
DECLARE_DELEGATE_OneParam(FOnV2BackfillProposalNotification, FAccelByteModelsV2MatchmakingBackfillProposalNotif const& /*Notification*/);
DECLARE_DELEGATE_OneParam(FOnV2SessionMemberChangedNotification, FAccelByteModelsV2GameSession const& /*Notification*/);
DECLARE_DELEGATE_OneParam(FOnV2SessionEndedNotification, FAccelByteModelsSessionEndedNotification const& /*Notification*/);
DECLARE_DELEGATE_OneParam(FOnV2SessionSecretUpdateNotification, FAccelByteModelsSessionSecretUpdateNotification const& /*Notification*/);
DECLARE_DELEGATE_OneParam(FOnV2BackfillTicketExpiredNotification, FAccelByteModelsV2MatchmakingBackfillTicketExpireNotif const& /*Notification*/);

/**
 * @brief DS Hub API for communication from dedicated server to backend.
 * Unlike other servers which use HTTP, DS hub server uses WebSocket (RFC 6455).
 */
class ACCELBYTEUE4SDK_API ServerDSHub : public FServerApiBase, public IWebsocketConfigurableReconnectStrategy
{
public:
	ServerDSHub(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f);

	explicit ServerDSHub(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f );

	~ServerDSHub();

	/**
	 * @brief Connect to the DSHub websocket. Must be logged in with server client credentials to continue.
	 */
	void Connect(FString const& InBoundServerName);

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

	/**
	 * @brief set a delegate to be triggered when successfully connect to DSHub.
	 *
	 * @param OnConnectSuccess delegate to set.
	 */
	void SetOnConnectSuccess(FConnectSuccess const& OnConnectSuccess);

	/**
	 * @brief set a delegate to be triggered when error connecting to DSHub.
	 *
	 * @param OnConnectError delegate to set.
	 */
	void SetOnConnectError(FConnectError const& OnConnectError);

	/**
	 * @brief set a delegate to be triggered when DSHub connection is closed.
	 *
	 * @param OnConnectionClosed delegate to set.
	 */
	void SetOnConnectionClosed(FConnectionClosed const& OnConnectionClosed);

	/**
	 * @brief Bind a delegate to listen to server claim notifications from DS hub
	 *
	 * @param InDelegate delegate to set.
	 */
	void SetOnServerClaimedNotificationDelegate(FOnServerClaimedNotification const& InDelegate);

	/**
	 * @brief Bind a delegate to listen to backfill proposal notifications from DS hub
	 *
	 * @param InDelegate delegate to set
	 */
	void SetOnV2BackfillProposalNotificationDelegate(FOnV2BackfillProposalNotification const& InDelegate);

	/**
	 * @brief Bind a delegate to listen to game session members changed notifications from DS hub
	 *
	 * @param InDelegate delegate to set
	 */
	void SetOnV2SessionMemberChangedNotificationDelegate(FOnV2SessionMemberChangedNotification const& InDelegate);

	/**
	 * @brief Bind a delegate to listen to game session ended notifications from DS hub
	 *
	 * @param InDelegate delegate to set
	 */
	void SetOnV2SessionEndedNotificationDelegate(FOnV2SessionEndedNotification const& InDelegate);

	/**
	 * @brief Bind a delegate to listen to session secret update from DS hub
	 *
	 * @param InDelegate delegate to set
	 */
	void SetOnV2SessionSecretUpdateNotification(FOnV2SessionSecretUpdateNotification const& InDelegate);

	/**
	 * @brief Bind a delegate to listen to backfill ticket expired notifications from DS hub
	 *
	 * @param InDelegate delegate to set
	 */
	void SetOnV2BackfillTicketExpiredNotificationDelegate(FOnV2BackfillTicketExpiredNotification const& InDelegate);

	/**
	 *	Unbind notification delegates
	 */
	void UnbindDelegates();

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
	float PingDelay{};
	float InitialBackoffDelay{};
	float MaxBackoffDelay{};
	float TotalTimeout{};

	/**
	 * Header key for specifying the server name to connect to the DS hub socket with.
	 */
	const FString DSHubServerNameHeader = TEXT("X-Ab-ServerID");

	/**
	 * Header key for specifying if we are using AMS or custom DS manager
	 */
	const FString DSHubCustomDSManagerHeader = TEXT("X-Ab-Custom");
	
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
	 * Delegate fired when the server associated with this DS hub connection receives game session member changed from session.
	 */
	FOnV2SessionMemberChangedNotification OnV2SessionMemberChangedNotification{};

	/**
	 * Delegate fired when the game session associated with this DS ended (game session deleted or all of the users left).
	 */
	FOnV2SessionEndedNotification OnV2SessionEndedNotification{};

	/**
	 * Delegate fired when the server associated with this DS hub connection receives session secret.
	 */
	FOnV2SessionSecretUpdateNotification OnV2SessionSecreteUpdateNotification{};

	/**
	 * Delegate fired when the server associated with this DS hub connection receives a backfill ticket expired from matchmaking V2.
	 */
	FOnV2BackfillTicketExpiredNotification OnV2BackfillTicketExpiredNotification{};

	/**
	 * Delegate fired when try to reconnect repetitively to DS Hub websocket.
	*/
	FReconnectAttempted ReconnectAttempted;

	/**
	 * Delegate fired when websocket reconnect getting massive outage.
	*/
	FMassiveOutage MassiveOutage;

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
	void OnConnectionError(FString const& Error);

	/**
	 * Delegate handler for when we receive a message from the DSHub
	 */
	void OnMessage(FString const& Message);

	/**
	 * Delegate fired when our websocket closes
	 */
	void OnClosed(int32 StatusCode, FString const& Reason, bool bWasClean);

	/**
	 * Delegate fired when reconnect attempt
	 */
	void OnReconnectAttempt(FReconnectAttemptInfo const& ReconnectAttemptInfo);

	/**
	 * Delegate fired when reconnect getting massive outage
	 */
	void OnMassiveOutage(FMassiveOutageInfo const& MassiveOutageInfo);

};

}
}
