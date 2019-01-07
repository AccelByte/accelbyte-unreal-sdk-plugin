// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteUserAuthenticationBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUserAuthentication : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE(FLoginWithClientCredentialsSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
    static void LoginWithClientCredentials(const FLoginWithClientCredentialsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FLoginWithUsernameAndPasswordSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
	static void LoginWithUsernameAndPassword(const FString& Username, const FString& Password, const FLoginWithUsernameAndPasswordSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE(FLoginWithOtherPlatformAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
    static void LoginWithOtherPlatformAccount(EAccelBytePlatformType PlatformId, const FString& Token, const FLoginWithOtherPlatformAccountSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FLoginWithDeviceIdSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
	static void LoginWithDeviceId(const FLoginWithDeviceIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
	static void ForgetAllCredentials();
};
