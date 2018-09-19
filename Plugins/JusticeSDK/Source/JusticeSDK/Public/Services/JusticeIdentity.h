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

class JUSTICESDK_API JusticeIdentity 
{
public:
	static void AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate2 OnComplete);
	static void UserLogin(FString Namespace, FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete);
	static void UserLogin(FString Namespace, FString ClientId, FString  ClientSecret, FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete);
	static void UserRefreshToken(FUserLoginCompleteDelegate OnComplete);
	static void UserRefresh();
	static void UserLogout(FDefaultCompleteDelegate OnComplete);
	static void DeviceLogin(FUserLoginCompleteDelegate OnComplete);
	static void RegisterNewPlayer(FString Namespace, FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	static void VerifyNewPlayer(FString Namespace, FString UserID, FString VerificationCode, FUserAuthTypeJustice AuthType, FDefaultCompleteDelegate OnComplete);
	static void ReissueVerificationCode(FString Namespace, FString UserID, FString LoginID, FDefaultCompleteDelegate OnComplete);
	static void ReissueVerificationCodeUpgradedAccount(FOAuthTokenJustice Token, FString LoginID, FDefaultCompleteDelegate OnComplete);
	static void ForgotPassword(FString Namespace, FString LoginID, FDefaultCompleteDelegate OnComplete);
	static void ResetPassword(FString Namespace, FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete);
	static void GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete);
	static void LinkPlatform(FString Namespace, FString PlatformID, FString Ticket, FDefaultCompleteDelegate OnComplete);
	static void UnlinkPlatform(FString Namespace, FString PlatformID, FDefaultCompleteDelegate OnComplete);
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