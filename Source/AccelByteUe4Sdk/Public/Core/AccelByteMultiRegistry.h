// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteApiClient.h"
#include "Api/AccelByteUserProfileApi.h"
#include "AccelByteServerApiClient.h"

using namespace AccelByte;

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FMultiRegistry
{
public:
	static FApiClientPtr GetApiClient(FString const Key = TEXT("default"));
	static FServerApiClientPtr GetServerApiClient(FString const Key = TEXT("default"));
	static bool RegisterApiClient(FString const Key, FApiClientPtr ApiClient);
	
private:
	static TMap<FString, FApiClientPtr> ApiClientInstances;
	static TMap<FString, FServerApiClientPtr> ServerApiClientInstances;

	FMultiRegistry() = delete;
	FMultiRegistry(FMultiRegistry const& Other) = delete;
	FMultiRegistry& operator=(FMultiRegistry const& Other) = delete;
	FMultiRegistry(FMultiRegistry&& Other) = delete;
	FMultiRegistry& operator=(FMultiRegistry&& Other) = delete;
	~FMultiRegistry() = delete;
};

}
