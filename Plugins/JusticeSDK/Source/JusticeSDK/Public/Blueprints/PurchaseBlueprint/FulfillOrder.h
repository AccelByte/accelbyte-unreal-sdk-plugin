// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/AWSXRayJustice.h"
#include "Models/OrderInfo.h"
#include "FulfillOrder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFulfillOrderOutputPin, const UOrderInfo*, OrderInformation);

UCLASS()
class JUSTICESDK_API UAsyncFulfillOrder : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FFulfillOrderOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FFulfillOrderOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Purchase"))
		static UAsyncFulfillOrder* FulfillOrder(FString OrderNo);

	virtual void Activate() override;
private:
	FString OrderNo;
};
	
	
	
