// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteCurrencyBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCurrency : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetCurrencyListSuccess, const TArray<FAccelByteModelsCurrencyList>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Currency | Api")
	static void GetCurrencyList(const FString& Namespace, const FGetCurrencyListSuccess& OnSuccess, const FBlueprintErrorHandler& OnError,
		EAccelByteCurrencyType CurrencyType);
};