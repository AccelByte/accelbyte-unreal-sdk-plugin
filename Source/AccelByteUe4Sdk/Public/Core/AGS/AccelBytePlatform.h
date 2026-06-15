// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "IPAddress.h"
#include "Core/AccelByteSettings.h"
#include "Core/ServerTime/AccelByteTimeManager.h"
#include "Models/AccelByteQosModels.h"

namespace AccelByte
{

/**
 * @brief AGS (AccelByte Gaming Services) Platform cached data that consists of information related to the services
 *		  such as environment variables, platform configs, list of QoS servers, etc.
 */
class ACCELBYTEUE4SDK_API FAccelBytePlatform 
	: public TSharedFromThis<FAccelBytePlatform, ESPMode::ThreadSafe>
{
public:
	FAccelBytePlatform(AccelByte::BaseSettingsPtr const& InSettings);

	virtual  ~FAccelBytePlatform();

	AccelByte::FAccelByteTimeManagerPtr GetTimeManager() const;

	void SetQosServers(FAccelByteModelsQosServerList const& InQosServers);
	TArray<FAccelByteModelsQosServer> GetQosServers();

	void SetLatencies(TArray<TPair<FString, float>> const& InLatencies);
	TArray<TPair<FString, float>> GetLatencies();
	void ClearLatencies();

	bool IsAddressResolved(FString const& NetAddress);
	void SetResolvedAddress(FString const& NetAddress, TSharedPtr<FInternetAddr> const& IPAddress);
	TSharedPtr<FInternetAddr> GetResolvedAddress(FString const& NetAddress);
	void RemoveResolvedAddress(FString const& NetAddress);
	void ClearResolvedAddresses();

	void SetIAMConfig(FIAMPublicSystemConfigResponse const& InConfig);
	FIAMPublicSystemConfigResponse GetIAMConfig();
	int32 GetMinQueryLength();
	int32 GetMaxQueryLength();
	int32 GetMaxEmailQueryLength();

	/**
	 * @brief Attempts to sync the TimeManager with the AccelByte backend. On failure, schedules
	 *        a retry with exponential backoff (2s, 4s, 8s, 16s, 32s) up to 5 attempts.
	 *        Without this, a single transient failure would leave TimeManager unsynced for the
	 *        entire process lifetime, causing TOTP generation/validation to silently fall back
	 *        to the local OS clock.
	 *
	 *        Also callable post-construction (e.g. after SDK environment switch updates the
	 *        TimeManager URL) to trigger a fresh round of sync attempts against the new URL.
	 *
	 * @param AttemptCount Pass 0 to start a fresh retry sequence.
	 */
	void AttemptTimeSyncWithRetry(int32 AttemptCount);

	/**
	 * @brief Force-restart the time-sync retry sequence even if one is already in flight.
	 *        Clears the in-progress flag set by AttemptTimeSyncWithRetry and starts a new
	 *        sequence. Intended for callers that need a fresh sync against new state - e.g.
	 *        FAccelByteInstance::OnSettingsEnvironmentChanges after updating the TimeManager URL.
	 */
	void RestartTimeSync();

private:
	AccelByte::BaseSettingsPtr SettingsPtr;

	AccelByte::FAccelByteTimeManagerPtr TimeManagerPtr;

	// Guards against concurrent retry chains: a public AttemptTimeSyncWithRetry called from the
	// constructor (deferred via FTSTicker) and again from environment-switch handlers can otherwise
	// start parallel ticker chains, each making redundant GetServerTime requests.
	FThreadSafeBool bSyncInProgress{ false };

	FRWLock QosServersMtx;
	FAccelByteModelsQosServerList QosServers;
	FRWLock LatenciesMtx;
	TArray<TPair<FString, float>> Latencies;
	FRWLock ResolvedAddressesMtx;
	TMap<FString, TSharedPtr<FInternetAddr>> ResolvedAddresses;

	FRWLock IAMConfigMtx;
	FIAMPublicSystemConfigResponse IAMConfig;
};

using FAccelBytePlatformRef = TSharedRef<FAccelBytePlatform, ESPMode::ThreadSafe>;
using FAccelBytePlatformPtr = TSharedPtr<FAccelBytePlatform, ESPMode::ThreadSafe>;
using FAccelBytePlatformWPtr = TWeakPtr<FAccelBytePlatform, ESPMode::ThreadSafe>;

}
