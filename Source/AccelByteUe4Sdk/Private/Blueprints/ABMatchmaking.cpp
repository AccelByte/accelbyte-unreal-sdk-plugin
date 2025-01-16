// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABMatchmaking.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;

void UABMatchmaking::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABMatchmaking::StartMatchmaking(
	FStartMatchmakingRequest const& Request,
	FDStartMatchmakingResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetStartMatchmakingResponseDelegate(
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
		LobbyPtr->SendStartMatchmaking(Request.gameMode);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABMatchmaking::CancelMatchmaking(
	FCancelMatchmakingRequest const& Request,
	FDCancelMatchmakingResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetCancelMatchmakingResponseDelegate(
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
		LobbyPtr->SendCancelMatchmaking(Request.gameMode, Request.isTempParty);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABMatchmaking::SetReadyConsent(
	FSetReadyConsentRequest const& Request,
	FDSetReadyConsentResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetReadyConsentResponseDelegate(
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
		LobbyPtr->SendReadyConsentRequest(Request.matchId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABMatchmaking::SetRejectConsent(
	FSetRejectConsentRequest const& Request,
	FDSetRejectConsentResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetRejectConsentResponseDelegate(
			Api::Lobby::FRejectConsentResponse::CreateLambda(
				[OnResponse](FAccelByteModelsRejectConsentRequest const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->SendRejectConsentRequest(Request.matchId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABMatchmaking::SetOnMatchmaking(FDMatchmakingNotif OnNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetMatchmakingNotifDelegate(
			Api::Lobby::FMatchmakingNotif::CreateLambda(
				[OnNotif](FAccelByteModelsMatchmakingNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMatchmaking::SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetReadyConsentNotifDelegate(
			Api::Lobby::FReadyConsentNotif::CreateLambda(
				[OnNotif](FAccelByteModelsReadyConsentNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMatchmaking::SetOnSetRejectConsent(FDSetRejectConsentNotif OnNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetRejectConsentNotifDelegate(
			Api::Lobby::FRejectConsentNotif::CreateLambda(
				[OnNotif](FAccelByteModelsRejectConsentNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMatchmaking::SetOnRematchmaking(FDRematchmakingNotif OnNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetRematchmakingNotifDelegate(
			Api::Lobby::FRematchmakingNotif::CreateLambda(
				[OnNotif](FAccelByteModelsRematchmakingNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABMatchmaking::SetOnDs(FDDsNotif OnNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetDsNotifDelegate(
			Api::Lobby::FDsNotif::CreateLambda(
				[OnNotif](FAccelByteModelsDsNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}