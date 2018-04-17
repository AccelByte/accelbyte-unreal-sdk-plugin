// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

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
DECLARE_DYNAMIC_DELEGATE_TwoParams(FForgotPasswordCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FResetPasswordCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr);



UCLASS()
class JUSTICESDK_API UJusticeIdentityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Identity Functions
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLogin(FString LoginId, FString Password, FUserLoginCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLogout(FUserLogoutCompleteDynamicDelegate OnComplete) ;
	
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ForgotPassword(FString LoginId, FForgotPasswordCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ResetPassword(FString UserId, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDynamicDelegate OnComplete);


	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static UOAuthTokenJustice* GetUserToken();

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static UOAuthTokenJustice* GetClientToken();

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static FString GetUserId();


	// Platform Specific 
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void LinkSteam(FUserLoginCompleteDynamicDelegate OnComplete);

	
};
