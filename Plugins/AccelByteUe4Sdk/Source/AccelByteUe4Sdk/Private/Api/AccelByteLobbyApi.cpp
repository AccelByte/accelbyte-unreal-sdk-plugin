// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteLobbyApi.h"
#include "ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"
#include "AccelByteLobbyApi.h"

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
        const FString StartMatchmaking = TEXT("matchmakingResponse");
    }

    namespace Prefix
    {
        const FString Party = TEXT("party");
        const FString Chat = TEXT("chat");
        const FString Presence = TEXT("presence");
        const FString Matchmaking = TEXT("matchmaking");
    }

Lobby & Lobby::Get()
{
    // Deferred/lazy initialization
    // Thread-safe in C++11
    static Lobby Instance;
    return Instance;
}

void Lobby::Connect()
{
	LobbyTickDelegate = FTickerDelegate::CreateRaw(this, &Lobby::Tick);
	TMap<FString, FString> Headers;
	Headers.Add("Authorization", "Bearer " + Credentials::Get().GetUserAccessToken());
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(*Settings::LobbyServerUrl, TEXT("wss"), Headers);
	WebSocket->OnMessage().AddRaw(this, &Lobby::OnMessage);
	WebSocket->OnConnected().AddRaw(this, &Lobby::OnConnected);
	WebSocket->OnConnectionError().AddRaw(this, &Lobby::OnConnectionError);
	WebSocket->OnClosed().AddRaw(this, &Lobby::OnClosed);
	WebSocket->Connect();	
	UE_LOG(LogTemp, Display, TEXT("Connecting to %s"), *Settings::LobbyServerUrl);
}

void Lobby::Disconnect()
{
	if(WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FTicker::GetCoreTicker().RemoveTicker(LobbyTickDelegateHandle);
		WebSocket->OnMessage().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->OnClosed().Clear();
		WebSocket->Close();
		WebSocket = nullptr;
	}
	UE_LOG(LogTemp, Display, TEXT("Disconnected"))
}

bool Lobby::IsConnected() const
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void Lobby::SendPing()
{
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
    return SendRawRequest(LobbyRequest::PersonalChat, Prefix::Chat,
        FString::Printf(TEXT("to: %s\npayload: %s\n"), *UserId, *Message));
}

FString Lobby::SendPartyMessage(const FString& Message)
{
    return SendRawRequest(LobbyRequest::PartyChat, Prefix::Chat,
        FString::Printf(TEXT("payload: %s\n"), *Message));
}

//-------------------------------------------------------------------------------------------------
// Party
//-------------------------------------------------------------------------------------------------
FString Lobby::SendInfoPartyRequest()
{
    return SendRawRequest(LobbyRequest::PartyInfo, Prefix::Party);
}

FString Lobby::SendCreatePartyRequest()
{
    return SendRawRequest(LobbyRequest::CreateParty, Prefix::Party);
}

FString Lobby::SendLeavePartyRequest()
{
    return SendRawRequest(LobbyRequest::LeaveParty, Prefix::Party);
}

FString Lobby::SendInviteToPartyRequest(const FString& UserId)
{
    return SendRawRequest(LobbyRequest::InviteParty, Prefix::Party,
        FString::Printf(TEXT("friendID: %s"), *UserId));
}

FString Lobby::SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken)
{
    return SendRawRequest(LobbyRequest::JoinParty, Prefix::Party,
        FString::Printf(TEXT("partyID: %s\ninvitationToken: %s"), *PartyId, *InvitationToken));
}

FString Lobby::SendKickPartyMemberRequest(const FString& UserId)
{
    return SendRawRequest(LobbyRequest::KickParty, Prefix::Party,
        FString::Printf(TEXT("memberID: %s\n"), *UserId));
}

//-------------------------------------------------------------------------------------------------
// Presence
//-------------------------------------------------------------------------------------------------
FString Lobby::SendSetPresenceStatus(const Availability Availability, const FString& Activity)
{
    return SendRawRequest(LobbyRequest::SetPresence, Prefix::Presence,
        FString::Printf(TEXT("availability: %d\nactivity: %s\n"), (int)Availability, *Activity));
}

FString Lobby::SendGetOnlineUsersRequest()
{
    return SendRawRequest(LobbyRequest::FriendsPresence, Prefix::Presence);
}

//-------------------------------------------------------------------------------------------------
// Notification
//-------------------------------------------------------------------------------------------------
void Lobby::GetAllAsyncNotification()
{
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
FString Lobby::SendStartMatchmaking(FString GameMode, FString PartyId, TArray<FString> MemberIds)
{
    return SendRawRequest(LobbyRequest::StartMatchmaking, Prefix::Matchmaking,
        FString::Printf(TEXT("gameMode: %s\npartyId: %s\nmemberId: [%s,]"), 
            *GameMode, *PartyId, *FString::Join(MemberIds, TEXT(","))));
}

FString Lobby::SendCancelMatchmaking(FString PartyId)
{
    return SendRawRequest(LobbyRequest::CancelMatchmaking, Prefix::Matchmaking,
        FString::Printf(TEXT("partyId: %s\n"),*PartyId));
}

void Lobby::UnbindEvent()
{
    PartyLeaveNotif.Unbind();
    PartyInviteNotif.Unbind();
    PartyGetInvitedNotif.Unbind();
    PartyJoinNotif.Unbind();
    PartyKickNotif.Unbind();
    PersonalChatNotif.Unbind();
    PartyChatNotif.Unbind();
    FriendStatusNotif.Unbind();	
}

void Lobby::OnConnected()
{
	UE_LOG(LogTemp, Display, TEXT("Connected"))
	ConnectSuccess.ExecuteIfBound();
	// start timer, ping every 4 second
	LobbyTickDelegateHandle = FTicker::GetCoreTicker().AddTicker(LobbyTickDelegate, 4);
}

void Lobby::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Display, TEXT("Error connecting: %s"), *Error)
	ConnectError.ExecuteIfBound(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed), ErrorMessages::Default.at(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed)) + TEXT(" Reason: ") + Error);
}

void Lobby::OnClosed(int32 StatusCode, const FString& Reason, bool WasClean)
{
	UE_LOG(LogTemp, Display, TEXT("Connection closed. Status code: %d  Reason: %s Clean: %d"), StatusCode, *Reason, WasClean)
	ConnectionClosed.ExecuteIfBound(StatusCode, Reason, WasClean);
}

FString Lobby::SendRawRequest(FString MessageType, FString MessageIDPrefix, FString CustomPayload)
{
    if (WebSocket.IsValid() && WebSocket->IsConnected())
    {
        FString MessageID = GenerateMessageID(MessageIDPrefix);
        FString Content = FString::Printf(TEXT("type: %s\nid: %s"),*MessageType, *MessageID);
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
	SendPing();
	return true;
}

FString Lobby::GenerateMessageID(FString Prefix)
{
    return FString::Printf(TEXT("%s-%d"), *Prefix, FMath::RandRange(1000,9999));
}

FString Lobby::LobbyMessageToJson(FString Message)
{
    FString ParsedJson = TEXT("{");
    TArray<FString> Out;
    Message.ParseIntoArray(Out, TEXT("\n"), true);
    for (int i = 0; i < Out.Num(); i++)
    {
        FString currentLine = Out[i];
        if (currentLine.Contains("["))
        {
            ParsedJson += "\"";
            currentLine.ReplaceInline(TEXT(": "), TEXT("\":"));
            if (!currentLine.Contains("[]"))
            {
                currentLine.ReplaceInline(TEXT("["), TEXT("[\""));
                currentLine.ReplaceInline(TEXT(",]"), TEXT("\"]"));
                currentLine.ReplaceInline(TEXT(","), TEXT("\",\""));
            }
            ParsedJson += currentLine;
        }
        else
        {
            ParsedJson += "\"";
            currentLine.ReplaceInline(TEXT(": "), TEXT("\":\""));
            ParsedJson += currentLine;
            ParsedJson += "\"";
        }
        if (i < Out.Num() - 1)
        {
            ParsedJson += ",";
        }
    }
    ParsedJson += TEXT("}");
    return ParsedJson;
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
    UE_LOG(LogTemp, Display, TEXT("Type: %s"), *lobbyResponseType);

#define HANDLE_LOBBY_MESSAGE(MessageType, Model, ResponseCallback) \
    if (lobbyResponseType.Equals(MessageType)) \
    { \
        Model Result; \
        bool bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0); \
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

	// Default
	if (lobbyResponseType.Equals(LobbyResponse::ConnectedNotif))
	{ 
		ConnectSuccess.ExecuteIfBound();
		return; 
	}



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
    HANDLE_LOBBY_MESSAGE(LobbyResponse::StartMatchmaking, FAccelByteModelsMatchmakingResponse, MatchmakingResponse);

#undef HANDLE_LOBBY_MESSAGE
    ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Warning: Unhandled message %s, Raw: %s"), *lobbyResponseType, *ParsedJson));

}

Lobby::Lobby()
{
}

Lobby::~Lobby()
{
    Disconnect();
}

} // Namespace Api
} // Namespace AccelByte
