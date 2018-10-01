// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsCategory.generated.h"

USTRUCT(BlueprintType)
struct FAccelByteModelsFullCategoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString ParentCategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString CategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		FString UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Category | FullCategoryInfo")
		bool Root;
};
