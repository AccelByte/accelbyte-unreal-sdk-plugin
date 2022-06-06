// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteError.h"
#include "Http.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;

namespace AccelByte
{

/**
 * @brief Contains interface to C++ functions.
 */
namespace Api
{

/**
 * @brief The API for getting OAuth2 access token. You should not use this directly; use UserAuthentication instead. Where the tokens are stored in memory for convenience.
 */
class ACCELBYTEUE4SDK_API Oauth2
{
public:
	/**
	 * @brief login from Accelbyte Launcher
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded. Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetTokenWithAuthorizationCode(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Log user in with their email account.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address. Dunno why it's called Login ID instead of username or something.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Log user in with their email account with 2FA enabled
	* 
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param Username The email address. Dunno why it's called Login ID instead of username or something.
	* @param Password The password.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError);
	
	/**
	* @brief Get client token.
	* he result is FAccelByteModelsOauth2Token.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetTokenWithClientCredentials(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError);

	//
	// Custom grant types
	// These shouldn't be here but, eh.
	//

	/**
	 * @brief This function allows user to login without entering any information.
	 * This will automatically obtain user's device information using UE4 function.
	 * Will return a "user" session id.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetTokenWithDeviceId(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Log user in with their other platform account, e.g., Steam, Google, Facebook, Twitter, Twitch, etc.
	* Will return a "user" session id.
	* 
	* @param PlatformId The PlatformId. The platform ID.
	* @param PlatformToken The Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Log user in with their other platform account, e.g., Steam, Google, Facebook, Twitter, Twitch, etc. with 2FA enabled
	* Will return a "user" session id.
	* 
	* @param PlatformId The PlatformId. The platform ID.
	* @param PlatformToken The Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	* @param bCreateHeadless This will create new account if needed, default value is true 
	*/
	static void GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken,
		const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bCreateHeadless = true);
	
	/**
	* @brief Refresh user session.
	* he result is FAccelByteModelsOauth2Token.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param RefreshId The issued Refresh Id during get user session.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetTokenWithRefreshToken(const FString& ClientId, const FString& ClientSecret, const FString& RefreshId, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError);
	
	/**
    * @brief Log user out.
    * Will make user session id invalid.
    *
    * @param AccessToken user Access Token or rather, Session Id.
    * @param OnSuccess This will be called when the operation succeeded.
    * @param OnError This will be called when the operation failed.
    */
    static void RevokeToken(const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Log user out.
	* Will make user session id invalid.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param AccessToken user Access Token or rather, Session Id.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	static void RevokeUserToken(const FString& ClientId, const FString& ClientSecret, const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Log user in with their username and password using V3 endpoint.
	* 
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param Username The email address. Dunno why it's called Login ID instead of username or something.
	* @param Password The password.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	* @param bRememberMe This will use for refresh token expiration extension, default value is false.
	*/
	static void GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError, bool bRememberMe = false);

	/**
	* @brief Log user in with their username and password using V3 endpoint with 2FA enabled
	* 
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param Username The email address. Dunno why it's called Login ID instead of username or something.
	* @param Password The password.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	* @param bRememberMe This will use for refresh token expiration extension, default value is false.
	*/
	static void GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberMe = false);
	
	/**
	* @brief Verify and Remember new device when user enabled 2FA.
	* 
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param MfaToken Multi Factor Authentication Code, got error when user login with new device.
	* @param AuthFactorType Type of authentication factor. Could be "EAccelByteLoginAuthFactorType::Authenticator" or "EAccelByteLoginAuthFactorType::BackupCode".
	* @param Code Code to verify new device, could be obtain by 3rd party authenticator app or back up code depend on what type of AuthFactor user set as default
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	* @param bRememberDevice This will use for refresh token expiration extension, default value is false.
	*/
	static void VerifyAndRememberNewDevice(const FString& ClientId, const FString& ClientSecret, const FString& MfaToken, EAccelByteLoginAuthFactorType AuthFactorType, const FString& Code, 
		const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberDevice = false);

	/**
	* @brief Log user in with create headless account after 3rd platform authenticated
	* Will return a "user" session id.
	* 
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param LinkingToken Platform Linking Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	*/
	static void CreateHeadlessAccountAndResponseToken(const FString& ClientId, const FString& ClientSecret, const FString& LinkingToken,
		const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError);
	
	/**
	* @brief Log user in with authenticate a user account and perform platform link. It validates user's email / username and password.
	* Will return a "user" session id.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param LinkingToken Platform Linking Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	* @param OnError This will be called when the operation failed.
	*/
	static void AuthenticationWithPlatformLink(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const FString& LinkingToken,
		const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError);
	
private:
	Oauth2() = delete; // static class can't have instance
	Oauth2(Oauth2 const&) = delete;
	Oauth2(Oauth2&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte