// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Misc/AssertionMacros.h"
#include "GameFramework/GameModeBase.h"
#include "Online.h"
#include "JusticeIdentity.h"
#include "JusticeSDKDemoGameModeBase.generated.h"

UCLASS()
class JUSTICESDKDEMO_API AJusticeSDKDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AJusticeSDKDemoGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaSeconds) override;
};
