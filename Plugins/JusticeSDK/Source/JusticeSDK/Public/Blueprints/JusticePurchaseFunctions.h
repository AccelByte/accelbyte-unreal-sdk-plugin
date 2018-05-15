// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/OrderInfo.h"
#include "JusticePurchaseFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FCreateNewOrderCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr, UOrderInfo*, OrderInformation);

UCLASS()
class JUSTICESDK_API UJusticePurchaseFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, Category = "JusticePurchaseFunctions")
		static void CreateNewOrder(FString itemId, int Price, FString Currency, FCreateNewOrderCompleteDynamicDelegate OnComplete);

};
