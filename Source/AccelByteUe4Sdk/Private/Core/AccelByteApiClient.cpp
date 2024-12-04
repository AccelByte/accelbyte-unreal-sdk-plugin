// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiClient.h"

namespace AccelByte
{
	
FApiClient::FApiClient()
	: bUseSharedCredentials(false)
	, MessagingSystem(MakeShared<FAccelByteMessagingSystem, ESPMode::ThreadSafe>())
	, CredentialsRef(MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>(*MessagingSystem.Get()))
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
	, QosPtr(MakeShared<Api::Qos, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *MessagingSystem.Get()))
	, LobbyPtr(MakeShared<AccelByte::Api::Lobby, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef, *MessagingSystem.Get(), NetworkConditioner))
	, ChatPtr(MakeShared<AccelByte::Api::Chat, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef, *MessagingSystem.Get(), NetworkConditioner))
	, GameTelemetryPtr(MakeShared<AccelByte::Api::GameTelemetry, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef))
	, PredefinedEventPtr( MakeShared<AccelByte::Api::PredefinedEvent, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef))
	, GameStandardEventPtr(MakeShared<AccelByte::Api::GameStandardEvent, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef))
	, Qos(*QosPtr.Get())
	, Lobby(*LobbyPtr.Get())
	, Chat(*ChatPtr.Get())
	, GameTelemetry(*GameTelemetryPtr.Get())
	, PredefinedEvent(*PredefinedEventPtr.Get())
	, GameStandardEvent(*GameStandardEventPtr.Get())
{
	HttpRef->Startup();
	CredentialsRef->Startup();
	QosPtr->Startup();
	LobbyPtr->Startup();
	GameTelemetryPtr->Startup();
	PredefinedEventPtr->Startup();
	GameStandardEventPtr->Startup();
	PresenceBroadcastEvent.Startup();
}

FApiClient::FApiClient(AccelByte::Credentials& Credentials
	, AccelByte::FHttpRetryScheduler& Http
	, FAccelByteMessagingSystemPtr InMessagingPtr)
	: bUseSharedCredentials(true)
	, MessagingSystem(InMessagingPtr.IsValid() ? InMessagingPtr: MakeShared<FAccelByteMessagingSystem, ESPMode::ThreadSafe>())
	, CredentialsRef(Credentials.AsShared())
	, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http, [](AccelByte::FHttpRetryScheduler*) {}))
	, QosPtr(MakeShared<Api::Qos, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *MessagingSystem.Get()))
	, LobbyPtr(MakeShared<AccelByte::Api::Lobby, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef, *MessagingSystem.Get(), NetworkConditioner))
	, ChatPtr(MakeShared<AccelByte::Api::Chat, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef, *MessagingSystem.Get(), NetworkConditioner))
	, GameTelemetryPtr(MakeShared<AccelByte::Api::GameTelemetry, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef))
	, PredefinedEventPtr(MakeShared<AccelByte::Api::PredefinedEvent, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef))
	, GameStandardEventPtr(MakeShared<AccelByte::Api::GameStandardEvent, ESPMode::ThreadSafe>(*CredentialsRef, FRegistry::Settings, *HttpRef))
	, Qos(*QosPtr.Get())
	, Lobby(*LobbyPtr.Get())
	, Chat(*ChatPtr.Get())
	, GameTelemetry(*GameTelemetryPtr.Get())
	, PredefinedEvent(*PredefinedEventPtr.Get())
	, GameStandardEvent(*GameStandardEventPtr.Get())
{
	QosPtr->Startup();
	LobbyPtr->Startup();
	GameTelemetryPtr->Startup();
	PredefinedEventPtr->Startup();
	GameStandardEventPtr->Startup();
	PresenceBroadcastEvent.Startup();
}

FApiClient::~FApiClient()
{
	GameTelemetryPtr->Shutdown();
	PredefinedEventPtr->Shutdown();
	GameStandardEventPtr->Shutdown();
	PresenceBroadcastEvent.Shutdown();
	MessagingSystem.Reset();
	
	if (!bUseSharedCredentials)
	{
		const bool bShouldExecuteImmediately = IsInGameThread() || !FTaskGraphInterface::IsRunning();
		if (bShouldExecuteImmediately)
		{
			CredentialsRef->Shutdown();
			HttpRef->Shutdown();
		}
		else
		{
			FFunctionGraphTask::CreateAndDispatchWhenReady(
				[Credentials = CredentialsRef, HttpScheduler = HttpRef]()
				{
					Credentials->Shutdown();
					HttpScheduler->Shutdown();
				}
				, TStatId()
					, nullptr
					, ENamedThreads::GameThread);
		}
	}
}

Api::LobbyWPtr FApiClient::GetLobbyApi() const
{
	return LobbyPtr;
}

Api::ChatWPtr FApiClient::GetChatApi() const
{
	return ChatPtr;
}

TWeakPtr<Api::Qos, ESPMode::ThreadSafe> FApiClient::GetQosApi() const
{
	return QosPtr;
}

Api::GameTelemetryWPtr FApiClient::GetGameTelemetryApi() const
{
	return GameTelemetryPtr;
}

Api::PredefinedEventWPtr FApiClient::GetPredefinedEventApi() const
{
	return PredefinedEventPtr;
}

Api::GameStandardEventWPtr FApiClient::GetGameStandardEventApi() const
{
	return GameStandardEventPtr;
}

}
