// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteOauth2Models.h"
#include "AccelByteUserProfileModels.h"
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
 * @brief The API for getting OAuth2 access token. You should not use directly; use UserAuthentication instead.
 */
class ACCELBYTEUE4SDK_API Oauth2
{
public:
#if 0
	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithAuthorizationCodeGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	 * @brief WARNING: THIS DOESN'T ACTUALLY WORK!!! DO NOT USE!!! This function *was* meant for logging in from Justice Launcher with their account on Justice Launcher.
	 * Also missing state parameter. Do not use this!!! You (user) will be susceptible to Cross-Site Request Forgery attack!!!
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded. Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithAuthorizationCodeGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString AuthorizationCode, FString RedirectUri, FGetAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, ErrorDelegate OnError);
#endif

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithPasswordGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	 * @brief Log user in with their email account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address. Dunno why it's called Login ID instead of username or something.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithPasswordGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Username, FString Password, FGetAccessTokenWithPasswordGrantSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithClientCredentialsGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	* @brief Get client token.
	* he result is FAccelByteModelsOauth2Token.
	*
	* @param ServerBaseUrl Your server's base URL.
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithClientCredentialsGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithRefreshTokenGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	* @brief Get new access token with refresh token.
	*
	* @param ServerBaseUrl Your server's base URL.
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param RefreshToken Refresh token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithRefreshTokenGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RefreshToken, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, ErrorDelegate OnError);

	//
	// Custom grant types
	// These shouldn't be here but, eh.
	//

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithDeviceGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	 * @brief This function allows user to login without entering any information.
	 * This will automatically obtain user's device information using UE4 function.
	 * Will return a "user" access token.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithDeviceGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, ErrorDelegate OnError);
	
	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithPlatformGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	* @brief Log user in with their other platform account, e.g., Steam, Google, Facebook, Twitter, Twitch, etc.
	* Will return a "user" access token.
	* 
	* @param ServerBaseUrl Your server's base URL.
	* @param PlatformId The PlatformId. The platform ID.
	* @param PlatformToken The Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithPlatformGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString PlatformId, FString PlatformToken, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, ErrorDelegate OnError);

private:
	Oauth2() = delete; // static class can't have instance
	Oauth2(Oauth2 const&) = delete;
	Oauth2(Oauth2&&) = delete;

#if 0
	static void GetAccessTokenWithAuthorizationCodeGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, ErrorDelegate OnError);
#endif
	static void GetAccessTokenWithPasswordGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithPasswordGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetAccessTokenWithClientCredentialsGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetAccessTokenWithRefreshTokenGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, ErrorDelegate OnError);
	static void GetAccessTokenWithDeviceGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, ErrorDelegate OnError);
    static void GetAccessTokenWithPlatformGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Api
} // Namespace AccelByte