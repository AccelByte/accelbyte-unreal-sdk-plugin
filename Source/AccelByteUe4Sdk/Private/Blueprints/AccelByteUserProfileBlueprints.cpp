// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteUserProfileBlueprints.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::UserProfile;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::FVoidHandler;

void UAccelByteBlueprintsUserProfile::GetUserProfile(const FGetUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::UserProfile.GetUserProfile(AccelByte::THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FUpdateUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::UserProfile.UpdateUserProfile(ProfileUpdateRequest, AccelByte::THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const FCreateUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::UserProfile.CreateUserProfile(ProfileCreateRequest, AccelByte::THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::GenerateUploadURL(const FString& Folder, EAccelByteFileType FileType, const FGenerateUploadURLSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::UserProfile.GenerateUploadURL(Folder, FileType, AccelByte::THandler<FAccelByteModelsUserProfileUploadURLResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileUploadURLResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserProfile::GenerateUploadURLForUserContent(const FString& UserId, EAccelByteFileType FileType, const FGenerateUploadURLForUserContentSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::UserProfile.GenerateUploadURLForUserContent(UserId, FileType, AccelByte::THandler<FAccelByteModelsUserProfileUploadURLResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserProfileUploadURLResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
