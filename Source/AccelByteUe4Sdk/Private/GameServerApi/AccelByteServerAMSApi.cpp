// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerAMSApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteWebSocket.h"
#include "Core/FUnrealWebSocketFactory.h"
#include "Core/IWebSocketFactory.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteReport.h"

DEFINE_LOG_CATEGORY(LogAccelByteAMS);

namespace AccelByte
{
namespace GameServerApi
{

ServerAMS::ServerAMS(
	ServerCredentials const& InCredentialsRef,
	ServerSettings const& InSettingsRef,
	FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
	auto Strategy = FReconnectionStrategy::CreateLimitlessStrategy();
	IWebsocketConfigurableReconnectStrategy::SetDefaultReconnectionStrategy(Strategy);
}

ServerAMS::~ServerAMS()
{
	// Only disconnect on destruct if the engine is still around
	if (UObjectInitialized())
	{
		if (AMSHeartbeatTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(AMSHeartbeatTickDelegateHandle);
			AMSHeartbeatTickDelegateHandle.Reset();
		}
		Disconnect(true);
		UnbindDelegates();
	}
}

void ServerAMS::Connect()
{
	FReport::Log(FString(__FUNCTION__));

	if (ServerSettingsRef.DSId.IsEmpty())
	{
		UE_LOG(LogAccelByteAMS, Warning, TEXT("Cannot connect to AMS, DS id is empty!"));
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

	WebSocket->Connect(true);

	UE_LOG(LogAccelByteAMS, Log, TEXT("Connecting to %s"), *AMSWatchdogUrl);
}

void ServerAMS::Disconnect(bool bForceCleanup /*= false*/)
{
	FReport::Log(FString(__FUNCTION__));

	if (WebSocket.IsValid())
	{
		WebSocket->Disconnect(bForceCleanup);
		if (AMSHeartbeatTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(AMSHeartbeatTickDelegateHandle);
			AMSHeartbeatTickDelegateHandle.Reset();
			bHeartbeatJobStarted = false;
		}
	}

	UE_LOG(LogAccelByteAMS, Log, TEXT("Disconnected"));
}

bool ServerAMS::IsConnected() const
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void ServerAMS::UnbindDelegates()
{
	OnConnectSuccessDelegate.Unbind();
	OnConnectErrorDelegate.Unbind();
	OnConnectionClosedDelegate.Unbind();
	ReconnectAttempted.Clear();
	MassiveOutage.Clear();
}

void ServerAMS::CreateWebSocket()
{
	if (WebSocket.IsValid())
	{
		WebSocket.Reset();
	}

	TMap<FString, FString> Headers;
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));

	if (!ServerSettingsRef.AMSServerWatchdogUrl.IsEmpty())
	{
		AMSWatchdogUrl = ServerSettingsRef.AMSServerWatchdogUrl;
	}

	FAccelByteUtilities::AppendModulesVersionToMap(Headers);

	WebSocket = AccelByteWebSocket::Create(*AMSWatchdogUrl,
		TEXT("ws"),
		ServerCredentialsRef.Get(),
		Headers,
		TSharedRef<IWebSocketFactory>(new FUnrealWebSocketFactory()),
		*this /*Reconnection Strategy*/,
		PingDelay);

	WebSocket->OnConnected().AddRaw(this, &ServerAMS::OnConnected);
	WebSocket->OnMessageReceived().AddRaw(this, &ServerAMS::OnMessage);
	WebSocket->OnConnectionError().AddRaw(this, &ServerAMS::OnConnectionError);
	WebSocket->OnConnectionClosed().AddRaw(this, &ServerAMS::OnClosed);
	WebSocket->OnReconnectAttempt().AddRaw(this, &ServerAMS::OnReconnectAttempt);
	WebSocket->OnMassiveOutage().AddRaw(this, &ServerAMS::OnMassiveOutage);

	if (ServerSettingsRef.AMSHeartbeatInterval > 0)
	{
		AMSHeartbeatInterval = FTimespan::FromSeconds(ServerSettingsRef.AMSHeartbeatInterval);
	}
}

void ServerAMS::OnConnected()
{
	UE_LOG(LogAccelByteAMS, Log, TEXT("Connected"));
	OnConnectSuccessDelegate.ExecuteIfBound();
}

void ServerAMS::OnConnectionError(const FString& Error)
{
	UE_LOG(LogAccelByteAMS, Log, TEXT("Error connecting: %s"), *Error);

	OnConnectErrorDelegate.ExecuteIfBound(Error);
}

void ServerAMS::SetOnConnectSuccess(const FConnectSuccess& OnConnectSuccess)
{
	OnConnectSuccessDelegate = OnConnectSuccess;
}

void ServerAMS::SetOnConnectError(const FConnectError& OnConnectError)
{
	OnConnectErrorDelegate = OnConnectError;
}

void ServerAMS::SetOnConnectionClosed(const FConnectionClosed& OnConnectionClosed)
{
	OnConnectionClosedDelegate = OnConnectionClosed;
}

void ServerAMS::OnMessage(const FString& Message)
{
	if (Message.IsEmpty())
	{
		return;
	}

	UE_LOG(LogAccelByteAMS, Log, TEXT("New message received from AMS:\n%s"), *Message);

	TSharedPtr<FJsonObject> MessageObject;
	TSharedRef<TJsonReader<>> MessageReader = TJsonReaderFactory<>::Create(Message);
	if (!FJsonSerializer::Deserialize(MessageReader, MessageObject))
	{
		UE_LOG(LogAccelByteAMS, Warning, TEXT("Failed to parse DS AMS message as a JSON object!"));
		return;
	}
	if (MessageObject->HasTypedField<EJson::Object>(TEXT("drain")))
	{
		OnAMSDrainReceivedDelegate.ExecuteIfBound();
	}
}

void ServerAMS::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogAccelByteAMS, Warning, TEXT("Connection closed. Status code: %d; Reason: %s; Clean: %d"), StatusCode, *Reason, bWasClean);

	OnConnectionClosedDelegate.ExecuteIfBound(StatusCode, Reason, bWasClean);
}

void ServerAMS::OnReconnectAttempt(FReconnectAttemptInfo const& ReconnectAttemptInfo)
{
	ReconnectAttempted.Broadcast(ReconnectAttemptInfo);
}

void ServerAMS::OnMassiveOutage(FMassiveOutageInfo const& MassiveOutageInfo)
{
	MassiveOutage.Broadcast(MassiveOutageInfo);
}

void ServerAMS::SetOnAMSDrainReceivedDelegate(FOnAMSDrainReceived OnAMSDrain)
{

	if (OnAMSDrainReceivedDelegate.IsBound())
	{
		OnAMSDrainReceivedDelegate.Unbind();
	}

	OnAMSDrainReceivedDelegate = OnAMSDrain;
}

void ServerAMS::SendReadyMessage()
{
	if (!IsConnected())
	{
		UE_LOG(LogAccelByteAMS, Warning, TEXT("DS is not connected to AMS yet!"));
		return;
	}

	FString ReadyMessage = FString::Format(TEXT("{\"ready\":{\"dsid\":\"{0}\"}}"), { ServerSettingsRef.DSId });

	UE_LOG(LogAccelByteAMS, Log, TEXT("Send ready message to AMS\n%s"), *ReadyMessage);
	WebSocket->Send(ReadyMessage);

	if (bHeartbeatJobStarted == false)
	{
		bHeartbeatJobStarted = true;
		AMSHeartbeatTickDelegate = FTickerDelegate::CreateRaw(this, &ServerAMS::PeriodicHeartbeat);
		AMSHeartbeatTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(AMSHeartbeatTickDelegate, static_cast<float>(AMSHeartbeatInterval.GetTotalSeconds()));
		UE_LOG(LogAccelByteAMS, Log, TEXT("Added AMS heartbeat event periodically: %i seconds"), (int)AMSHeartbeatInterval.GetTotalSeconds());
	}
}

void ServerAMS::SendHeartbeat()
{
	if (!IsConnected())
	{
		return;
	}

	FString HeartbeatMessage = TEXT("{\"heartbeat\":{}}");

	UE_LOG(LogAccelByteAMS, VeryVerbose, TEXT("Send heartbeat message to AMS\n%s"), *HeartbeatMessage);
	WebSocket->Send(HeartbeatMessage);
}

bool ServerAMS::PeriodicHeartbeat(float DeltaTime)
{
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelByteServerPeriodicHeartBeat"));
#endif
	SendHeartbeat();
	return true;
}

void ServerAMS::SetDSTimeout(int32 NewTimeout)
{
	if (!IsConnected())
	{
		UE_LOG(LogAccelByteAMS, Warning, TEXT("DS is not connected to AMS yet!"));
		return;
	}

	FString SessionTimeoutMessage = FString::Format(TEXT("{\"resetSessionTimeout\":{\"newTimeout\":\"{0}\"}}"), { NewTimeout });

	UE_LOG(LogAccelByteAMS, Log, TEXT("Send set session timeout message to AMS\n%s"), *SessionTimeoutMessage);
	WebSocket->Send(SessionTimeoutMessage);
}

void ServerAMS::ResetDSTimeout()
{
	if (!IsConnected())
	{
		UE_LOG(LogAccelByteAMS, Warning, TEXT("DS is not connected to AMS yet!"));
		return;
	}

	FString SessionTimeoutMessage = TEXT("{\"resetSessionTimeout\":{}}");

	UE_LOG(LogAccelByteAMS, Log, TEXT("Send reset session timeout message to AMS\n%s"), *SessionTimeoutMessage);
	WebSocket->Send(SessionTimeoutMessage);
}

}
}
