// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"

// Forward declarations
class IWebSocket;

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{

enum class EWebSocketState
{
	Closed = 0,
	Connecting = 1,
	Connected = 2,
	Closing = 3,
	Reconnecting = 4
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

ENUM_CLASS_FLAGS(EWebSocketEvent);
	
/**
 * @brief Lobby API for chatting and party management.
 * Unlike other servers which use HTTP, Lobby server uses WebSocket (RFC 6455).
 */
class ACCELBYTEUE4SDK_API Lobby
{
public:
	Lobby(const Credentials& Credentials, const Settings& Settings, float PingDelay = 30.f, float InitialBackoffDelay = 1.f, float MaxBackoffDelay = 30.f, float TotalTimeout = 60.f, TSharedPtr<IWebSocket> WebSocket = nullptr);
	~Lobby();
private:
	const Credentials& Credentials;
	const Settings& Settings;

public:

    // Party 
    /**
     * @brief delegate for handling info party response.
     */
    DECLARE_DELEGATE_OneParam(FPartyInfoResponse, const FAccelByteModelsInfoPartyResponse&); 


    /**
     * @brief delegate for handling create party response.
     */
    DECLARE_DELEGATE_OneParam(FPartyCreateResponse, const FAccelByteModelsCreatePartyResponse&); 

    /**
     * @brief delegate for handling leave party response.
     */
    DECLARE_DELEGATE_OneParam(FPartyLeaveResponse, const FAccelByteModelsLeavePartyResponse&); 

    /**
     * @brief delegate for handling leave party notification.
     */
    DECLARE_DELEGATE_OneParam(FPartyLeaveNotif, const FAccelByteModelsLeavePartyNotice&);                  // Passive


    /**
     * @brief delegate for handling invite party response.
     */
    DECLARE_DELEGATE_OneParam(FPartyInviteResponse, const FAccelByteModelsPartyInviteResponse&); 

    /**
     * @brief delegate for handling party invitation notification
     */
    DECLARE_DELEGATE_OneParam(FPartyInviteNotif, const FAccelByteModelsInvitationNotice&);       // Passive

    /**
     * @brief delegate for handling get party invitation notification
     */
    DECLARE_DELEGATE_OneParam(FPartyGetInvitedNotif, const FAccelByteModelsPartyGetInvitedNotice&);  // Passive

    /**
     * @brief delegate for handling join party response
     */
    DECLARE_DELEGATE_OneParam(FPartyJoinResponse, const FAccelByteModelsPartyJoinReponse&); 

    /**
     * @brief delegate for handling join party notification
     */
    DECLARE_DELEGATE_OneParam(FPartyJoinNotif, const FAccelByteModelsPartyJoinNotice&);              // Passive

    /**
     * @brief delegate for handling member kicked from party event
     */
    DECLARE_DELEGATE_OneParam(FPartyKickResponse, const FAccelByteModelsKickPartyMemberResponse&); 
    
    /**
     * @brief delegate for handling member kicked from party event notification
     */
    DECLARE_DELEGATE_OneParam(FPartyKickNotif, const FAccelByteModelsGotKickedFromPartyNotice&);   // Passive

	/**
	 * @brief delegate for handling member kicked from party event
	 */
	DECLARE_DELEGATE_OneParam(FPartyDataUpdateNotif, const FAccelByteModelsPartyDataNotif&);


    // Chat
    /**
     * @brief delegate for handling private message response
     */
    DECLARE_DELEGATE_OneParam(FPersonalChatResponse, const FAccelByteModelsPersonalMessageResponse&);    

    /**
     * @brief delegate for handling private message event notification
     */
    DECLARE_DELEGATE_OneParam(FPersonalChatNotif, const FAccelByteModelsPersonalMessageNotice&);         // Passive

    /**
     * @brief delegate for handling party message event response
     */
    DECLARE_DELEGATE_OneParam(FPartyChatResponse, const FAccelByteModelsPartyMessageResponse&);

    /**
     * @brief delegate for handling party message event notification
     */
    DECLARE_DELEGATE_OneParam(FPartyChatNotif, const FAccelByteModelsPartyMessageNotice&);              // Passive
    
	/**
	 * @brief delegate for handling join default channel message event response
	 */
	DECLARE_DELEGATE_OneParam(FJoinDefaultChannelChatResponse, const FAccelByteModelsJoinDefaultChannelResponse&);

	/**
	 * @brief delegate for handling channel message event response
	 */
	DECLARE_DELEGATE_OneParam(FChannelChatResponse, const FAccelByteModelsChannelMessageResponse&);

	/**
	 * @brief delegate for handling channel message event notification
	 */
	DECLARE_DELEGATE_OneParam(FChannelChatNotif, const FAccelByteModelsChannelMessageNotice&);

    // Presence
    /**
     * @brief delegate for handling user change presence status
     */
    DECLARE_DELEGATE_OneParam(FSetUserPresenceResponse, const FAccelByteModelsSetOnlineUsersResponse&);

    /**
     * @brief delegate for handling other user change their presence status event
     */
    DECLARE_DELEGATE_OneParam(FFriendStatusNotif, const FAccelByteModelsUsersPresenceNotice&);

    /**
     * @brief delegate for handling get all user presence
     */
    DECLARE_DELEGATE_OneParam(FGetAllFriendsStatusResponse, const FAccelByteModelsGetOnlineUsersResponse&);        


    // Notification
	/**
	 * @brief delegate for handling incoming notification
	*/
	DECLARE_DELEGATE_OneParam(FMessageNotif, const FAccelByteModelsNotificationMessage&); //Passive

    // Matchmaking
	/**
	 * @brief delegate for handling matchmaking response
	 */
	DECLARE_DELEGATE_OneParam(FMatchmakingResponse, const FAccelByteModelsMatchmakingResponse&);

	/**
	 * @brief delegate for handling ready consent response
	 */
	DECLARE_DELEGATE_OneParam(FReadyConsentResponse, const FAccelByteModelsReadyConsentRequest&);

    /**
     * @brief delegate for handling matchmaking notification
     */
    DECLARE_DELEGATE_OneParam(FMatchmakingNotif, const FAccelByteModelsMatchmakingNotice&);

	/*
	 * @brief delegate for handling ready consent notification
	 */
	DECLARE_DELEGATE_OneParam(FReadyConsentNotif, const FAccelByteModelsReadyConsentNotice&);

	/*
	 * @brief delegate for handling rematchmaking notification
	 */
	DECLARE_DELEGATE_OneParam(FRematchmakingNotif, const FAccelByteModelsRematchmakingNotice&);

	/*
	 * @brief delegate for handling DS notification
	 */
	DECLARE_DELEGATE_OneParam(FDsNotif, const FAccelByteModelsDsNotice&);

	// Friends
	/**
	 * @brief delegate for handling request friend response
	 */
	DECLARE_DELEGATE_OneParam(FRequestFriendsResponse, const FAccelByteModelsRequestFriendsResponse&);
	
	/**
	 * @brief delegate for handling unfriend response
	 */
	DECLARE_DELEGATE_OneParam(FUnfriendResponse, const FAccelByteModelsUnfriendResponse&);
	
	/**
	 * @brief delegate for handling list outgoing friends response
	 */
	DECLARE_DELEGATE_OneParam(FListOutgoingFriendsResponse, const FAccelByteModelsListOutgoingFriendsResponse&);
	
	/**
	 * @brief delegate for handling cancel friend response
	 */
	DECLARE_DELEGATE_OneParam(FCancelFriendsResponse, const FAccelByteModelsCancelFriendsResponse&);
	
	/**
	 * @brief delegate for handling list incoming friends response
	 */
	DECLARE_DELEGATE_OneParam(FListIncomingFriendsResponse, const FAccelByteModelsListIncomingFriendsResponse&);
	
	/**
	 * @brief delegate for handling accept friend response
	 */
	DECLARE_DELEGATE_OneParam(FAcceptFriendsResponse, const FAccelByteModelsAcceptFriendsResponse&);
	
	/**
	 * @brief delegate for handling reject friend response
	 */
	DECLARE_DELEGATE_OneParam(FRejectFriendsResponse, const FAccelByteModelsRejectFriendsResponse&);
	
	/**
	 * @brief delegate for handling load friend list response
	 */
	DECLARE_DELEGATE_OneParam(FLoadFriendListResponse, const FAccelByteModelsLoadFriendListResponse&);

	/**
	 * @brief delegate for handling get friendship status response
	 */
	DECLARE_DELEGATE_OneParam(FGetFriendshipStatusResponse, const FAccelByteModelsGetFriendshipStatusResponse&);

	// Friends + Notification
	/**
	 * @brief delegate for handling notification when your request friend is accepted
	 */
	DECLARE_DELEGATE_OneParam(FAcceptFriendsNotif, const FAccelByteModelsAcceptFriendsNotif&);

	/**
	 * @brief delegate for handling notification when you receive a request friend
	 */
	DECLARE_DELEGATE_OneParam(FRequestFriendsNotif, const FAccelByteModelsRequestFriendsNotif&);

	//Signaling
	/**
	 * @brief delegate for handling signaling P2P message
	 */
	DECLARE_DELEGATE_TwoParams(FSignalingP2P, const FString&, const FString&);

	DECLARE_DELEGATE(FConnectSuccess);
	DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);
	
public:
    /**
	 * @brief Connect to the Lobby server via websocket. You must connect to the server before you can start sending/receiving. Also make sure you have logged in first as this operation requires access token.
	 */
	void Connect();

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
	 * @brief Send ping
	 */
	void SendPing();

	/**
	 * @brief Send a private message to another user.
	 * 
	 * @param UserId The recipient's user ID.
	 * @param Message Message to be sent.
	 */
	FString SendPrivateMessage(const FString& UserId, const FString& Message);

	/**
	 * @brief Send a message to other party members.
	 * 
	 * @param Message Message to be sent.
	 */
    FString SendPartyMessage(const FString& Message);
	
	/**
	 * @brief Request to join the default channel chat.
	 */
	FString SendJoinDefaultChannelChatRequest();

	/**
	 * @brief Send a message to joined channel chat.
	 */
	FString SendChannelMessage(const FString& Message);

    //------------------------
    // Party
    //------------------------

	/**
	 * @brief Get information about current party.
	 */
	FString SendInfoPartyRequest();

	/**
	 * @brief Create a party.
	 */
    FString SendCreatePartyRequest();

	/**
	 * @brief Leave current party.
	 */
    FString SendLeavePartyRequest();

	/**
	 * @brief Invite a user to party.
	 * 
	 * @param UserId The target user ID to be invited.
	 */
    FString SendInviteToPartyRequest(const FString& UserId);

    /**
     * @brief Set presence status on lobby service
     *
     * @param State the presence state that you want to use. State is Presence type
     * @param GameName the game name that you play
     */
	FString SendSetPresenceStatus(const Availability Availability, const FString& Activity);

	/**
	 * @brief Accept a party invitation.
	 * 
	 * @param PartyId Party ID from the invitation notice.
	 * @param InvitationToken Random string from the invitation notice.
	 */
	FString SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken);
	
	/**
	 * @brief Kick a party member.
	 * 
	 * @param UserId The target user ID to be kicked.
	 */
	FString SendKickPartyMemberRequest(const FString& UserId);
	/**
	 * @brief Get a list of online users in the Lobby server.
	 */
    FString SendGetOnlineUsersRequest();

	//------------------------
	// Notification
	//------------------------
	/**
	* @brief Get all pending notification(s) that is sent to user when user is not connected to lobby. Please call this function after user connected to lobby.
	*
	*/
	void GetAllAsyncNotification();

	// Matchmaking
	/**
	* @brief start the matchmaking
	*
	* @param GameMode The mode that party member want to play.
	* @param ServerName The Local DS name, fill it blank if you don't use Local DS.
	* @param ClientVersion The version of DS, fill it blank to choose the default version.
	* @param Latencies list of servers and their latencies to client, DSM will created the server on one of this list. Fill it blank if you use Local DS.
	* @param PartyAttributes String map custom attributes to be added on matchmaking and also will be passed to ds too. Example: {"Map":"Dungeon1", "Rank":"B", "Stage":"04"}
	*/
	FString SendStartMatchmaking(FString GameMode, FString ServerName = TEXT(""), FString ClientVersion = TEXT(""), TArray<TPair<FString, float>> Latencies = TArray<TPair<FString, float>>(), TMap<FString, FString> PartyAttributes = TMap<FString, FString>());

	/**
	* @brief cancel the currently running matchmaking process
	*
	* @param GameMode The mode that party member want to cancel.
	*/
	FString SendCancelMatchmaking(FString GameMode);

	/**
	* @brief send ready consent request
	*
	* @param MatchId The id of a match user ready to play.
	*/
	FString SendReadyConsentRequest(FString MatchId);

	// Friends
	/**
	* @brief Send request friend request.
	*
	* @param param UserId Targeted user ID.
	*/
	void RequestFriend(FString UserId);

	/**
	* @brief Send unfriend request.
	*
	* @param UserId Targeted user ID.
	*/
	void Unfriend(FString UserId);

	/**
	* @brief Send list of outgoing friends request.
	*/
	void ListOutgoingFriends();

	/**
	* @brief Send cancel friend request.
	*
	* @param UserId Targeted user ID.
	*/
	void CancelFriendRequest(FString UserId);

	/**
	* @brief Send list of incoming friends request.
	*/
	void ListIncomingFriends();

	/**
	* @brief Send accept friend request.
	*/
	void AcceptFriend(FString UserId);

	/**
	* @brief Send reject friend request.
	*/
	void RejectFriend(FString UserId);

	/**
	* @brief Send load friends list request.
	*/
	void LoadFriendsList();

	/**
	* @brief Send get friendship status request.
	*
	* @param UserId Targeted user ID.
	*/
	void GetFriendshipStatus(FString UserId);

	/**
	 * @brief Send a signaling message to another user.
	 *
	 * @param UserId The recipient's user ID.
	 * @param Message Signaling message to be sent.
	 */
	FString SendSignalingMessage(const FString& UserId, const FString& Message);

	/**
	* @brief Unbind all delegates set previously.
	*/
	void UnbindEvent();

	void SetConnectSuccessDelegate(const FConnectSuccess& OnConnectSuccess)
	{
		ConnectSuccess = OnConnectSuccess;
	}
	void SetConnectFailedDelegate(const FErrorHandler& OnConnectError)
	{
		ConnectError = OnConnectError;
	}
	void SetConnectionClosedDelegate(const FConnectionClosed& OnConnectionClosed)
	{
		ConnectionClosed = OnConnectionClosed;
	}
	void SetPartyLeaveNotifDelegate(const FPartyLeaveNotif& OnLeavePartyNotice)
	{
		PartyLeaveNotif = OnLeavePartyNotice;
	}
	void SetPartyInviteNotifDelegate(const FPartyInviteNotif& OnPartyInviteNotif)
	{
		PartyInviteNotif = OnPartyInviteNotif;
	}
	void SetPartyGetInvitedNotifDelegate(const FPartyGetInvitedNotif& OnInvitePartyGetInvitedNotice)
	{
		PartyGetInvitedNotif = OnInvitePartyGetInvitedNotice;
	}
	void SetPartyJoinNotifDelegate(const FPartyJoinNotif& OnInvitePartyJoinNotice)
	{
		PartyJoinNotif = OnInvitePartyJoinNotice;
	}
	void SetPartyKickNotifDelegate(const FPartyKickNotif& OnInvitePartyKickedNotice)
	{
		PartyKickNotif = OnInvitePartyKickedNotice;
	}
	void SetPrivateMessageNotifDelegate(FPersonalChatNotif OnPersonalChatNotif)
	{
		PersonalChatNotif = OnPersonalChatNotif;
	};
	void SetPartyChatNotifDelegate(FPartyChatNotif OnPersonalChatNotif)
	{
		PartyChatNotif = OnPersonalChatNotif;
	}
	void SetUserPresenceNotifDelegate(FFriendStatusNotif OnUserPresenceNotif)
	{
		FriendStatusNotif = OnUserPresenceNotif;
	};
	void SetMessageNotifDelegate(const FMessageNotif& OnNotificationMessage)
	{
		MessageNotif = OnNotificationMessage;
	}
	void SetOnFriendRequestAcceptedNotifDelegate(const FAcceptFriendsNotif& OnAcceptFriendsNotif)
	{
		AcceptFriendsNotif = OnAcceptFriendsNotif;
	}
	void SetOnIncomingRequestFriendsNotifDelegate(const FRequestFriendsNotif& OnRequestFriendsNotif)
	{
		RequestFriendsNotif = OnRequestFriendsNotif;
	}
	void SetParsingErrorDelegate(const FErrorHandler& OnParsingError)
	{
		ParsingError = OnParsingError;
	}

	// Party
	/**
	* @brief set info party response
	*
	* @param OnInfoPartyResponse set delegate .
	*/
	void SetInfoPartyResponseDelegate(FPartyInfoResponse OnInfoPartyResponse)
	{
		PartyInfoResponse = OnInfoPartyResponse;
	};

	/**
	* @brief create party response delegate
	*
	* @param OnCreatePartyResponse set delegate .
	*/
	void SetCreatePartyResponseDelegate(FPartyCreateResponse OnCreatePartyResponse)
	{
		PartyCreateResponse = OnCreatePartyResponse;
	};

	/**
	* @brief set leave party response delegate
	*
	* @param OnLeavePartyResponse set delegate .
	*/
	void SetLeavePartyResponseDelegate(FPartyLeaveResponse OnLeavePartyResponse)
	{
		PartyLeaveResponse = OnLeavePartyResponse;
	};

	/**
	* @brief set invite party response delegate
	*
	* @param OnInvitePartyResponse set delegate .
	*/
	void SetInvitePartyResponseDelegate(FPartyInviteResponse OnInvitePartyResponse)
	{
		PartyInviteResponse = OnInvitePartyResponse;
	};

	/**
	* @brief set invite party join response
	*
	* @param OnInvitePartyJoinResponse set delegate .
	*/
	void SetInvitePartyJoinResponseDelegate(FPartyJoinResponse OnInvitePartyJoinResponse)
	{
		PartyJoinResponse = OnInvitePartyJoinResponse;
	};

	/**
	* @brief set invite party kick member reponse
	*
	* @param OnInvitePartyKickMemberResponse set delegate .
	*/
	void SetInvitePartyKickMemberResponseDelegate(FPartyKickResponse OnInvitePartyKickMemberResponse)
	{
		PartyKickResponse = OnInvitePartyKickMemberResponse;
	};

	void SetPartyDataUpdateResponseDelegate(FPartyDataUpdateNotif OnPartyDataUpdateResponse) 
	{
		PartyDataUpdateNotif = OnPartyDataUpdateResponse;
	}

	// Chat
	/**
	* @brief set private message delegate
	*
	* @param OnPrivateMessageResponse set delegate .
	*/
	void SetPrivateMessageResponseDelegate(FPersonalChatResponse OnPrivateMessageResponse)
	{
		PersonalChatResponse = OnPrivateMessageResponse;
	};

	/**
	* @brief set party message response
	*
	* @param OnPartyMessageResponse set delegate .
	*/
	void SetPartyMessageResponseDelegate(FPartyChatResponse OnPartyMessageResponse)
	{
		PartyChatResponse = OnPartyMessageResponse;
	};

	/**
	* @brief set join channel chat response
	*
	* @param OnJoinDefaultChannelResponse set delegate.
	*/
	void SetJoinChannelChatResponseDelegate(FJoinDefaultChannelChatResponse OnJoinDefaultChannelResponse)
	{
		JoinDefaultChannelResponse = OnJoinDefaultChannelResponse;
	};

	/**
	* @brief set channel message response
	*
	* @param OnChannelMessageResponse set delegate.
	*/
	void SetChannelMessageResponseDelegate(FChannelChatResponse OnChannelMessageResponse)
	{
		ChannelChatResponse = OnChannelMessageResponse;
	};

	/**
	* @brief set channel message notif
	*
	* @param OnChannelMessageNotif set delegate.
	*/
	void SetChannelMessageNotifDelegate(FChannelChatNotif OnChannelMessageNotif)
	{
		ChannelChatNotif = OnChannelMessageNotif;
	}

	// Presence
	/**
	* @brief set user presence response
	*
	* @param OnSetUserPresenceResponse set delegate .
	*/
	void SetUserPresenceResponseDelegate(FSetUserPresenceResponse OnSetUserPresenceResponse)
	{
		SetUserPresenceResponse = OnSetUserPresenceResponse;
	};

	/**
	* @brief set info party response
	*
	* @param OnGetAllUserPresenceResponse set delegate .
	*/
	void SetGetAllUserPresenceResponseDelegate(FGetAllFriendsStatusResponse OnGetAllUserPresenceResponse)
	{
		GetAllFriendsStatusResponse = OnGetAllUserPresenceResponse;
	};

	// Notification

	// Matchmaking
	/**
	* @brief set start matchmaking response
	*
	* @param OnMatchmakingStart set delegate .
	*/
	void SetStartMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingStart)
	{
		MatchmakingStartResponse = OnMatchmakingStart;
	};

	/**
	* @brief set cancel matchmaking notification
	*
	* @param OnMatchmakingCancel set delegate .
	*/
	void SetCancelMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingCancel)
	{
		MatchmakingCancelResponse = OnMatchmakingCancel;
	};

	/**
	* @brief set ready consent response notification
	*
	* @param OnReadyConsentResponse set delegate .
	*/
	void SetReadyConsentResponseDelegate(FReadyConsentResponse OnReadyConsentResponse)
	{
		ReadyConsentResponse = OnReadyConsentResponse;
	};

	/**
	* @brief set matchmaking notification
	*
	* @param OnMatchmakingNotification set delegate .
	*/
	void SetMatchmakingNotifDelegate(FMatchmakingNotif OnMatchmakingNotification)
	{
		MatchmakingNotif = OnMatchmakingNotification;
	};

	/**
	* @brief set ready consent notification
	*
	* @param OnReadyConsentNotification set delegate .
	*/
	void SetReadyConsentNotifDelegate(FReadyConsentNotif OnReadyConsentNotification)
	{
		ReadyConsentNotif = OnReadyConsentNotification;
	};

	/**
	* @brief set rematchmaking notification
	*
	* @param OnRematchmakingNotification set delegate .
	*/
	void SetRematchmakingNotifDelegate(FRematchmakingNotif OnRematchmakingNotification)
	{
		RematchmakingNotif = OnRematchmakingNotification;
	};

	/**
	* @brief set ready consent notification
	*
	* @param OnReadyConsentNotification set delegate .
	*/
	void SetDsNotifDelegate(FDsNotif OnDsNotification)
	{
		DsNotif = OnDsNotification;
	};

	// Friends
	/**
	* @brief Set request for friends response.
	*
	* @param OnRequestFriendsResponse Delegate that will be set.
	*/
	void SetRequestFriendsResponseDelegate(FRequestFriendsResponse OnRequestFriendsResponse)
	{
		RequestFriendsResponse = OnRequestFriendsResponse;
	};

	/**
	* @brief Set unfriend response.
	*
	* @param OnUnfriendResponse Delegate that will be set.
	*/
	void SetUnfriendResponseDelegate(FUnfriendResponse OnUnfriendResponse)
	{
		UnfriendResponse = OnUnfriendResponse;
	};

	/**
	* @brief Set list outgoing request of friend response.
	*
	* @param OnListOutgoingFriendsResponse Delegate that will be set.
	*/
	void SetListOutgoingFriendsResponseDelegate(FListOutgoingFriendsResponse OnListOutgoingFriendsResponse)
	{
		ListOutgoingFriendsResponse = OnListOutgoingFriendsResponse;
	};

	/**
	* @brief Set cancel request of friend response.
	*
	* @param OnCancelFriendsResponse Delegate that will be set.
	*/
	void SetCancelFriendsResponseDelegate(FCancelFriendsResponse OnCancelFriendsResponse)
	{
		CancelFriendsResponse = OnCancelFriendsResponse;
	};

	/**
	* @brief Set incoming request of friend response.
	*
	* @param OnListIncomingFriendsResponse Delegate that will be set.
	*/
	void SetListIncomingFriendsResponseDelegate(FListIncomingFriendsResponse OnListIncomingFriendsResponse)
	{
		ListIncomingFriendsResponse = OnListIncomingFriendsResponse;
	};

	/**
	* @brief Set accept friend response.
	*
	* @param OnAcceptFriendsResponse Delegate that will be set.
	*/
	void SetAcceptFriendsResponseDelegate(FAcceptFriendsResponse OnAcceptFriendsResponse)
	{
		AcceptFriendsResponse = OnAcceptFriendsResponse;
	};

	/**
	* @brief Set reject request for friend response.
	*
	* @param OnRejectFriendsResponse Delegate that will be set.
	*/
	void SetRejectFriendsResponseDelegate(FRejectFriendsResponse OnRejectFriendsResponse)
	{
		RejectFriendsResponse = OnRejectFriendsResponse;
	};

	/**
	* @brief Set load friend list response.
	*
	* @param OnLoadFriendListResponse Delegate that will be set.
	*/
	void SetLoadFriendListResponseDelegate(FLoadFriendListResponse OnLoadFriendListResponse)
	{
		LoadFriendListResponse = OnLoadFriendListResponse;
	};

	/**
	* @brief Set get friendship status response.
	*
	* @param OnGetFriendshipStatusResponse Delegate that will be set.
	*/
	void SetGetFriendshipStatusResponseDelegate(FGetFriendshipStatusResponse OnGetFriendshipStatusResponse)
	{
		GetFriendshipStatusResponse = OnGetFriendshipStatusResponse;
	};

	/**
	* @brief Set SignalingP2P delegate.
	*
	* @param OnSignalingP2P Delegate that will be set.
	*/
	void SetSignalingP2PDelegate(FSignalingP2P OnSignalingP2P)
	{
		SignalingP2P = OnSignalingP2P;
	};

	/**
	* @brief Bulk add friend(s), don't need any confirmation from the player.
	*
	* @param UserIds the list of UserId you want to make friend with.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed. 
	*/
	void BulkFriendRequest(FAccelByteModelsBulkFriendsRequest UserIds, FVoidHandler OnSuccess, FErrorHandler OnError);

	/**
	* @brief  Get party storage (attributes) by party ID.
	*
	* @param PartyId Targeted party Id.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
	* @param OnError This will be called when the operation failed.
	*/
	void GetPartyStorage(const FString& PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief  Write party storage (attributes) data to the targeted party ID.
	* Beware:
	* Object will not be write immediately, please take care of the original object until it written.
	*
	* @param PartyId Targeted party Id.
	* @param PayloadModifier Function to modify the latest party data with your customized modifier.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
	* @param OnError This will be called when the operation failed.
	* @param RetryAttempt the number of retry to do when there is an error in writing to party storage (likely due to write conflicts).
	*/
	void WritePartyStorage(const FString& PartyId, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, uint32 RetryAttempt = 1);

	static FString LobbyMessageToJson(FString Message);

private:
	Lobby(Lobby const&) = delete; // Copy constructor
	Lobby(Lobby&&) = delete; // Move constructor
	Lobby& operator=(Lobby const&) = delete; // Copy assignment operator
	Lobby& operator=(Lobby &&) = delete; // Move assignment operator

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessage(const FString& Message);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);

    FString SendRawRequest(FString MessageType, FString MessageIDPrefix, FString CustomPayload = TEXT(""));
    bool Tick(float DeltaTime);
    FString GenerateMessageID(FString Prefix = TEXT(""));
	void CreateWebSocket();

	const float LobbyTickPeriod = 0.5;
	const float PingDelay;
	const float InitialBackoffDelay;
	const float MaxBackoffDelay;
	const float TotalTimeout;
	bool bWasWsConnectionError = false;
	float BackoffDelay;
	float RandomizedBackoffDelay;
	float TimeSinceLastPing;
	float TimeSinceLastReconnect;
	float TimeSinceConnectionLost;
	FString ChannelSlug;
	EWebSocketState WsState;
	EWebSocketEvent WsEvents;
	FTickerDelegate LobbyTickDelegate;
    FDelegateHandle LobbyTickDelegateHandle;
	TSharedPtr<IWebSocket> WebSocket;
	FConnectSuccess ConnectSuccess;
	FErrorHandler ConnectError;
    FErrorHandler ParsingError;
	FConnectionClosed ConnectionClosed;
	
    // Party 
    FPartyInfoResponse PartyInfoResponse;
    FPartyCreateResponse PartyCreateResponse;
    FPartyLeaveResponse PartyLeaveResponse;
    FPartyLeaveNotif PartyLeaveNotif;
    FPartyInviteResponse PartyInviteResponse;
    FPartyInviteNotif PartyInviteNotif;
    FPartyGetInvitedNotif PartyGetInvitedNotif;
    FPartyJoinResponse PartyJoinResponse;
    FPartyJoinNotif PartyJoinNotif;
    FPartyKickResponse PartyKickResponse;
    FPartyKickNotif PartyKickNotif;
	FPartyDataUpdateNotif PartyDataUpdateNotif;

    // Chat
    FPersonalChatResponse PersonalChatResponse;
    FPersonalChatNotif PersonalChatNotif;
    FPartyChatResponse PartyChatResponse;
    FPartyChatNotif PartyChatNotif;
	FJoinDefaultChannelChatResponse JoinDefaultChannelResponse;
	FChannelChatResponse ChannelChatResponse;
	FChannelChatNotif ChannelChatNotif;

    // Presence
    FSetUserPresenceResponse SetUserPresenceResponse;
    FFriendStatusNotif FriendStatusNotif;
    FGetAllFriendsStatusResponse GetAllFriendsStatusResponse;

    // Notification
	FMessageNotif MessageNotif;

    // Matchmaking
	FMatchmakingResponse MatchmakingStartResponse;
	FMatchmakingResponse MatchmakingCancelResponse;
	FReadyConsentResponse ReadyConsentResponse;
    FMatchmakingNotif MatchmakingNotif;
	FReadyConsentNotif ReadyConsentNotif;
	FRematchmakingNotif RematchmakingNotif;
	FDsNotif DsNotif;

	// Friends
	FRequestFriendsResponse RequestFriendsResponse;
	FUnfriendResponse UnfriendResponse;
	FListOutgoingFriendsResponse ListOutgoingFriendsResponse;
	FCancelFriendsResponse CancelFriendsResponse;
	FListIncomingFriendsResponse ListIncomingFriendsResponse;
	FAcceptFriendsResponse AcceptFriendsResponse;
	FRejectFriendsResponse RejectFriendsResponse;
	FLoadFriendListResponse LoadFriendListResponse;
	FGetFriendshipStatusResponse GetFriendshipStatusResponse;

	// Friends + Notification
	FAcceptFriendsNotif AcceptFriendsNotif;
	FRequestFriendsNotif RequestFriendsNotif;

	struct PartyStorageWrapper
	{
		FString PartyId;
		int RemainingAttempt;
		THandler<FAccelByteModelsPartyDataNotif> OnSuccess;
		FErrorHandler OnError;
		TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier;
	};

	void RequestWritePartyStorage(const FString& PartyId, const FAccelByteModelsPartyDataUpdateRequest& Data, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, FSimpleDelegate OnConflicted = NULL);

	void WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper);

	//Signaling P2P
	FSignalingP2P SignalingP2P;
};

} // Namespace Api
} // Namespace AccelByte
