// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteLobbyApi.h"
#include "Modules/ModuleManager.h"
#include "IWebSocket.h"
#include "Api/AccelByteQos.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteNetworkConditioner.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/IWebSocketFactory.h"
#include "Core/FUnrealWebSocketFactory.h"
#include "Core/IAccelByteTokenGenerator.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteMessageParser.h"
#include "Core/AccelByteUtilities.h"
#include "JsonUtilities.h"
#include "Core/AccelByteNotificationSender.h"
#include "Engine/Engine.h"
#include "Misc/Base64.h"
#include "Core/AccelByteEntitlementTokenGenerator.h"
#include "Core/AccelByteWebSocketErrorTypes.h"

DEFINE_LOG_CATEGORY(LogAccelByteLobby);

namespace AccelByte
{
namespace Api
{
	namespace LobbyRequest
	{
		// Party
		const FString PartyInfo = TEXT("partyInfoRequest");
		const FString PartyCreate = TEXT("partyCreateRequest");
		const FString PartyLeave = TEXT("partyLeaveRequest");
		const FString PartyInvite = TEXT("partyInviteRequest");
		const FString PartyJoin = TEXT("partyJoinRequest");
		const FString PartyReject = TEXT("partyRejectRequest");
		const FString PartyKick = TEXT("partyKickRequest");
		const FString PartyGenerateCode = TEXT("partyGenerateCodeRequest");
		const FString PartyGetCode = TEXT("partyGetCodeRequest");
		const FString PartyDeleteCode = TEXT("partyDeleteCodeRequest");
		const FString PartyJoinViaCode = TEXT("partyJoinViaCodeRequest");
		const FString PartyPromoteLeader = TEXT("partyPromoteLeaderRequest");
		const FString PartySendNotif = TEXT("partySendNotifRequest");

		// Chat
		const FString PersonalChat = TEXT("personalChatRequest");
		const FString PartyChat = TEXT("partyChatRequest");
		const FString JoinDefaultChannelChat = TEXT("joinDefaultChannelRequest");
		const FString ChannelChat = TEXT("sendChannelChatRequest");

		// Presence
		const FString SetUserPresence = TEXT("setUserStatusRequest");
		const FString GetAllFriendsStatus = TEXT("friendsStatusRequest");

		// Notification

		// Matchmaking
		const FString MatchmakingStart = TEXT("startMatchmakingRequest");
		const FString MatchmakingCancel = TEXT("cancelMatchmakingRequest");
		const FString ReadyConsent = TEXT("setReadyConsentRequest");
		const FString RejectConsent = TEXT("setRejectConsentRequest");

		// Custom Game
		const FString CreateDS = TEXT("createDSRequest");

		// Friends
		const FString RequestFriends = TEXT("requestFriendsRequest");
		const FString RequestFriendsByPublicId = TEXT("requestFriendsByPublicIDRequest");
		const FString Unfriend = TEXT("unfriendRequest");
		const FString ListOutgoingFriends = TEXT("listOutgoingFriendsRequest");
		const FString ListOutgoingFriendsWithTime = TEXT("listOutgoingFriendsWithTimeRequest");
		const FString CancelFriends = TEXT("cancelFriendsRequest");
		const FString ListIncomingFriends = TEXT("listIncomingFriendsRequest");
		const FString ListIncomingFriendsWithTime = TEXT("listIncomingFriendsWithTimeRequest");
		const FString AcceptFriends = TEXT("acceptFriendsRequest");
		const FString RejectFriends = TEXT("rejectFriendsRequest");
		const FString LoadFriendList = TEXT("listOfFriendsRequest");
		const FString GetFriendshipStatus = TEXT("getFriendshipStatusRequest");

		// Block Users
		const FString BlockPlayer = TEXT("blockPlayerRequest");
		const FString UnblockPlayer = TEXT("unblockPlayerRequest");

		//Signaling
		const FString SignalingP2PNotif = TEXT("signalingP2PNotif");

		//Session Attribute
		const FString SetSessionAttribute = TEXT("setSessionAttributeRequest");
		const FString GetSessionAttribute = TEXT("getSessionAttributeRequest");
		const FString GetAllSessionAttribute = TEXT("getAllSessionAttributeRequest");

		// Refresh Token
		const FString RefreshToken = TEXT("refreshTokenRequest");

		// Metrics
		const FString ChangeUserRegion = TEXT("changeRegionRequest");
	}

	namespace LobbyResponse
	{
		// default
		const FString ConnectedNotif = TEXT("connectNotif");
		const FString DisconnectNotif = TEXT("disconnectNotif");

		// Party
		const FString PartyInfo = TEXT("partyInfoResponse");
		const FString PartyCreate = TEXT("partyCreateResponse");
		const FString PartyLeave = TEXT("partyLeaveResponse");
		const FString PartyMemberLeaveNotif = TEXT("partyLeaveNotif");
		const FString PartyInvite = TEXT("partyInviteResponse");
		const FString PartyInviteNotif = TEXT("partyInviteNotif");
		const FString PartyGetInvitedNotif = TEXT("partyGetInvitedNotif");
		const FString PartyJoin = TEXT("partyJoinResponse");
		const FString PartyJoinNotif = TEXT("partyJoinNotif");
		const FString PartyReject = TEXT("partyRejectResponse");
		const FString PartyRejectNotif = TEXT("partyRejectNotif");
		const FString PartyKick = TEXT("partyKickResponse");
		const FString PartyKickNotif = TEXT("partyKickNotif");
		const FString PartyDataUpdateNotif = TEXT("partyDataUpdateNotif");
		const FString PartyMemberConnectNotif = TEXT("partyConnectNotif");
		const FString PartyMemberDisconnectNotif = TEXT("partyDisconnectNotif");
		const FString PartyGenerateCode = TEXT("partyGenerateCodeResponse");
		const FString PartyGetCode = TEXT("partyGetCodeResponse");
		const FString PartyDeleteCode = TEXT("partyDeleteCodeResponse");
		const FString PartyJoinViaCode = TEXT("partyJoinViaCodeResponse");
		const FString PartyPromoteLeader = TEXT("partyPromoteLeaderResponse");
		const FString PartySendNotif = TEXT("partySendNotifResponse");
		const FString PartyNotif = TEXT("partyNotif");

		// Chat
		const FString PersonalChat = TEXT("personalChatResponse");
		const FString PersonalChatNotif = TEXT("personalChatNotif");
		const FString PartyChat = TEXT("partyChatResponse");
		const FString PartyChatNotif = TEXT("partyChatNotif");
		const FString JoinChannelChat = TEXT("joinDefaultChannelResponse");
		const FString ChannelChat = TEXT("sendChannelChatResponse");
		const FString ChannelChatNotif = TEXT("channelChatNotif");

		// Presence
		const FString SetUserPresence = TEXT("setUserStatusResponse");
		const FString FriendStatusNotif = TEXT("userStatusNotif");
		const FString GetAllFriendsStatus = TEXT("friendsStatusResponse");

		// Notification
		const FString MessageNotif = TEXT("messageNotif");
		const FString UserBannedNotification = TEXT("userBannedNotification");
		const FString UserUnbannedNotification = TEXT("userUnbannedNotification");

		// Matchmaking
		const FString MatchmakingStart = TEXT("startMatchmakingResponse");
		const FString MatchmakingCancel = TEXT("cancelMatchmakingResponse");
		const FString ReadyConsent = TEXT("setReadyConsentResponse");
		const FString RejectConsent = TEXT("setRejectConsentResponse");
		const FString MatchmakingNotif = TEXT("matchmakingNotif");
		const FString ReadyConsentNotif = TEXT("setReadyConsentNotif");
		const FString RejectConsentNotif = TEXT("setRejectConsentNotif");
		const FString RematchmakingNotif = TEXT("rematchmakingNotif");
		const FString DsNotif = TEXT("dsNotif");

		// Custom Game
		const FString CreateDS = TEXT("createDSResponse");

		// Friends
		const FString RequestFriends = TEXT("requestFriendsResponse");
		const FString RequestFriendsByPublicId = TEXT("requestFriendsByPublicIDResponse");
		const FString Unfriend = TEXT("unfriendResponse");
		const FString ListOutgoingFriends = TEXT("listOutgoingFriendsResponse");
		const FString ListOutgoingFriendsWithTime = TEXT("listOutgoingFriendsWithTimeResponse");
		const FString CancelFriends = TEXT("cancelFriendsResponse");
		const FString ListIncomingFriends = TEXT("listIncomingFriendsResponse");
		const FString ListIncomingFriendsWithTime = TEXT("listIncomingFriendsWithTimeResponse");
		const FString AcceptFriends = TEXT("acceptFriendsResponse");
		const FString RejectFriends = TEXT("rejectFriendsResponse");
		const FString LoadFriendList = TEXT("listOfFriendsResponse");
		const FString GetFriendshipStatus = TEXT("getFriendshipStatusResponse");

		// Friends + Notification
		const FString AcceptFriendsNotif = TEXT("acceptFriendsNotif");
		const FString RequestFriendsNotif = TEXT("requestFriendsNotif");
		const FString UnfriendNotif = TEXT("unfriendNotif");
		const FString CancelFriendsNotif = TEXT("cancelFriendsNotif");
		const FString RejectFriendsNotif = TEXT("rejectFriendsNotif");

		// Block
		const FString BlockPlayer = TEXT("blockPlayerResponse");
		const FString UnblockPlayer = TEXT("unblockPlayerResponse");

		// Block + Notification
		const FString BlockPlayerNotif = TEXT("blockPlayerNotif");
		const FString UnblockPlayerNotif = TEXT("unblockPlayerNotif");

		//Signaling
		const FString SignalingP2PNotif = TEXT("signalingP2PNotif");

		// Error Notif
		const FString ErrorNotif = TEXT("errorNotif");
		
		//Session Attribute
		const FString SetSessionAttribute = TEXT("setSessionAttributeResponse");
		const FString GetSessionAttribute = TEXT("getSessionAttributeResponse");
		const FString GetAllSessionAttribute = TEXT("getAllSessionAttributeResponse");
		
		// Refresh Token
		const FString RefreshToken = TEXT("refreshTokenResponse");

		// V2 session notif
		const FString SessionNotif = TEXT("messageSessionNotif");

		// Metrics
		const FString ChangeUserRegion = TEXT("changeRegionResponse");
	}

	namespace Prefix
	{
		const FString Party = TEXT("party");
		const FString Chat = TEXT("chat");
		const FString Presence = TEXT("presence");
		const FString Matchmaking = TEXT("matchmaking");
		const FString Friends = TEXT("friends");
		const FString Block = TEXT("blocks");
		const FString Signaling = TEXT("signaling");
		const FString Attribute = TEXT("attribute");
		const FString Token = TEXT("token");
		const FString Metrics = TEXT("metrics");
	}

	namespace Suffix
	{
		const FString Response = TEXT("Response");
		const FString Notif = TEXT("Notif"); // Note: current usage is not yet uniformized -> Notif/Notification
	}

	enum Response : uint8
	{
		Invalid_Response,

		// Party
		PartyInfo,
		PartyCreate,
		PartyLeave,
		PartyInvite,
		PartyJoin,
		PartyReject,
		PartyKick,
		PartyGenerateCode,
		PartyGetCode,
		PartyDeleteCode,
		PartyJoinViaCode,
		PartyPromoteLeader,
		PartySendNotif,

		// Chat
		PersonalChat,
		PartyChat,
		JoinChannelChat,
		ChannelChat,

		// Presence
		SetUserPresence,
		GetAllFriendsStatus,

		// Matchmaking
		MatchmakingStart,
		MatchmakingCancel,
		ReadyConsent,
		RejectConsent,

		// Custom Game
		CreateDS,

		// Friends
		RequestFriends,
		RequestFriendsByPublicId,
		Unfriend,
		ListOutgoingFriends,
		ListOutgoingFriendsWithTime,
		CancelFriends,
		ListIncomingFriends,
		ListIncomingFriendsWithTime,
		AcceptFriends,
		RejectFriends,
		LoadFriendList,
		GetFriendshipStatus,

		// Block
		BlockPlayer,
		UnblockPlayer,

		//Session Attribute
		SetSessionAttribute,
		GetSessionAttribute,
		GetAllSessionAttribute,

		// Refresh Token
		RefreshToken,

		// Metrics
		ChangeUserRegion,

		MAX_Response,
	};

	enum Notif : uint8
	{
		Invalid_Notif,

		// default
		ConnectedNotif,
		DisconnectNotif,

		// Party
		PartyMemberLeaveNotif,
		PartyInviteNotif,
		PartyGetInvitedNotif,
		PartyJoinNotif,
		PartyRejectNotif,
		PartyKickNotif,
		PartyDataUpdateNotif,
		PartyMemberConnectNotif,
		PartyMemberDisconnectNotif,
		PartyNotif,

		// Chat
		PersonalChatNotif,
		PartyChatNotif,
		ChannelChatNotif,

		// Presence
		FriendStatusNotif,

		// Notification
		MessageNotif,
		UserBannedNotification,
		UserUnbannedNotification,

		// Matchmaking
		MatchmakingNotif,
		ReadyConsentNotif,
		RejectConsentNotif,
		RematchmakingNotif,
		DsNotif,

		// Friends
		AcceptFriendsNotif,
		RequestFriendsNotif,
		UnfriendNotif,
		CancelFriendsNotif,
		RejectFriendsNotif,

		// Block
		BlockPlayerNotif,
		UnblockPlayerNotif,

		// Signaling
		SignalingP2PNotif,

		// Error Notif
		ErrorNotif,

		MAX_Notif,
	};

/**
* Helper macro to enforce uniform naming, easier pair initialization, and readibility
*/
#define FORM_STRING_ENUM_PAIR(Type, MessageType) \
	{ LobbyResponse::MessageType, Type::MessageType } \

	TMap<FString, Response> Lobby::ResponseStringEnumMap{
		FORM_STRING_ENUM_PAIR(Response,PartyInfo),
		FORM_STRING_ENUM_PAIR(Response,PartyCreate),
		FORM_STRING_ENUM_PAIR(Response,PartyLeave),
		FORM_STRING_ENUM_PAIR(Response,PartyInvite),
		FORM_STRING_ENUM_PAIR(Response,PartyJoin),
		FORM_STRING_ENUM_PAIR(Response,PartyReject),
		FORM_STRING_ENUM_PAIR(Response,PartyKick),
		FORM_STRING_ENUM_PAIR(Response,PartyGenerateCode),
		FORM_STRING_ENUM_PAIR(Response,PartyGetCode),
		FORM_STRING_ENUM_PAIR(Response,PartyDeleteCode),
		FORM_STRING_ENUM_PAIR(Response,PartyJoinViaCode),
		FORM_STRING_ENUM_PAIR(Response,PartyPromoteLeader),
		FORM_STRING_ENUM_PAIR(Response,PartySendNotif),
		FORM_STRING_ENUM_PAIR(Response,PersonalChat),
		FORM_STRING_ENUM_PAIR(Response,PartyChat),
		FORM_STRING_ENUM_PAIR(Response,JoinChannelChat),
		FORM_STRING_ENUM_PAIR(Response,ChannelChat),
		FORM_STRING_ENUM_PAIR(Response,SetUserPresence),
		FORM_STRING_ENUM_PAIR(Response,GetAllFriendsStatus),
		FORM_STRING_ENUM_PAIR(Response,MatchmakingStart),
		FORM_STRING_ENUM_PAIR(Response,MatchmakingCancel),
		FORM_STRING_ENUM_PAIR(Response,ReadyConsent),
		FORM_STRING_ENUM_PAIR(Response,RejectConsent),
		FORM_STRING_ENUM_PAIR(Response,RequestFriends),
		FORM_STRING_ENUM_PAIR(Response,RequestFriendsByPublicId),
		FORM_STRING_ENUM_PAIR(Response,Unfriend),
		FORM_STRING_ENUM_PAIR(Response,ListOutgoingFriends),
		FORM_STRING_ENUM_PAIR(Response,ListOutgoingFriendsWithTime),
		FORM_STRING_ENUM_PAIR(Response,CancelFriends),
		FORM_STRING_ENUM_PAIR(Response,ListIncomingFriends),
		FORM_STRING_ENUM_PAIR(Response,ListIncomingFriendsWithTime),
		FORM_STRING_ENUM_PAIR(Response,AcceptFriends),
		FORM_STRING_ENUM_PAIR(Response,RejectFriends),
		FORM_STRING_ENUM_PAIR(Response,LoadFriendList),
		FORM_STRING_ENUM_PAIR(Response,GetFriendshipStatus),
		FORM_STRING_ENUM_PAIR(Response,BlockPlayer),
		FORM_STRING_ENUM_PAIR(Response,UnblockPlayer),
		FORM_STRING_ENUM_PAIR(Response,SetSessionAttribute),
		FORM_STRING_ENUM_PAIR(Response,GetSessionAttribute),
		FORM_STRING_ENUM_PAIR(Response,GetAllSessionAttribute),
		FORM_STRING_ENUM_PAIR(Response,RefreshToken),
		FORM_STRING_ENUM_PAIR(Response,CreateDS),
		FORM_STRING_ENUM_PAIR(Response,ChangeUserRegion),
	};

	TMap<FString, Notif> Lobby::NotifStringEnumMap{
		FORM_STRING_ENUM_PAIR(Notif,ConnectedNotif),
		FORM_STRING_ENUM_PAIR(Notif,DisconnectNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyMemberLeaveNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyInviteNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyGetInvitedNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyJoinNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyRejectNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyKickNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyDataUpdateNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyMemberConnectNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyMemberDisconnectNotif),
		FORM_STRING_ENUM_PAIR(Notif,PersonalChatNotif),
		FORM_STRING_ENUM_PAIR(Notif,PartyChatNotif),
		FORM_STRING_ENUM_PAIR(Notif,ChannelChatNotif),
		FORM_STRING_ENUM_PAIR(Notif,FriendStatusNotif),
		FORM_STRING_ENUM_PAIR(Notif,MessageNotif),
		FORM_STRING_ENUM_PAIR(Notif,UserBannedNotification),
		FORM_STRING_ENUM_PAIR(Notif,UserUnbannedNotification),
		FORM_STRING_ENUM_PAIR(Notif,MatchmakingNotif),
		FORM_STRING_ENUM_PAIR(Notif,ReadyConsentNotif),
		FORM_STRING_ENUM_PAIR(Notif,RejectConsentNotif),
		FORM_STRING_ENUM_PAIR(Notif,RematchmakingNotif),
		FORM_STRING_ENUM_PAIR(Notif,DsNotif),
		FORM_STRING_ENUM_PAIR(Notif,AcceptFriendsNotif),
		FORM_STRING_ENUM_PAIR(Notif,RequestFriendsNotif),
		FORM_STRING_ENUM_PAIR(Notif,UnfriendNotif),
		FORM_STRING_ENUM_PAIR(Notif,CancelFriendsNotif),
		FORM_STRING_ENUM_PAIR(Notif,RejectFriendsNotif),
		FORM_STRING_ENUM_PAIR(Notif,BlockPlayerNotif),
		FORM_STRING_ENUM_PAIR(Notif,UnblockPlayerNotif),
		FORM_STRING_ENUM_PAIR(Notif,SignalingP2PNotif),
		FORM_STRING_ENUM_PAIR(Notif,ErrorNotif),
	};

#undef FORM_STRING_ENUM_PAIR

void Lobby::Connect(FString const& Token)
{
	FReport::Log(FString(__FUNCTION__));

	Startup();

	if(TokenGenerator.IsValid() && Token.IsEmpty())
	{
		TokenGenerator->RequestToken();
		return;
	}

	if (!WebSocket.IsValid())
	{
		CreateWebSocket(Token);
	}

	if (WebSocket->IsConnected())
	{
		return;
	}

	WebSocket->Connect();
	UE_LOG(LogAccelByteLobby, Log, TEXT("Connecting to %s"), *SettingsRef.LobbyServerUrl);

	// if shipping build, skip fetching lobby error messages.
	// avoid fetching large json file to store lobby error messages.
#if !UE_BUILD_SHIPPING
	if(LobbyErrorMessages.Num() == 0)
	{
		FetchLobbyErrorMessages();
	}
#endif
}

void Lobby::Disconnect(bool ForceCleanup)
{
	FReport::Log(FString(__FUNCTION__));

	ChannelSlug = "";

	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::AuthTokenSet, AuthTokenSetDelegateHandle);
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::NotificationSenderLobby, NotificationSenderListenerDelegateHandle);
	}

	NotificationSenderListenerDelegate.Unbind();
	
	if(WebSocket.IsValid())
	{
		WebSocket->Disconnect(ForceCleanup);
	}

	UE_LOG(LogAccelByteLobby, Log, TEXT("Disconnected"));
}

bool Lobby::IsConnected() const
{
	FReport::Log(FString(__FUNCTION__));

	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void Lobby::SendPing()
{
	FReport::Log(FString(__FUNCTION__));

	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(FString());
	}
}

/**
Helper macro for ErrorHandler, SuccessHandler and IdResponseMap variable name. Made as separate macro to prevent linux-compiler error
*/
#define ID_RESPONSE_MAP(MessageType) MessageId ## MessageType ## ResponseMap
#define MESSAGE_ERROR_HANDLER(MessageType) On ## MessageType ## Error
#define MESSAGE_SUCCESS_HANDLER(MessageType) MessageType ## Response

/**
* @brief will auto cache MessageId-Response pair into the target map after sending the raw request
* @see SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED
*/
#define SEND_RAW_REQUEST_CACHED_RESPONSE(MessageType, MessageIDPrefix, CustomPayload) \
	const FString MessageId = SendRawRequest(LobbyRequest::MessageType, Prefix::MessageIDPrefix, CustomPayload); \
	if (!MessageId.IsEmpty()) \
	{ \
		ID_RESPONSE_MAP(MessageType).Emplace(MessageId, MESSAGE_SUCCESS_HANDLER(MessageType)); \
	} \

/**
* @brief will auto cache MessageId-Response pair into the target map after sending the raw request
* @param MessageType - request message type
* @return MessageId
*/
#define SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(MessageType, MessageIDPrefix, CustomPayload) \
	SEND_RAW_REQUEST_CACHED_RESPONSE(MessageType, MessageIDPrefix, CustomPayload) \
	return MessageId; \

//-------------------------------------------------------------------------------------------------
// Chat
//-------------------------------------------------------------------------------------------------
FString Lobby::SendPrivateMessage(FString const& UserId
	, FString const& Message)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PersonalChat
		, Chat
		, FString::Printf(TEXT("to: %s\npayload: %s\n"), *UserId, *Message));
}

FString Lobby::SendPartyMessage(FString const& Message)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyChat
		, Chat
		, FString::Printf(TEXT("payload: %s\n"), *Message));
}

FString Lobby::SendJoinDefaultChannelChatRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(JoinDefaultChannelChat
		, Chat
		, {});
}

FString Lobby::SendChannelMessage(FString const& Message)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ChannelSlug.IsEmpty())
	{
		SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(ChannelChat, Chat,
			FString::Printf(TEXT("channelSlug: %s\npayload: %s\n"), *ChannelSlug, *Message));
	}
	else
	{
		FAccelByteModelsChannelMessageResponse ErrorResult;
		ErrorResult.Code = FString::FromInt((int)ErrorCodes::InvalidRequest);
		ErrorResult.Message = TEXT("You're not in any chat channel.");
		ChannelChatResponse.ExecuteIfBound(ErrorResult);
		return "";
	}
}

//-------------------------------------------------------------------------------------------------
// Party
//-------------------------------------------------------------------------------------------------
FString Lobby::SendInfoPartyRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyInfo
		, Party
		, {})
}

FString Lobby::SendInfoPartyRequest(FPartyInfoResponse const& OnInfoPartyResponse
	, FErrorHandler const& OnError)
{
	SetInfoPartyResponseDelegate(OnInfoPartyResponse, OnError);
	return SendInfoPartyRequest();
}

FString Lobby::SendCreatePartyRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyCreate
		, Party
		, {})
}

FString Lobby::SendLeavePartyRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyLeave
		, Party
		, {})
}

FString Lobby::SendInviteToPartyRequest(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));
	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyInvite
		, Party
		, FString::Printf(TEXT("friendID: %s"), *UserId))
}

FString Lobby::SendAcceptInvitationRequest(FString const& PartyId
	, FString const& InvitationToken)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyJoin
		, Party
		, FString::Printf(TEXT("partyID: %s\ninvitationToken: %s"), *PartyId, *InvitationToken))
}

FString Api::Lobby::SendRejectInvitationRequest(FString const& PartyId
	, FString const& InvitationToken)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyReject
		, Party
		, FString::Printf(TEXT("partyID: %s\ninvitationToken: %s"), *PartyId, *InvitationToken))
}

FString Lobby::SendKickPartyMemberRequest(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyKick
		, Party
		, FString::Printf(TEXT("memberID: %s\n"), *UserId))
}

FString Lobby::SendPartyGenerateCodeRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyGenerateCode
		, Party
		, {})
}
FString Lobby::SendPartyGetCodeRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyGetCode
		, Party
		, {})
}

FString Lobby::SendPartyDeleteCodeRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyDeleteCode
		, Party
		, {})
}

FString Lobby::SendPartyJoinViaCodeRequest(FString const& partyCode)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyJoinViaCode
		, Party
		, FString::Printf(TEXT("partyCode: %s\n"), *partyCode))
}

FString Lobby::SendPartyPromoteLeaderRequest(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartyPromoteLeader
		, Party
		, FString::Printf(TEXT("newLeaderUserId: %s\n"), *UserId))
}

FString Lobby::SendNotificationToPartyMember(FString const& Topic
	, FString const& Payload) 
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(PartySendNotif
		, Party
		, FString::Printf(TEXT("topic: %s\npayload: %s"), *Topic, *Payload));
}

FAccelByteTaskWPtr Lobby::SetPartySizeLimit(FString const& PartyId
	, int32 Limit
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if(Limit <= 0)
	{
		FReport::Log("Party size limit should be above 0");
		return nullptr;
	}
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s/limit")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *PartyId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FAccelByteModelsPartySetLimitRequest Content;
	Content.Limit = Limit;

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

//-------------------------------------------------------------------------------------------------
// Presence
//-------------------------------------------------------------------------------------------------
FString Lobby::SendSetPresenceStatus(EAvailability Availability
	, FString const& Activity)
{
	FReport::Log(FString(__FUNCTION__));
	if (Activity.Contains("\n"))
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Unhandled character detected"));
		return "";
	}
	FString CopyActivity = MessageParser::EscapeString(Activity);
	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(SetUserPresence
		, Presence
		, FString::Printf(TEXT("availability: %s\nactivity: %s\n"), *FAccelByteUtilities::GetUEnumValueAsString(Availability).ToLower(), *CopyActivity))
}

FString Lobby::SendGetOnlineUsersRequest()
{
	return Lobby::SendGetOnlineFriendPresenceRequest();
}

FString Lobby::SendGetOnlineFriendPresenceRequest()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(GetAllFriendsStatus
		, Presence
		, {})
}

//-------------------------------------------------------------------------------------------------
// Notification
//-------------------------------------------------------------------------------------------------
void Lobby::GetAllAsyncNotification()
{	
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), TEXT("Lobby version 2.4.0 and above doesn't support this anymore"));

	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type: offlineNotificationRequest\nid:%s")
			, *FGuid::NewGuid().ToString(EGuidFormats::Digits));
		WebSocket->Send(Content);
		UE_LOG(LogAccelByteLobby, Verbose, TEXT("Get async notification (id=%s)"), *Content)
	}
}

//-------------------------------------------------------------------------------------------------
// Matchmaking
//-------------------------------------------------------------------------------------------------
FString Lobby::SendStartMatchmaking(FString const& GameMode
	, FString const& ServerName
	, FString const& ClientVersion
	, TArray<TPair<FString, float>> const& Latencies
	, TMap<FString, FString> const& PartyAttributes
	, TArray<FString> const& TempPartyUserIds
	, TArray<FString> const& ExtraAttributes)
{
	FMatchmakingOptionalParams Params;
	Params.ServerName = ServerName;
	Params.ClientVersion = ClientVersion;
	Params.Latencies = Latencies;
	Params.PartyAttributes = PartyAttributes;
	Params.TempPartyUserIds = TempPartyUserIds;
	Params.ExtraAttributes = ExtraAttributes;

	return SendStartMatchmaking(GameMode, Params);
}

FString Lobby::SendStartMatchmaking(FString const& GameMode
	, TArray<FString> const& TempPartyUserIds
	, FString const& ServerName
	, FString const& ClientVersion
	, TArray<TPair<FString, float>> const& Latencies
	, TMap<FString, FString> const& PartyAttributes
	, TArray<FString> const& ExtraAttributes)
{
	return SendStartMatchmaking(GameMode
		, ServerName
		, ClientVersion
		, Latencies
		, PartyAttributes
		, TempPartyUserIds
		, ExtraAttributes);
}

FString Lobby::SendStartMatchmaking(FString const& GameMode
	, TMap<FString, FString> const& PartyAttributes
	, FString const& ServerName
	, FString const& ClientVersion
	, TArray<TPair<FString, float>> const& Latencies
	, TArray<FString> const& TempPartyUserIds
	, TArray<FString> const& ExtraAttributes)
{
	return SendStartMatchmaking(GameMode
		, ServerName
		, ClientVersion
		, Latencies
		, PartyAttributes
		, TempPartyUserIds
		, ExtraAttributes);
}

FString Lobby::SendStartMatchmaking(FString const& GameMode
	, TMap<FString, FString> const& PartyAttributes
	, TArray<FString> const& TempPartyUserIds
	, FString const& ServerName
	, FString const& ClientVersion
	, TArray<TPair<FString, float>> const& Latencies
	, TArray<FString> const& ExtraAttributes)
{
	return SendStartMatchmaking(GameMode
		, ServerName
		, ClientVersion
		, Latencies
		, PartyAttributes
		, TempPartyUserIds
		, ExtraAttributes);
}

FString Lobby::GetServerLatenciesJsonStr(TArray<TPair<FString, float>> const& SelectedLatencies)
{
	FString ServerLatencies = TEXT("{");
	for (int i = 0; i < SelectedLatencies.Num(); i++)
	{
		ServerLatencies.Append(FString::Printf(TEXT("\"%s\":%.0f"), *SelectedLatencies[i].Key, SelectedLatencies[i].Value));
		if (i + 1 < SelectedLatencies.Num())
		{
			ServerLatencies.Append(TEXT(","));
		}
	}
	ServerLatencies.Append(TEXT("}"));
	return FString::Printf(TEXT("latencies: %s\n"), *ServerLatencies);	
}

FString Lobby::SendStartMatchmaking(FString const& GameMode
	, FMatchmakingOptionalParams const& OptionalParams)
{
	FReport::Log(FString(__FUNCTION__));
	FString Contents = FString::Printf(TEXT("gameMode: %s\n"), *GameMode);
		
	if (!OptionalParams.ServerName.IsEmpty())
	{
		Contents.Append(FString::Printf(TEXT("serverName: %s\n"), *OptionalParams.ServerName));
	}
		
	if (!OptionalParams.ClientVersion.IsEmpty())
	{
		Contents.Append(FString::Printf(TEXT("clientVersion: %s\n"), *OptionalParams.ClientVersion));
	}

	// Use Qos cached Latencies, or were valid Latencies already provided?
	const bool bUseCustomLatencies = OptionalParams.Latencies.Num() > 0;
	const TArray<TPair<FString, float>> SelectedLatencies = bUseCustomLatencies
		? OptionalParams.Latencies
		: FRegistry::Qos.GetCachedLatencies();

	// Validate Latencies (now initially set, immediately after login success): Important for multi-regioning.
	if (SelectedLatencies.Num() == 0)
	{
		// There are reports of weirdness when !Latencies && Multi-Regioning: This should also already be set.
		UE_LOG(LogAccelByteLobby, Warning, TEXT("No latencies data provided either from cached latencies or optional params. "
			"Empty latencies will be passed to the server (possibly problematic, if multi-regioning)."));
	}

	// Latencies should be set: Convert to json str
	if (SelectedLatencies.Num() > 0)
		Contents.Append(GetServerLatenciesJsonStr(SelectedLatencies));

	auto PartyAttribute = OptionalParams.PartyAttributes;
	if (OptionalParams.NewSessionOnly)
		PartyAttribute.Add("new_session_only", "true");

	FString partyAttributeSerialized = "";
	if (PartyAttribute.Num() > 0)
	{
		TArray<FString> keys;
		PartyAttribute.GetKeys(keys);
		for (int i = 0 ; i < keys.Num() ; i++)
		{
			FString key = keys[i];
			FString value = PartyAttribute[keys[i]];
			key = key.ReplaceCharWithEscapedChar();
			value = value.ReplaceCharWithEscapedChar();

			//Convert to this format [ "key":"value" ]
			partyAttributeSerialized.Append(FString::Printf(TEXT("\"%s\":\"%s\""), *key, *value));

			//If there's more attribute, append a delimiter
			if (i < keys.Num() - 1)
			{
				partyAttributeSerialized.Append(", ");
			}
		}
	}

	if (OptionalParams.SubGameModes.Num() > 0)
	{
		// if there is party attribute already, prepend a comma
		if (PartyAttribute.Num() > 0)
		{
			partyAttributeSerialized.Append(", ");
		}

		FString SubGameModeValue {"["};
		for (int i = 0; i < OptionalParams.SubGameModes.Num(); i++)
		{
			if(i > 0)
				SubGameModeValue.Append(", ");

			SubGameModeValue.Append(FString::Printf(TEXT("\"%s\""), *OptionalParams.SubGameModes[i]));
		}
		SubGameModeValue.Append("]");

		partyAttributeSerialized.Append(FString::Printf(TEXT("\"sub_game_mode\": %s"), *SubGameModeValue));
	}

	if (!partyAttributeSerialized.IsEmpty())
	{
		Contents.Append(FString::Printf(TEXT("partyAttributes: {%s}\n"), *partyAttributeSerialized));
	}

	if (OptionalParams.TempPartyUserIds.Num() > 0)
	{
		FString STempParty = TEXT("");
		for (int i = 0; i < OptionalParams.TempPartyUserIds.Num(); i++)
		{
			STempParty.Append(FString::Printf(TEXT("%s"), *OptionalParams.TempPartyUserIds[i]));
			if (i + 1 < OptionalParams.TempPartyUserIds.Num())
			{
				STempParty.Append(TEXT(","));
			}
		}
		
		Contents.Append(FString::Printf(TEXT("tempParty: %s\n"), *STempParty));
	}

	if (OptionalParams.ExtraAttributes.Num() > 0)
	{
		FString SExtraAttributes = TEXT("");
		for (int i = 0; i < OptionalParams.ExtraAttributes.Num(); i++)
		{
			SExtraAttributes.Append(FString::Printf(TEXT("%s"), *OptionalParams.ExtraAttributes[i]));
			if (i + 1 < OptionalParams.ExtraAttributes.Num())
			{
				SExtraAttributes.Append(TEXT(","));
			}
		}
		Contents.Append(FString::Printf(TEXT("extraAttributes: %s\n"), *SExtraAttributes));
	}

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(MatchmakingStart, Matchmaking, Contents)
}


FString Lobby::SendCancelMatchmaking(FString const& GameMode
	, bool IsTempParty)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(MatchmakingCancel
		, Matchmaking
		, FString::Printf(TEXT("gameMode: %s\nisTempParty: %s"), *GameMode, (IsTempParty ? TEXT("true") : TEXT("false"))))
}
	
FString Lobby::SendReadyConsentRequest(FString const& MatchId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(ReadyConsent
		, Matchmaking
		, FString::Printf(TEXT("matchId: %s\n"), *MatchId));
}

FString Lobby::SendRejectConsentRequest(FString const& MatchId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(RejectConsent
		, Matchmaking
		, FString::Printf(TEXT("matchId: %s\n"), *MatchId));
}

FString Lobby::RequestDS(FString const& SessionID
	, FString const& GameMode
	, FString const& ClientVersion
	, FString const& Region
	, FString const& Deployment
	, FString const& ServerName)
{
	FReport::Log(FString(__FUNCTION__));
	return SendRawRequest(LobbyRequest::CreateDS
		, Prefix::Matchmaking
		, FString::Printf(TEXT("matchId: %s\ngameMode: %s\nserverName: %s\nclientVersion: %s\nregion: %s\ndeployment: %s\n"), *SessionID, *GameMode, *ServerName, *ClientVersion, *Region, *Deployment));
}

//-------------------------------------------------------------------------------------------------
// Friends
//-------------------------------------------------------------------------------------------------
void Lobby::RequestFriend(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(RequestFriends
		, Friends
		, FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::RequestFriendByPublicId(FString const& PublicId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(RequestFriendsByPublicId
		, Friends
		, FString::Printf(TEXT("friendPublicId: %s"), *PublicId));
}

void Lobby::Unfriend(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(Unfriend
		, Friends
		, FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::ListOutgoingFriends()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(ListOutgoingFriends
		, Friends
		, {});
}

void Lobby::ListOutgoingFriendsWithTime()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(ListOutgoingFriendsWithTime
		, Friends
		, {});
}

void Lobby::CancelFriendRequest(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(CancelFriends
		, Friends
		, FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::ListIncomingFriends()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(ListIncomingFriends
		, Friends
		, {});
}

void Lobby::ListIncomingFriendsWithTime()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(ListIncomingFriendsWithTime
		, Friends
		, {});
}

void Lobby::AcceptFriend(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(AcceptFriends
		, Friends
		, FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::RejectFriend(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(RejectFriends
		, Friends
		, FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::LoadFriendsList()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(LoadFriendList
		, Friends
		, {});
}

void Lobby::GetFriendshipStatus(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(GetFriendshipStatus
		, Friends
		, FString::Printf(TEXT("friendId: %s"), *UserId));
}

FAccelByteTaskWPtr Lobby::BulkFriendRequest(FAccelByteModelsBulkFriendsRequest const& UserIds
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.FriendIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("FriendsId can't be empty for BulkFriendRequest"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/users/%s/add/bulk")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, UserIds, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::SyncThirdPartyFriends(FAccelByteModelsSyncThirdPartyFriendsRequest const& Request
	, THandler<TArray<FAccelByteModelsSyncThirdPartyFriendsResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Request.FriendSyncDetails.Num() < 1)
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Request parameter must have at least 1 element!"));
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Request parameter must have at least 1 element!"));
		return nullptr;
	}

	const bool bContainEmptyPlatformId = Request.FriendSyncDetails.ContainsByPredicate(
		[](FAccelByteModelsSyncThirdPartyFriendInfo const& Item)
		{
			return Item.PlatformId.IsEmpty();
		});
	
	if (bContainEmptyPlatformId)
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Request details cannot contains empty platform ID!"));
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Request details cannot contains empty platform ID!"));
		return nullptr;
	}

	const bool bContainNotLoggedInAndEmptyPlatformToken = Request.FriendSyncDetails.ContainsByPredicate(
		[](FAccelByteModelsSyncThirdPartyFriendInfo const& Item)
		{
			return Item.PlatformToken.IsEmpty() && !Item.IsLogin;
		});

	if (bContainNotLoggedInAndEmptyPlatformToken)
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Request details cannot contains empty platform Token when IsLogin is false!"));
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Request details cannot contains empty platform Token when IsLogin is false!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/sync/namespaces/%s/me")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FString JSONString;
	FAccelByteUtilities::TArrayUStructToJsonString(Request.FriendSyncDetails, JSONString);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, JSONString, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::SyncThirdPartyBlockList(FAccelByteModelsSyncThirdPartyBlockListRequest const& Request
	, THandler<TArray<FAccelByteModelsSyncThirdPartyBlockListResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Request.BlockListSyncDetails.Num() < 1)
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Request parameter must have at least 1 element!"));
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Request parameter must have at least 1 element!"));
		return nullptr;
	}

	const bool bContainEmptyPlatformId = Request.BlockListSyncDetails.ContainsByPredicate(
		[](FAccelByteModelsSyncThirdPartyBlockListInfo const& Item) 
		{
			return Item.PlatformId.IsEmpty();
		});
	
	if (bContainEmptyPlatformId)
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Request details cannot contains empty platform ID!"));
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Request details cannot contains empty platform ID!"));
		return nullptr;
	}

	FString LobbyServerHttp = SettingsRef.LobbyServerUrl.Replace(TEXT("wss://"), TEXT("https://"), ESearchCase::IgnoreCase);

	// #NOTE: lobby server URL has a trailing slash at the end, leading to an error when formatting the URL and sending to
	// a request. Chop off the slash at the end to avoid the error.
	LobbyServerHttp.RemoveFromEnd(TEXT("/"), ESearchCase::IgnoreCase);

	const FString Url = FString::Printf(TEXT("%s/sync/namespaces/%s/me/block")
		, *LobbyServerHttp
		, *CredentialsRef->GetNamespace());

	FString JSONString{};
	FAccelByteUtilities::TArrayUStructToJsonString(Request.BlockListSyncDetails, JSONString);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, JSONString, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::QueryFriendList(THandler<FAccelByteModelsQueryFriendListResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::QueryIncomingFriendRequest(THandler<FAccelByteModelsIncomingFriendRequests> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/incoming-time")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::QueryOutgoingFriendRequest(THandler<FAccelByteModelsOutgoingFriendRequests> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/outgoing-time")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::SendFriendRequest(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/request")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsFriendRequestByUserId Payload;
	Payload.FriendId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::SendFriendRequestByPublicId(FString const& PublicId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/request")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsFriendRequestByPublicId Payload;
	Payload.FriendPublicId = PublicId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::CancelFriendRequest(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/request/cancel")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsCancelFriendRequest Payload;
	Payload.FriendId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::AcceptFriendRequest(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/request/accept")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsAcceptFriendRequest Payload;
	Payload.FriendId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::RejectFriendRequest(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/request/reject")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsRejectFriendRequest Payload;
	Payload.FriendId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::GetFriendshipStatus(FString const& UserId
	, THandler<FAccelByteModelsFriendshipStatusResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/status/%s")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::Unfriend(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/me/unfriend")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsUnfriendRequest Payload;
	Payload.FriendId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::BlockPlayer(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/me/block")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsBlockUserRequest Payload;
	Payload.BlockedUserId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::UnblockPlayer(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/me/unblock")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsUnlockUserRequest Payload;
	Payload.UserId = UserId;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Payload, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::GetPartyData(FString const& PartyId
	, THandler<FAccelByteModelsPartyData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *PartyId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::BulkGetUserPresence(TArray<FString> const& UserIds
	, THandler<FAccelByteModelsBulkUserStatusNotif> const& OnSuccess
	, FErrorHandler const& OnError
	, bool CountOnly)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return nullptr;
	}

	TArray<FString> ProcessedUserIds{};
	TArray<FString> NotProcessedUserIds{};

	if (UserIds.Num() > UserIdsURLLimit)
	{
		ProcessedUserIds.Append(UserIds.GetData(), UserIdsURLLimit);
		NotProcessedUserIds.Append(UserIds.GetData() + UserIdsURLLimit, UserIds.Num() - UserIdsURLLimit);
	}
	else
	{
		ProcessedUserIds = UserIds;
	}

	TMultiMap<FString, FString> QueryParams = {
		{TEXT("countOnly"), CountOnly ? TEXT("true") : TEXT("false")},
		{ TEXT("userIds"), FString::Join(ProcessedUserIds, TEXT(",")) }
	};

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/presence/namespaces/%s/users/presence")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams
		, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda(
			[OnSuccess, NotProcessedUserIds](FAccelByteModelsBulkUserStatusNotif const& Result)
			{
				FAccelByteModelsBulkUserStatusNotif FinalResult = Result;
				FinalResult.NotProcessed = NotProcessedUserIds;
				OnSuccess.ExecuteIfBound(FinalResult);
			})
		, OnError);
}

FAccelByteTaskWPtr Lobby::BulkGetUserPresenceV2(TArray<FString> const& UserIds,
	THandler<FAccelByteModelsBulkUserStatusNotif> const& OnSuccess, FErrorHandler const& OnError, bool CountOnly)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return nullptr;
	}

	TArray<FString> ProcessedUserIds{};
	TArray<FString> NotProcessedUserIds{};

	if (UserIds.Num() > MaximumQueryLimit)
	{
		ProcessedUserIds.Append(UserIds.GetData(), MaximumQueryLimit);
		NotProcessedUserIds.Append(UserIds.GetData() + MaximumQueryLimit, UserIds.Num() - MaximumQueryLimit);
	}
	else
	{
		ProcessedUserIds = UserIds;
	}

	TMultiMap<FString, FString> QueryParams = {
		{TEXT("countOnly"), CountOnly ? TEXT("true") : TEXT("false")}
	};

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/presence/namespaces/%s/users/presence")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	TArray<TSharedPtr<FJsonValue>> UserIdsJsonValues;
	for (auto UserId : ProcessedUserIds)
	{
		UserIdsJsonValues.Add(MakeShareable(new FJsonValueString(UserId)));
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetArrayField("userIds", UserIdsJsonValues);

	FString Content = TEXT("");
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, Content
		, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda(
			[OnSuccess, NotProcessedUserIds](FAccelByteModelsBulkUserStatusNotif const& Result)
			{
				FAccelByteModelsBulkUserStatusNotif FinalResult = Result;
				FinalResult.NotProcessed = NotProcessedUserIds;
				OnSuccess.ExecuteIfBound(FinalResult);
			})
		, OnError);
}

FAccelByteTaskWPtr Lobby::GetPartyStorage(FString const& PartyId
	, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *PartyId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::GetListOfBlockedUsers(FString const& UserId
	, THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("Please use GetListOfBlockedUsers(const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, FErrorHandler const& OnError)"));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/%s/blocked")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::GetListOfBlockedUsers(THandler<FAccelByteModelsListBlockedUserResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/me/blocked")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::GetListOfBlockers(FString const& UserId
	, THandler<FAccelByteModelsListBlockerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("please use GetListOfBlockers(const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, FErrorHandler const& OnError)"));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/%s/blocked-by")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::GetListOfBlockers(THandler<FAccelByteModelsListBlockerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/me/blocked-by")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::SendNotificationToUser(FString const& SendToUserId
	, FAccelByteModelsFreeFormNotificationRequest const& Message
	, bool bAsync
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/notification/namespaces/%s/users/%s/freeform")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *SendToUserId);

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("async"), bAsync ? TEXT("true") : TEXT("false")}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, Message, OnSuccess, OnError);
}

FAccelByteTaskWPtr Lobby::WritePartyStorage(FString const& PartyId
	, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> const& PayloadModifier
	, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 RetryAttempt)
{
	TSharedPtr<PartyStorageWrapper> Wrapper = MakeShared<PartyStorageWrapper>();
	Wrapper->PartyId = PartyId;
	Wrapper->OnSuccess = OnSuccess;
	Wrapper->OnError = OnError;
	Wrapper->RemainingAttempt = RetryAttempt;
	Wrapper->PayloadModifier = PayloadModifier;
	return WritePartyStorageRecursive(Wrapper);
}

//-------------------------------------------------------------------------------------------------
// Block
//-------------------------------------------------------------------------------------------------
void Lobby::BlockPlayer(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(BlockPlayer
		, Block
		, FString::Printf(TEXT("userId: %s\nblockedUserId: %s\nnamespace: %s"), *CredentialsRef->GetUserId(), *UserId, *CredentialsRef->GetNamespace()));
}

void Lobby::UnblockPlayer(FString const& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(UnblockPlayer
		, Friends
		, FString::Printf(TEXT("userId: %s\nunblockedUserId: %s\nnamespace: %s"), *CredentialsRef->GetUserId(), *UserId, *CredentialsRef->GetNamespace()));
}

//-------------------------------------------------------------------------------------------------
// Signaling
//-------------------------------------------------------------------------------------------------
FString Lobby::SendSignalingMessage(FString const& UserId
	, FString const& Message)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::SignalingP2PNotif
		, Prefix::Signaling
		, FString::Printf(TEXT("destinationId: %s\nmessage: %s\n"), *UserId, *Message));
}

//-------------------------------------------------------------------------------------------------
// Session Attributes
//-------------------------------------------------------------------------------------------------
FString Lobby::SetSessionAttribute(FString const& Key
	, FString const& Value)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(SetSessionAttribute
		, Attribute
		, FString::Printf(TEXT("namespace: %s\nkey: %s\nvalue: %s"), *CredentialsRef->GetNamespace(), *Key, *Value));
}

FString Lobby::GetSessionAttribute(FString const& Key)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(GetSessionAttribute
		, Attribute
		, FString::Printf(TEXT("namespace: %s\nkey: %s"), *CredentialsRef->GetNamespace(), *Key));
}

FString Lobby::GetAllSessionAttribute()
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(GetAllSessionAttribute
		, Attribute
		, FString::Printf(TEXT("namespace: %s"), *CredentialsRef->GetNamespace()));
}

//-------------------------------------------------------------------------------------------------
// Refresh Token
//-------------------------------------------------------------------------------------------------

FString Lobby::RefreshToken(FString const& AccessToken)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE_RETURNED(RefreshToken
		, Token
		, FString::Printf(TEXT("token: %s"), *AccessToken));
}

//-------------------------------------------------------------------------------------------------
// Metrics
//-------------------------------------------------------------------------------------------------
void AccelByte::Api::Lobby::ChangeUserRegion(FString const& Region)
{
	FReport::Log(FString(__FUNCTION__));

	SEND_RAW_REQUEST_CACHED_RESPONSE(ChangeUserRegion
		, Metrics
		, FString::Printf(TEXT("region: %s"), *Region));
}

void AccelByte::Api::Lobby::OnTokenReceived(FString const& Token)
{
	if (Token == AccelByte::ErrorTokenRequestFailed)
	{
		OnConnectionError(TEXT("Failed to request entitlement token!"));
		return;
	}

	Connect(Token);
}

bool Lobby::Tick(float DeltaTime)
{
	if(NotificationQueue.IsEmpty())
	{
		return true;
	}

	FHandleLobbyMessageData Message;
	if(!NotificationQueue.Dequeue(Message))
	{
		if(NotificationQueue.IsLocked())
    	{
    		UE_LOG(LogAccelByteLobby, VeryVerbose, TEXT("Notification queue is locked, not broadcasting notification"));
    	}
		else
		{
			UE_LOG(LogAccelByteLobby, VeryVerbose, TEXT("Failed to dequeue notification, not broadcasting notification"));
		}
		return true;
	}

	switch (Message.Type)
	{
		case EHandleLobbyMessageDataType::Other:
			{
				HandleMessageNotif(Message.MessageType, Message.ParsedJsonString, Message.ParsedJsonObject, Message.bSkipConditioner);
				break;
			}
		case EHandleLobbyMessageDataType::V2Matchmaking:
			{
				DispatchV2MatchmakingMessageByTopic(Message.Topic, Message.Payload);
				break;
			}
		case EHandleLobbyMessageDataType::Session:
			{
				DispatchV2SessionMessageByTopic(Message.Topic, Message.Payload, Message.ParsedJsonString);
				break;
			}
		default:
			{
				UE_LOG(LogAccelByteLobby, Warning, TEXT("Unknown lobby message type : %s"), *FAccelByteUtilities::GetUEnumValueAsString(Message.Type));
				break;
			};
	}
	
	return true;
}

void Lobby::HandleLobbyMessageByType(FHandleLobbyMessageData const& MessageData)
{
	if (MessageData.MessageType.Equals(LobbyResponse::SessionNotif))
	{
		HandleV2SessionNotif(MessageData.ParsedJsonString, MessageData.bSkipConditioner);
	}
	else if (MessageData.MessageType.Contains(Suffix::Response))
	{
		HandleMessageResponse(MessageData.MessageType, MessageData.ParsedJsonString, MessageData.ParsedJsonObject, nullptr);
	}
	else if (MessageData.MessageType.Contains(Suffix::Notif))
	{
		HandleMessageNotif(MessageData.MessageType, MessageData.ParsedJsonString, MessageData.ParsedJsonObject, MessageData.bSkipConditioner);
	}
	else // undefined; not Response nor Notif
	{
		ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error cannot parse message. Neither a response nor a notif type. %s, Raw: %s"), *MessageData.MessageType, *MessageData.ParsedJsonString));
	}
}

TSharedPtr<FAccelByteKey> Lobby::LockNotifications()
{
	return NotificationQueue.LockQueue();
}

FAccelByteTaskWPtr Lobby::GetNotifications(THandler<FAccelByteModelsGetUserNotificationsResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FDateTime const& StartTime
	, FDateTime const& EndTime
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/notification/namespaces/{namespace}/me")
		, *SettingsRef.BaseUrl);

	TMap<FString, FString> QueryParams;

	if (StartTime > FDateTime{0})
	{
		QueryParams.Add(TEXT("startTime"), FString::FromInt(StartTime.ToUnixTimestamp()));
	}

	if (EndTime > FDateTime{0})
	{
		QueryParams.Add(TEXT("endTime"), FString::FromInt(EndTime.ToUnixTimestamp()));
	}

	if (Offset >= 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}

	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Lobby::UnbindEvent()
{
	FReport::Log(FString(__FUNCTION__));

	MessageNotifBroadcaster.Clear();
	PartyDataUpdateNotif.Unbind();

	UnbindFriendNotifEvents();
	UnbindFriendResponseEvents();

	UnbindPartyNotifEvents();
	UnbindPartyResponseEvents();

	UnbindMatchmakingNotifEvents();
	UnbindMatchmakingResponseEvents();

	UnbindChatNotifEvents();
	UnbindChatResponseEvents();

	UnbindBlockNotifEvents();
	UnbindBlockResponseEvents();

	UnbindSessionAttributeEvents();

	UnbindV2PartyEvents();
	UnbindV2GameSessionEvents();
	UnbindV2MatchmakingEvents();
	UnbindV2SessionGeneralEvents();
	
	UserBannedNotification.Unbind();
	UserUnbannedNotification.Unbind();

	ChangeUserRegionResponse.Unbind();
}

void Lobby::UnbindFriendNotifEvents()
{
	FReport::Log(FString(__FUNCTION__));

	FriendStatusNotif.Unbind();
	AcceptFriendsNotif.Unbind();
	RequestFriendsNotif.Unbind();
	UnfriendNotif.Unbind();
	CancelFriendsNotif.Unbind();
	RejectFriendsNotif.Unbind();
}

void Lobby::UnbindFriendResponseEvents()
{
	FReport::Log(FString(__FUNCTION__));

	SetUserPresenceResponse.Unbind();
	GetAllFriendsStatusResponse.Unbind();
	RequestFriendsResponse.Unbind();
	RequestFriendsByPublicIdResponse.Unbind();
	UnfriendResponse.Unbind();
	ListOutgoingFriendsResponse.Unbind();
	ListOutgoingFriendsWithTimeResponse.Unbind();
	CancelFriendsResponse.Unbind();
	ListIncomingFriendsResponse.Unbind();
	ListIncomingFriendsWithTimeResponse.Unbind();
	AcceptFriendsResponse.Unbind();
	RejectFriendsResponse.Unbind();
	LoadFriendListResponse.Unbind();
	GetFriendshipStatusResponse.Unbind();

	OnSetUserPresenceError.Unbind();
	OnGetAllFriendsStatusError.Unbind();
	OnRequestFriendsError.Unbind();
	OnUnfriendError.Unbind();
	OnListOutgoingFriendsError.Unbind();
	OnListOutgoingFriendsWithTimeError.Unbind();
	OnCancelFriendsError.Unbind();
	OnListIncomingFriendsError.Unbind();
	OnListIncomingFriendsWithTimeError.Unbind();
	OnAcceptFriendsError.Unbind();
	OnRejectFriendsError.Unbind();
	OnLoadFriendListError.Unbind();
	OnGetFriendshipStatusError.Unbind();
}

void Lobby::UnbindPartyNotifEvents()
{
	FReport::Log(FString(__FUNCTION__));

	PartyLeaveNotif.Unbind(); // This Unbind is DEPRECATED
	PartyMemberLeaveNotif.Unbind();
	PartyInviteNotif.Unbind();
	PartyGetInvitedNotif.Unbind();
	PartyJoinNotif.Unbind();
	PartyRejectNotif.Unbind();
	PartyKickNotif.Unbind();
	PartyNotif.Unbind();
	PartyMemberConnectNotif.Unbind();
	PartyMemberDisconnectNotif.Unbind();
}

void Lobby::UnbindPartyResponseEvents()
{
	FReport::Log(FString(__FUNCTION__));

	PartyInfoResponse.Unbind();
	PartyCreateResponse.Unbind();
	PartyLeaveResponse.Unbind();
	PartyInviteResponse.Unbind();
	PartyJoinResponse.Unbind();
	PartyRejectResponse.Unbind();
	PartyKickResponse.Unbind();
	PartyGetCodeResponse.Unbind();
	PartyGenerateCodeResponse.Unbind();
	PartyDeleteCodeResponse.Unbind();
	PartyJoinViaCodeResponse.Unbind();
	PartyPromoteLeaderResponse.Unbind();

	OnPartyInfoError.Unbind();
	OnPartyCreateError.Unbind();
	OnPartyLeaveError.Unbind();
	OnPartyInviteError.Unbind();
	OnPartyJoinError.Unbind();
	OnPartyRejectError.Unbind();
	OnPartyKickError.Unbind();
	OnPartyGenerateCodeError.Unbind();
	OnPartyGetCodeError.Unbind();
	OnPartyDeleteCodeError.Unbind();
	OnPartyJoinViaCodeError.Unbind();
	OnPartyPromoteLeaderError.Unbind();
}

void Lobby::UnbindMatchmakingNotifEvents()
{
	FReport::Log(FString(__FUNCTION__));

	MatchmakingNotif.Unbind();
	ReadyConsentNotif.Unbind();
	RejectConsentNotif.Unbind();
	RematchmakingNotif.Unbind();
	DsNotif.Unbind();
}

void Lobby::UnbindMatchmakingResponseEvents()
{
	FReport::Log(FString(__FUNCTION__));

	MatchmakingStartResponse.Unbind();
	MatchmakingCancelResponse.Unbind();
	ReadyConsentResponse.Unbind();
	RejectConsentResponse.Unbind();

	OnMatchmakingStartError.Unbind();
	OnMatchmakingCancelError.Unbind();
	OnReadyConsentError.Unbind();
	OnRejectConsentError.Unbind();
}

void Lobby::UnbindChatNotifEvents()
{
	FReport::Log(FString(__FUNCTION__));

	PersonalChatNotif.Unbind();
	PartyChatNotif.Unbind();
	ChannelChatNotif.Unbind();
}

void Lobby::UnbindChatResponseEvents()
{
	FReport::Log(FString(__FUNCTION__));

	PersonalChatResponse.Unbind();
	PartyChatResponse.Unbind();
	JoinDefaultChannelChatResponse.Unbind();
	ChannelChatResponse.Unbind();

	OnPersonalChatError.Unbind();
	OnPartyChatError.Unbind();
	OnJoinDefaultChannelChatError.Unbind();
	OnChannelChatError.Unbind();
}

void Lobby::UnbindBlockNotifEvents()
{
	FReport::Log(FString(__FUNCTION__));

	BlockPlayerNotif.Unbind();
	UnblockPlayerNotif.Unbind();
}
	
void Lobby::UnbindBlockResponseEvents()
{
	FReport::Log(FString(__FUNCTION__));

	OnBlockPlayerError.Unbind();
	OnUnblockPlayerError.Unbind();
}

void Lobby::UnbindSessionAttributeEvents()
{
	FReport::Log(FString(__FUNCTION__));

	OnGetSessionAttributeError.Unbind();
	OnSetSessionAttributeError.Unbind();
	OnGetAllSessionAttributeError.Unbind();
}

void Lobby::UnbindV2PartyEvents()
{
	FReport::Log(FString(__FUNCTION__));

	V2PartyInvitedNotif.Unbind();
	V2PartyInviteTimeoutNotif.Unbind();
	V2PartyJoinedNotif.Unbind();
	V2PartyKickedNotif.Unbind();
	V2PartyRejectedNotif.Unbind();
	V2PartyMembersChangedNotif.Unbind();
	V2PartyUpdatedNotif.Unbind();
}

void Lobby::UnbindV2GameSessionEvents()
{
	FReport::Log(FString(__FUNCTION__));

	V2GameSessionInvitedNotif.Unbind();
	V2GameSessionInviteTimeoutNotif.Unbind();
	V2GameSessionJoinedNotif.Unbind();
	V2GameSessionMembersChangedNotif.Unbind();
	V2GameSessionUpdatedNotif.Unbind();
	V2DSStatusChangedNotif.Unbind();
	V2GameSessionKickedNotif.Unbind();
	V2GameSessionRejectedNotif.Unbind();
	V2GameSessionEndedNotif.Unbind();
}

void Lobby::UnbindV2SessionGeneralEvents()
{
	FReport::Log(FString(__FUNCTION__));

	V2SessionStorageChangedNotif.Unbind();
}

void Lobby::UnbindV2MatchmakingEvents()
{
	FReport::Log(FString(__FUNCTION__));

	V2MatchmakingMatchFoundNotif.Unbind();
	V2MatchmakingExpiredNotif.Unbind();
	V2MatchmakingStartNotif.Unbind();
	V2MatchmakingCanceledNotif.Unbind();
}

void Lobby::OnNotificationSenderMessageReceived(FString const& Payload)
{
	UE_LOG(LogAccelByteLobby, VeryVerbose, TEXT("Received message from notification sender, message %s"), *Payload);
	OnMessage(Payload, true);
}
	
void Lobby::OnConnected()
{
	UE_LOG(LogAccelByteLobby, Log, TEXT("Connected"));

	ConnectSuccess.ExecuteIfBound();

	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		LobbyWPtr LobbyWeak = AsShared();
		AuthTokenSetDelegateHandle = MessagingSystemPtr->SubscribeToTopic(EAccelByteMessagingTopic::AuthTokenSet
			, FOnMessagingSystemReceivedMessage::CreateLambda(
				[LobbyWeak](FString const& Message)
				{
					auto LobbyApi = LobbyWeak.Pin();
					if (LobbyApi.IsValid())
					{
						FOauth2Token Token;
						FJsonObjectConverter::JsonObjectStringToUStruct(Message, &Token);
						LobbyApi->RefreshToken(Token.Access_token);
					}
				}));

		NotificationSenderListenerDelegate = FOnMessagingSystemReceivedMessage::CreateThreadSafeSP(AsShared()
			, &Lobby::OnNotificationSenderMessageReceived);
		NotificationSenderListenerDelegateHandle = MessagingSystemPtr->SubscribeToTopic(EAccelByteMessagingTopic::NotificationSenderLobby
			, NotificationSenderListenerDelegate);
		MessagingSystemPtr->SendMessage(EAccelByteMessagingTopic::LobbyConnected);
	}
}

void Lobby::OnConnectionError(FString const& Error)
{
	UE_LOG(LogAccelByteLobby, Warning, TEXT("Error connecting: %s"), *Error);
	ConnectError.ExecuteIfBound(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed), ErrorMessages::Default.at(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed)) + TEXT(" Reason: ") + Error);
}

void Lobby::OnClosed(int32 StatusCode
	, FString const& Reason
	, bool WasClean)
{
	// disconnect only if status code > 4000 and we don't receive a login ban,
	// other ban will try to reconnect the websocket
	bool bIsReconnecting {true};
	if ((StatusCode > 4000 && !(BanNotifReceived && BanType != EBanType::LOGIN)) || StatusCode == static_cast<int32>(EWebsocketErrorTypes::NormalClosure))
	{
		bIsReconnecting = false;
		Disconnect();
	}
	
	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::AuthTokenSet, AuthTokenSetDelegateHandle);
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::NotificationSenderLobby, NotificationSenderListenerDelegateHandle);
	}
	NotificationSenderListenerDelegate.Unbind();

	BanNotifReceived = false;
	BanType = EBanType::EMPTY;
	
	UE_LOG(LogAccelByteLobby, Log, TEXT("Connection closed. Status code: %d  Reason: %s Clean: %s Reconnecting: %s"),
		StatusCode, *Reason, WasClean? TEXT("true") : TEXT("false"), bIsReconnecting? TEXT("true") : TEXT("false"));
	
	if(!bIsReconnecting)
	{
		ConnectionClosed.ExecuteIfBound(StatusCode, Reason, WasClean);
	}
	else
	{
		Reconnecting.ExecuteIfBound(StatusCode, Reason, WasClean);
	}
}

void Lobby::OnReconnectAttempt(FReconnectAttemptInfo const& ReconnectAttemptInfo)
{
	ReconnectAttempted.Broadcast(ReconnectAttemptInfo);
}

void Lobby::OnMassiveOutage(FMassiveOutageInfo const& MassiveOutageInfo)
{
	MassiveOutage.Broadcast(MassiveOutageInfo);
}
	
FString Lobby::SendRawRequest(FString const& MessageType
	, FString const& MessageIDPrefix
	, FString const& CustomPayload)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		const FString MessageID = GenerateMessageID(MessageIDPrefix);
		FString Content = FString::Printf(TEXT("type: %s\nid: %s"), *MessageType, *MessageID);
		if (!CustomPayload.IsEmpty())
		{
			Content.Append(FString::Printf(TEXT("\n%s"), *CustomPayload));
		}
		WebSocket->Send(Content);
		UE_LOG(LogAccelByteLobby, Verbose, TEXT("Sending request: %s"), *Content);
		return MessageID;
	}
	return TEXT("");
}

FString Lobby::GenerateMessageID(FString const& Prefix) const
{
	return FString::Printf(TEXT("%s-%d"), *Prefix, FMath::RandRange(1000, 9999));
}
 
void Lobby::CreateWebSocket(FString const& Token)
{
	if(WebSocket.IsValid())
	{
		WebSocket.Reset();
	}

	TMap<FString, FString> Headers;
	Headers.Add(LobbySessionHeaderName, LobbySessionId.LobbySessionID);
	Headers.Add(LobbyEnvelopeStartHeaderName, LobbyEnvelopeStartHeaderValue);
	Headers.Add(LobbyEnvelopeEndHeaderName, LobbyEnvelopeEndHeaderValue);

	const FString PlatformId = LobbyCredentialsRef->GetAuthToken().Platform_id;
	const FString PlatformUserId = LobbyCredentialsRef->GetAuthToken().Platform_user_id;
	if (!PlatformId.IsEmpty())
	{
		Headers.Add(LobbyPlatformIdHeaderName, PlatformId);
		Headers.Add(LobbyPlatformUserIdHeaderName, PlatformUserId);
	}
	if(!Token.IsEmpty())
	{
		Headers.Add("Entitlement", Token);
	}
	FAccelByteUtilities::AppendModulesVersionToMap(Headers);

	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));

	WebSocket = AccelByteWebSocket::Create(*SettingsRef.LobbyServerUrl
		, TEXT("wss")
		, CredentialsRef.Get()
		, Headers
		, TSharedRef<IWebSocketFactory>(new FUnrealWebSocketFactory())
		, *this /*Reconnection Strategy*/
		, PingDelay
	);

	WebSocket->OnConnected().AddThreadSafeSP(AsShared(), &Lobby::OnConnected);
	WebSocket->OnMessageReceived().AddThreadSafeSP(AsShared(), &Lobby::OnMessage, false);
	WebSocket->OnConnectionError().AddThreadSafeSP(AsShared(), &Lobby::OnConnectionError);
	WebSocket->OnConnectionClosed().AddThreadSafeSP(AsShared(), &Lobby::OnClosed);
	WebSocket->OnReconnectAttempt().AddThreadSafeSP(AsShared(), &Lobby::OnReconnectAttempt);
	WebSocket->OnMassiveOutage().AddThreadSafeSP(AsShared(), &Lobby::OnMassiveOutage);
}

FString Lobby::LobbyMessageToJson(FString const& Message)
{
	bool bFirst = true;
	FString JsonString = TEXT("{");
	TArray<FString> Lines;
	Message.ParseIntoArray(Lines, TEXT("\n"), true);
	for (const auto& Line : Lines)
	{
		FString Name;
		FString Value;
		if (bFirst)
		{
			bFirst = false;
		}
		else
		{
			JsonString.Append(",");
		}
		Line.Split(": ", &Name, &Value);
		JsonString.Appendf(TEXT("\"%s\":"), *Name);

		Value.TrimStartAndEndInline();

		const TCHAR* Cursor = GetData(Value);

		if (Cursor == nullptr)
		{
			JsonString.Append("null");
			continue;
		}
		
		// make sure it's null terminated
		checkf(*(Cursor + Value.Len()) == 0, TEXT("Invalid value: '%s' length: %d"), *Value, Value.Len());

		// Array
		if (*Cursor == '[')
		{
			++Cursor;
			// skip spaces
			while (*Cursor && *Cursor == ' ') ++Cursor;
			bool bWasArrayParsed;
			FString JsonArrayString;
			// array of JSON object
			if (*Cursor == '{')
			{
				bWasArrayParsed = MessageParser::ParseArrayOfObject(Cursor, JsonArrayString);
			}
			// array of string
			else
			{
				bWasArrayParsed = MessageParser::ParseArrayOfString(Cursor, JsonArrayString);
			}
			
			if (bWasArrayParsed)
			{
				JsonString.Append(JsonArrayString);
			}
			else
			{
				// if the array was not parsed, set to empty array
				JsonString.Append("[]");
				UE_LOG(LogAccelByte, Warning, TEXT("[LobbyMessageToJson] Invalid array for field '%s', set to empty array"), *Name);
			}
		}
		// JSON
		else if (*Cursor == '{')
		{
			FString ObjectString;
			// only append valid object
			if (MessageParser::ParseObject(Cursor, ObjectString))
			{
				JsonString.Append(ObjectString);
			}
			else
			{
				JsonString.Append("{}");
				UE_LOG(LogAccelByte, Warning, TEXT("[LobbyMessageToJson] Invalid object for field '%s', set to empty object"), *Name);
			}
		}
		// JSON string
		else if (Value.StartsWith("\"") && Value.EndsWith("\""))
		{
			MessageParser::ParseString(Cursor, JsonString);
		}
		// everything else
		else 
		{
			FString Escaped = MessageParser::EscapeString(Value, true);
			JsonString.Append(Escaped);
		}
	}

	JsonString += TEXT("}");
	return JsonString;
}

/**
* Default Response handler as templated function (compile time checked)
* @see HandleNotif
*/
template <typename DataStruct, typename ResponseCallbackType, typename ErrorCallbackType>
void HandleResponse(FString const& MessageType
	, ResponseCallbackType const& ResponseCallback
	, ErrorCallbackType const& ErrorCallback
	, FString const& ParsedJsonString
	, FString const& ReceivedMessageType
	, int lobbyResponseCode
	, TMap<FString, FString> const& LobbyErrorMessages)
{
	ensure(ReceivedMessageType.Equals(MessageType));
	DataStruct Result;
	bool bSuccess = false;
	if (lobbyResponseCode == 0)
	{
		bSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &Result);
	}
	else
	{
		const FString ErrorCodeString = FString::FromInt(lobbyResponseCode);

		auto it = ErrorMessages::Default.find(lobbyResponseCode);

		FString ErrorCodeName;
		if(LobbyErrorMessages.Contains(ErrorCodeString))
		{
			ErrorCodeName = LobbyErrorMessages[ErrorCodeString];
		}
		else if(it != ErrorMessages::Default.cend())
		{
			ErrorCodeName = ErrorMessages::Default.at(lobbyResponseCode);
		}
		else
		{
			ErrorCodeName = TEXT("Error name not defined");
		}
		
		UE_LOG(LogAccelByteLobby, Warning, TEXT("%s returned non zero error code, code is %d with codename %s"), *MessageType, lobbyResponseCode, *ErrorCodeName);
		if (ErrorCallback.IsBound())
		{
			ErrorCallback.ExecuteIfBound(lobbyResponseCode, *ErrorCodeName);
			bSuccess = false;
		}
		else
		{
			Result.Code = FString::FromInt(lobbyResponseCode);
			bSuccess = true;
		}
	}

	if (bSuccess)
	{
		ResponseCallback.ExecuteIfBound(Result);
	}
}

/**
* @brief convenient switch case for RESPONSE context; will call the singular assigned delegate on success with no caching/message-Id matching
* @see CASE_NOTIF
**/
#define CASE_RESPONSE(MessageType, Model) \
	case (Response::MessageType) : \
		{ \
			HandleResponse<Model>(LobbyResponse::MessageType, MESSAGE_SUCCESS_HANDLER(MessageType), MESSAGE_ERROR_HANDLER(MessageType), ParsedJsonString, ReceivedMessageType, lobbyResponseCode, LobbyErrorMessages);\
			break; \
		} \

/**
* Some delegate types can be derived from MessageType
*/
#define DELEGATE_TYPE(MessageType) F ## MessageType ## Response

/**
* @brief convenient switch case for RESPONSE context; will cache the (MessageId,SuccessDelegate) pair
* @param DelegateType explicit param for cases/responses that has different format naming between MessageType and DelegateType
*/
#define CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE(MessageType, Model, DelegateType) \
	case (Response::MessageType) : \
		{ \
			if(ID_RESPONSE_MAP(MessageType).Contains(ReceivedMessageId)) \
			{ \
				DelegateType* IdResponse = ID_RESPONSE_MAP(MessageType).Find(ReceivedMessageId); \
				check(IdResponse); \
				HandleResponse<Model>(LobbyResponse::MessageType, *IdResponse, MESSAGE_ERROR_HANDLER(MessageType), ParsedJsonString, ReceivedMessageType, lobbyResponseCode, LobbyErrorMessages);\
				ID_RESPONSE_MAP(MessageType).Remove(ReceivedMessageId); \
			} \
			else \
			{ \
				UE_LOG(LogAccelByteLobby, Warning, TEXT("Message Id response not found! type %s, id %s"), *LobbyResponse::MessageType, *ReceivedMessageId); \
			} \
			break; \
		} \

//@brief convenient switch case for RESPONSE context; will cache the (MessageId,SuccessDelegate) pair
#define CASE_RESPONSE_MESSAGE_ID(MessageType, Model) CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE(MessageType, Model, DELEGATE_TYPE(MessageType))

void Lobby::HandleMessageResponse(FString const& ReceivedMessageType
	, FString const& ParsedJsonString
	, TSharedPtr<FJsonObject> const& ParsedJsonObj
	, TSharedPtr<FLobbyMessageMetaData> const& MessageMeta = nullptr)
{
	int lobbyResponseCode{0};
	FString ReceivedMessageId{};

	if(MessageMeta.IsValid())
	{
		lobbyResponseCode = FCString::Atoi(*MessageMeta->Code);
		ReceivedMessageId = MessageMeta->Id;
	}
	else
	{
		lobbyResponseCode = ParsedJsonObj->GetIntegerField(TEXT("code"));
		ReceivedMessageId = ParsedJsonObj->GetStringField(TEXT("id"));
	}

	Response ResponseEnum = Response::Invalid_Response;
	Response* ResponseEnumPointer = ResponseStringEnumMap.Find(ReceivedMessageType);
	if (ResponseEnumPointer)
	{
		ResponseEnum = *ResponseEnumPointer;
	}

	switch (ResponseEnum)
	{
		// Party
		CASE_RESPONSE_MESSAGE_ID(PartyInfo			, FAccelByteModelsInfoPartyResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyCreate		, FAccelByteModelsCreatePartyResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyLeave			, FAccelByteModelsLeavePartyResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyInvite		, FAccelByteModelsPartyInviteResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyJoin			, FAccelByteModelsPartyJoinResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyReject		, FAccelByteModelsPartyRejectResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyKick			, FAccelByteModelsKickPartyMemberResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyGetCode		, FAccelByteModelsPartyGetCodeResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyGenerateCode	, FAccelByteModelsPartyGenerateCodeResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyDeleteCode	, FAccelByteModelsPartyDeleteCodeResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyJoinViaCode	, FAccelByteModelsPartyJoinResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyPromoteLeader	, FAccelByteModelsPartyPromoteLeaderResponse);
		CASE_RESPONSE_MESSAGE_ID(PartySendNotif		, FAccelByteModelsPartySendNotifResponse);
		// Chat
		CASE_RESPONSE_MESSAGE_ID(PersonalChat	, FAccelByteModelsPersonalMessageResponse);
		CASE_RESPONSE_MESSAGE_ID(PartyChat		, FAccelByteModelsPartyMessageResponse);
		CASE_RESPONSE_MESSAGE_ID(ChannelChat	, FAccelByteModelsChannelMessageResponse);
		case (Response::JoinChannelChat):
		{
			FAccelByteModelsJoinDefaultChannelResponse Result;
			if (const bool bParseSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &Result))
			{
				ChannelSlug = Result.ChannelSlug;
				JoinDefaultChannelChatResponse.ExecuteIfBound(Result);
			}
			break;
		}
		// Presence
		CASE_RESPONSE_MESSAGE_ID(SetUserPresence	, FAccelByteModelsSetOnlineUsersResponse);
		CASE_RESPONSE_MESSAGE_ID(GetAllFriendsStatus, FAccelByteModelsGetOnlineUsersResponse);
		// Matchmaking
		CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE(MatchmakingStart	, FAccelByteModelsMatchmakingResponse, FMatchmakingResponse);
		CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE(MatchmakingCancel, FAccelByteModelsMatchmakingResponse, FMatchmakingResponse);
		CASE_RESPONSE_MESSAGE_ID(ReadyConsent, FAccelByteModelsReadyConsentRequest);
		CASE_RESPONSE_MESSAGE_ID(RejectConsent, FAccelByteModelsRejectConsentRequest);
		// Custom Game
		CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE(CreateDS			, FAccelByteModelsLobbyBaseResponse, FBaseResponse);
		// Friends
		CASE_RESPONSE_MESSAGE_ID(RequestFriends		, FAccelByteModelsRequestFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE(RequestFriendsByPublicId		, FAccelByteModelsRequestFriendsResponse, FRequestFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID(Unfriend			, FAccelByteModelsUnfriendResponse);
		CASE_RESPONSE_MESSAGE_ID(ListOutgoingFriends, FAccelByteModelsListOutgoingFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID(ListOutgoingFriendsWithTime, FAccelByteModelsListOutgoingFriendsWithTimeResponse);
		CASE_RESPONSE_MESSAGE_ID(CancelFriends		, FAccelByteModelsCancelFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID(ListIncomingFriends, FAccelByteModelsListIncomingFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID(ListIncomingFriendsWithTime, FAccelByteModelsListIncomingFriendsWithTimeResponse);
		CASE_RESPONSE_MESSAGE_ID(AcceptFriends		, FAccelByteModelsAcceptFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID(RejectFriends		, FAccelByteModelsRejectFriendsResponse);
		CASE_RESPONSE_MESSAGE_ID(LoadFriendList		, FAccelByteModelsLoadFriendListResponse);
		case (Response::GetFriendshipStatus):
		{
			FAccelByteModelsGetFriendshipStatusStringResponse StringResult;
			bool bParseSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &StringResult);
			if (bParseSuccess)
			{
				FAccelByteModelsGetFriendshipStatusResponse Result;
				Result.Type = StringResult.Type;
				Result.Id = StringResult.Id;
				Result.Code = StringResult.Code;
				Result.friendshipStatus = (ERelationshipStatusCode)FCString::Atoi(*StringResult.friendshipStatus);
				Result.FriendId = StringResult.FriendId;
				GetFriendshipStatusResponse.ExecuteIfBound(Result);
			}
			break;
		}
		// Block
		CASE_RESPONSE_MESSAGE_ID(BlockPlayer	, FAccelByteModelsBlockPlayerResponse);
		CASE_RESPONSE_MESSAGE_ID(UnblockPlayer	, FAccelByteModelsUnblockPlayerResponse);
		// Session Attribute
		CASE_RESPONSE_MESSAGE_ID(SetSessionAttribute	, FAccelByteModelsSetSessionAttributesResponse);
		CASE_RESPONSE_MESSAGE_ID(GetSessionAttribute	, FAccelByteModelsGetSessionAttributesResponse);
		CASE_RESPONSE_MESSAGE_ID(GetAllSessionAttribute	, FAccelByteModelsGetAllSessionAttributesResponse);
		CASE_RESPONSE_MESSAGE_ID(RefreshToken			, FAccelByteModelsRefreshTokenResponse);
		// Metrics
		CASE_RESPONSE_MESSAGE_ID(ChangeUserRegion, FAccelByteModelsChangeUserRegionResponse);
		default:
		{
			ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error; Detected of type Response but no specific handler case assigned. %s, Raw: %s"), *ReceivedMessageType, *ParsedJsonString));
			break;
		}

	}
}

FString Lobby::GenerateUnbanScheduleKey(FAccelByteModelsUserBannedNotification const& Result)
{
	return FString::Printf(TEXT("%s-%s"), *Result.UserId, *FAccelByteUtilities::GetUEnumValueAsString(Result.Ban));
}

void Lobby::SetUnbanSchedule(FAccelByteModelsUserBannedNotification const& Result)
{
	const int64 UtcNow = FDateTime::UtcNow().ToUnixTimestamp();
	FDateTime BanEndDate{0};
	FDateTime::ParseIso8601(*Result.EndDate, BanEndDate);
	float BanDuration = BanEndDate.ToUnixTimestamp() - UtcNow;
	double ScheduledTime = FPlatformTime::Seconds() + BanDuration;

	FString Key = GenerateUnbanScheduleKey(Result);

	FScopeLock Lock(&UnbanScheduleLock);
	FUnbanScheduleRef* Schedule = UnbanSchedules.Find(Key);
	if (!Schedule || ScheduledTime < (*Schedule)->ScheduledTime)
	{
		if (Schedule)
		{
			FTickerAlias::GetCoreTicker().RemoveTicker((*Schedule)->DelegateHandle);
			UnbanSchedules.Remove(Key);
		}

		LobbyWPtr LobbyWeak = AsShared();
		TSharedRef<FAccelByteModelsUserBannedNotification> Data = MakeShared<FAccelByteModelsUserBannedNotification>(Result);
		FUnbanScheduleRef NewSchedule = MakeShared<FUnbanSchedule>();
		NewSchedule->DelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
			[LobbyWeak, Data, ScheduledTime](float DeltaTime)
			{
				auto LobbyApi = LobbyWeak.Pin();
				double CurrentTime = FPlatformTime::Seconds();
				if (!LobbyApi.IsValid() || CurrentTime >= ScheduledTime)
				{
					if (LobbyApi.IsValid())
					{
						Data->Enable = false;
						LobbyApi->UserUnbannedNotification.ExecuteIfBound(*Data);
					}
					return false;
				}
				return true;
			})
			, 0.2f);
		NewSchedule->ScheduledTime = ScheduledTime;
		UnbanSchedules.Add(Key, NewSchedule);
	}
}

void Lobby::RemoveUnbanSchedule(FAccelByteModelsUserBannedNotification const& Result)
{
	const FString Key = GenerateUnbanScheduleKey(Result);

	FScopeLock Lock(&UnbanScheduleLock);
	FUnbanScheduleRef* Schedule = UnbanSchedules.Find(Key);
	if(Schedule != nullptr)
	{
		FTickerAlias::GetCoreTicker().RemoveTicker((*Schedule)->DelegateHandle);
		UnbanSchedules.Remove(Key);
	}
}

#undef MESSAGE_SUCCESS_HANDLER
#undef MESSAGE_ERROR_HANDLER
#undef CASE_RESPONSE
#undef CASE_RESPONSE_MESSAGE_ID
#undef ID_RESPONSE_MAP
#undef DELEGATE_TYPE
#undef CASE_RESPONSE_MESSAGE_ID_DELEGATE_TYPE

/**
* Default Notif handler as templated function (compile time checked)
* @see HandleResponse
*/
template <typename DataStruct, typename ResponseCallbackType>
void HandleNotif(FString const& MessageType
	, ResponseCallbackType const& ResponseCallback
	, FString const& ParsedJsonString
	, FString const& ReceivedMessageType)
{
	ensure(ReceivedMessageType.Equals(MessageType));
	DataStruct Result;
	if (const bool bSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &Result)) {
		ResponseCallback.ExecuteIfBound(Result);
	}

};

/**
*  Programmer-friendly macro for switch case NOTIF scenario
* @see CASE_RESPONSE
*
**/
#define CASE_NOTIF(MessageType, Model) \
	case (Notif::MessageType) : \
		{ \
			HandleNotif<Model>(LobbyResponse::MessageType, MessageType, ParsedJsonString, ReceivedMessageType);\
			break; \
		} \
		
template <typename PayloadType>
bool MessageNotifJsonDeserialize(FString const& Payload, PayloadType& OutResult)
{
	if(FAccelByteJsonConverter::JsonObjectStringToUStruct(Payload, &OutResult))
	{
		return true;
	}
	else
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Unable to json parse payload from notification\n%s"), *Payload);
	}
	return false;
}

template <typename PayloadType, typename CallbackType>
void DispatchV2JsonNotif(FString const& Payload
	, CallbackType const& ResponseCallback)
{
	FString PayloadJsonString;
	if(!FBase64::Decode(Payload, PayloadJsonString))
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Unable to decode payload from notification\n%s"), *Payload);
		return;
	}

	UE_LOG(LogAccelByteLobby, Log, TEXT("MPv2 notif json:\n%s"), *PayloadJsonString);
	
	PayloadType Result;
	if (MessageNotifJsonDeserialize(PayloadJsonString, Result))
	{
		ResponseCallback.ExecuteIfBound(Result);
	}
}

template <typename PayloadType, typename CallbackType>
void DispatchMulticastV2JsonNotif(FString const& Payload, CallbackType const& ResponseCallback)
{
	FString PayloadJsonString;
	if(!FBase64::Decode(Payload, PayloadJsonString))
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Unable to decode payload from notification\n%s"), *Payload);
		return;
	}

	UE_LOG(LogAccelByteLobby, Log, TEXT("MPv2 notif json:\n%s"), *PayloadJsonString);
	
	PayloadType Result;
	if (MessageNotifJsonDeserialize(PayloadJsonString, Result))
	{
		ResponseCallback.Broadcast(Result);
	}
}

void Lobby::HandleV2SessionNotif(FString const& ParsedJsonString, bool bSkipConditioner)
{
	FAccelByteModelsSessionNotificationMessage Notif;
	if (FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &Notif) == false)
	{
		UE_LOG(LogAccelByteLobby, Log, TEXT("Cannot deserialize sessionMessageNotif to struct\nNotification: %s"), *ParsedJsonString);
		return;
	}

	if(!bSkipConditioner && NetworkConditioner.CalculateFail(Notif.Topic))
	{
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Dropped Session v2 notification %s"), *Notif.Topic);
		return;
	}

	if (TryBufferNotification(ParsedJsonString))
	{
		return;
	}

	FJsonObjectWrapper JsonWrapper;
	if(JsonWrapper.JsonObjectFromString(ParsedJsonString) && JsonWrapper.JsonObject->HasField(TEXT("SequenceID")))
	{
		FHandleLobbyMessageData Message;
		Message.Topic = Notif.Topic;
		Message.Payload = Notif.Payload;
		Message.ParsedJsonString = ParsedJsonString;
		Message.Type = EHandleLobbyMessageDataType::Session;
		
		NotificationQueue.Enqueue(Message);
		return;
	}

	DispatchV2SessionMessageByTopic(Notif.Topic, Notif.Payload, ParsedJsonString);
}

void Lobby::DispatchV2SessionMessageByTopic(FString const& Topic, FString const& Payload, FString const& ParsedJsonString)
{
	switch(FAccelByteUtilities::GetUEnumValueFromString<EV2SessionNotifTopic>(Topic))
	{
		case EV2SessionNotifTopic::OnPartyInvited:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartyInvitedEvent>(Payload, V2PartyInvitedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyInviteTimeout:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartyInviteTimeoutEvent>(Payload, V2PartyInviteTimeoutNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyMembersChanged:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartyMembersChangedEvent>(Payload, V2PartyMembersChangedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyJoined:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartyUserJoinedEvent>(Payload, V2PartyJoinedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyRejected:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartyUserRejectedEvent>(Payload, V2PartyRejectedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyKicked:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartyUserKickedEvent>(Payload, V2PartyKickedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyCreated:
		{
			DispatchMulticastV2JsonNotif<FAccelByteModelsV2PartyCreatedEvent>(Payload, V2PartyCreatedNotif);
		}
		case EV2SessionNotifTopic::OnSessionInvited:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionUserInvitedEvent>(Payload, V2GameSessionInvitedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnGameSessionInviteTimeout:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionUserInviteTimeoutEvent>(Payload, V2GameSessionInviteTimeoutNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionJoined:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionUserJoinedEvent>(Payload, V2GameSessionJoinedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionMembersChanged:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionMembersChangedEvent>(Payload, V2GameSessionMembersChangedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionKicked:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionUserKickedEvent>(Payload, V2GameSessionKickedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionRejected:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionUserRejectedEvent>(Payload, V2GameSessionRejectedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnDSStatusChanged:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2DSStatusChangedNotif>(Payload, V2DSStatusChangedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyUpdated:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2PartySession>(Payload, V2PartyUpdatedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnPartyCancelled:
		{
			DispatchMulticastV2JsonNotif<FAccelByteModelsV2PartyInviteCanceledEvent>(Payload, V2PartyInviteCanceledNotif);
			break;
		}
		case EV2SessionNotifTopic::OnGameSessionUpdated:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSession>(Payload, V2GameSessionUpdatedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionStorageChanged:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2SessionStorageChangedEvent>(Payload, V2SessionStorageChangedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionEnded:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2GameSessionEndedEvent>(Payload, V2GameSessionEndedNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionJoinedSecret:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2SessionJoinedSecret>(Payload, V2SessionJoinedSecretNotif);
			break;
		}
		case EV2SessionNotifTopic::OnGameSessionInviteCancelled:
		{
			DispatchMulticastV2JsonNotif<FAccelByteModelsV2GameSessionInviteCanceledEvent>(Payload, V2GameSessionInviteCanceledNotif);
			break;
		}
		case EV2SessionNotifTopic::OnSessionNativePlatformSynced:
		{
			DispatchV2JsonNotif<FAccelByteModelsV2NativeSessionSyncNotif>(Payload, V2NativeSessionSyncNotif);
			break;
		}
		default: UE_LOG(LogAccelByteLobby, Log, TEXT("Unknown session notification topic\nNotification: %s"), *ParsedJsonString);
	}
}

void Lobby::HandleV2MatchmakingNotif(const FAccelByteModelsNotificationMessage& Message, bool bSkipConditioner)
{
	UE_LOG(LogAccelByteLobby, Log, TEXT("Received MMv2 notification with topic : %s"), *Message.Topic);

	if(!bSkipConditioner && NetworkConditioner.CalculateFail(Message.Topic))
	{
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Dropped Matchmaking v2 notification %s"), *Message.Topic);
		return;
	}

	FString NotifJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(Message, NotifJsonString);
	if (TryBufferNotification(NotifJsonString))
	{
		return;
	}

	FJsonObjectWrapper JsonWrapper;
	if(JsonWrapper.JsonObjectFromString(NotifJsonString) && JsonWrapper.JsonObject->HasField(TEXT("SequenceID")))
	{
		FHandleLobbyMessageData EnqueueMessage;
		EnqueueMessage.Topic = Message.Topic;
		EnqueueMessage.Payload = Message.Payload;
		EnqueueMessage.Type = EHandleLobbyMessageDataType::V2Matchmaking;
		
		NotificationQueue.Enqueue(EnqueueMessage);
		return;
	}
	
	DispatchV2MatchmakingMessageByTopic(Message.Topic, Message.Payload);
}

void Lobby::DispatchV2MatchmakingMessageByTopic(FString const& Topic, FString const& Payload) const
{
	switch (FAccelByteUtilities::GetUEnumValueFromString<EV2MatchmakingNotifTopic>(Topic))
	{
		case EV2MatchmakingNotifTopic::OnMatchFound:
			{
				DispatchV2JsonNotif<FAccelByteModelsV2MatchFoundNotif>(Payload, V2MatchmakingMatchFoundNotif);
				break;
			}
		case EV2MatchmakingNotifTopic::OnMatchmakingStarted:
			{
				DispatchV2JsonNotif<FAccelByteModelsV2StartMatchmakingNotif>(Payload, V2MatchmakingStartNotif);
				break;
			}
		case EV2MatchmakingNotifTopic::OnMatchmakingTicketExpired:
			{
				DispatchV2JsonNotif<FAccelByteModelsV2MatchmakingExpiredNotif>(Payload, V2MatchmakingExpiredNotif);
				break;
			}
		case EV2MatchmakingNotifTopic::OnMatchmakingTicketCanceled:
			{
				DispatchV2JsonNotif<FAccelByteModelsV2MatchmakingCanceledNotif>(Payload, V2MatchmakingCanceledNotif);
				break;
			}
		default: UE_LOG(LogAccelByteLobby, Warning, TEXT("Unknown matchmaking v2 notification topic : %s"), *Topic);
	}
}

void Lobby::HandleOneTimeCodeLinkedNotif(const FAccelByteModelsNotificationMessage& Message)
{
	FAccelByteModelsOneTimeCodeLinked Result;
	if (MessageNotifJsonDeserialize(Message.Payload, Result))
	{
		OneTimeCodeLinkedNotif.Broadcast(Result);
	}
}

void Lobby::InitializeV2MatchmakingNotifTopics()
{
	MatchmakingV2NotifTopics = {
		EV2MatchmakingNotifTopic::OnMatchFound,
		EV2MatchmakingNotifTopic::OnMatchmakingStarted,
		EV2MatchmakingNotifTopic::OnMatchmakingTicketExpired,
		EV2MatchmakingNotifTopic::OnMatchmakingTicketCanceled,
	};
}

void Lobby::HandleMessageNotif(FString const& ReceivedMessageType
	, FString const& ParsedJsonString
	, TSharedPtr<FJsonObject> const& ParsedJsonObj
	, bool bSkipConditioner)
{
	Notif NotifEnum = Notif::Invalid_Notif;
	if (const Notif* NotifEnumPointer = NotifStringEnumMap.Find(ReceivedMessageType))
	{
		NotifEnum = *NotifEnumPointer;
	}

	switch (NotifEnum)
	{
		// Party
		case (Notif::ConnectedNotif):
		{
			FAccelByteModelsLobbySessionId SessionId;
			bool bSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &SessionId);
			if (bSuccess)
			{
				LobbySessionId = SessionId;
				WebSocket->UpdateUpgradeHeaders(LobbySessionHeaderName, LobbySessionId.LobbySessionID);
			}
			break;
		}
		CASE_NOTIF(DisconnectNotif, FAccelByteModelsDisconnectNotif);
		case (Notif::PartyMemberLeaveNotif):
		{
				FAccelByteModelsLeavePartyNotice PartyLeaveResult;
				bool bSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &PartyLeaveResult);
				if (bSuccess)
				{
					if (PartyLeaveNotif.IsBound())
					{
						PartyLeaveNotif.ExecuteIfBound(PartyLeaveResult);
					}
					else
					{
						PartyMemberLeaveNotif.ExecuteIfBound(PartyLeaveResult);
					}
				}
				break;
		}
		CASE_NOTIF(PartyInviteNotif, FAccelByteModelsInvitationNotice);
		CASE_NOTIF(PartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice);
		CASE_NOTIF(PartyJoinNotif, FAccelByteModelsPartyJoinNotice);
		CASE_NOTIF(PartyRejectNotif, FAccelByteModelsPartyRejectNotice);
		CASE_NOTIF(PartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice);
		case Notif::PartyNotif:
		{
			const FString PayloadKey(TEXT("payload"));
			const TSharedPtr<FJsonObject>* ObjectValue;
			if (ParsedJsonObj->TryGetObjectField(PayloadKey, ObjectValue))
			{
				FString StringValue;
				TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&StringValue, /*Indent=*/0);
				if (!FJsonSerializer::Serialize(ObjectValue->ToSharedRef(), JsonWriter))
				{
					UE_LOG(LogAccelByteLobby, Log, TEXT("PartyNotif: unable to serialize payload as string: %s"), *ParsedJsonString);
					return;
				}
				ParsedJsonObj->SetStringField(PayloadKey, StringValue);
			}

			FAccelByteModelsPartyNotif Notification;
			if (!FJsonObjectConverter::JsonObjectToUStruct(ParsedJsonObj.ToSharedRef(), &Notification, 0, 0))
			{
				UE_LOG(LogAccelByteLobby, Log, TEXT("PartyNotif: unable to deserialize to struct: %s"), *ParsedJsonString);
				return;
			}
			PartyNotif.ExecuteIfBound(Notification);
			break;
		}
		CASE_NOTIF(PartyDataUpdateNotif, FAccelByteModelsPartyDataNotif);
		CASE_NOTIF(PartyMemberConnectNotif, FAccelByteModelsPartyMemberConnectionNotice);
		CASE_NOTIF(PartyMemberDisconnectNotif, FAccelByteModelsPartyMemberConnectionNotice);
		// Chat
		CASE_NOTIF(PersonalChatNotif, FAccelByteModelsPersonalMessageNotice);
		CASE_NOTIF(PartyChatNotif, FAccelByteModelsPartyMessageNotice);
		CASE_NOTIF(ChannelChatNotif, FAccelByteModelsChannelMessageNotice);
		// Presence
		CASE_NOTIF(FriendStatusNotif, FAccelByteModelsUsersPresenceNotice);
		// Notification
		case(Notif::MessageNotif):
		{
			FAccelByteModelsNotificationMessage NotificationMessage;
			NotificationMessage.Type = ReceivedMessageType;
			FString PayloadKey = "payload";
			if (ParsedJsonObj->HasTypedField<EJson::Object>(PayloadKey))
			{
				TSharedPtr<FJsonObject> PayloadObject = ParsedJsonObj->GetObjectField(PayloadKey);
				if (PayloadObject == nullptr)
				{
					UE_LOG(LogAccelByteLobby, Log, TEXT("Cannot deserialize the whole MessageNotif to the struct\nNotification: %s"), *ParsedJsonString);
					return;
				}

				FString PayloadString;
				TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&PayloadString);
				if (FJsonSerializer::Serialize(PayloadObject.ToSharedRef(), JsonWriter) == false)
				{
					UE_LOG(LogAccelByteLobby, Log, TEXT("Cannot serialize payload field from MessageNotif\nNotification: %s"), *ParsedJsonString);
					return;
				}
				ParsedJsonObj->RemoveField(PayloadKey);
				ParsedJsonObj->SetStringField(PayloadKey, PayloadString);
				if (FJsonObjectConverter::JsonObjectToUStruct(ParsedJsonObj.ToSharedRef(), &NotificationMessage, 0, 0) == false)
				{
					UE_LOG(LogAccelByteLobby, Log, TEXT("Cannot deserialize the whole MessageNotif to the struct\nNotification: %s"), *ParsedJsonString);
					return;
				}
			}
			else
			{
				if (FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &NotificationMessage) == false)
				{
					UE_LOG(LogAccelByteLobby, Log, TEXT("Cannot deserialize the whole MessageNotif to the struct\nNotification: %s"), *ParsedJsonString);
					return;
				}
			}

			EV2MatchmakingNotifTopic MMNotifEnum = FAccelByteUtilities::GetUEnumValueFromString<EV2MatchmakingNotifTopic>(NotificationMessage.Topic);
			if(MMNotifEnum != EV2MatchmakingNotifTopic::Invalid && MatchmakingV2NotifTopics.Contains(MMNotifEnum))
			{
				HandleV2MatchmakingNotif(NotificationMessage, bSkipConditioner);
				break;
			}

			if (NotificationMessage.Topic.Equals(TEXT("oneTimeCodeLinked")))
			{
				HandleOneTimeCodeLinkedNotif(NotificationMessage);
				break;
			}
				
			MessageNotifBroadcaster.Broadcast(NotificationMessage);
			break;
		}
		// Matchmaking
		CASE_NOTIF(MatchmakingNotif		, FAccelByteModelsMatchmakingNotice);
		CASE_NOTIF(ReadyConsentNotif	, FAccelByteModelsReadyConsentNotice);
		CASE_NOTIF(RejectConsentNotif	, FAccelByteModelsRejectConsentNotice);
		CASE_NOTIF(RematchmakingNotif	, FAccelByteModelsRematchmakingNotice);
		CASE_NOTIF(DsNotif				, FAccelByteModelsDsNotice);
		// Friends + Notification
		CASE_NOTIF(AcceptFriendsNotif	, FAccelByteModelsAcceptFriendsNotif);
		CASE_NOTIF(RequestFriendsNotif	, FAccelByteModelsRequestFriendsNotif);
		CASE_NOTIF(UnfriendNotif		, FAccelByteModelsUnfriendNotif);
		CASE_NOTIF(CancelFriendsNotif	, FAccelByteModelsCancelFriendsNotif);
		CASE_NOTIF(RejectFriendsNotif	, FAccelByteModelsRejectFriendsNotif);
		// Block + Notification
		CASE_NOTIF(BlockPlayerNotif		, FAccelByteModelsBlockPlayerNotif);
		CASE_NOTIF(UnblockPlayerNotif	, FAccelByteModelsUnblockPlayerNotif);
		// Shadow Ban
		case (Notif::UserBannedNotification): // intended fallthrough
		case (Notif::UserUnbannedNotification):
		{
			BanNotifReceived = true;
			FAccelByteModelsUserBannedNotification Result;

			if (FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &Result))
			{
				BanType = Result.Ban;
				if (Result.UserId == CredentialsRef->GetUserId())
				{
					HttpRef.BearerAuthRejected();
				}
				
				if (ReceivedMessageType.Equals(LobbyResponse::UserBannedNotification))
				{
					UserBannedNotification.ExecuteIfBound(Result);

					SetUnbanSchedule(Result);
				}
				else if (ReceivedMessageType.Equals(LobbyResponse::UserUnbannedNotification))
				{
					UserUnbannedNotification.ExecuteIfBound(Result);

					RemoveUnbanSchedule(Result);
				}
			}
			break;
		}
		case (Notif::ErrorNotif):
		{
			FString ErrorNotifRequestType = ParsedJsonObj->GetStringField(TEXT("requestType"));

			// Handle trigger ResponseDelegates when ErrorNotif arrived with "requestType" field 
			if(!ErrorNotifRequestType.IsEmpty())
			{
				// Replace "Request" at end of string with "Response"
				const FString RequestString = {TEXT("Request")};
				if(ErrorNotifRequestType.EndsWith(RequestString))
				{
					ErrorNotifRequestType = ErrorNotifRequestType.LeftChop(RequestString.Len()).Append(TEXT("Response"));
				}
				
				FAccelByteModelsLobbyBaseResponse ErrorRequestResponse;
				ErrorRequestResponse.Code = ParsedJsonObj->GetStringField(TEXT("code"));
				ErrorRequestResponse.Id = ParsedJsonObj->GetStringField(TEXT("id"));
				ErrorRequestResponse.Type = ErrorNotifRequestType;

				TSharedPtr<FJsonObject> ErrorRequestJsonObject = FJsonObjectConverter::UStructToJsonObject(ErrorRequestResponse);
				FString ErrorRequestJsonString;
				FJsonObjectConverter::UStructToJsonObjectString(ErrorRequestResponse, ErrorRequestJsonString);
				HandleMessageResponse(ErrorNotifRequestType,  ErrorRequestJsonString, ErrorRequestJsonObject);
			}
			else
			{
				ErrorNotif.ExecuteIfBound(ParsedJsonObj->GetIntegerField(TEXT("code")), ParsedJsonObj->GetStringField(TEXT("message")));
			}
			break;
		}
		case (Notif::SignalingP2PNotif):
		{
			SignalingP2PNotif.ExecuteIfBound(ParsedJsonObj->GetStringField(TEXT("destinationId")), ParsedJsonObj->GetStringField(TEXT("message")));
			break;
		}
		default:
		{
			ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error; Detected of type notif but no specific handler case assigned. %s, Raw: %s"), *ReceivedMessageType, *ParsedJsonString));
			break;
		}
	}

}
#undef CASE_NOTIF

bool Lobby::ExtractLobbyMessageMetaData(FString const& InLobbyMessage
	, TSharedRef<FLobbyMessageMetaData> const& OutLobbyMessageMetaData)
{
	TArray<FString> MessageLines;
	InLobbyMessage.ParseIntoArrayLines(MessageLines);

	const FString CodeKey {"code: "};
	const FString TypeKey {"type: "};
	const FString IdKey {"id: "};
	
	for(FString const& Line : MessageLines)
	{
		if(Line.StartsWith(CodeKey))
		{
			OutLobbyMessageMetaData->Code = Line.RightChop(CodeKey.Len());
		}
		else if(Line.StartsWith(TypeKey))
		{
			OutLobbyMessageMetaData->Type = Line.RightChop(TypeKey.Len());
		}
		else if(Line.StartsWith(IdKey))
		{
			OutLobbyMessageMetaData->Id = Line.RightChop(IdKey.Len());
		}
	}

	UE_LOG(LogAccelByteLobby, Log, TEXT("Metadata found type %s, id %s, code %s"), *OutLobbyMessageMetaData->Type, *OutLobbyMessageMetaData->Id, *OutLobbyMessageMetaData->Code);

	return true;
}

void Lobby::SendBufferedNotifications()
{
	if (CredentialsRef->GetAccessToken().IsEmpty())
	{
		return;
	}

	TArray<FAccelByteModelsUserNotification> BufferedNotifications = NotificationBuffer.GetSortedBuffer();
	NotificationBuffer.Clear();

	// send all buffered data sequentially
	for (const FAccelByteModelsUserNotification& Notification : BufferedNotifications)
	{
		FString LobbyMessage;
		FLobbySequenceID SequenceID = FLobbySequenceID(Notification.SequenceID);
		FLobbySequenceNumber SequenceNumber = FLobbySequenceNumber(Notification.SequenceNumber);

		if (Notification.Type.Equals(LobbyResponse::ConnectedNotif))
		{
			LobbyMessage = FAccelByteNotificationSenderUtility::ComposeLobbyConnectedNotification(Notification.LobbySessionID, Notification.LoginType, Notification.ReconnectFromCode, SequenceID, SequenceNumber);
		}
		else if (Notification.Type.Equals(LobbyResponse::MessageNotif))
		{
			LobbyMessage = FAccelByteNotificationSenderUtility::ComposeMMv2Notification(Notification.Topic, Notification.Payload, true, SequenceID, SequenceNumber);
		}
		else if (Notification.Type.Equals(LobbyResponse::SessionNotif))
		{
			LobbyMessage = FAccelByteNotificationSenderUtility::ComposeSessionNotification(Notification.Topic, Notification.Payload, true, SequenceID, SequenceNumber);
		}

		if (LobbyMessage.IsEmpty())
		{
			continue;
		}

		OnMessage(LobbyMessage, true);
	}
}

bool Lobby::TryBufferNotification(FString const& ParsedJsonString)
{
	FAccelByteModelsUserNotification ReceivedNotification;
	if (!FAccelByteJsonConverter::JsonObjectStringToUStruct(ParsedJsonString, &ReceivedNotification))
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Cannot check missing notification, failed to deserialize %s to FAccelByteModelsUserNotification"), *ParsedJsonString);
		return false;
	}

	FScopeLock Lock(&NotificationBufferLock);

	if (NotificationBuffer.IsBuffering())
	{
		NotificationBuffer.TryAddBuffer(ReceivedNotification);
		return true;
	}

	// Successfully buffered means there is missing notification.
	// Only get missing notification time at the first time missing notification received.
	// Any other notification that arrive while GetNotifications() is in process will be buffered.
	if (NotificationBuffer.TryAddBuffer(ReceivedNotification))
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Missing notification(s) detected, received: %s"), *ParsedJsonString);

		// get missing notification from the time last valid (still in order) received notification up to the most recent received notification
		GetNotifications(THandler<FAccelByteModelsGetUserNotificationsResponse>::CreateThreadSafeSP(AsShared(), &Lobby::OnGetMissingNotificationSuccess)
			,FErrorHandler::CreateThreadSafeSP(AsShared(), &Lobby::OnGetMissingNotificationError));

		return true;
	}

	return false;
}

void Lobby::OnGetMissingNotificationSuccess(const FAccelByteModelsGetUserNotificationsResponse& MissingNotifications)
{
	FScopeLock Lock(&NotificationBufferLock);
	UE_LOG(LogAccelByteLobby, Log, TEXT("Missing notification(s) found"));
	NotificationBuffer.AddMissingNotifications(MissingNotifications.Notifications);
	SendBufferedNotifications();
}

void Lobby::OnGetMissingNotificationError(int32 ErrorCode, FString const& ErrorMessage)
{
	// In case of failed retrieving notification via REST API just send anything we have in the buffer
	UE_LOG(LogAccelByteLobby, Warning, TEXT("Missing notification(s) not found"));
	SendBufferedNotifications();
}

void Lobby::OnMessage(FString const& Message, bool bSkipConditioner /* = false */)
{
	UE_LOG(LogAccelByteLobby, Verbose, TEXT("Raw Lobby Response\n%s"), *Message);

	if (Message.IsEmpty())
	{
		return;
	}

	bool bIsFragmentedEnd {false};
	FString ProcessedMessage;
	MessageParser::ProcessFragmentedMessage(Message, LobbyEnvelopeStartHeaderValue, LobbyEnvelopeEndHeaderValue,
		EnvelopeContentBuffer, ProcessedMessage, bIsFragmentedEnd);

	if(!bIsFragmentedEnd)
	{
		UE_LOG(LogAccelByteLobby, Verbose, TEXT("Message fragmented, current content buffer\n%s"), *EnvelopeContentBuffer);
		return;
	}

	// Conversion : Custom -> Json
	const FString ParsedJsonString = LobbyMessageToJson(ProcessedMessage);
	
	UE_LOG(LogAccelByteLobby, VeryVerbose, TEXT("JSON Version: %s"), *ParsedJsonString);

	TSharedPtr<FJsonObject> ParsedJsonObj;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParsedJsonString);
	if (!FJsonSerializer::Deserialize(JsonReader, ParsedJsonObj))
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Failed to Deserialize. Json: %s"), *ParsedJsonString);

		TSharedRef<FLobbyMessageMetaData> MetaData = MakeShared<FLobbyMessageMetaData>();
		ExtractLobbyMessageMetaData(ProcessedMessage, MetaData);
		MetaData->Code = FString::FromInt(static_cast<int>(ErrorCodes::JsonDeserializationFailed));

		// handle error message if message type is response. if notif then ignore.
		if (MetaData->Type.Contains(Suffix::Response))
		{
			HandleMessageResponse(MetaData->Type, ParsedJsonString, ParsedJsonObj, MetaData);
		}
		
		return;
	}

	const FString JsonTypeIdentifier = TEXT("type");
	if (!ParsedJsonObj->HasTypedField<EJson::String>(JsonTypeIdentifier))
	{
		return;
	}

	const FString ReceivedMessageType = ParsedJsonObj->GetStringField(JsonTypeIdentifier);
	UE_LOG(LogAccelByteLobby, VeryVerbose, TEXT("Type: %s"), *ReceivedMessageType);

	// drop message if network conditioner deemed we should drop this.
	if(!bSkipConditioner && NetworkConditioner.CalculateFail(ReceivedMessageType))
	{
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Dropped message type %s"), *ReceivedMessageType);
		return;
	}

	FHandleLobbyMessageData MessageData;
	MessageData.bSkipConditioner = bSkipConditioner;
	MessageData.MessageType = ReceivedMessageType;
	MessageData.ParsedJsonString = ParsedJsonString;
	MessageData.ParsedJsonObject = ParsedJsonObj;
	MessageData.Type = EHandleLobbyMessageDataType::Other;

	if (ReceivedMessageType.Equals(LobbyResponse::ConnectedNotif))
	{
		if (TryBufferNotification(ParsedJsonString))
		{
			return;
		}

		NotificationQueue.Enqueue(MessageData);
	}

	HandleLobbyMessageByType(MessageData);
}

FAccelByteTaskWPtr Lobby::RequestWritePartyStorage(FString const& PartyId
	, FAccelByteModelsPartyDataUpdateRequest const& Data
	, THandler<FAccelByteModelsPartyDataNotif> const& OnSuccess
	, FErrorHandler const& OnError
	, FSimpleDelegate const& OnConflicted)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s/attributes")
		, *SettingsRef.BaseUrl
		, *CredentialsRef->GetNamespace()
		, *PartyId);

	FString Contents = "{\n";
	FString CustomAttribute;
	FJsonObjectConverter::UStructToJsonObjectString(Data.Custom_attribute, CustomAttribute);
	FString UpdatedAt = FString::Printf(TEXT("\"updatedAt\": %lld"), Data.UpdatedAt);
	FString CustomString = FString::Printf(TEXT("\"custom_attribute\": %s"), *CustomAttribute);
	Contents += UpdatedAt;
	Contents += ",\n";
	Contents += CustomString;
	Contents += "}";

	const FErrorHandler ErrorHandler = FErrorHandler::CreateLambda(
		[OnError, OnConflicted](int32 Code, FString Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::StatusPreconditionFailed) || Code == (int32)
				ErrorCodes::PartyStorageOutdatedUpdateData)
			{
				OnConflicted.ExecuteIfBound();
			}
			else
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		});

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, OnSuccess, ErrorHandler);
}

FAccelByteTaskWPtr Lobby::WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> const& DataWrapper)
{
	if (DataWrapper->RemainingAttempt <= 0)
	{
		DataWrapper->OnError.ExecuteIfBound(412, TEXT("Exhaust all retry attempt to modify party storage.."));
		return nullptr;
	}

	LobbyWPtr LobbyWeak = AsShared();
	return GetPartyStorage(DataWrapper->PartyId
		, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[LobbyWeak, DataWrapper](FAccelByteModelsPartyDataNotif Result)
			{
				auto LobbyApi = LobbyWeak.Pin();
				if (LobbyApi.IsValid())
				{
					Result.Custom_attribute = DataWrapper->PayloadModifier(Result.Custom_attribute);

					FAccelByteModelsPartyDataUpdateRequest PartyStorageBodyRequest;

					PartyStorageBodyRequest.UpdatedAt = FCString::Atoi64(*Result.UpdatedAt);
					PartyStorageBodyRequest.Custom_attribute = Result.Custom_attribute;

					LobbyApi->RequestWritePartyStorage(DataWrapper->PartyId
						, PartyStorageBodyRequest
						, DataWrapper->OnSuccess
						, DataWrapper->OnError
						, FSimpleDelegate::CreateLambda(
							[LobbyWeak, DataWrapper]()
							{
								DataWrapper->RemainingAttempt--;
								auto LobbyApi = LobbyWeak.Pin();
								if (LobbyApi.IsValid())
								{
									LobbyApi->WritePartyStorageRecursive(DataWrapper);
								}
							}));
				}
			})
		, FErrorHandler::CreateLambda(
			[DataWrapper](int32 ErrorCode, FString ErrorMessage)
			{
				DataWrapper->OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		);
}

FAccelByteTaskWPtr Lobby::FetchLobbyErrorMessages()
{
	FString Url = FString::Printf(TEXT("%s/lobby/v1/messages"), *SettingsRef.BaseUrl);

	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	return HttpClient.Request(TEXT("GET"), Url, {}, TEXT(""), Headers
		, THandler<TArray<FLobbyMessages>>::CreateLambda(
			[&](TArray<FLobbyMessages> const& Result)
			{
				for(const FLobbyMessages& LobbyMessages : Result)
				{
					LobbyErrorMessages.Add(LobbyMessages.Code, LobbyMessages.CodeName);
				}

				UE_LOG(LogAccelByteLobby, Log, TEXT("fetching lobby error messages DONE! %d lobby messages has been cached"), LobbyErrorMessages.Num());
			})
		, FErrorHandler::CreateLambda(
			[](int32 code, FString const& message)
			{
				UE_LOG(LogAccelByteLobby, Warning, TEXT("Error fetching lobby error messages! code %d, message %s"), code, *message);
			}));
}

void Lobby::ClearLobbyErrorMessages()
{
	if (LobbyErrorMessages.Num() > 0)
	{
		LobbyErrorMessages.Empty();
	}
}

void Lobby::SetTokenGenerator(TSharedPtr<IAccelByteTokenGenerator> const& TokenGeneratorRef)
{
	if (IsConnected())
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Cannot set token generator, lobby already connected"));
		return;
	}
	
	if (TokenGenerator.IsValid() && OnTokenReceivedDelegateHandle.IsValid())
	{
		// Remove previously bound delegate when setting new token generator
		TokenGenerator->OnTokenReceived().Remove(OnTokenReceivedDelegateHandle);
		OnTokenReceivedDelegateHandle.Reset();
	}

	TokenGenerator = TokenGeneratorRef;
	
	if (TokenGeneratorRef.IsValid())
	{
		OnTokenReceivedDelegateHandle = TokenGenerator->OnTokenReceived().AddThreadSafeSP(AsShared(), &Lobby::OnTokenReceived);
	}
}

void Lobby::InitializeMessaging()
{
	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		OnReceivedQosLatenciesUpdatedDelegate = FOnMessagingSystemReceivedMessage::CreateThreadSafeSP(AsShared(), &Lobby::OnReceivedQosLatencies);
		QosLatenciesUpdatedDelegateHandle = MessagingSystemPtr->SubscribeToTopic(EAccelByteMessagingTopic::QosRegionLatenciesUpdated
			, OnReceivedQosLatenciesUpdatedDelegate);
	}
}

Lobby::Lobby(Credentials & InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, FAccelByteMessagingSystem& InMessagingSystemRef
	, FAccelByteNetworkConditioner& InNetworkConditionerRef
	, float InPingDelay
	, float InInitialBackoffDelay
	, float InMaxBackoffDelay
	, float InTotalTimeout)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, LobbyCredentialsRef{InCredentialsRef.AsShared()}
#if ENGINE_MAJOR_VERSION < 5
	, MessagingSystemWPtr{InMessagingSystemRef.AsShared()}
#else
	, MessagingSystemWPtr{InMessagingSystemRef.AsWeak()}
#endif
	, NetworkConditioner{InNetworkConditionerRef}
	, PingDelay{InPingDelay}
	, TimeSinceLastPing{.0f}
	, TimeSinceLastReconnect{.0f}
	, TimeSinceConnectionLost{.0f}
{
	InitializeV2MatchmakingNotifTopics();

	auto Strategy = FReconnectionStrategy::CreateBalancedStrategy(
		FReconnectionStrategy::FBalancedMaxRetryInterval(FTimespan::FromSeconds(InMaxBackoffDelay)),
		FReconnectionStrategy::FTotalTimeoutDuration(FTimespan::FromSeconds(InTotalTimeout)),
		FReconnectionStrategy::FInitialBackoffDelay(FTimespan::FromSeconds(InInitialBackoffDelay))
	);
	IWebsocketConfigurableReconnectStrategy::SetDefaultReconnectionStrategy(Strategy);
}

Lobby::~Lobby()
{
	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::QosRegionLatenciesUpdated, QosLatenciesUpdatedDelegateHandle);
	}
	OnReceivedQosLatenciesUpdatedDelegate.Unbind();

	FTickerAlias::GetCoreTicker().RemoveTicker(LobbyTickerHandle);

	// only disconnect when engine is still valid
	if(UObjectInitialized())
	{
		Disconnect(true);
		UnbindEvent();
		ConnectSuccess.Unbind();
		ConnectError.Unbind();
		ConnectionClosed.Unbind();
		Reconnecting.Unbind();
		ReconnectAttempted.Clear();
		MassiveOutage.Clear();
	}
}

TMap<FString, FString> Lobby::LobbyErrorMessages{};

void Lobby::SetConnectSuccessDelegate(Lobby::FConnectSuccess const& OnConnectSuccess)
{
	ConnectSuccess = OnConnectSuccess;
}

void Lobby::SetConnectFailedDelegate(FErrorHandler const& OnConnectError)
{
	ConnectError = OnConnectError;
}

void Lobby::SetDisconnectNotifDelegate(Lobby::FDisconnectNotif const& OnDisconnectNotice)
{
	DisconnectNotif = OnDisconnectNotice;
}

void Lobby::SetConnectionClosedDelegate(Lobby::FConnectionClosed const& OnConnectionClosed)
{
	ConnectionClosed = OnConnectionClosed;
}

void Lobby::SetReconnectingDelegate(Lobby::FConnectionClosed const& OnReconnecting)
{
	Reconnecting = OnReconnecting;
}

Lobby::FReconnectAttempted& Lobby::OnReconnectAttemptedMulticastDelegate()
{
	return ReconnectAttempted;
}

Lobby::FMassiveOutage& Lobby::OnMassiveOutageMulticastDelegate()
{
	return MassiveOutage;
}

void Lobby::SetPartyLeaveNotifDelegate(Lobby::FPartyLeaveNotif const& OnPartyLeaveNotice)
{
	PartyLeaveNotif = OnPartyLeaveNotice;
}

void Lobby::SetPartyMemberLeaveNotifDelegate(Lobby::FPartyMemberLeaveNotif const& OnPartyMemberLeaveNotice)
{
	PartyMemberLeaveNotif = OnPartyMemberLeaveNotice;
}

void Lobby::SetPartyMemberConnectNotifDelegate(Lobby::FPartyMemberConnectNotif const& OnPartyMemberConnectNotif)
{
	PartyMemberConnectNotif = OnPartyMemberConnectNotif;
}

void Lobby::SetPartyMemberDisconnectNotifDelegate(Lobby::FPartyMemberDisconnectNotif const& OnPartyMemberDisconnectNotif)
{
	PartyMemberDisconnectNotif = OnPartyMemberDisconnectNotif;
}

void Lobby::SetPartyInviteNotifDelegate(Lobby::FPartyInviteNotif const& OnPartyInviteNotif)
{
	PartyInviteNotif = OnPartyInviteNotif;
}

void Lobby::SetPartyGetInvitedNotifDelegate(Lobby::FPartyGetInvitedNotif const& OnInvitePartyGetInvitedNotice)
{
	PartyGetInvitedNotif = OnInvitePartyGetInvitedNotice;
}

void Lobby::SetPartyJoinNotifDelegate(Lobby::FPartyJoinNotif const& OnInvitePartyJoinNotice)
{
	PartyJoinNotif = OnInvitePartyJoinNotice;
}

void Lobby::SetPartyInvitationRejectedNotifDelegate(Lobby::FPartyRejectNotif const& OnInvitePartyRejectNotice)
{
	PartyRejectNotif = OnInvitePartyRejectNotice;
}

void Lobby::SetPartyKickNotifDelegate(Lobby::FPartyKickNotif const& OnInvitePartyKickedNotice)
{
	PartyKickNotif = OnInvitePartyKickedNotice;
}

void Lobby::SetPartyNotifDelegate(Lobby::FPartyNotif const& OnPartyNotif)
{
	PartyNotif = OnPartyNotif;
}

void Lobby::SetPrivateMessageNotifDelegate(Lobby::FPersonalChatNotif const& OnPersonalChatNotif)
{
	PersonalChatNotif = OnPersonalChatNotif;
}

void Lobby::SetPartyChatNotifDelegate(Lobby::FPartyChatNotif const& OnPersonalChatNotif)
{
	PartyChatNotif = OnPersonalChatNotif;
}

void Lobby::SetUserPresenceNotifDelegate(Lobby::FFriendStatusNotif const& OnUserPresenceNotif)
{
	FriendStatusNotif = OnUserPresenceNotif;
}

void Lobby::SetMessageNotifDelegate(Lobby::FMessageNotif const& OnNotificationMessage)
{
	if (RemoveMessageNotifBroadcasterDelegate(NotificationMessageDelegateHandle))
	{
		NotificationMessageDelegateHandle.Reset();
	}

	NotificationMessageDelegateHandle = AddMessageNotifDelegate(OnNotificationMessage);
}

FDelegateHandle Lobby::AddMessageNotifDelegate(Lobby::FMessageNotif const& OnNotificationMessage)
{
	return MessageNotifBroadcaster.Add(OnNotificationMessage);
}

FDelegateHandle Lobby::AddMessageNotifBroadcasterDelegate(Lobby::FNotifBroadcaster const& OnNotificationBroadcasterMessage)
{
	return MessageNotifBroadcaster.AddLambda([OnNotificationBroadcasterMessage](FAccelByteModelsNotificationMessage const& Message)
		{
			OnNotificationBroadcasterMessage.Broadcast(Message);
		});
}

bool Lobby::RemoveMessageNotifBroadcasterDelegate(FDelegateHandle const& OnNotificationBroadcasterDelegate)
{
	return MessageNotifBroadcaster.Remove(OnNotificationBroadcasterDelegate);
}

void Lobby::OnReceivedQosLatencies(FString const& Payload)
{
	if (!IsConnected())
	{
		return;
	}

	FAccelByteModelsQosRegionLatencies RegionLatencies;
	FJsonObjectConverter::JsonObjectStringToUStruct(Payload, &RegionLatencies);

	const FString ClosestRegion = RegionLatencies.GetClosestRegion();

	if (ClosestRegion.IsEmpty())
	{
		return;
	}

	ChangeUserRegion(ClosestRegion);
}

void Lobby::Startup()
{
	if (!bIsStarted)
	{
		InitializeMessaging();
		LobbyTickerHandle = FTickerAlias::GetCoreTicker().AddTicker(FTickerDelegate::CreateThreadSafeSP(AsShared(), &Lobby::Tick), LobbyTickPeriod);
		bIsStarted = true;
	}
}

} // Namespace Api
} // Namespace AccelByte
