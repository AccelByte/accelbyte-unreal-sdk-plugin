// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

namespace AccelByte
{
FApiBase::FApiBase(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef)
	: CredentialsRef{ CredentialsRef }
	, SettingsRef{ SettingsRef }
	, HttpRef{ HttpRef }
	, HttpClient(CredentialsRef, SettingsRef, HttpRef)
{

}
	
FApiClient::FApiClient()
	: bUseSharedCredentials(false)
	, CredentialsRef(MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>())
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
{
	HttpRef->Startup();
	CredentialsRef->Startup();
	GameTelemetry.Startup();
}

FApiClient::FApiClient(AccelByte::Credentials& Credentials, AccelByte::FHttpRetryScheduler& Http)
	: bUseSharedCredentials(true)
	, CredentialsRef(MakeShareable<AccelByte::Credentials>(&Credentials, [](AccelByte::Credentials*) {}))
	, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http, [](AccelByte::FHttpRetryScheduler*) {}))
{
	GameTelemetry.Startup();
}

FApiClient::~FApiClient()
{
	GameTelemetry.Shutdown();
	
	if (!bUseSharedCredentials)
	{
		CredentialsRef->Shutdown();
		HttpRef->Shutdown();
	}
}

FApiClientPtr AccelByte::FMultiRegistry::GetApiClient(const FString Key)
{
	if (!ApiClientInstances.Contains(Key))
	{
		FApiClientPtr NewClient = nullptr;
		
		if (Key.Compare(TEXT("default")) == 0) 
		{
			NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>(FRegistry::Credentials, FRegistry::HttpRetryScheduler);
		}
		else 
		{
			NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>();
			NewClient->CredentialsRef->SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
		}

		ApiClientInstances.Add(Key, NewClient);
	}

	return ApiClientInstances[Key];
}

FServerApiClientPtr AccelByte::FMultiRegistry::GetServerApiClient(const FString Key)
{
	if (!ServerApiClientInstances.Contains(Key))
	{
		FServerApiClientPtr NewClient = nullptr;
		
		if (Key.Compare(TEXT("default")) == 0) 
		{
			NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>(FRegistry::ServerCredentials, FRegistry::HttpRetryScheduler);
		}
		else 
		{
			NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>();
			NewClient->ServerCredentialsRef->SetClientCredentials(FRegistry::ServerSettings.ClientId, FRegistry::ServerSettings.ClientSecret);
		}

		ServerApiClientInstances.Add(Key, NewClient);
	}
	
	return ServerApiClientInstances[Key];
}

bool FMultiRegistry::RegisterApiClient(FString const Key, FApiClientPtr ApiClient)
{
	bool bResult = false;

	if (!Key.IsEmpty())
	{
		ApiClientInstances.Add(Key, ApiClient);
		bResult = true;
	}

	return bResult;
}

TMap<FString, FApiClientPtr> FMultiRegistry::ApiClientInstances;
TMap<FString, FServerApiClientPtr> FMultiRegistry::ServerApiClientInstances;

}
