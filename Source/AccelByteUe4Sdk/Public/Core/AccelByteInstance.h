// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteServerApiClient.h"
#include "Core/AccelByteSettings.h"
#include "Core/IAccelByteDataStorage.h"

class ACCELBYTEUE4SDK_API FAccelByteInstance : public TSharedFromThis<FAccelByteInstance, ESPMode::ThreadSafe>
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInstanceDestroyed, uint32 /* InstanceIndex */);

	FAccelByteInstance(AccelByte::Settings& InSettings, AccelByte::ServerSettings& InServerSettings, TSharedPtr<AccelByte::IAccelByteDataStorage> LocalDataStorage, AccelByte::FAccelByteTimeManagerPtr TimeManager, int32 RegistryIndex = 0);

	FAccelByteInstance(FAccelByteInstance const&) = delete;
	FAccelByteInstance& operator=(FAccelByteInstance const&) = delete;

	FAccelByteInstance(FAccelByteInstance&&) = delete;
	FAccelByteInstance& operator=(FAccelByteInstance&&) = delete;

	~FAccelByteInstance();

	AccelByte::FApiClientPtr GetApiClient(FString const& Key = TEXT("default"), bool bCreateIfNotFound = true);

	bool RegisterApiClient(FString const& Key, AccelByte::FApiClientPtr ApiClient);

	bool RemoveApiClient(const FString& Key = TEXT("default"));

	void ClearApiClient();

	AccelByte::FServerApiClientPtr GetServerApiClient(FString const& Key = TEXT("default"));

	bool RemoveServerApiClient(const FString& Key = TEXT("default"));

	void ClearServerApiClient();

	void AddOnDestroyedDelegate(TFunction<void(uint32)> Fn);

	void OnSettingsEnvironmentChanges(ESettingsEnvironment SettingsEnvironment);

	void SetEnvironmentChangeDelegate();

	AccelByte::SettingsPtr GetSettings() const;

	AccelByte::ServerSettingsPtr GetServerSettings() const;

	AccelByte::FAccelByteTimeManagerWPtr GetTimeManager() const;

	FString GetFlightId() const;
	
	/**
	 * @brief Obtain an HMAC Buffer encoded MAC Address, using PublisherNamespace as key.
	 *
	 * @param bEncoded Flag to encode the return value. (default: true)
	 * @return Either plain MAC Address or encoded one.
	 */
	FString GetMacAddress(bool bEncoded) const;
	
	FString GetDeviceId(bool bIsEncoded = true) const;

private:
	FString GenerateDeviceId(int32 InIndex);

private:
	mutable FRWLock SettingsMtx;
	AccelByte::SettingsRef Settings;
	AccelByte::ServerSettingsRef ServerSettings;
	
	AccelByte::FAccelByteTimeManagerPtr TimeManager;

	FCriticalSection ApiClientMtx;
	TMap<FString, AccelByte::FApiClientPtr> ApiClients;

	FCriticalSection ServerApiClientMtx;
	TMap<FString, AccelByte::FServerApiClientPtr> ServerApiClients;

	TSharedPtr<AccelByte::IAccelByteDataStorage> LocalDataStorage;
	int32 Index {0};
	FString FlightId;
	FString DeviceId;
	FString EncodedDeviceId;

	FCriticalSection EnvChangeHandleMtx;
	FDelegateHandle OnEnvironmentChangeHandle;

	FCriticalSection OnDestroyedDelegateMtx;
	FOnInstanceDestroyed OnDestroyedDelegate;
};

using FAccelByteInstancePtr = TSharedPtr<FAccelByteInstance, ESPMode::ThreadSafe>;
using FAccelByteInstanceRef = TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe>;
using FAccelByteInstanceWPtr = TWeakPtr<FAccelByteInstance, ESPMode::ThreadSafe>;
