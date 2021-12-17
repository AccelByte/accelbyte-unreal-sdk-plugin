// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

namespace AccelByte
{
	
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
