// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteMetricModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FStatsDMetric
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Models | StatsDMetric")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Models | StatsDMetric")
	FString Value{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Models | StatsDMetric")
	TArray<FString> Tags{};
};