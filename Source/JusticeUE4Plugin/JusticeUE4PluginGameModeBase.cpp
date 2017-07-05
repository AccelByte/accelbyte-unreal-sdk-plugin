// Fill out your copyright notice in the Description page of Project Settings.

#include "JusticeUE4PluginGameModeBase.h"
#include "Misc/ConfigCacheIni.h"
#include "Online.h"

#define DO_CHECK 1

AJusticeUE4PluginGameModeBase::AJusticeUE4PluginGameModeBase()
{

}


void AJusticeUE4PluginGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	IOnlineSubsystem* const justice = IOnlineSubsystem::Get();
	check(justice);
	justice->Init();

	IOnlineIdentityPtr ident = justice->GetIdentityInterface();    
	ident->AutoLogin(0);
}

