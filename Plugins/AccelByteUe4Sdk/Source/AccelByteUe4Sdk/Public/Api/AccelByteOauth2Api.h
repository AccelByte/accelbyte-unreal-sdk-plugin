// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteOauth2Models.h"
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
	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithAuthorizationCodeGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	 * @brief WARNING: THIS DOESN'T ACTUALLY WORK!!! DO NOT USE!!! This function *was* meant for logging in from Justice Launcher with their account on Justice Launcher.
	 * Also missing state parameter. You (user) will be susceptible to Cross-Site Request Forgery attack.
	 * @todo IAM fix.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded. Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithAuthorizationCodeGrant(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const FGetAccessTokenWithAuthorizationCodeGrantSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithPasswordGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	 * @brief Log user in with their email account.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address. Dunno why it's called Login ID instead of username or something.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithPasswordGrant(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const FGetAccessTokenWithPasswordGrantSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithClientCredentialsGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	* @brief Get client token.
	* he result is FAccelByteModelsOauth2Token.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithClientCredentialsGrant(const FString& ClientId, const FString& ClientSecret, const FGetAccessTokenWithClientCredentialsGrantSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithRefreshTokenGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	* @brief Get new access token with refresh token.
	*
	* @param ClientId The issued OAuth2 client credentials.
	* @param ClientSecret The issued OAuth2 client credentials.
	* @param RefreshToken Refresh token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithRefreshTokenGrant(const FString& ClientId, const FString& ClientSecret, const FString& RefreshToken, const FGetAccessTokenWithRefreshTokenGrantSuccess& OnSuccess, const FErrorHandler& OnError);

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
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAccessTokenWithDeviceGrant(const FString& ClientId, const FString& ClientSecret, const FGetAccessTokenWithDeviceGrantSuccess& OnSuccess, const FErrorHandler& OnError);
	
	DECLARE_DELEGATE_OneParam(FGetAccessTokenWithPlatformGrantSuccess, const FAccelByteModelsOauth2Token&);
	/**
	* @brief Log user in with their other platform account, e.g., Steam, Google, Facebook, Twitter, Twitch, etc.
	* Will return a "user" access token.
	* 
	* @param PlatformId The PlatformId. The platform ID.
	* @param PlatformToken The Token.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Token.
	* @param OnError This will be called when the operation failed.
	*/
	static void GetAccessTokenWithPlatformGrant(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const FGetAccessTokenWithPlatformGrantSuccess& OnSuccess, const FErrorHandler& OnError);

private:
	Oauth2() = delete; // static class can't have instance
	Oauth2(Oauth2 const&) = delete;
	Oauth2(Oauth2&&) = delete;

	static void GetAccessTokenWithAuthorizationCodeGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, FErrorHandler OnError);
	static void GetAccessTokenWithPasswordGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithPasswordGrantSuccess OnSuccess, FErrorHandler OnError);
	static void GetAccessTokenWithClientCredentialsGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, FErrorHandler OnError);
	static void GetAccessTokenWithRefreshTokenGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, FErrorHandler OnError);
	static void GetAccessTokenWithDeviceGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, FErrorHandler OnError);
    static void GetAccessTokenWithPlatformGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, FErrorHandler OnError);
};

} // Namespace Api
} // Namespace AccelByte