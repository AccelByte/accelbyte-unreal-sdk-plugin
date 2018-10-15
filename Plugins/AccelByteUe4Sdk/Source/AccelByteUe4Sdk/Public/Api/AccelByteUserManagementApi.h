// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUserManagementModels.h"
#include "AccelByteError.h"


namespace AccelByte
{
namespace Api
{

/**
 * @brief User management API for creating user, verifying user, reset password.
 */
class ACCELBYTEUE4SDK_API UserManagement
{
public:

	DECLARE_DELEGATE_OneParam(FCreateUserAccountSuccess, const FAccelByteModelsUserCreateResponse&);
	/**
	 * @brief This function will register a new user with email-based account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The GameId.
	 * @param Username User email address or phone number.
	 * @param Password The Password.
	 * @param DisplayName The DisplayName.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Username, FString Password, FString DisplayName, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError);
	static void CreateUserAccountEasy(FString Username, FString Password, FString DisplayName, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FAddUsernameAndPasswordSuccess);
	/**
	 * @brief This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
	 * Headless account is an account that doesn't have an email and password.
	 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
	 * Therefore, the function requests user’s Username and Password for parameters.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken User's access token.
	 * @param Namespace The GameId.
	 * @param UserId The UserId.
	 * @param Username The Username.
	 * @param Password The Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void AddUsernameAndPassword(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Username, FString Password, FAddUsernameAndPasswordSuccess OnSuccess, ErrorDelegate OnError);
	static void AddUsernameAndPasswordEasy(FString Username, FString Password, FAddUsernameAndPasswordSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FSendUserAccountVerificationCodeSuccess);
	/**
	 * @brief Verify user's email. User should login with email and password first to get access token.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The GameId.
	 * @param UserId Obtained from token response obtained after user login. 
	 * @param Username User email address or phone number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendUserAccountVerificationCode(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Username, FSendUserAccountVerificationCodeSuccess OnSuccess, ErrorDelegate OnError);
	static void SendUserAccountVerificationCodeEasy(FString Username, FSendUserAccountVerificationCodeSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyUserAccountSuccess);
	/**
	 * @brief This function will verify the registered email **after** user receives verification code sent with ::SendUserAccountVerificationCode() to their email.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The GameId.
	 * @param UserId The user ID.
	 * @param VerificationCode The VerificationCode.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void VerifyUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString VerificationCode, FVerifyUserAccountSuccess OnSuccess, ErrorDelegate OnError);
	static void VerifyUserAccountEasy(FString VerificationCode, FVerifyUserAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FSendPasswordResetCodeSuccess);
    /**
	 * @brief Send a request to reset user's password. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * @param ClientSecret Client credentials. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * @param ClientNamespace The ClientNamespace.
	 * @param Username User email address or phone number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendPasswordResetCode(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString ClientNamespace, FString Username, FSendPasswordResetCodeSuccess OnSuccess, ErrorDelegate OnError);
	static void SendPasswordResetCodeEasy(FString Username, FSendPasswordResetCodeSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FResetPasswordSuccess);
	/**
	 * @brief This function verifies user's request to reset password. See Identity::SendPasswordResetCode().
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param Namespace The GameId.
	 * @param Username User email address or phone number.
	 * @param VerificationCode Code sent to the email address.
	 * @param NewPassword The new password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void ResetPassword(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Username, FString VerificationCode, FString NewPassword, FResetPasswordSuccess OnSuccess, ErrorDelegate OnError);
	static void ResetPasswordEasy(FString Username, FString VerificationCode, FString NewPassword, FResetPasswordSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetLinkedUserAccountsSuccess, const TArray<FAccelByteModelsLinkedPlatform>&)
	/**
	 * @brief This function gets user's platform accounts linked to user’s account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Client token. Required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsLinkedPlatform>.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetLinkedUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void GetLinkedUserAccountsEasy(FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLinkUserAccountsSuccess);
    /**
     * @brief This function links user's current account to their other account in other platform.
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
	 * The browser will redirect the URL to a site with a code in form of parameter URL. 
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Client token. Required.
	 * @param Namespace The GameId.
	 * @param PlatformId The PlatformId.
	 * @param Ticket The Ticket.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void LinkUserAccountsEasy(FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FUnlinkUserAccountsSuccess);
	/**
	 * @brief This function links user's current account to their other account in other platform
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
	 * The browser will redirect the URL to a site with a code in form of parameter URL.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Client token. Required.
	 * @param Namespace The GameId.
	 * @param PlatformId The PlatformId.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UnlinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void UnlinkUserAccountsEasy(FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

private:
	UserManagement() = delete; // static class can't have instance
	UserManagement(UserManagement const&) = delete;
	UserManagement(UserManagement&&) = delete;

	static void CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError);
	static void UpgradeHeadlessUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FAddUsernameAndPasswordSuccess OnSuccess, ErrorDelegate OnError);
	static void SendVerificationCodeForUserAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FSendUserAccountVerificationCodeSuccess OnSuccess, ErrorDelegate OnError);
	static void VerifyUserAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FVerifyUserAccountSuccess OnSuccess, ErrorDelegate OnError);
	static void SendVerificationCodeForPasswordResetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FSendPasswordResetCodeSuccess OnSuccess, ErrorDelegate OnError);
	static void VerifyPasswordResetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FResetPasswordSuccess OnSuccess, ErrorDelegate OnError);
	static void GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

};

} // Namespace Api
} // Namespace AccelByte
