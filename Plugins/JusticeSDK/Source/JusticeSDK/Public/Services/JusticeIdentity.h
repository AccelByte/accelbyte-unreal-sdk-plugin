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
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"

DECLARE_DELEGATE_ThreeParams(FUserLoginCompleteDelegate, bool, FString, UOAuthTokenJustice*);
DECLARE_DELEGATE_TwoParams(FUserLogoutCompleteDelegate, bool, FString);
DECLARE_DELEGATE_ThreeParams(FRegisterPlayerCompleteDelegate, bool, FString, UUserCreateResponse*);
DECLARE_DELEGATE_TwoParams(FVerifyNewPlayerCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FForgotPasswordCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FResetPasswordCompleteDelegate, bool, FString);
DECLARE_DELEGATE_ThreeParams(FGetLinkedPlatformCompleteDelegate, bool, FString, TArray<LinkedPlatform>);
DECLARE_DELEGATE_TwoParams(FLinkPlatformCompleteDelegate, bool, FString);
DECLARE_DELEGATE_TwoParams(FUnlinkPlatformCompleteDelegate, bool, FString);

enum FTaskTypeJustice
{
	IdentityRefresh = 1
};

enum FGrantTypeJustice
{
	PasswordGrant = 1,
	RefreshGrant = 2,
	ClientCredentialGrant = 3,
	Device = 4,
};

class JUSTICESDK_API JusticeIdentity 
{
public:
	static void UserLogin(FString LoginId, FString Password, FUserLoginCompleteDelegate OnComplete);
	static void UserLogout(FUserLogoutCompleteDelegate OnComplete);
	static void DeviceLogin(FUserLoginCompleteDelegate OnComplete);
	static void RefreshToken(FUserLoginCompleteDelegate OnComplete);
	static void RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	static void VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void ReissueVerificationCode(FString UserId, FString LoginId, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void ForgotPassword(FString LoginId, FForgotPasswordCompleteDelegate OnComplete);
	static void ResetPassword(FString UserId, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDelegate OnComplete);
	static void GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete);
	static void LinkPlatform(FString PlatformId, FString Ticket, FLinkPlatformCompleteDelegate OnComplete);
	static void UnlinkPlatform(FString PlatformId, FUnlinkPlatformCompleteDelegate OnComplete);
	
	// Client specific
	static void ClientLogin();
	static void ClientLogout();
	static void ClientRefreshToken();

	static void SetRefreshToken(FString RefreshToken);

	static OAuthTokenJustice* GetUserToken();
	static OAuthTokenJustice* GetClientToken();
	static FString GetUserId();

private:
	static void Login(FString LoginId, FString Password, FGrantTypeJustice GrantType, FUserLoginCompleteDelegate OnComplete);

private:
	static void OnLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete);
	static void OnUserRefreshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete);
	static void OnClientRefreshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace);
	static void OnClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace);
	static void OnLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLogoutCompleteDelegate OnComplete);	
	static void OnClientLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace);
	static void OnRegisterNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FRegisterPlayerCompleteDelegate OnComplete);
	static void OnVerifyNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FVerifyNewPlayerCompleteDelegate OnComplete);	
	static void OnReissueVerificationCodeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void OnForgotPasswordComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FForgotPasswordCompleteDelegate OnComplete);
	static void OnResetPasswordComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FResetPasswordCompleteDelegate OnComplete);
	static void OnGetLinkedPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetLinkedPlatformCompleteDelegate OnComplete);
	static void OnLinkPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FLinkPlatformCompleteDelegate OnComplete);
	static void OnUnlinkPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUnlinkPlatformCompleteDelegate OnComplete);
};