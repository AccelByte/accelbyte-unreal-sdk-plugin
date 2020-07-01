// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteLobbyApi.h"
#include "Modules/ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

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
		const FString KickParty = TEXT("partyKickRequest");

		// Chat
		const FString PersonalChat = TEXT("personalChatRequest");
		const FString PartyChat = TEXT("partyChatRequest");

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
		const FString PartyKick = TEXT("partyKickResponse");
		const FString PartyKickNotif = TEXT("partyKickNotif");

		// Chat
		const FString PersonalChat = TEXT("personalChatResponse");
		const FString PersonalChatNotif = TEXT("personalChatNotif");
		const FString PartyChat = TEXT("partyChatResponse");
		const FString PartyChatNotif = TEXT("partyChatNotif");

		// Presence
		const FString SetUserPresence = TEXT("setUserStatusResponse");
		const FString FriendStatusNotif = TEXT("userStatusNotif");
		const FString FriendsPresence = TEXT("friendsStatusResponse");

		// Notification
		const FString MessageNotif = TEXT("messageNotif");

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
	}

	namespace Prefix
	{
		const FString Party = TEXT("party");
		const FString Chat = TEXT("chat");
		const FString Presence = TEXT("presence");
		const FString Matchmaking = TEXT("matchmaking");
		const FString Friends = TEXT("friends");
	}

void Lobby::Connect()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	if (!WebSocket.IsValid())
	{
		TMap<FString, FString> Headers;
		Headers.Add("Authorization", "Bearer " + Credentials.GetUserSessionId());
		FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));
		WebSocket = FWebSocketsModule::Get().CreateWebSocket(*Settings.LobbyServerUrl, TEXT("wss"), Headers);
	}

	if (WebSocket->IsConnected())
	{
		return;
	}

	if (LobbyTickDelegateHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(LobbyTickDelegateHandle);
		LobbyTickDelegateHandle.Reset();
	}

	WebSocket->OnMessage().Clear();
	WebSocket->OnConnected().Clear();
	WebSocket->OnConnectionError().Clear();
	WebSocket->OnClosed().Clear();

	WebSocket->OnMessage().AddRaw(this, &Lobby::OnMessage);
	WebSocket->OnConnected().AddRaw(this, &Lobby::OnConnected);
	WebSocket->OnConnectionError().AddRaw(this, &Lobby::OnConnectionError);
	WebSocket->OnClosed().AddRaw(this, &Lobby::OnClosed);
	LobbyTickDelegateHandle = FTicker::GetCoreTicker().AddTicker(LobbyTickDelegate, LobbyTickPeriod);

	WebSocket->Connect();
	WsEvents |= EWebSocketEvent::Connect;
	UE_LOG(LogTemp, Display, TEXT("Connecting to %s"), *Settings.LobbyServerUrl);
}

void Lobby::Disconnect()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

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

		WebSocket = nullptr;
	}

	if (GEngine) UE_LOG(LogTemp, Display, TEXT("Disconnected"));
}

bool Lobby::IsConnected() const
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void Lobby::SendPing()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

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
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PersonalChat, Prefix::Chat,
		FString::Printf(TEXT("to: %s\npayload: %s\n"), *UserId, *Message));
}

FString Lobby::SendPartyMessage(const FString& Message)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyChat, Prefix::Chat,
		FString::Printf(TEXT("payload: %s\n"), *Message));
}

//-------------------------------------------------------------------------------------------------
// Party
//-------------------------------------------------------------------------------------------------
FString Lobby::SendInfoPartyRequest()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::PartyInfo, Prefix::Party);
}

FString Lobby::SendCreatePartyRequest()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::CreateParty, Prefix::Party);
}

FString Lobby::SendLeavePartyRequest()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::LeaveParty, Prefix::Party);
}

FString Lobby::SendInviteToPartyRequest(const FString& UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::InviteParty, Prefix::Party,
		FString::Printf(TEXT("friendID: %s"), *UserId));
}

FString Lobby::SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::JoinParty, Prefix::Party,
		FString::Printf(TEXT("partyID: %s\ninvitationToken: %s"), *PartyId, *InvitationToken));
}

FString Lobby::SendKickPartyMemberRequest(const FString& UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::KickParty, Prefix::Party,
		FString::Printf(TEXT("memberID: %s\n"), *UserId));
}

//-------------------------------------------------------------------------------------------------
// Presence
//-------------------------------------------------------------------------------------------------
FString Lobby::SendSetPresenceStatus(const Availability Availability, const FString& Activity)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::SetPresence, Prefix::Presence,
		FString::Printf(TEXT("availability: %d\nactivity: %s\n"), (int)Availability, *Activity));
}

FString Lobby::SendGetOnlineUsersRequest()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::FriendsPresence, Prefix::Presence);
}

//-------------------------------------------------------------------------------------------------
// Notification
//-------------------------------------------------------------------------------------------------
void Lobby::GetAllAsyncNotification()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type: offlineNotificationRequest\nid:%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
		WebSocket->Send(Content);
		UE_LOG(LogTemp, Display, TEXT("Get async notification (id=%s)"), *Content)
	}
}

//-------------------------------------------------------------------------------------------------
// Matchmaking
//-------------------------------------------------------------------------------------------------
FString Lobby::SendStartMatchmaking(FString GameMode, FString ServerName, FString ClientVersion, TArray<TPair<FString, float>> Latencies)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
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

	return SendRawRequest(LobbyRequest::StartMatchmaking, Prefix::Matchmaking,
		Contents);
}

FString Lobby::SendCancelMatchmaking(FString GameMode)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::CancelMatchmaking, Prefix::Matchmaking,
		FString::Printf(TEXT("gameMode: %s\n"), *GameMode));
}

FString Lobby::SendReadyConsentRequest(FString MatchId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	return SendRawRequest(LobbyRequest::ReadyConsent, Prefix::Matchmaking,
		FString::Printf(TEXT("matchId: %s\n"), *MatchId));
}

//-------------------------------------------------------------------------------------------------
// Friends
//-------------------------------------------------------------------------------------------------
void Lobby::RequestFriend(FString UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::RequestFriend, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::Unfriend(FString UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::Unfriend, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::ListOutgoingFriends()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::ListOutgoingFriends, Prefix::Friends);
}

void Lobby::CancelFriendRequest(FString UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::CancelFriends, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::ListIncomingFriends()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::ListIncomingFriends, Prefix::Friends);
}

void Lobby::AcceptFriend(FString UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::AcceptFriends, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::RejectFriend(FString UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::RejectFriends, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::LoadFriendsList()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::LoadFriendList, Prefix::Friends);
}

void Lobby::GetFriendshipStatus(FString UserId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	SendRawRequest(LobbyRequest::GetFriendshipStatus, Prefix::Friends,
		FString::Printf(TEXT("friendId: %s"), *UserId));
}

void Lobby::BulkFriendRequest(FAccelByteModelsBulkFriendsRequest UserIds, FVoidHandler OnSuccess, FErrorHandler OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString url;
	url = FRegistry::Settings.IamServerUrl;
	if (url.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/friends/namespaces/%s/users/%s/add/bulk"), *url, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
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
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Lobby::UnbindEvent()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	PartyLeaveNotif.Unbind();
	PartyInviteNotif.Unbind();
	PartyGetInvitedNotif.Unbind();
	PartyJoinNotif.Unbind();
	PartyKickNotif.Unbind();
	PersonalChatNotif.Unbind();
	PartyChatNotif.Unbind();
	FriendStatusNotif.Unbind();
	ReadyConsentNotif.Unbind();
	MessageNotif.Unbind();
	MatchmakingNotif.Unbind();
	RematchmakingNotif.Unbind();
	DsNotif.Unbind();
	AcceptFriendsNotif.Unbind();
	RequestFriendsNotif.Unbind();
}

void Lobby::OnConnected()
{
	WsEvents |= EWebSocketEvent::Connected;
	UE_LOG(LogTemp, Display, TEXT("Connected"));
	ConnectSuccess.ExecuteIfBound();
}

void Lobby::OnConnectionError(const FString& Error)
{
	WsEvents |= EWebSocketEvent::ConnectionError;
	UE_LOG(LogTemp, Display, TEXT("Error connecting: %s"), *Error);
	ConnectError.ExecuteIfBound(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed), ErrorMessages::Default.at(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed)) + TEXT(" Reason: ") + Error);
}

void Lobby::OnClosed(int32 StatusCode, const FString& Reason, bool WasClean)
{
	if (StatusCode >= 4000)
	{
		Disconnect();
	}
	else
	{
		WsEvents |= EWebSocketEvent::Closed;
	}

	UE_LOG(LogTemp, Display, TEXT("Connection closed. Status code: %d  Reason: %s Clean: %d"), StatusCode, *Reason, WasClean);
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
		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content);
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
			WebSocket->Connect();
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

FString Lobby::LobbyMessageToJson(FString Message)
{
	FString Json = TEXT("{");
	TArray<FString> Out;
	Message.ParseIntoArray(Out, TEXT("\n"), true);
	for (int i = 0; i < Out.Num(); i++)
	{
		FString CurrentLine = Out[i];
		if (CurrentLine.Contains("["))
		{
			Json += "\"";
			CurrentLine.ReplaceInline(TEXT(": "), TEXT("\":"));
			if (!CurrentLine.Contains("[]"))
			{
				CurrentLine.ReplaceInline(TEXT("["), TEXT("[\""));
				CurrentLine.ReplaceInline(TEXT(",]"), TEXT("\"]"));
				CurrentLine.ReplaceInline(TEXT(","), TEXT("\",\""));
			}
			Json += CurrentLine;
		}
		else
		{
			Json += "\"";
			FString Left;
			FString Right;
			CurrentLine.Split(": ", &Left, &Right);
			Json += Left;
			Json += "\":\"";
			Json += Right;
			Json += "\"";
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
	UE_LOG(LogTemp, Display, TEXT("Raw Lobby Response\n%s"), *Message);
	FString ParsedJson = LobbyMessageToJson(Message);
	UE_LOG(LogTemp, Display, TEXT("JSON Version: %s"), *ParsedJson);
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParsedJson);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to Deserialize. Json: %s"), *ParsedJson);
		return;
	}
	FString lobbyResponseType = JsonParsed->GetStringField("type");
	int lobbyResponseCode = 0;
	if (lobbyResponseType.Contains("Response"))
		lobbyResponseCode = JsonParsed->GetIntegerField("code");
	UE_LOG(LogTemp, Display, TEXT("Type: %s"), *lobbyResponseType);

#define HANDLE_LOBBY_MESSAGE(MessageType, Model, ResponseCallback) \
if (lobbyResponseType.Equals(MessageType)) \
{ \
Model Result; \
bool bSuccess; \
if(lobbyResponseCode == 0 || lobbyResponseType.Contains("Notif")) \
	bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0); \
else \
	bSuccess = true; \
if (bSuccess) \
{ \
    ResponseCallback.ExecuteIfBound(Result); \
} \
else \
{ \
    ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error cannot parse response %s, Raw: %s"), *MessageType, *ParsedJson)); \
} \
return; \
}\

    // Party
    HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyInfo, FAccelByteModelsInfoPartyResponse, PartyInfoResponse);
    HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyCreate, FAccelByteModelsCreatePartyResponse, PartyCreateResponse);
    HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyLeave, FAccelByteModelsLeavePartyResponse, PartyLeaveResponse);
    HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyLeaveNotif, FAccelByteModelsLeavePartyNotice, PartyLeaveNotif);
    HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyInvite, FAccelByteModelsPartyInviteResponse, PartyInviteResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyInviteNotif, FAccelByteModelsInvitationNotice, PartyInviteNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice, PartyGetInvitedNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyJoinNotif, FAccelByteModelsPartyJoinNotice, PartyJoinNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyJoin, FAccelByteModelsPartyJoinReponse, PartyJoinResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyKick, FAccelByteModelsKickPartyMemberResponse, PartyKickResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice, PartyKickNotif);

	// Chat
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PersonalChat, FAccelByteModelsPersonalMessageResponse, PersonalChatResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PersonalChatNotif, FAccelByteModelsPersonalMessageNotice, PersonalChatNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyChat, FAccelByteModelsPartyMessageResponse, PartyChatResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::PartyChatNotif, FAccelByteModelsPartyMessageNotice, PartyChatNotif);

	// Presence
	HANDLE_LOBBY_MESSAGE(LobbyResponse::SetUserPresence, FAccelByteModelsSetOnlineUsersResponse, SetUserPresenceResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::FriendStatusNotif, FAccelByteModelsUsersPresenceNotice, FriendStatusNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::FriendsPresence, FAccelByteModelsGetOnlineUsersResponse, GetAllFriendsStatusResponse);

	// Notification
	HANDLE_LOBBY_MESSAGE(LobbyResponse::MessageNotif, FAccelByteModelsNotificationMessage, MessageNotif);

	// Matchmaking
	HANDLE_LOBBY_MESSAGE(LobbyResponse::StartMatchmaking, FAccelByteModelsMatchmakingResponse, MatchmakingStartResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::CancelMatchmaking, FAccelByteModelsMatchmakingResponse, MatchmakingCancelResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::ReadyConsentResponse, FAccelByteModelsReadyConsentRequest, ReadyConsentResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::MatchmakingNotif, FAccelByteModelsMatchmakingNotice, MatchmakingNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::ReadyConsentNotif, FAccelByteModelsReadyConsentNotice, ReadyConsentNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::RematchmakingNotif, FAccelByteModelsRematchmakingNotice, RematchmakingNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::DsNotif, FAccelByteModelsDsNotice, DsNotif);

	// Friends
	HANDLE_LOBBY_MESSAGE(LobbyResponse::RequestFriends, FAccelByteModelsRequestFriendsResponse, RequestFriendsResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::Unfriend, FAccelByteModelsUnfriendResponse, UnfriendResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::ListOutgoingFriends, FAccelByteModelsListOutgoingFriendsResponse, ListOutgoingFriendsResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::CancelFriends, FAccelByteModelsCancelFriendsResponse, CancelFriendsResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::ListIncomingFriends, FAccelByteModelsListIncomingFriendsResponse, ListIncomingFriendsResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::AcceptFriends, FAccelByteModelsAcceptFriendsResponse, AcceptFriendsResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::RejectFriends, FAccelByteModelsRejectFriendsResponse, RejectFriendsResponse);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::LoadFriendList, FAccelByteModelsLoadFriendListResponse, LoadFriendListResponse);
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

	// Friends + Notification
	HANDLE_LOBBY_MESSAGE(LobbyResponse::AcceptFriendsNotif, FAccelByteModelsAcceptFriendsNotif, AcceptFriendsNotif);
	HANDLE_LOBBY_MESSAGE(LobbyResponse::RequestFriendsNotif, FAccelByteModelsRequestFriendsNotif, RequestFriendsNotif);

#undef HANDLE_LOBBY_MESSAGE
	ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Warning: Unhandled message %s, Raw: %s"), *lobbyResponseType, *ParsedJson));

}

Lobby::Lobby(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Settings, float PingDelay, float InitialBackoffDelay, float MaxBackoffDelay, float TotalTimeout, TSharedPtr<IWebSocket> WebSocket)
	: Credentials(Credentials)
	, Settings(Settings)
	, PingDelay(PingDelay)
	, InitialBackoffDelay(InitialBackoffDelay)
	, MaxBackoffDelay(MaxBackoffDelay)
	, TotalTimeout(TotalTimeout)
	, BackoffDelay(InitialBackoffDelay)
	, RandomizedBackoffDelay(InitialBackoffDelay)
	, TimeSinceLastPing(0.f)
	, TimeSinceLastReconnect(0.f)
	, WsState(EWebSocketState::Closed)
	, WsEvents(EWebSocketEvent::None)
	, WebSocket(WebSocket)
{
	LobbyTickDelegate = FTickerDelegate::CreateRaw(this, &Lobby::Tick);
	LobbyTickDelegateHandle.Reset();
}

Lobby::~Lobby()
{
	Disconnect();
}

} // Namespace Api
} // Namespace AccelByte
