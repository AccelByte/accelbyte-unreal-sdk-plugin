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
}

void AJusticeUE4PluginGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	IOnlineSubsystem* const justice = IOnlineSubsystem::Get();
	justice->Init();
	
	IOnlineIdentityPtr ident = justice->GetIdentityInterface();
	ident->Login(0, FOnlineAccountCredentials(TEXT("c60af7f2ce0f40b59ff32c737253c695"),TEXT("testtest"),TEXT("Justice")));
	//ident->AutoLogin(0);
}

