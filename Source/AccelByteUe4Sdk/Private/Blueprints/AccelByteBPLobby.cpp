#include "Blueprints/AccelByteBPLobby.h"

void UABLobby::Connect() const
{
	ApiClientPtr->Lobby.Connect();
}

void UABLobby::Disconnect() const
{
	return ApiClientPtr->Lobby.Disconnect();
}

void UABLobby::UnbindEvent() const
{
	ApiClientPtr->Lobby.UnbindEvent();
}

void UABLobby::SetOnConnected(FDHandler OnConnected) const
{
	ApiClientPtr->Lobby.SetConnectSuccessDelegate(
		FVoidHandler::CreateLambda(
			[OnConnected]()
			{
				auto _ = OnConnected.ExecuteIfBound();
			}));
}

bool UABLobby::IsConnected() const
{
	return ApiClientPtr->Lobby.IsConnected();
}

void UABLobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABLobby::SetOnErrorNotification(FDErrorHandler OnErrorNotification) const
{
	ApiClientPtr->Lobby.SetErrorNotifDelegate(
		Api::Lobby::FErrorNotif::CreateLambda(
			[OnErrorNotification](int32 Code, const FString& Message)
			{
				OnErrorNotification.ExecuteIfBound(Code, Message);
			}
		));
}

FString UABLobby::RefreshToken(
	FRefreshTokenRequest const& Request,
	FDRefreshTokenResponseDelegate OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetRefreshTokenDelegate(
		Api::Lobby::FRefreshTokenResponse::CreateLambda(
			[OnResponse](FAccelByteModelsRefreshTokenResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	return ApiClientPtr->Lobby.RefreshToken(Request.AccessToken);
}

void UABLobby::SetRetryParameters(FSetRetryParametersRequest const& Request) const
{
	ApiClientPtr->Lobby.SetRetryParameters(
		Request.NewTotalTimeout,
		Request.NewBackoffDelay,
		Request.NewMaxDelay);
}

void UABLobby::SetTokenGenerator(TSharedPtr<IAccelByteTokenGenerator> TokenGenerator)
{
	ApiClientPtr->Lobby.SetTokenGenerator(TokenGenerator);
}

FString UABLobby::RequestDS(FRequestDSModel const& Request) const
{
	return ApiClientPtr->Lobby.RequestDS(
		Request.SessionID,
		Request.GameMode,
		Request.ClientVersion,
		Request.Region,
		Request.Deployment,
		Request.ServerName);
}