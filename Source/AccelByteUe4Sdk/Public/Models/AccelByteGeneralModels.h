// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteGeneralModels.generated.h"

UENUM(BlueprintType)
enum class EAccelBytePaginationType : uint8
{
	FIRST = 0,
	NEXT,
	PREVIOUS
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaging
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString First{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString Last{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString Next{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Paging")
	FString Previous{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | General | Models | Miscellaneous")
	FDateTime CurrentTime{0};
};