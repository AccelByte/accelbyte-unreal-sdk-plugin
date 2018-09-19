// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeUser.h"
#include "JusticeIdentity.h"
#include "JusticePlatform.h"
#include "JusticeCatalog.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
#include "JusticeSDK.h"

static FString EPlatformString[] = { 
    TEXT("steam"),
    TEXT("google"),
    TEXT("facebook"),
    TEXT("android"),
    TEXT("ios"),
    TEXT("device"),
    TEXT("twitch"),
    TEXT("oculus"),
    TEXT("twitter"),
};


void JusticeUser::Login(EPlatformType PlatformID, FString Token, FUserLoginCompleteDelegate2 OnComplete)
{
}

void JusticeUser::LoginWithSteam(FUserLoginCompleteDelegate2 OnComplete)
{
	JusticeIdentity::LoginWithPlatform(JusticeGameNamespace, TEXT("steam"), JusticeIdentity::GetSteamAuthTicket(), FUserLoginCompleteDelegate2::CreateLambda([&, OnComplete](bool bSuccessful, FString ErrorStr, TSharedPtr<FOAuthTokenJustice> OAuthToken)
	{
		if (bSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""), OAuthToken);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		}
	}));
}

void JusticeUser::LoginWithSteam(FString SteamAuthTicket, FUserLoginCompleteDelegate2 OnComplete)
{
	JusticeIdentity::SetSteamAuthTicket(SteamAuthTicket);
	JusticeIdentity::LoginWithPlatform(JusticeGameNamespace, TEXT("steam"), JusticeIdentity::GetSteamAuthTicket(), FUserLoginCompleteDelegate2::CreateLambda([&, OnComplete](bool bSuccessful, FString ErrorStr, TSharedPtr<FOAuthTokenJustice> OAuthToken)
	{
		if (bSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""), OAuthToken);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		}
	}));
}

void JusticeUser::SetSteamAuthTicket(FString Ticket)
{
	JusticeIdentity::SetSteamAuthTicket(Ticket);
}

FString JusticeUser::GetSteamAuthTicket()
{
	return JusticeIdentity::GetSteamAuthTicket();
}

void JusticeUser::SetAvatar(UTexture2D* Avatar)
{
	JusticeIdentity::SetAvatar(Avatar);
}

UTexture2D* JusticeUser::GetAvatar()
{
	return JusticeIdentity::GetAvatar();
}

void JusticeUser::Login(FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete)
{
}

void JusticeUser::LoginFromLauncher(FUserLoginCompleteDelegate OnComplete)
{
}

void JusticeUser::Logout(FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::OnLogoutResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::LoginWithDeviceId(FUserLoginCompleteDelegate2 OnComplete)
{
}

void JusticeUser::UpgradeHeadlessAccount(FString Email, FString Password, FString DisplayName, FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::Register(FString LoginID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
}

void JusticeUser::Verify(FString VerificationCode, FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::ForgotPassword(FString LoginID, FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::ResetPassword(FString LoginID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::GetProfile(FRequestCurrentPlayerProfileCompleteDelegate OnComplete)
{	
}

void JusticeUser::UpdateProfile(UserProfileInfoUpdate UpdateProfile, FDefaultCompleteDelegate OnComplete)
{
}