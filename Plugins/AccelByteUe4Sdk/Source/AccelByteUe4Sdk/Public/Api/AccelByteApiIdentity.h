// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteModelsIdentity.h"
#include "AccelByteModelsUserProfile.h"
#include "AccelByteError.h"
#include "Http.h"

namespace AccelByte
{

/**
 * @brief Contains interface to C++ functions.
 */
namespace Api
{

/**
 * @brief Either Email or Phone.
 */
enum class EUserAuthType : uint8
{
	Email = 0,
	Phone = 1,
};

/**
 * @brief The API for user authentication and identity provider.
 * There are 2 types of access tokens: user access token and client access token. 
 */
class ACCELBYTEUE4SDK_API Identity
{
public:
#pragma region OAuth


	DECLARE_DELEGATE_OneParam(FGetUserAccessTokenWithAuthorizationCodeGrantSuccess, const FAccelByteModelsOAuthToken&);
	/**
	 * @brief WARNING: THIS DOESN'T ACTUALLY WORK!!! DO NOT USE!!! This function *was* meant for logging in from Justice Launcher with their account on Justice Launcher.
	 * Also missing state parameter. Do not use this!!! You (user) will be susceptible to Cross-Site Request Forgery attack!!!
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded. Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOAuthToken.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithAuthorizationCodeGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString AuthorizationCode, FString RedirectUri, FGetUserAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserAccessTokenWithPasswordGrantSuccess, const FAccelByteModelsOAuthToken &);
	/**
	 * @brief Log user in with their email account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Namespace The namespace.
	 * @param LoginId The email address. Dunno why it's called Login ID instead of username or something.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOAuthToken.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetUserAccessTokenWithPasswordGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FString Password, FGetUserAccessTokenWithPasswordGrantSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithClientCredentialsGrantSuccess, const FAccelByteModelsOAuthToken&);
	/**
	* @brief Get client token.
	* Client token is The is.for creating request from AccelByteServicesIdentity. The result is FAccelByteModelsOAuthToken.
	*
	* @param ServerBaseUrl Your server's base URL.
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithClientCredentialsGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithRefreshTokenGrantSuccess, const FAccelByteModelsOAuthToken&);
	/**
	* @brief Get new access token with refresh token.
	*
	* @param ServerBaseUrl Your server's base URL.
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param RefreshToken Refresh token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOAuthToken.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithRefreshTokenGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RefreshToken, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, ErrorDelegate OnError);

	//
	// Custom grant types
	// I have no idea why these are actually here at all
	//

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithDeviceGrantSuccess, const FAccelByteModelsOAuthToken&);
	/**
	 * @brief This function allows user to login without entering any information.
	 * This will automatically obtain user's device information using UE4 function.
	 * Will return a "user" access token.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Namespace Namespace.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOAuthToken.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithDeviceGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, ErrorDelegate OnError);
	
	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithPlatformGrantSuccess, const FAccelByteModelsOAuthToken&);
	/**
	* @brief Log user in with their other platform account, e.g., Steam, Google, Facebook, Twitter, Twitch, etc.
	* Will return a "user" access token.
	* 
	* @param ServerBaseUrl Your server's base URL.
	* @param Namespace The Namespace.
	* @param PlatformId The PlatformId. The platform ID.
	* @param Token The Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOAuthToken.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithPlatformGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString PlatformId, FString Token, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, ErrorDelegate OnError);

#pragma endregion OAuth

#pragma region Users

	DECLARE_DELEGATE_OneParam(FCreateUserAccountSuccess, const FAccelByteModelsUserCreateResponse&);
	/**
	 * @brief This function will register a new user with email-based account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The Namespace.
	 * @param UserId The UserId.
	 * @param Password The Password.
	 * @param DisplayName The DisplayName.
	 * @param AuthType Either EUserAuthType::Email or EUserAuthType::Phone.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Password, FString DisplayName, EUserAuthType AuthType, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FUpgradeHeadlessAccountSuccess);
	/**
	 * @brief This function will upgrade user's headless account.
	 * Headless account is an account that doesn't have an email-password.
	 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
	 * Therefore, the function requests user’s Email and Password for parameters.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The Namespace.
	 * @param UserId The UserId.
	 * @param Email The Email.
	 * @param Password The Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UpgradeHeadlessUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnError);
	
	DECLARE_DELEGATE(FSendVerificationCodeForUserAccountCreationSuccess);
	/**
	 * @brief This function shall be called after registering a new user account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The Namespace.
	 * @param UserId Obtained from token response obtained after user login. 
	 * @param LoginId The login ID. I don't know why it's called "login ID".
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendVerificationCodeForUserAccountCreation(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString LoginId, FSendVerificationCodeForUserAccountCreationSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyUserAccountCreation);
	/**
	 * @brief This function will verify the registered email **after** user receives verification code sent with Identity::::SendVerificationCodeForUserAccountCreation() to their email.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The Namespace.
	 * @param UserId The user ID.
	 * @param VerificationCode The VerificationCode.
	 * @param AuthType Either EUserAuthType::Email or EUserAuthType::Phone.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void VerifyUserAccountCreation(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString VerificationCode, EUserAuthType AuthType, FVerifyUserAccountCreation OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FSendVerificationCodeForPasswordResetSuccess);
    /**
	 * @brief Send a request to reset user's password. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * @param ClientSecret Client credentials. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * @param Namespace The Namespace.
	 * @param LoginId User's email address. I don't know why it's called "login ID".
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendVerificationCodeForPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FSendVerificationCodeForPasswordResetSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FVerifyResetPasswordSuccess);
	/**
	 * @brief This function verifies user's request to reset password. See Identity::SendVerificationCodeForPasswordReset().
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param Namespace The Namespace.
	 * @param UserId The UserId.
	 * @param VerificationCode Code sent to the email address.
	 * @param NewPassword The new password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void VerifyPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString UserId, FString VerificationCode, FString NewPassword, FVerifyResetPasswordSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetLinkedUserAccountsSuccess, TArray<FAccelByteModelsLinkedPlatform>)
	/**
	 * @brief This function gets user's platform accounts linked to user’s account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Client token. Required.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsLinkedPlatform>.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetLinkedUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLinkUserAccountsSuccess);
    /**
     * @brief This function links user's current account to their other account in other platform.
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
	 * The browser will redirect the URL to a site with a code in form of parameter URL. 
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Client token. Required.
	 * @param Namespace The Namespace.
	 * @param PlatformId The PlatformId.
	 * @param Ticket The Ticket.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FUnlinkUserAccountsSuccess);
	/**
	 * @brief This function links user's current account to their other account in other platform
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
	 * The browser will redirect the URL to a site with a code in form of parameter URL.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Client token. Required.
	 * @param Namespace The Namespace.
	 * @param PlatformId The PlatformId.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UnlinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
#pragma endregion Users

private:
#pragma region OAuth

	static void GetUserAccessTokenWithAuthorizationCodeGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetUserAccessTokenWithPasswordGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserAccessTokenWithPasswordGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetAccessTokenWithClientCredentialsGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetAccessTokenWithRefreshTokenGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetAccessTokenWithDeviceGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, ErrorDelegate OnError);
    static void GetAccessTokenWithPlatformGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, ErrorDelegate OnError);

#pragma endregion OAuth

#pragma region Users

	static void CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError);
	static void UpgradeHeadlessUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnError);
	static void SendVerificationCodeForUserAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FSendVerificationCodeForUserAccountCreationSuccess OnSuccess, ErrorDelegate OnError);
	static void VerifyUserAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FVerifyUserAccountCreation OnSuccess, ErrorDelegate OnError);
	static void SendVerificationCodeForPasswordResetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FSendVerificationCodeForPasswordResetSuccess OnSuccess, ErrorDelegate OnError);
	static void VerifyPasswordResetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FVerifyResetPasswordSuccess OnSuccess, ErrorDelegate OnError);
	static void GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);
	static void UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError);

#pragma endregion Users
};

} // Namespace Api
} // Namespace AccelByte