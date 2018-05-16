// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/AWSXRayJustice.h"
#include "Models/OrderInfo.h"
#include "GetWalletBalance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetWalletBalanceOutputPin, const int, WalletBalance);

UCLASS()
class JUSTICESDK_API UAsyncGetWalletBalance : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetWalletBalanceOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetWalletBalanceOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Wallet"))
		static UAsyncGetWalletBalance* GetWalletBalance(FString Currency);

	virtual void Activate() override;
private:
	FString Currency;
};
	
	
	
