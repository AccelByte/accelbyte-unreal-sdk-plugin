// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteModelsUserProfile.h"
#include "AccelByteApiUserProfile.h"
#include "AccelByteBlueprintsUserProfile.generated.h"

#pragma once

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUserProfile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | UserProfile")
		static void GetUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetUserProfileSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateDefaultUserProfileSuccess, const FAccelByteModelsUserProfileInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | UserProfile")
		static void CreateDefaultUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString DisplayName, FCreateDefaultUserProfileSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FUpdateUserProfileSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | UserProfile")
		static void UpdateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileInfoUpdate& NewUserProfile, FUpdateUserProfileSuccess OnSuccess, FBlueprintError OnError);

};
