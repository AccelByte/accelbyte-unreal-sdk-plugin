// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

namespace AccelByte
{

FCriticalSection MultiRegistryMutex;

FApiClientPtr AccelByte::FMultiRegistry::GetApiClient(const FString &Key, bool bCreateIfNotFound)
{
	FScopeLock Lock(&MultiRegistryMutex);
	
	if (!ApiClientInstances.Contains(Key))
	{
		if (!bCreateIfNotFound)
		{
			return nullptr;
		}

		FApiClientPtr NewClient = nullptr;
		
		if (Key.Compare(TEXT("default")) == 0) 
		{
			NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::HttpRetryScheduler, FRegistry::MessagingSystem);
		}
		else 
		{
			NewClient = MakeShared<FApiClient, ESPMode::ThreadSafe>();
		}

		ApiClientInstances.Add(Key, NewClient);
	}

	return ApiClientInstances[Key];
}

FServerApiClientPtr AccelByte::FMultiRegistry::GetServerApiClient(const FString &Key)
{
	FScopeLock Lock(&MultiRegistryMutex);
	if (!ServerApiClientInstances.Contains(Key))
	{
		FServerApiClientPtr NewClient = nullptr;
		
		if (Key.Compare(TEXT("default")) == 0) 
		{
			NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>(FRegistry::ServerCredentialsRef.Get(), FRegistry::HttpRetryScheduler);
		}
		else 
		{
			NewClient = MakeShared<FServerApiClient, ESPMode::ThreadSafe>();
		}

		ServerApiClientInstances.Add(Key, NewClient);
	}
	
	return ServerApiClientInstances[Key];
}

bool FMultiRegistry::RegisterApiClient(FString const &Key, FApiClientPtr ApiClient)
{
	FScopeLock Lock(&MultiRegistryMutex);
	bool bResult = false;

	if (!Key.IsEmpty())
	{
		ApiClientInstances.Add(Key, ApiClient);
		bResult = true;
	}

	return bResult;
}

bool FMultiRegistry::RemoveApiClient(const FString &Key)
{
	FScopeLock Lock(&MultiRegistryMutex);
	bool bResult = false;

    if (!Key.IsEmpty())
    {
        const int32 RemovedNum = ApiClientInstances.Remove(Key);
        bResult = RemovedNum > 0;
    }

    return bResult;
}

bool FMultiRegistry::RemoveServerApiClient(const FString &Key)
{
	FScopeLock Lock(&MultiRegistryMutex);
	bool bResult = false;
	
	if (!Key.IsEmpty())
	{
        const int32 RemovedNum = ServerApiClientInstances.Remove(Key);
        bResult = RemovedNum > 0;
	}

	return bResult;
}

void FMultiRegistry::Shutdown()
{
	FScopeLock Lock(&MultiRegistryMutex);

	TArray<FString> ApiClientKeys{};
	ApiClientInstances.GenerateKeyArray(ApiClientKeys);
	ApiClientKeys.Remove(TEXT("default"));
	for (const auto& Key : ApiClientKeys)
	{
		const auto& TempApiClient = GetApiClient(Key);
		RemoveApiClient(Key);
	}

	TArray<FString> ServerApiClientKeys{};
	ServerApiClientInstances.GenerateKeyArray(ServerApiClientKeys);
	ServerApiClientKeys.Remove(TEXT("default"));
	for (const auto& Key : ServerApiClientKeys)
	{
		const auto& TempApiClient = GetServerApiClient(Key);
		RemoveServerApiClient(Key);
	}
}

TMap<FString, FApiClientPtr> FMultiRegistry::ApiClientInstances;
TMap<FString, FServerApiClientPtr> FMultiRegistry::ServerApiClientInstances;

}
