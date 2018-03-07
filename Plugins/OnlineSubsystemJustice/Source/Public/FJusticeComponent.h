// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSubsystem.h"
#include "Online.h"
#include "Components/SceneComponent.h"
#include "OnlineSubsystemJusticePackage.h"
#include "SocketSubsystem.h"
#include "HttpRequestAdapter.h"
#include "Private/AWSXRayJustice.h"
#include "Private/Models/OAuthTokenJustice.h"
#include "Private/Models/UserCreateResponse.h"
#include "Private/Models/UserCreateRequest.h"
#include "Private/Models/UserProfileJustice.h"
#include "FJusticeComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FProfileReqestCompleteDelegate, bool, IsSuccess, FString, ErrorStr, UUserProfileJustice*, PlayerProfile);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FRegisterPlayerCompleteDelegate, bool, IsSuccess, FString, ErrorStr, UUserCreateResponse*, Response);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FVerifyNewPlayerCompleteDelegate, bool, IsSuccess, FString, ErrorStr);

UCLASS()
class ONLINESUBSYSTEMJUSTICE_API UFJusticeComponent : public USceneComponent
{
	GENERATED_BODY()
public:	
	static UFJusticeComponent * Instance;
	static UFJusticeComponent* GetInstance() { return Instance; };

	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
	void RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate onComplete);
	
	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
	void RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDelegate onComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
	void VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate onComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
	void ClientCredentialLogin();

	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
		void ForgotPasswordStep1(FString LoginId/*, FForgotPasswordStep1CompleteDelegate onComplete*/);

	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
		void ForgotPasswordStep2(FString UserId, FString VerificationCode, FString NewPassword /*, FForgotPasswordStep2CompleteDelegate onComplete*/);

private:
	FString						   GameClientNameSpace;
	FOAuthTokenJustice			   GameClientToken;

	FRegisterPlayerCompleteDelegate	RegisterPlayerCompleteDelegate;
	FProfileReqestCompleteDelegate  PlayerProfileCompleteDelegate;
	FVerifyNewPlayerCompleteDelegate VerifyNewPlayerCompleteDelegate;

private: 
	// callback
	void TokenGameClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, /* default FHttpRequestCompleteDelegate*/		
		TSharedRef<FAWSXRayJustice> RequestTrace);

	void TokenRegisterPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, /* default FHttpRequestCompleteDelegate*/
		TSharedRef<FAWSXRayJustice> RequestTrace);

	void TokenPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful,
		FString UserId,
		TSharedRef<FAWSXRayJustice> RequestTrace);

	void TokenVerifyNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful,
		FString UserId,
		TSharedRef<FAWSXRayJustice> RequestTrace);
};
