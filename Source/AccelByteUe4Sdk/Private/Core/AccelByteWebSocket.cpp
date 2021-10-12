// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteWebSocket.h"
#include "WebSocketsModule.h"
#include "Core/IWebSocketFactory.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{	
AccelByteWebSocket::AccelByteWebSocket(
	const Credentials& Credentials,
	float PingDelay,
	float InitialBackoffDelay,
	float MaxBackoffDelay,
	float TotalTimeout)
	:
	TimeSinceLastPing(0.f),
	TimeSinceLastReconnect(0.f),
	InitialBackoffDelay(InitialBackoffDelay),
	PingDelay(PingDelay),
	TotalTimeout(TotalTimeout),
	MaxBackoffDelay(MaxBackoffDelay),
	Creds(Credentials),
	WsState(EWebSocketState::Closed),
	WsEvents(EWebSocketEvent::None)
{
	TickerDelegate = FTickerDelegate::CreateRaw(this, &AccelByteWebSocket::Tick);
	TickerDelegateHandle.Reset();
}

AccelByteWebSocket::~AccelByteWebSocket()
{
	if(UObjectInitialized())
	{
		Disconnect();
	}
}

void AccelByteWebSocket::SetupWebSocket()
{
	bWasWsConnectionError = false;

	if(WebSocket.IsValid())
	{
		WebSocket->OnMessage().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->OnClosed().Clear();
		WebSocket->Close();
		WebSocket.Reset();
	}

	TMap<FString, FString> Headers = UpgradeHeaders;
	Headers.Add("Authorization", "Bearer " + Creds.GetAccessToken());
	
	WebSocket = WebSocketFactory->CreateWebSocket(Url, Protocol, Headers);

	WebSocket->OnMessage().AddRaw(this, &AccelByteWebSocket::OnMessageReceived);
	WebSocket->OnConnected().AddRaw(this, &AccelByteWebSocket::OnConnectionConnected);
	WebSocket->OnConnectionError().AddRaw(this, &AccelByteWebSocket::OnConnectionError);
	WebSocket->OnClosed().AddRaw(this, &AccelByteWebSocket::OnClosed);
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

AccelByteWebSocket::FConnectionCloseDelegate& AccelByteWebSocket::OnConnectionClosed()
{
	return ConnectionCloseDelegate;
}

TSharedPtr<AccelByteWebSocket, ESPMode::ThreadSafe> AccelByteWebSocket::Create(
	const FString& Url,
	const FString& Protocol,
	const Credentials& Credentials,
	const TMap<FString, FString>& UpgradeHeaders,
	const TSharedRef<IWebSocketFactory> WebSocketFactory,
	float PingDelay,
	float InitialBackoffDelay,
	float MaxBackoffDelay,
	float TotalTimeout
)
{
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));
	TSharedRef<AccelByteWebSocket, ESPMode::ThreadSafe> Ws = MakeShared<AccelByteWebSocket, ESPMode::ThreadSafe>(Credentials);
		
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

	if(TickerDelegateHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(TickerDelegateHandle);
		TickerDelegateHandle.Reset();
	}

	TickerDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickerDelegate, TickPeriod);

	WebSocket->Connect();
	WsEvents |= EWebSocketEvent::Connect;
}
	

void AccelByteWebSocket::Disconnect()
{
	FReport::Log(FString(__FUNCTION__));
	
	if (TickerDelegateHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(TickerDelegateHandle);
		TickerDelegateHandle.Reset();
	}

	if (WebSocket.IsValid())
	{
		WebSocket->OnMessage().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->OnClosed().Clear();
		WebSocket->Close();

		WebSocket.Reset();
	}
}

bool AccelByteWebSocket::IsConnected() const
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
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
	WebSocket->Send(Message);
}

	
void AccelByteWebSocket::OnConnectionConnected()
{
	WsEvents |= EWebSocketEvent::Connected;
	ConnectDelegate.Broadcast();
}

void AccelByteWebSocket::OnConnectionError(const FString& Error)
{
	WsEvents |= EWebSocketEvent::ConnectionError;
	bWasWsConnectionError = true;

	ConnectionErrorDelegate.Broadcast(Error);
}

void AccelByteWebSocket::OnClosed(int32 StatusCode, const FString& Reason, bool WasClean)
{
	OnMessageReceived(Reason);
	if(StatusCode < 4000)
	{
		WsEvents |= EWebSocketEvent::Closed;
	}
	
	ConnectionCloseDelegate.Broadcast(StatusCode, Reason, WasClean);
}

void AccelByte::AccelByteWebSocket::Reconnect()
{
	WsEvents |= EWebSocketEvent::Closed;
}

void AccelByteWebSocket::OnMessageReceived(const FString& Message)
{
	MessageReceiveDelegate.Broadcast(Message);
}


bool AccelByteWebSocket::Tick(float DeltaTime)
{
	StateTick(DeltaTime);

	return true;
}

bool AccelByteWebSocket::StateTick(float DeltaTime)
{
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
			TimeSinceLastReconnect = FPlatformTime::Seconds();
			TimeSinceConnectionLost = FPlatformTime::Seconds();
			BackoffDelay = InitialBackoffDelay;
			RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-InitialBackoffDelay, InitialBackoffDelay) / 4);
			Connect();
			WsState = EWebSocketState::Reconnecting;
		}
		else if ((FPlatformTime::Seconds() - TimeSinceLastPing) >= PingDelay)
		{
			TimeSinceLastPing = FPlatformTime::Seconds();
			SendPing();
		}
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
		else if ((FPlatformTime::Seconds() - TimeSinceConnectionLost) >= TotalTimeout)
		{
			TimeSinceLastReconnect = FPlatformTime::Seconds();
			BackoffDelay = InitialBackoffDelay;

			WsState = EWebSocketState::Closed;
		}
		else if ((FPlatformTime::Seconds() - TimeSinceLastReconnect) >= RandomizedBackoffDelay)
		{
			if (BackoffDelay < MaxBackoffDelay)
			{
				BackoffDelay *= 2;
			}
			RandomizedBackoffDelay = BackoffDelay + (FMath::RandRange(-BackoffDelay, BackoffDelay) / 4);
			if (bWasWsConnectionError)
			{
				// websocket state is error can't be reconnect, need to create a new instance
				SetupWebSocket();
			}
			Connect();
			TimeSinceLastReconnect = FPlatformTime::Seconds();
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
}