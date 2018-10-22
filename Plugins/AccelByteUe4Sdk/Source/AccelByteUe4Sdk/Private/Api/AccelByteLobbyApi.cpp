// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteLobbyApi.h"
#include "ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Lobby & Lobby::Get()
{
    // Deferred/lazy initialization
    // Thread-safe in C++11
    static Lobby Instance;
    return Instance;
}

void Lobby::Connect(const FConnectSuccess& OnSuccess, const FErrorHandler& OnError, const FConnectionClosed& OnConnectionClosed)
{
	ConnectionClosed = OnConnectionClosed;
	ConnectSuccess = OnSuccess;
	ConnectError = OnError;

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

void Lobby::SendPrivateMessage(const FString & UserId, const FString& Message)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:1to1Chat\nid:%s\nto:%s\npayload:%s\n"), *FGuid::NewGuid().ToString(EGuidFormats::Digits), *UserId, *Message);
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::SendPartyMessage(const FString & Message)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:partyChat\nid:%s\npayload:%s\n"), *FGuid::NewGuid().ToString(EGuidFormats::Digits), *Message);
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::SendInfoPartyRequest()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:info\n"));
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::SendCreatePartyRequest()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:create\n"));
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::SendLeavePartyRequest()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:leave\n"));
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::SendInviteToPartyRequest(const FString& UserId)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:invite\nfriendID:%s\n"), *UserId);
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::SendAcceptInvitationRequest(const FString& PartyId, FString InvitationToken)
{
	
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		FString Content = FString::Printf(TEXT("type:join\npartyID:%s\ninvitationToken:%s\n"), *PartyId, *InvitationToken);
		WebSocket->Send(Content);

		UE_LOG(LogTemp, Display, TEXT("Sending request: %s"), *Content)
	}
}

void Lobby::BindDelegates(const FPrivateMessageNotice& OnPrivateMessageReceived,
	const FPartyMessageNotice& OnPartyMessageReceived,
	const FInfoPartyResponse& OnInfoPartyResponse,
	const FCreatePartyResponse& OnCreatePartyResponse,
	const FLeavePartyResponse& OnLeavePartyResponse,
	const FInviteToPartyResponse& OnInviteToPartyResponse,
	const FPartyInvitationNotice& OnPartyInvitationReceived,
	const FAcceptInvitationResponse& OnAcceptInvitationResponse,
	const FPartyInvitationAcceptanceNotice& OnPartyInvitationAcceptanceReceived)
{
	PrivateMessageNotice = OnPrivateMessageReceived;
	PartyMessageNotice = OnPartyMessageReceived;
	InfoPartyResponse = OnInfoPartyResponse;
	CreatePartyResponse = OnCreatePartyResponse;
	LeavePartyResponse = OnLeavePartyResponse;
	InviteToPartyResponse = OnInviteToPartyResponse;
	PartyInvitationNotice = OnPartyInvitationReceived;
	AcceptInvitationResponse = OnAcceptInvitationResponse;
	PartyInvitationAcceptanceNotice = OnPartyInvitationAcceptanceReceived;
}


void Lobby::UnbindDelegates()
{
	PrivateMessageNotice.Unbind();
	PartyMessageNotice.Unbind();
	InfoPartyResponse.Unbind();
	CreatePartyResponse.Unbind();
	LeavePartyResponse.Unbind();
	InviteToPartyResponse.Unbind();
	PartyInvitationNotice.Unbind();
	AcceptInvitationResponse.Unbind();
	PartyInvitationAcceptanceNotice.Unbind();
}

void Lobby::OnConnected()
{
	UE_LOG(LogTemp, Display, TEXT("Connected"))
	ConnectSuccess.ExecuteIfBound();
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

void Lobby::OnMessage(const FString& Message)
{
	UE_LOG(LogTemp, Display, TEXT("I GOT SOMETHING: %s"), *Message)

	FString ParsedJson = TEXT("{\"");
	ParsedJson += Message;
	ParsedJson.RemoveFromEnd(TEXT("\n"));
	ParsedJson += TEXT("\"}");
	ParsedJson.ReplaceInline(TEXT(":"), TEXT("\":\""));
	ParsedJson.ReplaceInline(TEXT("\n"), TEXT("\",\""));
	
	UE_LOG(LogTemp, Display, TEXT("Parsed: %s"), *ParsedJson)

	FAccelByteModelsLobbyType Type;
	FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Type, 0, 0);
	
	const FString ResponseType = Type.Type;
	UE_LOG(LogTemp, Display, TEXT("Type: %s"), *ResponseType)

	if(ResponseType.Compare(TEXT("1to1Chat")) == 0)
	{
		FAccelByteModelsPrivateMessageNotice Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		PrivateMessageNotice.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("partyChat")) == 0)
	{
		FAccelByteModelsPartyMessageNotice Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		PartyMessageNotice.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("info")) == 0)
	{
		FAccelByteModelsInfoPartyResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		InfoPartyResponse.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("create")) == 0)
	{
		FAccelByteModelsCreatePartyResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		CreatePartyResponse.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("leave")) == 0)
	{
		FAccelByteModelsLeavePartyResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		LeavePartyResponse.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("invite")) == 0)
	{
		FAccelByteModelsInviteToPartyResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		InviteToPartyResponse.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("invitation")) == 0)
	{
		FAccelByteModelsPartyInvitationNotice Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		PartyInvitationNotice.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("join")) == 0)
	{
		FAccelByteModelsAcceptInvitationReponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		AcceptInvitationResponse.ExecuteIfBound(Result);
	}
	else if (ResponseType.Compare(TEXT("joinNotice")) == 0)
	{
		FAccelByteModelsPartyInvitationAcceptanceNotice Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(ParsedJson, &Result, 0, 0);
		PartyInvitationAcceptanceNotice.ExecuteIfBound(Result);
	}

	
}

Lobby::Lobby()
{
}

Lobby::~Lobby()
{
}

} // Namespace Api
} // Namespace AccelByte
