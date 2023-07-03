// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiClient.h"

namespace AccelByte
{
	
FApiClient::FApiClient()
	: bUseSharedCredentials(false)
	, CredentialsRef(MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>())
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
{
	HttpRef->Startup();
	CredentialsRef->Startup();
	GameTelemetry.Startup();
	PresenceBroadcastEvent.Startup();
}

FApiClient::FApiClient(AccelByte::Credentials& Credentials, AccelByte::FHttpRetryScheduler& Http)
	: bUseSharedCredentials(true)
	, CredentialsRef(MakeShareable<AccelByte::Credentials>(&Credentials,
		[](AccelByte::Credentials*) {}))
	, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http,
		[](AccelByte::FHttpRetryScheduler*) {}))
{
	GameTelemetry.Startup();
	PresenceBroadcastEvent.Startup();
}

FApiClient::~FApiClient()
{
	GameTelemetry.Shutdown();
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
