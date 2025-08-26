// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteAMSModels.generated.h"

/** @brief Data Model for fleet limits */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAMSFleetLimits
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | AMS | Models")
	int32 FleetCount{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | AMS | Models")
	int32 FleetVmCount{0};
};

/** @brief Data Model for get AMS account details */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAMSGetAccountResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | AMS | Models")
	FAccelByteModelsAMSFleetLimits Limits{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | AMS | Models")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | AMS | Models")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | AMS | Models")
	TArray<FString> Namespaces{};
};
