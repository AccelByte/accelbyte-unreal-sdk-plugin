// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteTurnModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsTurnServer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	int32 Qos_port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Last_update{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	int64 Current_time{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsTurnServerList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | QosServerList")
	TArray<FAccelByteModelsTurnServer> Servers{};
};
