// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteUtilities.h"
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
	 * @brief Get access token using authorization code from AccelByte Launcher.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded.
	 *			Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithAuthorizationCodeV3(const FString& ClientId
	 *		, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri
	 *		, THandler<FOauth2Token>& OnSuccess, const FOAuthErrorHandler& OnError, const FString& IamUrl)
	 */
	static void GetTokenWithAuthorizationCode(const FString& ClientId
		, const FString& ClientSecret
		, const FString& AuthorizationCode
		, const FString& RedirectUri
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Get access token using authorization code from AccelByte Launcher.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded.
	 *			Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithAuthorizationCodeV3(const FString& ClientId
	 *		, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri
	 *		, THandler<FOauth2Token>& OnSuccess, const FOAuthErrorHandler& OnError, const FString& IamUrl)
	 */
	static void GetTokenWithAuthorizationCode(const FString& ClientId
		, const FString& ClientSecret
		, const FString& AuthorizationCode
		, const FString& RedirectUri
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token from the user using a registered Email account.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithPasswordCredentialsV3(const FString& ClientId
	 *		, const FString& ClientSecret, const FString& Username, const FString& Password, THandler<FOauth2Token>& OnSuccess
	 *		, const FOAuthErrorHandler& OnError, const FString& IamUrl)
	 */
	static void GetTokenWithPasswordCredentials(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Username
		, const FString& Password
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token from the user using a registered Email Account with 2FA enabled.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithPasswordCredentialsV3(const FString& ClientId
	 *		, const FString& ClientSecret, const FString& Username, const FString& Password, THandler<FOauth2Token>& OnSuccess
	 *		, const FOAuthErrorHandler& OnError, const FString& IamUrl)
	 */
	static void GetTokenWithPasswordCredentials(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Username
		, const FString& Password
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token from specified OAuth/IAM client.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithClientCredentials(const FString& ClientId
		, const FString& ClientSecret
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token using the user's device information and its unique Id.
	 *		This will automatically obtain user's device information using a utilities function.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithDeviceId(const FString& ClientId
		, const FString& ClientSecret
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	* @brief Get access token using the user's device information and its unique Id.
	 *		This will automatically obtain user's device information using a utilities function.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithDeviceId(const FString& ClientId
		, const FString& ClientSecret
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Get access token from the user with their native platform account,
	 *		e.g., Steam, Google, Facebook, Twitter, Twitch, etc.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param PlatformId The PlatformId. The platform ID.
	 * @param PlatformToken The Token.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 *
	 * @note When 2FA is enabled please use the method with FOAuthErrorHandler to get more details about the OAuth errors.
	 */
	static void GetTokenWithOtherPlatformToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& PlatformId
		, const FString& PlatformToken
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token from the user with their native platform account,
	 *		e.g., Steam, Google, Facebook, Twitter, Twitch, etc. with 2FA enabled
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param PlatformId The PlatformId. The platform ID.
	 * @param PlatformToken The Token.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param bCreateHeadless (optional) A boolean flag to specify new account creation if needed, default value is true 
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithOtherPlatformToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& PlatformId
		, const FString& PlatformToken
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, bool bCreateHeadless = true
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Get access token using specified refresh token as long as the refresh token is still valid.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param RefreshToken The issued refresh token.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithRefreshToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& RefreshToken
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Get access token using specified refresh token as long as the refresh token is still valid with 2FA enabled.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param RefreshToken The issued refresh token.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithRefreshToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& RefreshToken
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
     * @brief Revoke specified access token to make it invalid.
     *
     * @param AccessToken Specified Access Token.
     * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 *
	 * @deprecated This method is using wrong auth type, so please use RevokeToken(const FString& ClientId
	 *		, const FString& ClientSecret, const FString& AccessToken, const FVoidHandler& OnSuccess
	 *		, const FOAuthErrorHandler& OnError, const FString& IamUrl)
     */
    static void RevokeToken(const FString& AccessToken
    	, const FVoidHandler& OnSuccess
    	, const FErrorHandler& OnError
    	, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Revoke specified access token to make it invalid.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AccessToken user Access Token or rather, Session Id.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void RevokeToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& AccessToken
		, const FVoidHandler& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Revoke specified access token to make it invalid.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AccessToken user Access Token or rather, Session Id.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void RevokeToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& AccessToken
		, const FVoidHandler& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Get access token using authorization code from AccelByte Launcher.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded. Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 *
	 * @note When 2FA is enabled please use the method with FOAuthErrorHandler to get more details about the OAuth errors.
	 */
	static void GetTokenWithAuthorizationCodeV3(const FString& ClientId
		, const FString& ClientSecret
		, const FString& AuthorizationCode
		, const FString& RedirectUri
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Get access token using authorization code from AccelByte Launcher with 2FA enabled.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param AuthorizationCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectUri The URL the IAM server will redirect you to when the operation succeeded. Again, this doesn't work at all. Do not use this function!!!
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithAuthorizationCodeV3(const FString& ClientId
		, const FString& ClientSecret
		, const FString& AuthorizationCode
		, const FString& RedirectUri
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token from the user using a registered Email account.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param bRememberMe This will use for refresh token expiration extension, default value is false.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 *			
	 * @note When 2FA is enabled please use the method with FOAuthErrorHandler to get more details about the OAuth errors.
	 */
	static void GetTokenWithPasswordCredentialsV3(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Username
		, const FString& Password
		, const THandler<FOauth2Token>& OnSuccess
		, const FErrorHandler& OnError
		, bool bRememberMe = false
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Get access token from the user using a registered Email account with 2FA enabled.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address.
	 * @param Password The password.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param bRememberMe This will use for refresh token expiration extension, default value is false.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GetTokenWithPasswordCredentialsV3(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Username
		, const FString& Password
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, bool bRememberMe = false
		, const FString& IamUrl = TEXT(""));
	
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
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void VerifyAndRememberNewDevice(const FString& ClientId
		, const FString& ClientSecret
		, const FString& MfaToken
		, EAccelByteLoginAuthFactorType AuthFactorType
		, const FString& Code 
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, bool bRememberDevice = false
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Log user in with create headless account after 3rd platform authenticated
	 * Will return a "user" session id.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param LinkingToken Platform Linking Token.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void CreateHeadlessAccountAndResponseToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& LinkingToken
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Log user in with authenticate a user account and perform platform link. It validates user's email / username and password.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Username The email address.
	 * @param Password The password.
	 * @param LinkingToken Platform Linking Token.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void AuthenticationWithPlatformLink(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Username
		, const FString& Password
		, const FString& LinkingToken
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Verify OAuth2 token verification API.
	 * 
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Token Token to be verified.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void VerifyToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Token
		, const FVoidHandler& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

	/**
	 * @brief Generate one time linking code. 
	 * 
	 * @param AccessToken user Access Token or rather, Session Id.
	 * @param PlatformId The PlatformId. The platform ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GenerateOneTimeCode(const FString& AccessToken
		, const FString& PlatformId
		, const THandler<FGeneratedOneTimeCode>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));
	
	/**
	 * @brief Generate publisher user's game token. Required a code from request game token. 
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The IAM service URL used to call the API.
	 * @param Code code from request game token.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 */
	static void GenerateGameToken(const FString& ClientId
		, const FString& ClientSecret
		, const FString& Code
		, const THandler<FOauth2Token>& OnSuccess
		, const FOAuthErrorHandler& OnError
		, const FString& IamUrl = TEXT(""));

private:
	Oauth2() = delete; // static class can't have instance
	Oauth2(Oauth2 const&) = delete;
	Oauth2(Oauth2&&) = delete;

	static FHttpRequestPtr ConstructTokenRequest(const FString& Url
		, const FString& ClientId
		, const FString& ClientSecret);
};

} // Namespace Api
} // Namespace AccelByte