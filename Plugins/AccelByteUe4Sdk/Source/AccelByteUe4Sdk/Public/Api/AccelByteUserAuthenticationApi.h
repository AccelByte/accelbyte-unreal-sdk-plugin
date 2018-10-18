// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteOauth2Api.h"



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
#pragma once

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
	
	DECLARE_DELEGATE(FLoginWithClientCredentialsSuccess);
	/**
	 * @brief This is to get access token from `client_credentials` grant, then store the access token in memory.
	 * You shouldn't use this if your application is a client. It is intended for machine-to-machine (server).
	 * It is "required" for user management (create new user, reset password, etc).
	 * @todo IAM, PLEASE remove this requirement.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void LoginWithClientCredentials(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FLoginWithClientCredentialsSuccess OnSuccess, ErrorDelegate OnError);
    static void LoginWithClientCredentialsEasy(FLoginWithClientCredentialsSuccess OnSuccess, ErrorDelegate OnError);
	
	DECLARE_DELEGATE(FLoginWithUsernameAndPasswordSuccess);
	/**
	 * @brief Log in with email account.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param Username User email address.
	 * @param Password Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LoginWithUsernameAndPassword(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Username, FString Password, FLoginWithUsernameAndPasswordSuccess OnSuccess, ErrorDelegate OnError);
	static void LoginWithUsernameAndPasswordEasy(FString Username, FString Password, FLoginWithUsernameAndPasswordSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginWithOtherPlatformAccountSuccess);
	/**
	 * @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
	 * 
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param PlatformId Specify platform type that chosen by user to log in.
	 * @param PlatformToken Authentication code that provided by another platform.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
    static void LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, std::underlying_type<EPlatformType>::type PlatformId, FString PlatformToken, FLoginWithOtherPlatformAccountSuccess OnSuccess, ErrorDelegate OnError);
    static void LoginWithOtherPlatformAccountEasy(std::underlying_type<EPlatformType>::type PlatformId, FString PlatformToken, FLoginWithOtherPlatformAccountSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE(FLoginWithDeviceIdSuccess);
	/**
	 * @brief Log in with device ID (anonymous log in).
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param ClientId Client credentials.
	 * @param ClientSecret Client credentials.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FLoginWithDeviceIdSuccess OnSuccess, ErrorDelegate OnError);
	static void LoginWithDeviceIdEasy(FLoginWithDeviceIdSuccess OnSuccess, ErrorDelegate OnError);

	/**
	 * @brief Remove access tokens, user ID, and other credentials from memory.
	 */
	static void ForgetAllCredentials();
};

} // Namespace Api
} // Namespace AccelByte
