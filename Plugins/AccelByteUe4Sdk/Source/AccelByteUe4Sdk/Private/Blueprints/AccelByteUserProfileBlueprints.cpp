// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserProfileBlueprints.h"

using AccelByte::Api::UserProfile;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsUserProfile::GetUserProfile(const FGetUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserProfile::GetUserProfile(UserProfile::FGetUserProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FUpdateUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserProfile::UpdateUserProfile(ProfileUpdateRequest, UserProfile::FUpdateUserProfileSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}


void UAccelByteBlueprintsUserProfile::CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const FCreateUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	UserProfile::CreateUserProfile(ProfileCreateRequest, UserProfile::FCreateUserProfileSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
