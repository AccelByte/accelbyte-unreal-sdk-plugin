// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/AssertionMacros.h"
#include "GameFramework/GameModeBase.h"
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

};
