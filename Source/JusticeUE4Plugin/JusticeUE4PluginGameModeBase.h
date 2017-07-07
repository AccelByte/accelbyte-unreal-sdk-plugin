// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/AssertionMacros.h"
#include "GameFramework/GameModeBase.h"
#include "Online.h"
#include "JusticeUE4PluginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class JUSTICEUE4PLUGIN_API AJusticeUE4PluginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AJusticeUE4PluginGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	void BeginPlay();

private:

	void OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr);

	const IOnlineSubsystem* OnlineSub;
	IOnlineIdentityPtr Identity;
	
	FDelegateHandle LoginCompleteHandle;

	// Default to first local user 0
	const int32 DefaultLocalUserNum = 0;

};
