// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABMatchmaking.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"

void UABMatchmaking::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABMatchmaking::StartMatchmaking(
	FStartMatchmakingRequest const& Request,
	FDStartMatchmakingResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetStartMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendStartMatchmaking(Request.gameMode);
}

void UABMatchmaking::CancelMatchmaking(
	FCancelMatchmakingRequest const& Request,
	FDCancelMatchmakingResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetCancelMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendCancelMatchmaking(Request.gameMode, Request.isTempParty);
}

void UABMatchmaking::SetReadyConsent(
	FSetReadyConsentRequest const& Request,
	FDSetReadyConsentResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetReadyConsentResponseDelegate(
		Api::Lobby::FReadyConsentResponse::CreateLambda(
			[OnResponse](FAccelByteModelsReadyConsentRequest const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendReadyConsentRequest(Request.matchId);
}

void UABMatchmaking::SetOnMatchmaking(FDMatchmakingNotif OnNotif)
{
	ApiClientPtr->Lobby.SetMatchmakingNotifDelegate(
		Api::Lobby::FMatchmakingNotif::CreateLambda(
			[OnNotif](FAccelByteModelsMatchmakingNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABMatchmaking::SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif)
{
	ApiClientPtr->Lobby.SetReadyConsentNotifDelegate(
		Api::Lobby::FReadyConsentNotif::CreateLambda(
			[OnNotif](FAccelByteModelsReadyConsentNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABMatchmaking::SetOnRematchmaking(FDRematchmakingNotif OnNotif)
{
	ApiClientPtr->Lobby.SetRematchmakingNotifDelegate(
		Api::Lobby::FRematchmakingNotif::CreateLambda(
			[OnNotif](FAccelByteModelsRematchmakingNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABMatchmaking::SetOnDs(FDDsNotif OnNotif)
{
	ApiClientPtr->Lobby.SetDsNotifDelegate(
		Api::Lobby::FDsNotif::CreateLambda(
			[OnNotif](FAccelByteModelsDsNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}