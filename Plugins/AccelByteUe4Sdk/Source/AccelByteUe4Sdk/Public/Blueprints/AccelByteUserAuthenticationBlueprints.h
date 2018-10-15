// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteUserAuthenticationBlueprints.generated.h"

UENUM(BlueprintType)
enum class EAccelBytePlatformType : uint8
{
	Steam,
	Google,
	Facebook,
	Android,
	iOS,
	Device,
	Twitch,
	Oculus,
	Twitter
};

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUserAuthentication : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE(FLoginWithClientCredentialsSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
    static void LoginWithClientCredentialsEasy(FLoginWithClientCredentialsSuccess OnSuccess, FBlueprintError OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FLoginWithUsernameAndPasswordSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
	static void LoginWithUsernameAndPasswordEasy(FString Username, FString Password, FLoginWithUsernameAndPasswordSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FLoginWithOtherPlatformAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
    static void LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FLoginWithDeviceIdSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
	static void LoginWithDeviceIdEasy(FLoginWithDeviceIdSuccess OnSuccess, FBlueprintError OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserAuthentication | Api ")
	static void ForgetAllCredentials();
};
