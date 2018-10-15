// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteApiIdentity.h"
#include "AccelByteApiUserProfile.h"
#include "AccelByteModelsUserProfile.h"



namespace AccelByte
{
namespace Api
{

/**
 * @brief A convenience class for using Identity and UserProfile APIs.
 * Stores access tokens and other credentials after successful login.
 */
class ACCELBYTEUE4SDK_API User
{
public:

	/**
	 *  @brief Supported platforms by AccelByte IAM.
	 */
	enum class EPlatformType : uint8
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
	
	DECLARE_DELEGATE(FClientLoginSuccess);
	/**
	 * @brief This is to get access token from `client_credentials` grant.
	 * It is "required" for user management (create new user, reset password, etc).
	 * @todo IAM, PLEASE remove this requirement.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void ClientLogin(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FClientLoginSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginWithOtherPlatformAccountSuccess);
	/**
	 * @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param Namespace Namespace.
	 * @param PlatformId Specify platform type that chosen by user to log in.
	 * @param Token Authentication code that provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, uint8 PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginWithEmailAccountSuccess);
	/**
	 * @brief Log in with email account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param Namespace Namespace.
	 * @param Email User email address.
	 * @param Password Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LoginWithEmailAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FString Password, FLoginWithEmailAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginWithDeviceIdSuccess);
	/**
	 * @brief Log in as a guest (anonymous log in).
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param Namespace Namespace.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FLoginWithDeviceIdSuccess OnSuccess, ErrorDelegate OnError);

	/**
	 * @brief Remove access tokens, user ID, and other credentials from memory.
	 */
	static void ResetCredentials();

	DECLARE_DELEGATE(FUpgradeHeadlessAccountSuccess);
	/**
	 * @brief User should log in with headless account to upgrade it with email.
	 *
	 * @param Email User's valid email.
	 * @param Password User's password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UpgradeHeadlessAccount(FString ServerBaseUrl, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FCreateEmailAccountSuccess, const FAccelByteModelsUserCreateResponse&);
	/**
	 * @brief Register a new user with email-based account, then send a verification code to that email.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param Email User email address.
	 * @param Password User chosen password.
	 * @param DisplayName User display name.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateEmailAccount(FString ServerBaseUrl, FString Email, FString Password, FString DisplayName, FCreateEmailAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyEmailAccountSuccess);
	/**
	 * @brief Verify user email address.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param UserId UserId from User::CreateEmailAccount().
	 * @param VerificationCode The verification code that was sent to email address.
	 * @param OnSuccess This will be called when the operation succeeded. 
	 * @param OnError This will be called when the operation failed.
	 */
	static void VerifyEmailAccount(FString ServerBaseUrl, FString UserId, FString VerificationCode, FVerifyEmailAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FRequestPasswordResetSuccess);
	/**
	 * @brief Request a password reset code to be used for VerifyPasswordReset().
	 * 
	 * @param ServerBaseUrl Your server base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param Namespace Namespace.
	 * @param LoginId User email address.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	static void RequestPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FRequestPasswordResetSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyPasswordResetSuccess);
	/**
	 * @brief Reset password with the code from RequestPasswordReset().
	 * 
	 * @param ServerBaseUrl Your server base URL.
	 * @param ClientId Client credentials.
	 * @param VerificationCode The code from User::RequestPasswordReset()
	 * @param LoginId User email address.
	 * @param NewPassword The new password.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	static void VerifyPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FString VerificationCode, FString NewPassword, FVerifyPasswordResetSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginFromLauncherSuccess);
	/**
	 * @brief DOES NOT WORK AT THE MOMENT. DO NOT USE. This function should be called automatically if the game started by AccelByte's launcher.
	 *
	 * @param ServerBaseUrl
	 * @param OnSuccess Called when the operation succeeded.
	 */
	static void LoginFromLauncher(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetProfileSuccess, const FAccelByteModelsUserProfileInfo&);
	/**
	 * @brief Get user's profile information.
	 *
	 * @param ServerBaseUrl The server base URL.
	 * @param OnSuccess Called when the operation succeeded. The result is FUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetProfile(FString ServerBaseUrl, FGetProfileSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FUpdateProfileSuccess);
	/**
	 * @brief Update user's profile information.
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param UpdateProfile This is the new profile information that'll be updated.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FUserProfileInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UpdateProfile(FString ServerBaseUrl, const FAccelByteModelsUserProfileInfoUpdate& UpdateProfile, FUpdateProfileSuccess OnSuccess, ErrorDelegate OnError);

private:
	User() = delete; // static class can't have instance
	User(User const&) = delete;
	User(User&&) = delete;

	DECLARE_DELEGATE(FInitializeProfileSuccess);
	/**
	 * @brief Create default user profile when user doesn't have one yet.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void InitializeProfile(FString ServerBaseUrl, FInitializeProfileSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Api
} // Namespace AccelByte
