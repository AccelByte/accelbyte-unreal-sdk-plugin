// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Models/AccelByteOauth2Models.h"
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
	Oauth2(FHttpRetrySchedulerBase& InHttpRef);
	Oauth2(FHttpRetrySchedulerBase& InHttpRef, FString const& InIamServerUrl);
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithAuthorizationCodeV3(FString const& ClientId
	 *		, FString const& ClientSecret, FString const& AuthorizationCode, FString const& RedirectUri
	 *		, THandler<FOauth2Token>& OnSuccess, FOAuthErrorHandler const& OnError, FString const& IamUrl)
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithAuthorizationCode(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AuthorizationCode
		, FString const& RedirectUri
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithAuthorizationCodeV3(FString const& ClientId
	 *		, FString const& ClientSecret, FString const& AuthorizationCode, FString const& RedirectUri
	 *		, THandler<FOauth2Token>& OnSuccess, FOAuthErrorHandler const& OnError, FString const& IamUrl)
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithAuthorizationCode(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AuthorizationCode
		, FString const& RedirectUri
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithPasswordCredentialsV3(FString const& ClientId
	 *		, FString const& ClientSecret, FString const& Username, FString const& Password, THandler<FOauth2Token>& OnSuccess
	 *		, FOAuthErrorHandler const& OnError, FString const& IamUrl)
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithPasswordCredentials(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @deprecated This method will be removed in the future, so please use GetTokenWithPasswordCredentialsV3(FString const& ClientId
	 *		, FString const& ClientSecret, FString const& Username, FString const& Password, THandler<FOauth2Token>& OnSuccess
	 *		, FOAuthErrorHandler const& OnError, FString const& IamUrl)
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithPasswordCredentials(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

	/**
	 * @brief Get access token from specified OAuth/IAM client.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithClientCredentials(FString const& ClientId
		, FString const& ClientSecret
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithDeviceId(FString const& ClientId
		, FString const& ClientSecret
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, bool bCreateHeadless = true
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithDeviceId(FString const& ClientId
		, FString const& ClientSecret
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, bool bCreateHeadless = true
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * @param OptionalParams Will be passed to the backend.
	 *
	 * @note When 2FA is enabled please use the method with FOAuthErrorHandler to get more details about the OAuth errors.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithOtherPlatformToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& PlatformId
		, FString const& PlatformToken
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams = {}
		, FString const& EncodedMacAddress = TEXT("")
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * @param OptionalParams Will be passed to the backend.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithOtherPlatformToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& PlatformId
		, FString const& PlatformToken
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, bool bCreateHeadless = true
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams = {}
		, FString const& EncodedMacAddress = TEXT("")
		, FString const& DeviceId = TEXT(""));
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithRefreshToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& RefreshToken
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithRefreshToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& RefreshToken
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

	/**
	 * @brief Refresh the platform token that is stored in the IAM backend.
	 * Therefore we can prevent expiration on the backend.
	 * This endpoint also not generate any event or AB Access/Refresh Token.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param PlatformID The targeted platform to be refreshed.
	 * @param PlatformToken The platform token that will used to refresh IAM storage.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr RefreshPlatformToken(FString const& ClientID
		, FString const& ClientSecret
		, FString const& PlatformID
		, FString const& PlatformToken
		, THandler<FPlatformTokenRefreshResponse> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

	/**
     * @brief Revoke specified access token to make it invalid.
     *
     * @param AccessToken Specified Access Token.
     * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 *
	 * @deprecated This method is using wrong auth type, so please use RevokeToken(FString const& ClientId
	 *		, FString const& ClientSecret, FString const& AccessToken, FVoidHandler const& OnSuccess
	 *		, FOAuthErrorHandler const& OnError, FString const& IamUrl)
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
     FAccelByteTaskWPtr RevokeToken(FString const& AccessToken
    	, FVoidHandler const& OnSuccess
    	, FErrorHandler const& OnError
    	, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr RevokeToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AccessToken
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr RevokeToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AccessToken
		, FVoidHandler const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 *
	 * @note When 2FA is enabled please use the method with FOAuthErrorHandler to get more details about the OAuth errors.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithAuthorizationCodeV3(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AuthorizationCode
		, FString const& RedirectUri
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithAuthorizationCodeV3(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AuthorizationCode
		, FString const& RedirectUri
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 *			
	 * @note When 2FA is enabled please use the method with FOAuthErrorHandler to get more details about the OAuth errors.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithPasswordCredentialsV3(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, THandler<FOauth2Token> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bRememberMe = false
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithPasswordCredentialsV3(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, bool bRememberMe = false
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

	/**
	 * @brief Login with native platform and secondary platform. Currently support Windows only.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param NativePlatformName From the native subsystem
	 * @param NativePlatformToken The auth ticket from native identity interface
	 * @param SecondaryPlatformName From the secondary platform subsystem
	 * @param SecondaryPlatformToken The auth ticket from secondary platform interface
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithSimultaneousPlatformToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& NativePlatformName
		, FString const& NativePlatformToken
		, FString const& SecondaryPlatformName
		, FString const& SecondaryPlatformToken
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr VerifyAndRememberNewDevice(FString const& ClientId
		, FString const& ClientSecret
		, FString const& MfaToken
		, EAccelByteLoginAuthFactorType AuthFactorType
		, FString const& Code 
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, bool bRememberDevice = false
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr CreateHeadlessAccountAndResponseToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& LinkingToken
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
	/**
	 * @brief Log user in with authenticate a user account and perform platform link. It validates user's email and password.
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr AuthenticationWithPlatformLink(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, FString const& LinkingToken
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr VerifyToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Token
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

	/**
	 * @brief Generate one time linking code. 
	 * 
	 * @param AccessToken user Access Token or rather, Session Id.
	 * @param PlatformId The PlatformId. The platform ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GenerateOneTimeCode(FString const& AccessToken
		, FString const& PlatformId
		, THandler<FGeneratedOneTimeCode> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});
	
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GenerateGameToken(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Code
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

	/**
	 * @brief This function generate a code that can be exchanged into publisher namespace token (i.e. by web portal)
	 *
	 * @param AccessToken Player's access token.
	 * @param PublisherNamespace The targeted game's publisher Namespace.
	 * @param PublisherClientID The targeted game's publisher ClientID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GenerateCodeForPublisherTokenExchange(FString const& AccessToken
		, FString const& PublisherNamespace
		, FString const& PublisherClientID
		, THandler<FCodeForTokenExchangeResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

	/**
	 * @brief This function is used for retrieving third party platform token for user that login using third party,
	 * if user have not link requested platform in game namespace, will try to retrieving third party platform token from publisher namespace.
	 * Passing platform group name or it's member will return same access token that can be used across the platform members.
	 * Note: The third party platform and platform group covered for this is:
	 *    (psn) ps4web, (psn) ps4, (psn) ps5, epicgames, twitch, awscognito.
	 * @param Namespace User's namespace
	 * @param UserId UserId.
	 * @param PlatformId Platform type value.
	 * @param Authorization Authorization.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FThirdPartyPlatformTokenData.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr RetrieveUserThirdPartyPlatformToken(FString const& Namespace
	 	, FString const& UserId
		, FString const& PlatformId
		, FString const& Authorization
		, THandler<FThirdPartyPlatformTokenData> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT(""));

#pragma region OAuthV4
	/**
	 * @brief Get access token from the user using a registered Email account with 2FA enabled and queue feature.
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithPasswordCredentialsV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, bool bRememberMe = false
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithDeviceIdV4(FString const& ClientId
		, FString const& ClientSecret
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, bool bCreateHeadless = true
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithAuthorizationCodeV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& AuthorizationCode
		, FString const& RedirectUri
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * @param OptionalParams Will be passed to the backend.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithOtherPlatformTokenV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& PlatformId
		, FString const& PlatformToken
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, bool bCreateHeadless = true
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams = {}
		, FString const& DeviceId = TEXT("")
		, FString const& EncodedMacAddress = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr VerifyAndRememberNewDeviceV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& MfaToken
		, EAccelByteLoginAuthFactorType AuthFactorType
		, FString const& Code
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, bool bRememberDevice = false
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GenerateGameTokenV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Code
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

	/**
	 * @brief Log user in with authenticate a user account and perform platform link. It validates user's email and password.
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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr AuthenticationWithPlatformLinkV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& Username
		, FString const& Password
		, FString const& LinkingToken
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr CreateHeadlessAccountAndResponseTokenV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& LinkingToken
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

	/**
	 * @brief Login with native platform and secondary platform. Currently support Windows only.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param NativePlatformName From the native subsystem
	 * @param NativePlatformToken The auth ticket from native identity interface
	 * @param SecondaryPlatformName From the secondary platform subsystem
	 * @param SecondaryPlatformToken The auth ticket from secondary platform interface
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOauth2Session.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithSimultaneousPlatformTokenV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& NativePlatformName
		, FString const& NativePlatformToken
		, FString const& SecondaryPlatformName
		, FString const& SecondaryPlatformToken
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));

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
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithRefreshTokenV4(FString const& ClientId
		, FString const& ClientSecret
		, FString const& RefreshToken
		, THandler<FOauth2TokenV4> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {});

	/**
	 * @brief Get access token from exchange login ticket.
	 *
	 * @param ClientId The issued OAuth2 client credentials.
	 * @param ClientSecret The issued OAuth2 client credentials.
	 * @param Data The Data required to exchange, also contain the login ticket itself.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param IamUrl (optional) The IAM service URL used to call the API, if it's an empty string
	 *			then the value from FRegistry is used instead.
	 * @param AdditionalHeaders (optional) Additional headers that will be added to the HTTP Request.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetTokenWithLoginTicket(FString const& ClientId
		, FString const& ClientSecret
		, FString const& LoginTicket
		, THandler<FOauth2Token> const& OnSuccess
		, FOAuthErrorHandler const& OnError
		, FString const& IamUrl = TEXT("")
		, TMap<FString, FString> AdditionalHeaders = {}
		, FString const& DeviceId = TEXT(""));
#pragma endregion

private:
	Oauth2(Oauth2 const&) = delete;
	Oauth2(Oauth2&&) = delete;

	FHttpRequestPtr ConstructTokenRequest(FString const& Url
	, FString const& ClientId
	, FString const& ClientSecret
	, TMap<FString, FString> const& AdditionalHeaders);

	FString ConstructAdditionalData();
	FHttpRetrySchedulerBase& HttpRef;
	FString IamServerUrl;
};

} // Namespace Api
} // Namespace AccelByte