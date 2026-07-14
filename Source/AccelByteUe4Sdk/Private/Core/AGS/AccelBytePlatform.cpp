// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AGS/AccelBytePlatform.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteBaseSettings.h"

#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteDefines.h"

namespace AccelByte
{

FAccelBytePlatform::FAccelBytePlatform(AccelByte::BaseSettingsPtr const& InSettings)
	: SettingsPtr(InSettings)
{
	TimeManagerPtr = MakeShared<AccelByte::FAccelByteTimeManager, ESPMode::ThreadSafe>(SettingsPtr->BasicServerUrl);

	// AsShared() cannot be used inside the constructor (TSharedFromThis's internal weak self
	// pointer is only initialized once MakeShared finishes wrapping this object). Defer the
	// first sync attempt to the next tick so AttemptTimeSyncWithRetry can safely capture a
	// weak self for its async retry callbacks.
	FTickerAlias::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda([this](float /*DeltaTime*/) -> bool
		{
			AttemptTimeSyncWithRetry(0);
			return false; // run once
		}),
		0.0f);
}

void FAccelBytePlatform::AttemptTimeSyncWithRetry(int32 AttemptCount)
{
	constexpr int32 MaxAttempts = 5;
	constexpr float BaseDelaySec = 2.0f;

	if (!TimeManagerPtr.IsValid())
	{
		return;
	}

	// Re-entry guard: only the AttemptCount == 0 entry-point checks the flag. Subsequent
	// recursive calls (AttemptCount > 0) are continuations of the same chain and skip the check.
	// AtomicSet returns the previous value, so a return of true means another chain is already
	// running and this entry should bail out.
	if (AttemptCount == 0 && bSyncInProgress.AtomicSet(true))
	{
		UE_LOG(LogAccelByte, Verbose,
			TEXT("FAccelBytePlatform::AttemptTimeSyncWithRetry: another sync sequence is already in flight; skipping. Use RestartTimeSync() to force a fresh sequence."));
		return;
	}

	TWeakPtr<FAccelBytePlatform, ESPMode::ThreadSafe> SelfWeak = AsShared();

	TimeManagerPtr->GetServerTime(
		THandler<FTime>::CreateLambda([SelfWeak, AttemptCount](const FTime& /*ServerTime*/)
		{
			UE_LOG(LogAccelByte, Log,
				TEXT("FAccelBytePlatform: TimeManager initial sync successful after %d attempt(s). TOTP generation will use AccelByte server time."),
				AttemptCount + 1);
			if (auto Self = SelfWeak.Pin())
			{
				Self->bSyncInProgress.AtomicSet(false);
			}
		}),
		FErrorHandler::CreateLambda([SelfWeak, AttemptCount, MaxAttempts, BaseDelaySec](int32 Code, const FString& Msg)
		{
			const int32 NextAttempt = AttemptCount + 1;
			if (NextAttempt >= MaxAttempts)
			{
				UE_LOG(LogAccelByte, Warning,
					TEXT("FAccelBytePlatform: TimeManager initial sync failed after %d attempts (last error %d: %s). TOTP generation/validation will fall back to the local OS clock; on devices with drifted clocks this may cause TOTP rejections. To recover, call TimeManager->GetServerTime() manually when the network is available."),
					NextAttempt, Code, *Msg);
				if (auto Self = SelfWeak.Pin())
				{
					Self->bSyncInProgress.AtomicSet(false);
				}
				return;
			}

			const float DelaySec = BaseDelaySec * FMath::Pow(2.0f, static_cast<float>(AttemptCount));
			UE_LOG(LogAccelByte, Warning,
				TEXT("FAccelBytePlatform: TimeManager sync failed (attempt %d/%d, error %d: %s); retrying in %.1fs"),
				NextAttempt, MaxAttempts, Code, *Msg, DelaySec);

			FTickerAlias::GetCoreTicker().AddTicker(
				FTickerDelegate::CreateLambda([SelfWeak, NextAttempt](float /*DeltaTime*/) -> bool
				{
					if (auto Self = SelfWeak.Pin())
					{
						Self->AttemptTimeSyncWithRetry(NextAttempt);
					}
					return false; // run once
				}),
				DelaySec);
		}));
}

void FAccelBytePlatform::RestartTimeSync()
{
	// Force-clear the in-progress flag so a new sequence can start even if one is mid-flight.
	// The in-flight chain's network callback may still fire and clear the flag again - that's
	// fine; once the new chain begins, it owns the flag.
	bSyncInProgress.AtomicSet(false);
	AttemptTimeSyncWithRetry(0);
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