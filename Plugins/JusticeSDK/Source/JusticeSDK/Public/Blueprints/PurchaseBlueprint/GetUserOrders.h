// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OrderInfo.h"
#include "Services/JusticePurchase.h"
#include "GetUserOrders.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserOrdersOutputPin, const TArray<UOrderInfo*>&, OrdersInformation);

UCLASS()
class JUSTICESDK_API UAsyncGetUserOrders : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetUserOrdersOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetUserOrdersOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Purchase"))
		static UAsyncGetUserOrders* GetUserOrders(int32 Page, int32 Size);

	virtual void Activate() override;
private:
	int32 Page;
	int32 Size;
};
	
	
	
