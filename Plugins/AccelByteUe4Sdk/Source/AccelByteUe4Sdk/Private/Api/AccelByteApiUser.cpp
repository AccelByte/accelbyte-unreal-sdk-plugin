// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteApiUser.h"
#include "AccelByteApiIdentity.h"
#include "AccelByteModelsIdentity.h"
#include "AccelByteApiUserProfile.h"
#include "AccelByteCredentials.h"
#include "AccelByteModelsUserProfile.h"

namespace AccelByte
{
namespace Api
{

static FString PlatformStrings[] = {
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

void User::ClientLogin(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FClientLoginSuccess OnSuccess, AccelByte::ErrorDelegate OnError)
{
	Identity::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Identity::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess, OnError](FAccelByteModelsOAuthToken Result)
	{
		CredentialStore.SetClientToken(Result.Access_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, uint8 PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::GetAccessTokenWithPlatformGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, PlatformStrings[static_cast<std::underlying_type<EPlatformType>::type>(PlatformId)], Token, Identity::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnError](FAccelByteModelsOAuthToken Result)
	{
		CredentialStore.SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		User::InitializeProfile(ServerBaseUrl, OnSuccess, OnError);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::LoginWithEmailAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FString Password, FLoginWithEmailAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::GetUserAccessTokenWithPasswordGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, Email, Password, Identity::FGetUserAccessTokenWithPasswordGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnError](FAccelByteModelsOAuthToken Result)
	{
		CredentialStore.SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		User::InitializeProfile(ServerBaseUrl, OnSuccess, OnError);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::LoginFromLauncher(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, ErrorDelegate OnError)
{
	TSharedPtr<TCHAR> AuthorizationCode;
	FGenericPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthorizationCode.Get(), 512);
	Identity::GetAccessTokenWithAuthorizationCodeGrant(ServerBaseUrl, ClientId, ClientSecret, FString(AuthorizationCode.Get()), RedirectUri, Identity::FGetUserAccessTokenWithAuthorizationCodeGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnError](FAccelByteModelsOAuthToken Result)
	{
		CredentialStore.SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		User::InitializeProfile(ServerBaseUrl, OnSuccess, OnError);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FLoginWithDeviceIdSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::GetAccessTokenWithDeviceGrant(ServerBaseUrl, ClientId, ClientSecret, Namespace, Identity::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnError](FAccelByteModelsOAuthToken Result)
	{
		CredentialStore.SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		User::InitializeProfile(ServerBaseUrl, OnSuccess, OnError);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::ResetCredentials()
{
	CredentialStore = Credentials();
}

void User::UpgradeHeadlessAccount(FString ServerBaseUrl, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	Credentials Cred = CredentialStore;
	Identity::UpgradeHeadlessUserAccount(ServerBaseUrl, Cred.GetClientAccessToken(), Cred.GetClientNamespace(), Cred.GetUserId(), Email, Password,
		Identity::FUpgradeHeadlessAccountSuccess::CreateLambda([ServerBaseUrl, OnSuccess, OnError, Email, Cred]()
	{
		Identity::SendVerificationCodeForUserAccountCreation(ServerBaseUrl, Cred.GetClientAccessToken(), Cred.GetClientNamespace(), Cred.GetUserId(), Email,
			Identity::FSendVerificationCodeForUserAccountCreationSuccess::CreateLambda([OnSuccess, OnError]()
		{
			OnSuccess.ExecuteIfBound();
		}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::CreateEmailAccount(FString ServerBaseUrl, FString Email, FString Password, FString DisplayName, FCreateEmailAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	const Credentials Cred = CredentialStore;
	Identity::CreateUserAccount(ServerBaseUrl, CredentialStore.GetClientAccessToken(), CredentialStore.GetClientNamespace(), Email, Password, DisplayName, EUserAuthType::Email,
		Identity::FCreateUserAccountSuccess::CreateLambda([OnSuccess, OnError, Cred, ServerBaseUrl, Email](FAccelByteModelsUserCreateResponse Result)
	{
		Identity::SendVerificationCodeForUserAccountCreation(ServerBaseUrl, CredentialStore.GetClientAccessToken(), CredentialStore.GetClientNamespace(), Result.UserId, Email,
			Identity::FSendVerificationCodeForUserAccountCreationSuccess::CreateLambda([OnSuccess, OnError, Result]()
		{
			OnSuccess.ExecuteIfBound(Result);
		}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::VerifyEmailAccount(FString ServerBaseUrl, FString UserId, FString VerificationCode, FVerifyEmailAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::VerifyUserAccountCreation(ServerBaseUrl, CredentialStore.GetClientAccessToken(), CredentialStore.GetClientNamespace(), UserId, VerificationCode, EUserAuthType::Email, OnSuccess, OnError);
}

void User::RequestPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FRequestPasswordResetSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::SendVerificationCodeForPasswordReset(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId,
		Identity::FSendVerificationCodeForPasswordResetSuccess::CreateLambda([OnSuccess, OnError]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::VerifyPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FString VerificationCode, FString NewPassword, FVerifyPasswordResetSuccess OnSuccess, ErrorDelegate OnError)
{
	Identity::VerifyPasswordReset(ServerBaseUrl, ClientId, ClientSecret, Namespace, LoginId, VerificationCode, NewPassword, Identity::FVerifyResetPasswordSuccess::CreateLambda([OnSuccess, OnError]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::GetProfile(FString ServerBaseUrl, FGetProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	const Credentials Cred = CredentialStore;
	UserProfile::GetUserProfile(ServerBaseUrl, Cred.GetUserAccessToken(), Cred.GetUserNamespace(), Cred.GetUserId(), UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess, OnError](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::UpdateProfile(FString ServerBaseUrl, const FAccelByteModelsUserProfileInfoUpdate& UpdateProfile, FUpdateProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	const Credentials Cred = CredentialStore;
	UserProfile::UpdateUserProfile(ServerBaseUrl, Cred.GetUserAccessToken(), Cred.GetUserNamespace(), Cred.GetUserId(),
		UpdateProfile, UserProfile::FUpdateUserProfileSuccess::CreateLambda([OnSuccess, OnError]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void User::InitializeProfile(FString ServerBaseUrl, FInitializeProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	Credentials Cred = CredentialStore;
	UserProfile::GetUserProfile(ServerBaseUrl, Cred.GetUserAccessToken(), Cred.GetUserNamespace(), Cred.GetUserId(), UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess, OnError, Cred](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([ServerBaseUrl, OnSuccess, OnError, Cred](int32 Code, FString Message)
	{
		if (Code  == 2241) // errors.net.accelbyte.platform.profile.user_profile_not_found 
		{
			FString DisplayName = Cred.GetUserDisplayName().IsEmpty() ? FGenericPlatformMisc::GetDeviceId() : Cred.GetUserDisplayName();
			UserProfile::CreateDefaultUserProfile(ServerBaseUrl, Cred.GetUserAccessToken(), Cred.GetUserNamespace(), Cred.GetUserId(), DisplayName,
				UserProfile::FCreateDefaultUserProfileSuccess::CreateLambda([OnSuccess, OnError](const FAccelByteModelsUserProfileInfo& Result)
			{
				OnSuccess.ExecuteIfBound();
			}), ErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
		}
		else
		{
			OnError.ExecuteIfBound(Code, Message);
		}
	}));
}

} // Namespace Api
} // Namespace AccelByte
