// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

#include "AccelByteUe4SdkModule.h"

namespace AccelByte
{
	
FAccelByteInstancePtr FMultiRegistry::AccelByteInstance;
	
FApiClientPtr AccelByte::FMultiRegistry::GetApiClient(const FString &Key, bool bCreateIfNotFound)
{
	if(!AccelByteInstance.IsValid())
	{
		AccelByteInstance = IAccelByteUe4SdkModuleInterface::Get().CreateAccelByteInstance();
	}

	return AccelByteInstance->GetApiClient(Key, bCreateIfNotFound);
}

FServerApiClientPtr AccelByte::FMultiRegistry::GetServerApiClient(const FString &Key)
{
	if(!AccelByteInstance.IsValid())
	{
		AccelByteInstance = IAccelByteUe4SdkModuleInterface::Get().CreateAccelByteInstance();
	}

	return AccelByteInstance->GetServerApiClient(Key);
}

bool FMultiRegistry::RegisterApiClient(FString const &Key, FApiClientPtr ApiClient)
{
	if(!AccelByteInstance.IsValid())
	{
		AccelByteInstance = IAccelByteUe4SdkModuleInterface::Get().CreateAccelByteInstance();
	}

	return AccelByteInstance->RegisterApiClient(Key, ApiClient);
}

bool FMultiRegistry::RemoveApiClient(const FString &Key)
{
	if(!AccelByteInstance.IsValid())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("FMultiRegistry::RemoveApiClient failed, AccelByteInstance is invalid!"));
		return false;
	}

	return AccelByteInstance->RemoveApiClient(Key);
}

bool FMultiRegistry::RemoveServerApiClient(const FString &Key)
{
	if(!AccelByteInstance.IsValid())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("FMultiRegistry::RemoveServerApiClient failed, AccelByteInstance is invalid!"));
		return false;
	}

	return AccelByteInstance->RemoveServerApiClient(Key);
}

void FMultiRegistry::Shutdown()
{
	if(!AccelByteInstance.IsValid())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("FMultiRegistry::Shutdown failed, AccelByteInstance is invalid!"));
		return;
	}

	AccelByteInstance->ClearApiClient();
	AccelByteInstance->ClearServerApiClient();
}

bool FMultiRegistry::HasValidAccelByteInstance()
{
	return AccelByteInstance.IsValid();
}

bool FMultiRegistry::SetAccelByteInstance(FAccelByteInstanceRef InInstance)
{
	if(HasValidAccelByteInstance())
	{
		return false;
	}
	
	AccelByteInstance = InInstance;
	return true;
}

FAccelByteInstanceWPtr FMultiRegistry::GetAccelByteInstance()
{
	return AccelByteInstance;
}
}
