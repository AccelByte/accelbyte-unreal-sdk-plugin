// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Private/Models/OAuthTokenJustice.h"
#include "JusticeIdentity.h"
#include "JusticeIdentityFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FUserLoginCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr, UOAuthTokenJustice*, Token);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FUserLogoutCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FRegisterPlayerCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr, UUserCreateResponse*, Response);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FVerifyNewPlayerCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr);

UCLASS()
class ONLINESUBSYSTEMJUSTICE_API UJusticeIdentityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLogin(FString LoginId, FString Password, FUserLoginCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLogout(FUserLogoutCompleteDynamicDelegate OnComplete) ;
	
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ForgotPasswordStep1(FString LoginId/*, FForgotPasswordStep1CompleteDelegate onComplete*/);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ForgotPasswordStep2(FString UserId, FString VerificationCode, FString NewPassword /*, FForgotPasswordStep2CompleteDelegate onComplete*/);

	// Platform specific
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLoginWithSteam(FUserLoginCompleteDynamicDelegate OnComplete);

	
};
