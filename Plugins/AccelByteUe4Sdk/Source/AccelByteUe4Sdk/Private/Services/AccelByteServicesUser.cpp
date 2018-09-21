// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteServicesUser.h"
#include "AccelByteServicesIdentity.h"
#include "AccelByteModelsIdentity.h"
#include "AccelByteServicesUserProfile.h"
#include "AccelbyteSettings.h"
#include "AccelbyteCredentials.h"

using namespace AccelByte;
using namespace Services;

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

void User::LoginWithPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, EPlatformType PlatformId, FString Token, FLoginWithPlatformAccountSuccess OnSuccess, ErrorDelegate OnFail)
{
	Identity::GetAccessTokenWithPlatformGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, EPlatformString[static_cast<std::underlying_type<EPlatformType>::type>(PlatformId)], Token, Identity::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnFail](FAccelByteModelsOAuthToken Result)
	{
		UserCredentials.SetUserAccessToken(Result);
		User::InitProfile(ServerBaseUrl, OnSuccess, OnFail);
	}),
		ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::LoginWithEmail(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FString Password, FLoginWithEmailSuccess OnSuccess, ErrorDelegate OnFail)
{
	Identity::GetUserAccessTokenWithPasswordGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, Email, Password, Identity::FGetUserAccessTokenWithPasswordGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnFail](FAccelByteModelsOAuthToken Result)
	{
		UserCredentials.SetUserAccessToken(Result);
		User::InitProfile(ServerBaseUrl, OnSuccess, OnFail);
	}),
		ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::LoginFromLauncher(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, ErrorDelegate OnFail)
{
	TSharedPtr<TCHAR> AuthorizationCode;
	FGenericPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthorizationCode.Get(), 512);
	Identity::GetAccessTokenWithAuthorizationCodeGrant(ServerBaseUrl, ClientId, ClientSecret, FString(AuthorizationCode.Get()), RedirectUri, Identity::FGetUserAccessTokenWithAuthorizationCodeGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnFail](FAccelByteModelsOAuthToken Result)
	{
		UserCredentials.SetUserAccessToken(Result);
		User::InitProfile(ServerBaseUrl, OnSuccess, OnFail);
	}),
		ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::ResetCredentials()
{
	UserCredentials.SetUserAccessToken(FAccelByteModelsOAuthToken());
	UserCredentials.SetClientAccessToken(FAccelByteModelsOAuthToken());
}

void User::LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FLoginWithDeviceIdSuccess OnSuccess, ErrorDelegate OnFail)
{
	Identity::GetAccessTokenWithDeviceGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, Identity::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnFail](FAccelByteModelsOAuthToken Result)
	{
		UserCredentials.SetUserAccessToken(Result);
		User::InitProfile(ServerBaseUrl, OnSuccess, OnFail);
	}),
		ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::UpgradeHeadlessAccount(FString ServerBaseUrl, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken UserAccessToken = UserCredentials.GetUserAccessToken();
	Identity::UpgradeHeadlessUserAccount(ServerBaseUrl, UserAccessToken.Access_token, UserAccessToken.Namespace, UserAccessToken.User_id, Email, Password,
		Identity::FUpgradeHeadlessAccountSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnFail, Email, UserAccessToken]()
	{
		Identity::SendVerificationCodeForUserAccountCreation(ServerBaseUrl, UserAccessToken.Access_token, UserAccessToken.Namespace, UserAccessToken.User_id, Email,
			Identity::FSendVerificationCodeForUserAccountCreationSuccess::CreateLambda([OnSuccess, OnFail]()
		{
			OnSuccess.ExecuteIfBound();
		}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
		{
			OnFail.ExecuteIfBound(Code, Message);
		}));
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::RegisterEmailAccount(FString ServerBaseUrl, FString Email, FString Password, FString DisplayName, Identity::FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken ClientAccessToken = UserCredentials.GetClientAccessToken();
	Identity::CreateUserAccount(ServerBaseUrl, ClientAccessToken.Access_token, ClientAccessToken.Namespace, Email, Password, DisplayName, EUserAuthTypeJustice::Email,
		Identity::FCreateUserAccountSuccess::CreateLambda([OnSuccess, OnFail, ClientAccessToken, ServerBaseUrl, Email](FAccelByteModelsUserCreateResponse Result)
	{
		Identity::SendVerificationCodeForUserAccountCreation(ServerBaseUrl, ClientAccessToken.Access_token, ClientAccessToken.Namespace, Result.UserId, Email,
			Identity::FSendVerificationCodeForUserAccountCreationSuccess::CreateLambda([OnSuccess, OnFail, Result]()
		{
			OnSuccess.ExecuteIfBound(Result);
		}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
		{
			OnFail.ExecuteIfBound(Code, Message);
		}));
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::VerifyRegistration(FString ServerBaseUrl, FString UserId, FString VerificationCode, FVerifyRegistrationSuccess OnSuccess, ErrorDelegate OnFail)
{
	// User doesn't need to log in first to verify registration.
	FAccelByteModelsOAuthToken ClientAccessToken = UserCredentials.GetClientAccessToken();
	Identity::VerifyUserAccountCreation(ServerBaseUrl, ClientAccessToken.Access_token, ClientAccessToken.Namespace, UserId, VerificationCode, EUserAuthTypeJustice::Email,
		Identity::FVerifyUserAccountCreation::CreateLambda([OnSuccess, OnFail]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::RequestResetPassword(FString ServerBaseUrl, FString LoginId, FVerifyRegistrationSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken ClientAccessToken = UserCredentials.GetClientAccessToken();
	Identity::SendVerificationCodeForPasswordReset(ServerBaseUrl, ClientAccessToken.Access_token, ClientAccessToken.Namespace, LoginId,
		Identity::FSendVerificationCodeForPasswordResetSuccess::CreateLambda([OnSuccess, OnFail]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::VerifyResetPassword(FString ServerBaseUrl, FString LoginId, FString VerificationCode, FString NewPassword, FVerifyResetPasswordSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken ClientAccessToken = UserCredentials.GetClientAccessToken();
	Identity::VerifyPasswordReset(ServerBaseUrl, ClientAccessToken.Access_token, ClientAccessToken.Namespace, LoginId, VerificationCode, NewPassword,
		Identity::FVerifyResetPasswordSuccess::CreateLambda([OnSuccess, OnFail]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::GetProfile(FString ServerBaseUrl, FGetProfileSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken UserAccessToken = UserCredentials.GetUserAccessToken();
	UserProfile::GetUserProfile(ServerBaseUrl, UserAccessToken.Access_token, UserAccessToken.Namespace, UserAccessToken.User_id, UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess, OnFail](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::UpdateProfile(FString ServerBaseUrl, FAccelByteModelsUserProfileInfoUpdate UpdateProfile, FUpdateProfileSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken UserAccessToken = UserCredentials.GetUserAccessToken();
	UserProfile::UpdateUserProfile(ServerBaseUrl, UserAccessToken.Access_token, UserAccessToken.Namespace, UserAccessToken.User_id,
		UpdateProfile, UserProfile::FUpdateUserProfileSuccess::CreateLambda([OnSuccess, OnFail]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
	{
		OnFail.ExecuteIfBound(Code, Message);
	}));
}

void User::InitProfile(FString ServerBaseUrl, FInitProfileSuccess OnSuccess, ErrorDelegate OnFail)
{
	FAccelByteModelsOAuthToken UserAccessToken = UserCredentials.GetUserAccessToken();
	UserProfile::GetUserProfile(ServerBaseUrl, UserAccessToken.Access_token, UserAccessToken.Namespace, UserAccessToken.User_id, UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess, OnFail, UserAccessToken](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([ServerBaseUrl, OnSuccess, OnFail, UserAccessToken](int32 Code, FString Message)
	{
		FString DisplayName = (UserAccessToken.Display_name == TEXT("")) ? FGenericPlatformMisc::GetDeviceId() : UserAccessToken.Display_name;
		UserProfile::CreateDefaultUserProfile(ServerBaseUrl, UserAccessToken.Access_token, UserAccessToken.Namespace, UserAccessToken.User_id, DisplayName,
			UserProfile::FCreateDefaultUserProfileSuccess::CreateLambda([OnSuccess, OnFail](const FAccelByteModelsUserProfileInfo& Result)
		{
			OnSuccess.ExecuteIfBound();
		}), ErrorDelegate::CreateLambda([OnFail](int32 Code, FString Message)
		{
			OnFail.ExecuteIfBound(Code, Message);
		}));
		OnFail.ExecuteIfBound(Code, Message);
	}));
}
