// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteServerApiClient.h"
#include "Core/AccelByteSettings.h"
#include "Core/IAccelByteDataStorage.h"

using namespace AccelByte;

#define FAccelByteInstancePtr TSharedPtr<FAccelByteInstance, ESPMode::ThreadSafe>
#define FAccelByteInstanceRef TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe>
#define FAccelByteInstanceWPtr TWeakPtr<FAccelByteInstance, ESPMode::ThreadSafe>

class ACCELBYTEUE4SDK_API FAccelByteInstance
{
public:
	~FAccelByteInstance();
	
	FAccelByteInstance(Settings& InSettings, ServerSettings& InServerSettings, TSharedPtr<IAccelByteDataStorage> LocalDataStorage, FAccelByteTimeManagerPtr TimeManager, int32 RegistryIndex = 0);
	FApiClientPtr GetApiClient(FString const& Key = TEXT("default"), bool bCreateIfNotFound = true);
	SettingsPtr GetSettings();

	bool RegisterApiClient(FString const& Key, FApiClientPtr ApiClient);
	bool RemoveApiClient(const FString& Key = TEXT("default"));

	void ClearApiClient();
	void ClearServerApiClient();
	
	FServerApiClientPtr GetServerApiClient(FString const& Key = TEXT("default"));
	ServerSettingsPtr GetServerSettings();
	bool RemoveServerApiClient(const FString& Key = TEXT("default"));

	FAccelByteTimeManagerWPtr GetTimeManager();

	FString GetFlightId();
	FString GetDeviceId();

private:
	SettingsRef Settings;
	TMap<FString, FApiClientPtr> ApiClients;

	ServerSettingsRef ServerSettings;
	TMap<FString, FServerApiClientPtr> ServerApiClients;
	TSharedPtr<IAccelByteDataStorage> LocalDataStorage;
	FAccelByteTimeManagerPtr TimeManager;
	int32 Index {0};

	FString FlightId;
	FString DeviceId;

	FString GenerateDeviceId(int32 InIndex, bool bIsDeviceIdRequireEncode);
};