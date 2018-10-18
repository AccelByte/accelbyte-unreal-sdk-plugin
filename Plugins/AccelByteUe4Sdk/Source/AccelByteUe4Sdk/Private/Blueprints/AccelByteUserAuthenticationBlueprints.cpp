// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserAuthenticationBlueprints.h"
#include "AccelByteUserAuthenticationApi.h"

using AccelByte::Api::UserAuthentication;
using AccelByte::ErrorDelegate;

void UAccelByteBlueprintsUserAuthentication::LoginWithClientCredentialsEasy(FLoginWithClientCredentialsSuccess OnSuccess, FBlueprintError OnError)
{
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithUsernameAndPasswordEasy(FString Username, FString Password, FLoginWithUsernameAndPasswordSuccess OnSuccess, FBlueprintError OnError)
{
	UserAuthentication::LoginWithUsernameAndPasswordEasy(Username, Password, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, FBlueprintError OnError)
{
	UserAuthentication::LoginWithOtherPlatformAccountEasy(static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId), Token, UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::LoginWithDeviceIdEasy(FLoginWithDeviceIdSuccess OnSuccess, FBlueprintError OnError)
{
	UserAuthentication::LoginWithDeviceIdEasy(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserAuthentication::ForgetAllCredentials()
{
	UserAuthentication::ForgetAllCredentials();
}

