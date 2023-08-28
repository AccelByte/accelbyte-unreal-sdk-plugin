#include "Blueprints/AccelByteBPLobby.h"

using namespace AccelByte;

void UABLobby::Connect() 
{
	ApiClientPtr->Lobby.Connect();
}

void UABLobby::Disconnect() 
{
	return ApiClientPtr->Lobby.Disconnect();
}

void UABLobby::UnbindEvent() 
{
	ApiClientPtr->Lobby.UnbindEvent();
}

bool UABLobby::IsConnected() 
{
	return ApiClientPtr->Lobby.IsConnected();
}

void UABLobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABLobby::SetOnConnected(FDHandler OnConnected) 
{
	ApiClientPtr->Lobby.SetConnectSuccessDelegate(
		FVoidHandler::CreateLambda(
			[OnConnected]()
			{
				auto _ = OnConnected.ExecuteIfBound();
			}));
}

void UABLobby::SetOnConnectFailed(const FDErrorHandler& OnConnectError)
{
	ApiClientPtr->Lobby.SetConnectFailedDelegate(
		FErrorHandler::CreateLambda([OnConnectError](int32 Code, const FString& Message)
		{
			OnConnectError.ExecuteIfBound(Code, Message);
		}));
}

void UABLobby::SetOnDisconnect(const FDDisconnectNotifDelegate& OnDisconnected)
{
	ApiClientPtr->Lobby.SetDisconnectNotifDelegate(
		Api::Lobby::FDisconnectNotif::CreateLambda([OnDisconnected](const FAccelByteModelsDisconnectNotif& Notif)
		{
			OnDisconnected.ExecuteIfBound(Notif);
		}));
}

void UABLobby::SetOnConnectionClosed(FDConnectionClosedDelegate OnConnectionClosed)
{
	ApiClientPtr->Lobby.SetConnectionClosedDelegate(
		Api::Lobby::FConnectionClosed::CreateLambda([OnConnectionClosed](int32  StatusCode, const FString& Reason, bool  bWasClean)
		{
			OnConnectionClosed.ExecuteIfBound(StatusCode, Reason, bWasClean);
		}));
}

void UABLobby::SetOnErrorNotification(FDErrorHandler OnErrorNotification) 
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
	FDErrorHandler OnError) 
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

void UABLobby::SetRetryParameters(FSetRetryParametersRequest const& Request) 
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

FString UABLobby::RequestDS(FRequestDSModel const& Request) 
{
	return ApiClientPtr->Lobby.RequestDS(
		Request.SessionID,
		Request.GameMode,
		Request.ClientVersion,
		Request.Region,
		Request.Deployment,
		Request.ServerName);
}