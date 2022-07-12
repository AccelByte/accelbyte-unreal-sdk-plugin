// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABUtilities.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UABUtilities : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static FString DateTimeToString(const FDateTime& DateTime);
};
