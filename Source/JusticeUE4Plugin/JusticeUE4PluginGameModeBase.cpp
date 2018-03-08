// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeUE4PluginGameModeBase.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeIdentity.h"

AJusticeUE4PluginGameModeBase::AJusticeUE4PluginGameModeBase()
{
	JusticeComponent = CreateDefaultSubobject<UFJusticeComponent>(TEXT("JusticeComponent"));
}

void AJusticeUE4PluginGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AJusticeUE4PluginGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	JusticeComponent->ClientCredentialLogin();
	JusticeIdentity::UserLogin("test@example.com", "123456", FUserLoginCompleteDelegate::CreateUObject(this, &AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate));
}

void AJusticeUE4PluginGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate(bool IsSuccess, FString ErrorStr, UOAuthTokenJustice * Token)
{
	UE_LOG(LogOnline, Log, TEXT("AJusticeUE4PluginGameModeBase::OnLoginCompleteDelegate Status:%s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
}

