// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteOauth2Models.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"
#include "Runtime/Core/Public/Containers/Ticker.h"

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

void UserAuthentication::LoginWithClientCredentials(const FLoginWithClientCredentialsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithClientCredentialsGrant(Settings::ClientId, Settings::ClientSecret, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetClientToken(Result.Access_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPlatformGrant(Settings::ClientId, Settings::ClientSecret, PlatformStrings[static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId)], PlatformToken, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		FTicker::GetCoreTicker().RemoveTicker(FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate()));
		FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate(), Credentials::Get().GetRefreshTokenDuration());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithUsernameAndPassword(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPasswordGrant(Settings::ClientId, Settings::ClientSecret, Username, Password, Oauth2::FGetAccessTokenWithPasswordGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		FTicker::GetCoreTicker().RemoveTicker(FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate()));
		FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate(), Credentials::Get().GetRefreshTokenDuration());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithDeviceId(const FLoginWithDeviceIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithDeviceGrant(Settings::ClientId, Settings::ClientSecret, Oauth2::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		FTicker::GetCoreTicker().RemoveTicker(FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate()));
		FTicker::GetCoreTicker().AddTicker(Credentials::Get().GetRefreshTokenTickerDelegate(), Credentials::Get().GetRefreshTokenDuration());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginFromLauncher(const FString & AuthorizationCode, const FGetAccessTokenWithAuthorizationCodeGrantSuccess & OnSuccess, const FErrorHandler & OnError)
{
    Oauth2::GetAccessTokenWithAuthorizationCodeGrant(Settings::ClientId, Settings::ClientSecret, AuthorizationCode, Settings::RedirectURI, Oauth2::FGetAccessTokenWithAuthorizationCodeGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Token) {
        Credentials::Get().SetUserToken(Token.Access_token, Token.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Token.Expires_in), Token.User_id, Token.Display_name, Token.Namespace);
        OnSuccess.ExecuteIfBound(Token);
    }), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) {
        OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
    }));
}

void UserAuthentication::RefreshToken(const FRefreshTokenSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithRefreshTokenGrant(Settings::ClientId, Settings::ClientSecret, Credentials::Get().GetUserRefreshToken(), Oauth2::FGetAccessTokenWithRefreshTokenGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		Credentials::Get().SetUserToken(Result.Access_token, Result.Refresh_token, FDateTime::UtcNow() + FTimespan::FromSeconds(Result.Expires_in), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::ForgetAllCredentials()
{
	Credentials::Get().ForgetAll();
}

} // Namespace Api
} // Namespace AccelByte
