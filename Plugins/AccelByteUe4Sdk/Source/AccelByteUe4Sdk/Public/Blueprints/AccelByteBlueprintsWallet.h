// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteApiWallet.h"
#include "AccelByteModelsWallet.h"
#include "AccelByteBlueprintsWallet.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsWallet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetWalletByCurrencyCodeSuccess, const FAccelByteModelsWalletInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Wallet")
	static void GetWalletInfoByCurrencyCode(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString CurrencyCode, FGetWalletByCurrencyCodeSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Wallet")
	static void GetWalletInfoByCurrencyCodeEasy(FString CurrencyCode, FGetWalletByCurrencyCodeSuccess OnSuccess, FBlueprintError OnError);
};
