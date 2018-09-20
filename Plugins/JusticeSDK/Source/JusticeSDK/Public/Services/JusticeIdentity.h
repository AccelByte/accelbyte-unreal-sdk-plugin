// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Models/OAuthTokenJustice.h"
#include "Models/UserCreateResponse.h"
#include "Models/UserCreateRequest.h"
#include "Models/ResetPasswordRequest.h"
#include "Models/LinkedPlatform.h"
#include "Models/TelemetryEvent.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "RetryTaskManagerJustice.h"
#include "JusticeSDK.h"

DECLARE_DELEGATE_ThreeParams(FAuthCodeCompleteDelegate, bool, FString, FString);
DECLARE_DELEGATE_ThreeParams(FUserLoginCompleteDelegate, bool, FString, FOAuthTokenJustice*);
DECLARE_DELEGATE_ThreeParams(FUserLoginCompleteDelegate2, bool, FString, TSharedPtr<FOAuthTokenJustice>);
DECLARE_DELEGATE_ThreeParams(FRegisterPlayerCompleteDelegate, bool, FString, TSharedPtr<FUserCreateResponse>);
DECLARE_DELEGATE_ThreeParams(FGetLinkedPlatformCompleteDelegate, bool, FString, TArray<FLinkedPlatform>);

/**
 * @brief Either Email or Phone.
 */
enum FUserAuthTypeJustice
{
	Email = 0,
	Phone = 1,
};

UENUM(BlueprintType)
enum class UUserAuthTypeJustice : uint8
{
	AT_Email = 0	UMETA(DisplayName = "Email"),
	AT_Phone = 1 	UMETA(DisplayName = "Phone")
};

/**
 * @brief Justice Identity is the first service required before we can proceed to other service such as platform service.
 */
class JUSTICESDK_API JusticeIdentity 
{
public:
	/**
	 * @brief This function will login the user automatically from Justice Launcher with their account on Justice Launcher too.
	 * 
	 * @param AuthCode This should be filled with "JUSTICE_AUTHORIZATION_CODE" environment variable.
	 * @param RedirectURI This is optional.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is FUserLoginCompleteDelegate, set in OAuthTokenJustice callback.
	 */
	static void AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate2 OnComplete);

	/**
	 * @brief Log user in with their email account.
	 * 
	 * @param Namespace Required.
	 * @param LoginID Required.
	 * @param Password Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. 
	 * The result is FUserLoginCompleteDelegate, set in OAuthTokenJustice callback.
	 */
	static void UserLogin(FString Namespace, FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete);

	static void UserLogin(FString Namespace, FString ClientId, FString  ClientSecret, FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete);
	static void UserRefreshToken(FUserLoginCompleteDelegate OnComplete);
	static void UserRefresh();

	/**
	 * @brief Log the user out from the game.
	 * 
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is FUserLoginCompleteDelegate, set in OAuthTokenJustice callback.
	 */
	static void UserLogout(FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function allows user to login without entering any information.
	 * It'll automatically obtain their device's information for their account.
	 * 
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is FUserLoginCompleteDelegate, set in OAuthTokenJustice callback.
	 */
	static void DeviceLogin(FUserLoginCompleteDelegate OnComplete);
	/**
	 * @brief This function will register a new user with email-based account.
	 * 
	 * @param Namespace Required.
	 * @param UserID Required.
	 * @param Password Required.
	 * @param DisplayName Required.
	 * @param AuthType Either FUserAuthTypeJustice::Email or FUserAuthTypeJustice::Phone.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is FRegisterPlayerCompleteDelegate, set in a callback complete information from the user that has been registered, wrapped in UserCreateResponse object.
	 */
	static void RegisterNewPlayer(FString Namespace, FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	/**
	 * @brief This function will verify the registered email **after** user receives verification code sent with ::ReissueVerificationCode() to their email.
	 * 
	 * @param Namespace Required.
	 * @param UserID Required.
	 * @param VerificationCode Required.
	 * @param AuthType Either FUserAuthTypeJustice::Email or FUserAuthTypeJustice::Phone.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is FUserLoginCompleteDelegate, set in OAuthTokenJustice callback.
	 */
	static void VerifyNewPlayer(FString Namespace, FString UserID, FString VerificationCode, FUserAuthTypeJustice AuthType, FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function would be called after registering a new user account.
	 * 
	 * @param Namespace Required.
	 * @param UserID Obtained from token response obtained after user login. 
	 * @param LoginID Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void ReissueVerificationCode(FString Namespace, FString UserID, FString LoginID, FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief Reissue verification code.
	 * 
	 * @TODO Add more description here. What's this for?
	 */
	static void ReissueVerificationCodeUpgradedAccount(FOAuthTokenJustice Token, FString LoginID, FDefaultCompleteDelegate OnComplete);

    /**
	 * @brief Send a request to reset user's password.
	 * 
	 * @param Namespace Required.
	 * @param LoginID User's email address.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void ForgotPassword(FString Namespace, FString LoginID, FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function verifies user's request to reset password ::ForgotPassword.
	 * 
	 * @param Namespace Required.
	 * @param UserID User's email address.
	 * @param VerificationCode Code sent to the email address.
	 * @param NewPassword New user's password.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void ResetPassword(FString Namespace, FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function gets user's platform accounts linked to user’s account.
	 * 
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is TArray<LinkedPlatform> in FGetLinkedPlatformCompleteDelegate.
	 */
	static void GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete);

    /**
     * @brief This function links user's current account to their other account in other platform.
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
	 * The browser will redirect the url to a site with a code in form of parameter URL. 
	 * 
	 * @param Namespace Required.
	 * @param PlatformID Required.
	 * @param Ticket Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void LinkPlatform(FString Namespace, FString PlatformID, FString Ticket, FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function links user's current account to their other account in other platform.
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
	 * The browser will redirect the URL to a site with a code in form of parameter URL.
	 * 
	 * @param Namespace Required.
	 * @param PlatformID Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void UnlinkPlatform(FString Namespace, FString PlatformID, FDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function will upgrade user's headless account.
	 * Headless account is an account that doesn't have an email-password.
	 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
	 * Therefore, the function requests user’s Email and Password for parameters.
	 * 
	 * @param Namespace Required.
	 * @param UserID Required.
	 * @param Email Required.
	 * @param Password Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void UpgradeHeadlessAccount(FString Namespace, FString UserID, FString Email, FString Password, FDefaultCompleteDelegate OnComplete);
	
	static void ClientLogin();
	static void ClientLogout();
	static void ClientRefreshToken();
	static void SetRefreshToken(FString UserRefreshToken);

	static void LoginWithPlatform(FString Namespace, FString PlatformID, FString Token, FUserLoginCompleteDelegate2 OnComplete);
	static void GenerateAuthorizationCode(FOAuthTokenJustice OAuthToken, FString ClientID, FString RedirectURI, FAuthCodeCompleteDelegate OnComplete);

    // Steam Specific
    static void SetSteamAuthTicket(FString ticket);
    static void SetSteamNickName(FString nickname);
    static void SetAvatar(UTexture2D* avatar);
    static FString GetSteamAuthTicket();
    static FString GetSteamNickName();
    static UTexture2D* GetAvatar();

private:

/* Delegates. */
	static void OnDeviceLoginResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserLoginResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate2 OnComplete);
	static void OnUserRefreshResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserLogoutResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnClientLoginResponse(FJusticeResponsePtr Response, bool *IsReady);
	static void OnClientRefreshResponse(FJusticeResponsePtr Response);
	static void OnClientLogoutResponse(FJusticeResponsePtr Response);
	static void OnRegisterNewPlayerResponse(FJusticeResponsePtr Response, FRegisterPlayerCompleteDelegate OnComplete);
	static void OnVerifyNewPlayerResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnReissueVerificationCodeResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnForgotPasswordResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnResetPasswordResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnGetLinkedPlatformResponse(FJusticeResponsePtr Response, FGetLinkedPlatformCompleteDelegate OnComplete);
	static void OnLinkPlatformResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnUnlinkPlatformResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnUpgradeHeadlessAccountResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
    static void OnPlatformLoginResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate2 OnComplete);
	static void OnGenerateAuthorizationCodeResponse(FJusticeResponsePtr Response, FAuthCodeCompleteDelegate OnComplete);

};