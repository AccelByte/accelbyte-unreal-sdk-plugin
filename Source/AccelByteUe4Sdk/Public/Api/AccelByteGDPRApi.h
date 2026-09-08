// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteApiBase.h"
#include "Models/AccelByteGDPRModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{
/**
 * @brief GDPR Api is used for managing the deletion of user data
 */
class ACCELBYTEUE4SDK_API GDPR 
	: public FApiBase
	, public TSharedFromThis<GDPR, ESPMode::ThreadSafe>
{
public:
	GDPR(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	GDPR(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~GDPR();
		
	/**
	 * @brief Used to submit user's account deletion requests. Used when your account is full account
	 * 
	 * @param Password IAM password of the user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRSubmitAccountDeletionResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitAccountDeletion(FString const& Password
		, THandler<FAccelByteModelsGDPRSubmitAccountDeletionResponse> const& OnSuccess
		, FErrorHandler const& OnError);
		
	/**
	 * @brief Used to submit account deletion requests. Used when your account is headless account
	 * 
	 * @param PlatformType The PlatformType (Steam, PS4, Xbox, etc).
	 * @param PlatformToken Authentication code that provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRSubmitAccountDeletionResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitAccountDeletionOtherPlatform(EAccelBytePlatformType PlatformType
		, FString const& PlatformToken
		, THandler<FAccelByteModelsGDPRSubmitAccountDeletionResponse> const& OnSuccess
		, FErrorHandler const& OnError);
		
	/**
	 * @brief Used to submit account deletion requests. Used when your account is headless account
	 * 
	 * @param PlatformId Specify platform type, string type of this field makes support OpenID Connect (OIDC).
	 * @param PlatformToken Authentication code that provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRSubmitAccountDeletionResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitAccountDeletionOtherPlatformId(FString const& PlatformId
		, FString const& PlatformToken
		, THandler<FAccelByteModelsGDPRSubmitAccountDeletionResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Used to get status of account deletion requests.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRAccountDeletionStatusResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetStatusAccountDeletion(THandler<FAccelByteModelsGDPRAccountDeletionStatusResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Used to cancel account deletion requests.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelAccountDeletion(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Used to submit a personal data request. Used when your account is a headless account (game client, platform-token flow).
	 *
	 * @param PlatformType The PlatformType (Steam, PS4, Xbox, etc).
	 * @param PlatformToken Authentication token provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRSubmitPersonalDataRequestResponse.
	 * @param OnError This will be called when the operation failed. NOTE: a platformId/platformToken mismatch may currently surface as a generic 500/502 rather than a clean 4xx
	 * @param Email Optional email address to receive notifications and the download link once the request completes. NOTE: there is currently no other way to retrieve the completed download link for this flow (no generate-URL endpoint exists for the platform-token path), so a request submitted without Email has no retrieval path once completed.
	 * @param LanguageTag Optional language tag for the notification email.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitMyPersonalDataRequest(EAccelBytePlatformType PlatformType
		, FString const& PlatformToken
		, THandler<FAccelByteModelsGDPRSubmitPersonalDataRequestResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Email = TEXT("")
		, FString const& LanguageTag = TEXT(""));

	/**
	 * @brief Used to submit a personal data request. Used when your account is a headless account (game client, platform-token flow).
	 *
	 * @param PlatformId Specify platform type, string type of this field makes support OpenID Connect (OIDC).
	 * @param PlatformToken Authentication token provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRSubmitPersonalDataRequestResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Email Optional email address to receive notifications and the download link once the request completes. See SubmitMyPersonalDataRequest for why omitting it currently means the download link is unrecoverable.
	 * @param LanguageTag Optional language tag for the notification email.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SubmitMyPersonalDataRequestOtherPlatformId(FString const& PlatformId
		, FString const& PlatformToken
		, THandler<FAccelByteModelsGDPRSubmitPersonalDataRequestResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Email = TEXT("")
		, FString const& LanguageTag = TEXT(""));

	/**
	 * @brief Used to get the caller's personal data requests.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGDPRPersonalDataRequestsResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the request data.
	 * @param Limit The limit of the request data.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMyPersonalDataRequests(THandler<FAccelByteModelsGDPRPersonalDataRequestsResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20);

	/**
	 * @brief Used to cancel a pending personal data request.
	 *
	 * @param RequestDate The request date identifying which request to cancel, as returned by SubmitMyPersonalDataRequest(OtherPlatformId) or GetMyPersonalDataRequests.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelMyPersonalDataRequest(FDateTime const& RequestDate
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	GDPR() = delete;
	GDPR(GDPR const&) = delete;
	GDPR(GDPR&&) = delete; 
};
	
using GDPRRef = TSharedRef<GDPR, ESPMode::ThreadSafe>;
using GDPRPtr = TSharedPtr<GDPR, ESPMode::ThreadSafe>;
using GDPRWPtr = TWeakPtr<GDPR, ESPMode::ThreadSafe>;

}// Namespace Api
}// Namespace AccelByte