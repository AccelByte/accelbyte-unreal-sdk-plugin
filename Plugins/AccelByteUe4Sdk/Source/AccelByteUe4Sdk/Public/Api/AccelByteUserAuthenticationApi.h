// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteUserAuthenticationApi.generated.h"

UENUM(BlueprintType)
/**
 *  @brief Supported platforms by AccelByte IAM.
 */
enum class EAccelBytePlatformType : uint8
{
	Steam,
	Google,
	Facebook,
	Android,
	iOS,
	Device,
	Twitch,
	Oculus,
	Twitter
};

namespace AccelByte
{
namespace Api
{

/**
 * @brief API for authenticating user; the access tokens are stored in memory for subsequent API calls.
 */
class ACCELBYTEUE4SDK_API UserAuthentication
{
public:

	DECLARE_DELEGATE(FLoginWithClientCredentialsSuccess);
	/**
	 * @brief This is to get access token from `client_credentials` grant, then store the access token in memory.
	 * You shouldn't use this if your application is a client. It is intended for machine-to-machine (server).
	 * It is "required" for user management (create new user, reset password, etc).
	 * 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void LoginWithClientCredentials(const FLoginWithClientCredentialsSuccess& OnSuccess, const FErrorHandler& OnError);
	
	DECLARE_DELEGATE(FLoginWithUsernameAndPasswordSuccess);
	/**
	 * @brief Log in with email/phone number account.
	 * 
	 * @param Username User email address.
	 * @param Password Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LoginWithUsernameAndPassword(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FLoginWithOtherPlatformAccountSuccess);
	/**
	 * @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
	 * 
	 * @param PlatformId Specify platform type that chosen by user to log in.
	 * @param PlatformToken Authentication code that provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void LoginWithOtherPlatformAccount(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FLoginWithDeviceIdSuccess);
	/**
	 * @brief Log in with device ID (anonymous log in).
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LoginWithDeviceId(const FLoginWithDeviceIdSuccess& OnSuccess, const FErrorHandler& OnError);
	
    DECLARE_DELEGATE_OneParam(FGetAccessTokenWithAuthorizationCodeGrantSuccess, const FAccelByteModelsOauth2Token&);
    /**
     * @brief login from Accelbyte Launcher
     *
     * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
     * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
     * @param OnError This will be called when the operation failed.
     */
    static void LoginFromLauncher(const FString& AuthorizationCode, const FGetAccessTokenWithAuthorizationCodeGrantSuccess& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Remove access tokens, user ID, and other credentials from memory.
	 */
	static void ForgetAllCredentials();
};

} // Namespace Api
} // Namespace AccelByte
