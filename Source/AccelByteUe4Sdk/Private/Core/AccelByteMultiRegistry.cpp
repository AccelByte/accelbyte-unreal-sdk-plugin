// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

using namespace AccelByte;
using namespace AccelByte::Api;

TSharedPtr<FApiClient> AccelByte::FMultiRegistry::GetApiClient(FString key)
{
	if (!ApiClientInstances.Contains(key))
	{
		TSharedPtr<FApiClient> NewClient = MakeShared<FApiClient>();

		NewClient->Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);

		ApiClientInstances.Add(key, NewClient);
	}

	return ApiClientInstances[key];
}

TMap<FString, TSharedPtr<FApiClient>> FMultiRegistry::ApiClientInstances;
