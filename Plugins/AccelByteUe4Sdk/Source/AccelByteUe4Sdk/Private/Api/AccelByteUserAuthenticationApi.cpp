// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
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

void UserAuthentication::LoginWithClientCredentials(const FString& ClientId, const FString& ClientSecret, const FLoginWithClientCredentialsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithClientCredentialsGrant(ClientId, ClientSecret, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetClientToken(Result.Access_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithClientCredentialsEasy(const FLoginWithClientCredentialsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	LoginWithClientCredentials(Settings::ClientId, Settings::ClientSecret, OnSuccess, OnError);
}

void UserAuthentication::LoginWithOtherPlatformAccount(const FString& ClientId, const FString& ClientSecret, EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPlatformGrant(ClientId, ClientSecret, PlatformStrings[static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId)], PlatformToken, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	LoginWithOtherPlatformAccount(Settings::ClientId, Settings::ClientSecret, PlatformId, PlatformToken, OnSuccess, OnError);
}

void UserAuthentication::LoginWithUsernameAndPassword(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPasswordGrant(ClientId, ClientSecret, Username, Password, Oauth2::FGetAccessTokenWithPasswordGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithUsernameAndPasswordEasy(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	LoginWithUsernameAndPassword(Settings::ClientId, Settings::ClientSecret, Username, Password, OnSuccess, OnError);
}


void UserAuthentication::LoginWithDeviceId(const FString& ClientId, const FString& ClientSecret, const FLoginWithDeviceIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithDeviceGrant(ClientId, ClientSecret, Oauth2::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithDeviceIdEasy(const FLoginWithDeviceIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	LoginWithDeviceId(Settings::ClientId, Settings::ClientSecret, OnSuccess, OnError);
}

void UserAuthentication::RefreshToken(const FString& ClientId, const FString& ClientSecret, const FString& RefreshToken, const FRefreshTokenSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithRefreshTokenGrant(ClientId, ClientSecret, RefreshToken, Oauth2::FGetAccessTokenWithRefreshTokenGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::RefreshTokenEasy(const FRefreshTokenSuccess& OnSuccess, const FErrorHandler& OnError)
{
	RefreshToken(Settings::ClientId, Settings::ClientSecret, Credentials::Get().GetUserRefreshToken(), OnSuccess, OnError);
}

void UserAuthentication::ForgetAllCredentials()
{
	Credentials::Get().ForgetAll();
}

} // Namespace Api
} // Namespace AccelByte
