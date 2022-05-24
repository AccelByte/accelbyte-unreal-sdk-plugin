// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABMessage.h"

void UABMessage::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABMessage::SendPing() 
{
	ApiClientPtr->Lobby.SendPing();
}

FString UABMessage::SendPrivateMessage(
	FSendPrivateMessageRequest const& Request,
	FDPersonalChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPrivateMessageResponseDelegate(
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

	return ApiClientPtr->Lobby.SendPrivateMessage(Request.UserID, Request.Message);
}

FString UABMessage::SendPartyMessage(
	FSendPartyMessageRequest const& Request,
	FDPartyChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPartyMessageResponseDelegate(
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

	return ApiClientPtr->Lobby.SendPartyMessage(Request.Message);
}

FString UABMessage::SendJoinDefaultChannelChatRequest(
	FDJoinDefaultChannelChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetJoinChannelChatResponseDelegate(
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

	return ApiClientPtr->Lobby.SendJoinDefaultChannelChatRequest();
}

FString UABMessage::SendChannelMessage(
	FSendChannelMessageRequest const& Request,
	FDChannelChatResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetChannelMessageResponseDelegate(
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

	return ApiClientPtr->Lobby.SendChannelMessage(Request.Message);
}

void UABMessage::SetPrivateMessageNotifDelegate(FDPersonalChatNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPrivateMessageNotifDelegate(
		Api::Lobby::FPersonalChatNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPersonalMessageNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABMessage::SetMessageNotifDelegate(FDMessageNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetMessageNotifDelegate(
		Api::Lobby::FMessageNotif::CreateLambda(
			[OnNotif](FAccelByteModelsNotificationMessage const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABMessage::SetChannelMessageNotifDelegate(FDChannelChatNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetChannelMessageNotifDelegate(
		Api::Lobby::FChannelChatNotif::CreateLambda(
			[OnNotif](FAccelByteModelsChannelMessageNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}
