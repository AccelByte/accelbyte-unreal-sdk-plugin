// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Models/OAuthTokenJustice.h"
#include "Models/UserCreateResponse.h"
#include "Models/UserCreateRequest.h"
#include "Models/UserProfileInfo.h"
#include "Models/ResetPasswordRequest.h"
#include "Models/LinkedPlatform.h"
#include "Models/TelemetryEvent.h"
#include "Misc/ConfigCacheIni.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/JusticeLog.h"
#include "Utilities/HTTPJustice.h"
#include "RetryTaskManagerJustice.h"
#include "JusticeSDK.h"
#include "Services/JusticeIdentity.h"
#include "Services/JusticeCatalog.h"
#include "Services/JusticePurchase.h"
#include "Services/JusticePlatform.h"

enum class EPlatformType
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
 * @brief Justice User
 * User identity access management and profile information are handled by this class.
 */
class JUSTICESDK_API JusticeUser
{
public:
	/**
	* @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
	* 
	* @param PlatformType Required. Specify platform type that chosen by user to log in.
	* @param Token Required. Authentication code that provided by another platform.
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
    static void Login(EPlatformType PlatformType, FString Token, FDefaultCompleteDelegate OnComplete);
	static void Login(FString Email, FString Password, FDefaultCompleteDelegate OnComplete);
	static void Logout(FDefaultCompleteDelegate OnComplete);

	/**
	* @brief Log in as a guest (anonymous log in).
	*
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
	static void LoginWithDeviceId(FDefaultCompleteDelegate OnComplete);

	/**
	* @brief User should log in with headless account to upgrade it with email.
	*
	* @param Email Required. User's valid email.
	* @param Password Required. User's password.
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
	static void UpgradeHeadlessAccount(FString Email, FString Password, FDefaultCompleteDelegate OnComplete);
	static void Register(FString Email, FString Password, FString DisplayName, FDefaultCompleteDelegate OnComplete);
	static void Verify(FString VerificationCode, FDefaultCompleteDelegate OnComplete);
	static void ForgotPassword(FString Email, FDefaultCompleteDelegate OnComplete);
	static void ResetPassword(FString VerificationCode, FString Email, FString NewPassword, FDefaultCompleteDelegate OnComplete);
	
	/**
	* @brief This function should be called automatically if the game started by AccelByte's launcher.
	*
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	*/
	static void LoginFromLauncher(FDefaultCompleteDelegate OnComplete);

	/**
	* @brief Get user's profile information.
	*
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is FUserProfileInfo, set in FRequestCurrentPlayerProfileCompleteDelegate callback.
	*/
	static void GetProfile(FRequestCurrentPlayerProfileCompleteDelegate OnComplete);

	/**
	* @brief Update user's profile information.
	*
	* @param UpdateProfile Required. This is the new profile information that'll be updated.
	* @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is FUserProfileInfo, set in FRequestCurrentPlayerProfileCompleteDelegate callback.
	*/
	static void UpdateProfile(UserProfileInfoUpdate UpdateProfile, FDefaultCompleteDelegate OnComplete);

private:
	static void InitProfile(FOAuthTokenJustice OAuthToken, FDefaultCompleteDelegate OnComplete);
	static void OnLogoutResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
};
