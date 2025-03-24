// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "JsonDomBuilder.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteWebSocket.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteNetworkConditioner.h"
#include "Models/AccelByteChatModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

enum HandleType : uint8;

/**
 * @brief Chat API for chatting between users
 * Unlike other servers which use HTTP, Chat server uses WebSocket (RFC 6455).
 * @note This chat service features more functionality/capacity than the conventional chat in Lobby API
 * 
 * 
 * Chat service enables user to send message to other user / group of user.
 * We use topic as wrapper for chat connection.
 * Topic can have two different types:
 * 1. Personal
 *		Topic for bidirectional chat between one user to other user (==2)
 * 2. Group
 *		Topic for chat between multiple user (>2)
 *		a. Can be public (join able) and private (not joinable)
 *			- public : user can join by knowing topic id
 *			- private: only admin can add or remove member
 */
class ACCELBYTEUE4SDK_API Chat 
	: public FApiBase
	, public IWebsocketConfigurableReconnectStrategy
	, public TSharedFromThis<Chat, ESPMode::ThreadSafe>
{
public:
	Chat(Credentials & InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler & InHttpRef
		, FAccelByteMessagingSystem & InMessagingSystemRef
		, FAccelByteNetworkConditioner & InNetworkConditionerRef
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f);

	Chat(Credentials & InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler & InHttpRef
		, FAccelByteMessagingSystem & InMessagingSystemRef
		, FAccelByteNetworkConditioner & InNetworkConditionerRef
		, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f);

	~Chat();
private:
	const FString ChatSessionHeaderName = TEXT("X-Ab-ChatSessionID");

	FCredentialsRef ChatCredentialsRef;
	FAccelByteMessagingSystemWPtr MessagingSystemWPtr;
	FAccelByteNetworkConditioner& NetworkConditioner;

	bool bBanNotifReceived = false;
	EBanType BanType = EBanType::EMPTY;

	Chat() = delete;
	Chat(Chat const&) = delete; // Copy constructor
	Chat(Chat&&) = delete; // Move constructor
	Chat& operator=(Chat const&) = delete; // Copy assignment operator
	Chat& operator=(Chat&&) = delete; // Move assignment operator

#pragma region DELEGATE DECLARATIONS
public:

	//				CONNECTIONS
	DECLARE_DELEGATE(FChatConnectSuccess);
	DECLARE_DELEGATE_OneParam(FChatDisconnectNotif, FAccelByteModelsChatDisconnectNotif const&);
	typedef AccelByteWebSocket::FConnectionCloseDelegate FChatConnectionClosed;
	typedef AccelByteWebSocket::FReconnectAttemptMulticastDelegate FReconnectAttempted;
	typedef AccelByteWebSocket::FMassiveOutageMulticastDelegate FMassiveOutage;

	/**
	* @brief delegate for handling response when refreshing chat token.
	*/
	DECLARE_DELEGATE_OneParam(FChatRefreshTokenResponse, FAccelByteModelsChatRefreshTokenResponse const&);

	//				RESPONSES
	/**
	 * @brief delegate for handling sent chat response.
	 */
	DECLARE_DELEGATE_OneParam(FSendChatResponse, FAccelByteModelsChatSendChatResponse const&);

	/**
	 * @brief delegate for handling chat topic query response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryTopicResponse, FAccelByteModelsChatQueryTopicResponse const&);

	/**
	 * @brief delegate for handling chat topic query by id response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryTopicByIdResponse, FAccelByteModelsChatQueryTopicByIdResponse const&);

	/**
	 * @brief delegate for handling public topic query response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryPublicTopicResponse, FAccelByteModelsChatQueryPublicTopicResponse const&);

	/**
	 * @brief delegate for handling chat query response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryChatResponse, FAccelByteModelsChatQueryChatResponse const&);

	/**
	 * @brief delegate for handling read chat response.
	 */
	DECLARE_DELEGATE_OneParam(FReadChatResponse, FAccelByteModelsChatReadChatResponse const&);

	/**
	 * @brief delegate for handling user block response.
	 */
	DECLARE_DELEGATE_OneParam(FChatBlockUserResponse, FAccelByteModelsChatBlockUserResponse const&);

	/**
	 * @brief delegate for handling user unblock response.
	 */
	DECLARE_DELEGATE_OneParam(FChatUnblockUserResponse, FAccelByteModelsChatUnblockUserResponse const&);

	/**
	 * @brief delegate for handling action chat topic response (create, update, delete, join, quit)
	 */
	DECLARE_DELEGATE_OneParam(FChatActionTopicResponse, FAccelByteModelsChatActionTopicResponse const&);

	/**
	 * @brief delegate for handling add user to topic and remove user from topic response.
	 */
	DECLARE_DELEGATE_OneParam(FAddRemoveUserFromTopicResponse, FAccelByteModelsChatActionUserTopicResponse const&);

	/**
	 * @brief delegate for handling delete system message(s) response.
	 */
	DECLARE_DELEGATE_OneParam(FDeleteSystemMessagesResponse, FAccelByteModelsDeleteSystemMessagesResponse const&);

	/**
	 * @brief delegate for handling update system message(s) response.
	 */
	DECLARE_DELEGATE_OneParam(FUpdateSystemMessagesResponse, FAccelByteModelsUpdateSystemMessagesResponse const&);

	/**
	 * @brief delegate for handling query system message response.
	 */
	DECLARE_DELEGATE_OneParam(FQuerySystemMessageResponse, FAccelByteModelsQuerySystemMessagesResponse const&);

	/** Delegate for handling get system message stats response. */
	DECLARE_DELEGATE_OneParam(FGetSystemMessageStatsResponse, FAccelByteGetSystemMessageStatsResponse const&);

	/** Delegate for handling get user chat configuration response. */
	DECLARE_DELEGATE_OneParam(FGetUserChatConfigurationResponse, FAccelByteModelsGetUserChatConfigurationResponse const&);

	/** Delegate for handling set user chat configuration response. */
	DECLARE_DELEGATE_OneParam(FSetUserChatConfigurationResponse, FAccelByteSetUserChatConfigurationResponse const&);

	//				NOTIFICATIONS
	/**
	 * @brief delegate for handling incoming chat notification.
	 */
	DECLARE_DELEGATE_OneParam(FChatNotif, FAccelByteModelsChatNotif const&);

	/**
	 * @brief delegate for handling read chat notification.
	 */
	DECLARE_DELEGATE_OneParam(FReadChatNotif, FAccelByteModelsReadChatNotif const&);

	/**
	 * @brief delegate for handling add to/ remove from topic notification.
	 */
	DECLARE_DELEGATE_OneParam(FAddRemoveFromTopicNotif, FAccelByteModelsChatUpdateUserTopicNotif const&);

	/**
	 * @brief delegate for handling chat topic removal/update notification.
	 */
	DECLARE_DELEGATE_OneParam(FDeleteUpdateTopicNotif, FAccelByteModelsChatUpdateTopicNotif const&);

	/**
	 * @brief delegate for handling chat incoming system messages.
	 */
	DECLARE_DELEGATE_OneParam(FSystemMessageNotif, FAccelByteModelsChatSystemMessageNotif const&);

	/*
	* @brief delegate for handling ban / unban notification
	*/
	DECLARE_DELEGATE_OneParam(FUserBanUnbanNotif, FAccelByteModelsChatUserBanUnbanNotif const&);

	/*
	* @brief delegate for handling chat muted notification
	*/
	DECLARE_DELEGATE_OneParam(FUserMutedNotif, FAccelByteModelsChatMutedNotif const&);

	/*
	* @brief delegate for handling chat unmuted notification
	*/
	DECLARE_DELEGATE_OneParam(FUserUnmutedNotif, FAccelByteModelsChatUnmutedNotif const&);;

#pragma endregion DELEGATE DECLARATIONS

#pragma region WEB SOCKET CONNECTIONS

public:

	/**
	 * @brief Connect to the Chat server via websocket.
	 * @note You must connect to the server before you can start sending/receiving.
	 * Also make sure you have logged in first as this operation requires access token.
	 */
	void Connect();

	/**
	 * @brief Disconnect from server if and only if the you have connected to server. If not currently connected, then this does nothing.
	 */
	void Disconnect();

	/**
	 * @brief Check whether the websocket is currently connected to the Chat server.
	 *
	 * @return true if it's connected, false otherwise.
	 */
	bool IsConnected() const;

	/**
	 * @brief Send and empty string through the web socket connection
	 */
	void SendPing();

	/**
	 * @brief Delegate setter for chat websocket connection success event
	 *
	 * @param OnConnectSuccess Callback delegate when successfully connected to Chat Websocket.
	 */
	void SetConnectSuccessDelegate(FChatConnectSuccess const& OnConnectSuccess)
	{
		ConnectSuccess = OnConnectSuccess;
	}

	/**
	 * @brief Delegate setter for chat websocket connection failure event
	 *
	 * @param OnConnectError Callback delegate when failed connecting to Chat Websocket.
	 */
	void SetConnectFailedDelegate(FErrorHandler const& OnConnectError)
	{
		ConnectError = OnConnectError;
	}

	/**
	 * @brief Delegate setter for chat websocket disconnected event
	 *
	 * @param OnDisconnectNotif Notification Delegate when disconnected from Chat Websocket. 
	 */
	void SetDisconnectNotifDelegate(FChatDisconnectNotif const& OnDisconnectNotif)
	{
		DisconnectNotif = OnDisconnectNotif;
	}

	/**
	 * @brief Delegate setter for chat websocket connection closed event (connection closed by peer)
	 *
	 * @param OnConnectionClosed Callback delegate when connection closed from Chat Websocket.
	 */
	void SetConnectionClosedDelegate(FChatConnectionClosed const& OnConnectionClosed)
	{
		ConnectionClosed = OnConnectionClosed;
	}

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

	const float PingDelay;
	bool bWasWsConnectionError = false;
	TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> WebSocket;
	FAccelByteModelsChatConnectNotif ChatSessionId;

	const FString WsEnvelopeStart { TEXT("CaSr") };
	const FString WsEnvelopeEnd { TEXT("CaEd") };
	FString EnvelopeContentBuffer;

	FChatConnectSuccess ConnectSuccess;
	FErrorHandler ConnectError;
	FErrorHandler ParsingError;
	FChatDisconnectNotif DisconnectNotif;
	FChatConnectionClosed ConnectionClosed;
	FChatConnectionClosed Reconnecting;
	FDelegateHandle AuthTokenSetDelegateHandle;
	FReconnectAttempted ReconnectAttempted;
	FMassiveOutage MassiveOutage;

	void OnConnected();
	void OnConnectionError(FString const& Error);
private:
	void OnMessage(FString const& Message);
	void OnClosed(int32 StatusCode, FString const& Reason, bool WasClean);
	void OnReconnectAttempt(FReconnectAttemptInfo const& ReconnectAttemptInfo);
	void OnMassiveOutage(FMassiveOutageInfo const& MassiveOutageInfo);
	void CreateWebSocket();
	
	FString SendWebSocketContent(FString const& Method, TSharedRef<FJsonObject> const& Params);
	FString SendWebSocketContent(FString const& Method, FJsonDomBuilder::FObject const& Params)
	{
		return SendWebSocketContent(Method, Params.AsJsonObject());
	}

#pragma endregion WEB SOCKET CONNECTIONS

#pragma region SESSION

public:
	/**
	* @brief Refresh access token used in chat.
	*
	* @param AccessToken new access token to be used in chat
	* @param OnSuccess delegate to be triggered when operation success
	* @param OnError delegate to be triggered when operation error
	* 
	* @return TBA
	*/
	FString RefreshToken(FString const& AccessToken
		, FChatRefreshTokenResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:

	void SetRefreshTokenResponseDelegate(FChatRefreshTokenResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		RefreshTokenResponse = OnSuccess;
		OnRefreshTokenError = OnError;
	}

#pragma endregion SESSION

#pragma region UNBIND EVENTS

public:

	/**
	 * @brief Unbind all delegates set previously.
	 */
	void UnbindEvent();

private:

	void ClearCachedMessageIdResponseMaps();
	void ClearResponseHandlers();
	void ClearErrorHandlers();
	void ClearNotificationHandlers();

#pragma endregion UNBIND EVENTS

#pragma region CHAT TOPIC

public:
	/**
	 * @brief Create a personal chat topic between logged in user and a TargetUserid (chat members == 2)
	 *
	 * @param TargetUserId - User id for 2nd member of the personal chat topic (1st member is self/logged in user)
	 * @param OnSuccess - Callback for successful chat topic creation response
	 * @param OnError - Callback for failed chat topic creation
	 * @see SetAddToTopicNotifDelegate
	 */
	void CreatePersonalTopic(FString const& TargetUserId
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Create a chat topic between multiple users (chat members > 2). Deprecated.
	 *
	 * @param Users - UserIds to be added to the chat topic, self/logged in user is automatically added as first user
	 * @param Admins - UserIds to be added to the chat topic as admin , self/logged in user is automatically added as admin
	 * @param TopicName - Friendly name of the chat topic (alias)
	 * @param bIsJoinable - Whether new user can join this group topic without invitation from admin
	 * @param OnSuccess - Callback for successful chat topic creation response
	 * @param OnError - Callback for failed chat topic creation
	 * @see SetAddToTopicNotifDelegate
	 * 
	 * @deprecated Manual topic creation is deprecated - please use V2 Sessions to auto-create chat topics!
	 */
	void CreateGroupTopic(TSet<FString> const& Users
		, TSet<FString> const& Admins
		, FString const& TopicName
		, bool bIsJoinable
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		CreateGroupTopic(FAccelByteModelsChatCreateTopicRequest(TopicName, Users, Admins, bIsJoinable)
			, OnSuccess
			, OnError);
	}

	/**
	 * @brief Create a chat topic between multiple users (chat members > 2). Deprecated.
	 *
	 * @param Request - Parameter request in struct form for creating a group topic
	 * @param OnSuccess - Callback for successful chat topic creation response
	 * @param OnError - Callback for failed chat topic creation
	 * @see SetAddToTopicNotifDelegate
	 * 
	 * @deprecated Manual topic creation is deprecated - please use V2 Sessions to auto-create chat topics!
	 */
	void CreateGroupTopic(FAccelByteModelsChatCreateTopicRequest const& Request
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Update the topic information, only topic admin can use this operation
	 *
	 * @param TopicId - To update topic id
	 * @param NewFriendlyTopicName - new alias for the topic id
	 * @param bIsJoinable - Whether new user can join this group topic without invitation from admin
	 * @param OnSuccess - Callback for successful chat topic update response
	 * @param OnError - Callback for failed chat topic update
	 * @see SetUpdateTopicNotifDelegate
	 */
	void UpdateTopic(FString const& TopicId
		, FString const& NewFriendlyTopicName
		, bool bIsJoinable
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		UpdateTopic(FAccelByteModelsChatUpdateTopicRequest(TopicId, NewFriendlyTopicName, bIsJoinable), OnSuccess, OnError);
	}

	/**
	 * @brief Update the topic information, only topic admin can use this operation
	 *
	 * @param Request - Parameter request in struct form for updating a group topic
	 * @param OnSuccess - Callback for successful chat topic update response
	 * @param OnError - Callback for failed chat topic update
	 * @see SetUpdateTopicNotifDelegate
	 */
	void UpdateTopic(FAccelByteModelsChatUpdateTopicRequest const& Request
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Delete the group topic id, only topic admin can use this operation. PERSONAL topic cannot be deleted
	 *
	 * @param ToDeleteGroupTopicId - Group Topic id that will be deleted. PERSONAL topic cannot be deleted
	 * @param OnSuccess - Callback for successful group chat topic deletion response
	 * @param OnError - Callback for failed chat topic deletion
	 * @see SetDeleteTopicNotifDelegate
	 */
	void DeleteTopic(FString const& ToDeleteGroupTopicId
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Add a new user to an existing GROUP topic id, only topic admin can use this operation
	 *
	 * @param Request - Parameter request in struct form for adding a user to a group topic
	 * @param OnSuccess - Callback for successful chat topic user addition response
	 * @param OnError - Callback for failed chat topic user addition
	 * @see SetAddToTopicNotifDelegate
	 */
	void AddUserToTopic(FAccelByteModelsChatAddUserToTopicRequest const& Request
		, FAddRemoveUserFromTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Add an user to an existing topic id, only topic admin can use this operation
	 *
	 * @param TopicId - Topic id that will receive the new user
	 * @param ToAddUserId - User Id that will be added to the topic
	 * @param OnSuccess - Callback for successful chat topic user addition response
	 * @param OnError - Callback for failed chat topic user addition
	 * @see SetAddToTopicNotifDelegate
	 */
	void AddUserToTopic(FString const& TopicId
		, FString const& ToAddUserId
		, FAddRemoveUserFromTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		AddUserToTopic(FAccelByteModelsChatAddUserToTopicRequest(TopicId, ToAddUserId), OnSuccess, OnError);
	}

	/**
	 * @brief Remove an user from an existing topic id, only topic admin can use this operation
	 *
	 * @param Request - Parameter request in struct form for removing an existing member from a group topic
	 * @param OnSuccess - Callback for successful chat topic user removal response
	 * @param OnError - Callback for failed chat topic user removal
	 * @see SetRemoveFromTopicNotifDelegate
	 */
	void RemoveUserFromTopic(FAccelByteModelsChatRemoveUserFromTopicRequest const& Request
		, FAddRemoveUserFromTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Remove an user from an existing topic id, only topic admin can use this operation
	 *
	 * @param TopicId - Topic id that will remove an existing user
	 * @param ToRemoveUserId - User Id that will be removed from the topic
	 * @param OnSuccess - Callback for successful chat topic user removal response
	 * @param OnError - Callback for failed chat topic user removal
	 * @see SetRemoveFromTopicNotifDelegate
	 */
	void RemoveUserFromTopic(FString const& TopicId
		, FString const& ToRemoveUserId
		, FAddRemoveUserFromTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		RemoveUserFromTopic(FAccelByteModelsChatRemoveUserFromTopicRequest(TopicId, ToRemoveUserId), OnSuccess, OnError);
	}

	/**
	 * @brief Join a joinable group topic
	 *
	 * @param TopicId - Joinable group topic id
	 * @param OnSuccess - Callback for successful join
	 * @param OnError - Callback for failed join
	 * @see SetJoinTopicResponseDelegate
	 */
	void JoinTopic(FString const& TopicId
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Leave a group topic
	 *
	 * @param TopicId - Group topic id that the logged in user is leaving
	 * @param OnSuccess - Callback for successful quit
	 * @param OnError - Callback for failed quit
	 * @see SetQuitTopicResponseDelegate
	 */
	void QuitTopic(FString const& TopicId
		, FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:

	void SetCreateTopicResponseDelegate(FChatActionTopicResponse const& OnCreateTopicResponse
		, FErrorHandler const& OnError = nullptr)
	{
		CreateTopicResponse = OnCreateTopicResponse;
		OnCreateTopicError = OnError;
	}

	void SetUpdateTopicResponseDelegate(FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		UpdateTopicResponse = OnSuccess;
		OnUpdateTopicError = OnError;
	}

	void SetDeleteTopicResponseDelegate(FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		DeleteTopicResponse = OnSuccess;
		OnDeleteTopicError = OnError;
	}

	void SetAddUserToTopicResponseDelegate(FAddRemoveUserFromTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		AddUserToTopicResponse = OnSuccess;
		OnAddUserToTopicError = OnError;
	}

	void SetRemoveUserFromTopicResponseDelegate(FAddRemoveUserFromTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		RemoveUserFromTopicResponse = OnSuccess;
		OnRemoveUserFromTopicError = OnError;
	}

	void SetJoinTopicResponseDelegate(FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		JoinTopicResponse = OnSuccess;
		OnJoinTopicError = OnError;
	}

	void SetQuitTopicResponseDelegate(FChatActionTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QuitTopicResponse = OnSuccess;
		OnQuitTopicError = OnError;
	}

#pragma endregion CHAT TOPIC

#pragma region SEND CHAT

public:
	/**
	 * @brief Create a personal chat topic between logged in user and a TargetUserid (chat members == 2)
	 *
	 * @param TopicId - a chat topic's topic id as chat destination
	 * @param Message - Chat message to be send
	 * @param OnSuccess - Callback for successful sent chat response
	 * @param OnError - Callback for failed sent chat
	 * @see CreatePersonalTopic, CreateGroupTopic, SetChatNotifDelegate, SetReadChatNotifDelegate
	 */
	void SendChat(FString const& TopicId
		, FString const& Message
		, FSendChatResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:
	void SetSendChatResponseDelegate(FSendChatResponse const& OnSendChatResponse
		, FErrorHandler const& OnError = nullptr)
	{
		SendChatResponse = OnSendChatResponse;
		OnSendChatError = OnError;
	}

#pragma endregion SEND CHAT

#pragma region SYSTEM INBOX

public:
	/**
	 * @brief Delete system message(s) in user system inbox
	 *
	 * @param MessageIds - Array of system message id
	 * @param OnSuccess - Callback for successful deleted system message(s) in user system inbox
	 * @param OnError - Callback for failed deleted system message(s) in user system inbox
	 */
	void DeleteSystemMessages(TSet<FString> const& MessageIds
		, FDeleteSystemMessagesResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Update system message(s) in user system inbox
	 *
	 * @param ActionUpdateSystemMessages - Array of ActionUpdateSystemMessage containing action to mark read/unread and keep
	 * @param OnSuccess - Callback for successful deleted system message(s) in user system inbox
	 * @param OnError - Callback for failed updated system message(s) in user system inbox
	 */
	void UpdateSystemMessages(TArray<FAccelByteModelsActionUpdateSystemMessage> const& ActionUpdateSystemMessages
		, FUpdateSystemMessagesResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Query system messages in user's system inbox
	 *
	 * @param OnSuccess - Callback for successful deleted system message(s) in user system inbox
	 * @param OnError - Callback for failed updated system message(s) in user system inbox
	 * @param OptionalParams - Optional parameters for query inbox message
	 */
	void QuerySystemMessage(FQuerySystemMessageResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr
		, FQuerySystemMessageOptions const& OptionalParams = {});

	/**
	 * Get system message stats (number of unread messages and date time of oldest unread message)
	 *
	 * @param OnSuccess - Callback for successful get system message stats
	 * @param OnError - Callback for failed get system message stats
	 * @param Request - Optional request parameters
	 */
	void GetSystemMessageStats(FGetSystemMessageStatsResponse const& OnSuccess
		, FErrorHandler const& OnError
		, FAccelByteGetSystemMessageStatsRequest const& Request = {});

private:
	void SetDeleteSystemMessagesResponseDelegate(FDeleteSystemMessagesResponse const& OnDeleteInboxMessagesResponse
		, FErrorHandler const& OnError = nullptr)
	{
		DeleteSystemMessagesResponse = OnDeleteInboxMessagesResponse;
		OnDeleteSystemMessagesError = OnError;
	}

	void SetUpdateSystemMessagesResponseDelegate(FUpdateSystemMessagesResponse const& OnUpdateSystemMessagesResponse
		, FErrorHandler const& OnError = nullptr)
	{
		UpdateSystemMessagesResponse = OnUpdateSystemMessagesResponse;
		OnUpdateSystemMessagesError = OnError;
	}

	void SetQuerySystemMessageResponseDelegate(FQuerySystemMessageResponse const& OnQuerySystemMessagesResponse
			, FErrorHandler const& OnError = nullptr)
	{
		QuerySystemMessageResponse = OnQuerySystemMessagesResponse;
		OnUpdateSystemMessagesError = OnError;
	}

	void SetGetSystemMessageStatsResponseDelegate(FGetSystemMessageStatsResponse const& OnGetSystemMessageStatsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetSystemMessageStatsResponse = OnGetSystemMessageStatsResponse;
		OnGetSystemMessageStatsError = OnError;
	}

#pragma endregion SYSTEM INBOX

#pragma region QUERY TOPIC

public:

	/**
	 * @brief Query PERSONAL chat topics that the logged in user is a member of
	 *
	 * if user has sequential 100 topics, 1 is oldest, 100 is most recent
	 * result will be returned from most recent. so in this order: 100 - 1
	 *
	 * @param Offset - Result offset from most recent.
	 * e.g. if user has 100 topics, offset 25 means result will start from 75
	 * @param Limit - Max count of returned results from the most recent
	 * e.g. if user has 100 topics, limit 20 will only return the last 20 (100-81)
	 * @param OnSuccess - Callback for successful personal topic query response
	 * @param OnError - Callback for failed personal topic query
	 * @see QueryTopicById, QueryGroupTopic, QueryTopic, QueryPublicTopic, QueryChat
	 */
	void QueryPersonalTopic(int Offset
		, int Limit
		, FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Query GROUP chat topics that the logged in user is a member of
	 *
	 * @param Request - Parameter request in struct form
	 * @param OnSuccess - Callback for successful personal topic query response
	 * @param OnError - Callback for failed personal topic query
	 * @see QueryTopicById, QueryPersonalTopic, QueryTopic, QueryChat
	 */
	void QueryGroupTopic(FAccelByteModelsChatQueryTopicRequest const& Request
		, FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Query GROUP chat topics that are subscribed by the logged in user
	 *
	 * if user has sequential 100 topics, 1 is oldest, 100 is most recent
	 * result will be returned from most recent. so in this order: 100 - 1
	 *
	 * @param Keyword - Friendly name keyword for the topic, e.g. "gg guild"
	 * @param Offset - Result offset from most recent.
	 * e.g. if user has 100 topics, offset 25 means result will start from 75
	 * @param Limit - Max count of returned results from the most recent
	 * e.g. if user has 100 topics, limit 20 will only return the last 20 (100-81)
	 * @param OnSuccess - Callback for successful group topic query response
	 * @param OnError - Callback for failed group topic query
	 * @see QueryTopicById, QueryGroupTopic, QueryPersonalTopic, QueryPublicTopic, QueryChat
	 */
	void QueryGroupTopic(FString const& Keyword
		, int Offset
		, int Limit
		, FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryGroupTopic(FAccelByteModelsChatQueryTopicRequest(Keyword, Offset, Limit), OnSuccess, OnError);
	}

	/**
	 * @brief Query chat topics (PERSONAL and GROUP, excluding PUBLIC) that are subscribed by the logged in user
	 *
	 * @param Request - Parameter request in struct form
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryTopicById, QueryGroupTopic, QueryPersonalTopic, QueryPublicTopic, QueryChat
	 */
	void QueryTopic(FAccelByteModelsChatQueryTopicRequest const& Request
		, FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Query chat topics (PERSONAL and GROUP, excluding PUBLIC) that are subscribed by the logged in user
	 *
	 * if user has sequential 100 topics, 1 is oldest, 100 is most recent
	 * result will be returned from most recent. so in this order: 100 - 1
	 *
	 * @param Keyword Friendly name keyword for the topic, only relevant for topic topic and not personal topic e.g. "gg guild"
	 * @param Offset - Result offset from most recent.
	 * e.g. if user has 100 topics, offset 25 means result will start from 75
	 * @param Limit - Max count of returned results from the most recent
	 * e.g. if user has 100 topics, limit 20 will only return the last 20 (100-81)
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryTopicById, QueryGroupTopic, QueryPersonalTopic, QueryPublicTopic, QueryChat
	 */
	void QueryTopic(FString const& Keyword
		, int Offset
		, int Limit
		, FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryTopic(FAccelByteModelsChatQueryTopicRequest(Keyword, Offset, Limit), OnSuccess, OnError);
	}

	/**
	 * @brief Query chat topics (PERSONAL and GROUP, excluding PUBLIC) that are subscribed by the logged in user By using Id as keyword instead of group name
	 *
	 * @param TopicId - Parameter request in struct form
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryGroupTopic, QueryPersonalTopic, QueryPublicTopic, QueryChat
	 */
	void QueryTopicById(FString const& TopicId
		, FQueryTopicByIdResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Query PUBLIC (joinable) chat topics (server topic / channel)
	 * 
	 * @param Request - Parameter request in struct form
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryTopicById, QueryGroupTopic, QueryPersonalTopic, QueryTopic, QueryChat
	 */
	void QueryPublicTopic(FAccelByteModelsChatQueryTopicRequest const& Request
		, FQueryPublicTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:
	void SetQueryTopicResponseDelegate(FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryTopicResponse = OnSuccess;
		OnQueryTopicError = OnError;
	}

	void SetQueryTopicByIdResponseDelegate(FQueryTopicByIdResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryTopicByIdResponse = OnSuccess;
		OnQueryTopicByIdError = OnError;
	}

	void SetQueryPersonalTopicResponseDelegate(FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryPersonalTopicResponse = OnSuccess;
		OnQueryPersonalTopicError = OnError;
	}

	void SetQueryGroupTopicResponseDelegate(FQueryTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryGroupTopicResponse = OnSuccess;
		OnQueryGroupTopicError = OnError;
	}

	void SetQueryPublicTopicResponseDelegate(FQueryPublicTopicResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryPublicTopicResponse = OnSuccess;
		OnQueryPublicTopicError = OnError;
	}

#pragma endregion QUERY TOPIC

#pragma region QUERY CHAT

public:

	/**
	 * @brief Query chat messages that are subscribed by the logged in user
	 *
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryChat
	 */
	void QueryChat(FAccelByteModelsChatQueryChatRequest const& Request
		, FQueryChatResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Query chat messages that are subscribed by the logged in user
	 *
	 * @param TopicId - target topic from which the chats are being queried
	 * @param Limit - max count of returned results from the most recent
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryChat
	 */
	void QueryChat(FString const& TopicId
		, int Limit
		, FQueryChatResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryChat(FAccelByteModelsChatQueryChatRequest(TopicId, Limit),OnSuccess,OnError);
	}

private:
	void SetQueryChatResponseDelegate(FQueryChatResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		QueryChatResponse = OnSuccess;
		OnQueryChatError = OnError;
	}

#pragma endregion QUERY CHAT

#pragma region READ CHAT

public:
	/**
	 * @brief Flag the supplied ChatIds as has been read
	 *
	 * @param ChatIds - Chat message Ids to mark as has been read
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryChat
	 */
	void ReadChat(TSet<FString> const& ChatIds
		, FReadChatResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:
	void SetReadChatResponseDelegate(FReadChatResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		ReadChatResponse = OnSuccess;
		OnReadChatError = OnError;
	}

#pragma endregion READ CHAT

#pragma region BLOCK USER

public:
	/**
	 * @brief Block user from chatting
	 *
	 * @param UserId - User ID to block
	 * @param OnSuccess - Callback for successful block user response
	 * @param OnError - Callback for failed block user
	 * @see SetBlockUserResponseDelegate
	 */
	void BlockUser(FString const& UserId
		, FChatBlockUserResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Unblock user from chatting
	 *
	 * @param UserId - User ID to unblock
	 * @param OnSuccess - Callback for successful block user response
	 * @param OnError - Callback for failed Unblock user
	 * @see SetUnblockUserResponseDelegate
	 */
	void UnblockUser(FString const& UserId
		, FChatUnblockUserResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:
	void SetBlockUserResponseDelegate(FChatBlockUserResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		BlockUserResponse = OnSuccess;
		OnBlockUserError = OnError;
	}

	void SetUnblockUserResponseDelegate(FChatUnblockUserResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr)
	{
		UnblockUserResponse = OnSuccess;
		OnBlockUserError = OnError;
	}

#pragma endregion BLOCK USER

#pragma region GROUP CHAT AS MODERATOR

public:
	/**
	 * @brief Delete a message from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param ChatId Id of message to delete.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteGroupChat(FString const& GroupId
		, FString const& ChatId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Mute user from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserId Id of the user to be muted.
	 * @param DurationInSeconds Duration of mute in seconds.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr MuteGroupUserChat(FString const& GroupId
		, FString const& UserId
		, int32 DurationInSeconds
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Unmute user from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserId Id of the user to be un-muted.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UnmuteGroupUserChat(FString const& GroupId
		, FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get chat snapshot (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param ChatId Id of message.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGroupChatSnapshot(FString const& GroupId
		, FString const& ChatId
		, THandler<FAccelByteModelsChatSnapshotResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Ban users from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserIds Array of user ids to ban.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BanGroupUserChat(FString const& GroupId
		, TArray<FString> const& UserIds
		, THandler<FAccelByteModelsBanGroupChatResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Unban users from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserIds Array of user ids to unban.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UnbanGroupUserChat(FString const& GroupId
		, TArray<FString> const& UserIds
		, THandler<FAccelByteModelsUnbanGroupChatResponse> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	/**
	 * @brief Generate group topic id based on group id
	 * @param GroupId Id of the Group
	 * @return Group topic id
	 */
	static FString GenerateGroupTopicId(FString const& GroupId);

#pragma endregion GROUP CHAT AS MODERATOR

#pragma region USER CHAT CONFIGURATION

public:
	/**
	 * @brief Get chat configuration for current user.
	 *
	 * @param OnSuccess - Callback for successful get user chat configuration.
	 * @param OnError - Callback for failed get user chat configuration.
	 */
	void GetUserChatConfiguration(FGetUserChatConfigurationResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

	/**
	 * @brief Set chat configuration for current user.
	 *
	 * @param Request - The chat configuration.
	 * @param OnSuccess - Callback for successful set user chat configuration.
	 * @param OnError - Callback for failed set user chat configuration.
	 */
	void SetUserChatConfiguration(const FAccelByteModelsSetUserChatConfigurationRequest& Request
		, FSetUserChatConfigurationResponse const& OnSuccess
		, FErrorHandler const& OnError = nullptr);

private:
	void SubscribeGetUserChatConfigurationResponseDelegate(FGetUserChatConfigurationResponse const& OnGetUserChatConfigurationResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetUserChatConfigurationResponse = OnGetUserChatConfigurationResponse;
		OnGetUserChatConfigurationError = OnError;
	}

	void SubscribeSetUserChatConfigurationResponseDelegate(FSetUserChatConfigurationResponse const& OnGetUserChatConfigurationResponse
		, FErrorHandler const& OnError = nullptr)
	{
		SetUserChatConfigurationResponse = OnGetUserChatConfigurationResponse;
		OnSetUserChatConfigurationError = OnError;
	}

#pragma endregion 
	
#pragma region NOTIFICATION SETTER
public:

	/**
	 * @brief Delegate setter for incoming chat message event
	 */
	void SetChatNotifDelegate(FChatNotif const& Delegate)
	{
		ChatNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for logged in user added to a topic event
	 */
	void SetAddToTopicNotifDelegate(FAddRemoveFromTopicNotif const& Delegate)
	{
		AddToTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for logged in user removed from a topic event
	 */
	void SetRemoveFromTopicNotifDelegate(FAddRemoveFromTopicNotif const& Delegate)
	{
		RemoveFromTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when a subscribed topic is deleted
	 */
	void SetDeleteTopicNotifDelegate(FDeleteUpdateTopicNotif const& Delegate)
	{
		DeleteTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when a subscribed topic is updated
	 */
	void SetUpdateTopicNotifDelegate(FDeleteUpdateTopicNotif const& Delegate)
	{
		UpdateTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when a subscribed topic's chat has been read by others
	 */
	void SetReadChatNotifDelegate(FReadChatNotif const& Delegate)
	{
		ReadChatNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when user received ban event
	 */
	void SetUserBanNotifDelegate(FUserBanUnbanNotif const& Delegate)
	{
		UserBanNotif = Delegate;
	}
	
	/**
	 * @brief Delegate setter for when user received a System message
	 */
	void SetSystemMessageNotifDelegate(FSystemMessageNotif const& Delegate)
	{
		SystemMessageNotif = Delegate;
	}
	
	/**
	 * @brief Delegate setter for when user received unban event
	 */
	void SetUserUnbanNotifDelegate(FUserBanUnbanNotif const& Delegate)
	{
		UserUnbanNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when user received muted event
	 */
	void SetUserMutedNotifDelegate(FUserMutedNotif const& Delegate)
	{
		UserMutedNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when user received unmuted event
	 */
	void SetUserUnmutedNotifDelegate(FUserUnmutedNotif const& Delegate)
	{
		UserUnmutedNotif = Delegate;
	}

#pragma endregion NOTIFICATION SETTER

#pragma region CALLBACK HANDLERS

private:
	static TMap<FString, HandleType> HandlerStringEnumMap;

	TMap<FString, FChatActionTopicResponse> MessageIdCreateTopicResponseMap;
	TMap<FString, FSendChatResponse> MessageIdSendChatResponseMap;
	TMap<FString, FQueryTopicResponse> MessageIdQueryTopicResponseMap;
	TMap<FString, FQueryTopicByIdResponse> MessageIdQueryTopicByIdResponseMap;
	TMap<FString, FQueryTopicResponse> MessageIdQueryPersonalTopicResponseMap;
	TMap<FString, FQueryTopicResponse> MessageIdQueryGroupTopicResponseMap;
	TMap<FString, FQueryPublicTopicResponse> MessageIdQueryPublicTopicResponseMap;
	TMap<FString, FQueryChatResponse> MessageIdQueryChatResponseMap;
	TMap<FString, FReadChatResponse> MessageIdReadChatResponseMap;
	TMap<FString, FChatActionTopicResponse> MessageIdUpdateTopicResponseMap;
	TMap<FString, FChatActionTopicResponse> MessageIdDeleteTopicResponseMap;
	TMap<FString, FAddRemoveUserFromTopicResponse> MessageIdAddUserToTopicResponseMap;
	TMap<FString, FAddRemoveUserFromTopicResponse> MessageIdRemoveUserFromTopicResponseMap;
	TMap<FString, FChatActionTopicResponse> MessageIdJoinTopicResponseMap;
	TMap<FString, FChatActionTopicResponse> MessageIdQuitTopicResponseMap;
	TMap<FString, FChatRefreshTokenResponse> MessageIdRefreshTokenResponseMap;
	TMap<FString, FChatBlockUserResponse> MessageIdBlockUserResponseMap;
	TMap<FString, FChatUnblockUserResponse> MessageIdUnblockUserResponseMap;
	TMap<FString, FDeleteSystemMessagesResponse> MessageIdDeleteSystemMessagesResponseMap;
	TMap<FString, FUpdateSystemMessagesResponse> MessageIdUpdateSystemMessagesResponseMap;
	TMap<FString, FQuerySystemMessageResponse> MessageIdQuerySystemMessageResponseMap;
	TMap<FString, FGetSystemMessageStatsResponse> MessageIdGetSystemMessageStatsResponseMap;
	TMap<FString, FGetUserChatConfigurationResponse> MessageIdGetUserChatConfigurationResponseMap;
	TMap<FString, FSetUserChatConfigurationResponse> MessageIdSetUserChatConfigurationResponseMap;

	FChatActionTopicResponse CreateTopicResponse;
	FSendChatResponse SendChatResponse;
	FQueryTopicResponse QueryTopicResponse;
	FQueryTopicByIdResponse QueryTopicByIdResponse;
	FQueryTopicResponse QueryPersonalTopicResponse;
	FQueryTopicResponse QueryGroupTopicResponse;
	FQueryPublicTopicResponse QueryPublicTopicResponse;
	FQueryChatResponse QueryChatResponse;
	FReadChatResponse ReadChatResponse;
	FChatActionTopicResponse UpdateTopicResponse;
	FChatActionTopicResponse DeleteTopicResponse;
	FAddRemoveUserFromTopicResponse AddUserToTopicResponse;
	FAddRemoveUserFromTopicResponse RemoveUserFromTopicResponse;
	FChatActionTopicResponse JoinTopicResponse;
	FChatActionTopicResponse QuitTopicResponse;
	FChatRefreshTokenResponse RefreshTokenResponse;
	FChatBlockUserResponse BlockUserResponse;
	FChatUnblockUserResponse UnblockUserResponse;
	FDeleteSystemMessagesResponse DeleteSystemMessagesResponse;
	FUpdateSystemMessagesResponse UpdateSystemMessagesResponse;
	FQuerySystemMessageResponse QuerySystemMessageResponse;
	FGetSystemMessageStatsResponse GetSystemMessageStatsResponse;
	FGetUserChatConfigurationResponse GetUserChatConfigurationResponse;
	FSetUserChatConfigurationResponse SetUserChatConfigurationResponse;

	FErrorHandler OnCreateTopicError;
	FErrorHandler OnSendChatError;
	FErrorHandler OnQueryTopicError;
	FErrorHandler OnQueryTopicByIdError;
	FErrorHandler OnQueryPersonalTopicError;
	FErrorHandler OnQueryGroupTopicError;
	FErrorHandler OnQueryPublicTopicError;
	FErrorHandler OnQueryChatError;
	FErrorHandler OnReadChatError;
	FErrorHandler OnUpdateTopicError;
	FErrorHandler OnDeleteTopicError;
	FErrorHandler OnAddUserToTopicError;
	FErrorHandler OnRemoveUserFromTopicError;
	FErrorHandler OnJoinTopicError;
	FErrorHandler OnQuitTopicError;
	FErrorHandler OnRefreshTokenError;
	FErrorHandler OnBlockUserError;
	FErrorHandler OnUnblockUserError;
	FErrorHandler OnDeleteSystemMessagesError;
	FErrorHandler OnUpdateSystemMessagesError;
	FErrorHandler OnQuerySystemMessageError;
	FErrorHandler OnGetSystemMessageStatsError;
	FErrorHandler OnGetUserChatConfigurationError;
	FErrorHandler OnSetUserChatConfigurationError;

	FChatNotif ChatNotif;
	FReadChatNotif ReadChatNotif;
	FAddRemoveFromTopicNotif AddToTopicNotif;
	FAddRemoveFromTopicNotif RemoveFromTopicNotif;
	FDeleteUpdateTopicNotif DeleteTopicNotif;
	FDeleteUpdateTopicNotif UpdateTopicNotif;
	FSystemMessageNotif SystemMessageNotif;
	FUserMutedNotif UserMutedNotif;
	FUserUnmutedNotif UserUnmutedNotif;
	
	FUserBanUnbanNotif UserBanNotif;
	FUserBanUnbanNotif UserUnbanNotif;

	FString GenerateMessageID(FString const& Prefix = TEXT("")) const;

	const int32 DefaultQuerySystemMessageLimit {20};
	const int32 DefaultQuerySystemMessageOffset {0};

#pragma endregion

#pragma region CHAT CONFIGURATION

public:

	/**
	 * @brief Get chat config of a namespace.
	 *
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetChatConfig(THandler<FAccelByteModelsChatPublicConfigResponse> const& OnSuccess
		, FErrorHandler const& OnError);

#pragma  endregion CHAT CONFIGURATION
};

typedef TSharedRef<Chat, ESPMode::ThreadSafe> ChatRef;
typedef TSharedPtr<Chat, ESPMode::ThreadSafe> ChatPtr;
typedef TWeakPtr<Chat, ESPMode::ThreadSafe> ChatWPtr;

} // Namespace Api
} // Namespace AccelByte