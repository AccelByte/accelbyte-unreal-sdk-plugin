// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteOauth2Models.h"
#include "AccelByteRegistry.h"
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
	Oauth2::GetAccessTokenWithClientCredentialsGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		FRegistry::Credentials.SetClientToken(Result.Access_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPlatformGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, PlatformStrings[static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId)], PlatformToken, Oauth2::FGetAccessTokenWithPlatformGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);
		
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithUsernameAndPassword(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPasswordGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, Username, Password, Oauth2::FGetAccessTokenWithPasswordGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);

		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginWithDeviceId(const FLoginWithDeviceIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithDeviceGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, Oauth2::FGetAccessTokenWithDeviceGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result)
	{
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);

		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UserAuthentication::LoginFromLauncher(const FString & AuthorizationCode, const FGetAccessTokenWithAuthorizationCodeGrantSuccess & OnSuccess, const FErrorHandler & OnError)
{
    Oauth2::GetAccessTokenWithAuthorizationCodeGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, AuthorizationCode, FRegistry::Settings.RedirectURI, Oauth2::FGetAccessTokenWithAuthorizationCodeGrantSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOauth2Token& Result) {
        FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);
        OnSuccess.ExecuteIfBound(Result);
    }), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) {
        OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
    }));
}

void UserAuthentication::ForgetAllCredentials()
{
	FRegistry::Credentials.ForgetAll();
}

} // Namespace Api
} // Namespace AccelByte
