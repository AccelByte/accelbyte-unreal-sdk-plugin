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

DECLARE_DELEGATE_ThreeParams(FUserLoginCompleteDelegate, bool, FString, FOAuthTokenJustice*);
DECLARE_DELEGATE_ThreeParams(FRegisterPlayerCompleteDelegate, bool, FString, FUserCreateResponse*);
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
	static void AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate OnComplete);
	static void UserLogin(FString LoginID, FString Password, FUserLoginCompleteDelegate OnComplete);
	static void UserRefreshToken(FUserLoginCompleteDelegate OnComplete);
	static void UserRefresh();
	static void UserLogout(FDefaultCompleteDelegate OnComplete);
	static void DeviceLogin(FUserLoginCompleteDelegate OnComplete);
	static void RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	static void VerifyNewPlayer(FString UserID, FString VerificationCode, FUserAuthTypeJustice AuthType, FDefaultCompleteDelegate OnComplete);
	static void ReissueVerificationCode(FString UserID, FString LoginID, FDefaultCompleteDelegate OnComplete);
	static void ForgotPassword(FString LoginID, FDefaultCompleteDelegate OnComplete);
	static void ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete);
	static void GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete);
	static void LinkPlatform(FString PlatformID, FString Ticket, FDefaultCompleteDelegate OnComplete);
	static void UnlinkPlatform(FString PlatformID, FDefaultCompleteDelegate OnComplete);
	static void UpgradeHeadlessAccount(FString Email, FString Password, FDefaultCompleteDelegate OnComplete);
	
	static void ClientLogin(FUserLoginCompleteDelegate OnComplete = nullptr);
	static void ClientLogout();
	static void ClientRefreshToken();
	static void SetRefreshToken(FString UserRefreshToken);

private:
	static void OnDeviceLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserRefreshResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnUserLogoutResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnClientLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete);
	static void OnClientRefreshResponse(FJusticeHttpResponsePtr Response);
	static void OnClientLogoutResponse(FJusticeHttpResponsePtr Response);
	static void OnRegisterNewPlayerResponse(FJusticeHttpResponsePtr Response, FRegisterPlayerCompleteDelegate OnComplete);
	static void OnVerifyNewPlayerResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnReissueVerificationCodeResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnForgotPasswordResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnResetPasswordResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnGetLinkedPlatformResponse(FJusticeHttpResponsePtr Response, FGetLinkedPlatformCompleteDelegate OnComplete);
	static void OnLinkPlatformResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnUnlinkPlatformResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnUpgradeHeadlessAccountResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete, FString LoginID);
};