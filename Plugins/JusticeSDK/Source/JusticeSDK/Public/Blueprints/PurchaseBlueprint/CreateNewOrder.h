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
#include "CreateNewOrder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateNewOrderOutputPin, const UOrderInfo*, OrderInformation);

UCLASS()
class JUSTICESDK_API UAsyncCreateNewOrder : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FCreateNewOrderOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FCreateNewOrderOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Purchase"))
		static UAsyncCreateNewOrder* CreateNewOrder(FString itemId, int Price, FString Currency);

	virtual void Activate() override;
private:
	FString ItemId;
	int Price;
	FString Currency;
};
	
	
	
