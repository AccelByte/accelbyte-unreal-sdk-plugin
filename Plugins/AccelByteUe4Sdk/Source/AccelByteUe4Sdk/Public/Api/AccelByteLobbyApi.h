// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"

// Forward declarations
class IWebSocket;

namespace AccelByte
{
namespace Api
{
	
/**
 * @brief Lobby API for chatting and party management.
 * Unlike other servers which use HTTP, Lobby server uses WebSocket (RFC 6455).
 */
class ACCELBYTEUE4SDK_API Lobby
{
public:
	/**
	 * @brief Get instance of this singleton class.
	 */
	static Lobby& Get();
	DECLARE_DELEGATE(FConnectSuccess);
	DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);
	/**
	 * @brief Connect to the Lobby server via websocket. You must connect to the server before you can start sending/receiving. Also make sure you have logged in first as this operation requires access token.
	 * 
	 * @param OnSuccess Callback for when the connection was successful.
	 * @param OnError Callback for when the connection was unsuccessful.
	 * @param OnConnectionClosed Callback for when the connection was closed. The callback takes 3 parameter: int32 StatusCode, const FString& Reason, and bool Was Clean.
	 */
	void Connect(const FConnectSuccess& OnSuccess, const FErrorHandler& OnError, const FConnectionClosed& OnConnectionClosed);

	/**
	 * @brief Disconnect from server if and only if the you have connected to server. If not currently connected, then this does nothing.
	 */
	void Disconnect();

	/**
	 * @brief Check whether the websocket is currently connected to the Lobby server.
	 * 
	 * @return true if it's connected, false otherwise.
	 */
	bool IsConnected() const;
	
	/**
	 * @brief Send ping; you should call this every some time (for example every 4 seconds) so that the server doesn't close the connection.
	 * Note that the UE4 WebSocket library (which is just a simple implementation of libwebsockets.org) doesn't support sending ping control frame (RFC 6455 Section 5.5.2). This is LWS_WRITE_PING in libwebsockets.org.
	 * So we just send a text data frame (RFC 6455 Section 5.6) containing empty text. This is LWS_WRITE_TEXT in libwebsockets.org.
	 */
	void SendPing();

	/**
	 * @brief Send a private message to another user.
	 * 
	 * @param UserId The recipient's user ID.
	 * @param Message Message to be sent.
	 */
	void SendPrivateMessage(const FString& UserId, const FString& Message);

	/**
	 * @brief Send a message to other party members.
	 * 
	 * @param Message Message to be sent.
	 */
	void SendPartyMessage(const FString& Message);
	
	/**
	 * @brief Get information about current party.
	 */
	void SendInfoPartyRequest();

	/**
	 * @brief Create a party.
	 */
	void SendCreatePartyRequest();

	/**
	 * @brief Leave current party.
	 */
	void SendLeavePartyRequest();

	/**
	 * @brief Invite a user to party.
	 * 
	 * @param UserId The target user ID to be invited.
	 */
	void SendInviteToPartyRequest(const FString& UserId);

	/**
	 * @brief Accept a party invitation.
	 * 
	 * @param PartyId Party ID from the invitation notice.
	 * @param InvitationToken Random string from the invitation notice.
	 */
	void SendAcceptInvitationRequest(const FString& PartyId, FString InvitationToken);

	/**
	 * @brief Get a list of online users in the Lobby server.
	 */
	void SendGetOnlineUsersRequest();

	DECLARE_DELEGATE_OneParam(FPrivateMessageNotice, const FAccelByteModelsPrivateMessageNotice&);
	DECLARE_DELEGATE_OneParam(FPartyMessageNotice, const FAccelByteModelsPartyMessageNotice&);
	DECLARE_DELEGATE_OneParam(FInfoPartyResponse, const FAccelByteModelsInfoPartyResponse&);
	DECLARE_DELEGATE_OneParam(FCreatePartyResponse, const FAccelByteModelsCreatePartyResponse&);
	DECLARE_DELEGATE_OneParam(FLeavePartyResponse, const FAccelByteModelsLeavePartyResponse&);
	DECLARE_DELEGATE_OneParam(FInviteToPartyResponse, const FAccelByteModelsInviteToPartyResponse&);
	DECLARE_DELEGATE_OneParam(FPartyInvitationNotice, const FAccelByteModelsPartyInvitationNotice&);
	DECLARE_DELEGATE_OneParam(FAcceptInvitationResponse, const FAccelByteModelsAcceptInvitationReponse&);
	DECLARE_DELEGATE_OneParam(FPartyInvitationAcceptanceNotice, const FAccelByteModelsPartyInvitationAcceptanceNotice&);
	DECLARE_DELEGATE_OneParam(FGetOnlineUsersResponse, const FAccelByteModelsGetOnlineUsersResponse&);

	/**
	 * @brief You must bind delegates/callbacks first to handle the events.
	 * For example when a user received a private message or a response to create party request.
	 * A delegate which ends with Notice means that it's like a notification, while one which ends with Response means it's like a response to a request.
	 * The delegates can be `nullptr` if you want to not bind the callback. All delegates have one parameter `Result` with different types.
	 * 
	 * @param OnPrivateMessageNotice Called when a private message was received.
	 * @param OnPartyMessageNotice Called when a party message was received.
	 * @param OnInfoPartyResponse Called when server has responsed to "get party information".
	 * @param OnCreatePartyResponse Called when server has responsed to "create party".
	 * @param OnLeavePartyResponse Called when server has responsed to "leave party".
	 * @param OnInviteToPartyResponse Called when server has responsed to "invite a user to party".
	 * @param OnPartyInvitationNotice Called when you receive an invitation to join other's party.
	 * @param OnAcceptInvitationResponse Called when server has responsed to "accept a party invitation".
	 * @param OnPartyInvitationAcceptanceNotice Called when your party invitation has been accepted.
	 * @param OnGetOnlineUsersResponse Called when server has responsed to "get online users".
	 */
	void BindDelegates(
		const FPrivateMessageNotice& OnPrivateMessageNotice,
		const FPartyMessageNotice& OnPartyMessageNotice,
		const FInfoPartyResponse& OnInfoPartyResponse,
		const FCreatePartyResponse& OnCreatePartyResponse,
		const FLeavePartyResponse& OnLeavePartyResponse,
		const FInviteToPartyResponse& OnInviteToPartyResponse,
		const FPartyInvitationNotice& OnPartyInvitationNotice,
		const FAcceptInvitationResponse& OnAcceptInvitationResponse,
		const FPartyInvitationAcceptanceNotice& OnPartyInvitationAcceptanceNotice,
		const FGetOnlineUsersResponse& OnGetOnlineUsersResponse
	);
	/**
	 * @brief Unbind all delegates set previously.
	 */
	void UnbindDelegates();
private:
	Lobby();
	~Lobby();
	Lobby(Lobby const&) = delete; // Copy constructor
	Lobby(Lobby&&) = delete; // Move constructor
	Lobby& operator=(Lobby const&) = delete; // Copy assignment operator
	Lobby& operator=(Lobby &&) = delete; // Move assignment operator

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessage(const FString& Message);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);

	TSharedPtr<IWebSocket> WebSocket;
	FConnectSuccess ConnectSuccess;
	FErrorHandler ConnectError;
	FConnectionClosed ConnectionClosed;
	
	FPrivateMessageNotice PrivateMessageNotice;
	FPartyMessageNotice PartyMessageNotice;
	FInfoPartyResponse InfoPartyResponse;
	FCreatePartyResponse CreatePartyResponse;
	FLeavePartyResponse LeavePartyResponse;
	FInviteToPartyResponse InviteToPartyResponse;
	FPartyInvitationNotice PartyInvitationNotice;
	FAcceptInvitationResponse AcceptInvitationResponse;
	FPartyInvitationAcceptanceNotice PartyInvitationAcceptanceNotice;
	FGetOnlineUsersResponse GetOnlineUsersResponse;
};

} // Namespace Api
} // Namespace AccelByte
