// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerWatchdogApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteWebSocket.h"
#include "Core/FUnrealWebSocketFactory.h"
#include "Core/IWebSocketFactory.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteReport.h"

DEFINE_LOG_CATEGORY(LogAccelByteWatchdog);

namespace AccelByte
{
namespace GameServerApi
{

ServerWatchdog::ServerWatchdog(
	ServerCredentials const& InCredentialsRef,
	ServerSettings const& InSettingsRef,
	FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

ServerWatchdog::~ServerWatchdog()
{
	// Only disconnect on destruct if the engine is still around
	if (UObjectInitialized())
	{
		if (WatchdogHeartbeatTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(WatchdogHeartbeatTickDelegateHandle);
			WatchdogHeartbeatTickDelegateHandle.Reset();
		}
		Disconnect(true);
		UnbindDelegates();
	}
}

void ServerWatchdog::Connect()
{
	FReport::Log(FString(__FUNCTION__));

	if (ServerSettingsRef.DSId.IsEmpty())
	{
		UE_LOG(LogAccelByteWatchdog, Warning, TEXT("Cannot connect to watchdog, DS id is empty!"));
		return;
	}

	if (!WebSocket.IsValid())
	{
		CreateWebSocket();
	}

	if (WebSocket->IsConnected())
	{
		return;
	}

	WebSocket->Connect();

	UE_LOG(LogAccelByteWatchdog, Log, TEXT("Connecting to %s"), *ServerSettingsRef.WatchdogServerUrl);
}

void ServerWatchdog::Disconnect(bool bForceCleanup /*= false*/)
{
	FReport::Log(FString(__FUNCTION__));

	if (WebSocket.IsValid())
	{
		WebSocket->Disconnect(bForceCleanup);
		if (WatchdogHeartbeatTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(WatchdogHeartbeatTickDelegateHandle);
			WatchdogHeartbeatTickDelegateHandle.Reset();
			bHeartbeatJobStarted = false;
		}
	}

	UE_LOG(LogAccelByteWatchdog, Log, TEXT("Disconnected"));
}

bool ServerWatchdog::IsConnected() const
{
	FReport::Log(FString(__FUNCTION__));

	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void ServerWatchdog::UnbindDelegates()
{
	OnConnectSuccessDelegate.Unbind();
	OnConnectErrorDelegate.Unbind();
	OnConnectionClosedDelegate.Unbind();
}

void ServerWatchdog::CreateWebSocket()
{
	if (WebSocket.IsValid())
	{
		WebSocket.Reset();
	}

	TMap<FString, FString> Headers;
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));

	WebSocket = AccelByteWebSocket::Create(*ServerSettingsRef.WatchdogServerUrl,
		TEXT("ws"),
		ServerCredentialsRef,
		Headers,
		TSharedRef<IWebSocketFactory>(new FUnrealWebSocketFactory()));

	WebSocket->OnConnected().AddRaw(this, &ServerWatchdog::OnConnected);
	WebSocket->OnMessageReceived().AddRaw(this, &ServerWatchdog::OnMessage);
	WebSocket->OnConnectionError().AddRaw(this, &ServerWatchdog::OnConnectionError);
	WebSocket->OnConnectionClosed().AddRaw(this, &ServerWatchdog::OnClosed);
	WatchdogHeartbeatInterval = FTimespan::FromSeconds(ServerSettingsRef.WatchdogHeartbeatInterval);
}

void ServerWatchdog::OnConnected()
{
	UE_LOG(LogAccelByteWatchdog, Log, TEXT("Connected"));
	OnConnectSuccessDelegate.ExecuteIfBound();
}

void ServerWatchdog::OnConnectionError(const FString& Error)
{
	UE_LOG(LogAccelByteWatchdog, Log, TEXT("Error connecting: %s"), *Error);

	OnConnectErrorDelegate.ExecuteIfBound(Error);
}

void ServerWatchdog::SetOnConnectSuccess(const FConnectSuccess& OnConnectSuccess)
{
	OnConnectSuccessDelegate = OnConnectSuccess;
}

void ServerWatchdog::SetOnConnectError(const FConnectError& OnConnectError)
{
	OnConnectErrorDelegate = OnConnectError;
}

void ServerWatchdog::SetOnConnectionClosed(const FConnectionClosed& OnConnectionClosed)
{
	OnConnectionClosedDelegate = OnConnectionClosed;
}

void ServerWatchdog::OnMessage(const FString& Message)
{
	UE_LOG(LogAccelByteWatchdog, Log, TEXT("New message received from Watchdog:\n%s"), *Message);

	TSharedPtr<FJsonObject> MessageObject;
	TSharedRef<TJsonReader<>> MessageReader = TJsonReaderFactory<>::Create(Message);
	if (!FJsonSerializer::Deserialize(MessageReader, MessageObject))
	{
		UE_LOG(LogAccelByteWatchdog, Warning, TEXT("Failed to parse DS watchdog message as a JSON object!"));
		return;
	}
	if (MessageObject->HasTypedField<EJson::Object>(TEXT("drain")))
	{
		OnWatchdogDrainReceivedDelegate.ExecuteIfBound();
	}
}

void ServerWatchdog::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogAccelByteWatchdog, Warning, TEXT("Connection closed. Status code: %d; Reason: %s; Clean: %d"), StatusCode, *Reason, bWasClean);

	OnConnectionClosedDelegate.ExecuteIfBound(StatusCode, Reason, bWasClean);
}

void ServerWatchdog::SetOnWatchdogDrainReceivedDelegate(FOnWatchdogDrainReceived OnWatchdogDrain)
{

	if (OnWatchdogDrainReceivedDelegate.IsBound())
	{
		OnWatchdogDrainReceivedDelegate.Unbind();
	}

	OnWatchdogDrainReceivedDelegate = OnWatchdogDrain;
}

void ServerWatchdog::SendReadyMessage()
{
	if (!IsConnected())
	{
		return;
	}

	FString ReadyMessage = FString::Format(TEXT("{\"ready\":{\"dsid\":\"{0}\"}}"), { ServerSettingsRef.DSId });
	WebSocket->Send(ReadyMessage);

	if (bHeartbeatJobStarted == false)
	{
		bHeartbeatJobStarted = true;
		WatchdogHeartbeatTickDelegate = FTickerDelegate::CreateRaw(this, &ServerWatchdog::PeriodicHeartbeat);
		WatchdogHeartbeatTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(WatchdogHeartbeatTickDelegate, static_cast<float>(WatchdogHeartbeatInterval.GetTotalSeconds()));
	}
}

void ServerWatchdog::SendHeartbeat()
{
	if (!IsConnected())
	{
		return;
	}

	FString HeartbeatMessage = TEXT("{\"heartbeat\":{}}");
	WebSocket->Send(HeartbeatMessage);
}

bool ServerWatchdog::PeriodicHeartbeat(float DeltaTime)
{
	SendHeartbeat();
	return true;
}

}
}
