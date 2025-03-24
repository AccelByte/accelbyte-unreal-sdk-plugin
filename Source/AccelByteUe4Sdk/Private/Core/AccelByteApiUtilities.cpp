// Copyright (c) 2025 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiUtilities.h"

#include "openssl/sha.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteUtilities.h"
#include "Core/ServerTime/AccelByteTimeManager.h"

namespace AccelByte
{

FAccelByteApiUtilities::FAccelByteApiUtilities(TSharedRef<BaseCredentials, ESPMode::ThreadSafe> const InCredentialsRef
	, TSharedRef<BaseSettings, ESPMode::ThreadSafe> const InSettingsRef
	, TSharedRef<FHttpRetryScheduler, ESPMode::ThreadSafe> const InHttpRef
	, TSharedPtr<FAccelByteTimeManager, ESPMode::ThreadSafe> InTimeManager)
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
		UE_LOG(LogAccelByte, Warning, TEXT("TimeManager is not in sync with server, generating TOTP using local time."))
	}

	return GenerateTOTP(CurrentTime, SecretKey, CodeLength, TimeStep);
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
	
	constexpr int32 AcceptableWindow{30};
	constexpr int32 CodeLength{6};
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
		UE_LOG(LogAccelByte, Warning, TEXT("TimeManager is not in sync with server, generating TOTP using local time."))
	}

	for (int32 i = 0; i < AcceptableWindow; i++)
	{
		const FString ServerGeneratedTOTP = GenerateTOTP(CurrentTime - i, HashString, CodeLength, AcceptableWindow);
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
