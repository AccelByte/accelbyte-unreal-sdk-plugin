// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsTelemetry.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsTelemetryEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	int32 AgentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	int32 AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	FString ClientId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	FString Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	FString DeviceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	int32 EventId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	int32 EventLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	FString EventTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	int32 EventType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	FString Uuid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	int32 Ux;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Telemetry | TelemetryEvent")
	FString UserId;

	FAccelByteModelsTelemetryEvent()
	{
#if PLATFORM_DESKTOP && PLATFORM_64BITS
		if (!IsRunningDedicatedServer()) { AgentType = 20; }
#endif
#if PLATFORM_DESKTOP && PLATFORM_32BITS
		if (!IsRunningDedicatedServer()) { AgentType = 21; }
#endif
#if PLATFORM_XBOXONE
		AgentType = 31;
#endif
#if PLATFORM_PS4
		AgentType = 40;
#endif
#if PLATFORM_SWITCH
		AgentType = 50;
#endif
#if PLATFORM_LINUX && PLATFORM_64BITS
		if (IsRunningDedicatedServer()) { AgentType = 60; }
#endif
#if PLATFORM_LINUX && PLATFORM_32BITS
		if (IsRunningDedicatedServer()) { AgentType = 61; }
#endif
#if PLATFORM_WINDOWS && PLATFORM_64BITS
		if (IsRunningDedicatedServer()) { AgentType = 62; }
#endif
#if PLATFORM_WINDOWS && PLATFORM_32BITS
		if (IsRunningDedicatedServer()) { AgentType = 63; }
#endif
	}
};
