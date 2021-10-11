// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

namespace AccelByte
{
FApiClient::FApiClient()
	: CredentialsRef(MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>())
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
{
	HttpRef->Startup();
	CredentialsRef->Startup();
	GameTelemetry.Startup();
}

FApiClient::FApiClient(AccelByte::Credentials& Credentials, AccelByte::FHttpRetryScheduler& Http)
	: CredentialsRef(MakeShareable<AccelByte::Credentials>(&Credentials))
	, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http))
{
}

FApiClient::~FApiClient()
{
	GameTelemetry.Shutdown();
	CredentialsRef->Shutdown();
	HttpRef->Shutdown();
}

TSharedPtr<FApiClient> AccelByte::FMultiRegistry::GetApiClient(const FString Key)
{
	if (!ApiClientInstances.Contains(Key))
	{
		const TSharedPtr<FApiClient> NewClient = MakeShared<FApiClient>();

		NewClient->CredentialsRef->SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);

		ApiClientInstances.Add(Key, NewClient);
	}

	return ApiClientInstances[Key];
}

TMap<FString, TSharedPtr<FApiClient>> FMultiRegistry::ApiClientInstances;
}