// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/IAccelByteTokenGenerator.h"
#include "Core/AccelByteWebSocket.h"
#include "Models/AccelByteLobbyModels.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLobby, Log, All);

// Forward declarations
class IWebSocket;

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{	

// Start Matchmaking Optional Params
struct FMatchmakingOptionalParams
{
	FString ServerName{};
	FString ClientVersion{};

	/** @brief If !Latencies, we will refresh them from Qos */
	TArray<TPair<FString, float>> Latencies{};
	
	TMap<FString, FString> PartyAttributes{};
	TArray<FString> TempPartyUserIds{};
	TArray<FString> ExtraAttributes{};
	bool NewSessionOnly {false};
	TArray<FString> SubGameModes{};
};

struct FLobbyMessageMetaData
{
	FString Code{};
	FString Type{};
	FString Id{};
};
	
enum Response : uint8;
enum Notif : uint8;
/**
 * @brief Lobby API for chatting and party management.
 * Unlike other servers which use HTTP, Lobby server uses WebSocket (RFC 6455).
 */
class ACCELBYTEUE4SDK_API Lobby
{
public:
	Lobby(Credentials& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f
		, TSharedPtr<IWebSocket> InWebSocket = nullptr);
	~Lobby();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials& CredentialsRef;
	Settings const& SettingsRef;

	const FString LobbySessionHeaderName = "X-Ab-LobbySessionID";

	bool BanNotifReceived = false;
public:
	
	// Party 
	/**
	 * @brief delegate for handling info party response.
	 */
	DECLARE_DELEGATE_OneParam(FBaseResponse, const FAccelByteModelsLobbyBaseResponse&); 

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
	 * @brief delegate for handling leave party notification. This delegate is DEPRECATED
	 */
	DECLARE_DELEGATE_OneParam(FPartyLeaveNotif, const FAccelByteModelsLeavePartyNotice&);                  // Passive

    /**
     * @brief delegate for handling leave party notification.
     */
    DECLARE_DELEGATE_OneParam(FPartyMemberLeaveNotif, const FAccelByteModelsLeavePartyNotice&);                  // Passive
	
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
    DECLARE_DELEGATE_OneParam(FPartyJoinResponse, const FAccelByteModelsPartyJoinResponse&); 

    /**
     * @brief delegate for handling join party notification
     */
    DECLARE_DELEGATE_OneParam(FPartyJoinNotif, const FAccelByteModelsPartyJoinNotice&);              // Passive

	/**
	* @brief delegate for handling reject party response
	*/
	DECLARE_DELEGATE_OneParam(FPartyRejectResponse, const FAccelByteModelsPartyRejectResponse&); 

	/**
	* @brief delegate for handling reject party notification
	*/
	DECLARE_DELEGATE_OneParam(FPartyRejectNotif, const FAccelByteModelsPartyRejectNotice&);;              // Passive
	
    /**
     * @brief delegate for handling member kicked from party event
     */
    DECLARE_DELEGATE_OneParam(FPartyKickResponse, const FAccelByteModelsKickPartyMemberResponse&); 
    
    /**
     * @brief delegate for handling member kicked from party event notification
     */
    DECLARE_DELEGATE_OneParam(FPartyKickNotif, const FAccelByteModelsGotKickedFromPartyNotice&);   // Passive

	/**
	 * @brief delegate for handling update data notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyDataUpdateNotif, const FAccelByteModelsPartyDataNotif&);
	
	/**
	 * @brief delegate for handling party connect notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyMemberConnectNotif, const FAccelByteModelsPartyMemberConnectionNotice&);

	/**
	 * @brief delegate for handling party disconnect notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyMemberDisconnectNotif, const FAccelByteModelsPartyMemberConnectionNotice&);

	/**
	 * @brief delegate for handling generate party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyGenerateCodeResponse, const FAccelByteModelsPartyGenerateCodeResponse&);

	/**
	 * @brief delegate for handling get party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyGetCodeResponse, const FAccelByteModelsPartyGetCodeResponse&);

	/**
	 * @brief delegate for handling delete party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyDeleteCodeResponse, const FAccelByteModelsPartyDeleteCodeResponse&);

	/**
	 * @brief delegate for handling join party via party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyJoinViaCodeResponse, const FAccelByteModelsPartyJoinResponse&);

	/**
	 * @brief delegate for handling promote party leader response
	 */
	DECLARE_DELEGATE_OneParam(FPartyPromoteLeaderResponse, const FAccelByteModelsPartyPromoteLeaderResponse&);

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

	/*
	* @brief delegate for handling ban notification
	*/
	DECLARE_DELEGATE_OneParam(FUserBannedNotification, const FAccelByteModelsUserBannedNotification&); //Passive

	/*
	* @brief delegate for handling unban notification
	*/
	DECLARE_DELEGATE_OneParam(FUserUnbannedNotification, const FAccelByteModelsUserBannedNotification&); //Passive

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

	/**
	 * @brief delegate for handling unfriend notification
	 */
	DECLARE_DELEGATE_OneParam(FUnfriendNotif, const FAccelByteModelsUnfriendNotif&);
	
	/**
	 * @brief delegate for handling cancel friend notification
	 */
	DECLARE_DELEGATE_OneParam(FCancelFriendsNotif, const FAccelByteModelsCancelFriendsNotif&);

	/**
	 * @brief delegate for handling reject friend notification
	 */
	DECLARE_DELEGATE_OneParam(FRejectFriendsNotif, const FAccelByteModelsRejectFriendsNotif&);
	
	//Block
	/**
	 * @brief delegate for handling block player response
	 */
	DECLARE_DELEGATE_OneParam(FBlockPlayerResponse, const FAccelByteModelsBlockPlayerResponse&);

	/**
	 * @brief delegate for handling unblock player response
	 */
	DECLARE_DELEGATE_OneParam(FUnblockPlayerResponse, const FAccelByteModelsUnblockPlayerResponse&);
	
	/**
	 * @brief delegate for handling list blocked user response
	 */
	DECLARE_DELEGATE_OneParam(FListBlockedUserResponse, const FAccelByteModelsListBlockedUserResponse&);

	/**
	 * @brief delegate for handling list blocker response
	 */
	DECLARE_DELEGATE_OneParam(FListBlockerResponse, const FAccelByteModelsListBlockerResponse&);

	// Block + Notif
	/**
	 * @brief delegate for handling notification when you block a player
	 */
	DECLARE_DELEGATE_OneParam(FBlockPlayerNotif, const FAccelByteModelsBlockPlayerNotif&);

	/**
	 * @brief delegate for handling notification when you unblock a player
	 */
	DECLARE_DELEGATE_OneParam(FUnblockPlayerNotif, const FAccelByteModelsUnblockPlayerNotif&);

	//Signaling
	/**
	 * @brief delegate for handling signaling P2P message
	 */
	DECLARE_DELEGATE_TwoParams(FSignalingP2P, const FString&, const FString&);

	//Error
	/**
	* @brief delegate for handling signaling P2P message
	*/
	DECLARE_DELEGATE_TwoParams(FErrorNotif, int32, const FString&);				//passive
	
	/**
	 * @brief delegate for handling response when setting session attribute
	 */
	DECLARE_DELEGATE_OneParam(FSetSessionAttributeResponse, const FAccelByteModelsSetSessionAttributesResponse&);

	/**
	 * @brief delegate for handling response when setting session attribute
	 */
	DECLARE_DELEGATE_OneParam(FGetSessionAttributeResponse, const FAccelByteModelsGetSessionAttributesResponse&);

	/**
	 * @brief delegate for handling response when setting session attribute
	 */
	DECLARE_DELEGATE_OneParam(FGetAllSessionAttributeResponse, const FAccelByteModelsGetAllSessionAttributesResponse&);

	/**
	* @brief delegate for handling response when refreshing lobby token.
	*/
	DECLARE_DELEGATE_OneParam(FRefreshTokenResponse, const FAccelByteModelsRefreshTokenResponse&)

	DECLARE_DELEGATE(FConnectSuccess);
	DECLARE_DELEGATE_OneParam(FDisconnectNotif, const FAccelByteModelsDisconnectNotif&)
	DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32 /* StatusCode */, const FString& /* Reason */, bool /* WasClean */);
	
public:
    /**
	 * @brief Connect to the Lobby server via websocket. You must connect to the server before you can start sending/receiving. Also make sure you have logged in first as this operation requires access token.
	 */
	void Connect(const FString& Token = "");

	/**
	 * @brief Disconnect from server if and only if the you have connected to server. If not currently connected, then this does nothing.
	 */
	void Disconnect(bool ForceCleanup = false);

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
	FString SendInfoPartyRequest(const FPartyInfoResponse& OnInfoPartyResponse, const FErrorHandler& OnError = {});


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
	FString SendSetPresenceStatus(const EAvailability Availability, const FString& Activity);

	/**
	 * @brief Accept a party invitation.
	 * 
	 * @param PartyId Party ID from the invitation notice.
	 * @param InvitationToken Random string from the invitation notice.
	 */
	FString SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken);
	
	/**
	* @brief Reject a party invitation.
	* 
	* @param PartyId Party ID from the invitation notice.
	* @param InvitationToken Random string from the invitation notice.
	*/
	FString SendRejectInvitationRequest(const FString& PartyId, const FString& InvitationToken);
	
	/**
	 * @brief Kick a party member.
	 * 
	 * @param UserId The target user ID to be kicked.
	 */
	FString SendKickPartyMemberRequest(const FString& UserId);
	
	/**
	 * @brief Generate Party code.
	 * 
	 */
	FString SendPartyGenerateCodeRequest();
	
	/**
	 * @brief Get Party code.
	 * 
	 */
	FString SendPartyGetCodeRequest();

	/**
	 * @brief Remove Party invite code.
	 * 
	 */
	FString SendPartyDeleteCodeRequest();

	/**
	 * @brief Join Party via party code.
	 * 
	 * @param partyCode The target party Code to be joined.
	 */
	FString SendPartyJoinViaCodeRequest(const FString& partyCode);

	/**
	 * @brief (Obsolete) the function name is misleading, please use SendGetOnlineFriendPresenceRequest().
	 */
    FString SendGetOnlineUsersRequest();

	/**
	 * @brief Get list of online friends in the lobby server.
	 */
	FString SendGetOnlineFriendPresenceRequest();

	/**
	 * @brief Promote party member to party leader.
	 * 
	 * @param UserId The target user ID to be promoted.
	 */
	FString SendPartyPromoteLeaderRequest(const FString& UserId);

	/**
	* @brief Set the party's member limit.
	* 
	* @param PartyId The party id to be set.
	* @param Limit The party member limit, should be above 0 and below the configured limit in admin portal.
	* @param OnSuccess The callback triggered when operation is successfull
	* @param OnError the callback triggered when operation failed.
	*/
	void SetPartySizeLimit(const FString& PartyId, const int32 Limit, const FVoidHandler& OnSuccess,
						const FErrorHandler& OnError) const;

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
	* @param TempPartyUserIds UserIDs to form a temporary party with (include user who started the matchmaking). Temporary party will disband when matchmaking finishes.
	* @param ExtraAttributes custom attributes defined in game mode's matching/flexing rule.
	*/
	FString SendStartMatchmaking(FString GameMode, FString ServerName, FString ClientVersion = TEXT(""), TArray<TPair<FString, float>> Latencies = TArray<TPair<FString, float>>(), TMap<FString, FString> PartyAttributes = TMap<FString, FString>(), TArray<FString> TempPartyUserIds = TArray<FString>(), TArray<FString> ExtraAttributes = TArray<FString>());

	/**
	* @brief start the matchmaking
	*
	* @param GameMode The mode that party member want to play.
	* @param TempPartyUserIds UserIDs to form a temporary party with (include user who started the matchmaking). Temporary party will disband when matchmaking finishes.
	* @param ServerName The Local DS name, fill it blank if you don't use Local DS.
	* @param ClientVersion The version of DS, fill it blank to choose the default version.
	* @param Latencies list of servers and their latencies to client, DSM will created the server on one of this list. Fill it blank if you use Local DS.
	* @param PartyAttributes String map custom attributes to be added on matchmaking and also will be passed to ds too. Example: {"Map":"Dungeon1", "Rank":"B", "Stage":"04"}
	* @param ExtraAttributes custom attributes defined in game mode's matching/flexing rule.
	*/
	FString SendStartMatchmaking(FString GameMode, TArray<FString> TempPartyUserIds, FString ServerName = TEXT(""), FString ClientVersion = TEXT(""), TArray<TPair<FString, float>> Latencies = TArray<TPair<FString, float>>(), TMap<FString, FString> PartyAttributes = TMap<FString, FString>(), TArray<FString> ExtraAttributes = TArray<FString>());

	/**
	* @brief start the matchmaking
	*
	* @param GameMode The mode that party member want to play.
	* @param PartyAttributes String map custom attributes to be added on matchmaking and also will be passed to ds too. Example: {"Map":"Dungeon1", "Rank":"B", "Stage":"04"}
	* @param ServerName The Local DS name, fill it blank if you don't use Local DS.
	* @param ClientVersion The version of DS, fill it blank to choose the default version.
	* @param Latencies list of servers and their latencies to client, DSM will created the server on one of this list. Fill it blank if you use Local DS.
	* @param TempPartyUserIds UserIDs to form a temporary party with (include user who started the matchmaking). Temporary party will disband when matchmaking finishes.
	* @param ExtraAttributes custom attributes defined in game mode's matching/flexing rule.
	*/
	FString SendStartMatchmaking(FString GameMode, TMap<FString, FString> PartyAttributes, FString ServerName = TEXT(""), FString ClientVersion = TEXT(""), TArray<TPair<FString, float>> Latencies = TArray<TPair<FString, float>>(), TArray<FString> TempPartyUserIds = TArray<FString>(), TArray<FString> ExtraAttributes = TArray<FString>());

	/**
	* @brief start the matchmaking
	*
	* @param GameMode The mode that party member want to play.
	* @param PartyAttributes String map custom attributes to be added on matchmaking and also will be passed to ds too. Example: {"Map":"Dungeon1", "Rank":"B", "Stage":"04"}
	* @param TempPartyUserIds UserIDs to form a temporary party with (include user who started the matchmaking). Temporary party will disband when matchmaking finishes.
	* @param ServerName The Local DS name, fill it blank if you don't use Local DS.
	* @param ClientVersion The version of DS, fill it blank to choose the default version.
	* @param Latencies list of servers and their latencies to client, DSM will created the server on one of this list. Fill it blank if you use Local DS.
	* @param ExtraAttributes custom attributes defined in game mode's matching/flexing rule.
	*/
	FString SendStartMatchmaking(FString GameMode, TMap<FString, FString> PartyAttributes, TArray<FString> TempPartyUserIds, FString ServerName = TEXT(""), FString ClientVersion = TEXT(""), TArray<TPair<FString, float>> Latencies = TArray<TPair<FString, float>>(), TArray<FString> ExtraAttributes = TArray<FString>());

	/**
	 * @brief Serialize the Qos Latencies obj into a stringified json object.
	 * - Multiple regions are possible (hence the TArray).
	 * @param SelectedLatencies 
	 * @return 
	 */
	static FString GetServerLatenciesJsonStr(TArray<TPair<FString, float>> SelectedLatencies);

	/**
	 * @brief On success, we poll Latencies (if Settings allows us to).
	 */
	void SetOnMatchmakingSuccessInitQosScheduler();
	
	/**
	* @brief start the matchmaking
	*
	* @param GameMode The mode that party member want to play.
	* @param OptionalParams Optional parameters to be considered while matchmaking.
	*/
	FString SendStartMatchmaking(const FString& GameMode, const FMatchmakingOptionalParams& OptionalParams = {});

	/**
	* @brief cancel the currently running matchmaking process
	*
	* @param GameMode The mode that party member want to cancel.
	* @param IsTempParty Is canceling matchmaking that was started using temporary party.
	*/
	FString SendCancelMatchmaking(FString GameMode, bool IsTempParty = false);

	/**
	* @brief send ready consent request
	*
	* @param MatchId The id of a match user ready to play.
	*/
	FString SendReadyConsentRequest(FString MatchId);

	/**
	* @brief Request Dedicated custom server
	*
	* @param SessionID Session ID of the game session.
	* @param GameMode The mode that party member want to play.
	* @param ClientVersion The version of DS, fill it blank to choose the default version.
	* @param Region The region where the party member want to play.
	* @param Deployment The deployment mode where the party member requested
	* @param ServerName The Local DS name, fill it blank if you don't use Local DS.
	*/
	FString RequestDS(FString const& SessionID, FString const& GameMode, FString const& ClientVersion,  FString const& Region, FString const& Deployment, FString const& ServerName = TEXT(""));

	// Friends
	/**
	* @brief Send request friend request.
	*
	* @param UserId Targeted user ID.
	*/
	void RequestFriend(FString UserId);
	
	/**
    * @brief Send friend request by public ID.
    *
    * @param PublicId Targeted user's PublicId.
    */
    void RequestFriendByPublicId(FString PublicId);

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
	 * @brief Block specified player from being able to do certain action against current user
	 * the specified player will be removed from friend list
	 * 
	 * Action that blocked player can't do : 
	 * -Add Friend
	 * -Direct Chat
	 * -Invite to Party
	 * -Invite to Group
	 * -Matchmaking as one alliance
	 * 
	 *	Additional Limitation : 
	 *	Blocked Player can't access blocker/current user's user ID
	 *  
	 * @param UserId the specified player's user ID. (Target to block)
	 * 
	 */
	 void BlockPlayer(const FString& UserId);

	 /**
	  * @brief Unblock specified player enabling them to do certain action against current user
	  *
	  * Action that unblocked player can do :
	  * -Add Friend
	  * -Direct Chat
	  * -Invite to Party
	  * -Invite to Group
	  * -Matchmaking as one alliance
	  *  
	  *	Additional Limitation :
	  *	Unblocked Player can now access blocker/current user's user ID
	  *
	  * @param UserID the specified player's user ID. (Target to block)
	  *
	  */
	 void UnblockPlayer(const FString& UserId);

	/**
	 * @brief Send a signaling message to another user.
	 *
	 * @param UserId The recipient's user ID.
	 * @param Message Signaling message to be sent.
	 */
	FString SendSignalingMessage(const FString& UserId, const FString& Message);

	/**
	 * @brief Set user attribute to lobby session. 
	 *
	 * @param Key the attribute's key.
	 * @param Value the attribute's value.
	 */
	FString SetSessionAttribute(const FString& Key, const FString& Value);

	/**
	 * @brief Get user attribute from lobby session.
	 *
	 * @param Key the attribute's key.
	 */
	FString GetSessionAttribute(const FString& Key);

	/**
	 * @brief Get all user attribute from lobby session.
	 */
	FString GetAllSessionAttribute();

	/**
	* @brief Refresh access token used in lobby.
	*/
	FString RefreshToken(const FString& AccessToken);

	/**
	* @brief Unbind all delegates set previously.
	*/
	void UnbindEvent();

	/**
	* @brief Unbind all party notif delegates set previously.
	*/
	void UnbindPartyNotifEvents();

	/**
	* @brief Unbind all party response delegates set previously.
	*/
	void UnbindPartyResponseEvents();

	/**
	* @brief Unbind all friend notif delegates set previously.
	*/
	void UnbindFriendNotifEvents();

	/**
	* @brief Unbind all friend response delegates set previously.
	*/
	void UnbindFriendResponseEvents();

	/**
	* @brief Unbind all matchmaking notif delegates set previously.
	*/
	void UnbindMatchmakingNotifEvents();

	/**
	* @brief Unbind all matchmaking response delegates set previously.
	*/
	void UnbindMatchmakingResponseEvents();

	/**
	* @brief Unbind all chat notif delegates set previously.
	*/
	void UnbindChatNotifEvents();

	/**
	* @brief Unbind all chat response delegates set previously.
	*/
	void UnbindChatResponseEvents();

	/**
	* @brief Unbind all Block notif delegates set previously.
	*/
	void UnbindBlockNotifEvents();

	/**
	* @brief Unbind all Block response delegates set previously.
	*/
	void UnbindBlockResponseEvents();

	/**
	* @brief Unbind all session attribute response delegates set previously.
	*/
	void UnbindSessionAttributeEvents();

	void SetConnectSuccessDelegate(const FConnectSuccess& OnConnectSuccess)
	{
		ConnectSuccess = OnConnectSuccess;
	}
	void SetConnectFailedDelegate(const FErrorHandler& OnConnectError)
	{
		ConnectError = OnConnectError;
	}
	void SetDisconnectNotifDelegate(const FDisconnectNotif OnDisconnectNotice)
	{
		DisconnectNotif = OnDisconnectNotice;
	}
	void SetConnectionClosedDelegate(const FConnectionClosed& OnConnectionClosed)
	{
		ConnectionClosed = OnConnectionClosed;
	}
	/**
	 * @brief Set a trigger function when a party member leave from the party. This function is DEPRECATED
	 * @param OnPartyLeaveNotice delegate parameter with returned model called FAccelByteModelsLeavePartyNotice
	 */
	void SetPartyLeaveNotifDelegate(const FPartyLeaveNotif& OnPartyLeaveNotice)
	{
		PartyLeaveNotif = OnPartyLeaveNotice;
	}
	/**
	 * @brief Set a trigger function when a party member leave from the party
	 * @param OnPartyMemberLeaveNotice delegate parameter with returned model called FAccelByteModelsLeavePartyNotice
	 */
	void SetPartyMemberLeaveNotifDelegate(const FPartyMemberLeaveNotif& OnPartyMemberLeaveNotice)
	{
		PartyMemberLeaveNotif = OnPartyMemberLeaveNotice;
	}
	/**
	 * @brief Set a trigger function when a party member reconnecting to the lobby
	 * @param OnPartyMemberConnectNotif return models called FAccelByteModelsPartyMemberConnectionNotice
	 */
	void SetPartyMemberConnectNotifDelegate(FPartyMemberConnectNotif OnPartyMemberConnectNotif) 
	{
		PartyMemberConnectNotif = OnPartyMemberConnectNotif;
	}
	/**
	 * @brief Set a trigger function when a party member disconnect from lobby
	 * @param OnPartyMemberDisconnectNotif return models called FAccelByteModelsPartyMemberConnectionNotice
	 */
	void SetPartyMemberDisconnectNotifDelegate(FPartyMemberDisconnectNotif OnPartyMemberDisconnectNotif) 
	{
		PartyMemberDisconnectNotif = OnPartyMemberDisconnectNotif;
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
	void SetPartyInvitationRejectedNotifDelegate(const FPartyRejectNotif& OnInvitePartyRejectNotice)
	{
		PartyRejectNotif = OnInvitePartyRejectNotice;
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
	void SetUserBannedNotificationDelegate(FUserBannedNotification OnUserBannedNotification)
	{
		UserBannedNotification = OnUserBannedNotification;
	}
	void SetUserUnbannedNotificationDelegate(FUserUnbannedNotification OnUserUnbannedNotification)
	{
		UserUnbannedNotification = OnUserUnbannedNotification;
	}
	void SetOnFriendRequestAcceptedNotifDelegate(const FAcceptFriendsNotif& OnAcceptFriendsNotif)
	{
		AcceptFriendsNotif = OnAcceptFriendsNotif;
	}
	void SetOnIncomingRequestFriendsNotifDelegate(const FRequestFriendsNotif& OnRequestFriendsNotif)
	{
		RequestFriendsNotif = OnRequestFriendsNotif;
	}
	void SetOnUnfriendNotifDelegate(const FUnfriendNotif& OnUnfriendNotif)
	{
		UnfriendNotif = OnUnfriendNotif;
	}
	void SetOnCancelFriendsNotifDelegate(const FCancelFriendsNotif& OnCancelFriendsNotif)
	{
		CancelFriendsNotif = OnCancelFriendsNotif;
	}
	void SetOnRejectFriendsNotifDelegate(const FRejectFriendsNotif& OnRejectFriendsNotif) 
	{
		RejectFriendsNotif = OnRejectFriendsNotif;
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
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetInfoPartyResponseDelegate(FPartyInfoResponse OnInfoPartyResponse, FErrorHandler OnError = {})
	{
		PartyInfoResponse = OnInfoPartyResponse;
		OnPartyInfoError = OnError;
	};

	/**
	* @brief create party response delegate
	*
	* @param OnCreatePartyResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetCreatePartyResponseDelegate(FPartyCreateResponse OnCreatePartyResponse, FErrorHandler OnError = {})
	{
		PartyCreateResponse = OnCreatePartyResponse;
		OnPartyCreateError = OnError;
	};

	/**
	* @brief set leave party response delegate
	*
	* @param OnLeavePartyResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetLeavePartyResponseDelegate(FPartyLeaveResponse OnLeavePartyResponse, FErrorHandler OnError = {})
	{
		PartyLeaveResponse = OnLeavePartyResponse;
		OnPartyLeaveError = OnError;
	};

	/**
	* @brief set invite party response delegate
	*
	* @param OnInvitePartyResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetInvitePartyResponseDelegate(FPartyInviteResponse OnInvitePartyResponse, FErrorHandler OnError = {})
	{
		PartyInviteResponse = OnInvitePartyResponse;
		OnPartyInviteError = OnError;
	};

	/**
	* @brief set invite party join response
	*
	* @param OnInvitePartyJoinResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetInvitePartyJoinResponseDelegate(FPartyJoinResponse OnInvitePartyJoinResponse, FErrorHandler OnError = {})
	{
		PartyJoinResponse = OnInvitePartyJoinResponse;
		OnPartyJoinError = OnError;
	};
	
	/**
	* @brief set invite party reject response
	*
	* @param OnInvitePartyRejectResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetInvitePartyRejectResponseDelegate(FPartyRejectResponse OnInvitePartyRejectResponse, FErrorHandler OnError = {})
	{
		PartyRejectResponse = OnInvitePartyRejectResponse;
		OnPartyRejectError = OnError;
	};

	/**
	* @brief set invite party kick member response
	*
	* @param OnInvitePartyKickMemberResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetInvitePartyKickMemberResponseDelegate(FPartyKickResponse OnInvitePartyKickMemberResponse, FErrorHandler OnError = {})
	{
		PartyKickResponse = OnInvitePartyKickMemberResponse;
		OnPartyKickError = OnError;
	};

	/**
	 * @brief misnamed, same as SetPartyDataUpdateNotifDelegate
	 *
	 * @param OnPartyDataUpdateNotif set delegate
	 */
	void SetPartyDataUpdateResponseDelegate(FPartyDataUpdateNotif OnPartyDataUpdateNotif) 
	{
		PartyDataUpdateNotif = OnPartyDataUpdateNotif;
	};
	
	/**
	* @brief set party data update notif delegate
	*
	* @param OnPartyDataUpdateNotif set delegate
	*/
	void SetPartyDataUpdateNotifDelegate(FPartyDataUpdateNotif OnPartyDataUpdateNotif) 
	{
		PartyDataUpdateNotif = OnPartyDataUpdateNotif;
	};

	/**
	* @brief generate party code response
	*
	* @param OnPartyGenerateCodeResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPartyGenerateCodeResponseDelegate(FPartyGenerateCodeResponse OnPartyGenerateCodeResponse, FErrorHandler OnError = {})
	{
		PartyGenerateCodeResponse = OnPartyGenerateCodeResponse;
		OnPartyGenerateCodeError = OnError;
	};

	/**
	* @brief get party code response
	*
	* @param OnPartyGetCodeResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPartyGetCodeResponseDelegate(FPartyGetCodeResponse OnPartyGetCodeResponse, FErrorHandler OnError = {})
	{
		PartyGetCodeResponse = OnPartyGetCodeResponse;
		OnPartyGetCodeError = OnError;
	};

	/**
	* @brief delete party code response
	*
	* @param OnPartyDeleteCodeResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPartyDeleteCodeResponseDelegate(FPartyDeleteCodeResponse OnPartyDeleteCodeResponse, FErrorHandler OnError = {})
	{
		PartyDeleteCodeResponse = OnPartyDeleteCodeResponse;
		OnPartyDeleteCodeError = OnError;
	};

	/**
	* @brief join party via party code response
	*
	* @param OnPartyJoinViaCodeResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPartyJoinViaCodeResponseDelegate(FPartyJoinViaCodeResponse OnPartyJoinViaCodeResponse, FErrorHandler OnError = {})
	{
		PartyJoinViaCodeResponse = OnPartyJoinViaCodeResponse;
		OnPartyJoinViaCodeError = OnError;
	};

	/**
	* @brief set promote party leader response
	*
	* @param OnPromotePartyLeaderResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPartyPromoteLeaderResponseDelegate(FPartyPromoteLeaderResponse OnPartyPromoteLeaderResponse, FErrorHandler OnError = {})
	{
		PartyPromoteLeaderResponse = OnPartyPromoteLeaderResponse;
		OnPartyPromoteLeaderError = OnError;
	};

	// Chat
	/**
	* @brief set private message delegate
	*
	* @param OnPrivateMessageResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPrivateMessageResponseDelegate(FPersonalChatResponse OnPrivateMessageResponse, FErrorHandler OnError = {})
	{
		PersonalChatResponse = OnPrivateMessageResponse;
		OnPersonalChatError = OnError;
	};

	/**
	* @brief set party message response
	*
	* @param OnPartyMessageResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetPartyMessageResponseDelegate(FPartyChatResponse OnPartyMessageResponse, FErrorHandler OnError = {})
	{
		PartyChatResponse = OnPartyMessageResponse;
		OnPartyChatError = OnError;
	};

	/**
	* @brief set join channel chat response
	*
	* @param OnJoinDefaultChannelResponse set delegate.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetJoinChannelChatResponseDelegate(FJoinDefaultChannelChatResponse OnJoinDefaultChannelResponse, FErrorHandler OnError = {})
	{
		JoinDefaultChannelChatResponse = OnJoinDefaultChannelResponse;
		OnJoinDefaultChannelChatError = OnError;
	};

	/**
	* @brief set channel message response
	*
	* @param OnChannelMessageResponse set delegate.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetChannelMessageResponseDelegate(FChannelChatResponse OnChannelMessageResponse, FErrorHandler OnError = {})
	{
		ChannelChatResponse = OnChannelMessageResponse;
		OnChannelChatError = OnError;
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
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetUserPresenceResponseDelegate(FSetUserPresenceResponse OnSetUserPresenceResponse, FErrorHandler OnError = {})
	{
		SetUserPresenceResponse = OnSetUserPresenceResponse;
		OnSetUserPresenceError = OnError;
	};

	/**
	* @brief (obsolete) misnamed, please use SetGetOnlineFriendsPresenceResponseDelegate
	*
	* @param OnGetAllUserPresenceResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetGetAllUserPresenceResponseDelegate(FGetAllFriendsStatusResponse OnGetAllUserPresenceResponse, FErrorHandler OnError = {})
	{
		GetAllFriendsStatusResponse = OnGetAllUserPresenceResponse;
		OnGetAllFriendsStatusError = OnError;
	};

	/**
	* @brief Set GetFriendsPresenceResponse delegate. 
	*
	* @param OnGetFriendsPresenceResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetGetOnlineFriendsPresenceResponseDelegate(FGetAllFriendsStatusResponse OnGetFriendsPresenceResponse, FErrorHandler OnError = {})
	{
		GetAllFriendsStatusResponse = OnGetFriendsPresenceResponse;
		OnGetAllFriendsStatusError = OnError;
	};

	// Notification

	// Matchmaking
	/**
	* @brief set start matchmaking response
	*
	* @param OnMatchmakingStart set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetStartMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingStart, FErrorHandler OnError = {})
	{
		MatchmakingStartResponse = OnMatchmakingStart;
		OnMatchmakingStartError = OnError;
	};

	/**
	* @brief set cancel matchmaking notification
	*
	* @param OnMatchmakingCancel set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetCancelMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingCancel, FErrorHandler OnError = {})
	{
		MatchmakingCancelResponse = OnMatchmakingCancel;
		OnMatchmakingCancelError = OnError;
	};

	/**
	* @brief set ready consent response notification
	*
	* @param OnReadyConsentResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetReadyConsentResponseDelegate(FReadyConsentResponse OnReadyConsentResponse, FErrorHandler OnError = {})
	{
		ReadyConsentResponse = OnReadyConsentResponse;
		OnReadyConsentError = OnError;
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

	/**
	 * @brief set create DS response delegate
	 * *
	 * */
	void SetCreateDSDelegate(FBaseResponse OnCreateDSResponse, FErrorHandler OnError = {})
	{
		CreateDSResponse = OnCreateDSResponse;
		OnCreateDSError = OnError;
	}

	// Friends
	/**
	* @brief Set request for friends response.
	*
	* @param OnRequestFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetRequestFriendsResponseDelegate(FRequestFriendsResponse OnRequestFriendsResponse, FErrorHandler OnError = {})
	{
		RequestFriendsResponse = OnRequestFriendsResponse;
		OnRequestFriendsError = OnError;
	};
	
	/**
	* @brief Set request for friends by public ID response.
	*
	* @param OnRequestFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetRequestFriendsByPublicIdResponseDelegate(FRequestFriendsResponse OnRequestFriendsResponse, FErrorHandler OnError = {})
	{
		RequestFriendsByPublicIdResponse = OnRequestFriendsResponse;
		OnRequestFriendsByPublicIdError = OnError;
	};

	/**
	* @brief Set unfriend response.
	*
	* @param OnUnfriendResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetUnfriendResponseDelegate(FUnfriendResponse OnUnfriendResponse, FErrorHandler OnError = {})
	{
		UnfriendResponse = OnUnfriendResponse;
		OnUnfriendError = OnError;
	};

	/**
	* @brief Set list outgoing request of friend response.
	*
	* @param OnListOutgoingFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetListOutgoingFriendsResponseDelegate(FListOutgoingFriendsResponse OnListOutgoingFriendsResponse, FErrorHandler OnError = {})
	{
		ListOutgoingFriendsResponse = OnListOutgoingFriendsResponse;
		OnListOutgoingFriendsError = OnError;
	};

	/**
	* @brief Set cancel request of friend response.
	*
	* @param OnCancelFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetCancelFriendsResponseDelegate(FCancelFriendsResponse OnCancelFriendsResponse, FErrorHandler OnError = {})
	{
		CancelFriendsResponse = OnCancelFriendsResponse;
		OnCancelFriendsError = OnError;
	};

	/**
	* @brief Set incoming request of friend response.
	*
	* @param OnListIncomingFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetListIncomingFriendsResponseDelegate(FListIncomingFriendsResponse OnListIncomingFriendsResponse, FErrorHandler OnError = {})
	{
		ListIncomingFriendsResponse = OnListIncomingFriendsResponse;
		OnListIncomingFriendsError = OnError;
	};

	/**
	* @brief Set accept friend response.
	*
	* @param OnAcceptFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetAcceptFriendsResponseDelegate(FAcceptFriendsResponse OnAcceptFriendsResponse, FErrorHandler OnError = {})
	{
		AcceptFriendsResponse = OnAcceptFriendsResponse;
		OnAcceptFriendsError = OnError;
	};

	/**
	* @brief Set reject request for friend response.
	*
	* @param OnRejectFriendsResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetRejectFriendsResponseDelegate(FRejectFriendsResponse OnRejectFriendsResponse, FErrorHandler OnError = {})
	{
		RejectFriendsResponse = OnRejectFriendsResponse;
		OnRejectFriendsError = OnError;
	};

	/**
	* @brief Set load friend list response.
	*
	* @param OnLoadFriendListResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetLoadFriendListResponseDelegate(FLoadFriendListResponse OnLoadFriendListResponse, FErrorHandler OnError = {})
	{
		LoadFriendListResponse = OnLoadFriendListResponse;
		OnLoadFriendListError = OnError;
	};

	/**
	* @brief Set get friendship status response.
	*
	* @param OnGetFriendshipStatusResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetGetFriendshipStatusResponseDelegate(FGetFriendshipStatusResponse OnGetFriendshipStatusResponse, FErrorHandler OnError = {})
	{
		GetFriendshipStatusResponse = OnGetFriendshipStatusResponse;
		OnGetFriendshipStatusError = OnError;
	};

	// Player
	/**
	* @brief set unblock player responses
	*
	* @param OnBlockPlayerResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	* */
	void SetBlockPlayerResponseDelegate(FBlockPlayerResponse OnBlockPlayerResponse, FErrorHandler OnError = {})
	{
		BlockPlayerResponse = OnBlockPlayerResponse;
		OnBlockPlayerError = OnError;
	};

	/**
	 * @brief set unblock player responses
	 *
	 * @param OnUnblockPlayerResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 * */
	void SetUnblockPlayerResponseDelegate(FUnblockPlayerResponse OnUnblockPlayerResponse, FErrorHandler OnError = {})
	{
		UnblockPlayerResponse = OnUnblockPlayerResponse;
		OnUnblockPlayerError = OnError;
	};

	void SetBlockPlayerNotifDelegate(FBlockPlayerNotif OnBlockPlayerNotif)
	{
		BlockPlayerNotif = OnBlockPlayerNotif;
	};

	void SetUnblockPlayerNotifDelegate(FUnblockPlayerNotif OnUnblockPlayerNotif)
	{
		UnblockPlayerNotif = OnUnblockPlayerNotif;
	};

	// Error
	void SetErrorNotifDelegate(FErrorNotif OnErrorNotif)
	{
		ErrorNotif = OnErrorNotif;
	};

	/**
	* @brief Set SignalingP2P delegate.
	*
	* @param OnSignalingP2P Delegate that will be set.
	*/
	void SetSignalingP2PDelegate(FSignalingP2P OnSignalingP2P)
	{
		SignalingP2PNotif = OnSignalingP2P;
	};

	/**
	* @brief Set SetSessionAttribute delegate.
	*
	* @param OnSetSessionAttributeResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetSetSessionAttributeDelegate(FSetSessionAttributeResponse OnSetSessionAttributeResponse, FErrorHandler OnError = {})
	{
		SetSessionAttributeResponse = OnSetSessionAttributeResponse;
		OnSetSessionAttributeError = OnError;
		
	};

	/**
	* @brief Set GetSessionAttribute delegate.
	*
	* @param OnGetSessionAttributeResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetGetSessionAttributeDelegate(FGetSessionAttributeResponse OnGetSessionAttributeResponse, FErrorHandler OnError = {})
	{
		GetSessionAttributeResponse = OnGetSessionAttributeResponse;
		OnGetSessionAttributeError = OnError;
	};

	/**
	* @brief Set SetSessionAttribute delegate.
	*
	* @param OnGetAllSessionAttributeResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetGetAllSessionAttributeDelegate(FGetAllSessionAttributeResponse OnGetAllSessionAttributeResponse, FErrorHandler OnError = {})
	{
		GetAllSessionAttributeResponse = OnGetAllSessionAttributeResponse;
		OnGetAllSessionAttributeError = OnError;
	};

	/**
	* @brief Set SetSessionAttribute delegate.
	*
	* @param OnRefreshTokenResponse Delegate that will be set.
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetRefreshTokenDelegate(const FRefreshTokenResponse& OnRefreshTokenResponse, FErrorHandler OnError = {})
	{
		RefreshTokenResponse = OnRefreshTokenResponse;
		OnRefreshTokenError = OnError;
	}

	/**
	* @brief Bulk add friend(s), don't need any confirmation from the player.
	*
	* @param UserIds the list of UserId you want to make friend with.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed. 
	*/
	void BulkFriendRequest(FAccelByteModelsBulkFriendsRequest UserIds, FVoidHandler OnSuccess, FErrorHandler OnError);

	/**
	* @brief  Get party data by party ID.
	*
	* @param PartyId Targeted party Id.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyData model.
	* @param OnError This will be called when the operation failed.
	*/
	void GetPartyData(const FString& PartyId, const THandler<FAccelByteModelsPartyData>& OnSuccess, const FErrorHandler& OnError) const;

	/*
	* @brief Bulk Get User(s) Presence, can get specific user's presence status not limited to friend.
	*
	* @param UserIds the list of UserId you want to request.
	* @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsBulkUserStatusNotif.
	* @param OnError This will be called when the operation failed.
	* @param CountOnly Will only return the status count, without the user's data when set to true.
	*/
	void BulkGetUserPresence(const TArray<FString>& UserIds, const THandler<FAccelByteModelsBulkUserStatusNotif>& OnSuccess, const FErrorHandler& OnError, bool CountOnly = false);


	/**
	* @brief  Get party storage (attributes) by party ID.
	*
	* @param PartyId Targeted party Id.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
	* @param OnError This will be called when the operation failed.
	*/
	void GetPartyStorage(const FString& PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief  List Blocked User's Id
	*
	* @param UserId User Id of the Player.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockedUserResponse model.
	* @param OnError This will be called when the operation failed.
	*/
	void GetListOfBlockedUsers(const FString& UserId, const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, const FErrorHandler& OnError);

	/*
	* @brief List Blocked User's Id, override of GetListOfBlockedUser,
	* will get current user's List of Blocked User's Id
	*
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockedUserResponse model.
	* @param OnError This will be called when the operation failed.
	*/
	void GetListOfBlockedUsers(const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief List Id of Player that blocks an User.
	*
	* @param UserId User Id of the Player.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
	* @param OnError This will be called when the operation failed.
	*/
	void GetListOfBlockers(const FString& UserId, const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, const FErrorHandler& OnError);

	/*
	* @brief List Blockers User's Id, override of GetListOfBlocker,
	* will get current user's List of Blocker User's Id
	*
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
	* @param OnError This will be called when the operation failed.
	*/

	void GetListOfBlockers(const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, const FErrorHandler& OnError);

	void SendNotificationToUser(const FString& SendToUserId, const FAccelByteModelsFreeFormNotificationRequest& Message, bool bAsync, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

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

	/**
	* @brief Change the delay parameters to maintain connection in the lobby before lobby connected.
	*
	* @param NewTotalTimeout new Time limit until stop to re-attempt.
	* @param NewBackoffDelay new Initial delay time.
	* @param NewMaxDelay new Maximum delay time.
	*/
	void SetRetryParameters(int32 NewTotalTimeout = 60000, int32 NewBackoffDelay = 1000, int32 NewMaxDelay = 30000);

	/**
	* @brief Set token generator to be used when trying to connect to lobby using ownership token.
	*
	* @param TokenGenerator The token generator.
	*/
	void SetTokenGenerator(TSharedPtr<IAccelByteTokenGenerator> TokenGenerator);

	static FString LobbyMessageToJson(FString Message);

	void ClearLobbyErrorMessages();

private:
	Lobby(Lobby const&) = delete; // Copy constructor
	Lobby(Lobby&&) = delete; // Move constructor
	Lobby& operator=(Lobby const&) = delete; // Copy assignment operator
	Lobby& operator=(Lobby &&) = delete; // Move assignment operator

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessage(const FString& Message);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);

    FString SendRawRequest(const FString& MessageType, const FString& MessageIDPrefix, const FString& CustomPayload = TEXT(""));
    FString GenerateMessageID(const FString& Prefix = TEXT("")) const;
	void CreateWebSocket(const FString& Token = "");
	void FetchLobbyErrorMessages();
	bool ExtractLobbyMessageMetaData(const FString& InLobbyMessage, TSharedRef<FLobbyMessageMetaData>& OutLobbyMessageMetaData);
	
	THandler<const FString&> OnTokenReceived = THandler<const FString&>::CreateLambda([&](const FString& Token)
	{
		Connect(Token);
	});

#pragma region Message Parsing
	void HandleMessageResponse(const FString& ReceivedMessageType, const FString& ParsedJsonString, const TSharedPtr<FJsonObject>& ParsedJsonObj, const TSharedPtr<FLobbyMessageMetaData>& MessageMeta);
	void HandleMessageNotif(const FString& ReceivedMessageType, const FString& ParsedJsonString, const TSharedPtr<FJsonObject>& ParsedJsonObj);
	static TMap<FString, Response> ResponseStringEnumMap; 
	static TMap<FString, Notif> NotifStringEnumMap;
#pragma endregion

	TMap<FString, FString> LobbyErrorMessages;
	const float LobbyTickPeriod = 0.5;
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
	FString ChannelSlug;
	TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> WebSocket;
	FAccelByteModelsLobbySessionId LobbySessionId;
	FConnectSuccess ConnectSuccess;
	FErrorHandler ConnectError;
    FErrorHandler ParsingError;
	FDisconnectNotif DisconnectNotif;
	FConnectionClosed ConnectionClosed;
	TSharedPtr<IAccelByteTokenGenerator> TokenGenerator;

	const FVoidHandler RefreshTokenDelegate = FVoidHandler::CreateLambda([&]()
	{
		RefreshToken(CredentialsRef.GetAccessToken());
	});
	
#pragma region Message Id - Response Map
	TMap<FString, FPartyInfoResponse> MessageIdPartyInfoResponseMap;
	TMap<FString, FPartyCreateResponse> MessageIdPartyCreateResponseMap;
	TMap<FString, FPartyLeaveResponse> MessageIdPartyLeaveResponseMap;
	TMap<FString, FPartyInviteResponse> MessageIdPartyInviteResponseMap;
	TMap<FString, FPartyJoinResponse> MessageIdPartyJoinResponseMap;
	TMap<FString, FPartyRejectResponse> MessageIdPartyRejectResponseMap;
	TMap<FString, FPartyKickResponse> MessageIdPartyKickResponseMap;
	TMap<FString, FPartyGenerateCodeResponse> MessageIdPartyGenerateCodeResponseMap;
	TMap<FString, FPartyGetCodeResponse> MessageIdPartyGetCodeResponseMap;
	TMap<FString, FPartyDeleteCodeResponse> MessageIdPartyDeleteCodeResponseMap;
	TMap<FString, FPartyJoinViaCodeResponse> MessageIdPartyJoinViaCodeResponseMap;
	TMap<FString, FPartyPromoteLeaderResponse> MessageIdPartyPromoteLeaderResponseMap;

	// Chat
	TMap<FString, FPersonalChatResponse> MessageIdPersonalChatResponseMap;
	TMap<FString, FPartyChatResponse> MessageIdPartyChatResponseMap;
	TMap<FString, FJoinDefaultChannelChatResponse> MessageIdJoinDefaultChannelChatResponseMap;
	TMap<FString, FChannelChatResponse> MessageIdChannelChatResponseMap;

	// Presence
	TMap<FString, FSetUserPresenceResponse> MessageIdSetUserPresenceResponseMap;
	TMap<FString, FGetAllFriendsStatusResponse> MessageIdGetAllFriendsStatusResponseMap;

	// Matchmaking
	TMap<FString, FMatchmakingResponse> MessageIdMatchmakingStartResponseMap;
	TMap<FString, FMatchmakingResponse> MessageIdMatchmakingCancelResponseMap;
	TMap<FString, FReadyConsentResponse> MessageIdReadyConsentResponseMap;

	// Custom Game
	TMap<FString, FBaseResponse> MessageIdCreateDSResponseMap;

	// Friends
	TMap<FString, FRequestFriendsResponse> MessageIdRequestFriendsResponseMap;
	TMap<FString, FRequestFriendsResponse> MessageIdRequestFriendsByPublicIdResponseMap;
	TMap<FString, FUnfriendResponse> MessageIdUnfriendResponseMap;
	TMap<FString, FListOutgoingFriendsResponse> MessageIdListOutgoingFriendsResponseMap;
	TMap<FString, FCancelFriendsResponse> MessageIdCancelFriendsResponseMap;
	TMap<FString, FListIncomingFriendsResponse> MessageIdListIncomingFriendsResponseMap;
	TMap<FString, FAcceptFriendsResponse> MessageIdAcceptFriendsResponseMap;
	TMap<FString, FRejectFriendsResponse> MessageIdRejectFriendsResponseMap;
	TMap<FString, FLoadFriendListResponse> MessageIdLoadFriendListResponseMap;
	TMap<FString, FGetFriendshipStatusResponse> MessageIdGetFriendshipStatusResponseMap;

	// Block
	TMap<FString, FBlockPlayerResponse> MessageIdBlockPlayerResponseMap;
	TMap<FString, FUnblockPlayerResponse> MessageIdUnblockPlayerResponseMap;
	TMap<FString, FListBlockedUserResponse> MessageIdListBlockedUserResponseMap;
	TMap<FString, FListBlockerResponse> MessageIdListBlockerResponseMap;

	//Session Attribute
	TMap<FString, FSetSessionAttributeResponse> MessageIdSetSessionAttributeResponseMap;
	TMap<FString, FGetSessionAttributeResponse> MessageIdGetSessionAttributeResponseMap;
	TMap<FString, FGetAllSessionAttributeResponse> MessageIdGetAllSessionAttributeResponseMap;
	TMap<FString, FRefreshTokenResponse> MessageIdRefreshTokenResponseMap;
#pragma endregion

#pragma region Response/Notif Delegates
    // Party 
    FPartyInfoResponse PartyInfoResponse;
    FPartyCreateResponse PartyCreateResponse;
    FPartyLeaveResponse PartyLeaveResponse;
	FPartyLeaveNotif PartyLeaveNotif; // This Delegate is DEPRECATED
    FPartyMemberLeaveNotif PartyMemberLeaveNotif;
    FPartyInviteResponse PartyInviteResponse;
    FPartyInviteNotif PartyInviteNotif;
    FPartyGetInvitedNotif PartyGetInvitedNotif;
    FPartyJoinResponse PartyJoinResponse;
    FPartyJoinNotif PartyJoinNotif;
    FPartyRejectResponse PartyRejectResponse;
    FPartyRejectNotif PartyRejectNotif;
    FPartyKickResponse PartyKickResponse;
    FPartyKickNotif PartyKickNotif;
	FPartyDataUpdateNotif PartyDataUpdateNotif;
	FPartyMemberConnectNotif PartyMemberConnectNotif;
	FPartyMemberDisconnectNotif PartyMemberDisconnectNotif;
	FPartyGenerateCodeResponse PartyGenerateCodeResponse;
	FPartyGetCodeResponse PartyGetCodeResponse;
	FPartyDeleteCodeResponse PartyDeleteCodeResponse;
	FPartyJoinViaCodeResponse PartyJoinViaCodeResponse;
	FPartyPromoteLeaderResponse PartyPromoteLeaderResponse;

    // Chat
    FPersonalChatResponse PersonalChatResponse;
    FPersonalChatNotif PersonalChatNotif;
    FPartyChatResponse PartyChatResponse;
    FPartyChatNotif PartyChatNotif;
	FJoinDefaultChannelChatResponse JoinDefaultChannelChatResponse;
	FChannelChatResponse ChannelChatResponse;
	FChannelChatNotif ChannelChatNotif;

    // Presence
    FSetUserPresenceResponse SetUserPresenceResponse;
    FFriendStatusNotif FriendStatusNotif;
    FGetAllFriendsStatusResponse GetAllFriendsStatusResponse;

    // Notification
	FMessageNotif MessageNotif;
	FUserBannedNotification UserBannedNotification;
	FUserUnbannedNotification UserUnbannedNotification;

    // Matchmaking
	FMatchmakingResponse MatchmakingStartResponse;
	FMatchmakingResponse MatchmakingCancelResponse;
	FReadyConsentResponse ReadyConsentResponse;
    FMatchmakingNotif MatchmakingNotif;
	FReadyConsentNotif ReadyConsentNotif;
	FRematchmakingNotif RematchmakingNotif;
	FDsNotif DsNotif;

	// Custom Game
	FBaseResponse CreateDSResponse;

	// Friends
	FRequestFriendsResponse RequestFriendsResponse;
	FRequestFriendsResponse RequestFriendsByPublicIdResponse;
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
	FUnfriendNotif UnfriendNotif;
	FCancelFriendsNotif CancelFriendsNotif;
	FRejectFriendsNotif RejectFriendsNotif;

	// Block
	FBlockPlayerResponse BlockPlayerResponse;
	FUnblockPlayerResponse UnblockPlayerResponse;
	FListBlockedUserResponse ListBlockedUserResponse;
	FListBlockerResponse ListBlockerResponse;

	// Block + Notification
	FBlockPlayerNotif BlockPlayerNotif;
	FUnblockPlayerNotif UnblockPlayerNotif;

	// Error
	FErrorNotif ErrorNotif;

	//Signaling P2P
	FSignalingP2P SignalingP2PNotif;

	//Session Attribute
	FSetSessionAttributeResponse SetSessionAttributeResponse;
	FGetSessionAttributeResponse GetSessionAttributeResponse;
	FGetAllSessionAttributeResponse GetAllSessionAttributeResponse;
	
	// Refresh Token
	FRefreshTokenResponse RefreshTokenResponse;
#pragma endregion


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

	// Error Handler
	FErrorHandler OnPartyInfoError;
	FErrorHandler OnPartyCreateError;
	FErrorHandler OnPartyLeaveError;
	FErrorHandler OnPartyInviteError;
	FErrorHandler OnPartyJoinError;
	FErrorHandler OnPartyRejectError;
	FErrorHandler OnPartyKickError;
	FErrorHandler OnPartyGenerateCodeError;
	FErrorHandler OnPartyGetCodeError;
	FErrorHandler OnPartyDeleteCodeError;
	FErrorHandler OnPartyJoinViaCodeError;
	FErrorHandler OnPartyPromoteLeaderError;
	FErrorHandler OnPersonalChatError;
	FErrorHandler OnPartyChatError;
	FErrorHandler OnJoinDefaultChannelChatError;
	FErrorHandler OnChannelChatError;
	FErrorHandler OnSetUserPresenceError;
	FErrorHandler OnGetAllFriendsStatusError;
	FErrorHandler OnMatchmakingStartError;
	FErrorHandler OnMatchmakingCancelError;
	FErrorHandler OnReadyConsentError;
	FErrorHandler OnRequestFriendsError;
	FErrorHandler OnRequestFriendsByPublicIdError;
	FErrorHandler OnUnfriendError;
	FErrorHandler OnListOutgoingFriendsError;
	FErrorHandler OnCancelFriendsError;
	FErrorHandler OnListIncomingFriendsError;
	FErrorHandler OnAcceptFriendsError;
	FErrorHandler OnRejectFriendsError;
	FErrorHandler OnLoadFriendListError;
	FErrorHandler OnGetFriendshipStatusError;
	FErrorHandler OnBlockPlayerError;
	FErrorHandler OnUnblockPlayerError;
	FErrorHandler OnSetSessionAttributeError;
	FErrorHandler OnGetSessionAttributeError;
	FErrorHandler OnGetAllSessionAttributeError;
	FErrorHandler OnRefreshTokenError;
	FErrorHandler OnCreateDSError;
};

} // Namespace Api
} // Namespace AccelByte
