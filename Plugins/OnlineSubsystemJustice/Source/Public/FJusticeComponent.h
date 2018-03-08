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
#include "FJusticeComponent.generated.h"


UCLASS()
class ONLINESUBSYSTEMJUSTICE_API UFJusticeComponent : public USceneComponent
{
	GENERATED_BODY()
public:	
	static UFJusticeComponent * Instance;
	static UFJusticeComponent* GetInstance() { return Instance; };


	UFJusticeComponent(const class FObjectInitializer& ObjectInitializer)
		:Super(ObjectInitializer)
	{
		GameClientToken = CreateDefaultSubobject<UOAuthTokenJustice>(TEXT("GameClientToken"));
		UFJusticeComponent::Instance = this;
	}



	UFUNCTION(BlueprintCallable, Category = "JusticeComponent")
	void ClientCredentialLogin();
public:
	UOAuthTokenJustice*		UserToken;
	UOAuthTokenJustice*		GameClientToken;
	
private: 
	// callback
	void TokenGameClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, /* default FHttpRequestCompleteDelegate*/		
		TSharedRef<FAWSXRayJustice> RequestTrace);

};
