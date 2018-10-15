// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUe4SdkDemoGameModeBase.h"
#include "Misc/ConfigCacheIni.h"
#include "AccelByteUserManagementApi.h"
#include "AccelByteWalletModels.h"

AAccelByteUe4SdkDemoGameModeBase::AAccelByteUe4SdkDemoGameModeBase()
{
}

void AAccelByteUe4SdkDemoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AAccelByteUe4SdkDemoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}


void AAccelByteUe4SdkDemoGameModeBase::OnClientCredentialReady_Implementation()
{

}



void AAccelByteUe4SdkDemoGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}



void AAccelByteUe4SdkDemoGameModeBase::Tick(float DeltaSeconds)
{
}

void AAccelByteUe4SdkDemoGameModeBase::OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr)
{
}
