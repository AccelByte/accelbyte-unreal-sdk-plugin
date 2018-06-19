// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JusticeWalletFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGetWalletCompleteDelegate, bool, IsSuccess, int32, Balance);

UCLASS()
class JUSTICESDK_API UJusticeWalletFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
	UFUNCTION(BlueprintCallable, Category = "JusticeWalletFunctions")
	static void GetWalletBalance(FString CurrencyCode, FGetWalletCompleteDelegate OnComplete);
	
};
