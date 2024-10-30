// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteWebSocket.h"
#include "WebSocketsModule.h"
#include "Core/IWebSocketFactory.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteUtilities.h"
#include "Logging/AccelByteServiceLogger.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteWebsocket, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteWebsocket);

namespace AccelByte
{	
AccelByteWebSocket::AccelByteWebSocket(
	const Credentials& Credentials,
	IWebsocketConfigurableReconnectStrategy& InParentReconnectionStrategyRef,
	float PingDelay
)
	: ParentReconnectionStrategyRef(InParentReconnectionStrategyRef)
	, PingDelay(PingDelay)
	, ClientCreds(&Credentials)
{
	TickerDelegate = FTickerDelegate::CreateRaw(this, &AccelByteWebSocket::Tick);
	TickerDelegateHandle.Reset();
}

AccelByteWebSocket::AccelByteWebSocket(
	const ServerCredentials& Credentials,
	IWebsocketConfigurableReconnectStrategy& InParentReconnectionStrategyRef,
	float PingDelay /*= 30.f*/
	)
	: ParentReconnectionStrategyRef(InParentReconnectionStrategyRef)
	, PingDelay(PingDelay)
	, ServerCreds(&Credentials)
{
	TickerDelegate = FTickerDelegate::CreateRaw(this, &AccelByteWebSocket::Tick);
	TickerDelegateHandle.Reset();
}

AccelByteWebSocket::~AccelByteWebSocket()
{
	/*
	 * DO NOT remove UObjectInitialized check unless it doesn't work.
	 * Checks if the UObject subsystem is active.
	 * If it active then the core ticker or the websocket module also active, and we can safely call WebSocket->Close
	 */
	if (UObjectInitialized())
	{
		Disconnect(true);
	}

	TeardownTicker();
	TeardownWebsocket();

	ClientCreds = nullptr;
	ServerCreds = nullptr;
}

void AccelByteWebSocket::SetupWebSocket()
{
	FReport::Log(FString(__FUNCTION__));

	bWasWsConnectionError = false;

	bConnectTriggered = false;
	OnMessageQueue.Empty();
	OnConnectionClosedQueue.Empty();
	OnConnectionErrorQueue.Empty();
	OnReconnectingAttemptQueue.Empty();
	OnMassiveOutageQueue.Empty();

	TeardownWebsocket();

	UpgradeHeaders.Add(TEXT("x-flight-id"), FAccelByteUtilities::GetFlightId());

	TMap<FString, FString> Headers = UpgradeHeaders;

	if (ClientCreds != nullptr)
	{
		Headers.Add("Authorization", "Bearer " + ClientCreds->GetAccessToken());
	}
	else if (ServerCreds != nullptr)
	{
		Headers.Add("Authorization", "Bearer " + ServerCreds->GetClientAccessToken());
	}
	WebSocket = WebSocketFactory->CreateWebSocket(Url, Protocol, Headers);

	WebSocket->OnMessage().AddRaw(this, &AccelByteWebSocket::OnMessageReceived);
	WebSocket->OnConnected().AddRaw(this, &AccelByteWebSocket::OnConnectionConnected);
	WebSocket->OnConnectionError().AddRaw(this, &AccelByteWebSocket::OnConnectionError);
	WebSocket->OnClosed().AddRaw(this, &AccelByteWebSocket::OnClosed);
}

void AccelByteWebSocket::UpdateUpgradeHeaders(const FString& Key, const FString& Value)
{
	UpgradeHeaders.Emplace(Key, Value);
}

AccelByteWebSocket::FConnectDelegate& AccelByteWebSocket::OnConnected()
{
	return ConnectDelegate;
}

AccelByteWebSocket::FMessageReceiveDelegate& AccelByteWebSocket::OnMessageReceived()
{
	return MessageReceiveDelegate;
}

AccelByteWebSocket::FConnectionErrorDelegate& AccelByteWebSocket::OnConnectionError()
{
	return ConnectionErrorDelegate;
}

AccelByteWebSocket::FConnectionCloseMulticastDelegate& AccelByteWebSocket::OnConnectionClosed()
{
	return ConnectionCloseDelegate;
}

AccelByteWebSocket::FReconnectAttemptMulticastDelegate& AccelByteWebSocket::OnReconnectAttempt()
{
	return ReconnectAttemptDelegate;
}

AccelByteWebSocket::FMassiveOutageMulticastDelegate& AccelByteWebSocket::OnMassiveOutage()
{
	return MassiveOutageDelegate;
}

TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> AccelByteWebSocket::Create(
	const FString& Url,
	const FString& Protocol,
	const Credentials& Credentials,
	const TMap<FString, FString>& UpgradeHeaders,
	const TSharedRef<IWebSocketFactory> WebSocketFactory,
	IWebsocketConfigurableReconnectStrategy& InParentReconnectionStrategyRef,
	float PingDelay
)
{
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));
	TSharedRef<AccelByteWebSocket, ESPMode::ThreadSafe> Ws = MakeShared<AccelByteWebSocket, ESPMode::ThreadSafe>(Credentials, InParentReconnectionStrategyRef, PingDelay);
		
	Ws->Url = Url;
	Ws->Protocol = Protocol;
	Ws->UpgradeHeaders = UpgradeHeaders;
	Ws->WebSocketFactory = WebSocketFactory;

	Ws->SetupWebSocket();

	return Ws;	
}

TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> AccelByteWebSocket::Create(
	const FString& Url,
	const FString& Protocol,
	const ServerCredentials& Credentials,
	const TMap<FString, FString>& UpgradeHeaders,
	const TSharedRef<IWebSocketFactory> WebSocketFactory,
	IWebsocketConfigurableReconnectStrategy& InParentReconnectionStrategyRef,
	float PingDelay /*= 30.f*/
)
{
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));
	TSharedRef<AccelByteWebSocket, ESPMode::ThreadSafe> Ws = MakeShared<AccelByteWebSocket, ESPMode::ThreadSafe>(Credentials, InParentReconnectionStrategyRef, PingDelay);

	Ws->Url = Url;
	Ws->Protocol = Protocol;
	Ws->UpgradeHeaders = UpgradeHeaders;
	Ws->WebSocketFactory = WebSocketFactory;

	Ws->SetupWebSocket();

	return Ws;
}

void AccelByteWebSocket::Connect()
{
	FReport::Log(FString(__FUNCTION__));
	
	if(!WebSocket.IsValid())
	{
		SetupWebSocket();
	}

	if(IsConnected())
	{
		return;
	}

	if(bWasWsConnectionError)
	{
		SetupWebSocket();
	}

	bConnectedBroadcasted = false;

	TeardownTicker();

	TickerDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(TickerDelegate, TickPeriod);

	FString HeaderString;

	for (const auto& UpgradeHeader : UpgradeHeaders)
	{
		HeaderString.Append(FString::Printf(TEXT("key %s, value %s\n"), *UpgradeHeader.Key, *UpgradeHeader.Value));
	}
	
	UE_LOG(LogAccelByteWebsocket, VeryVerbose, TEXT("Connecting websocket with header:\n%s"), *HeaderString);
	WebSocket->Connect();
	WsEvents |= EWebSocketEvent::Connect;
}

void AccelByteWebSocket::Disconnect(bool ForceCleanup)
{
	FReport::Log(FString(__FUNCTION__));
	
	if(!ForceCleanup && (bConnectTriggered || !OnMessageQueue.IsEmpty() || !OnConnectionClosedQueue.IsEmpty() || !OnConnectionErrorQueue.IsEmpty() || !OnReconnectingAttemptQueue.IsEmpty() || !OnMassiveOutageQueue.IsEmpty()))
	{
		bDisconnectOnNextTick = true;
	}
	else
	{
		bConnectedBroadcasted = false;

		if (WebSocket.IsValid())
		{
			WebSocket->Close();
		}
	}	
	LatestWebsocketDisonnectionCode = 0;
	MassiveOutageReminderCounter = 0;
}

bool AccelByteWebSocket::IsConnected() const
{
	return WebSocket.IsValid() && WebSocket->IsConnected() && bConnectedBroadcasted;
}

void AccelByteWebSocket::SendPing() const
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(FString());
	}
}

void AccelByteWebSocket::Send(const FString& Message) const
{
	ACCELBYTE_SERVICE_LOGGING_WEBSOCKET_REQUEST(Message);
	WebSocket->Send(Message);
}

void AccelByteWebSocket::OnConnectionConnected()
{
	FReport::Log(FString(__FUNCTION__));
	
	WsEvents |= EWebSocketEvent::Connected;
	bConnectTriggered = true;
	LatestWebsocketDisonnectionCode = 0;
}

void AccelByteWebSocket::OnConnectionError(const FString& Error)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::Log(FString::Printf(TEXT("AccelByteWebSocket::OnConnectionError = %s"), *Error));

	WsEvents |= EWebSocketEvent::ConnectionError;
	bWasWsConnectionError = true;
	OnConnectionErrorQueue.Enqueue(Error);
}

void AccelByteWebSocket::OnClosed(int32 StatusCode, const FString& Reason, bool WasClean)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::Log(FString::Printf(TEXT("AccelByteWebSocket::OnClosed = %d"), StatusCode));

	// Broadcast message DisconnectNotif
	OnMessageReceived(Reason);
	
	// trigger closed event, on prepare to reconnect on next StateTick
	if(StatusCode <= 4000)
	{
		// Add event websocket closed so state tick can reconnect lobby.
		WsEvents |= EWebSocketEvent::Closed;
	}

	LatestWebsocketDisonnectionCode = StatusCode;

	OnConnectionClosedQueue.Enqueue(FConnectionClosedParams({StatusCode, Reason, WasClean}));
}

void AccelByteWebSocket::OnMessageReceived(const FString& Message)
{
	ACCELBYTE_SERVICE_LOGGING_WEBSOCKET_RESPONSE(Message);
	FReport::Log(FString(__FUNCTION__));	
	OnMessageQueue.Enqueue(Message);
}

void AccelByteWebSocket::Reconnect()
{
	Connect();
}
	
bool AccelByteWebSocket::Tick(float DeltaTime)
{
	StateTick(DeltaTime);
	MessageTick(DeltaTime);

	if(bDisconnectOnNextTick)
	{
		bDisconnectOnNextTick = false;
		Disconnect();
	}
	
	return true;
}

bool AccelByteWebSocket::StateTick(float DeltaTime)
{
	auto& CurrentStrategy = GetCurrentReconnectionStrategy();

	switch (WsState)
	{
	case EWebSocketState::Closed:
		if (WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Connected) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Connected;
		}
		else if ((WsEvents & EWebSocketEvent::Connect) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Connecting;
		}
		ReconnectingAttemptCount = 0;
		MassiveOutageReminderCounter = 0;
		LatestWebsocketDisonnectionCode = 0;
		break;
	case EWebSocketState::Connecting:
		if ((WsEvents & EWebSocketEvent::ConnectionError) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closed;
		}
		else if (WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Connected) != EWebSocketEvent::None)
		{
			TimeSinceLastPing = FPlatformTime::Seconds();
			WsState = EWebSocketState::Connected;
		}
		else if ((WsEvents & EWebSocketEvent::Close) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closing;
		}
		else if (!WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Closed) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closed;
		}
		break;
	case EWebSocketState::Connected:
		if ((WsEvents & EWebSocketEvent::Close) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closing;
		}
		else if (!WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Closed) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::WaitingReconnect;
		}
		else if ((FPlatformTime::Seconds() - TimeSinceLastPing) >= PingDelay)
		{
			TimeSinceLastPing = FPlatformTime::Seconds();
			SendPing();
		}
		break;
	case EWebSocketState::WaitingReconnect:
		TimeSinceLastReconnect = FPlatformTime::Seconds();
		TimeSinceConnectionLost = FPlatformTime::Seconds();
		BackoffDelay = CurrentStrategy.InitialBackoffDelay.GetTotalSeconds();
		RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-BackoffDelay, BackoffDelay) / 4);
		Connect();
		WsState = EWebSocketState::Reconnecting;
		break;
	case EWebSocketState::Reconnecting:
		if (WebSocket->IsConnected() || (WsEvents & EWebSocketEvent::Connected) != EWebSocketEvent::None)
		{
			TimeSinceLastPing = FPlatformTime::Seconds();
			WsState = EWebSocketState::Connected;
		}
		else if ((WsEvents & EWebSocketEvent::Close) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closing;
		}
		// If the retry to reconnect has been exhausted & reach the limit
		else if ((FPlatformTime::Seconds() - TimeSinceConnectionLost) >= CurrentStrategy.TotalTimeout.GetTotalSeconds())
		{
			TimeSinceLastReconnect = FPlatformTime::Seconds();
			BackoffDelay = CurrentStrategy.InitialBackoffDelay.GetTotalSeconds();

			const int32 StatusCode = static_cast<int32>(EWebsocketErrorTypes::DisconnectFromExternalReconnect); //4401
			const FString Reason(TEXT("Reconnection total timeout limit reached"));
			const bool WasClean = false;
			OnConnectionClosedQueue.Enqueue(FConnectionClosedParams({StatusCode, Reason, WasClean}));

			WsState = EWebSocketState::Closed;
		}
		// If it's time to retry to reconnect again
		else if ((FPlatformTime::Seconds() - TimeSinceLastReconnect) >= RandomizedBackoffDelay)
		{
			switch (CurrentStrategy.StrategyType)
			{
			case EReconnectionStrategyType::AGGRESSIVE:
				BackoffDelay = CurrentStrategy.AggressiveRetryInterval.GetTotalSeconds();
				RandomizedBackoffDelay = BackoffDelay;
				break;
			case EReconnectionStrategyType::LIMITLESS:
				BackoffDelay *= CurrentStrategy.GetBaseFactorBackoff();
				BackoffDelay = FMath::Clamp(BackoffDelay, CurrentStrategy.CAP_MINIMUM_RETRY_INTERVAL_SECONDS, CurrentStrategy.CAP_LIMITLESS_RETRY_INTERVAL_SECONDS);
				RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-BackoffDelay, BackoffDelay) / 4);
				break;
			case EReconnectionStrategyType::BALANCED:
			default:
				if (BackoffDelay < CurrentStrategy.BalancedMaxRetryInterval.GetTotalSeconds())
				{
					BackoffDelay *= CurrentStrategy.GetBaseFactorBackoff();

					//Clamp the value
					BackoffDelay = FMath::Clamp(BackoffDelay, CurrentStrategy.CAP_MINIMUM_RETRY_INTERVAL_SECONDS, (int32)CurrentStrategy.BalancedMaxRetryInterval.GetTotalSeconds());
				}
				RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-BackoffDelay, BackoffDelay) / 4);
				break;
			}

			if (bWasWsConnectionError)
			{
				// websocket state is error can't be reconnect, need to create a new instance
				UE_LOG(LogAccelByteWebsocket, Log, TEXT("Connecting from Reconnecting state, setting up websocket"));
				SetupWebSocket();
			}
			UE_LOG(LogAccelByteWebsocket, Log, TEXT("Connecting from Reconnecting state"));
			Connect();
			TimeSinceLastReconnect = FPlatformTime::Seconds();

			ReconnectingAttemptCount += 1;
			OnReconnectingAttemptQueue.Enqueue(FReconnectAttemptInfo({ReconnectingAttemptCount, FTimespan::FromSeconds(RandomizedBackoffDelay)}));
		}

		// Extra condition: massive outage reminder that recurring each X minutes. Default : 5 minute, but configurable by setting the reconnection strategy.
		if ((FPlatformTime::Seconds() - TimeSinceConnectionLost) >= CurrentStrategy.GetMassiveOutageDuration().GetTotalSeconds())
		{
			float TotalOutageDuration = FPlatformTime::Seconds() - TimeSinceConnectionLost;
			int32 RoundedTotalOutageSeconds = FMath::RoundToInt(TotalOutageDuration);
			int32 FixedMassiveOutageInterval = FMath::RoundToInt(CurrentStrategy.GetMassiveOutageDuration().GetTotalSeconds());
			if (uint32(RoundedTotalOutageSeconds / FixedMassiveOutageInterval) > MassiveOutageReminderCounter)
			{
				MassiveOutageReminderCounter += 1; // The Enqueue of reminder will occur again if we exceed the next windows of massive outage.
				OnMassiveOutageQueue.Enqueue(FMassiveOutageInfo{ FTimespan::FromSeconds(TotalOutageDuration) });
			}
		}
		break;
	case EWebSocketState::Closing:
		if ((WsEvents & EWebSocketEvent::Closed) != EWebSocketEvent::None)
		{
			WsState = EWebSocketState::Closed;
		}
		break;
	}

	WsEvents = EWebSocketEvent::None;

	return true;
}

bool AccelByteWebSocket::MessageTick(float DeltaTime)
{
	if(bConnectTriggered)
	{
		bConnectTriggered = false;
		bConnectedBroadcasted = true;
		ConnectDelegate.Broadcast();
	}

	while(!OnConnectionErrorQueue.IsEmpty())
	{	
		FString Msg;
		OnConnectionErrorQueue.Dequeue(Msg);

		ConnectionErrorDelegate.Broadcast(Msg);
	}

	while(!OnMessageQueue.IsEmpty())
	{
		FString Msg;
		OnMessageQueue.Dequeue(Msg);

		MessageReceiveDelegate.Broadcast(Msg);
	}

	while(!OnConnectionClosedQueue.IsEmpty())
	{
		FConnectionClosedParams Params;
		OnConnectionClosedQueue.Dequeue(Params);

		ConnectionCloseDelegate.Broadcast(Params.Code, Params.Reason, Params.WasClean);

		if (Params.Code == static_cast<int32>(EWebsocketErrorTypes::NormalClosure))
		{
			TeardownWebsocket();
			TeardownTicker();
		}
	}

	while (!OnReconnectingAttemptQueue.IsEmpty())
	{
		FReconnectAttemptInfo Params;
		OnReconnectingAttemptQueue.Dequeue(Params);

		ReconnectAttemptDelegate.Broadcast(Params);
	}

	while (!OnMassiveOutageQueue.IsEmpty())
	{
		FMassiveOutageInfo Params;
		OnMassiveOutageQueue.Dequeue(Params);

		MassiveOutageDelegate.Broadcast(Params);
	}

	return true;
}

void AccelByteWebSocket::TeardownTicker()
{
	if (TickerDelegateHandle.IsValid())
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(TickerDelegateHandle);
		TickerDelegateHandle.Reset();
	}
}

void AccelByteWebSocket::TeardownWebsocket()
{
	if(WebSocket.IsValid())
	{
		WebSocket->OnMessage().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->OnClosed().Clear();
		WebSocket.Reset();
	}
}

FReconnectionStrategy& AccelByteWebSocket::GetCurrentReconnectionStrategy()
{
	// When there is no closure code from a disconnection, return default strategy
	if (LatestWebsocketDisonnectionCode == (int32)EWebsocketClosureCodeForSpecificRetry::None)
	{
		return ParentReconnectionStrategyRef.GetDefaultReconnectionStrategy();
	}

	// When there's a specific closure code
	if (ParentReconnectionStrategyRef.GetReconnectionStrategyDictionary().Contains(LatestWebsocketDisonnectionCode))
	{
		auto& SpecificStrategy = ParentReconnectionStrategyRef.GetReconnectionStrategyDictionary()[LatestWebsocketDisonnectionCode];
		switch (SpecificStrategy.StrategyType)
		{
		case EReconnectionStrategyType::AGGRESSIVE:
			return SpecificStrategy;
		case EReconnectionStrategyType::BALANCED:
			return SpecificStrategy;
		case EReconnectionStrategyType::LIMITLESS:
			return SpecificStrategy;
		default:
			return ParentReconnectionStrategyRef.GetDefaultReconnectionStrategy();
		}
	}
	else
	{
		return ParentReconnectionStrategyRef.GetDefaultReconnectionStrategy();
	}
}

}