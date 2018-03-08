// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "JusticeBlueprintFunction.generated.h"

UCLASS()
class ONLINESUBSYSTEMJUSTICE_API UJusticeBlueprintFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "JusticeBlueprintFunction")
		static UFJusticeComponent* GetJusticeComponent();
};

