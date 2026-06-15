// Copyright (c) 2025 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiUtilities.h"

#include "openssl/sha.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"
#include "Core/ServerTime/AccelByteTimeManager.h"

namespace AccelByte
{

FAccelByteApiUtilities::FAccelByteApiUtilities(TSharedRef<BaseCredentials, ESPMode::ThreadSafe> const& InCredentialsRef
	, TSharedRef<BaseSettings, ESPMode::ThreadSafe> const& InSettingsRef
	, TSharedRef<FHttpRetrySchedulerBase, ESPMode::ThreadSafe> const& InHttpRef
	, TWeakPtr<FAccelByteTimeManager, ESPMode::ThreadSafe> const& InTimeManager)
	: CredentialsRef(InCredentialsRef)
	, SettingsRef(InSettingsRef)
	, HttpRef(InHttpRef)
	, HttpClient(*InCredentialsRef, *InSettingsRef, *InHttpRef)
	, TimeManager(InTimeManager)
{
}

// used in ServerDSM::RegisterLocalServerToDSM
void FAccelByteApiUtilities::GetPublicIP(THandler<FAccelByteModelsPubIp> const& OnSuccess
	, FErrorHandler const& OnError) const
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("ipify support is deprecated. Please use ISocketSubsystem to get public IP address."));
	
	FString Url = FString::Printf(TEXT("https://api.ipify.org?format=json"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	
	HttpRef->ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

// used for UGC download client and server
void FAccelByteApiUtilities::DownloadFrom(FString const& Url
      , FHttpRequestProgressDelegate const& OnProgress
      , THandler<TArray<uint8>> const& OnDownloaded
      , FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("FHttpRequestProgressDelegate is Deprecated - please use DownloadFrom with FHttpRequestProgressDelegate64 param instead."));
#endif

	HttpClient.Request(TEXT("GET"), Url, {}, TEXT(""), {{TEXT("Accept"), TEXT("application/octet-stream")}}, OnDownloaded, OnProgress, OnError);
}

// used for UGC upload client and server
void FAccelByteApiUtilities::UploadTo(FString const& Url
	, TArray<uint8> const& DataUpload
	, FHttpRequestProgressDelegate const& OnProgress
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& ContentType)
{
	FReport::Log(FString(__FUNCTION__));
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("FHttpRequestProgressDelegate is Deprecated - please use UploadTo with FHttpRequestProgressDelegate64 param instead."));
#endif

	HttpClient.Request(TEXT("PUT"), Url, {}, DataUpload, { {TEXT("Content-Type"), ContentType} }, OnSuccess, OnProgress, OnError);
	
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
void FAccelByteApiUtilities::DownloadFrom(FString const& Url
	, FHttpRequestProgressDelegate64 const& OnProgress
	, THandler<TArray<uint8>> const& OnDownloaded
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	HttpClient.Request(TEXT("GET"), Url, {}, TEXT(""), { {TEXT("Accept"), TEXT("application/octet-stream")} }, OnDownloaded, OnProgress, OnError);
}

void FAccelByteApiUtilities::UploadTo(FString const& Url
	, TArray<uint8> const& DataUpload
	, FHttpRequestProgressDelegate64 const& OnProgress
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& ContentType)
{
	FReport::Log(FString(__FUNCTION__));

	HttpClient.Request(TEXT("PUT"), Url, {}, DataUpload, { {TEXT("Content-Type"), ContentType} }, OnSuccess, OnProgress, OnError);

}
#endif

// used in SessionInterfaceV2, server side operations
EAccelByteCurrentServerManagementType FAccelByteApiUtilities::GetCurrentServerManagementType() const
{
	//Sort this decision based on priority
	if (!IsRunningDedicatedServer())
	{
		return EAccelByteCurrentServerManagementType::NOT_A_SERVER;
	}

	const ServerSettingsPtr ServerSettingsPtr = StaticCastSharedPtr<ServerSettings, BaseSettings, ESPMode::ThreadSafe>(SettingsRef);

	if (ServerSettingsPtr.IsValid() && !ServerSettingsPtr->DSId.IsEmpty())
	{
		return EAccelByteCurrentServerManagementType::ONLINE_AMS;
	}

	const FString PodNameValue = FPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"));
	if (!PodNameValue.IsEmpty())
	{
		return EAccelByteCurrentServerManagementType::ONLINE_ARMADA;
	}
	else
	{
		return EAccelByteCurrentServerManagementType::LOCAL_SERVER;
	}
}

FString FAccelByteApiUtilities::GenerateTOTP(int64 CurrentTime, const FString& SecretKey, int32 CodeLength, int32 TimeStep)
{
	// Calculate the time step
	const uint64 TimeStepCount = CurrentTime / TimeStep;

	// Convert the time step count to network byte order
	const uint64 TimeStepCountNetworkOrder = ((TimeStepCount & 0x00000000000000FF) << 56) |
		((TimeStepCount & 0x000000000000FF00) << 40) |
		((TimeStepCount & 0x0000000000FF0000) << 24) |
		((TimeStepCount & 0x00000000FF000000) << 8) |
		((TimeStepCount & 0x000000FF00000000) >> 8) |
		((TimeStepCount & 0x0000FF0000000000) >> 24) |
		((TimeStepCount & 0x00FF000000000000) >> 40) |
		((TimeStepCount & 0xFF00000000000000) >> 56);

	// Convert the secret key to bytes
	uint8 EncodeOutput[ShaDigestLength];
	TArray<uint8> SecretKeyBytes = FAccelByteArrayByteFStringConverter::FStringToBytes(SecretKey);

	// Calculate the HMAC-SHA-1
	FSHA1::HMACBuffer(SecretKeyBytes.GetData(), SecretKeyBytes.Num(), 
		reinterpret_cast<const uint8*>(&TimeStepCountNetworkOrder), sizeof(TimeStepCountNetworkOrder), 
		EncodeOutput); 

	// Dynamic truncation to get the offset and Calculate the truncated value
	uint32 TruncationValue = (EncodeOutput[15] & 0x7f) << 24 |
		(EncodeOutput[16] & 0xff) << 16 |
		(EncodeOutput[17] & 0xff) << 8 |
		(EncodeOutput[18] & 0xff);
	 
	// Modulo to get the final code 
	uint32 TruncatedHash = TruncationValue; 
	TruncatedHash %= static_cast<uint32>(FMath::Pow(10.0f, CodeLength));

	// Convert to string with leading zeros if necessary
	FString Result = FString::Printf(TEXT("%0*d"), CodeLength, TruncatedHash);

	return Result;
}

// used in session validation, client side
const FString FAccelByteApiUtilities::GenerateTOTP(const FString& SecretKey, int CodeLength, int TimeStep)
{
	FAccelByteTimeManagerPtr TimeManagerPtr = TimeManager.Pin();
	if(!TimeManagerPtr.IsValid())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("FAccelByteApiUtilities::GenerateTOTP failed, TimeManager is invalid!"));
		return TEXT("");
	}

	// Get the current Unix time
	int64 CurrentTime;
	if (TimeManagerPtr->IsInSync())
	{
		CurrentTime = TimeManagerPtr->GetCurrentServerTime().ToUnixTimestamp();
	}
	else
	{
		CurrentTime = FDateTime::UtcNow().ToUnixTimestamp();
		UE_LOG(LogAccelByte, Warning,
			TEXT("FAccelByteApiUtilities::GenerateTOTP: TimeManager is not in sync with server; falling back to local OS clock. The generated TOTP may be rejected by remote validators if the local clock is drifted. Prefer GenerateTOTPAsync() which ensures sync before generating."))
	}

	return GenerateTOTP(CurrentTime, SecretKey, CodeLength, TimeStep);
}

bool FAccelByteApiUtilities::IsTimeManagerInSync() const
{
	auto Pin = TimeManager.Pin();
	return Pin.IsValid() && Pin->IsInSync();
}

void FAccelByteApiUtilities::GenerateTOTPAsync(const FString& SecretKey
	, THandler<FString> const& OnReady
	, FErrorHandler const& OnFailed
	, int CodeLength
	, int TimeStep)
{
	FAccelByteTimeManagerPtr TimeManagerPtr = TimeManager.Pin();
	if (!TimeManagerPtr.IsValid())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("FAccelByteApiUtilities::GenerateTOTPAsync: TimeManager is invalid"));
		OnFailed.ExecuteIfBound(-1, TEXT("TimeManager is invalid"));
		return;
	}

	// Already synced - skip the network round trip and generate immediately.
	if (TimeManagerPtr->IsInSync())
	{
		const int64 CurrentTime = TimeManagerPtr->GetCurrentServerTime().ToUnixTimestamp();
		OnReady.ExecuteIfBound(GenerateTOTP(CurrentTime, SecretKey, CodeLength, TimeStep));
		return;
	}

	// DEFENSIVE: push the current Settings.BasicServerUrl into the TimeManager just-in-time
	// before triggering sync. This self-heals the cached URL on every async TOTP generation
	// without relying on the env-switch refresh chain to have fired correctly. Covers cases
	// like:
	//   - env was switched before this AccelByteInstance subscribed to OnEnvironmentChanged
	//   - the event dispatch didn't fire in a packaged build
	//   - Settings was resolved into a valid URL only after the TimeManager was constructed
	//
	// IMPORTANT: `Settings` (the client subclass) and `ServerSettings` BOTH shadow
	// `BaseSettings::BasicServerUrl` with their own field. So reading via the BaseSettings
	// type returns the always-empty base field, not the value that Settings::Reset writes.
	// Cast down to the derived type to access the actually-populated URL.
	//
	// Guarded by IsRunningDedicatedServer(): FAccelByteApiUtilities is shared infrastructure
	// also used by the server SDK path. If a server-side caller ever invokes GenerateTOTPAsync,
	// SettingsRef would point at a ServerSettings - the StaticCastSharedRef<Settings> would
	// produce a mismatched pointer and trigger UB on the next member access.
	if (!IsRunningDedicatedServer())
	{
		// GenerateTOTPAsync is intended for client-side use; the underlying object behind
		// SettingsRef is therefore an AccelByte::Settings.
		const TSharedRef<Settings, ESPMode::ThreadSafe> ClientSettingsRef = StaticCastSharedRef<Settings>(SettingsRef);
		TimeManagerPtr->SetBasicServerUrl(ClientSettingsRef->BasicServerUrl);
	}

	// Not synced yet. Trigger sync and complete after.
	TWeakPtr<FAccelByteApiUtilities, ESPMode::ThreadSafe> SelfWeak = AsShared();

	TimeManagerPtr->GetServerTime(
		THandler<FTime>::CreateLambda(
			[SelfWeak, SecretKey, CodeLength, TimeStep, OnReady, OnFailed](const FTime& ServerTime)
			{
				auto Self = SelfWeak.Pin();
				if (!Self.IsValid())
				{
					OnFailed.ExecuteIfBound(-1, TEXT("ApiUtilities was destroyed during TimeManager sync"));
					return;
				}
				const int64 CurrentTime = ServerTime.CurrentTime.ToUnixTimestamp();
				OnReady.ExecuteIfBound(Self->GenerateTOTP(CurrentTime, SecretKey, CodeLength, TimeStep));
			}),
		FErrorHandler::CreateLambda(
			[OnFailed](int32 Code, const FString& Msg)
			{
				UE_LOG(LogAccelByte, Warning,
					TEXT("FAccelByteApiUtilities::GenerateTOTPAsync: TimeManager sync failed (error %d: %s). TOTP not generated. Caller should retry or surface this to the player."),
					Code, *Msg);
				OnFailed.ExecuteIfBound(Code, FString::Printf(TEXT("TimeManager sync failed: %s"), *Msg));
			}));
}

// used in Session validation, server side
TArray<FString> FAccelByteApiUtilities::GenerateAcceptableTOTP(const FString& ServerSecretKey, const FString& UserId)
{
	FAccelByteTimeManagerPtr TimeManagerPtr = TimeManager.Pin();
	if(!TimeManagerPtr.IsValid())
	{
		UE_LOG(LogAccelByte, Warning, TEXT("FAccelByteApiUtilities::GenerateTOTP failed, TimeManager is invalid!"));
		return TArray<FString>();
	}
	
	// Accept buckets in both directions from the server's current time. Asymmetric on purpose:
	//
	//   - NumBackwardSteps (60s): tolerates travel + DS-processing latency between the moment
	//     the client generates the TOTP and the moment the DS validates it. Normal traffic is
	//     sub-second; 60s comfortably covers slow networks, mild retransmits, and bursty CPU
	//     stalls on the DS side.
	//
	//   - NumForwardSteps (30s): RFC 6238 default. The previous patch series used 10 steps
	//     (5 min) to absorb VR / Quest client-clock skew that ran AHEAD of server time. That
	//     widening is no longer needed once consumers follow the canonical flow exposed by
	//     this SDK - call GenerateTOTPAsync (which forces a fresh TimeManager sync before
	//     producing the code) and pull the per-session secret immediately before DS travel.
	//     VAIL (ASP-13243) confirmed adopting this flow; the wider window only enabled
	//     latent drift bugs in consumers that bypassed it.
	//
	// Security note: any forward window directly extends the replay surface for an intercepted
	// TOTP. Keeping forward at 1 step (~30s) caps the replay window. Primary mitigation against
	// replay remains the per-(session, user) scoping of the underlying secret - an intercepted
	// TOTP only authorises that specific user against that specific session, and the DS-side
	// join logic typically rejects duplicate joins for an already-joined user. A future
	// enhancement could add an explicit replay cache here for defence-in-depth.
	constexpr int32 CodeLength{6};
	constexpr int32 TimeStep{30};
	constexpr int32 NumBackwardSteps{2};
	constexpr int32 NumForwardSteps{1};
	TArray<FString> AcceptableTOTP;
	FString HashString = FAccelByteUtilities::GenerateHashString(ServerSecretKey + UserId);

	// Get the current Unix time
	int64 CurrentTime;
	if (TimeManagerPtr->IsInSync())
	{
		CurrentTime = TimeManagerPtr->GetCurrentServerTime().ToUnixTimestamp();
	}
	else
	{
		CurrentTime = FDateTime::UtcNow().ToUnixTimestamp();
		UE_LOG(LogAccelByte, Warning,
			TEXT("FAccelByteApiUtilities::GenerateAcceptableTOTP: TimeManager is not in sync with server; the DS is falling back to its local OS clock for validation. If both client and DS are unsynced, they may disagree and reject valid TOTPs. Ensure the DS has network access to the AccelByte time endpoint."))
	}

	// Current bucket + NumBackwardSteps previous buckets.
	for (int32 i = 0; i <= NumBackwardSteps; i++)
	{
		const FString ServerGeneratedTOTP = GenerateTOTP(CurrentTime - i * TimeStep, HashString, CodeLength, TimeStep);
		if (!AcceptableTOTP.Contains(ServerGeneratedTOTP))
		{
			AcceptableTOTP.Emplace(ServerGeneratedTOTP);
		}
	}
	// NumForwardSteps future buckets.
	for (int32 i = 1; i <= NumForwardSteps; i++)
	{
		const FString ServerGeneratedTOTP = GenerateTOTP(CurrentTime + i * TimeStep, HashString, CodeLength, TimeStep);
		if (!AcceptableTOTP.Contains(ServerGeneratedTOTP))
		{
			AcceptableTOTP.Emplace(ServerGeneratedTOTP);
		}
	}

	return AcceptableTOTP;
}

bool FAccelByteApiUtilities::ValidateTOTP(const FString& ServerSecretKey, const FString& TOTP, const FString& UserId)
{
	TArray<FString> AcceptableTOTP = GenerateAcceptableTOTP(ServerSecretKey, UserId);

	return AcceptableTOTP.Contains(TOTP);
}
}
