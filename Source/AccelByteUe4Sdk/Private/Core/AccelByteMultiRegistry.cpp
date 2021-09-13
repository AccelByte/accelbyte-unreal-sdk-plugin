// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMultiRegistry.h"

using namespace AccelByte;
using namespace AccelByte::Api;

FApiClient::FApiClient()
{
	Http.Startup();
	Credentials.Startup();
	GameTelemetry.Startup();
}

FApiClient::~FApiClient()
{
	GameTelemetry.Shutdown();
	Credentials.Shutdown();
	Http.Shutdown();
}

TSharedPtr<FApiClient> AccelByte::FMultiRegistry::GetApiClient(const FString Key)
{
	if (!ApiClientInstances.Contains(Key))
	{
		const TSharedPtr<FApiClient> NewClient = MakeShared<FApiClient>();

		NewClient->Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);

		ApiClientInstances.Add(Key, NewClient);
	}

	return ApiClientInstances[Key];
}

TMap<FString, TSharedPtr<FApiClient>> FMultiRegistry::ApiClientInstances;
