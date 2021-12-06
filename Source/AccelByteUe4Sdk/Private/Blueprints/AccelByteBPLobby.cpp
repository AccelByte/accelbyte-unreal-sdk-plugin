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

bool ULobby::IsConnected() const
{
	return ApiClientPtr->Lobby.IsConnected();
}

void ULobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void ULobby::SetOnErrorNotification(FDErrorHandler OnErrorNotification) const
{
	ApiClientPtr->Lobby.SetErrorNotifDelegate(
		Api::Lobby::FErrorNotif::CreateLambda(
			[OnErrorNotification](int32 Code, const FString& Message)
			{
				OnErrorNotification.ExecuteIfBound(Code, Message);
			}
		));
}
