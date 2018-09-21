// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsUserProfile.h"

using namespace AccelByte;
using namespace Services;


void UAccelByteBlueprintsUserProfile::GetUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace,
	FString UserId, FGetUserProfileSuccess OnSuccess, FBlueprintError OnError)
{
	UserProfile::GetUserProfile(ServerBaseUrl, AccessToken, Namespace, UserId, UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::CreateDefaultUserProfile(FString ServerBaseUrl, FString AccessToken,
	FString Namespace, FString UserId, FString DisplayName, FCreateDefaultUserProfileSuccess OnSuccess,
	FBlueprintError OnError)
{
	UserProfile::CreateDefaultUserProfile(ServerBaseUrl, AccessToken, Namespace, UserId, DisplayName, UserProfile::FCreateDefaultUserProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::UpdateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace,
	FString UserId, const FAccelByteModelsUserProfileInfoUpdate& NewUserProfile, FUpdateUserProfileSuccess OnSuccess,
	FBlueprintError OnError)
{
	UserProfile::UpdateUserProfile(ServerBaseUrl, AccessToken, Namespace, UserId, NewUserProfile, UserProfile::FUpdateUserProfileSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

