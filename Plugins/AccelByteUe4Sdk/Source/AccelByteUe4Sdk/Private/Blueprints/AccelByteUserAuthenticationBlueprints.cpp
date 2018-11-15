// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserAuthenticationBlueprints.h"
#include "AccelByteUserAuthenticationApi.h"

using AccelByte::Api::UserAuthentication;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsUserAuthentication::LoginWithClientCredentials(const FLoginWithClientCredentialsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithUsernameAndPassword(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithUsernameAndPassword(Username, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithOtherPlatformAccount(EAccelBytePlatformType PlatformId, const FString& Token, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithOtherPlatformAccount(PlatformId, Token, UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithDeviceId(const FLoginWithDeviceIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::RefreshToken(const FRefreshTokenSuccess & OnSuccess, const FBlueprintErrorHandler & OnError)
{
	UserAuthentication::RefreshToken(UserAuthentication::FRefreshTokenSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::ForgetAllCredentials()
{
	UserAuthentication::ForgetAllCredentials();
}

