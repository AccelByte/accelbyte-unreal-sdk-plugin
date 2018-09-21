// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineSubsystem.h"
#include "AccelByteUe4SdkDemoGameModeBase.generated.h"

UCLASS()
class ACCELBYTEUE4SDKDEMO_API AAccelByteUe4SdkDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AAccelByteUe4SdkDemoGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaSeconds) override;

public:
	// Create Event
	UFUNCTION(BlueprintNativeEvent, Category = "AccelByteUe4SdkDemoGameModeBase")
	void OnClientCredentialReady();

	void OnClientCredentialReady_Implementation();

private:
	const IOnlineSubsystem* OnlineSub;
	IOnlineIdentityPtr Identity;
	FDelegateHandle LoginCompleteHandle;
	void OnLoginCompleteDelegate(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr);
};

