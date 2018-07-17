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

DECLARE_DELEGATE_ThreeParams(FUserLoginCompleteDelegate, bool, FString, OAuthTokenJustice*);
DECLARE_DELEGATE_TwoParams(FUserLogoutCompleteDelegate, bool, FString);
DECLARE_DELEGATE_ThreeParams(FRegisterPlayerCompleteDelegate, bool, FString, UserCreateResponse*);
DECLARE_DELEGATE_TwoParams(FVerifyNewPlayerCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FForgotPasswordCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FResetPasswordCompleteDelegate, bool, FString);
DECLARE_DELEGATE_ThreeParams(FGetLinkedPlatformCompleteDelegate, bool, FString, TArray<LinkedPlatform>);
DECLARE_DELEGATE_TwoParams(FLinkPlatformCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FUnlinkPlatformCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FUpgradeHeadlessAccountCompleteDelegate, bool, FString);

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
	static void AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate OnComplete);
	static void UserLogin(FString LoginID, FString Password, FUserLoginCompleteDelegate OnComplete);
	static void UserRefreshToken(FUserLoginCompleteDelegate OnComplete);
	static void UserRefresh();
	static void UserLogout(FUserLogoutCompleteDelegate OnComplete);
	static void DeviceLogin(FUserLoginCompleteDelegate OnComplete);
	static void RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	static void VerifyNewPlayer(FString UserID, FString VerificationCode, FUserAuthTypeJustice AuthType, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void ReissueVerificationCode(FString UserID, FString LoginID, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void ForgotPassword(FString LoginID, FForgotPasswordCompleteDelegate OnComplete);
	static void ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDelegate OnComplete);
	static void GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete);
	static void LinkPlatform(FString PlatformID, FString Ticket, FLinkPlatformCompleteDelegate OnComplete);
	static void UnlinkPlatform(FString PlatformID, FUnlinkPlatformCompleteDelegate OnComplete);
	static void UpgradeHeadlessAccount(FString Namespace, FString ClientAccessToken, FString UserId, FString Email, FString Password, FUpgradeHeadlessAccountCompleteDelegate OnComplete);
	
	static void ClientLogin(FUserLoginCompleteDelegate OnComplete = nullptr);
	static void ClientLogout();
	static void ClientRefreshToken();
	static void SetRefreshToken(FString UserRefreshToken);

private:
	static void OnDeviceLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserRefreshResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserLogoutResponse(FJusticeHttpResponsePtr Response, FUserLogoutCompleteDelegate OnComplete);
	static void OnClientLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnClientRefreshResponse(FJusticeHttpResponsePtr Response);
	static void OnClientLogoutResponse(FJusticeHttpResponsePtr Response);
	static void OnRegisterNewPlayerResponse(FJusticeHttpResponsePtr Response, FRegisterPlayerCompleteDelegate OnComplete);
	static void OnVerifyNewPlayerResponse(FJusticeHttpResponsePtr Response, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void OnReissueVerificationCodeResponse(FJusticeHttpResponsePtr Response, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void OnForgotPasswordResponse(FJusticeHttpResponsePtr Response, FForgotPasswordCompleteDelegate OnComplete);
	static void OnResetPasswordResponse(FJusticeHttpResponsePtr Response, FResetPasswordCompleteDelegate OnComplete);
	static void OnGetLinkedPlatformResponse(FJusticeHttpResponsePtr Response, FGetLinkedPlatformCompleteDelegate OnComplete);
	static void OnLinkPlatformResponse(FJusticeHttpResponsePtr Response, FLinkPlatformCompleteDelegate OnComplete);
	static void OnUnlinkPlatformResponse(FJusticeHttpResponsePtr Response, FUnlinkPlatformCompleteDelegate OnComplete);
	static void OnUpgradeHeadlessAccountResponse(FJusticeHttpResponsePtr Response, FUpgradeHeadlessAccountCompleteDelegate OnComplete, FString LoginID);
};