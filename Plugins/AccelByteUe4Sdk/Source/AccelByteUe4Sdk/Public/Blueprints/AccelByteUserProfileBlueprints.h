// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteUserProfileModels.h"
#include "AccelByteUserProfileApi.h"
#include "AccelByteUserProfileBlueprints.generated.h"

#pragma once

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUserProfile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void GetUserProfileEasy(FGetUserProfileSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FUpdateUserProfileSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void UpdateUserProfileEasy(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, FUpdateUserProfileSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserProfile | Api")
	static void CreateUserProfileEasy(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, FCreateUserProfileSuccess OnSuccess, FBlueprintError OnError);
};
