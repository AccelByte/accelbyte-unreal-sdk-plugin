// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteModelsIdentity.h"
#include "AccelByteBlueprintsIdentity.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsIdentity : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAccessTokenWithDeviceGrantSuccess, const FAccelByteModelsOAuthToken&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Identity")
	static void GetAccessTokenWithDeviceGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FLoginWithDeviceIdGetUserAccessTokenWithPasswordGrantSuccess, const FAccelByteModelsOAuthToken&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Identity")
	static void GetUserAccessTokenWithPasswordGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FString Password, FLoginWithDeviceIdGetUserAccessTokenWithPasswordGrantSuccess OnSuccess, FBlueprintError OnError);
};
