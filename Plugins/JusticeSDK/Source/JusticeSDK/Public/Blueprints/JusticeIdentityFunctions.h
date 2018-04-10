// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/OAuthTokenJustice.h"
#include "Models/LinkedPlatform.h"
#include "JusticeIdentity.h"
#include "JusticeSDK.h"
#include "JusticeIdentityFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FUserLoginCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FRegisterPlayerCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr, UUserCreateResponse*, Response);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDefaultCompleteDynamicDelegate, bool, bSuccessful, FString, ErrorStr);


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
		static void UserLogout(FDefaultCompleteDynamicDelegate OnComplete) ;
	
	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void VerifyNewPlayer(FString UserID, FString VerificationCode, UUserAuthTypeJustice AuthType, FDefaultCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ForgotPassword(FString LoginID, FDefaultCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
		static void ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDynamicDelegate OnComplete);

	UFUNCTION(BlueprintPure, Category = "JusticeIdentityFunctions")
		static FString GetUserId();

	UFUNCTION(BlueprintPure, Category = "JusticeIdentityFunctions")
		static bool IsHeadlessAccount() { return FJusticeSDKModule::Get().bHeadlessAccount; }




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



    // Steam Specific 
    UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
        static void LoginWithSteam(FUserLoginCompleteDynamicDelegate OnComplete);
    
    UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
        static UTexture2D* GetSteamAvatar();

    UFUNCTION(BlueprintCallable, Category = "JusticeIdentityFunctions")
        static FString GetSteamNickName();

    

};
