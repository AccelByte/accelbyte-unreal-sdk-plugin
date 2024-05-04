// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteQosModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsQosServer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServer")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServer")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServer")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServer")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServer")
	FString Last_update{};

	/**
	 * Cached resolved IP for this Qos server. Only used for AMS Qos where the 'Ip' field is a domain name.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServer")
	FString ResolvedIp{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsQosServerList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosServerList")
	TArray<FAccelByteModelsQosServer> Servers{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsQosRegionLatency
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosRegionLatency")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosRegionLatency")
	float Latency{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsQosRegionLatencies
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Qos | Models | QosRegionLatency")
	TArray<FAccelByteModelsQosRegionLatency> Data{};

	FString GetClosestRegion()
	{
		float LowestPing = TNumericLimits<float>::Max();
		FString ClosestRegion{};

		for (const auto& RegionLatency : Data)
		{
			if (RegionLatency.Latency < LowestPing)
			{
				ClosestRegion = RegionLatency.Region;
				LowestPing = RegionLatency.Latency;
			}
		}

		return ClosestRegion;
	}
};
