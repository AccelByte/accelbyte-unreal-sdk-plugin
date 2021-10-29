#include "Blueprints/AccelByteBPLobby.h"

void ULobby::Connect() const
{
	ApiClientPtr->Lobby.Connect();
}

void ULobby::Disconnect() const
{
	return ApiClientPtr->Lobby.Disconnect();
}

void ULobby::UnbindEvent() const
{
	ApiClientPtr->Lobby.UnbindEvent();
}

void ULobby::SetOnConnected(FDHandler OnConnected) const
{
	ApiClientPtr->Lobby.SetConnectSuccessDelegate(
		FVoidHandler::CreateLambda(
			[OnConnected]()
			{
				auto _ = OnConnected.ExecuteIfBound();
			}));
}

void ULobby::StartMatchmaking(
	FStartMatchmakingRequest const& Request,
	FDStartMatchmakingResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetStartMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendStartMatchmaking(Request.gameMode);
}

void ULobby::CancelMatchmaking(
	FCancelMatchmakingRequest const& Request,
	FDCancelMatchmakingResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetCancelMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendCancelMatchmaking(Request.gameMode, Request.isTempParty);
}

void ULobby::SetReadyConsent(
	FSetReadyConsentRequest const& Request,
	FDSetReadyConsentResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetReadyConsentResponseDelegate(
		Api::Lobby::FReadyConsentResponse::CreateLambda(
			[OnResponse](FAccelByteModelsReadyConsentRequest const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendReadyConsentRequest(Request.matchId);
}

void ULobby::SetOnMatchmaking(FDMatchmakingNotif OnNotif)
{
	ApiClientPtr->Lobby.SetMatchmakingNotifDelegate(
		Api::Lobby::FMatchmakingNotif::CreateLambda(
			[OnNotif](FAccelByteModelsMatchmakingNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif)
{
	ApiClientPtr->Lobby.SetReadyConsentNotifDelegate(
		Api::Lobby::FReadyConsentNotif::CreateLambda(
			[OnNotif](FAccelByteModelsReadyConsentNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnRematchmaking(FDRematchmakingNotif OnNotif)
{
	ApiClientPtr->Lobby.SetRematchmakingNotifDelegate(
		Api::Lobby::FRematchmakingNotif::CreateLambda(
			[OnNotif](FAccelByteModelsRematchmakingNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

bool ULobby::IsConnected() const
{
	return ApiClientPtr->Lobby.IsConnected();
}

void ULobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}
