// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserProfileBlueprints.h"

using AccelByte::Api::UserProfile;
using AccelByte::FErrorDelegate;

void UAccelByteBlueprintsUserProfile::GetUserProfileEasy(FGetUserProfileSuccess OnSuccess, FBlueprintError OnError)
{
	UserProfile::GetUserProfileEasy(UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::UpdateUserProfileEasy(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, FUpdateUserProfileSuccess OnSuccess, FBlueprintError OnError)
{
	UserProfile::UpdateUserProfileEasy(ProfileUpdateRequest, UserProfile::FUpdateUserProfileSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}


void UAccelByteBlueprintsUserProfile::CreateUserProfileEasy(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, FCreateUserProfileSuccess OnSuccess, FBlueprintError OnError)
{
	UserProfile::CreateUserProfileEasy(ProfileCreateRequest, UserProfile::FCreateUserProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
