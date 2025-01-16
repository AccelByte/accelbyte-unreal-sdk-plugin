// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteDefines.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/IAccelByteTokenGenerator.h"
#include "Core/AccelByteWebSocket.h"
#include "Models/AccelByteLobbyModels.h"
#include "Models/AccelByteMatchmakingModels.h"
#include "Models/AccelByteSessionModels.h"
#include "Core/AccelByteMessagingSystem.h"
#include "Core/AccelByteNetworkConditioner.h"
#include "Core/AccelByteNotificationBuffer.h"
#include "Core/AccelByteLockableQueue.h"

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

struct FHandleLobbyMessageData
{
	EHandleLobbyMessageDataType Type {EHandleLobbyMessageDataType::None};
	FString Topic; // for MM and Session
	FString Payload; // for MM and Session
	FString MessageType;
	FString ParsedJsonString;
	TSharedPtr<FJsonObject> ParsedJsonObject;
	bool bSkipConditioner;
};
	
enum Response : uint8;
enum Notif : uint8;

/**
 * @brief Lobby API for chatting and party management.
 * Unlike other servers which use HTTP, Lobby server uses WebSocket (RFC 6455).
 */
class ACCELBYTEUE4SDK_API Lobby 
	: public FApiBase
	, public IWebsocketConfigurableReconnectStrategy
	, public TSharedFromThis<Lobby, ESPMode::ThreadSafe>
{
public:
	Lobby(Credentials& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, FAccelByteMessagingSystem& InMessagingSystemRef
		, FAccelByteNetworkConditioner& InNetworkConditionerRef
		, float InPingDelay = 30.f
		, float InInitialBackoffDelay = 1.f
		, float InMaxBackoffDelay = 30.f
		, float InTotalTimeout = 60.f);
	~Lobby();
private:
	FCredentialsRef LobbyCredentialsRef;
	FAccelByteMessagingSystemWPtr MessagingSystemWPtr;
	FAccelByteNetworkConditioner& NetworkConditioner;

	const FString LobbyPlatformIdHeaderName = "X-Ab-Platform";
	const FString LobbyPlatformUserIdHeaderName = "X-Ab-Platform-User-Id";
	const FString LobbySessionHeaderName = "X-Ab-LobbySessionID";
	const FString LobbyEnvelopeStartHeaderName = "X-Ab-EnvelopeStart";
	const FString LobbyEnvelopeEndHeaderName = "X-Ab-EnvelopeEnd";
	
	const FString LobbyEnvelopeStartHeaderValue = LobbyMessageEnvelopeStartContent;
	const FString LobbyEnvelopeEndHeaderValue = LobbyMessageEnvelopeEndContent;
	FString EnvelopeContentBuffer;

	bool BanNotifReceived = false;
	EBanType BanType = EBanType::EMPTY;
	FDelegateHandle AuthTokenSetDelegateHandle;
	
	FDelegateHandleAlias LobbyTickerHandle;

public:
	
	// Party 
	/**
	 * @brief delegate for handling info party response.
	 */
	DECLARE_DELEGATE_OneParam(FBaseResponse, FAccelByteModelsLobbyBaseResponse const&); 

	// Party 
	/**
	 * @brief delegate for handling info party response.
	 */
	DECLARE_DELEGATE_OneParam(FPartyInfoResponse, FAccelByteModelsInfoPartyResponse const&); 
	
	/**
	 * @brief delegate for handling create party response.
	 */
	DECLARE_DELEGATE_OneParam(FPartyCreateResponse, FAccelByteModelsCreatePartyResponse const&); 

	/**
	 * @brief delegate for handling leave party response.
	 */
	DECLARE_DELEGATE_OneParam(FPartyLeaveResponse, FAccelByteModelsLeavePartyResponse const&);

	/**
	 * @brief delegate for handling leave party notification. This delegate is DEPRECATED
	 */
	DECLARE_DELEGATE_OneParam(FPartyLeaveNotif, FAccelByteModelsLeavePartyNotice const&);                  // Passive

	/**
	 * @brief delegate for handling leave party notification.
	 */
	DECLARE_DELEGATE_OneParam(FPartyMemberLeaveNotif, FAccelByteModelsLeavePartyNotice const&);                  // Passive
	
	/**
	 * @brief delegate for handling invite party response.
	 */
	DECLARE_DELEGATE_OneParam(FPartyInviteResponse, FAccelByteModelsPartyInviteResponse const&); 

	/**
	 * @brief delegate for handling party invitation notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyInviteNotif, FAccelByteModelsInvitationNotice const&);       // Passive

	/**
	 * @brief delegate for handling get party invitation notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice const&);  // Passive

	/**
	 * @brief delegate for handling join party response
	 */
	DECLARE_DELEGATE_OneParam(FPartyJoinResponse, FAccelByteModelsPartyJoinResponse const&); 

	/**
	 * @brief delegate for handling join party notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyJoinNotif, FAccelByteModelsPartyJoinNotice const&);              // Passive

	/**
	 * @brief delegate for handling reject party response
	 */
	DECLARE_DELEGATE_OneParam(FPartyRejectResponse, FAccelByteModelsPartyRejectResponse const&); 

	/**
	 * @brief delegate for handling reject party notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyRejectNotif, FAccelByteModelsPartyRejectNotice const&);;              // Passive
	
	/**
	 * @brief delegate for handling member kicked from party event
	 */
	DECLARE_DELEGATE_OneParam(FPartyKickResponse, FAccelByteModelsKickPartyMemberResponse const&); 
	
	/**
	 * @brief delegate for handling member kicked from party event notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice const&);   // Passive

	/**
	 * @brief delegate for handling update data notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyDataUpdateNotif, FAccelByteModelsPartyDataNotif const&);
	
	/**
	 * @brief delegate for handling party connect notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyMemberConnectNotif, FAccelByteModelsPartyMemberConnectionNotice const&);

	/**
	 * @brief delegate for handling party disconnect notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyMemberDisconnectNotif, FAccelByteModelsPartyMemberConnectionNotice const&);

	/**
	 * @brief delegate for handling generate party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyGenerateCodeResponse, FAccelByteModelsPartyGenerateCodeResponse const&);

	/**
	 * @brief delegate for handling get party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyGetCodeResponse, FAccelByteModelsPartyGetCodeResponse const&);

	/**
	 * @brief delegate for handling delete party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyDeleteCodeResponse, FAccelByteModelsPartyDeleteCodeResponse const&);

	/**
	 * @brief delegate for handling join party via party code event
	 */
	DECLARE_DELEGATE_OneParam(FPartyJoinViaCodeResponse, FAccelByteModelsPartyJoinResponse const&);

	/**
	 * @brief delegate for handling promote party leader response
	 */
	DECLARE_DELEGATE_OneParam(FPartyPromoteLeaderResponse, FAccelByteModelsPartyPromoteLeaderResponse const&);

	/**
	 * @brief delegate for handling sending a notification to a party member
	 */
	DECLARE_DELEGATE_OneParam(FPartySendNotifResponse, FAccelByteModelsPartySendNotifResponse const&);

	/**
	 * @brief delegate for handling notification from a party member
	 */
	DECLARE_DELEGATE_OneParam(FPartyNotif, FAccelByteModelsPartyNotif const&);

	// Chat
	/**
	 * @brief delegate for handling private message response
	 */
	DECLARE_DELEGATE_OneParam(FPersonalChatResponse, FAccelByteModelsPersonalMessageResponse const&);    

	/**
	 * @brief delegate for handling private message event notification
	 */
	DECLARE_DELEGATE_OneParam(FPersonalChatNotif, FAccelByteModelsPersonalMessageNotice const&);         // Passive

	/**
	 * @brief delegate for handling party message event response
	 */
	DECLARE_DELEGATE_OneParam(FPartyChatResponse, FAccelByteModelsPartyMessageResponse const&);

	/**
	 * @brief delegate for handling party message event notification
	 */
	DECLARE_DELEGATE_OneParam(FPartyChatNotif, FAccelByteModelsPartyMessageNotice const&);              // Passive
	
	/**
	 * @brief delegate for handling join default channel message event response
	 */
	DECLARE_DELEGATE_OneParam(FJoinDefaultChannelChatResponse, FAccelByteModelsJoinDefaultChannelResponse const&);

	/**
	 * @brief delegate for handling channel message event response
	 */
	DECLARE_DELEGATE_OneParam(FChannelChatResponse, FAccelByteModelsChannelMessageResponse const&);

	/**
	 * @brief delegate for handling channel message event notification
	 */
	DECLARE_DELEGATE_OneParam(FChannelChatNotif, FAccelByteModelsChannelMessageNotice const&);

	// Presence
	/**
	 * @brief delegate for handling user change presence status
	 */
	DECLARE_DELEGATE_OneParam(FSetUserPresenceResponse, FAccelByteModelsSetOnlineUsersResponse const&);

	/**
	 * @brief delegate for handling other user change their presence status event
	 */
	DECLARE_DELEGATE_OneParam(FFriendStatusNotif, FAccelByteModelsUsersPresenceNotice const&);

	/**
	 * @brief delegate for handling get all user presence
	 */
	DECLARE_DELEGATE_OneParam(FGetAllFriendsStatusResponse, FAccelByteModelsGetOnlineUsersResponse const&);        
	
	// Notification
	/**
	 * @brief delegate for handling incoming notification
	 */
	DECLARE_DELEGATE_OneParam(FMessageNotif, FAccelByteModelsNotificationMessage const&); //Passive

	/*
	* @brief multicast delegate for handling incoming notification
	*/
	DECLARE_MULTICAST_DELEGATE_OneParam(FNotifBroadcaster, FAccelByteModelsNotificationMessage const&); //Passive

	/**
	 * @brief delegate for handling ban notification
	 */
	DECLARE_DELEGATE_OneParam(FUserBannedNotification, FAccelByteModelsUserBannedNotification const&); //Passive

	/**
	 * @brief delegate for handling unban notification
	 */
	DECLARE_DELEGATE_OneParam(FUserUnbannedNotification, FAccelByteModelsUserBannedNotification const&); //Passive

	// Matchmaking
	/**
	 * @brief delegate for handling matchmaking response
	 */
	DECLARE_DELEGATE_OneParam(FMatchmakingResponse, FAccelByteModelsMatchmakingResponse const&);

	/**
	 * @brief delegate for handling ready consent response
	 */
	DECLARE_DELEGATE_OneParam(FReadyConsentResponse, FAccelByteModelsReadyConsentRequest const&);

	/**
	 * @brief delegate for handling reject consent response
	 */
	DECLARE_DELEGATE_OneParam(FRejectConsentResponse, FAccelByteModelsRejectConsentRequest const&);
	
	/**
	 * @brief delegate for handling matchmaking notification
	 */
	DECLARE_DELEGATE_OneParam(FMatchmakingNotif, FAccelByteModelsMatchmakingNotice const&);

	/*
	 * @brief delegate for handling ready consent notification
	 */
	DECLARE_DELEGATE_OneParam(FReadyConsentNotif, FAccelByteModelsReadyConsentNotice const&);

	/**
	 * @brief delegate for handling reject consent notification
	 */
	DECLARE_DELEGATE_OneParam(FRejectConsentNotif, FAccelByteModelsRejectConsentNotice const&);

	/**
	 * @brief delegate for handling rematchmaking notification
	 */
	DECLARE_DELEGATE_OneParam(FRematchmakingNotif, FAccelByteModelsRematchmakingNotice const&);

	/**
	 * @brief delegate for handling DS notification
	 */
	DECLARE_DELEGATE_OneParam(FDsNotif, FAccelByteModelsDsNotice const&);

	// Friends
	/**
	 * @brief delegate for handling request friend response
	 */
	DECLARE_DELEGATE_OneParam(FRequestFriendsResponse, FAccelByteModelsRequestFriendsResponse const&);
	
	/**
	 * @brief delegate for handling unfriend response
	 */
	DECLARE_DELEGATE_OneParam(FUnfriendResponse, FAccelByteModelsUnfriendResponse const&);
	
	/**
	 * @brief delegate for handling list outgoing friends response
	 */
	DECLARE_DELEGATE_OneParam(FListOutgoingFriendsResponse, FAccelByteModelsListOutgoingFriendsResponse const&);

	/**
	 * @brief delegate for handling list outgoing friends response with added requiredAt timestamp
	 */
	DECLARE_DELEGATE_OneParam(FListOutgoingFriendsWithTimeResponse, FAccelByteModelsListOutgoingFriendsWithTimeResponse const&);
	
	/**
	 * @brief delegate for handling cancel friend response
	 */
	DECLARE_DELEGATE_OneParam(FCancelFriendsResponse, FAccelByteModelsCancelFriendsResponse const&);
	
	/**
	 * @brief delegate for handling list incoming friends response
	 */
	DECLARE_DELEGATE_OneParam(FListIncomingFriendsResponse, FAccelByteModelsListIncomingFriendsResponse const&);

	/**
	 * @brief delegate for handling list incoming friends response with added requiredAt timestamp
	 */
	DECLARE_DELEGATE_OneParam(FListIncomingFriendsWithTimeResponse, FAccelByteModelsListIncomingFriendsWithTimeResponse const&);
	
	/**
	 * @brief delegate for handling accept friend response
	 */
	DECLARE_DELEGATE_OneParam(FAcceptFriendsResponse, FAccelByteModelsAcceptFriendsResponse const&);
	
	/**
	 * @brief delegate for handling reject friend response
	 */
	DECLARE_DELEGATE_OneParam(FRejectFriendsResponse, FAccelByteModelsRejectFriendsResponse const&);
	
	/**
	 * @brief delegate for handling load friend list response
	 */
	DECLARE_DELEGATE_OneParam(FLoadFriendListResponse, FAccelByteModelsLoadFriendListResponse const&);

	/**
	 * @brief delegate for handling get friendship status response
	 */
	DECLARE_DELEGATE_OneParam(FGetFriendshipStatusResponse, FAccelByteModelsGetFriendshipStatusResponse const&);

	// Friends + Notification
	/**
	 * @brief delegate for handling notification when your request friend is accepted
	 */
	DECLARE_DELEGATE_OneParam(FAcceptFriendsNotif, FAccelByteModelsAcceptFriendsNotif const&);

	/**
	 * @brief delegate for handling notification when you receive a request friend
	 */
	DECLARE_DELEGATE_OneParam(FRequestFriendsNotif, FAccelByteModelsRequestFriendsNotif const&);

	/**
	 * @brief delegate for handling unfriend notification
	 */
	DECLARE_DELEGATE_OneParam(FUnfriendNotif, FAccelByteModelsUnfriendNotif const&);
	
	/**
	 * @brief delegate for handling cancel friend notification
	 */
	DECLARE_DELEGATE_OneParam(FCancelFriendsNotif, FAccelByteModelsCancelFriendsNotif const&);

	/**
	 * @brief delegate for handling reject friend notification
	 */
	DECLARE_DELEGATE_OneParam(FRejectFriendsNotif, FAccelByteModelsRejectFriendsNotif const&);
	
	//Block
	/**
	 * @brief delegate for handling block player response
	 */
	DECLARE_DELEGATE_OneParam(FBlockPlayerResponse, FAccelByteModelsBlockPlayerResponse const&);

	/**
	 * @brief delegate for handling unblock player response
	 */
	DECLARE_DELEGATE_OneParam(FUnblockPlayerResponse, FAccelByteModelsUnblockPlayerResponse const&);
	
	/**
	 * @brief delegate for handling list blocked user response
	 */
	DECLARE_DELEGATE_OneParam(FListBlockedUserResponse, FAccelByteModelsListBlockedUserResponse const&);

	/**
	 * @brief delegate for handling list blocker response
	 */
	DECLARE_DELEGATE_OneParam(FListBlockerResponse, FAccelByteModelsListBlockerResponse const&);

	// Block + Notif
	/**
	 * @brief delegate for handling notification when you block a player
	 */
	DECLARE_DELEGATE_OneParam(FBlockPlayerNotif, FAccelByteModelsBlockPlayerNotif const&);

	/**
	 * @brief delegate for handling notification when you unblock a player
	 */
	DECLARE_DELEGATE_OneParam(FUnblockPlayerNotif, FAccelByteModelsUnblockPlayerNotif const&);

	//Signaling
	/**
	 * @brief delegate for handling signaling P2P message
	 */
	DECLARE_DELEGATE_TwoParams(FSignalingP2P, FString const&, FString const&);

	//Error
	/**
	 * @brief delegate for handling signaling P2P message
	 */
	DECLARE_DELEGATE_TwoParams(FErrorNotif, int32, FString const&);				//passive
	
	/**
	 * @brief delegate for handling response when setting session attribute
	 */
	DECLARE_DELEGATE_OneParam(FSetSessionAttributeResponse, FAccelByteModelsSetSessionAttributesResponse const&);

	/**
	 * @brief delegate for handling response when setting session attribute
	 */
	DECLARE_DELEGATE_OneParam(FGetSessionAttributeResponse, FAccelByteModelsGetSessionAttributesResponse const&);

	/**
	 * @brief delegate for handling response when setting session attribute
	 */
	DECLARE_DELEGATE_OneParam(FGetAllSessionAttributeResponse, FAccelByteModelsGetAllSessionAttributesResponse const&);

	/**
	 * @brief delegate for handling response when refreshing lobby token.
	 */
	DECLARE_DELEGATE_OneParam(FRefreshTokenResponse, FAccelByteModelsRefreshTokenResponse const&);

	/**
	* @brief delegate for handling successfully connecting to lobby.
	*/
	DECLARE_DELEGATE(FConnectSuccess);

	/**
	* @brief delegate for handling lobby connection disconnected.
	*/
	typedef TDelegate<void(FAccelByteModelsDisconnectNotif const&)> FDisconnectNotif;

	typedef AccelByteWebSocket::FConnectionCloseDelegate FConnectionClosed;
	typedef AccelByteWebSocket::FReconnectAttemptMulticastDelegate FReconnectAttempted;
	typedef AccelByteWebSocket::FMassiveOutageMulticastDelegate FMassiveOutage;

	/**
	 * @brief Delegate for party members changed event.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyMembersChangedNotif, FAccelByteModelsV2PartyMembersChangedEvent);

	/**
	 * @brief Delegate for user invited to party event.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyInvitedNotif, FAccelByteModelsV2PartyInvitedEvent);
	
	/**
	 * @brief Delegate for user party invitation timed out event.
	 * Notification when a party invitation has been timeout.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyInviteTimeoutNotif, FAccelByteModelsV2PartyInviteTimeoutEvent);

	/**
	 * @brief Delegate for user rejected invitation event.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyRejectedNotif, FAccelByteModelsV2PartyUserRejectedEvent);

	/**
	 * @brief Delegate for user joined party event.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyJoinedNotif, FAccelByteModelsV2PartyUserJoinedEvent);

	/**
	 * @brief Delegate for user kicked from party event.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyKickedNotif, FAccelByteModelsV2PartyUserKickedEvent);

	/**
	 * @brief Delegate for party invite canceled event. Will be triggered when any outgoing invitation canceled. Inviter and Invitee will receive it.
	 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FV2PartyInviteCanceledNotif, FAccelByteModelsV2PartyInviteCanceledEvent);

	/*
	 * @brief Delegate for party updated event.
	 */
	DECLARE_DELEGATE_OneParam(FV2PartyUpdatedNotif, FAccelByteModelsV2PartySession);

	/*
	* @brief Delegate for party created event.
	*/
	DECLARE_MULTICAST_DELEGATE_OneParam(FV2PartyCreatedNotif, FAccelByteModelsV2PartyCreatedEvent);

	/**
	 * @brief Delegate for user invited to game session event.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionInvitedNotif, FAccelByteModelsV2GameSessionUserInvitedEvent);

	/**
	 * @brief Delegate for user game session invitation timed out event.
	 * Notification when a game session invitation has been timeout.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionInviteTimeoutNotif, FAccelByteModelsV2GameSessionUserInviteTimeoutEvent);

	/**
	 * @brief Delegate for user joined game session event.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionJoinedNotif, FAccelByteModelsV2GameSessionUserJoinedEvent);

	/**
	 * @brief Delegate for game session members changed event.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionMembersChangedNotif, FAccelByteModelsV2GameSessionMembersChangedEvent);

	/**
	 * @brief Delegate for game session updated event.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionUpdatedNotif, FAccelByteModelsV2GameSession);
	
	/**
	 * @brief Delegate for game session updated event.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionKickedNotif, FAccelByteModelsV2GameSessionUserKickedEvent);

	/**
	 * @brief Delegate for game session invite rejected event.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionRejectedNotif, FAccelByteModelsV2GameSessionUserRejectedEvent);

	/**
	 * @brief Delegate for game session ended event. Will be triggered when DS unregistered.
	 */
	DECLARE_DELEGATE_OneParam(FV2GameSessionEndedNotif, FAccelByteModelsV2GameSessionEndedEvent);

	/**
	 * @brief Delegate for game session invite canceled event. Will be triggered when any outgoing invitation canceled. Inviter and Invitee will receive it.
	 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FV2GameSessionInviteCanceledNotif, FAccelByteModelsV2GameSessionInviteCanceledEvent);

	/**
	 * @brief Delegate for session storage changed event.
	 */
	DECLARE_DELEGATE_OneParam(FV2SessionStorageChangedNotif, FAccelByteModelsV2SessionStorageChangedEvent);

	/**
	 * @brief Delegate for session joined secret event.
	 */
	DECLARE_DELEGATE_OneParam(FV2SessionJoinedSecretNotif, FAccelByteModelsV2SessionJoinedSecret const&);

	/**
	 * @brief Delegate for game session when DS status is changed.
	 */
	DECLARE_DELEGATE_OneParam(FV2DSStatusChangedNotif, FAccelByteModelsV2DSStatusChangedNotif);

	/**
	 * @brief Delegate for notif when match is found.
	 */
	DECLARE_DELEGATE_OneParam(FV2MatchmakingMatchFoundNotif, FAccelByteModelsV2MatchFoundNotif);

	/**
	 * @brief Delegate for notification when party leader started matchmaking
	 */
	DECLARE_DELEGATE_OneParam(FV2MatchmakingStartNotif, FAccelByteModelsV2StartMatchmakingNotif);

	/**
	* @brief Delegate for notification when matchmaking ticket expired
	*/
	DECLARE_DELEGATE_OneParam(FV2MatchmakingExpiredNotif, FAccelByteModelsV2MatchmakingExpiredNotif);

	/**
	 * @brief Delegate for notification when party leader canceled matchmaking
	 */
	DECLARE_DELEGATE_OneParam(FV2MatchmakingCanceledNotif, FAccelByteModelsV2MatchmakingCanceledNotif);

	// Metrics
	/**
	 * @brief delegate for handling set user region response
	 */
	DECLARE_DELEGATE_OneParam(FChangeUserRegionResponse, FAccelByteModelsChangeUserRegionResponse const&);

	/**
	 * @brief Delegate for a native session sync event from session service
	 */
	DECLARE_DELEGATE_OneParam(FV2NativeSessionSyncNotif, FAccelByteModelsV2NativeSessionSyncNotif);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOneTimeCodeLinkedNotif, FAccelByteModelsOneTimeCodeLinked const&);

public:
	/**
	 * @brief Connect to the Lobby server via websocket. You must connect to the server before you can start sending/receiving. Also make sure you have logged in first as this operation requires access token.
	 */
	void Connect(FString const& Token = "");

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
	FString SendPrivateMessage(FString const& UserId, FString const& Message);

	/**
	 * @brief Send a message to other party members.
	 * 
	 * @param Message Message to be sent.
	 */
	FString SendPartyMessage(FString const& Message);
	
	/**
	 * @brief Request to join the default channel chat.
	 */
	FString SendJoinDefaultChannelChatRequest();

	/**
	 * @brief Send a message to joined channel chat.
	 */
	FString SendChannelMessage(FString const& Message);

	//------------------------
	// Party
	//------------------------

	/**
	 * @brief Get information about current party.
	 */
	FString SendInfoPartyRequest();
	FString SendInfoPartyRequest(FPartyInfoResponse const& OnInfoPartyResponse
		, FErrorHandler const& OnError = nullptr);

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
	FString SendInviteToPartyRequest(FString const& UserId);

	/**
	 * @brief Set presence status on lobby service
	 *
	 * @param Availability Presence state that you want to use. State is EAvailability type
	 * @param Activity User's custom activity. It will be escaped and unescaped automatically, can handle JSON.
	 */
	FString SendSetPresenceStatus(EAvailability Availability, FString const& Activity);

	/**
	 * @brief Accept a party invitation.
	 * 
	 * @param PartyId Party ID from the invitation notice.
	 * @param InvitationToken Random string from the invitation notice.
	 */
	FString SendAcceptInvitationRequest(FString const& PartyId, FString const& InvitationToken);
	
	/**
	 * @brief Reject a party invitation.
	 * 
	 * @param PartyId Party ID from the invitation notice.
	 * @param InvitationToken Random string from the invitation notice.
	 */
	FString SendRejectInvitationRequest(FString const& PartyId, FString const& InvitationToken);
	
	/**
	 * @brief Kick a party member.
	 * 
	 * @param UserId The target user ID to be kicked.
	 */
	FString SendKickPartyMemberRequest(FString const& UserId);
	
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
	FString SendPartyJoinViaCodeRequest(FString const& partyCode);

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
	FString SendPartyPromoteLeaderRequest(FString const& UserId);

	/**
	 * @brief Send notification to party member 
	 *
	 * @param Topic The topic of the request. Can use this as ID to know how to marshal the payload
	 * @param Payload The Payload of the request. Can be JSON string
	 */
	FString SendNotificationToPartyMember(FString const& Topic, FString const& Payload);

	/**
	 * @brief Set the party's member limit.
	 * 
	 * @param PartyId The party id to be set.
	 * @param Limit The party member limit, should be above 0 and below the configured limit in admin portal.
	 * @param OnSuccess The callback triggered when operation is successful
	 * @param OnError the callback triggered when operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SetPartySizeLimit(FString const& PartyId
		, int32 Limit
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	//------------------------
	// Notification
	//------------------------
	/**
	 * @brief Get all pending notification(s) that is sent to user when user is not connected to lobby. Please call this function after user connected to lobby.
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
	FString SendStartMatchmaking(FString const& GameMode
		, FString const& ServerName
		, FString const& ClientVersion = TEXT("")
		, TArray<TPair<FString, float>> const& Latencies = {}
		, TMap<FString, FString> const& PartyAttributes = {}
		, TArray<FString> const& TempPartyUserIds = {}
		, TArray<FString> const& ExtraAttributes = {});

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
	FString SendStartMatchmaking(FString const& GameMode
		, TArray<FString> const& TempPartyUserIds
		, FString const& ServerName = TEXT("")
		, FString const& ClientVersion = TEXT("")
		, TArray<TPair<FString, float>> const& Latencies = {}
		, TMap<FString, FString> const& PartyAttributes = {}
		, TArray<FString> const& ExtraAttributes = {});

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
	FString SendStartMatchmaking(FString const& GameMode
		, TMap<FString, FString> const& PartyAttributes
		, FString const& ServerName = TEXT("")
		, FString const& ClientVersion = TEXT("")
		, TArray<TPair<FString, float>> const& Latencies = {}
		, TArray<FString> const& TempPartyUserIds = {}
		, TArray<FString> const& ExtraAttributes = {});

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
	FString SendStartMatchmaking(FString const& GameMode
		, TMap<FString, FString> const& PartyAttributes
		, TArray<FString> const& TempPartyUserIds
		, FString const& ServerName = TEXT("")
		, FString const& ClientVersion = TEXT("")
		, TArray<TPair<FString, float>> const& Latencies = {}
		, TArray<FString> const& ExtraAttributes = {});

	/**
	 * @brief Serialize the Qos Latencies obj into a stringified json object.
	 * - Multiple regions are possible (hence the TArray).
	 * @param SelectedLatencies Array of latencies pair, key is region name and value is latency in ms.
	 * @return 
	 */
	static FString GetServerLatenciesJsonStr(TArray<TPair<FString, float>> const& SelectedLatencies);

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
	FString SendStartMatchmaking(FString const& GameMode
		, FMatchmakingOptionalParams const& OptionalParams = {});

	/**
	 * @brief cancel the currently running matchmaking process
	 *
	 * @param GameMode The mode that party member want to cancel.
	 * @param IsTempParty Is canceling matchmaking that was started using temporary party.
	 */
	FString SendCancelMatchmaking(FString const& GameMode
		, bool IsTempParty = false);

	/**
	 * @brief send ready consent request
	 *
	 * @param MatchId The id of a match user ready to play.
	 */
	FString SendReadyConsentRequest(FString const& MatchId);

	/**
	 * @brief send reject consent request
	 *
	 * @param MatchId The id of a match user ready to play.
	 */
	FString SendRejectConsentRequest(FString const& MatchId);

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
	FString RequestDS(FString const& SessionID
		, FString const& GameMode
		, FString const& ClientVersion
		, FString const& Region
		, FString const& Deployment
		, FString const& ServerName = TEXT(""));

	// Friends
	/**
	 * @brief Query the friend list of current user. The result is friend ids in pagination format.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the result. Default value is 0.
	 * @param Limit The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryFriendList(THandler<FAccelByteModelsQueryFriendListResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Offset = 0
		, int32 Limit = 20);

	/**
	 * @brief Query the incoming friend requests for current user. The result is friend requests with requested time info in pagination format.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the result. Default value is 0.
	 * @param Limit The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryIncomingFriendRequest(THandler<FAccelByteModelsIncomingFriendRequests> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Offset = 0
		, int32 Limit = 20);

	/**
	 * @brief Query the outgoing friend requests for current user. The result is friend requests with requested time info in pagination format.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the result. Default value is 0.
	 * @param Limit The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryOutgoingFriendRequest(THandler<FAccelByteModelsOutgoingFriendRequests> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Offset = 0
		, int32 Limit = 20);

	/**
	 * @brief Send friend request to other user by user id.
	 *
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendFriendRequest(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send friend request to other user by public id.
	 *
	 * @param PublicId Targeted public ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendFriendRequestByPublicId(FString const& PublicId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Cancel outgoing friend request.
	 *
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelFriendRequest(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Accept friend request from other user.
	 *
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr AcceptFriendRequest(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Reject friend request from other user
	 *
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RejectFriendRequest(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get friendship status of current user with other user.
	 *
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetFriendshipStatus(FString const& UserId
		, THandler<FAccelByteModelsFriendshipStatusResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Unfriend a user from friend list.
	 *
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr Unfriend(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

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
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BlockPlayer(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

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
	 * @param UserId Targeted user ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UnblockPlayer(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send request friend request.
	 *
	 * @param UserId Targeted user ID.
	 */
	void RequestFriend(FString const& UserId);
	
	/**
	* @brief Send friend request by public ID.
	*
	* @param PublicId Targeted user's PublicId.
	*/
	void RequestFriendByPublicId(FString const& PublicId);

	/**
	 * @brief Send unfriend request.
	 *
	 * @param UserId Targeted user ID.
	 */
	void Unfriend(FString const& UserId);

	/**
	 * @brief Send list of outgoing friends request.
	 */
	void ListOutgoingFriends();

	/**
	 * @brief Send list of outgoing friends request with requestedAt timestamp.
	 */
	void ListOutgoingFriendsWithTime();

	/**
	 * @brief Send cancel friend request.
	 *
	 * @param UserId Targeted user ID.
	 */
	void CancelFriendRequest(FString const& UserId);

	/**
	 * @brief Send list of incoming friends request.
	 */
	void ListIncomingFriends();

	/**
	 * @brief Send list of incoming friends request with requestedAt timestamp.
	 */
	void ListIncomingFriendsWithTime();

	/**
	 * @brief Send accept friend request.
	 */
	void AcceptFriend(FString const& UserId);

	/**
	 * @brief Send reject friend request.
	 */
	void RejectFriend(FString const& UserId);

	/**
	 * @brief Send load friends list request.
	 */
	void LoadFriendsList();

	/**
	 * @brief Send get friendship status request.
	 *
	 * @param UserId Targeted user ID.
	 */
	void GetFriendshipStatus(FString const& UserId);

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
	 void BlockPlayer(FString const& UserId);

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
	  * @param UserId the specified player's user ID. (Target to block)
	  *
	  */
	 void UnblockPlayer(FString const& UserId);

	/**
	 * @brief Send a signaling message to another user.
	 *
	 * @param UserId The recipient's user ID.
	 * @param Message Signaling message to be sent.
	 */
	FString SendSignalingMessage(FString const& UserId
		, FString const& Message);

	/**
	 * @brief Set user attribute to lobby session. 
	 *
	 * @param Key the attribute's key.
	 * @param Value the attribute's value.
	 */
	FString SetSessionAttribute(FString const& Key
		, FString const& Value);

	/**
	 * @brief Get user attribute from lobby session.
	 *
	 * @param Key the attribute's key.
	 */
	FString GetSessionAttribute(FString const& Key);

	/**
	 * @brief Get all user attribute from lobby session.
	 */
	FString GetAllSessionAttribute();

	/**
	* @brief Refresh access token used in lobby.
	* 
	* @param AccessToken New access token to use.
	*/
	FString RefreshToken(FString const& AccessToken);

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

	/**
	 * @brief Unbind all V2 party delegates set previously.
	 */
	void UnbindV2PartyEvents();

	/**
	 * @brief Unbind all V2 party delegates set previously.
	 */
	void UnbindV2GameSessionEvents();

	/**
	 * @brief Unbind all shared notif for session and v2 party set previously.
	 */
	void UnbindV2SessionGeneralEvents();

	/**
	 * @brief Unbind all V2 matchmaking delegates set previously.
	 */
	void UnbindV2MatchmakingEvents();
	
	/**
	 * @brief Set a trigger function when successfully connect to lobby.
	 */
	void SetConnectSuccessDelegate(FConnectSuccess const& OnConnectSuccess)
	{
		ConnectSuccess = OnConnectSuccess;
	}

	/**
	 * @brief Set a trigger function when connection to lobby failed.
	 */
	void SetConnectFailedDelegate(FErrorHandler const& OnConnectError)
	{
		ConnectError = OnConnectError;
	}
	
	/**
	 * @brief Set a trigger function when connection to lobby is disconnected.
	 */
	void SetDisconnectNotifDelegate(FDisconnectNotif const& OnDisconnectNotice)
	{
		DisconnectNotif = OnDisconnectNotice;
	}

	/**
	 * @brief Set a trigger function when connection is closed.
	 */
	void SetConnectionClosedDelegate(FConnectionClosed const& OnConnectionClosed)
	{
		ConnectionClosed = OnConnectionClosed;
	}

	/**
	 * @brief Set a trigger function when connection is closed and reconnecting.
	 */
	void SetReconnectingDelegate(FConnectionClosed const& OnReconnecting)
	{
		Reconnecting = OnReconnecting;
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
	 * 
	 * @param OnPartyMemberLeaveNotice delegate parameter with returned model called FAccelByteModelsLeavePartyNotice
	 */
	void SetPartyMemberLeaveNotifDelegate(FPartyMemberLeaveNotif const& OnPartyMemberLeaveNotice)
	{
		PartyMemberLeaveNotif = OnPartyMemberLeaveNotice;
	}
	
	/**
	 * @brief Set a trigger function when a party member reconnecting to the lobby
	 * 
	 * @param OnPartyMemberConnectNotif return models called FAccelByteModelsPartyMemberConnectionNotice
	 */
	void SetPartyMemberConnectNotifDelegate(FPartyMemberConnectNotif const& OnPartyMemberConnectNotif)
	{
		PartyMemberConnectNotif = OnPartyMemberConnectNotif;
	}
	
	/**
	 * @brief Set a trigger function when a party member disconnect from lobby
	 * @param OnPartyMemberDisconnectNotif return models called FAccelByteModelsPartyMemberConnectionNotice
	 */
	void SetPartyMemberDisconnectNotifDelegate(FPartyMemberDisconnectNotif const& OnPartyMemberDisconnectNotif)
	{
		PartyMemberDisconnectNotif = OnPartyMemberDisconnectNotif;
	}

	/**
	 * @brief Set a trigger function when other user is invited to party
	 * @param OnPartyInviteNotif return models called FAccelByteModelsInvitationNotice
	 */
	void SetPartyInviteNotifDelegate(FPartyInviteNotif const& OnPartyInviteNotif)
	{
		PartyInviteNotif = OnPartyInviteNotif;
	}
	
	/**
	 * @brief Set a trigger function when receiving a party invite
	 * @param OnInvitePartyGetInvitedNotice return models called FAccelByteModelsPartyGetInvitedNotice
	 */
	void SetPartyGetInvitedNotifDelegate(FPartyGetInvitedNotif const& OnInvitePartyGetInvitedNotice)
	{
		PartyGetInvitedNotif = OnInvitePartyGetInvitedNotice;
	}
	
	/**
	 * @brief Set a trigger function when a new member joins the party
	 * @param OnInvitePartyJoinNotice return models called FAccelByteModelsPartyJoinNotice
	 */
	void SetPartyJoinNotifDelegate(FPartyJoinNotif const& OnInvitePartyJoinNotice)
	{
		PartyJoinNotif = OnInvitePartyJoinNotice;
	}

	/**
	 * @brief Set a trigger function when a party invitation is rejected
	 * @param OnInvitePartyRejectNotice return models called FAccelByteModelsPartyRejectNotice
	 */
	void SetPartyInvitationRejectedNotifDelegate(FPartyRejectNotif const& OnInvitePartyRejectNotice)
	{
		PartyRejectNotif = OnInvitePartyRejectNotice;
	}

	/**
	 * @brief Set a trigger function when kicked from party
	 * @param OnInvitePartyKickedNotice return models called FAccelByteModelsGotKickedFromPartyNotice
	 */
	void SetPartyKickNotifDelegate(FPartyKickNotif const& OnInvitePartyKickedNotice)
	{
		PartyKickNotif = OnInvitePartyKickedNotice;
	}

	/**
	 * @brief Set a trigger function when party data is updated
	 * @param OnPartyNotif return models called FAccelByteModelsPartyNotif
	 */
	void SetPartyNotifDelegate(FPartyNotif const& OnPartyNotif)
	{
		PartyNotif = OnPartyNotif;
	}

	/**
	 * @brief Set a trigger function when party data is updated
	 * @param OnPersonalChatNotif return models called FAccelByteModelsPartyNotif
	 */
	void SetPrivateMessageNotifDelegate(FPersonalChatNotif const& OnPersonalChatNotif)
	{
		PersonalChatNotif = OnPersonalChatNotif;
	};

	/**
	 * @brief Set a trigger function when received a party chat (v1)
	 * @param OnPersonalChatNotif return models called FAccelByteModelsPartyMessageNotice
	 */
	void SetPartyChatNotifDelegate(FPartyChatNotif const& OnPersonalChatNotif)
	{
		PartyChatNotif = OnPersonalChatNotif;
	}

	/**
	 * @brief Set a trigger function when friends presence is updated
	 * @param OnUserPresenceNotif return models called FAccelByteModelsUsersPresenceNotice
	 */
	void SetUserPresenceNotifDelegate(FFriendStatusNotif const& OnUserPresenceNotif)
	{
		FriendStatusNotif = OnUserPresenceNotif;
	};

	/**
	 * @brief Set a trigger function when receiving a notification message
	 * @param OnNotificationMessage return models called FAccelByteModelsNotificationMessage
	 */
	void SetMessageNotifDelegate(FMessageNotif const& OnNotificationMessage)
	{
		if (RemoveMessageNotifBroadcasterDelegate(NotificationMessageDelegateHandle))
		{
			NotificationMessageDelegateHandle.Reset();
		}

		NotificationMessageDelegateHandle = MessageNotifBroadcaster.AddLambda(
			[OnNotificationMessage](FAccelByteModelsNotificationMessage const& Message)
			{
				OnNotificationMessage.ExecuteIfBound(Message);
			});
	}

	/**
	 * @brief Set a trigger function when receiving a notification message
	 * @param OnNotificationBroadcasterMessage return models called FAccelByteModelsNotificationMessage
	 */
	FDelegateHandle AddMessageNotifBroadcasterDelegate(FNotifBroadcaster const& OnNotificationBroadcasterMessage)
	{
		return MessageNotifBroadcaster.AddLambda([OnNotificationBroadcasterMessage](FAccelByteModelsNotificationMessage const& Message)
			{
				OnNotificationBroadcasterMessage.Broadcast(Message);
			});
	}

	/**
	 * @brief Remove a trigger function when receiving a notification message
	 * @param OnNotificationBroadcasterDelegate Delegate handle to be removed
	 */
	bool RemoveMessageNotifBroadcasterDelegate(FDelegateHandle const& OnNotificationBroadcasterDelegate)
	{
		return MessageNotifBroadcaster.Remove(OnNotificationBroadcasterDelegate);
	}

	/**
	 * @brief Set a trigger function when receiving invite to a party (v2)
	 * @param OnPartyInvitedNotif return models called FAccelByteModelsV2PartyInvitedEvent
	 */
	void SetV2PartyInvitedNotifDelegate(FV2PartyInvitedNotif const& OnPartyInvitedNotif)
	{
		V2PartyInvitedNotif = OnPartyInvitedNotif;
	}

	/**
	 * @brief Set a trigger function when an party (v2) invitation is already timeout
	 * @param OnPartyInviteTimeoutNotif return models called FV2PartyInviteTimeoutNotif
	 */
	void SetV2PartyInviteTimeoutNotifDelegate(FV2PartyInviteTimeoutNotif const& OnPartyInviteTimeoutNotif)
	{
		V2PartyInviteTimeoutNotif = OnPartyInviteTimeoutNotif;
	}

	/**
	 * @brief Set a trigger function when party members changed (v2)
	 * @param OnPartyMembersChanged return models called FAccelByteModelsV2PartyMembersChangedEvent
	 */
	void SetV2PartyMembersChangedNotifDelegate(FV2PartyMembersChangedNotif const& OnPartyMembersChanged)
	{
		V2PartyMembersChangedNotif = OnPartyMembersChanged;
	}

	/**
	 * @brief Set a trigger function when a new member joined current party (v2)
	 * @param OnPartyJoinedNotif return models called FAccelByteModelsV2PartyUserJoinedEvent
	 */
	void SetV2PartyJoinedNotifDelegate(FV2PartyJoinedNotif const& OnPartyJoinedNotif)
	{
		V2PartyJoinedNotif = OnPartyJoinedNotif;
	}

	/**
	 * @brief Set a trigger function when a party invitation is rejected (v2)
	 * @param OnPartyRejectedNotif return models called FAccelByteModelsV2PartyUserRejectedEvent
	 */
	void SetV2PartyRejectedNotifDelegate(FV2PartyRejectedNotif const& OnPartyRejectedNotif)
	{
		V2PartyRejectedNotif = OnPartyRejectedNotif;
	}

	/**
	 * @brief Set a trigger function when getting kicked from a party (v2)
	 * @param OnPartyKickedNotif return models called FAccelByteModelsV2PartyUserKickedEvent
	 */
	void SetV2PartyKickedNotifDelegate(FV2PartyKickedNotif const& OnPartyKickedNotif)
	{
		V2PartyKickedNotif = OnPartyKickedNotif;
	}

	/**
	 * @brief Set a trigger function when receiving invite to a game session (v2)
	 * @param OnGameSessionInvitedNotif return models called FAccelByteModelsV2GameSessionUserInvitedEvent
	 */
	void SetV2GameSessionInvitedNotifDelegate(FV2GameSessionInvitedNotif const& OnGameSessionInvitedNotif)
	{
		V2GameSessionInvitedNotif = OnGameSessionInvitedNotif;
	}

	/**
	 * @brief Set a trigger function when an game session (v2) invitation is already timeout 
	 * @param OnGameSessionInviteTimeoutNotif return models called FV2GameSessionInviteTimeoutNotif
	 */
	void SetV2GameSessionInviteTimeoutNotifDelegate(FV2GameSessionInviteTimeoutNotif const& OnGameSessionInviteTimeoutNotif)
	{
		V2GameSessionInviteTimeoutNotif = OnGameSessionInviteTimeoutNotif;
	}

	/**
	 * @brief Set a trigger function when a new member joined the game session (v2)
	 * @param OnGameSessionJoinedNotif return models called FAccelByteModelsV2GameSessionUserJoinedEvent
	 */
	void SetV2GameSessionJoinedNotifDelegate(FV2GameSessionJoinedNotif const& OnGameSessionJoinedNotif)
	{
		V2GameSessionJoinedNotif = OnGameSessionJoinedNotif;
	}

	/**
	 * @brief Set a trigger function when a game session member changes (v2)
	 * @param OnGameSessionMembersChangedNotif return models called FAccelByteModelsV2GameSessionMembersChangedEvent
	 */
	void SetV2GameSessionMembersChangedNotifDelegate(FV2GameSessionMembersChangedNotif const& OnGameSessionMembersChangedNotif)
	{
		V2GameSessionMembersChangedNotif = OnGameSessionMembersChangedNotif;
	}

	/**
	 * @brief Set a trigger function when a game session data changes (v2)
	 * @param OnGameSessionUpdatedNotif return models called FAccelByteModelsV2GameSession
	 */
	void SetV2GameSessionUpdatedNotifDelegate(FV2GameSessionUpdatedNotif const& OnGameSessionUpdatedNotif)
	{
		V2GameSessionUpdatedNotif = OnGameSessionUpdatedNotif;
	}

	/**
	 * @brief Set a trigger function when kicked from a game session (v2)
	 * @param OnGameSessionKickedNotif return models called FAccelByteModelsV2GameSessionUserKickedEvent
	 */
	void SetV2GameSessionKickedNotifDelegate(FV2GameSessionKickedNotif const& OnGameSessionKickedNotif)
	{
		V2GameSessionKickedNotif = OnGameSessionKickedNotif;
	}

	/**
	 * @brief Set a trigger function when an invitation is rejected (v2)
	 * @param OnGameSessionRejectedNotif return models called FAccelByteModelsV2GameSessionUserRejectedEvent
	 */
	void SetV2GameSessionRejectedNotifDelegate(FV2GameSessionRejectedNotif const& OnGameSessionRejectedNotif)
	{
		V2GameSessionRejectedNotif = OnGameSessionRejectedNotif;
	}

	/**
	 * @brief Set a trigger function when game session ended by the DS (v2)
	 * @param OnGameSessionEndededNotif return models called FAccelByteModelsV2GameSessionEndedEvent
	 */
	void SetV2GameSessionEndedNotifDelegate(FV2GameSessionEndedNotif const& OnGameSessionEndededNotif)
	{
		V2GameSessionEndedNotif = OnGameSessionEndededNotif;
	}

	FDelegateHandle AddV2GameSessionInviteCanceledNotifDelegate(THandler<FAccelByteModelsV2GameSessionInviteCanceledEvent> const& OnGameSessionInviteCanceled)
	{
		return V2GameSessionInviteCanceledNotif.AddLambda([OnGameSessionInviteCanceled](const FAccelByteModelsV2GameSessionInviteCanceledEvent& Notif)
		{
			OnGameSessionInviteCanceled.ExecuteIfBound(Notif);
		});
	}

	bool RemoveV2GameSessionInviteCanceledNotifDelegate(FDelegateHandle const& DelegateHandle)
	{
		return V2GameSessionInviteCanceledNotif.Remove(DelegateHandle);
	}


	FDelegateHandle AddV2PartyCreatedNotifDelegate(THandler<FAccelByteModelsV2PartyCreatedEvent> const& OnPartyCreated)
	{
		return V2PartyCreatedNotif.AddLambda([OnPartyCreated](const FAccelByteModelsV2PartyCreatedEvent& Notif)
		{
			OnPartyCreated.ExecuteIfBound(Notif);
		});
	}

	bool RemoveV2PartyCreatedNotifDelegate(FDelegateHandle const& DelegateHandle)
	{
		return V2PartyCreatedNotif.Remove(DelegateHandle);
	}

	FDelegateHandle AddV2PartyInviteCanceledNotifDelegate(THandler<FAccelByteModelsV2PartyInviteCanceledEvent> const& OnPartyInviteCanceled)
	{
		return V2PartyInviteCanceledNotif.AddLambda([OnPartyInviteCanceled](const FAccelByteModelsV2PartyInviteCanceledEvent& Notif)
		{
			OnPartyInviteCanceled.ExecuteIfBound(Notif);
		});
	}

	bool RemoveV2PartyInviteCanceledNotifDelegate(FDelegateHandle const& DelegateHandle)
	{
		return V2PartyInviteCanceledNotif.Remove(DelegateHandle);
	}

	/**
	 * @brief Set a trigger function when session storage content changes (v2)
	 * @param OnSessionStorageChangedNotif return models called FAccelByteModelsV2SessionStorageChangedEvent
	 */
	void SetV2SessionStorageChangedNotifDelegate(FV2SessionStorageChangedNotif const& OnSessionStorageChangedNotif)
	{
		V2SessionStorageChangedNotif = OnSessionStorageChangedNotif;
	}

	/**
    * @brief Set a trigger function when session (with secret server enabled) joined (v2)
    * @param OnSessionJoinedSecretNotif return models called FAccelByteModelsV2SessionJoinedSecret
    */
	void SetV2SessionJoinedSecretNotifDelegate(FV2SessionJoinedSecretNotif const& OnSessionJoinedSecretNotif)
	{
		V2SessionJoinedSecretNotif = OnSessionJoinedSecretNotif;
	}

	/**
	 * @brief Set a trigger function when a DS status changes (v2)
	 * @param OnDSStatusChangedNotif return models called FAccelByteModelsV2DSStatusChangedNotif
	 */
	void SetV2DSStatusChangedNotifDelegate(FV2DSStatusChangedNotif const& OnDSStatusChangedNotif)
	{
		V2DSStatusChangedNotif = OnDSStatusChangedNotif;
	}

	/**
	 * @brief Set a trigger function when a party data is updated (v2)
	 * @param OnPartyUpdatedNotif return models called FAccelByteModelsV2PartySession
	 */
	void SetV2PartyUpdatedNotifDelegate(FV2PartyUpdatedNotif const& OnPartyUpdatedNotif)
	{
		V2PartyUpdatedNotif = OnPartyUpdatedNotif;
	}

	/**
	 * @brief Set a trigger function when matchmaking ticket is successfully matched (v2)
	 * @param OnMatchFoundNotif return models called FAccelByteModelsV2MatchFoundNotif
	 */
	void SetV2MatchmakingMatchFoundNotifDelegate(FV2MatchmakingMatchFoundNotif const& OnMatchFoundNotif)
	{
		V2MatchmakingMatchFoundNotif = OnMatchFoundNotif;
	}

	/**
	 * @brief Set a trigger function when leader created a matchmaking ticket (v2)
	 * @param OnMatchmakingStartNotif return models called FAccelByteModelsV2StartMatchmakingNotif
	 */
	void SetV2MatchmakingStartNotifDelegate(FV2MatchmakingStartNotif const& OnMatchmakingStartNotif)
	{
		V2MatchmakingStartNotif = OnMatchmakingStartNotif;
	}

	/**
	 * @brief Set a trigger function when matchmaking ticket is expired and matchmaking stopped (v2)
	 * @param OnMatchmakingExpiredNotif return models called FAccelByteModelsV2MatchmakingExpiredNotif
	 */
	void SetV2MatchmakingExpiredNotifDelegate(FV2MatchmakingExpiredNotif const& OnMatchmakingExpiredNotif)
	{
		V2MatchmakingExpiredNotif = OnMatchmakingExpiredNotif;
	}

	/**
	 * @brief Set a trigger function when leader cancelled matchmaking (v2)
	 * @param OnMatchmakingCanceledNotif return models called FAccelByteModelsV2MatchmakingCanceledNotif
	 */
	void SetV2MatchmakingCanceledNotifDelegate(FV2MatchmakingCanceledNotif const& OnMatchmakingCanceledNotif)
	{
		V2MatchmakingCanceledNotif = OnMatchmakingCanceledNotif;
	}

	/**
	 * @brief Set a trigger function when leader cancelled matchmaking (v2)
	 * @param OnMatchmakingCanceledNotif return models called FAccelByteModelsV2MatchmakingCanceledNotif
	 */
	void SetV2NativeSessionSyncNotifDelegate(const FV2NativeSessionSyncNotif& OnV2NativeSessionSyncNotif)
	{
		V2NativeSessionSyncNotif = OnV2NativeSessionSyncNotif;
	}

	/**
	 * @brief Set a trigger function when user is banned
	 * @param OnUserBannedNotification return models called FAccelByteModelsUserBannedNotification
	 */
	void SetUserBannedNotificationDelegate(FUserBannedNotification const& OnUserBannedNotification)
	{
		UserBannedNotification = OnUserBannedNotification;
	}

	/**
	 * @brief Set a trigger function when user is unbanned
	 * @param OnUserUnbannedNotification return models called FAccelByteModelsUserBannedNotification
	 */
	void SetUserUnbannedNotificationDelegate(FUserUnbannedNotification const& OnUserUnbannedNotification)
	{
		UserUnbannedNotification = OnUserUnbannedNotification;
	}

	/**
	 * @brief Set a trigger function when a friend request is accepted
	 * @param OnAcceptFriendsNotif return models called FAccelByteModelsAcceptFriendsNotif
	 */
	void SetOnFriendRequestAcceptedNotifDelegate(FAcceptFriendsNotif const& OnAcceptFriendsNotif)
	{
		AcceptFriendsNotif = OnAcceptFriendsNotif;
	}

	/**
	 * @brief Set a trigger function when a friend request is received
	 * @param OnRequestFriendsNotif return models called FAccelByteModelsRequestFriendsNotif
	 */
	void SetOnIncomingRequestFriendsNotifDelegate(FRequestFriendsNotif const& OnRequestFriendsNotif)
	{
		RequestFriendsNotif = OnRequestFriendsNotif;
	}

	/**
	 * @brief Set a trigger function when unfriended.
	 * @param OnUnfriendNotif return models called FAccelByteModelsUnfriendNotif
	 */
	void SetOnUnfriendNotifDelegate(FUnfriendNotif const& OnUnfriendNotif)
	{
		UnfriendNotif = OnUnfriendNotif;
	}

	/**
	 * @brief Set a trigger function when friend request is cancelled
	 * @param OnCancelFriendsNotif return models called FAccelByteModelsCancelFriendsNotif
	 */
	void SetOnCancelFriendsNotifDelegate(FCancelFriendsNotif const& OnCancelFriendsNotif)
	{
		CancelFriendsNotif = OnCancelFriendsNotif;
	}

	/**
	 * @brief Set a trigger function when friend request is rejected
	 * @param OnRejectFriendsNotif return models called FAccelByteModelsRejectFriendsNotif
	 */
	void SetOnRejectFriendsNotifDelegate(FRejectFriendsNotif const& OnRejectFriendsNotif)
	{
		RejectFriendsNotif = OnRejectFriendsNotif;
	}

	/**
	 * @brief Set a trigger function when a lobby message failed to be parsed
	 * @param OnParsingError return models with the error code and message
	 */
	void SetParsingErrorDelegate(FErrorHandler const& OnParsingError)
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
	void SetInfoPartyResponseDelegate(FPartyInfoResponse const& OnInfoPartyResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyInfoResponse = OnInfoPartyResponse;
		OnPartyInfoError = OnError;
	}

	/**
	 * @brief create party response delegate
	 *
	 * @param OnCreatePartyResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetCreatePartyResponseDelegate(FPartyCreateResponse const& OnCreatePartyResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyCreateResponse = OnCreatePartyResponse;
		OnPartyCreateError = OnError;
	}

	/**
	 * @brief set leave party response delegate
	 *
	 * @param OnLeavePartyResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetLeavePartyResponseDelegate(FPartyLeaveResponse const& OnLeavePartyResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyLeaveResponse = OnLeavePartyResponse;
		OnPartyLeaveError = OnError;
	}

	/**
	 * @brief set invite party response delegate
	 *
	 * @param OnInvitePartyResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetInvitePartyResponseDelegate(FPartyInviteResponse const& OnInvitePartyResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyInviteResponse = OnInvitePartyResponse;
		OnPartyInviteError = OnError;
	}

	/**
	 * @brief set invite party join response
	 *
	 * @param OnInvitePartyJoinResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetInvitePartyJoinResponseDelegate(FPartyJoinResponse const& OnInvitePartyJoinResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyJoinResponse = OnInvitePartyJoinResponse;
		OnPartyJoinError = OnError;
	}
	
	/**
	 * @brief set invite party reject response
	 *
	 * @param OnInvitePartyRejectResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetInvitePartyRejectResponseDelegate(FPartyRejectResponse const& OnInvitePartyRejectResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyRejectResponse = OnInvitePartyRejectResponse;
		OnPartyRejectError = OnError;
	}

	/**
	 * @brief set invite party kick member response
	 *
	 * @param OnInvitePartyKickMemberResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetInvitePartyKickMemberResponseDelegate(FPartyKickResponse const& OnInvitePartyKickMemberResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyKickResponse = OnInvitePartyKickMemberResponse;
		OnPartyKickError = OnError;
	}

	/**
	 * @brief misnamed, same as SetPartyDataUpdateNotifDelegate
	 *
	 * @param OnPartyDataUpdateNotif set delegate
	 */
	void SetPartyDataUpdateResponseDelegate(FPartyDataUpdateNotif const& OnPartyDataUpdateNotif)
	{
		PartyDataUpdateNotif = OnPartyDataUpdateNotif;
	}
	
	/**
	 * @brief set party data update notif delegate
	 *
	 * @param OnPartyDataUpdateNotif set delegate
	 */
	void SetPartyDataUpdateNotifDelegate(FPartyDataUpdateNotif const& OnPartyDataUpdateNotif)
	{
		PartyDataUpdateNotif = OnPartyDataUpdateNotif;
	}

	/**
	 * @brief generate party code response
	 *
	 * @param OnPartyGenerateCodeResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPartyGenerateCodeResponseDelegate(FPartyGenerateCodeResponse const& OnPartyGenerateCodeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyGenerateCodeResponse = OnPartyGenerateCodeResponse;
		OnPartyGenerateCodeError = OnError;
	}

	/**
	 * @brief get party code response
	 *
	 * @param OnPartyGetCodeResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPartyGetCodeResponseDelegate(FPartyGetCodeResponse const& OnPartyGetCodeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyGetCodeResponse = OnPartyGetCodeResponse;
		OnPartyGetCodeError = OnError;
	}

	/**
	 * @brief delete party code response
	 *
	 * @param OnPartyDeleteCodeResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPartyDeleteCodeResponseDelegate(FPartyDeleteCodeResponse const& OnPartyDeleteCodeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyDeleteCodeResponse = OnPartyDeleteCodeResponse;
		OnPartyDeleteCodeError = OnError;
	}

	/**
	 * @brief join party via party code response
	 *
	 * @param OnPartyJoinViaCodeResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPartyJoinViaCodeResponseDelegate(FPartyJoinViaCodeResponse const& OnPartyJoinViaCodeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyJoinViaCodeResponse = OnPartyJoinViaCodeResponse;
		OnPartyJoinViaCodeError = OnError;
	}

	/**
	 * @brief set promote party leader response
	 *
	 * @param OnPartyPromoteLeaderResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPartyPromoteLeaderResponseDelegate(FPartyPromoteLeaderResponse const& OnPartyPromoteLeaderResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyPromoteLeaderResponse = OnPartyPromoteLeaderResponse;
		OnPartyPromoteLeaderError = OnError;
	}

	/**
	 * @brief set party send notification response
	 *
	 * @param OnPartySendNotifResponse set delegate .
	 */
	void SetPartySendNotifResponseDelegate(FPartySendNotifResponse const& OnPartySendNotifResponse
		, FErrorHandler const& OnError = nullptr) 
	{
		PartySendNotifResponse = OnPartySendNotifResponse;
		OnPartySendNotifError = OnError;
	}

	// Chat
	/**
	 * @brief set private message delegate
	 *
	 * @param OnPrivateMessageResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPrivateMessageResponseDelegate(FPersonalChatResponse const& OnPrivateMessageResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PersonalChatResponse = OnPrivateMessageResponse;
		OnPersonalChatError = OnError;
	}

	/**
	 * @brief set party message response
	 *
	 * @param OnPartyMessageResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetPartyMessageResponseDelegate(FPartyChatResponse const& OnPartyMessageResponse
		, FErrorHandler const& OnError = nullptr)
	{
		PartyChatResponse = OnPartyMessageResponse;
		OnPartyChatError = OnError;
	}

	/**
	 * @brief set join channel chat response
	 *
	 * @param OnJoinDefaultChannelResponse set delegate.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetJoinChannelChatResponseDelegate(FJoinDefaultChannelChatResponse const& OnJoinDefaultChannelResponse
		, FErrorHandler const& OnError = nullptr)
	{
		JoinDefaultChannelChatResponse = OnJoinDefaultChannelResponse;
		OnJoinDefaultChannelChatError = OnError;
	}

	/**
	 * @brief set channel message response
	 *
	 * @param OnChannelMessageResponse set delegate.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetChannelMessageResponseDelegate(FChannelChatResponse const& OnChannelMessageResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ChannelChatResponse = OnChannelMessageResponse;
		OnChannelChatError = OnError;
	}

	/**
	 * @brief set channel message notif
	 *
	 * @param OnChannelMessageNotif set delegate.
	 */
	void SetChannelMessageNotifDelegate(FChannelChatNotif const& OnChannelMessageNotif)
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
	void SetUserPresenceResponseDelegate(FSetUserPresenceResponse const& OnSetUserPresenceResponse
		, FErrorHandler const& OnError = nullptr)
	{
		SetUserPresenceResponse = OnSetUserPresenceResponse;
		OnSetUserPresenceError = OnError;
	}

	/**
	 * @brief (obsolete) misnamed, please use SetGetOnlineFriendsPresenceResponseDelegate
	 *
	 * @param OnGetAllUserPresenceResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetGetAllUserPresenceResponseDelegate(FGetAllFriendsStatusResponse const& OnGetAllUserPresenceResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetAllFriendsStatusResponse = OnGetAllUserPresenceResponse;
		OnGetAllFriendsStatusError = OnError;
	}

	/**
	 * @brief Set GetFriendsPresenceResponse delegate. 
	 *
	 * @param OnGetFriendsPresenceResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetGetOnlineFriendsPresenceResponseDelegate(FGetAllFriendsStatusResponse const& OnGetFriendsPresenceResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetAllFriendsStatusResponse = OnGetFriendsPresenceResponse;
		OnGetAllFriendsStatusError = OnError;
	}

	// Notification

	// Matchmaking
	/**
	 * @brief set start matchmaking response
	 *
	 * @param OnMatchmakingStart set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetStartMatchmakingResponseDelegate(FMatchmakingResponse const& OnMatchmakingStart
		, FErrorHandler const& OnError = nullptr)
	{
		MatchmakingStartResponse = OnMatchmakingStart;
		OnMatchmakingStartError = OnError;
	}

	/**
	 * @brief set cancel matchmaking notification
	 *
	 * @param OnMatchmakingCancel set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetCancelMatchmakingResponseDelegate(FMatchmakingResponse const& OnMatchmakingCancel
		, FErrorHandler const& OnError = nullptr)
	{
		MatchmakingCancelResponse = OnMatchmakingCancel;
		OnMatchmakingCancelError = OnError;
	}

	/**
	 * @brief set ready consent response notification
	 *
	 * @param OnReadyConsentResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetReadyConsentResponseDelegate(FReadyConsentResponse const& OnReadyConsentResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ReadyConsentResponse = OnReadyConsentResponse;
		OnReadyConsentError = OnError;
	}

	/**
	 * @brief set reject consent response notification
	 *
	 * @param OnRejectConsentResponse set delegate .
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetRejectConsentResponseDelegate(FRejectConsentResponse const& OnRejectConsentResponse
		, FErrorHandler const& OnError = nullptr)
	{
		RejectConsentResponse = OnRejectConsentResponse;
		OnRejectConsentError = OnError;
	}

	/**
	 * @brief set matchmaking notification
	 *
	 * @param OnMatchmakingNotification set delegate .
	 */
	void SetMatchmakingNotifDelegate(FMatchmakingNotif const& OnMatchmakingNotification)
	{
		MatchmakingNotif = OnMatchmakingNotification;
	}

	/**
	 * @brief set ready consent notification
	 *
	 * @param OnReadyConsentNotification set delegate .
	 */
	void SetReadyConsentNotifDelegate(FReadyConsentNotif const& OnReadyConsentNotification)
	{
		ReadyConsentNotif = OnReadyConsentNotification;
	}

	/**
	 * @brief set reject consent notification
	 *
	 * @param OnRejectConsentNotification set delegate .
	 */
	void SetRejectConsentNotifDelegate(FRejectConsentNotif const& OnRejectConsentNotification)
	{
		RejectConsentNotif = OnRejectConsentNotification;
	};

	/**
	 * @brief set rematchmaking notification
	 *
	 * @param OnRematchmakingNotification set delegate .
	 */
	void SetRematchmakingNotifDelegate(FRematchmakingNotif const& OnRematchmakingNotification)
	{
		RematchmakingNotif = OnRematchmakingNotification;
	}

	/**
	* @brief set ready consent notification
	*
	* @param OnDsNotification set delegate.
	*/
	void SetDsNotifDelegate(FDsNotif const& OnDsNotification)
	{
		DsNotif = OnDsNotification;
	}

	/**
	 * @brief set create DS response delegate
	 *
	 * @param OnCreateDSResponse Delegate to set.
	 * @param OnError Delegate that will be called when operation failed.
	 * */
	void SetCreateDSDelegate(FBaseResponse const& OnCreateDSResponse
		, FErrorHandler const& OnError = nullptr)
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
	void SetRequestFriendsResponseDelegate(FRequestFriendsResponse const& OnRequestFriendsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		RequestFriendsResponse = OnRequestFriendsResponse;
		OnRequestFriendsError = OnError;
	}
	
	/**
	 * @brief Set request for friends by public ID response.
	 *
	 * @param OnRequestFriendsResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetRequestFriendsByPublicIdResponseDelegate(FRequestFriendsResponse const& OnRequestFriendsResponse
		, FErrorHandler const& OnError = nullptr)
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
	void SetUnfriendResponseDelegate(FUnfriendResponse const& OnUnfriendResponse
		, FErrorHandler const& OnError = nullptr)
	{
		UnfriendResponse = OnUnfriendResponse;
		OnUnfriendError = OnError;
	}

	/**
	 * @brief Set list outgoing request of friend response.
	 *
	 * @param OnListOutgoingFriendsResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetListOutgoingFriendsResponseDelegate(FListOutgoingFriendsResponse const& OnListOutgoingFriendsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ListOutgoingFriendsResponse = OnListOutgoingFriendsResponse;
		OnListOutgoingFriendsError = OnError;
	}

	/**
	 * @brief Set list outgoing request of friend response with time.
	 *
	 * @param OnListOutgoingFriendsWithTimeResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetListOutgoingFriendsWithTimeResponseDelegate(FListOutgoingFriendsWithTimeResponse const& OnListOutgoingFriendsWithTimeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ListOutgoingFriendsWithTimeResponse = OnListOutgoingFriendsWithTimeResponse;
		OnListOutgoingFriendsWithTimeError = OnError;
	}

	/**
	 * @brief Set cancel request of friend response.
	 *
	 * @param OnCancelFriendsResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetCancelFriendsResponseDelegate(FCancelFriendsResponse const& OnCancelFriendsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		CancelFriendsResponse = OnCancelFriendsResponse;
		OnCancelFriendsError = OnError;
	}

	/**
	 * @brief Set incoming request of friend response.
	 *
	 * @param OnListIncomingFriendsResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetListIncomingFriendsResponseDelegate(FListIncomingFriendsResponse const& OnListIncomingFriendsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ListIncomingFriendsResponse = OnListIncomingFriendsResponse;
		OnListIncomingFriendsError = OnError;
	}

	/**
	 * @brief Set incoming request of friend response with time.
	 *
	 * @param OnListIncomingFriendsWithTimeResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetListIncomingFriendsWithTimeResponseDelegate(FListIncomingFriendsWithTimeResponse const& OnListIncomingFriendsWithTimeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ListIncomingFriendsWithTimeResponse = OnListIncomingFriendsWithTimeResponse;
		OnListIncomingFriendsWithTimeError = OnError;
	}

	/**
	 * @brief Set accept friend response.
	 *
	 * @param OnAcceptFriendsResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetAcceptFriendsResponseDelegate(FAcceptFriendsResponse const& OnAcceptFriendsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		AcceptFriendsResponse = OnAcceptFriendsResponse;
		OnAcceptFriendsError = OnError;
	}

	/**
	 * @brief Set reject request for friend response.
	 *
	 * @param OnRejectFriendsResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetRejectFriendsResponseDelegate(FRejectFriendsResponse const& OnRejectFriendsResponse
		, FErrorHandler const& OnError = nullptr)
	{
		RejectFriendsResponse = OnRejectFriendsResponse;
		OnRejectFriendsError = OnError;
	}

	/**
	 * @brief Set load friend list response.
	 *
	 * @param OnLoadFriendListResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetLoadFriendListResponseDelegate(FLoadFriendListResponse const& OnLoadFriendListResponse
		, FErrorHandler const& OnError = nullptr)
	{
		LoadFriendListResponse = OnLoadFriendListResponse;
		OnLoadFriendListError = OnError;
	}

	/**
	 * @brief Set get friendship status response.
	 *
	 * @param OnGetFriendshipStatusResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetGetFriendshipStatusResponseDelegate(FGetFriendshipStatusResponse const& OnGetFriendshipStatusResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetFriendshipStatusResponse = OnGetFriendshipStatusResponse;
		OnGetFriendshipStatusError = OnError;
	}

	// Player
	/**
	 * @brief set unblock player responses
	 *
	 * @param OnBlockPlayerResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetBlockPlayerResponseDelegate(FBlockPlayerResponse const& OnBlockPlayerResponse
		, FErrorHandler const& OnError = nullptr)
	{
		BlockPlayerResponse = OnBlockPlayerResponse;
		OnBlockPlayerError = OnError;
	}

	/**
	 * @brief set unblock player responses
	 *
	 * @param OnUnblockPlayerResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetUnblockPlayerResponseDelegate(FUnblockPlayerResponse const& OnUnblockPlayerResponse
		, FErrorHandler const& OnError = nullptr)
	{
		UnblockPlayerResponse = OnUnblockPlayerResponse;
		OnUnblockPlayerError = OnError;
	}

	/**
	 * @brief set a function triggered when user is blocked
	 *
	 * @param OnBlockPlayerNotif Delegate that will be set.
	 * */
	void SetBlockPlayerNotifDelegate(FBlockPlayerNotif const& OnBlockPlayerNotif)
	{
		BlockPlayerNotif = OnBlockPlayerNotif;
	}

	/**
	 * @brief set a function triggered when user is unblocked
	 *
	 * @param OnUnblockPlayerNotif Delegate that will be set.
	 * */
	void SetUnblockPlayerNotifDelegate(FUnblockPlayerNotif const& OnUnblockPlayerNotif)
	{
		UnblockPlayerNotif = OnUnblockPlayerNotif;
	}

	// Error
	/**
	 * @brief set a function triggered when there is a general lobby error notification.
	 *
	 * @param OnErrorNotif Delegate that will be set.
	 * */
	void SetErrorNotifDelegate(FErrorNotif const& OnErrorNotif)
	{
		ErrorNotif = OnErrorNotif;
	}

	/**
	 * @brief Set SignalingP2P delegate.
	 *
	 * @param OnSignalingP2P Delegate that will be set.
	 */
	void SetSignalingP2PDelegate(FSignalingP2P const& OnSignalingP2P)
	{
		SignalingP2PNotif = OnSignalingP2P;
	}

	/**
	 * @brief Set SetSessionAttribute delegate.
	 *
	 * @param OnSetSessionAttributeResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetSetSessionAttributeDelegate(FSetSessionAttributeResponse const& OnSetSessionAttributeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		SetSessionAttributeResponse = OnSetSessionAttributeResponse;
		OnSetSessionAttributeError = OnError;
		
	}

	/**
	 * @brief Set GetSessionAttribute delegate.
	 *
	 * @param OnGetSessionAttributeResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetGetSessionAttributeDelegate(FGetSessionAttributeResponse const& OnGetSessionAttributeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetSessionAttributeResponse = OnGetSessionAttributeResponse;
		OnGetSessionAttributeError = OnError;
	}

	/**
	 * @brief Set SetSessionAttribute delegate.
	 *
	 * @param OnGetAllSessionAttributeResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetGetAllSessionAttributeDelegate(FGetAllSessionAttributeResponse const& OnGetAllSessionAttributeResponse
		, FErrorHandler const& OnError = nullptr)
	{
		GetAllSessionAttributeResponse = OnGetAllSessionAttributeResponse;
		OnGetAllSessionAttributeError = OnError;
	}

	/**
	 * @brief Set SetSessionAttribute delegate.
	 *
	 * @param OnRefreshTokenResponse Delegate that will be set.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetRefreshTokenDelegate(const FRefreshTokenResponse& OnRefreshTokenResponse
		, FErrorHandler const& OnError = nullptr)
	{
		RefreshTokenResponse = OnRefreshTokenResponse;
		OnRefreshTokenError = OnError;
	}

	// Metrics
	/**
	 * @brief Set delegate for set user region response.
	 *
	 * @param OnChangeUserRegionResponse Delegate that will be called when operation success.
	 * @param OnError Delegate that will be called when operation failed.
	 */
	void SetChangeUserRegionDelegate(FChangeUserRegionResponse const& OnChangeUserRegionResponse
		, FErrorHandler const& OnError = nullptr)
	{
		ChangeUserRegionResponse = OnChangeUserRegionResponse;
		OnChangeUserRegionError = OnError;
	}

	FDelegateHandle AddOneTimeCodeLinkedNotifDelegate(THandler<FAccelByteModelsOneTimeCodeLinked> const& OneTimeCodeLinkedNotifDelegate)
	{
		return OneTimeCodeLinkedNotif.AddLambda([OneTimeCodeLinkedNotifDelegate](const FAccelByteModelsOneTimeCodeLinked& Response)
			{
				OneTimeCodeLinkedNotifDelegate.ExecuteIfBound(Response);
			});
	}

	bool RemoveOneTimeCodeLinkedNotifDelegate(FDelegateHandle const& OneTimeCodeLinkedNotifDelegateHandle)
	{
		return OneTimeCodeLinkedNotif.Remove(OneTimeCodeLinkedNotifDelegateHandle);
	}

	/**
	 * @brief Bulk add friend(s), don't need any confirmation from the player.
	 *
	 * @param UserIds the list of UserId you want to make friend with.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkFriendRequest(FAccelByteModelsBulkFriendsRequest const& UserIds
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync platform specific friend list to AccelByte friend list.
	 * This will automatically add platform specific friend list to AccelByte friend list if the friends already linked
	 * their account to AccelByte service.
	 *
	 * @param Request the request of S2S friend sync.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncThirdPartyFriends(FAccelByteModelsSyncThirdPartyFriendsRequest const& Request
		, THandler<TArray<FAccelByteModelsSyncThirdPartyFriendsResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync third party platform block list to the AccelByte block list. Automatically will add players from the
	 * platform block list to the AccelByte block list, if the blocked player has linked their account with an AccelByte account.
	 *
	 * @param Request Request model used to configure the block list sync API call
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncThirdPartyBlockList(FAccelByteModelsSyncThirdPartyBlockListRequest const& Request
		, THandler<TArray<FAccelByteModelsSyncThirdPartyBlockListResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Get party data by party ID.
	 *
	 * @param PartyId Targeted party Id.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyData model.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartyData(FString const& PartyId
		, THandler<FAccelByteModelsPartyData> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk Get User(s) Presence, can get specific user's presence status not limited to friend.
	 *
	 * @param UserIds the list of UserId you want to request.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsBulkUserStatusNotif.
	 * @param OnError This will be called when the operation failed.
	 * @param CountOnly Will only return the status count, without the user's data when set to true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetUserPresence(TArray<FString> const& UserIds
		, THandler<FAccelByteModelsBulkUserStatusNotif> const& OnSuccess
		, FErrorHandler const& OnError
		, bool CountOnly = false);

	/**
	 * @brief Bulk Get User(s) Presence, can get specific user's presence status not limited to friend.
	 *
	 * @param UserIds the list of UserId you want to request.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a FAccelByteModelsBulkUserStatusNotif.
	 * @param OnError This will be called when the operation failed.
	 * @param CountOnly Will only return the status count, without the user's data when set to true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetUserPresenceV2(TArray<FString> const& UserIds
		, THandler<FAccelByteModelsBulkUserStatusNotif> const& OnSuccess
		, FErrorHandler const& OnError
		, bool CountOnly = false);

	/**
	 * @brief  Get party storage (attributes) by party ID.
	 *
	 * @param PartyId Targeted party Id.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsPartyDataNotif model.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartyStorage(FString const& PartyId
		, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  List Blocked User's Id
	 *
	 * @param UserId User Id of the Player.
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockedUserResponse model.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListOfBlockedUsers(FString const& UserId
		, THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess
		, FErrorHandler const& OnError);

	/**
	* @brief List Blocked User's Id, override of GetListOfBlockedUser,
	* will get current user's List of Blocked User's Id
	*
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockedUserResponse model.
	* @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListOfBlockedUsers(THandler<FAccelByteModelsListBlockedUserResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	* @brief List Id of Player that blocks an User.
	*
	* @param UserId User Id of the Player.
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
	* @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListOfBlockers(FString const& UserId
		, THandler<FAccelByteModelsListBlockerResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	* @brief List Blockers User's Id, override of GetListOfBlocker,
	* will get current user's List of Blocker User's Id
	*
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
	* @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListOfBlockers(THandler<FAccelByteModelsListBlockerResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	* @brief (DEPRECATED) Send a freeform notification message to a user
	* 
	* @param SendToUserId user id to send this notification message
	* @param Message notification message
	* @param bAsync shou;d the message be sent even if the receiving user is offline?
	* @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsListBlockerResponse model.
	* @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendNotificationToUser(FString const& SendToUserId
		, FAccelByteModelsFreeFormNotificationRequest const& Message
		, bool bAsync
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
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
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr WritePartyStorage(FString const& PartyId
		, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> const& PayloadModifier
		, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
		, FErrorHandler const& OnError
		, uint32 RetryAttempt = 1);

	/**
	 * @brief Set token generator to be used when trying to connect to lobby using ownership token.
	 *
	 * @param TokenGenerator The token generator.
	 */
	void SetTokenGenerator(TSharedPtr<IAccelByteTokenGenerator> const& TokenGenerator);

	/**
	 * @brief Change current user region for metric.
	 *
	 * @param Region The region of current user based on lowest ping to QOS servers.
	 */
	void ChangeUserRegion(FString const& Region);

	/**
	 * @brief Get list of notifications from current user from specified time.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. Will return FAccelByteModelsGetUserNotificationsResponse model.
	 * @param OnError This will be called when the operation failed.
	 * @param StartTime	[Optional] The beginning sequenceID to query.
	 * @param EndTime [Optional] The end sequenceID to query.
	 * @param Offset [Optional] The offset of the result. Default value is 0.
	 * @param Limit [Optional] The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetNotifications(THandler<FAccelByteModelsGetUserNotificationsResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FDateTime const& StartTime = FDateTime{0}
		, FDateTime const& EndTime= FDateTime{0}
		, int32 Offset = 0
		, int32 Limit = 25);

	static FString LobbyMessageToJson(FString const& Message);

	static void ClearLobbyErrorMessages();

	/**
	 * @brief Lock notifications that has SequenceID. Next notification received won't be broadcasted until key is destroyed.
	 * @return Key of the lock, while key exist next notifications won't be broadcasted.
	 */
	TSharedPtr<FAccelByteKey> LockNotifications();

	/**
	 * @brief Startup module
	 */
	void Startup();
private:
	Lobby(Lobby const&) = delete; // Copy constructor
	Lobby(Lobby&&) = delete; // Move constructor
	Lobby& operator=(Lobby const&) = delete; // Copy assignment operator
	Lobby& operator=(Lobby &&) = delete; // Move assignment operator

	void OnConnected();
	
	void OnConnectionError(FString const& Error);
	
	void OnMessage(FString const& Message, bool bSkipConditioner = false);
	
	void OnClosed(int32 StatusCode
		, FString const& Reason
		, bool WasClean);

	void OnReconnectAttempt(FReconnectAttemptInfo const& ReconnectAttemptInfo);

	void OnMassiveOutage(FMassiveOutageInfo const& MassiveOutageInfo);

    FString SendRawRequest(FString const& MessageType
    	, FString const& MessageIDPrefix
    	, FString const& CustomPayload = TEXT(""));
	
    FString GenerateMessageID(FString const& Prefix = TEXT("")) const;
	
	void CreateWebSocket(FString const& Token = "");
	
	FAccelByteTaskWPtr FetchLobbyErrorMessages();
	
	bool ExtractLobbyMessageMetaData(FString const& InLobbyMessage
		, TSharedRef<FLobbyMessageMetaData> const& OutLobbyMessageMetaData);
	void SendBufferedNotifications();

	bool TryBufferNotification(FString const& ParsedJsonString);

	void OnGetMissingNotificationSuccess(FAccelByteModelsGetUserNotificationsResponse const& MissingNotifications);

	void OnGetMissingNotificationError(int32 ErrorCode, FString const& ErrorMessage);

	void OnTokenReceived(FString const& Token);

	bool Tick(float DeltaTime);

	void HandleLobbyMessageByType(FHandleLobbyMessageData const& MessageData);
	
#pragma region Notification Buffer
private:
	mutable FCriticalSection NotificationBufferLock{};
	FAccelByteNotificationBuffer NotificationBuffer{};
#pragma endregion

#pragma region Message Parsing
	void HandleMessageResponse(FString const& ReceivedMessageType
		, FString const& ParsedJsonString
		, TSharedPtr<FJsonObject> const& ParsedJsonObj
		, TSharedPtr<FLobbyMessageMetaData> const& MessageMeta);

	mutable FCriticalSection UnbanScheduleLock{};

	FString GenerateUnbanScheduleKey(FAccelByteModelsUserBannedNotification const& Result);
	
	void SetUnbanSchedule(FAccelByteModelsUserBannedNotification const& Result);

	void RemoveUnbanSchedule(FAccelByteModelsUserBannedNotification const& Result);
	
	void HandleMessageNotif(FString const& ReceivedMessageType
		, FString const& ParsedJsonString
		, TSharedPtr<FJsonObject> const& ParsedJsonObj
		, bool bSkipConditioner);
	
	void HandleV2SessionNotif(FString const& ParsedJsonString, bool bSkipConditioner);
	void DispatchV2SessionMessageByTopic(FString const& Topic, FString const& Payload, FString const& ParsedJsonString);

	void HandleV2MatchmakingNotif(FAccelByteModelsNotificationMessage const& Message, bool bSkipConditioner);
	void DispatchV2MatchmakingMessageByTopic(FString const& Topic, FString const& Payload) const;

	void HandleOneTimeCodeLinkedNotif(FAccelByteModelsNotificationMessage const& Message);

	void InitializeV2MatchmakingNotifTopics();

	void OnNotificationSenderMessageReceived(FString const& Payload);
	
	TArray<EV2MatchmakingNotifTopic> MatchmakingV2NotifTopics;
	
	static TMap<FString, Response> ResponseStringEnumMap;
	static TMap<FString, Notif> NotifStringEnumMap;
#pragma endregion

	static TMap<FString, FString> LobbyErrorMessages;
	const float LobbyTickPeriod = 0.5;
	const float PingDelay;
	bool bWasWsConnectionError = false;
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
	FConnectionClosed Reconnecting;
	FReconnectAttempted ReconnectAttempted;
	FMassiveOutage MassiveOutage;
	TSharedPtr<IAccelByteTokenGenerator> TokenGenerator;
	FAccelByteLockableQueue<FHandleLobbyMessageData> NotificationQueue;

	FDelegateHandle OnTokenReceivedDelegateHandle{};

#pragma region Unban Schedule
	struct FUnbanSchedule
	{
		virtual ~FUnbanSchedule()
		{
			if (DelegateHandle.IsValid())
			{
				FTickerAlias::GetCoreTicker().RemoveTicker(DelegateHandle);
			}
		}
		
		FDelegateHandleAlias DelegateHandle;
		double ScheduledTime;
	};

	typedef TSharedRef<FUnbanSchedule> FUnbanScheduleRef;
	typedef TSharedPtr<FUnbanSchedule> FUnbanSchedulePtr;
	
	TMap<FString, FUnbanScheduleRef> UnbanSchedules;
#pragma endregion

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
	TMap<FString, FPartySendNotifResponse> MessageIdPartySendNotifResponseMap;

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
	TMap<FString, FRejectConsentResponse> MessageIdRejectConsentResponseMap;

	// Custom Game
	TMap<FString, FBaseResponse> MessageIdCreateDSResponseMap;

	// Friends
	TMap<FString, FRequestFriendsResponse> MessageIdRequestFriendsResponseMap;
	TMap<FString, FRequestFriendsResponse> MessageIdRequestFriendsByPublicIdResponseMap;
	TMap<FString, FUnfriendResponse> MessageIdUnfriendResponseMap;
	TMap<FString, FListOutgoingFriendsResponse> MessageIdListOutgoingFriendsResponseMap;
	TMap<FString, FListOutgoingFriendsWithTimeResponse> MessageIdListOutgoingFriendsWithTimeResponseMap;
	TMap<FString, FCancelFriendsResponse> MessageIdCancelFriendsResponseMap;
	TMap<FString, FListIncomingFriendsResponse> MessageIdListIncomingFriendsResponseMap;
	TMap<FString, FListIncomingFriendsWithTimeResponse> MessageIdListIncomingFriendsWithTimeResponseMap;
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

	// Metrics
	TMap<FString, FChangeUserRegionResponse> MessageIdChangeUserRegionResponseMap;
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
	FPartySendNotifResponse PartySendNotifResponse;
	FPartyNotif PartyNotif;

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
	FNotifBroadcaster MessageNotifBroadcaster;
	FUserBannedNotification UserBannedNotification;
	FUserUnbannedNotification UserUnbannedNotification;
	FOneTimeCodeLinkedNotif OneTimeCodeLinkedNotif;

	// session v2
	FV2PartyInvitedNotif V2PartyInvitedNotif;
	FV2PartyInviteTimeoutNotif V2PartyInviteTimeoutNotif;
	FV2PartyMembersChangedNotif V2PartyMembersChangedNotif;
	FV2PartyJoinedNotif V2PartyJoinedNotif;
	FV2PartyRejectedNotif V2PartyRejectedNotif;
	FV2PartyKickedNotif V2PartyKickedNotif;
	FV2PartyUpdatedNotif V2PartyUpdatedNotif;
	FV2PartyInviteCanceledNotif V2PartyInviteCanceledNotif;
	FV2PartyCreatedNotif V2PartyCreatedNotif;

	FV2GameSessionInvitedNotif V2GameSessionInvitedNotif;
	FV2GameSessionInviteTimeoutNotif V2GameSessionInviteTimeoutNotif;
	FV2GameSessionJoinedNotif V2GameSessionJoinedNotif;
	FV2GameSessionMembersChangedNotif V2GameSessionMembersChangedNotif;
	FV2GameSessionUpdatedNotif V2GameSessionUpdatedNotif;
	FV2GameSessionKickedNotif V2GameSessionKickedNotif;
	FV2GameSessionRejectedNotif V2GameSessionRejectedNotif;
	FV2GameSessionEndedNotif V2GameSessionEndedNotif;
	FV2GameSessionInviteCanceledNotif V2GameSessionInviteCanceledNotif;

	FV2SessionStorageChangedNotif V2SessionStorageChangedNotif;
	FV2SessionJoinedSecretNotif V2SessionJoinedSecretNotif; 

	FV2DSStatusChangedNotif V2DSStatusChangedNotif;

	FV2MatchmakingMatchFoundNotif V2MatchmakingMatchFoundNotif;
	FV2MatchmakingStartNotif V2MatchmakingStartNotif;
	FV2MatchmakingExpiredNotif V2MatchmakingExpiredNotif;
	FV2MatchmakingCanceledNotif V2MatchmakingCanceledNotif;
	FV2NativeSessionSyncNotif V2NativeSessionSyncNotif;

	// Matchmaking
	FMatchmakingResponse MatchmakingStartResponse;
	FMatchmakingResponse MatchmakingCancelResponse;
	FReadyConsentResponse ReadyConsentResponse;
	FRejectConsentResponse RejectConsentResponse;
	FMatchmakingNotif MatchmakingNotif;
	FReadyConsentNotif ReadyConsentNotif;
	FRejectConsentNotif RejectConsentNotif;
	FRematchmakingNotif RematchmakingNotif;
	FDsNotif DsNotif;

	// Custom Game
	FBaseResponse CreateDSResponse;

	// Friends
	FRequestFriendsResponse RequestFriendsResponse;
	FRequestFriendsResponse RequestFriendsByPublicIdResponse;
	FUnfriendResponse UnfriendResponse;
	FListOutgoingFriendsResponse ListOutgoingFriendsResponse;
	FListOutgoingFriendsWithTimeResponse ListOutgoingFriendsWithTimeResponse;
	FCancelFriendsResponse CancelFriendsResponse;
	FListIncomingFriendsResponse ListIncomingFriendsResponse;
	FListIncomingFriendsWithTimeResponse ListIncomingFriendsWithTimeResponse;
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

	// Metrics
	FChangeUserRegionResponse ChangeUserRegionResponse;
#pragma endregion
	
	struct PartyStorageWrapper
	{
		FString PartyId;
		int RemainingAttempt;
		THandler<FAccelByteModelsPartyDataNotif> OnSuccess;
		FErrorHandler OnError;
		TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier;
	};

	FAccelByteTaskWPtr RequestWritePartyStorage(FString const& PartyId
		, FAccelByteModelsPartyDataUpdateRequest const& Data
		, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
		, FErrorHandler const& OnError
		, FSimpleDelegate const& OnConflicted = NULL);

	FAccelByteTaskWPtr WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> const& DataWrapper);

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
	FErrorHandler OnPartySendNotifError;
	FErrorHandler OnPersonalChatError;
	FErrorHandler OnPartyChatError;
	FErrorHandler OnJoinDefaultChannelChatError;
	FErrorHandler OnChannelChatError;
	FErrorHandler OnSetUserPresenceError;
	FErrorHandler OnGetAllFriendsStatusError;
	FErrorHandler OnMatchmakingStartError;
	FErrorHandler OnMatchmakingCancelError;
	FErrorHandler OnReadyConsentError;
	FErrorHandler OnRejectConsentError;
	FErrorHandler OnRequestFriendsError;
	FErrorHandler OnRequestFriendsByPublicIdError;
	FErrorHandler OnUnfriendError;
	FErrorHandler OnListOutgoingFriendsError;
	FErrorHandler OnListOutgoingFriendsWithTimeError;
	FErrorHandler OnCancelFriendsError;
	FErrorHandler OnListIncomingFriendsError;
	FErrorHandler OnListIncomingFriendsWithTimeError;
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
	FErrorHandler OnChangeUserRegionError;

#pragma region Messaging System
private:
	FOnMessagingSystemReceivedMessage OnReceivedQosLatenciesUpdatedDelegate;
	FDelegateHandle QosLatenciesUpdatedDelegateHandle;

	FDelegateHandle NotificationMessageDelegateHandle;
	FOnMessagingSystemReceivedMessage NotificationSenderListenerDelegate;
	FDelegateHandle NotificationSenderListenerDelegateHandle;

	bool bIsStarted = false;

	void InitializeMessaging();

	void OnReceivedQosLatencies(FString const& Payload);

#pragma endregion
};

typedef TSharedRef<Lobby, ESPMode::ThreadSafe> LobbyRef;
typedef TSharedPtr<Lobby, ESPMode::ThreadSafe> LobbyPtr;
typedef TWeakPtr<Lobby, ESPMode::ThreadSafe> LobbyWPtr;

} // Namespace Api
} // Namespace AccelByte
