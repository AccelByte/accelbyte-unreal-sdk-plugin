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
	FString Alias{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	float Cpu_usage{};

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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsTurnServerCredential
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	int Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Username{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Password{};
};

UENUM(BlueprintType)
enum class EP2PConnectionType : uint8
{
	None	UMETA(DisplayName = "none"),
	Host	UMETA(DisplayName = "host"),
	Srflx	UMETA(DisplayName = "srflx"),
	Prflx	UMETA(DisplayName = "prflx"),
	Relay	UMETA(DisplayName = "relay")
};

USTRUCT(BlueprintType)
struct FAccelByteModelsTurnManagerMetric
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	int32 Latency{};
};


USTRUCT(BlueprintType)
struct FAccelByteModelsServerResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Alias{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Turn | Models | TurnServer")
	FString Type{};
};