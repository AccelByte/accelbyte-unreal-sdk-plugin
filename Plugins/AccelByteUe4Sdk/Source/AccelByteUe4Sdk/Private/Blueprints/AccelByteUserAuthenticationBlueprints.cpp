// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserAuthenticationBlueprints.h"
#include "AccelByteUserAuthenticationApi.h"

using AccelByte::Api::UserAuthentication;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsUserAuthentication::LoginWithClientCredentialsEasy(const FLoginWithClientCredentialsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithUsernameAndPasswordEasy(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithUsernameAndPasswordEasy(Username, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType PlatformId, const FString& Token, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithOtherPlatformAccountEasy(PlatformId, Token, UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithDeviceIdEasy(const FLoginWithDeviceIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserAuthentication::LoginWithDeviceIdEasy(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::ForgetAllCredentials()
{
	UserAuthentication::ForgetAllCredentials();
}

