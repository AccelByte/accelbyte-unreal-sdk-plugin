// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JusticePlatformFunctions.h"
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
	device,
	Twitch,
	Oculus,
	Twitter
};

class JUSTICESDK_API JusticeUser
{
public:
    
    static void Login(EPlatformType PlatformType, FString Token, FUserLoginCompleteDelegate2 OnComplete);
	static void Login(FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete);
	static void Logout(FDefaultCompleteDelegate OnComplete);
	static void LoginWithDeviceId(FUserLoginCompleteDelegate2 OnComplete);
	static void UpgradeHeadlessAccount(FString Email, FString Password, FString DisplayName, FDefaultCompleteDelegate OnComplete);
	static void Register(FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	static void Verify(FString VerificationCode, FDefaultCompleteDelegate OnComplete);
	static void ForgotPassword(FString LoginID, FDefaultCompleteDelegate OnComplete);
	static void ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete);
	static void LoginFromLauncher(FUserLoginCompleteDelegate OnComplete);

	// Steam Specific
	static void LoginWithSteam(FUserLoginCompleteDelegate2 OnComplete);
	static void LoginWithSteam(FString SteamAuthTicket, FUserLoginCompleteDelegate2 OnComplete);
	static void SetSteamAuthTicket(FString Ticket);
	static FString GetSteamAuthTicket();
	static void SetAvatar(UTexture2D* Avatar);
	static UTexture2D* GetAvatar();

	// User
	static void GetProfile(FRequestCurrentPlayerProfileCompleteDelegate OnComplete);
	static void UpdateProfile(UserProfileInfoUpdate UpdateProfile, FDefaultCompleteDelegate OnComplete);

private:    
	static void OnLogoutResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
};