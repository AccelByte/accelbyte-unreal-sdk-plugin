// Copyright (c) 2012 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "AccelByteLobbyModels.h"
#include "AccelByteDSHubModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerClaimedNotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSHub | Models | ServerClaimedNotif")
	FString Game_mode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSHub | Models | ServerClaimedNotif")
	TArray<FAccelByteModelsMatchingAlly> Matching_allies{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSHub | Models | ServerClaimedNotif")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSHub | Models | ServerClaimedNotif")
	FString Session_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionEndedNotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSHub | Models | SessionEndedNotif")
	bool Is_active{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSHub | Models | SessionEndedNotif")
	FString Session_id{};
};