// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/OAuthTokenJustice.h"
#include "Models/LinkedPlatform.h"
#include "JusticeIdentity.h"
#include "JusticeIdentityFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FUserLoginCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr, UOAuthTokenJustice*, Token);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FUserLogoutCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FRegisterPlayerCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr, UUserCreateResponse*, Response);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FVerifyNewPlayerCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FForgotPasswordCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FResetPasswordCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr);

UCLASS()
class JUSTICESDK_API UJusticeIdentityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// Identity Functions
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLogin(FString LoginID, FString Password, FUserLoginCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void WebLoginRefresh(FString UserRefreshToken, FUserLoginCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void UserLogout(FUserLogoutCompleteDynamicDelegate OnComplete) ;
	
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void VerifyNewPlayer(FString UserID, FString VerificationCode, UUserAuthTypeJustice AuthType, FVerifyNewPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ForgotPassword(FString LoginID, FForgotPasswordCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDynamicDelegate OnComplete);


	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static UOAuthTokenJustice* GetUserToken();

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static UOAuthTokenJustice* GetClientToken();

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static FString GetUserId();

	// Platform Specific 
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void LinkSteam(FUserLoginCompleteDynamicDelegate OnComplete);


	UFUNCTION(BlueprintCallable, Category = "JusticeWebBrowserFunctions")
		static void ClearCacheAndLocalStorage();

	UFUNCTION(BlueprintPure, Category = "JusticeIdentityFunctions")
		static TArray<ULinkedPlatform*> GetUnlinkedPlatforms(TArray<ULinkedPlatform*> LinkedPlatforms);

	UFUNCTION(BlueprintPure, Category = "JusticeIdentityFunctions")
		static TArray<ULinkedPlatform*> GetCompleteListOfPlatforms();

	UFUNCTION(BlueprintPure, Category = "JusticeIdentityFunctions")
		static FString GetPlatformPageURL(FString PlatformID);

	UFUNCTION(BlueprintPure, Category = "JusticeIdentityFunctions")
		static FString GetPlatformRedirectURL(FString PlatformID);
};
