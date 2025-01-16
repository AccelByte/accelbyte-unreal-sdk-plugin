// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABMessage.h"

using namespace AccelByte;

void UABMessage::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABMessage::SendPing() 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SendPing();
	}
}

FString UABMessage::SendPrivateMessage(
	FSendPrivateMessageRequest const& Request,
	FDPersonalChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPrivateMessageResponseDelegate(
			Api::Lobby::FPersonalChatResponse::CreateLambda(
				[OnResponse](FAccelByteModelsPersonalMessageResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->SendPrivateMessage(Request.UserID, Request.Message);
	}
	return TEXT("");
}

FString UABMessage::SendPartyMessage(
	FSendPartyMessageRequest const& Request,
	FDPartyChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyMessageResponseDelegate(
			Api::Lobby::FPartyChatResponse::CreateLambda(
				[OnResponse](FAccelByteModelsPartyMessageResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->SendPartyMessage(Request.Message);
	}
	return TEXT("");
}

FString UABMessage::SendJoinDefaultChannelChatRequest(
	FDJoinDefaultChannelChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetJoinChannelChatResponseDelegate(
			Api::Lobby::FJoinDefaultChannelChatResponse::CreateLambda(
				[OnResponse](FAccelByteModelsJoinDefaultChannelResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->SendJoinDefaultChannelChatRequest();
	}
	return TEXT("");
}

FString UABMessage::SendChannelMessage(
	FSendChannelMessageRequest const& Request,
	FDChannelChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetChannelMessageResponseDelegate(
			Api::Lobby::FChannelChatResponse::CreateLambda(
				[OnResponse](FAccelByteModelsChannelMessageResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->SendChannelMessage(Request.Message);
	}
	return TEXT("");
}

void UABMessage::SetPrivateMessageNotifDelegate(FDPersonalChatNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPrivateMessageNotifDelegate(
			Api::Lobby::FPersonalChatNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPersonalMessageNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMessage::SetMessageNotifDelegate(FDMessageNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetMessageNotifDelegate(
			Api::Lobby::FMessageNotif::CreateLambda(
				[OnNotif](FAccelByteModelsNotificationMessage const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMessage::SetChannelMessageNotifDelegate(FDChannelChatNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetChannelMessageNotifDelegate(
			Api::Lobby::FChannelChatNotif::CreateLambda(
				[OnNotif](FAccelByteModelsChannelMessageNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMessage::SetOnPartyChatNotification(FDPartyChatNotif OnPartyChatNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyChatNotifDelegate(
			Api::Lobby::FPartyChatNotif::CreateLambda([OnPartyChatNotif](const FAccelByteModelsPartyMessageNotice& Notif)
				{
					OnPartyChatNotif.ExecuteIfBound(Notif);
				}));
	}
}
