// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Private/Models/OAuthTokenJustice.h"
#include "JusticeSingleton.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMJUSTICE_API UJusticeSingleton : public UObject
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintPure, Category = "Justice Singleton")
	static UJusticeSingleton* Instance();
public:
	UOAuthTokenJustice *	UserToken;
	UOAuthTokenJustice*		GameClientToken;
	FString BaseURL;
	FString Namespace;
	FString ClientID;
	FString ClientSecret;


	
};
