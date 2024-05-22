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
class ACCELBYTEUE4SDK_API GDPR : public FApiBase
{
public:
	GDPR(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~GDPR();
		
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

private:
	GDPR() = delete;
	GDPR(GDPR const&) = delete;
	GDPR(GDPR&&) = delete; 
};
	
}// Namespace Api
}// Namespace AccelByte