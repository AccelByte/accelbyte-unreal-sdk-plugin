// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteLobbyApi.h"
#include "Modules/ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLobby, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLobby);

namespace AccelByte
{
namespace Api
{
	namespace LobbyRequest
	{
		// Party
		const FString PartyInfo = TEXT("partyInfoRequest");
		const FString CreateParty = TEXT("partyCreateRequest");
		const FString LeaveParty = TEXT("partyLeaveRequest");
		const FString InviteParty = TEXT("partyInviteRequest");
		const FString JoinParty = TEXT("partyJoinRequest");
		const FString RejectParty = TEXT("partyRejectRequest");
		const FString KickParty = TEXT("partyKickRequest");
		const FString PartyGenerateCodeRequest = TEXT("partyGenerateCodeRequest");
		const FString PartyGetCodeRequest = TEXT("partyGetCodeRequest");
		const FString PartyDeleteCodeRequest = TEXT("partyDeleteCodeRequest");
		const FString PartyJoinViaCodeRequest = TEXT("partyJoinViaCodeRequest");
		const FString PartyPromoteLeaderRequest = TEXT("partyPromoteLeaderRequest");

		// Chat
		const FString PersonalChat = TEXT("personalChatRequest");
		const FString PartyChat = TEXT("partyChatRequest");
		const FString JoinChannelChat = TEXT("joinDefaultChannelRequest");
		const FString ChannelChat = TEXT("sendChannelChatRequest");

		// Presence
		const FString SetPresence = TEXT("setUserStatusRequest");
		const FString FriendsPresence = TEXT("friendsStatusRequest");

		// Notification

		// Matchmaking
		const FString StartMatchmaking = TEXT("startMatchmakingRequest");
		const FString CancelMatchmaking = TEXT("cancelMatchmakingRequest");
		const FString ReadyConsent = TEXT("setReadyConsentRequest");

		// Friends
		const FString RequestFriend = TEXT("requestFriendsRequest");
		const FString Unfriend = TEXT("unfriendRequest");
		const FString ListOutgoingFriends = TEXT("listOutgoingFriendsRequest");
		const FString CancelFriends = TEXT("cancelFriendsRequest");
		const FString ListIncomingFriends = TEXT("listIncomingFriendsRequest");
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
		const FString PartyLeaveNotif = TEXT("partyLeaveNotif");
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
		const FString PartyGenerateCode = TEXT("partyGenerateCodeResponse");
		const FString PartyGetCode = TEXT("partyGetCodeResponse");
		const FString PartyDeleteCode = TEXT("partyDeleteCodeResponse");
		const FString PartyJoinViaCode = TEXT("partyJoinViaCodeResponse");
		const FString PartyPromoteLeader = TEXT("partyPromoteLeaderResponse");

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
		const FString FriendsPresence = TEXT("friendsStatusResponse");

		// Notification
		const FString MessageNotif = TEXT("messageNotif");
		const FString UserBannedNotification = TEXT("userBannedNotification");
		const FString UserUnbannedNotification = TEXT("userUnbannedNotification");

		// Matchmaking
		const FString StartMatchmaking = TEXT("startMatchmakingResponse");
		const FString CancelMatchmaking = TEXT("cancelMatchmakingResponse");
		const FString ReadyConsentResponse = TEXT("setReadyConsentResponse");
		const FString MatchmakingNotif = TEXT("matchmakingNotif");
		const FString ReadyConsentNotif = TEXT("setReadyConsentNotif");
		const FString RematchmakingNotif = TEXT("rematchmakingNotif");
		const FString DsNotif = TEXT("dsNotif");

		// Friends
		const FString RequestFriends = TEXT("requestFriendsResponse");
		const FString Unfriend = TEXT("unfriendResponse");
		const FString ListOutgoingFriends = TEXT("listOutgoingFriendsResponse");
		const FString CancelFriends = TEXT("cancelFriendsResponse");
		const FString ListIncomingFriends = TEXT("listIncomingFriendsResponse");
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
		const FString SessionAttributeSet = TEXT("setSessionAttributeResponse");
		const FString SessionAttributeGet = TEXT("getSessionAttributeResponse");
		const FString SessionAttributeGetAll = TEXT("getAllSessionAttributeResponse");
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
	}

void Lobby::Connect()
{
	FReport::Log(FString(__FUNCTION__));

	if (!WebSocket.IsValid())
	{
		CreateWebSocket();
	}

	if (WebSocket->IsConnected())
	{
		return;
	}

	if (bWasWsConnectionError)
	{
		// websocket state is error can't be reconnect, need to create a new instance
		CreateWebSocket();
	}

	if (LobbyTickDelegateHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(LobbyTickDelegateHandle);
		LobbyTickDelegateHandle.Reset();
	}

	LobbyTickDelegateHandle = FTicker::GetCoreTicker().AddTicker(LobbyTickDelegate, LobbyTickPeriod);

	WebSocket->Connect();
	WsEvents |= EWebSocketEvent::Connect;
	UE_LOG(LogAccelByteLobby, Display, TEXT("Connecting to %s"), *Settings.LobbyServerUrl);
	
	if(LobbyErrorMessages.Num() == 0)
	{
		FetchLobbyErrorMessages();
	}
}

void Lobby::Disconnect()
{
	FReport::Log(FString(__FUNCTION__));

	ChannelSlug = "";
	if (LobbyTickDelegateHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(LobbyTickDelegateHandle);
		LobbyTickDelegateHandle.Reset();
	}

	if (WebSocket.IsValid())
	{
		WebSocket->OnMessage().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->OnClosed().Clear();
		WebSocket->Close();

		WebSocket.Reset();
	}

	if (GEngine) UE_LOG(LogAccelByteLobby, Display, TEXT("Disconnected"));
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

//-------------------------------------------------------------------------------------------------
// Chat
//-------------------------------------------------------------------------------------------------
FString Lobby::SendPrivateMessage(const FString& UserId, const FString& Message)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PersonalChat, Prefix::Chat,
		FString::Printf(TEXT("to: %s\npayload: %s\n"), *UserId, *Message));
}

FString Lobby::SendPartyMessage(const FString& Message)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyChat, Prefix::Chat,
		FString::Printf(TEXT("payload: %s\n"), *Message));
}

FString Lobby::SendJoinDefaultChannelChatRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::JoinChannelChat, Prefix::Chat);
}

FString Lobby::SendChannelMessage(const FString& Message)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ChannelSlug.IsEmpty())
	{
		return SendRawRequest(LobbyRequest::ChannelChat, Prefix::Chat,
			FString::Printf(TEXT("channelSlug: %s\npayload: %s\n"), *ChannelSlug, *Message));
	}
	else
	{
		FAccelByteModelsChannelMessageResponse ErrorResult
		{
			FString::FromInt((int)ErrorCodes::InvalidRequest),
			"You're not in any chat channel."
		};
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

	return SendRawRequest(LobbyRequest::PartyInfo, Prefix::Party);
}

FString Lobby::SendCreatePartyRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::CreateParty, Prefix::Party);
}

FString Lobby::SendLeavePartyRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::LeaveParty, Prefix::Party);
}

FString Lobby::SendInviteToPartyRequest(const FString& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::InviteParty, Prefix::Party,
		FString::Printf(TEXT("friendID: %s"), *UserId));
}

FString Lobby::SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::JoinParty, Prefix::Party,
		FString::Printf(TEXT("partyID: %s\ninvitationToken: %s"), *PartyId, *InvitationToken));
}

FString Api::Lobby::SendRejectInvitationRequest(const FString& PartyId, const FString& InvitationToken)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::RejectParty, Prefix::Party,
		FString::Printf(TEXT("partyID: %s\ninvitationToken: %s"), *PartyId, *InvitationToken));
}

FString Lobby::SendKickPartyMemberRequest(const FString& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::KickParty, Prefix::Party,
		FString::Printf(TEXT("memberID: %s\n"), *UserId));
}

FString Lobby::SendPartyGenerateCodeRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyGenerateCodeRequest, Prefix::Party);
}

FString Lobby::SendPartyGetCodeRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyGetCodeRequest, Prefix::Party);
}

FString Lobby::SendPartyDeleteCodeRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyDeleteCodeRequest, Prefix::Party);
}

FString Lobby::SendPartyJoinViaCodeRequest(const FString& partyCode)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyJoinViaCodeRequest, Prefix::Party,
		FString::Printf(TEXT("partyCode: %s\n"), *partyCode));
}

FString Lobby::SendPartyPromoteLeaderRequest(const FString& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyPromoteLeaderRequest, Prefix::Party,
		FString::Printf(TEXT("newLeaderUserId: %s\n"), *UserId));
}

//-------------------------------------------------------------------------------------------------
// Presence
//-------------------------------------------------------------------------------------------------
FString Lobby::SendSetPresenceStatus(const Availability Availability, const FString& Activity)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::SetPresence, Prefix::Presence,
		FString::Printf(TEXT("availability: %d\nactivity: %s\n"), (int)Availability, *Activity));
}

FString Lobby::SendGetOnlineUsersRequest()
{
	return Lobby::SendGetOnlineFriendPresenceRequest();
}

FString Lobby::SendGetOnlineFriendPresenceRequest()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::FriendsPresence, Prefix::Presence);
}

//-------------------------------------------------------------------------------------------------
// Notification
//-------------------------------------------------------------------------------------------------
void Lobby::GetAllAsyncNotification()
{
	FReport::Log(FString(__FUNCTION__));

	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type: offlineNotificationRequest\nid:%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
		WebSocket->Send(Content);
		UE_LOG(LogAccelByteLobby, Display, TEXT("Get async notification (id=%s)"), *Content)
	}
}

//-------------------------------------------------------------------------------------------------
// Matchmaking
//-------------------------------------------------------------------------------------------------
FString Lobby::SendStartMatchmaking(FString GameMode, FString ServerName, FString ClientVersion, TArray<TPair<FString, float>> Latencies, TMap<FString, FString> PartyAttributes, TArray<FString> TempPartyUserIds, TArray<FString> ExtraAttributes)
{
	FReport::Log(FString(__FUNCTION__));
	FString Contents = FString::Printf(TEXT("gameMode: %s\n"), *GameMode);
	if (!ServerName.IsEmpty())
	{
		Contents.Append(FString::Printf(TEXT("serverName: %s\n"), *ServerName));
	}
	if (!ClientVersion.IsEmpty())
	{
		Contents.Append(FString::Printf(TEXT("clientVersion: %s\n"), *ClientVersion));
	}

	if (Latencies.Num() > 0)
	{
		FString ServerLatencies = TEXT("{");
		for (int i = 0; i < Latencies.Num(); i++)
		{
			ServerLatencies.Append(FString::Printf(TEXT("\"%s\":%.0f"), *Latencies[i].Key, Latencies[i].Value));
			if (i + 1 < Latencies.Num())
			{
				ServerLatencies.Append(TEXT(","));
			}
		}
		ServerLatencies.Append(TEXT("}"));
		Contents.Append(FString::Printf(TEXT("latencies: %s\n"), *ServerLatencies));
	}

	if (PartyAttributes.Num() > 0)
	{
		FString partyAttributeSerialized = "";
		TArray<FString> keys;
		PartyAttributes.GetKeys(keys);
		for (int i = 0 ; i < keys.Num() ; i++)
		{
			FString key = keys[i];
			FString value = PartyAttributes[keys[i]];
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
		Contents.Append(FString::Printf(TEXT("partyAttributes: {%s}\n"), *partyAttributeSerialized));
	}

	if (TempPartyUserIds.Num() > 0)
	{
		FString STempParty = TEXT("");
		for (int i = 0; i < TempPartyUserIds.Num(); i++)
		{
			STempParty.Append(FString::Printf(TEXT("%s"), *TempPartyUserIds[i]));
			if (i + 1 < TempPartyUserIds.Num())
			{
				STempParty.Append(TEXT(","));
			}
		}
		Contents.Append(FString::Printf(TEXT("tempParty: %s\n"), *STempParty));
	}

	if (ExtraAttributes.Num() > 0)
	{
		FString SExtraAttributes = TEXT("");
		for (int i = 0; i < ExtraAttributes.Num(); i++)
		{
			SExtraAttributes.Append(FString::Printf(TEXT("%s"), *ExtraAttributes[i]));
			if (i + 1 < ExtraAttributes.Num())
			{
				SExtraAttributes.Append(TEXT(","));
			}
		}
		Contents.Append(FString::Printf(TEXT("extraAttributes: %s\n"), *SExtraAttributes));
	}

	return SendRawRequest(LobbyRequest::StartMatchmaking, Prefix::Matchmaking,
		Contents);
}

FString Lobby::SendStartMatchmaking(FString GameMode, TArray<FString> TempPartyUserIds, FString ServerName, FString ClientVersion, TArray<TPair<FString, float>> Latencies, TMap<FString, FString> PartyAttributes, TArray<FString> ExtraAttributes)
{
	return SendStartMatchmaking(GameMode, ServerName, ClientVersion, Latencies, PartyAttributes, TempPartyUserIds, ExtraAttributes);
}

FString Lobby::SendStartMatchmaking(FString GameMode, TMap<FString, FString> PartyAttributes, FString ServerName, FString ClientVersion, TArray<TPair<FString, float>> Latencies, TArray<FString> TempPartyUserIds, TArray<FString> ExtraAttributes)
{
	return SendStartMatchmaking(GameMode, ServerName, ClientVersion, Latencies, PartyAttributes, TempPartyUserIds, ExtraAttributes);
}

FString Lobby::SendStartMatchmaking(FString GameMode, TMap<FString, FString> PartyAttributes, TArray<FString> TempPartyUserIds, FString ServerName, FString ClientVersion, TArray<TPair<FString, float>> Latencies, TArray<FString> ExtraAttributes)
{
	return SendStartMatchmaking(GameMode, ServerName, ClientVersion, Latencies, PartyAttributes, TempPartyUserIds, ExtraAttributes);
}

FString Lobby::SendCancelMatchmaking(FString GameMode, bool IsTempParty)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::CancelMatchmaking, Prefix::Matchmaking,
		FString::Printf(TEXT("gameMode: %s\nisTempParty: %s"), *GameMode, (IsTempParty ? TEXT("true") : TEXT("false"))));
}

FString Lobby::SendReadyConsentRequest(FString MatchId)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::ReadyConsent, Prefix::Matchmaking,
		FString::Printf(TEXT("matchId: %s\n"), *MatchId));
}

//-------------------------------------------------------------------------------------------------
// Friends
//-------------------------------------------------------------------------------------------------
void Lobby::RequestFriend(FString UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::RequestFriend, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::Unfriend(FString UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::Unfriend, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::ListOutgoingFriends()
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::ListOutgoingFriends, Prefix::Friends);
}

void Lobby::CancelFriendRequest(FString UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::CancelFriends, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::ListIncomingFriends()
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::ListIncomingFriends, Prefix::Friends);
}

void Lobby::AcceptFriend(FString UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::AcceptFriends, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::RejectFriend(FString UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::RejectFriends, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::LoadFriendsList()
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::LoadFriendList, Prefix::Friends);
}

void Lobby::GetFriendshipStatus(FString UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::GetFriendshipStatus, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::BulkFriendRequest(FAccelByteModelsBulkFriendsRequest UserIds, FVoidHandler OnSuccess, FErrorHandler OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/users/%s/add/bulk"), *Settings.BaseUrl, *Credentials.GetNamespace(), *Credentials.GetUserId());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents;
	FJsonObjectConverter::UStructToJsonObjectString(UserIds, Contents);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::GetPartyData(const FString& PartyId, const THandler<FAccelByteModelsPartyData>& OnSuccess, const FErrorHandler& OnError) const
{
	FReport::Log(FString(__FUNCTION__));

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	const FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s"), *Settings.BaseUrl, *Credentials.GetNamespace(), *PartyId);
	const FString Verb = TEXT("GET");
	const FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::BulkGetUserPresence(const TArray<FString>& UserIds, const THandler<FAccelByteModelsBulkUserStatusNotif>& OnSuccess, const FErrorHandler& OnError, bool CountOnly)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::InvalidRequest, TEXT("UserIds cannot be empty!"));
		return;
	}

	FString Query = TEXT("?userIds=");
	for (int i = 0; i < UserIds.Num(); i++)
	{
		Query.Append(FString::Printf(TEXT("%s"), *FGenericPlatformHttp::UrlEncode(UserIds[i])));
		if (i < UserIds.Num() - 1)
		{
			Query.Append(TEXT(","));
		}
	}

	Query.Append(FString::Printf(TEXT("&countOnly=%s"), CountOnly ? TEXT("true") : TEXT("false")));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/presence/namespaces/%s/users/presence%s"), *Settings.BaseUrl, *Credentials.GetNamespace(), *Query);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::GetPartyStorage(const FString & PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s"), *Settings.BaseUrl, *Credentials.GetNamespace(), *PartyId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::GetListOfBlockedUsers(const FString& UserId, const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/%s/blocked"), *Settings.BaseUrl, *Credentials.GetNamespace(), *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::GetListOfBlockedUsers(const THandler<FAccelByteModelsListBlockedUserResponse> OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/me/blocked"), *Settings.BaseUrl, *Credentials.GetNamespace());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());

}

void Lobby::GetListOfBlockers(const FString& UserId, const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/%s/blocked-by"), *Settings.BaseUrl, *Credentials.GetNamespace(), *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::GetListOfBlockers(const THandler<FAccelByteModelsListBlockerResponse> OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/player/namespaces/%s/users/me/blocked-by"), *Settings.BaseUrl, *Credentials.GetNamespace());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::SendNotificationToUser(const FString& SendToUserId, const FAccelByteModelsFreeFormNotificationRequest& Message, bool bAsync, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/notification/namespaces/%s/users/%s/freeform?async=%s"), *Settings.BaseUrl, *Credentials.GetNamespace(), *SendToUserId, bAsync ? TEXT("true") : TEXT("false"));
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FJsonObjectConverter::UStructToJsonObjectString(Message, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::WritePartyStorage(const FString & PartyId, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler & OnError, uint32 RetryAttempt)
{
	TSharedPtr<PartyStorageWrapper> Wrapper = MakeShared<PartyStorageWrapper>();
	Wrapper->PartyId = PartyId;
	Wrapper->OnSuccess = OnSuccess;
	Wrapper->OnError = OnError;
	Wrapper->RemainingAttempt = RetryAttempt;
	Wrapper->PayloadModifier = PayloadModifier;
	WritePartyStorageRecursive(Wrapper);
}

//-------------------------------------------------------------------------------------------------
// Block
//-------------------------------------------------------------------------------------------------

void Lobby::BlockPlayer(const FString& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::BlockPlayer, Prefix::Block,
		FString::Printf(TEXT("userId: %s\nblockedUserId: %s\nnamespace: %s"), *Credentials.GetUserId(), *UserId, *Credentials.GetNamespace()));
}

void Lobby::UnblockPlayer(const FString& UserId)
{
	FReport::Log(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::UnblockPlayer, Prefix::Friends,
		FString::Printf(TEXT("userId: %s\nunblockedUserId: %s\nnamespace: %s"), *Credentials.GetUserId(), *UserId, *Credentials.GetNamespace()));
}

//-------------------------------------------------------------------------------------------------
// Signaling
//-------------------------------------------------------------------------------------------------
FString Lobby::SendSignalingMessage(const FString& UserId, const FString& Message) 
{
	FReport::Log(FString(__FUNCTION__));
	
	return SendRawRequest(LobbyRequest::SignalingP2PNotif, Prefix::Signaling,
		FString::Printf(TEXT("destinationId: %s\nmessage: %s\n"), *UserId, *Message));
}

//-------------------------------------------------------------------------------------------------
// Session Attributes
//-------------------------------------------------------------------------------------------------
FString Lobby::SetSessionAttribute(const FString& Key, const FString& Value)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::SetSessionAttribute, Prefix::Attribute,
		FString::Printf(TEXT("namespace: %s\nkey: %s\nvalue: %s"), *Credentials.GetNamespace(), *Key, *Value));
}

FString Lobby::GetSessionAttribute(const FString& Key)
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::GetSessionAttribute, Prefix::Attribute,
		FString::Printf(TEXT("namespace: %s\nkey: %s"), *Credentials.GetNamespace(), *Key));
}

FString Lobby::GetAllSessionAttribute()
{
	FReport::Log(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::GetAllSessionAttribute, Prefix::Attribute,
		FString::Printf(TEXT("namespace: %s"), *Credentials.GetNamespace()));
}

void Lobby::UnbindEvent()
{
	FReport::Log(FString(__FUNCTION__));

	MessageNotif.Unbind();
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
	
	UserBannedNotification.Unbind();
	UserUnbannedNotification.Unbind();
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
	UnfriendResponse.Unbind();
	ListOutgoingFriendsResponse.Unbind();
	CancelFriendsResponse.Unbind();
	ListIncomingFriendsResponse.Unbind();
	AcceptFriendsResponse.Unbind();
	RejectFriendsResponse.Unbind();
	LoadFriendListResponse.Unbind();
	GetFriendshipStatusResponse.Unbind();

	OnSetUserPresenceError.Unbind();
	OnGetAllFriendsStatusError.Unbind();
	OnRequestFriendsError.Unbind();
	OnUnfriendError.Unbind();
	OnListOutgoingFriendsError.Unbind();
	OnCancelFriendsError.Unbind();
	OnListIncomingFriendsError.Unbind();
	OnAcceptFriendsError.Unbind();
	OnRejectFriendsError.Unbind();
	OnLoadFriendListError.Unbind();
	OnGetFriendshipStatusError.Unbind();
}

void Lobby::UnbindPartyNotifEvents()
{
	FReport::Log(FString(__FUNCTION__));

	PartyLeaveNotif.Unbind();
	PartyInviteNotif.Unbind();
	PartyGetInvitedNotif.Unbind();
	PartyJoinNotif.Unbind();
	PartyRejectNotif.Unbind();
	PartyKickNotif.Unbind();
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
	RematchmakingNotif.Unbind();
	DsNotif.Unbind();
}

void Lobby::UnbindMatchmakingResponseEvents()
{
	FReport::Log(FString(__FUNCTION__));

	MatchmakingStartResponse.Unbind();
	MatchmakingCancelResponse.Unbind();
	ReadyConsentResponse.Unbind();

	OnMatchmakingStartError.Unbind();
	OnMatchmakingCancelError.Unbind();
	OnReadyConsentError.Unbind();
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
	JoinDefaultChannelResponse.Unbind();
	ChannelChatResponse.Unbind();

	OnPersonalChatError.Unbind();
	OnPartyChatError.Unbind();
	OnJoinDefaultChannelChatError.Unbind();
	OnChannelChatError.Unbind();
}

void Lobby::UnbindBlockNotifEvents()
{
	BlockPlayerNotif.Unbind();
	UnblockPlayerNotif.Unbind();
}
	
void Lobby::UnbindBlockResponseEvents()
{
	OnBlockPlayerError.Unbind();
	OnUnblockPlayerError.Unbind();
}

void Lobby::UnbindSessionAttributeEvents()
{
	OnGetSessionAttributeError.Unbind();
	OnSetSessionAttributeError.Unbind();
	OnGetAllSessionAttributeError.Unbind();
}

void Lobby::OnConnected()
{
	WsEvents |= EWebSocketEvent::Connected;
	UE_LOG(LogAccelByteLobby, Display, TEXT("Connected"));
		
	ConnectSuccess.ExecuteIfBound();
}

void Lobby::OnConnectionError(const FString& Error)
{
	WsEvents |= EWebSocketEvent::ConnectionError;
	bWasWsConnectionError = true;
	UE_LOG(LogAccelByteLobby, Display, TEXT("Error connecting: %s"), *Error);
	ConnectError.ExecuteIfBound(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed), ErrorMessages::Default.at(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed)) + TEXT(" Reason: ") + Error);
}

void Lobby::OnClosed(int32 StatusCode, const FString& Reason, bool WasClean)
{
	OnMessage(Reason);
	if (StatusCode >= 4000 && !BanNotifReceived)
	{
		Disconnect();
	}
	else
	{
		WsEvents |= EWebSocketEvent::Closed;
	}

	BanNotifReceived = false;
	UE_LOG(LogAccelByteLobby, Display, TEXT("Connection closed. Status code: %d  Reason: %s Clean: %d"), StatusCode, *Reason, WasClean);
	ConnectionClosed.ExecuteIfBound(StatusCode, Reason, WasClean);
}

FString Lobby::SendRawRequest(FString MessageType, FString MessageIDPrefix, FString CustomPayload)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString MessageID = GenerateMessageID(MessageIDPrefix);
		FString Content = FString::Printf(TEXT("type: %s\nid: %s"), *MessageType, *MessageID);
		if (!CustomPayload.IsEmpty())
		{
			Content.Append(FString::Printf(TEXT("\n%s"), *CustomPayload));
		}
		WebSocket->Send(Content);
		UE_LOG(LogAccelByteLobby, Display, TEXT("Sending request: %s"), *Content);
		return MessageID;
	}
	return TEXT("");
}

bool Lobby::Tick(float DeltaTime)
{
	switch (WsState)
	{
	case EWebSocketState::Closed:
		if (WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Connected) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Connected;
		}
		else if ((WsEvents & EWebSocketEvent::Connect) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Connecting;
		}
		break;
	case EWebSocketState::Connecting:
		if ((WsEvents & EWebSocketEvent::ConnectionError) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closed;
		}
		else if (WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Connected) != EWebSocketEvent::None)
		{
			ChannelSlug = "";
			TimeSinceLastPing = FPlatformTime::Seconds();
			WsState = EWebSocketState::Connected;
		}
		else if ((WsEvents & EWebSocketEvent::Close) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closing;
		}
		else if (!WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Closed) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closed;
		}
		break;
	case EWebSocketState::Connected:

		if ((WsEvents & EWebSocketEvent::Close) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closing;
		}
		else if (!WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Closed) != EWebSocketEvent::None)
		{
			TimeSinceLastReconnect = FPlatformTime::Seconds();
			TimeSinceConnectionLost = FPlatformTime::Seconds();
			BackoffDelay = InitialBackoffDelay;
			RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-InitialBackoffDelay, InitialBackoffDelay) / 4);
			Connect();
			WsState = EWebSocketState::Reconnecting;
		}
		else if ((FPlatformTime::Seconds() - TimeSinceLastPing) >= PingDelay)
		{
			TimeSinceLastPing = FPlatformTime::Seconds();
			SendPing();
		}
		break;
	case EWebSocketState::Reconnecting:
		if (WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Connected) != EWebSocketEvent::None)
		{
			TimeSinceLastPing = FPlatformTime::Seconds();
			WsState = EWebSocketState::Connected;
		}
		else if ((WsEvents & EWebSocketEvent::Close) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closing;
		}
		else if ((FPlatformTime::Seconds() - TimeSinceConnectionLost) >= TotalTimeout)
		{
			TimeSinceLastReconnect = FPlatformTime::Seconds();
			BackoffDelay = InitialBackoffDelay;

			WsState = EWebSocketState::Closed;
		}
		else if ((FPlatformTime::Seconds() - TimeSinceLastReconnect) >= RandomizedBackoffDelay)
		{
			if (BackoffDelay < MaxBackoffDelay)
			{
				BackoffDelay *= 2;
			}
			RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-BackoffDelay, BackoffDelay) / 4);
			if (bWasWsConnectionError)
			{
				// websocket state is error can't be reconnect, need to create a new instance
				CreateWebSocket();
			}

			WebSocket->Connect();
			TimeSinceLastReconnect = FPlatformTime::Seconds();
		}
		break;
	case EWebSocketState::Closing:
		if ((WsEvents & EWebSocketEvent::Closed) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closed;
		}
		break;
	}

	WsEvents = EWebSocketEvent::None;

	return true;
}

FString Lobby::GenerateMessageID(FString Prefix)
{
	return FString::Printf(TEXT("%s-%d"), *Prefix, FMath::RandRange(1000, 9999));
}

void Lobby::CreateWebSocket()
{
	bWasWsConnectionError = false;

	if(WebSocket.IsValid())
	{
		WebSocket->OnMessage().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->OnClosed().Clear();
		WebSocket->Close();
		WebSocket.Reset();
	}

	TMap<FString, FString> Headers;
	Headers.Add("Authorization", "Bearer " + Credentials.GetAccessToken());
	Headers.Add("X-Ab-LobbySessionID", LobbySessionId.LobbySessionID);
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(*Settings.LobbyServerUrl, TEXT("wss"), Headers);

	WebSocket->OnMessage().AddRaw(this, &Lobby::OnMessage);
	WebSocket->OnConnected().AddRaw(this, &Lobby::OnConnected);
	WebSocket->OnConnectionError().AddRaw(this, &Lobby::OnConnectionError);
	WebSocket->OnClosed().AddRaw(this, &Lobby::OnClosed);
}

FString Lobby::LobbyMessageToJson(FString Message)
{
	FString Json = TEXT("{");
	TArray<FString> Out;
	Message.ParseIntoArray(Out, TEXT("\n"), true);
	for (int i = 0; i < Out.Num(); i++)
	{
		FString CurrentLine = Out[i];

		FString Key;
		FString Value;

		CurrentLine.Split(": ", &Key, &Value);
		Json += FString::Printf(TEXT("\"%s\":"), *Key);
		Value.TrimStartAndEndInline();

		if (Value.StartsWith("["))
		{
			if (Value.Equals("[]"))
			{
				Json += Value;
			}
			else
			{
				bool Quote = false;
				bool ElementStart = false;
				FString Element;
				Json += "[";
				for (int j = 1; j < Value.Len() - 1; j++)
				{
					if (!ElementStart)
					{
						if (Value[j] == ' ') 
						{
							continue;
						}

						ElementStart = true;
						Element.AppendChar('"');
						if (Value[j] == '"')
						{
							Quote = true;
							continue;
						}
					}

					if (!Quote && Value[j] == ',')
					{
						ElementStart = false;
						Element.TrimEndInline();
						Json += Element;

						if (!Element.EndsWith("\""))
						{
							Json.AppendChar('"');
						}
						Element = "";

						if (j == Value.Len() - 2) 
						{
							break;
						}
					}

					Element.AppendChar(Value[j]);

					if (Quote && Value[j] == '\\')
					{
						Element.AppendChar(Value[++j]);
					}
					else if (Value[j] == '"')
					{
						Quote = false;
					}
				}

				if (!Element.IsEmpty())
				{
					Json += Element;
					if (!Element.EndsWith("\""))
					{
						Json.AppendChar('"');
					}
				}
				Json += "]";
			}
		}
		else if (Value.StartsWith("{"))
		{
			Json += Value;
		}
		else
		{
			Json += FString::Printf(TEXT("\"%s\""), *Value);
		}

		if (i < Out.Num() - 1)
		{
			Json += ",";
		}
	}
	Json += TEXT("}");
	return Json;
}

void Lobby::OnMessage(const FString& Message)
{
	UE_LOG(LogAccelByteLobby, Display, TEXT("Raw Lobby Response\n%s"), *Message);
	FString ParsedJson = LobbyMessageToJson(Message);
	UE_LOG(LogAccelByteLobby, Display, TEXT("JSON Version: %s"), *ParsedJson);
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParsedJson);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		UE_LOG(LogAccelByteLobby, Display, TEXT("Failed to Deserialize. Json: %s"), *ParsedJson);
		return;
	}
	FString lobbyResponseType = JsonParsed->GetStringField("type");
	int lobbyResponseCode = 0;
	if (lobbyResponseType.Contains("Response"))
		lobbyResponseCode = JsonParsed->GetIntegerField("code");
	UE_LOG(LogAccelByteLobby, Display, TEXT("Type: %s"), *lobbyResponseType);

#define HANDLE_LOBBY_MESSAGE_NOTIF(MessageType, Model, ResponseCallback) \
if (lobbyResponseType.Equals(MessageType)) \
{ \
	Model Result; \
	bool bSuccess = false; \
	if(lobbyResponseType.Contains("Notif")) \
		bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0); \
	if (bSuccess) \
	{ \
		ResponseCallback.ExecuteIfBound(Result); \
	} \
	else if(lobbyResponseType.Contains("Response")) { \
		UE_LOG(LogAccelByteLobby, Display, TEXT("Use HANDLE_LOBBY_MESSAGE_RESPONSE to process Type: %s"), *lobbyResponseType); \
	} \
	else \
	{ \
		ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error cannot parse response %s, Raw: %s"), *MessageType, *ParsedJson)); \
	} \
	return; \
} \
	// NOTIF
	// Party
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::DisconnectNotif, FAccelByteModelsDisconnectNotif, DisconnectNotif);
    HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyLeaveNotif, FAccelByteModelsLeavePartyNotice, PartyLeaveNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyInviteNotif, FAccelByteModelsInvitationNotice, PartyInviteNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice, PartyGetInvitedNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyJoinNotif, FAccelByteModelsPartyJoinNotice, PartyJoinNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyRejectNotif, FAccelByteModelsPartyRejectNotice, PartyRejectNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice, PartyKickNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyDataUpdateNotif, FAccelByteModelsPartyDataNotif, PartyDataUpdateNotif);
	// Chat
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PersonalChatNotif, FAccelByteModelsPersonalMessageNotice, PersonalChatNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::PartyChatNotif, FAccelByteModelsPartyMessageNotice, PartyChatNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::ChannelChatNotif, FAccelByteModelsChannelMessageNotice, ChannelChatNotif);
	if (lobbyResponseType.Equals(LobbyResponse::JoinChannelChat))
	{
		FAccelByteModelsJoinDefaultChannelResponse Result;
		bool bParseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		if (bParseSuccess)
		{
			ChannelSlug = Result.ChannelSlug;
			JoinDefaultChannelResponse.ExecuteIfBound(Result);
			return;
		}
	}
	// Presence
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::FriendStatusNotif, FAccelByteModelsUsersPresenceNotice, FriendStatusNotif);
	// Notification
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::MessageNotif, FAccelByteModelsNotificationMessage, MessageNotif);
	// Matchmaking
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::MatchmakingNotif, FAccelByteModelsMatchmakingNotice, MatchmakingNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::ReadyConsentNotif, FAccelByteModelsReadyConsentNotice, ReadyConsentNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::RematchmakingNotif, FAccelByteModelsRematchmakingNotice, RematchmakingNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::DsNotif, FAccelByteModelsDsNotice, DsNotif);
	// Friends + Notification
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::AcceptFriendsNotif, FAccelByteModelsAcceptFriendsNotif, AcceptFriendsNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::RequestFriendsNotif, FAccelByteModelsRequestFriendsNotif, RequestFriendsNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::UnfriendNotif, FAccelByteModelsUnfriendNotif, UnfriendNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::CancelFriendsNotif, FAccelByteModelsCancelFriendsNotif, CancelFriendsNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::RejectFriendsNotif, FAccelByteModelsRejectFriendsNotif, RejectFriendsNotif);
	// Block + Notification
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::BlockPlayerNotif, FAccelByteModelsBlockPlayerNotif, BlockPlayerNotif);
	HANDLE_LOBBY_MESSAGE_NOTIF(LobbyResponse::UnblockPlayerNotif, FAccelByteModelsUnblockPlayerNotif, UnblockPlayerNotif);

	// Shadow Ban
	if (lobbyResponseType.Equals(LobbyResponse::UserBannedNotification) ||
		lobbyResponseType.Equals(LobbyResponse::UserUnbannedNotification)) 
	{
		BanNotifReceived = true;
		FAccelByteModelsUserBannedNotification Result;
		bool bParseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		if (bParseSuccess)
		{
			HttpRef.BearerAuthRejected();
			if (lobbyResponseType.Equals(LobbyResponse::UserBannedNotification))
			{
				UserBannedNotification.ExecuteIfBound(Result);
			}
			else if (lobbyResponseType.Equals(LobbyResponse::UserUnbannedNotification))
			{
				UserUnbannedNotification.ExecuteIfBound(Result);
			}
		}
	}

	// Error
	if(lobbyResponseType.Equals(LobbyResponse::ErrorNotif))
	{
		ErrorNotif.ExecuteIfBound(JsonParsed->GetIntegerField(TEXT("code")), JsonParsed->GetStringField(TEXT("message")));
	}
		
	// Signaling
	if (lobbyResponseType.Equals(LobbyResponse::SignalingP2PNotif))
	{
		SignalingP2P.ExecuteIfBound(JsonParsed->GetStringField(TEXT("destinationId")), JsonParsed->GetStringField(TEXT("message")));
		return;
	}

#undef HANDLE_LOBBY_MESSAGE_NOTIF
		
#define HANDLE_LOBBY_MESSAGE_RESPONSE(MessageType, Model, ResponseCallback, ErrorCallback) \
if (lobbyResponseType.Equals(MessageType)) \
{ \
	Model Result; \
	bool bSuccess = false; \
	if(lobbyResponseType.Contains("Response")) {\
		if(lobbyResponseCode == 0) \
			bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0); \
		else { \
			FString ErrorCodeString = FString::FromInt(lobbyResponseCode); \
			bool bHasErrorName = LobbyErrorMessages.Contains(ErrorCodeString); \
			UE_LOG(LogAccelByteLobby, Display, TEXT("%s returned non zero error code, code is %d with codename %s"), *MessageType, lobbyResponseCode, (bHasErrorName ? *LobbyErrorMessages[ErrorCodeString] : TEXT("NO NAME"))); \
			if(ErrorCallback.IsBound()) \
			{ \
				ErrorCallback.ExecuteIfBound(lobbyResponseCode, bHasErrorName ? *LobbyErrorMessages[ErrorCodeString] : TEXT("NO NAME")); \
				bSuccess = false; \
			} \
			else \
			{ \
				Result.Code = FString::FromInt(lobbyResponseCode); \
				bSuccess = true; \
			} \
		} \
	} \
	if (bSuccess) \
	{ \
		ResponseCallback.ExecuteIfBound(Result); \
	} \
	else if(lobbyResponseType.Contains("Notif")) { \
		UE_LOG(LogAccelByteLobby, Display, TEXT("Use HANDLE_LOBBY_MESSAGE_NOTIF to process Type: %s"), *lobbyResponseType); \
	} \
	else \
	{ \
		ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error cannot parse response %s, Raw: %s"), *MessageType, *ParsedJson)); \
	} \
	return; \
}\

	if(lobbyResponseType.Equals(LobbyResponse::ConnectedNotif))
	{
		FAccelByteModelsLobbySessionId SessionId;
		bool bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &SessionId, 0, 0);
		if(bSuccess)
		{
			LobbySessionId = SessionId;
		}
	}

	// RESPONSE
	// Party
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyInfo, FAccelByteModelsInfoPartyResponse, PartyInfoResponse, OnPartyInfoError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyCreate, FAccelByteModelsCreatePartyResponse, PartyCreateResponse, OnPartyCreateError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyLeave, FAccelByteModelsLeavePartyResponse, PartyLeaveResponse, OnPartyLeaveError);
    HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyInvite, FAccelByteModelsPartyInviteResponse, PartyInviteResponse, OnPartyInviteError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyJoin, FAccelByteModelsPartyJoinReponse, PartyJoinResponse, OnPartyJoinError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyReject, FAccelByteModelsPartyRejectResponse, PartyRejectResponse, OnPartyRejectError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyKick, FAccelByteModelsKickPartyMemberResponse, PartyKickResponse, OnPartyKickError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyGetCode, FAccelByteModelsPartyGetCodeResponse, PartyGetCodeResponse, OnPartyGetCodeError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyGenerateCode, FAccelByteModelsPartyGenerateCodeResponse, PartyGenerateCodeResponse, OnPartyGenerateCodeError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyDeleteCode, FAccelByteModelsPartyDeleteCodeResponse, PartyDeleteCodeResponse, OnPartyDeleteCodeError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyJoinViaCode, FAccelByteModelsPartyJoinReponse, PartyJoinViaCodeResponse, OnPartyJoinViaCodeError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyPromoteLeader, FAccelByteModelsPartyPromoteLeaderResponse, PartyPromoteLeaderResponse, OnPartyPromoteLeaderError);
	// Chat
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PersonalChat, FAccelByteModelsPersonalMessageResponse, PersonalChatResponse, OnPersonalChatError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::PartyChat, FAccelByteModelsPartyMessageResponse, PartyChatResponse, OnPartyChatError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::ChannelChat, FAccelByteModelsChannelMessageResponse, ChannelChatResponse, OnChannelChatError);
	// Presence
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::SetUserPresence, FAccelByteModelsSetOnlineUsersResponse, SetUserPresenceResponse, OnSetUserPresenceError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::FriendsPresence, FAccelByteModelsGetOnlineUsersResponse, GetAllFriendsStatusResponse, OnGetAllFriendsStatusError);
	// Matchmaking
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::StartMatchmaking, FAccelByteModelsMatchmakingResponse, MatchmakingStartResponse, OnMatchmakingStartError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::CancelMatchmaking, FAccelByteModelsMatchmakingResponse, MatchmakingCancelResponse, OnMatchmakingCancelError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::ReadyConsentResponse, FAccelByteModelsReadyConsentRequest, ReadyConsentResponse, OnReadyConsentError);
	// Friends
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::RequestFriends, FAccelByteModelsRequestFriendsResponse, RequestFriendsResponse, OnRequestFriendsError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::Unfriend, FAccelByteModelsUnfriendResponse, UnfriendResponse, OnUnfriendError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::ListOutgoingFriends, FAccelByteModelsListOutgoingFriendsResponse, ListOutgoingFriendsResponse, OnListOutgoingFriendsError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::CancelFriends, FAccelByteModelsCancelFriendsResponse, CancelFriendsResponse, OnCancelFriendsError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::ListIncomingFriends, FAccelByteModelsListIncomingFriendsResponse, ListIncomingFriendsResponse, OnListIncomingFriendsError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::AcceptFriends, FAccelByteModelsAcceptFriendsResponse, AcceptFriendsResponse, OnAcceptFriendsError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::RejectFriends, FAccelByteModelsRejectFriendsResponse, RejectFriendsResponse, OnRejectFriendsError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::LoadFriendList, FAccelByteModelsLoadFriendListResponse, LoadFriendListResponse, OnLoadFriendListError);
	if (lobbyResponseType.Equals(LobbyResponse::GetFriendshipStatus))
	{
		FAccelByteModelsGetFriendshipStatusStringResponse StringResult;
		bool bParseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &StringResult, 0, 0);
		if (bParseSuccess)
		{
			FAccelByteModelsGetFriendshipStatusResponse Result;
			Result.Code = StringResult.Code;
			Result.friendshipStatus = (ERelationshipStatusCode)FCString::Atoi(*StringResult.friendshipStatus);
			GetFriendshipStatusResponse.ExecuteIfBound(Result);
			return;
		}
	}
	// Block
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::BlockPlayer, FAccelByteModelsBlockPlayerResponse, BlockPlayerResponse, OnBlockPlayerError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::UnblockPlayer, FAccelByteModelsUnblockPlayerResponse, UnblockPlayerResponse, OnUnblockPlayerError);
	// Session Attribute
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::SessionAttributeSet, FAccelByteModelsSetSessionAttributesResponse, SetSessionAttributeResponse, OnSetSessionAttributeError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::SessionAttributeGet, FAccelByteModelsGetSessionAttributesResponse, GetSessionAttributeResponse, OnGetSessionAttributeError);
	HANDLE_LOBBY_MESSAGE_RESPONSE(LobbyResponse::SessionAttributeGetAll, FAccelByteModelsGetAllSessionAttributesResponse, GetAllSessionAttributeResponse, OnGetAllSessionAttributeError);

#undef HANDLE_LOBBY_MESSAGE_RESPONSE
		
#ifdef DEBUG_LOBBY_MESSAGE
	ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Warning: Unhandled message %s, Raw: %s"), *lobbyResponseType, *ParsedJson));
#endif
}

void Lobby::RequestWritePartyStorage(const FString& PartyId, const FAccelByteModelsPartyDataUpdateRequest& Data, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, FSimpleDelegate OnConflicted)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/public/party/namespaces/%s/parties/%s/attributes"), *Settings.BaseUrl, *Credentials.GetNamespace(), *PartyId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FString Contents = "{\n";
	FString CustomAttribute;
	FJsonObjectConverter::UStructToJsonObjectString(Data.Custom_attribute, CustomAttribute);
	FString UpdatedAt = FString::Printf(TEXT("\"updatedAt\": %lld"), Data.UpdatedAt);
	FString CustomString = FString::Printf(TEXT("\"custom_attribute\": %s"), *CustomAttribute);
	Contents += UpdatedAt;
	Contents += ",\n";
	Contents += CustomString;
	Contents += "}";

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);

	FErrorHandler ErrorHandler = FErrorHandler::CreateLambda(
		[OnConflicted](int32 Code, FString Message)
		{
			if (Code == (int32)ErrorCodes::StatusPreconditionFailed || Code == (int32)
				ErrorCodes::PartyStorageOutdatedUpdateData)
			{
				OnConflicted.ExecuteIfBound();
			}
		});

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, ErrorHandler), FPlatformTime::Seconds());
}

void Lobby::WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper)
{
	if (DataWrapper->RemainingAttempt <= 0)
	{
		DataWrapper->OnError.ExecuteIfBound(412, TEXT("Exhaust all retry attempt to modify party storage.."));
	}

	GetPartyStorage(
		DataWrapper->PartyId,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[this, DataWrapper](FAccelByteModelsPartyDataNotif Result)
			{
				Result.Custom_attribute = DataWrapper->PayloadModifier(Result.Custom_attribute);

				FAccelByteModelsPartyDataUpdateRequest PartyStorageBodyRequest;

				PartyStorageBodyRequest.UpdatedAt = FCString::Atoi64(*Result.UpdatedAt);
				PartyStorageBodyRequest.Custom_attribute = Result.Custom_attribute;

				RequestWritePartyStorage(
					DataWrapper->PartyId,
					PartyStorageBodyRequest,
					DataWrapper->OnSuccess,
					DataWrapper->OnError,
					FSimpleDelegate::CreateLambda(
						[this, DataWrapper]()
						{
							DataWrapper->RemainingAttempt--;
							WritePartyStorageRecursive(DataWrapper);
						}));
			}),
		FErrorHandler::CreateLambda(
			[DataWrapper](int32 ErrorCode, FString ErrorMessage)
			{
				DataWrapper->OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		);
}

void Lobby::SetRetryParameters(int32 NewTotalTimeout, int32 NewBackoffDelay, int32 NewMaxDelay)
{
	FReport::Log(FString(__FUNCTION__));

	if (WebSocket.IsValid())
	{
		UE_LOG(LogAccelByteLobby, Log, TEXT("Can't change retry parameters! Lobby is already connected."));
		return;
	}

	Lobby::TotalTimeout = NewTotalTimeout;
	Lobby::InitialBackoffDelay = NewBackoffDelay;
	Lobby::MaxBackoffDelay = NewMaxDelay;
}

void Lobby::FetchLobbyErrorMessages()
{
	FString Url = FString::Printf(TEXT("%s/lobby/v1/messages"), *Settings.BaseUrl);

	bool bFetchErrorMessageDone = false;
	FHttpClient HttpClient(Credentials, Settings, HttpRef);
	HttpClient.Request("GET", Url, {}, "", {}, THandler<TArray<FLobbyMessages>>::CreateLambda([&](const TArray<FLobbyMessages>& Result)
	{
		for(const FLobbyMessages& LobbyMessages : Result)
		{
			LobbyErrorMessages.Add(LobbyMessages.Code, LobbyMessages.CodeName);
		}

		bFetchErrorMessageDone = true;
		UE_LOG(LogAccelByteLobby, Log, TEXT("fetching lobby error messages DONE! %d lobby messages has been cached"), LobbyErrorMessages.Num());
	}), FErrorHandler::CreateLambda([&bFetchErrorMessageDone](const int32 code, const FString& message)
	{
		UE_LOG(LogAccelByteLobby, Warning, TEXT("Error fetching lobby error messages! code %d, message %s"), code, *message);
		bFetchErrorMessageDone = true;
	}));
}

void Lobby::ClearLobbyErrorMessages()
{
	if(LobbyErrorMessages.Num() > 0)
	{
		LobbyErrorMessages.Empty();
	}
}

	Lobby::Lobby(
		const AccelByte::Credentials& Credentials,
		const AccelByte::Settings& Settings,
		FHttpRetryScheduler& HttpRef,
		float PingDelay,
		float InitialBackoffDelay,
		float MaxBackoffDelay,
		float TotalTimeout,
		TSharedPtr<IWebSocket> WebSocket)
		:
		HttpRef(HttpRef),
		Credentials(Credentials),
		Settings(Settings),
		PingDelay(PingDelay),
		InitialBackoffDelay(InitialBackoffDelay),
		MaxBackoffDelay(MaxBackoffDelay),
		TotalTimeout(TotalTimeout),
		BackoffDelay(InitialBackoffDelay),
		RandomizedBackoffDelay(InitialBackoffDelay),
		TimeSinceLastPing(0.f),
		TimeSinceLastReconnect(0.f),
		WsState(EWebSocketState::Closed),
		WsEvents(EWebSocketEvent::None),
		WebSocket(WebSocket)
{
	LobbyTickDelegate = FTickerDelegate::CreateRaw(this, &Lobby::Tick);
	LobbyTickDelegateHandle.Reset();
}

Lobby::~Lobby()
{
	// only disconnect when engine is still valid
	if(UObjectInitialized())
	{
		Disconnect();
	}
}

} // Namespace Api
} // Namespace AccelByte
