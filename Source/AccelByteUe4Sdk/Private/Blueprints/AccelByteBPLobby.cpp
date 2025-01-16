#include "Blueprints/AccelByteBPLobby.h"

using namespace AccelByte;

void UABLobby::Connect() 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->Connect();
	}
}

void UABLobby::Disconnect() 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		return LobbyPtr->Disconnect();
	}
}

void UABLobby::UnbindEvent() 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->UnbindEvent();
	}
}

bool UABLobby::IsConnected() 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		return LobbyPtr->IsConnected();
	}
	return false;
}

void UABLobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABLobby::SetOnConnected(FDHandler OnConnected) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetConnectSuccessDelegate(
			FVoidHandler::CreateLambda(
				[OnConnected]()
				{
					auto _ = OnConnected.ExecuteIfBound();
				}));
	}
}

void UABLobby::SetOnConnectFailed(const FDErrorHandler& OnConnectError)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetConnectFailedDelegate(
			FErrorHandler::CreateLambda([OnConnectError](int32 Code, const FString& Message)
				{
					OnConnectError.ExecuteIfBound(Code, Message);
				}));
	}
}

void UABLobby::SetOnDisconnect(const FDDisconnectNotifDelegate& OnDisconnected)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetDisconnectNotifDelegate(
			Api::Lobby::FDisconnectNotif::CreateLambda([OnDisconnected](const FAccelByteModelsDisconnectNotif& Notif)
				{
					OnDisconnected.ExecuteIfBound(Notif);
				}));
	}
}

void UABLobby::SetOnConnectionClosed(FDConnectionClosedDelegate OnConnectionClosed)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetConnectionClosedDelegate(
			Api::Lobby::FConnectionClosed::CreateLambda([OnConnectionClosed](int32  StatusCode, const FString& Reason, bool  bWasClean)
				{
					OnConnectionClosed.ExecuteIfBound(StatusCode, Reason, bWasClean);
				}));
	}
}

void UABLobby::SetOnErrorNotification(FDErrorHandler OnErrorNotification) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetErrorNotifDelegate(
			Api::Lobby::FErrorNotif::CreateLambda(
				[OnErrorNotification](int32 Code, const FString& Message)
				{
					OnErrorNotification.ExecuteIfBound(Code, Message);
				}
		));
	}
}

FString UABLobby::RefreshToken(
	FRefreshTokenRequest const& Request,
	FDRefreshTokenResponseDelegate OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetRefreshTokenDelegate(
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
		return LobbyPtr->RefreshToken(Request.AccessToken);
	}
	return TEXT("");
}

void UABLobby::SetRetryParameters(FSetRetryParametersRequest const& Request) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetDefaultReconnectionStrategy(FReconnectionStrategy::CreateBalancedStrategy(
			FReconnectionStrategy::FBalancedMaxRetryInterval(Request.NewTotalTimeout),
			FReconnectionStrategy::FTotalTimeoutDuration(FTimespan::FromSeconds(Request.NewBackoffDelay)),
			FReconnectionStrategy::FInitialBackoffDelay(FTimespan::FromSeconds(Request.NewMaxDelay))
		)
		);
	}
}

void UABLobby::SetTokenGenerator(TSharedPtr<IAccelByteTokenGenerator> TokenGenerator)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetTokenGenerator(TokenGenerator);
	}
}

FString UABLobby::RequestDS(FRequestDSModel const& Request) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		return LobbyPtr->RequestDS(
			Request.SessionID,
			Request.GameMode,
			Request.ClientVersion,
			Request.Region,
			Request.Deployment,
			Request.ServerName);
	}
	return TEXT("");
}