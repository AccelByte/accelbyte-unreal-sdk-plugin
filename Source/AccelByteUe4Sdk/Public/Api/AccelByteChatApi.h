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
class ACCELBYTEUE4SDK_API Chat : public FApiBase
{
public:
	Chat(Credentials& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, FAccelByteMessagingSystem& InMessagingSystemRef
		, FAccelByteNetworkConditioner& InNetworkConditionerRef
		, float PingDelay = 30.f
		, float InitialBackoffDelay = 1.f
		, float MaxBackoffDelay = 30.f
		, float TotalTimeout = 60.f
		, TSharedPtr<IWebSocket> WebSocket = nullptr);

	~Chat();
private:
	const FString ChatSessionHeaderName = TEXT("X-Ab-ChatSessionID");

	Credentials& ChatCredentialsRef;
	FAccelByteMessagingSystem& MessagingSystem;
	FAccelByteNetworkConditioner& NetworkConditioner;

	bool bBanNotifReceived = false;
	EBanType BanType = EBanType::EMPTY;

	Chat() = delete;
	Chat(const Chat&) = delete; // Copy constructor
	Chat(Chat&&) = delete; // Move constructor
	Chat& operator=(Chat const&) = delete; // Copy assignment operator
	Chat& operator=(Chat&&) = delete; // Move assignment operator

#pragma region DELEGATE DECLARATIONS
public:

	//				CONNECTIONS
	DECLARE_DELEGATE(FChatConnectSuccess);
	DECLARE_DELEGATE_OneParam(FChatDisconnectNotif, const FAccelByteModelsChatDisconnectNotif&)
	DECLARE_DELEGATE_ThreeParams(FChatConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);

	/**
	* @brief delegate for handling response when refreshing chat token.
	*/
	DECLARE_DELEGATE_OneParam(FChatRefreshTokenResponse, const FAccelByteModelsChatRefreshTokenResponse&)


	//				RESPONSES
	/**
	 * @brief delegate for handling sent chat response.
	 */
	DECLARE_DELEGATE_OneParam(FSendChatResponse, const FAccelByteModelsChatSendChatResponse&)

	/**
	 * @brief delegate for handling chat topic query response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryTopicResponse, const FAccelByteModelsChatQueryTopicResponse&)

	/**
	 * @brief delegate for handling chat topic query by id response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryTopicByIdResponse, const FAccelByteModelsChatQueryTopicByIdResponse&)

	/**
	 * @brief delegate for handling public topic query response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryPublicTopicResponse, const FAccelByteModelsChatQueryPublicTopicResponse&)

	/**
	 * @brief delegate for handling chat query response.
	 */
	DECLARE_DELEGATE_OneParam(FQueryChatResponse, const FAccelByteModelsChatQueryChatResponse&)

	/**
	 * @brief delegate for handling read chat response.
	 */
	DECLARE_DELEGATE_OneParam(FReadChatResponse, const FAccelByteModelsChatReadChatResponse&)

	/**
	 * @brief delegate for handling user block response.
	 */
	DECLARE_DELEGATE_OneParam(FChatBlockUserResponse, const FAccelByteModelsChatBlockUserResponse&)

	/**
	 * @brief delegate for handling user unblock response.
	 */
	DECLARE_DELEGATE_OneParam(FChatUnblockUserResponse, const FAccelByteModelsChatUnblockUserResponse&)

	/**
	 * @brief delegate for handling action chat topic response (create, update, delete, join, quit)
	 */
	DECLARE_DELEGATE_OneParam(FChatActionTopicResponse, const FAccelByteModelsChatActionTopicResponse&)

	/**
	 * @brief delegate for handling add user to topic and remove user from topic response.
	 */
	DECLARE_DELEGATE_OneParam(FAddRemoveUserFromTopicResponse, const FAccelByteModelsChatActionUserTopicResponse&)

	/**
	 * @brief delegate for handling delete system message(s) response.
	 */
	DECLARE_DELEGATE_OneParam(FDeleteSystemMessagesResponse, const FAccelByteModelsDeleteSystemMessagesResponse&)

	/**
	 * @brief delegate for handling update system message(s) response.
	 */
	DECLARE_DELEGATE_OneParam(FUpdateSystemMessagesResponse, const FAccelByteModelsUpdateSystemMessagesResponse&)

	/**
	 * @brief delegate for handling query system message response.
	 */
	DECLARE_DELEGATE_OneParam(FQuerySystemMessageResponse, const FAccelByteModelsQuerySystemMessagesResponse&)

	/** Delegate for handling get system message stats response. */
	DECLARE_DELEGATE_OneParam(FGetSystemMessageStatsResponse, const FAccelByteGetSystemMessageStatsResponse&)

	//				NOTIFICATIONS
	/**
	 * @brief delegate for handling incoming chat notification.
	 */
	DECLARE_DELEGATE_OneParam(FChatNotif, const FAccelByteModelsChatNotif&)

	/**
	 * @brief delegate for handling read chat notification.
	 */
	DECLARE_DELEGATE_OneParam(FReadChatNotif, const FAccelByteModelsReadChatNotif&)

	/**
	 * @brief delegate for handling add to/ remove from topic notification.
	 */
	DECLARE_DELEGATE_OneParam(FAddRemoveFromTopicNotif, const FAccelByteModelsChatUpdateUserTopicNotif&)

	/**
	 * @brief delegate for handling chat topic removal/update notification.
	 */
	DECLARE_DELEGATE_OneParam(FDeleteUpdateTopicNotif, const FAccelByteModelsChatUpdateTopicNotif&)

	/**
	 * @brief delegate for handling chat incoming system messages.
	 */
	DECLARE_DELEGATE_OneParam(FSystemMessageNotif, const FAccelByteModelsChatSystemMessageNotif&)

	/*
	* @brief delegate for handling ban / unban notification
	*/
	DECLARE_DELEGATE_OneParam(FUserBanUnbanNotif, const FAccelByteModelsChatUserBanUnbanNotif&);

	/*
	* @brief delegate for handling chat muted notification
	*/
	DECLARE_DELEGATE_OneParam(FUserMutedNotif, const FAccelByteModelsChatMutedNotif&);

	/*
	* @brief delegate for handling chat unmuted notification
	*/
	DECLARE_DELEGATE_OneParam(FUserUnmutedNotif, const FAccelByteModelsChatUnmutedNotif&);


#pragma endregion

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
	void SetConnectSuccessDelegate(const FChatConnectSuccess& OnConnectSuccess)
	{
		ConnectSuccess = OnConnectSuccess;
	}

	/**
	 * @brief Delegate setter for chat websocket connection failure event
	 *
	 * @param OnConnectError Callback delegate when failed connecting to Chat Websocket.
	 */
	void SetConnectFailedDelegate(const FErrorHandler& OnConnectError)
	{
		ConnectError = OnConnectError;
	}

	/**
	 * @brief Delegate setter for chat websocket disconnected event
	 *
	 * @param OnDisconnectNotif Notification Delegate when disconnected from Chat Websocket. 
	 */
	void SetDisconnectNotifDelegate(const FChatDisconnectNotif OnDisconnectNotif)
	{
		DisconnectNotif = OnDisconnectNotif;
	}

	/**
	 * @brief Delegate setter for chat websocket connection closed event (connection closed by peer)
	 *
	 * @param OnConnectionClosed Callback delegate when connection closed from Chat Websocket.
	 */
	void SetConnectionClosedDelegate(const FChatConnectionClosed& OnConnectionClosed)
	{
		ConnectionClosed = OnConnectionClosed;
	}

private:

	const float PingDelay;
	float InitialBackoffDelay;
	float MaxBackoffDelay;
	float TotalTimeout;
	bool bWasWsConnectionError = false;
	float BackoffDelay;
	float RandomizedBackoffDelay;
	float TimeSinceLastPing;
	float TimeSinceLastReconnect;
	float TimeSinceConnectionLost;
	TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> WebSocket;
	FAccelByteModelsChatConnectNotif ChatSessionId;

	const FString WsEnvelopeStart {"CaSr"};
	const FString WsEnvelopeEnd {"CaEd"};
	FString EnvelopeContentBuffer;

	FChatConnectSuccess ConnectSuccess;
	FErrorHandler ConnectError;
	FErrorHandler ParsingError;
	FChatDisconnectNotif DisconnectNotif;
	FChatConnectionClosed ConnectionClosed;
	FChatConnectionClosed Reconnecting;
	FDelegateHandle AuthTokenSetDelegateHandle;

	void OnConnected();
	void OnConnectionError(const FString& Error);
private:
	void OnMessage(const FString& Message);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);
	void CreateWebSocket();
	
	FString SendWebSocketContent(const FString& Method, const TSharedRef<FJsonObject>& Params);
	FString SendWebSocketContent(const FString& Method, const FJsonDomBuilder::FObject& Params)
	{
		return SendWebSocketContent(Method, Params.AsJsonObject());
	}

#pragma endregion

#pragma region SESSION

public:
	/**
	* @brief Refresh access token used in chat.
	*
	* @param AccessToken new access token to be used in chat
	* @param OnSuccess delegate to be triggered when operation success
	* @param OnError delegate to be triggered when operation error
	*/
	FString RefreshToken(const FString& AccessToken
		, const FChatRefreshTokenResponse& OnSuccess
		, const FErrorHandler& OnError = {});

private:

	void SetRefreshTokenResponseDelegate(const FChatRefreshTokenResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		RefreshTokenResponse = OnSuccess;
		OnRefreshTokenError = OnError;
	}

#pragma endregion

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

#pragma endregion

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
	void CreatePersonalTopic(const FString& TargetUserId
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

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
	void CreateGroupTopic(const TSet<FString>& Users
		, const TSet<FString>& Admins
		, const FString& TopicName
		, bool bIsJoinable
		, const FChatActionTopicResponse & OnSuccess
		, const FErrorHandler& OnError = {})
	{
		CreateGroupTopic(FAccelByteModelsChatCreateTopicRequest(TopicName,Users,Admins,bIsJoinable),OnSuccess,OnError);
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
	void CreateGroupTopic(const FAccelByteModelsChatCreateTopicRequest& Request
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

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
	void UpdateTopic(const FString& TopicId
		, const FString& NewFriendlyTopicName
		, bool bIsJoinable
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
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
	void UpdateTopic(const FAccelByteModelsChatUpdateTopicRequest& Request
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Delete the group topic id, only topic admin can use this operation. PERSONAL topic cannot be deleted
	 *
	 * @param ToDeleteGroupTopicId - Group Topic id that will be deleted. PERSONAL topic cannot be deleted
	 * @param OnSuccess - Callback for successful group chat topic deletion response
	 * @param OnError - Callback for failed chat topic deletion
	 * @see SetDeleteTopicNotifDelegate
	 */
	void DeleteTopic(const FString& ToDeleteGroupTopicId
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Add a new user to an existing GROUP topic id, only topic admin can use this operation
	 *
	 * @param Request - Parameter request in struct form for adding a user to a group topic
	 * @param OnSuccess - Callback for successful chat topic user addition response
	 * @param OnError - Callback for failed chat topic user addition
	 * @see SetAddToTopicNotifDelegate
	 */
	void AddUserToTopic(const FAccelByteModelsChatAddUserToTopicRequest& Request
		, const FAddRemoveUserFromTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Add an user to an existing topic id, only topic admin can use this operation
	 *
	 * @param TopicId - Topic id that will receive the new user
	 * @param ToAddUserId - User Id that will be added to the topic
	 * @param OnSuccess - Callback for successful chat topic user addition response
	 * @param OnError - Callback for failed chat topic user addition
	 * @see SetAddToTopicNotifDelegate
	 */
	void AddUserToTopic(const FString& TopicId
		, const FString& ToAddUserId
		, const FAddRemoveUserFromTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
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
	void RemoveUserFromTopic(const FAccelByteModelsChatRemoveUserFromTopicRequest& Request
		, const FAddRemoveUserFromTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Remove an user from an existing topic id, only topic admin can use this operation
	 *
	 * @param TopicId - Topic id that will remove an existing user
	 * @param ToRemoveUserId - User Id that will be removed from the topic
	 * @param OnSuccess - Callback for successful chat topic user removal response
	 * @param OnError - Callback for failed chat topic user removal
	 * @see SetRemoveFromTopicNotifDelegate
	 */
	void RemoveUserFromTopic(const FString& TopicId
		, const FString& ToRemoveUserId
		, const FAddRemoveUserFromTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
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
	void JoinTopic(const FString& TopicId
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Leave a group topic
	 *
	 * @param TopicId - Group topic id that the logged in user is leaving
	 * @param OnSuccess - Callback for successful quit
	 * @param OnError - Callback for failed quit
	 * @see SetQuitTopicResponseDelegate
	 */
	void QuitTopic(const FString& TopicId
		, const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

private:

	void SetCreateTopicResponseDelegate(const FChatActionTopicResponse& OnCreateTopicResponse
		, const FErrorHandler& OnError = {})
	{
		CreateTopicResponse = OnCreateTopicResponse;
		OnCreateTopicError = OnError;
	}

	void SetUpdateTopicResponseDelegate(const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		UpdateTopicResponse = OnSuccess;
		OnUpdateTopicError = OnError;
	}

	void SetDeleteTopicResponseDelegate(const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		DeleteTopicResponse = OnSuccess;
		OnDeleteTopicError = OnError;
	}

	void SetAddUserToTopicResponseDelegate(const FAddRemoveUserFromTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		AddUserToTopicResponse = OnSuccess;
		OnAddUserToTopicError = OnError;
	}

	void SetRemoveUserFromTopicResponseDelegate(const FAddRemoveUserFromTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		RemoveUserFromTopicResponse = OnSuccess;
		OnRemoveUserFromTopicError = OnError;
	}

	void SetJoinTopicResponseDelegate(const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		JoinTopicResponse = OnSuccess;
		OnJoinTopicError = OnError;
	}

	void SetQuitTopicResponseDelegate(const FChatActionTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QuitTopicResponse = OnSuccess;
		OnQuitTopicError = OnError;
	}

#pragma endregion

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
	void SendChat(const FString& TopicId
		, const FString& Message
		, const FSendChatResponse& OnSuccess
		, const FErrorHandler& OnError = {});

private:
	void SetSendChatResponseDelegate(const FSendChatResponse& OnSendChatResponse
		, const FErrorHandler& OnError = {})
	{
		SendChatResponse = OnSendChatResponse;
		OnSendChatError = OnError;
	}

#pragma endregion

#pragma region SYSTEM INBOX

public:
	/**
	 * @brief Delete system message(s) in user system inbox
	 *
	 * @param MessageIds - Array of system message id
	 * @param OnSuccess - Callback for successful deleted system message(s) in user system inbox
	 * @param OnError - Callback for failed deleted system message(s) in user system inbox
	 */
	void DeleteSystemMessages(TSet<FString> MessageIds
		, const FDeleteSystemMessagesResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Update system message(s) in user system inbox
	 *
	 * @param ActionUpdateSystemMessages - Array of ActionUpdateSystemMessage containing action to mark read/unread and keep
	 * @param OnSuccess - Callback for successful deleted system message(s) in user system inbox
	 * @param OnError - Callback for failed updated system message(s) in user system inbox
	 */
	void UpdateSystemMessages(TArray<FAccelByteModelsActionUpdateSystemMessage> ActionUpdateSystemMessages
		, const FUpdateSystemMessagesResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Query system messages in user's system inbox
	 *
	 * @param OnSuccess - Callback for successful deleted system message(s) in user system inbox
	 * @param OnError - Callback for failed updated system message(s) in user system inbox
	 * @param OptionalParams - Optional parameters for query inbox message
	 */
	void QuerySystemMessage(const FQuerySystemMessageResponse& OnSuccess
		, const FErrorHandler& OnError = {}
		, const FQuerySystemMessageOptions OptionalParams = {});

	/**
	 * Get system message stats (number of unread messages and date time of oldest unread message)
	 *
	 * @param OnSuccess - Callback for successful get system message stats
	 * @param OnError - Callback for failed get system message stats
	 * @param Request - Optional request parameters
	 */
	void GetSystemMessageStats(const FGetSystemMessageStatsResponse& OnSuccess
		, const FErrorHandler& OnError
		, const FAccelByteGetSystemMessageStatsRequest Request = {});

private:
	void SetDeleteSystemMessagesResponseDelegate(const FDeleteSystemMessagesResponse& OnDeleteInboxMessagesResponse
		, const FErrorHandler& OnError = {})
	{
		DeleteSystemMessagesResponse = OnDeleteInboxMessagesResponse;
		OnDeleteSystemMessagesError = OnError;
	}

	void SetUpdateSystemMessagesResponseDelegate(const FUpdateSystemMessagesResponse& OnUpdateSystemMessagesResponse
		, const FErrorHandler& OnError = {})
	{
		UpdateSystemMessagesResponse = OnUpdateSystemMessagesResponse;
		OnUpdateSystemMessagesError = OnError;
	}

	void SetQuerySystemMessageResponseDelegate(const FQuerySystemMessageResponse& OnQuerySystemMessagesResponse
			, const FErrorHandler& OnError = {})
	{
		QuerySystemMessageResponse = OnQuerySystemMessagesResponse;
		OnUpdateSystemMessagesError = OnError;
	}

	void SetGetSystemMessageStatsResponseDelegate(const FGetSystemMessageStatsResponse& OnGetSystemMessageStatsResponse
		, const FErrorHandler& OnError = {})
	{
		GetSystemMessageStatsResponse = OnGetSystemMessageStatsResponse;
		OnGetSystemMessageStatsError = OnError;
	}

#pragma endregion

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
		, const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Query GROUP chat topics that the logged in user is a member of
	 *
	 * @param Request - Parameter request in struct form
	 * @param OnSuccess - Callback for successful personal topic query response
	 * @param OnError - Callback for failed personal topic query
	 * @see QueryTopicById, QueryPersonalTopic, QueryTopic, QueryChat
	 */
	void QueryGroupTopic(const FAccelByteModelsChatQueryTopicRequest& Request
		, const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

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
	void QueryGroupTopic(const FString& Keyword
		, int Offset
		, int Limit
		, const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
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
	void QueryTopic(const FAccelByteModelsChatQueryTopicRequest& Request
		, const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

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
	void QueryTopic(const FString& Keyword
		, int Offset
		, int Limit
		, const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
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
	void QueryTopicById(const FString& TopicId
		, const FQueryTopicByIdResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Query PUBLIC (joinable) chat topics (server topic / channel)
	 * 
	 * @param Request - Parameter request in struct form
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryTopicById, QueryGroupTopic, QueryPersonalTopic, QueryTopic, QueryChat
	 */
	void QueryPublicTopic(const FAccelByteModelsChatQueryTopicRequest& Request
		, const FQueryPublicTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {});

private:
	void SetQueryTopicResponseDelegate(const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryTopicResponse = OnSuccess;
		OnQueryTopicError = OnError;
	}

	void SetQueryTopicByIdResponseDelegate(const FQueryTopicByIdResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryTopicByIdResponse = OnSuccess;
		OnQueryTopicByIdError = OnError;
	}

	void SetQueryPersonalTopicResponseDelegate(const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryPersonalTopicResponse = OnSuccess;
		OnQueryPersonalTopicError = OnError;
	}

	void SetQueryGroupTopicResponseDelegate(const FQueryTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryGroupTopicResponse = OnSuccess;
		OnQueryGroupTopicError = OnError;
	}

	void SetQueryPublicTopicResponseDelegate(const FQueryPublicTopicResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryPublicTopicResponse = OnSuccess;
		OnQueryPublicTopicError = OnError;
	}

#pragma endregion

#pragma region QUERY CHAT

public:

	/**
	 * @brief Query chat messages that are subscribed by the logged in user
	 *
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryChat
	 */
	void QueryChat(const FAccelByteModelsChatQueryChatRequest& Request
		, const FQueryChatResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Query chat messages that are subscribed by the logged in user
	 *
	 * @param TopicId - target topic from which the chats are being queried
	 * @param Limit - max count of returned results from the most recent
	 * @param OnSuccess - Callback for successful topic query response
	 * @param OnError - Callback for failed topic query
	 * @see QueryChat
	 */
	void QueryChat(const FString& TopicId
		, int Limit
		, const FQueryChatResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryChat(FAccelByteModelsChatQueryChatRequest(TopicId, Limit),OnSuccess,OnError);
	}

private:
	void SetQueryChatResponseDelegate(const FQueryChatResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		QueryChatResponse = OnSuccess;
		OnQueryChatError = OnError;
	}

#pragma endregion

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
	void ReadChat(const TSet<FString>& ChatIds
		, const FReadChatResponse& OnSuccess
		, const FErrorHandler& OnError = {});

private:
	void SetReadChatResponseDelegate(const FReadChatResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		ReadChatResponse = OnSuccess;
		OnReadChatError = OnError;
	}

#pragma endregion

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
	void BlockUser(const FString& UserId
		, const FChatBlockUserResponse& OnSuccess
		, const FErrorHandler& OnError = {});

	/**
	 * @brief Unblock user from chatting
	 *
	 * @param UserId - User ID to unblock
	 * @param OnSuccess - Callback for successful block user response
	 * @param OnError - Callback for failed Unblock user
	 * @see SetUnblockUserResponseDelegate
	 */
	void UnblockUser(const FString& UserId
		, const FChatUnblockUserResponse& OnSuccess
		, const FErrorHandler& OnError = {});

private:
	void SetBlockUserResponseDelegate(const FChatBlockUserResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		BlockUserResponse = OnSuccess;
		OnBlockUserError = OnError;
	}

	void SetUnblockUserResponseDelegate(const FChatUnblockUserResponse& OnSuccess
		, const FErrorHandler& OnError = {})
	{
		UnblockUserResponse = OnSuccess;
		OnBlockUserError = OnError;
	}

#pragma endregion

#pragma region GROUP CHAT AS MODERATOR

public:
	/**
	 * @brief Delete a message from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param ChatId Id of message to delete.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	void DeleteGroupChat(const FString& GroupId, const FString& ChatId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Mute user from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserId Id of the user to be muted.
	 * @param DurationInSeconds Duration of mute in seconds.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	void MuteGroupUserChat(const FString& GroupId, const FString& UserId, int32 DurationInSeconds, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Unmute user from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserId Id of the user to be un-muted.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	void UnmuteGroupUserChat(const FString& GroupId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get chat snapshot (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param ChatId Id of message.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	void GetGroupChatSnapshot(const FString& GroupId, const FString& ChatId, const THandler<FAccelByteModelsChatSnapshotResponse>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Ban users from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserIds Array of user ids to ban.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	void BanGroupUserChat(const FString& GroupId, const TArray<FString>& UserIds, const THandler<FAccelByteModelsBanGroupChatResponse>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Unban users from group chat (used by group moderator).
	 *
	 * @param GroupId Id of group from group service.
	 * @param UserIds Array of user ids to unban.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	void UnbanGroupUserChat(const FString& GroupId, const TArray<FString>& UserIds, const THandler<FAccelByteModelsUnbanGroupChatResponse>& OnSuccess, const FErrorHandler& OnError);

private:
	/**
	 * @brief Generate group topic id based on group id
	 * @param GroupId Id of the Group
	 * @return Group topic id
	 */
	static FString GenerateGroupTopicId(const FString& GroupId);

#pragma endregion

#pragma region NOTIFICATION SETTER
public:

	/**
	 * @brief Delegate setter for incoming chat message event
	 */
	void SetChatNotifDelegate(const FChatNotif& Delegate)
	{
		ChatNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for logged in user added to a topic event
	 */
	void SetAddToTopicNotifDelegate(const FAddRemoveFromTopicNotif& Delegate)
	{
		AddToTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for logged in user removed from a topic event
	 */
	void SetRemoveFromTopicNotifDelegate(const FAddRemoveFromTopicNotif& Delegate)
	{
		RemoveFromTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when a subscribed topic is deleted
	 */
	void SetDeleteTopicNotifDelegate(const FDeleteUpdateTopicNotif& Delegate)
	{
		DeleteTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when a subscribed topic is updated
	 */
	void SetUpdateTopicNotifDelegate(const FDeleteUpdateTopicNotif& Delegate)
	{
		UpdateTopicNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when a subscribed topic's chat has been read by others
	 */
	void SetReadChatNotifDelegate(const FReadChatNotif& Delegate)
	{
		ReadChatNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when user received ban event
	 */
	void SetUserBanNotifDelegate(const FUserBanUnbanNotif& Delegate)
	{
		UserBanNotif = Delegate;
	}
	
	/**
	 * @brief Delegate setter for when user received a System message
	 */
	void SetSystemMessageNotifDelegate(const FSystemMessageNotif& Delegate)
	{
		SystemMessageNotif = Delegate;
	}
	
	/**
	 * @brief Delegate setter for when user received unban event
	 */
	void SetUserUnbanNotifDelegate(const FUserBanUnbanNotif& Delegate)
	{
		UserUnbanNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when user received muted event
	 */
	void SetUserMutedNotifDelegate(const FUserMutedNotif& Delegate)
	{
		UserMutedNotif = Delegate;
	}

	/**
	 * @brief Delegate setter for when user received unmuted event
	 */
	void SetUserUnmutedNotifDelegate(const FUserUnmutedNotif& Delegate)
	{
		UserUnmutedNotif = Delegate;
	}

#pragma endregion

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

	FString GenerateMessageID(const FString& Prefix = TEXT("")) const;

	const int32 DefaultQuerySystemMessageLimit {20};
	const int32 DefaultQuerySystemMessageOffset {0};

#pragma endregion

};

} // Namespace Api

} // Namespace AccelByte