// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsUser.h"
#include "AccelByteApiUser.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::User;
using AccelByte::ErrorDelegate;
using AccelByte::Settings;
using AccelByte::CredentialStore;

void UAccelByteBlueprintsUser::ClientLogin(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FClientLoginSuccess OnSuccess, FBlueprintError OnError)
{
	User::ClientLogin(ServerBaseUrl, ClientId, ClientSecret,
		User::FClientLoginSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::ClientLoginEasy(FClientLoginSuccess OnSuccess, FBlueprintError OnError)
{
	ClientLogin(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId,
	FString ClientSecret, FString Namespace, EAccelBytePlatformType PlatformId, FString Token,
	FLoginWithOtherPlatformAccountSuccess OnSuccess, FBlueprintError OnError)
{
	User::LoginWithOtherPlatformAccount(ServerBaseUrl, ClientId,
		ClientSecret, Namespace, static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId), Token,
		User::FLoginWithOtherPlatformAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, FBlueprintError OnError)
{
	LoginWithOtherPlatformAccount(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::Namespace, PlatformId, Token, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::LoginWithEmailAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FString Password, FLoginWithEmailAccountSuccess OnSuccess, FBlueprintError OnError)
{
	User::LoginWithEmailAccount(ServerBaseUrl, ClientId,
		ClientSecret, Namespace, Email, Password,
		User::FLoginWithEmailAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::LoginWithEmailAccountEasy(FString Email, FString Password, FLoginWithEmailAccountSuccess OnSuccess, FBlueprintError OnError)
{
	LoginWithEmailAccount(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::Namespace, Email, Password, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::ResetCredentials()
{
	User::ResetCredentials();
}

void UAccelByteBlueprintsUser::LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret,
	FString Namespace, FLoginWithDeviceIdSuccess OnSuccess, FBlueprintError OnError)
{
	User::LoginWithDeviceId(ServerBaseUrl, ClientId,
		ClientSecret, Namespace,
		User::FLoginWithDeviceIdSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::LoginWithDeviceIdEasy(FLoginWithDeviceIdSuccess OnSuccess, FBlueprintError OnError)
{
	LoginWithDeviceId(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::Namespace, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::UpgradeHeadlessAccount(FString ServerBaseUrl, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, FBlueprintError OnError)
{
	User::UpgradeHeadlessAccount(ServerBaseUrl, Email, Password,
		User::FUpgradeHeadlessAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::UpgradeHeadlessAccounEasy(FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, FBlueprintError OnError)
{
	UpgradeHeadlessAccount(Settings::ServerBaseUrl, Email, Password, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::CreateEmailAccount(FString ServerBaseUrl, FString Namespace, FString Email, FString Password, FString DisplayName, FCreateEmailAccountSuccess OnSuccess, FBlueprintError OnError)
{
	User::CreateEmailAccount(ServerBaseUrl, Email, Password, DisplayName,
		User::FCreateEmailAccountSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserCreateResponse& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::CreateEmailAccountEasy(FString Email, FString Password, FString DisplayName, FCreateEmailAccountSuccess OnSuccess, FBlueprintError OnError)
{
	CreateEmailAccount(Settings::ServerBaseUrl, Settings::Namespace, Email, Password, DisplayName, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::VerifyEmailAccount(FString ServerBaseUrl, FString Namespace, FString UserId, FString VerificationCode, FVerifyEmailAccountSuccess OnSuccess, FBlueprintError OnError)
{
	User::VerifyEmailAccount(ServerBaseUrl, UserId, VerificationCode,
		User::FVerifyEmailAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::VerifyEmailAccountEasy(FString UserId, FString VerificationCode, FVerifyEmailAccountSuccess OnSuccess, FBlueprintError OnError)
{
	VerifyEmailAccount(Settings::ServerBaseUrl, Settings::Namespace, UserId, VerificationCode, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::RequestPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FRequestPasswordResetSuccess OnSuccess, FBlueprintError OnError)
{
	User::RequestPasswordReset(ServerBaseUrl, ClientId, ClientSecret, Namespace, Email,
		User::FRequestPasswordResetSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::RequestPasswordResetEasy(FString Email, FRequestPasswordResetSuccess OnSuccess, FBlueprintError OnError)
{
	RequestPasswordReset(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::Namespace, Email, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::VerifyPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString VerificationCode, FString Email, FString NewPassword, FVerifyPasswordResetSuccess OnSuccess, FBlueprintError OnError)
{
	User::VerifyPasswordReset(ServerBaseUrl, ClientId, ClientSecret, Namespace, Email, VerificationCode, NewPassword,
		User::FVerifyPasswordResetSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::VerifyPasswordResetEasy(FString VerificationCode, FString Email, FString NewPassword, FVerifyPasswordResetSuccess OnSuccess, FBlueprintError OnError)
{
	VerifyPasswordReset(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::Namespace, VerificationCode, Email, NewPassword, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::LoginFromLauncher(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, FBlueprintError OnError)
{
	User::LoginFromLauncher(ServerBaseUrl, ClientId, ClientSecret, RedirectUri,
		User::FLoginFromLauncherSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::LoginFromLauncherEasy(FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, FBlueprintError OnError)
{
	LoginFromLauncher(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, RedirectUri, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::GetProfile(FString ServerBaseUrl, FGetProfileSuccess OnSuccess, FBlueprintError OnError)
{
	User::GetProfile(ServerBaseUrl,
		User::FGetProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::GetProfileEasy(FGetProfileSuccess OnSuccess, FBlueprintError OnError)
{
	GetProfile(Settings::ServerBaseUrl, OnSuccess, OnError);
}

void UAccelByteBlueprintsUser::UpdateProfile(FString ServerBaseUrl,
	const FAccelByteModelsUserProfileInfoUpdate& NewProfile, FUpdateProfileSuccess OnSuccess,
	FBlueprintError OnError)
{
	User::UpdateProfile(ServerBaseUrl, NewProfile,
		User::FLoginWithOtherPlatformAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUser::UpdateProfileEasy(const FAccelByteModelsUserProfileInfoUpdate& NewProfile, FUpdateProfileSuccess OnSuccess, FBlueprintError OnError)
{
	UpdateProfile(Settings::ServerBaseUrl, NewProfile, OnSuccess, OnError);
}

FString UAccelByteBlueprintsUser::GetUserAccessToken()
{
	return CredentialStore.GetUserAccessToken();
}

FString UAccelByteBlueprintsUser::GetUserRefreshToken()
{
	return CredentialStore.GetUserRefreshToken();
}

FDateTime UAccelByteBlueprintsUser::GetUserAccessTokenExpirationUtc()
{
	return CredentialStore.GetUserAccessTokenExpirationUtc();
}

FString UAccelByteBlueprintsUser::GetUserId()
{
	return CredentialStore.GetUserId();
}

FString UAccelByteBlueprintsUser::GetUserDisplayName()
{
	return CredentialStore.GetUserDisplayName();
}

FString UAccelByteBlueprintsUser::GetUserNamespace()
{
	return CredentialStore.GetUserNamespace();
}

FString UAccelByteBlueprintsUser::GetSettingsServerBaseUrl()
{
	return Settings::ServerBaseUrl;
}

FString UAccelByteBlueprintsUser::GetSettingsClientId()
{
	return Settings::ClientId;
}

FString UAccelByteBlueprintsUser::GetSettingsClientSecret()
{
	return Settings::ClientSecret;
}

FString UAccelByteBlueprintsUser::GetSettingsNamespace()
{
	return Settings::Namespace;
}
