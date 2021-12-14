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
