// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiClient.h"

namespace AccelByte
{
	
FApiClient::FApiClient()
	: bUseSharedCredentials(false)
	, MessagingSystem(MakeShared<FAccelByteMessagingSystem>())
	, CredentialsRef(MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>(*MessagingSystem.Get()))
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
{
	HttpRef->Startup();
	CredentialsRef->Startup();
	GameTelemetry.Startup();
	PredefinedEvent.Startup();
	GameStandardEvent.Startup();
	PresenceBroadcastEvent.Startup();
}

FApiClient::FApiClient(AccelByte::Credentials& Credentials, AccelByte::FHttpRetryScheduler& Http, TSharedPtr<AccelByte::FAccelByteMessagingSystem> MessagingRef)
	: bUseSharedCredentials(true)
	, MessagingSystem(MessagingRef == nullptr ? MakeShared<FAccelByteMessagingSystem>() : MessagingRef)
	, CredentialsRef(Credentials.AsShared())
	, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http,
		[](AccelByte::FHttpRetryScheduler*) {}))
{
	GameTelemetry.Startup();
	PredefinedEvent.Startup();
	GameStandardEvent.Startup();
	PresenceBroadcastEvent.Startup();
}

FApiClient::~FApiClient()
{
	GameTelemetry.Shutdown();
	PredefinedEvent.Shutdown();
	GameStandardEvent.Shutdown();
	PresenceBroadcastEvent.Shutdown();
	
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

}
