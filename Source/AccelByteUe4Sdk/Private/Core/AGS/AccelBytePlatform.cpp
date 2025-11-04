// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AGS/AccelBytePlatform.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteBaseSettings.h"

#include "AccelByteUe4SdkModule.h"

namespace AccelByte
{

FAccelBytePlatform::FAccelBytePlatform(AccelByte::BaseSettingsPtr const& InSettings)
	: SettingsPtr(InSettings)
{
	TimeManagerPtr = MakeShared<AccelByte::FAccelByteTimeManager, ESPMode::ThreadSafe>(SettingsPtr->BasicServerUrl);
	TimeManagerPtr->GetServerTime({}, {});
}

FAccelBytePlatform::~FAccelBytePlatform()
{
	
}

AccelByte::FAccelByteTimeManagerPtr FAccelBytePlatform::GetTimeManager() const
{
	return TimeManagerPtr;
}

void FAccelBytePlatform::SetQosServers(FAccelByteModelsQosServerList const& InQosServers)
{
	FWriteScopeLock WriteLock(QosServersMtx);
	QosServers = InQosServers; // Cache for the session
}

TArray<FAccelByteModelsQosServer> FAccelBytePlatform::GetQosServers()
{
	FReadScopeLock ReadLock(QosServersMtx);
	return QosServers.Servers;
}

void FAccelBytePlatform::SetLatencies(TArray<TPair<FString, float>> const& InLatencies)
{
	FWriteScopeLock WriteLock(LatenciesMtx);
	Latencies = InLatencies;
}

TArray<TPair<FString, float>> FAccelBytePlatform::GetLatencies()
{
	FReadScopeLock ReadLock(LatenciesMtx);
	return Latencies;
}

void FAccelBytePlatform::ClearLatencies()
{
	FWriteScopeLock Lock(LatenciesMtx);
	Latencies.Empty();
}

bool FAccelBytePlatform::IsAddressResolved(FString const& NetAddress)
{
	FReadScopeLock ReadLock(ResolvedAddressesMtx);
	return ResolvedAddresses.Contains(NetAddress);
}

void FAccelBytePlatform::SetResolvedAddress(FString const& NetAddress, TSharedPtr<FInternetAddr> const& IPAddress)
{
	if (!IsAddressResolved(NetAddress))
	{
		FWriteScopeLock WriteLock(ResolvedAddressesMtx);
		ResolvedAddresses.Emplace(NetAddress, nullptr);
	}

	FWriteScopeLock WriteLock(ResolvedAddressesMtx);
	ResolvedAddresses[NetAddress] = IPAddress;
}

TSharedPtr<FInternetAddr> FAccelBytePlatform::GetResolvedAddress(FString const& NetAddress)
{
	TSharedPtr<FInternetAddr> IpAddress = nullptr;

	if (IsAddressResolved(NetAddress))
	{
		FReadScopeLock ReadLock(ResolvedAddressesMtx);
		IpAddress = ResolvedAddresses[NetAddress];
	}

	return IpAddress;
}

void FAccelBytePlatform::RemoveResolvedAddress(FString const& NetAddress)
{
	if (IsAddressResolved(NetAddress))
	{
		FWriteScopeLock WriteLock(ResolvedAddressesMtx);
		ResolvedAddresses.Remove(NetAddress);
	}
}

void FAccelBytePlatform::ClearResolvedAddresses()
{
	FWriteScopeLock WriteLock(ResolvedAddressesMtx);
	ResolvedAddresses.Empty();
}

void FAccelBytePlatform::SetIAMConfig(FIAMPublicSystemConfigResponse const& InConfig)
{
	FWriteScopeLock WriteLock(IAMConfigMtx);
	IAMConfig = InConfig;
}

FIAMPublicSystemConfigResponse FAccelBytePlatform::GetIAMConfig()
{
	FReadScopeLock ReadLock(IAMConfigMtx);
	return IAMConfig;
}

int32 FAccelBytePlatform::GetMinQueryLength()
{
	FReadScopeLock ReadLock(IAMConfigMtx);
	return IAMConfig.SearchQueryMinLength;
}

int32 FAccelBytePlatform::GetMaxQueryLength()
{
	FReadScopeLock ReadLock(IAMConfigMtx);
	return IAMConfig.SearchQueryMaxLength;
}

int32 FAccelBytePlatform::GetMaxEmailQueryLength()
{
	FReadScopeLock ReadLock(IAMConfigMtx);
	return IAMConfig.SearchQueryMaxLengthForEmail;
}

}