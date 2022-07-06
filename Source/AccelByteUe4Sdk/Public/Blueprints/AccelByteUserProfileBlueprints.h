// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteUserProfileModels.h"
#include "Api/AccelByteUserProfileApi.h"
#include "AccelByteUserProfileBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUserProfile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void GetUserProfile(const FGetUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FUpdateUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const FCreateUserProfileSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGenerateUploadURLSuccess, const FAccelByteModelsUserProfileUploadURLResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void GenerateUploadURL(const FString& Folder, EAccelByteFileType FileType, const FGenerateUploadURLSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGenerateUploadURLForUserContentSuccess, const FAccelByteModelsUserProfileUploadURLResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void GenerateUploadURLForUserContent(const FString& UserId, EAccelByteFileType FileType, const FGenerateUploadURLForUserContentSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};
