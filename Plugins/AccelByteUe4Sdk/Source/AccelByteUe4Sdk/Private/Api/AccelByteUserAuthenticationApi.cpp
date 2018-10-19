// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteOauth2Models.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"

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

void UserAuthentication::LoginWithClientCredentials(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FLoginWithClientCredentialsSuccess OnSuccess, FErrorDelegate OnError)
{
	Oauth2::GetAccessTokenWithClientCredentialsGrant(ServerBaseUrl, ClientId, ClientSecret, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetClientToken(Result.Access_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}),
		FErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UserAuthentication::LoginWithClientCredentialsEasy(FLoginWithClientCredentialsSuccess OnSuccess, FErrorDelegate OnError)
{
	LoginWithClientCredentials(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, OnSuccess, OnError);
}

void UserAuthentication::LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, std::underlying_type<EPlatformType>::type PlatformId, FString PlatformToken, FLoginWithOtherPlatformAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	Oauth2::GetAccessTokenWithPlatformGrant(ServerBaseUrl, ClientId, ClientSecret, PlatformStrings[PlatformId], PlatformToken, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UserAuthentication::LoginWithOtherPlatformAccountEasy(std::underlying_type<EPlatformType>::type PlatformId, FString PlatformToken, FLoginWithOtherPlatformAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	LoginWithOtherPlatformAccount(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, PlatformId, PlatformToken, OnSuccess, OnError);
}

void UserAuthentication::LoginWithUsernameAndPassword(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Username, FString Password, FLoginWithUsernameAndPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	Oauth2::GetAccessTokenWithPasswordGrant(ServerBaseUrl, ClientId, ClientSecret, Username, Password, Oauth2::FGetAccessTokenWithPasswordGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UserAuthentication::LoginWithUsernameAndPasswordEasy(FString Username, FString Password, FLoginWithUsernameAndPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	LoginWithUsernameAndPassword(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Username, Password, OnSuccess, OnError);
}


void UserAuthentication::LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FLoginWithDeviceIdSuccess OnSuccess, FErrorDelegate OnError)
{
	Oauth2::GetAccessTokenWithDeviceGrant(ServerBaseUrl, ClientId, ClientSecret, Oauth2::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UserAuthentication::LoginWithDeviceIdEasy(FLoginWithDeviceIdSuccess OnSuccess, FErrorDelegate OnError)
{
	LoginWithDeviceId(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, OnSuccess, OnError);
}

void UserAuthentication::ForgetAllCredentials()
{
	Credentials::Get().ForgetAll();
}

} // Namespace Api
} // Namespace AccelByte
