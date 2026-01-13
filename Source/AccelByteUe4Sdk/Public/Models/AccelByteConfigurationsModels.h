// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteConfigurationsModels.generated.h"

/** @brief Data Model for backend public configuration */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Configurations | Models")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Configurations | Models")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Configurations | Models")
	FString Value{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Configurations | Models")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Configurations | Models")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Configurations | Models")
	bool IsPublic{false};
};
