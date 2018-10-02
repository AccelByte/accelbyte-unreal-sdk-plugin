// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsError.generated.h"

USTRUCT(BlueprintType)
struct FAccelByteModelsErrorEntity
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		int32 NumericErrorCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString ErrorCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString ErrorMessage;
};
