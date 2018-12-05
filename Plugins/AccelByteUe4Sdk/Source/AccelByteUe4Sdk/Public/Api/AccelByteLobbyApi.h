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
     * @brief presence enumeration.
     */
    enum class Presence : int {
        Online = 21,
        InParty = 22,
        Playing = 23,
        Offline = 24,
    };

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
    
    // Presence
    /**
     * @brief delegate for handling user change presence status
     */
    DECLARE_DELEGATE_OneParam(FSetUserPresenceResponse, const FAccelByteModelsSetOnlineUsersResponse&);

    /**
     * @brief delegate for handling other user change their presence status event
     */
    DECLARE_DELEGATE_OneParam(FUserPresenceNotif, const FAccelByteModelsUsersPresenceNotice&);             // Passive

    /**
     * @brief delegate for handling get all user presence
     */
    DECLARE_DELEGATE_OneParam(FGetAllUserPresenceResponse, const FAccelByteModelsGetOnlineUsersResponse&);        


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
	 * @brief Get instance of this singleton class.
	 */
	static Lobby& Get();
	DECLARE_DELEGATE(FConnectSuccess);
	DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);
	
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
	FString SendSetPresenceStatus(const Presence State, const FString& GameName);

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
     * @param PartyId Party ID.
     * @param MemberIds list of user id that in the party.
     */
    FString SendStartMatchmaking(FString GameMode, FString PartyId, TArray<FString> MemberIds);

    /**
     * @brief cancel the currently running matchmaking process
     *     
     * @param PartyId Party ID.     
     */
    FString SendCancelMatchmaking(FString PartyId);

	/**
	 * @brief You must bind delegates/callbacks first to handle the events.
	 * For example when a user received a private message or a response to create party request.
	 * A delegate which ends with Notice means that it's like a notification, while one which ends with Response means it's like a response to a request.
	 * The delegates can be `nullptr` if you want to not bind the callback. All delegates have one parameter `Result` with different types.
	 * 
     * @param OnConnectSuccess Called when user connect to lobby successfully.
     * @param OnConnectError Called when user fail to connect to lobby.
     * @param FConnectionClosed Called when user disconnected from lobby.     
     * @param OnLeavePartyNotice Called when other user leave the party.
     * @param OnInvitePartyInvitationNotice Called when user invite to the party.
     * @param OnInvitePartyGetInvitedNotice Called when user got invited to the party.
     * @param OnInvitePartyJoinNotice Called when user join to the party.
     * @param OnInvitePartyKickedNotice Called when other user has been kicked from the party.
     * @param OnPrivateMessageNotice Called when user got private message.
     * @param OnPartyMessageNotice Called when user got party message.
     * @param OnUserPresenceNotice Called when other user change their presence status.
	 * @param OnParsingError Called when receive invalid response from server.
	 */
	void BindEvent(
        const FConnectSuccess& OnConnectSuccess, 
        const FErrorHandler& OnConnectError,
        const FConnectionClosed& OnConnectionClosed,
		const FPartyLeaveNotif& OnLeavePartyNotice,
		const FPartyInviteNotif& OnInvitePartyInvitationNotice,
		const FPartyGetInvitedNotif& OnInvitePartyGetInvitedNotice,
		const FPartyJoinNotif& OnInvitePartyJoinNotice,
		const FPartyKickNotif& OnInvitePartyKickedNotice,
		const FPersonalChatNotif& OnPrivateMessageNotice,
		const FPartyChatNotif& OnPartyMessageNotice,
		const FUserPresenceNotif& OnUserPresenceNotice,
		const FMessageNotif& OnNotificationMessage,
        const FErrorHandler& OnParsingError
	);


	/**
	 * @brief Unbind all delegates set previously.
	 */
	void UnbindEvent();


    // Party
    /**
     * @brief set info party response
     *
     * @param OnInfoPartyResponse set delegate .
     */
    void SetInfoPartyResponseDelegate(FPartyInfoResponse OnInfoPartyResponse) { PartyInfoResponse = OnInfoPartyResponse; };

    /**
     * @brief create party response delegate
     *
     * @param OnCreatePartyResponse set delegate .
     */
    void SetCreatePartyResponseDelegate(FPartyCreateResponse OnCreatePartyResponse) { PartyCreateResponse = OnCreatePartyResponse; };

    /**
     * @brief set leave party response delegate
     *
     * @param OnLeavePartyResponse set delegate .
     */
    void SetLeavePartyResponseDelegate(FPartyLeaveResponse OnLeavePartyResponse) { PartyLeaveResponse = OnLeavePartyResponse; };

    /**
     * @brief set invite party response delegate
     *
     * @param OnInvitePartyResponse set delegate .
     */
    void SetInvitePartyResponseDelegate(FPartyInviteResponse OnInvitePartyResponse) { PartyInviteResponse = OnInvitePartyResponse; };

    /**
     * @brief set invite party join response
     *
     * @param OnInvitePartyJoinResponse set delegate .
     */
    void SetInvitePartyJoinResponseDelegate(FPartyJoinResponse OnInvitePartyJoinResponse) { PartyJoinResponse = OnInvitePartyJoinResponse; };

    /**
     * @brief set invite party kick member reponse
     *
     * @param OnInvitePartyKickMemberResponse set delegate .
     */
    void SetInvitePartyKickMemberResponseDelegate(FPartyKickResponse OnInvitePartyKickMemberResponse) { PartyKickResponse = OnInvitePartyKickMemberResponse; };

    // Chat
    /**
     * @brief set private message delegate
     *
     * @param OnPrivateMessageResponse set delegate .
     */
    void SetPrivateMessageResponseDelegate(FPersonalChatResponse OnPrivateMessageResponse) { PersonalChatResponse = OnPrivateMessageResponse; };

    /**
     * @brief set party message response
     *
     * @param OnPartyMessageResponse set delegate .
     */
    void SetPartyMessageResponseDelegate(FPartyChatResponse OnPartyMessageResponse) { PartyChatResponse = OnPartyMessageResponse; };


    // Presence
    /**
     * @brief set user presence response
     *
     * @param OnSetUserPresenceResponse set delegate .
     */
    void SetUserPresenceResponseDelegate(FSetUserPresenceResponse OnSetUserPresenceResponse) { SetUserPresenceResponse = OnSetUserPresenceResponse; };

    /**
     * @brief set info party response
     *
     * @param OnGetAllUserPresenceResponse set delegate .
     */
    void SetGetAllUserPresenceResponseDelegate(FGetAllUserPresenceResponse OnGetAllUserPresenceResponse) { GetAllUserPresenceResponse = OnGetAllUserPresenceResponse; };

    // Notification

    // Matchmaking
    /**
     * @brief set matchmaking response
     *
     * @param OnMatchmakingResponse set delegate .
     */
    void SetMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingResponse) { MatchmakingResponse = OnMatchmakingResponse; };


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

    FString SendRawRequest(FString MessageType, FString MessageIDPrefix, FString CustomPayload = TEXT(""));
    bool Tick(float DeltaTime);
    FString GenerateMessageID(FString Prefix = TEXT(""));
    FString LobbyMessageToJson(FString Message);

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

    // Chat
    FPersonalChatResponse PersonalChatResponse;
    FPersonalChatNotif PersonalChatNotif;
    FPartyChatResponse PartyChatResponse;
    FPartyChatNotif PartyChatNotif;

    // Presence
    FSetUserPresenceResponse SetUserPresenceResponse;
    FUserPresenceNotif UserPresenceNotif;
    FGetAllUserPresenceResponse GetAllUserPresenceResponse;

    // Notification
	FMessageNotif MessageNotif;

    // Matchmaking
    FMatchmakingResponse MatchmakingResponse;
};

} // Namespace Api
} // Namespace AccelByte
