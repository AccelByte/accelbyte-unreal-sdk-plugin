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


void JusticeUser::Login(EPlatformType PlatformID, FString Token, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::LoginWithPlatform(JusticeGameNamespace, EPlatformString[(int)PlatformID], Token, FUserLoginCompleteDelegate2::CreateLambda([OnComplete](bool bLoginSuccessful, FString LoginErrorStr, TSharedPtr<FOAuthTokenJustice> OAuthToken)
	{
		if (bLoginSuccessful)
		{
			FJusticeSDKModule::Get().UserToken = OAuthToken;
			JusticeUser::InitProfile(*OAuthToken.Get(), OnComplete);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, LoginErrorStr);
		}
	}));
}

void JusticeUser::Login(FString Email, FString Password, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::UserLogin(JusticeGameNamespace, Email, Password,
		FUserLoginCompleteDelegate2::CreateLambda([OnComplete](bool bSuccessful, FString LoginErrorStr, TSharedPtr<FOAuthTokenJustice> OAuthToken)
	{
		if (bSuccessful)
		{
			FJusticeSDKModule::Get().UserToken = OAuthToken;
			JusticeUser::InitProfile(*OAuthToken.Get(), OnComplete);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, LoginErrorStr);
		}
	}));
}

void JusticeUser::LoginFromLauncher(FDefaultCompleteDelegate OnComplete)
{
	TSharedPtr<TCHAR> AuthCode;
	FGenericPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthCode.Get(), 512);
	JusticeIdentity::AuthCodeLogin(FString(AuthCode.Get()), FJusticeSDKModule::Get().RedirectURI, FUserLoginCompleteDelegate2::CreateLambda([&, OnComplete](bool bSuccessful, FString ErrorStr, TSharedPtr<FOAuthTokenJustice> OAuthToken)
	{
		if (bSuccessful)
		{
			FJusticeSDKModule::Get().UserToken = OAuthToken;
			JusticeUser::InitProfile(*OAuthToken.Get(), OnComplete);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr);
		}
	}));
}

void JusticeUser::Logout(FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::OnLogoutResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
}

void JusticeUser::LoginWithDeviceId(FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::DeviceLogin(JusticeGameNamespace, FUserLoginCompleteDelegate2::CreateLambda([&, OnComplete](bool bSuccessful, FString ErrorStr, TSharedPtr<FOAuthTokenJustice> OAuthToken)
	{
		if (bSuccessful)
		{
			FJusticeSDKModule::Get().UserToken = OAuthToken;
			JusticeUser::InitProfile(*OAuthToken.Get(), OnComplete);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr);
		}
	}));
}

void JusticeUser::UpgradeHeadlessAccount(FString Email, FString Password, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::UpgradeHeadlessAccount(JusticeGameNamespace, FJusticeUserID, Email, Password, FDefaultCompleteDelegate::CreateLambda([&, OnComplete, Email](bool bSuccessful, FString ErrorStr)
	{
		if (bSuccessful)
		{
			JusticeIdentity::ReissueVerificationCode(*FJusticeUserToken.Get(), Email, FDefaultCompleteDelegate::CreateLambda([&, OnComplete](bool bSendSuccessful, FString SendErrorStr)
			{
				if (bSendSuccessful)
				{
					OnComplete.ExecuteIfBound(true, TEXT(""));
				}
				else
				{
					OnComplete.ExecuteIfBound(false, SendErrorStr);
				}
			}));
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr);
		}
	}));
}

void JusticeUser::Register(FString Email, FString Password, FString DisplayName, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::RegisterNewPlayer(JusticeGameNamespace, Email, Password, DisplayName, FUserAuthTypeJustice::Email,
		FRegisterPlayerCompleteDelegate::CreateLambda([Email, OnComplete](bool bSuccessful, FString ErrorStr, TSharedPtr<FUserCreateResponse> UserCreateResponse)
	{
		if (bSuccessful)
		{
			JusticeIdentity::ReissueVerificationCode(JusticeGameNamespace, UserCreateResponse.Get()->UserID, Email,
				FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr)
			{
				if (bSuccessful)
				{
					OnComplete.ExecuteIfBound(true, TEXT(""));
				}
				else
				{
					OnComplete.ExecuteIfBound(false, "ErrorStr");
				}
			}));
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr);
		}
	}));
}

void JusticeUser::Verify(FString VerificationCode, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::VerifyNewPlayer(JusticeGameNamespace, FJusticeUserID, VerificationCode, FUserAuthTypeJustice::Email,
		FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bLoginSuccessful, FString ErrorStr)
	{
		if (bLoginSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""));

		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr);
		}
	}));
}

void JusticeUser::ForgotPassword(FString Email, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::ForgotPassword(JusticeGameNamespace, Email,
		FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr)
	{

		if (bSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""));
		}
		else
		{
			OnComplete.ExecuteIfBound(false, "ErrorStr");
		}
	}));
}

void JusticeUser::ResetPassword(FString VerificationCode, FString Email, FString NewPassword, FDefaultCompleteDelegate OnComplete)
{
	JusticeIdentity::ResetPassword(JusticeGameNamespace, Email, VerificationCode, NewPassword,
		FDefaultCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr)
	{

		if (bSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""));
		}
		else
		{
			OnComplete.ExecuteIfBound(false, "ErrorStr");
		}
	}));
}

void JusticeUser::GetProfile(FRequestCurrentPlayerProfileCompleteDelegate OnComplete)
{	
	JusticePlatform::RequestCurrentPlayerProfile(*FJusticeUserToken.Get(), FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&, OnComplete](bool bSuccessful, FString ErrorStr, FUserProfileInfo * UserProfile)
	{
		if (bSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""), UserProfile);
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		}
	}));
}

void JusticeUser::UpdateProfile(UserProfileInfoUpdate UpdateProfile, FDefaultCompleteDelegate OnComplete)
{
	JusticePlatform::UpdatePlayerProfile(*FJusticeUserToken.Get(), UpdateProfile, FDefaultCompleteDelegate::CreateLambda([&, OnComplete](bool bSuccessful, FString ErrorStr)
	{
		if (bSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""));
		}
		else
		{
			OnComplete.ExecuteIfBound(false, ErrorStr);
		}
	}));
}

void JusticeUser::InitProfile(FOAuthTokenJustice OAuthToken, FDefaultCompleteDelegate OnComplete)
{
	JusticePlatform::RequestCurrentPlayerProfile(OAuthToken, FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&, OnComplete, OAuthToken](bool bGetProfileSuccessful, FString GetProfileErrorStr, FUserProfileInfo * UserProfile)
	{
		if (bGetProfileSuccessful)
		{
			OnComplete.ExecuteIfBound(true, TEXT(""));
		}
		else
		{
			FString DisplayName = (OAuthToken.DisplayName == TEXT("")) ? FGenericPlatformMisc::GetDeviceId() : OAuthToken.DisplayName;
			JusticePlatform::CreateDefaultPlayerProfile(OAuthToken, DisplayName, FDefaultCompleteDelegate::CreateLambda([&, OnComplete, OAuthToken](bool bCreateProfileSuccessful, FString CreateProfileErrorStr)
			{
				if (bCreateProfileSuccessful)
				{
					OnComplete.ExecuteIfBound(true, TEXT(""));
				}
				else
				{
					OnComplete.ExecuteIfBound(false, CreateProfileErrorStr);
				}
			}));
		}
	}));
}
