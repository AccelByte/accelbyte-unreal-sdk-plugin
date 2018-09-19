// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "JusticeWalletBlueprints.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetWalletBalanceOutputPin, const int32, WalletBalance);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetWalletBalance : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetWalletBalanceOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetWalletBalanceOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Wallet"))
		static UGetWalletBalance* GetWalletBalance(FString Currency);
	virtual void Activate() override;
private:
	FString Currency;
};