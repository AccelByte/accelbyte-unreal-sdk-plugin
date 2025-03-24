// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteServerApiClient.h"
#include "Core/AccelByteSettings.h"
#include "Core/IAccelByteDataStorage.h"

using namespace AccelByte;

class ACCELBYTEUE4SDK_API FAccelByteInstance : public TSharedFromThis<FAccelByteInstance, ESPMode::ThreadSafe>
{
public:
	~FAccelByteInstance();

	FAccelByteInstance(Settings& InSettings, ServerSettings& InServerSettings, TSharedPtr<IAccelByteDataStorage> LocalDataStorage, FAccelByteTimeManagerPtr TimeManager, int32 RegistryIndex = 0);

	void OnSettingsEnvironmentChanges(ESettingsEnvironment SettingsEnvironment);
	void SetEnvironmentChangeDelegate();
	
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
	
	/**
	 * @brief Obtain an HMAC Buffer encoded MAC Address, using PublisherNamespace as key.
	 *
	 * @param bEncoded Flag to encode the return value. (default: true)
	 * @return Either plain MAC Address or encoded one.
	 */
	FString GetMacAddress(bool bEncoded) const;
	
	FString GetDeviceId(bool bIsEncoded = true) const;

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
	FString EncodedDeviceId;
	FDelegateHandle OnEnvironmentChangeHandle;

	FString GenerateDeviceId(int32 InIndex);
};

using FAccelByteInstancePtr = TSharedPtr<FAccelByteInstance, ESPMode::ThreadSafe>;
using FAccelByteInstanceRef = TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe>;
using FAccelByteInstanceWPtr = TWeakPtr<FAccelByteInstance, ESPMode::ThreadSafe>;