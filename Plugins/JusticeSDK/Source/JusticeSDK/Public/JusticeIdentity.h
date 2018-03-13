// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Private/Models/OAuthTokenJustice.h"
#include "Private/Models/UserCreateResponse.h"
#include "Private/Models/UserCreateRequest.h"



DECLARE_DELEGATE_ThreeParams(FUserLoginCompleteDelegate, bool, FString, UOAuthTokenJustice*);
DECLARE_DELEGATE_TwoParams(FUserLogoutCompleteDelegate, bool, FString);
DECLARE_DELEGATE_ThreeParams(FRegisterPlayerCompleteDelegate, bool, FString, UUserCreateResponse*);
DECLARE_DELEGATE_TwoParams(FVerifyNewPlayerCompleteDelegate, bool, FString);


class JUSTICESDK_API JusticeIdentity 
{
	
public:
	static void UserLogin(FString LoginId, FString Password, FUserLoginCompleteDelegate OnComplete);
	static void UserLogout(FUserLogoutCompleteDelegate OnComplete);
	static void RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDelegate OnComplete);
	static void VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate OnComplete);
	static void ForgotPasswordStep1(FString LoginId/*, FForgotPasswordStep1CompleteDelegate onComplete*/);
	static void ForgotPasswordStep2(FString UserId, FString VerificationCode, FString NewPassword /*, FForgotPasswordStep2CompleteDelegate onComplete*/);

	// Platform specific
	static void LinkSteam(FUserLoginCompleteDelegate OnComplete);
	
	// Client specific
	static void ClientLogin();

	static UOAuthTokenJustice* GetUserToken();
	static UOAuthTokenJustice* GetClientToken();
	static FString GetUserId();


};
