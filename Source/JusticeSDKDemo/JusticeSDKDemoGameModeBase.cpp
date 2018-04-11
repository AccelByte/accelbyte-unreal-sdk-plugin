// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeSDKDemoGameModeBase.h"
#include "Misc/ConfigCacheIni.h"


AJusticeSDKDemoGameModeBase::AJusticeSDKDemoGameModeBase()
{
}

void AJusticeSDKDemoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	JusticeIdentity::ClientLogin();
}

void AJusticeSDKDemoGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	JusticeIdentity::ClientLogout();
}

void AJusticeSDKDemoGameModeBase::Tick(float DeltaSeconds)
{
}


