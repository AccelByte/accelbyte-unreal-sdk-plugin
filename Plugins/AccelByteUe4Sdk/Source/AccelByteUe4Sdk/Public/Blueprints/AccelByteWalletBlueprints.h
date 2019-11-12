// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Api/AccelByteWalletApi.h"
#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteWalletBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsWallet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetWalletByCurrencyCodeSuccess, const FAccelByteModelsWalletInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Wallet | Api ")
	static void GetWalletInfoByCurrencyCode(const FString& CurrencyCode, const FGetWalletByCurrencyCodeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};
