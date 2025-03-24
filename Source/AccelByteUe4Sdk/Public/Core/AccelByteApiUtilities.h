// Copyright (c) 2025 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "AccelByteBaseCredentials.h"
#include "AccelByteBaseSettings.h"
#include "AccelByteError.h"
#include "AccelByteHttpClient.h"
#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteUtilities.h"

struct FAccelByteModelsPubIp;

namespace AccelByte
{
	class FAccelByteTimeManager;

class ACCELBYTEUE4SDK_API FAccelByteApiUtilities : public TSharedFromThis<FAccelByteApiUtilities, ESPMode::ThreadSafe>
{
public:
	FAccelByteApiUtilities(TSharedRef<BaseCredentials, ESPMode::ThreadSafe> const InCredentialsRef
		, TSharedRef<BaseSettings, ESPMode::ThreadSafe> const InSettingsRef
		, TSharedRef<FHttpRetryScheduler, ESPMode::ThreadSafe> const InHttpRef
		, TSharedPtr<FAccelByteTimeManager, ESPMode::ThreadSafe> InTimeManager);

	/**
	 * @brief Get Public IP using api.ipify.org
	 *
	 * @param OnSuccess Callback function for successful delegate.
	 * @param OnError Callback function for error delegate.
	 */
	void GetPublicIP(THandler<FAccelByteModelsPubIp> const& OnSuccess, FErrorHandler const& OnError) const;

	/**
	 * @brief Download  acontent from specified URL with several callback functions including OnProgress, OnDownloaded, and OnError.
	 *
	 * @param Url Specified URL to download the content.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnDownloaded Callback function for successful download delegate.
	 * @param OnError Callback function for error delegate.
	 */
	void DownloadFrom(FString const& Url
	  , FHttpRequestProgressDelegate const& OnProgress
	  , THandler<TArray<uint8>> const& OnDownloaded
	  , FErrorHandler const& OnError);

	/**
	 * @brief Upload a content using specified URL with several callback functions including OnProgress, OnSuccess, and OnError.
	 *
	 * @param Url Specified URL to upload the content.
	 * @param DataUpload Content to upload.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnSuccess Callback function for successful upload delegate.
	 * @param OnError Callback function for error delegate.
	 * @param ContentType Specified content-type header which determine the type of uploaded content (default: application/octet-stream)
	 */
	void UploadTo(FString const& Url
	, TArray<uint8> const& DataUpload
	, FHttpRequestProgressDelegate const& OnProgress
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& ContentType = TEXT("application/octet-stream"));

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
	/**
	 * @brief Download a content from specified URL with several callback functions including OnProgress, OnDownloaded, and OnError.
	 *
	 * @param Url Specified URL to download the content.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnDownloaded Callback function for successful download delegate.
	 * @param OnError Callback function for error delegate.
	 */
	void DownloadFrom(FString const& Url
		, FHttpRequestProgressDelegate64 const& OnProgress
		, THandler<TArray<uint8>> const& OnDownloaded
		, FErrorHandler const& OnError);

	/**
	 * @brief Upload a content using specified URL with several callback functions including OnProgress, OnSuccess, and OnError.
	 *
	 * @param Url Specified URL to upload the content.
	 * @param DataUpload Content to upload.
	 * @param OnProgress Callback function for on progress delegate.
	 * @param OnSuccess Callback function for successful upload delegate.
	 * @param OnError Callback function for error delegate.
	 * @param ContentType Specified content-type header which determine the type of uploaded content (default: application/octet-stream)
	 */
	void UploadTo(FString const& Url
		, TArray<uint8> const& DataUpload
		, FHttpRequestProgressDelegate64 const& OnProgress
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& ContentType);
#endif

	/**
	 * @brief To be called from game server to decide the type of the game server management.
	 */
	EAccelByteCurrentServerManagementType GetCurrentServerManagementType() const;

	/**
	 * @brief Generate TOTP string
	 *
	 * @param SecretKey
	 * @param CodeLength
	 * @param TimeStep
	 * @return TOTP string, may be empty if failed to generate
	 */
	const FString GenerateTOTP(const FString& SecretKey, int CodeLength = 6, int TimeStep = 30);

	/**
	 * @brief Validate a TOTP against a User.
	 *
	 * @param ServerSecretKey Secret key of the server.
	 * @param TOTP TOTP to validate.
	 * @param UserId UserId to validate.
	 * @return returns true if TOTP is valid for the UserId provided.
	 */
	bool ValidateTOTP(const FString& ServerSecretKey, const FString& TOTP, const FString& UserId);

private:
	TSharedRef<BaseCredentials, ESPMode::ThreadSafe> CredentialsRef;
	TSharedRef<BaseSettings, ESPMode::ThreadSafe> SettingsRef;
	TSharedRef<FHttpRetryScheduler, ESPMode::ThreadSafe> HttpRef;
	FHttpClient HttpClient;
	TWeakPtr<FAccelByteTimeManager, ESPMode::ThreadSafe> TimeManager;
	
	/**
	 * @brief Generate TOTP string
	 *
	 * @param CurrentTime
	 * @param SecretKey
	 * @param CodeLength
	 * @param TimeStep
	 * @return TOTP string
	 */
	FString GenerateTOTP(int64 CurrentTime, const FString& SecretKey, int32 CodeLength, int32 TimeStep);

	/**
	 * @brief Generate array of acceptable TOTPs
	 *
	 * @param ServerSecretKey
	 * @param UserId
	 * @return Array of acceptable TOTPs
	 */
	TArray<FString> GenerateAcceptableTOTP(const FString& ServerSecretKey, const FString& UserId);
};

using FAccelByteApiUtilitiesPtr = TSharedPtr<FAccelByteApiUtilities, ESPMode::ThreadSafe>;
using FAccelByteApiUtilitiesRef = TSharedRef<FAccelByteApiUtilities, ESPMode::ThreadSafe>;
using FAccelByteApiUtilitiesWPtr = TWeakPtr<FAccelByteApiUtilities, ESPMode::ThreadSafe>;
}
