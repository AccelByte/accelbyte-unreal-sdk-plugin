// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteServicesIdentity.h"
#include "AccelByteServicesUserProfile.h"
#include "AccelByteModelsUserProfile.h"

enum class EPlatformType : int32
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


/**
 * @brief AccelByte User
 * A convenience class for using Identity (Identity Access Management) and UserProfile services.
 * Stores tokens after successful login.
 */
class ACCELBYTEUE4SDK_API User
{
public:

	DECLARE_DELEGATE(FLoginWithPlatformAccountSuccess)
	/**
	* @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
	* 
	* @param PlatformType Required. Specify platform type that chosen by user to log in.
	* @param Token Required. Authentication code that provided by another platform.
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
    static void LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, EPlatformType PlatformId, FString Token, FLoginWithPlatformAccountSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginWithEmailSuccess)
	/**
	 * @brief Log in with email account.
	 * @param ServerBaseUrl 
	 * @param ClientId 
	 * @param ClientSecret 
	 * @param Namespace 
	 * @param Email 
	 * @param Password 
	 * @param OnSuccess 
	 * @param OnError 
	 */
	static void LoginWithEmailAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FString Password, FLoginWithEmailSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	static void ResetCredentials();

	DECLARE_DELEGATE(FLoginWithDeviceIdSuccess)
	/**
	* @brief Log in as a guest (anonymous log in).
	*
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
	static void LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FLoginWithDeviceIdSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	/**
	* @brief User should log in with headless account to upgrade it with email.
	*
	* @param Email Required. User's valid email.
	* @param Password Required. User's password.
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
	DECLARE_DELEGATE(FUpgradeHeadlessAccountSuccess)
	static void UpgradeHeadlessAccount(FString ServerBaseUrl, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FRegisterEmailAccountSuccess, FAccelByteModelsUserCreateResponse)
	static void CreateEmailAccount(FString ServerBaseUrl, FString Email, FString Password, FString DisplayName, AccelByte::Services::Identity::FCreateUserAccountSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyRegistrationSuccess)
	static void VerifyEmailAccount(FString ServerBaseUrl, FString UserId, FString VerificationCode, FVerifyRegistrationSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE(FRequestResetPasswordSuccess)
	/**
	 * @brief Request a password reset code to be used for ResetPasswordWithCode().
	 * \param ServerBaseUrl Your server base URL.
	 * \param Email User email address.
	 * \param OnSuccess Called when the operation succeeded.
	 * \param OnError Called when the operation failed.
	 */
	static void RequestPasswordResetCode(FString ServerBaseUrl, FString Email, FRequestResetPasswordSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyResetPasswordSuccess)
	/**
	 * \brief Reset password with the code from RequestPasswordResetCode().
	 * \param ServerBaseUrl Your server base URL.
	 * \param VerificationCode The code from RequestPasswordResetCode()
	 * \param Email User email address.
	 * \param NewPassword The new password.
	 * \param OnSuccess Called when the operation succeeded.
	 * \param OnError Called when the operation failed.
	 */
	static void ResetPasswordWithCode(FString ServerBaseUrl, FString VerificationCode, FString Email, FString NewPassword, FVerifyResetPasswordSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginFromLauncherSuccess)
	/**
	* @brief This function should be called automatically if the game started by AccelByte's launcher.
	*
	 * @param OnSuccess Called when the operation succeeded.
	*/
	static void LoginFromLauncher(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetProfileSuccess, const FAccelByteModelsUserProfileInfo&)
	/**
	* @brief Get user's profile information.
	*
	* @param OnComplete Called when the operation succeeded.. The result is FUserProfileInfo.
	*/
	static void GetProfile(FString ServerBaseUrl, FGetProfileSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

	DECLARE_DELEGATE(FUpdateProfileSuccess)
	/**
	* @brief Update user's profile information.
	*
	* @param UpdateProfile Required. This is the new profile information that'll be updated.
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is FUserProfileInfo, set in FRequestCurrentPlayerProfileCompleteDelegate callback.
	*/
	static void UpdateProfile(FString ServerBaseUrl, FAccelByteModelsUserProfileInfoUpdate UpdateProfile, FUpdateProfileSuccess OnSuccess, AccelByte::ErrorDelegate OnError);

private:
	DECLARE_DELEGATE(FInitializeProfileSuccess)
	/**
	 * @brief Create default user profile when user doesn't have one yet.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param OnSuccess Called when the operation succeeded.
	 * @param OnError Called when the operation failed.
	 */
	static void InitializeProfile(FString ServerBaseUrl, FInitializeProfileSuccess OnSuccess, AccelByte::ErrorDelegate OnError);
};
