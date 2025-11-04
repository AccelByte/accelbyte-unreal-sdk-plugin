// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
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
private:
	AccelByte::BaseSettingsPtr SettingsPtr;

	AccelByte::FAccelByteTimeManagerPtr TimeManagerPtr;

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
