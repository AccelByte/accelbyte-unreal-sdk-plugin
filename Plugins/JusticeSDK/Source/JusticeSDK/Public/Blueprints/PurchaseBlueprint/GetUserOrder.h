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
#include "GetUserOrder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserOrderOutputPin, const UOrderInfo*, OrderInformation);

UCLASS()
class JUSTICESDK_API UAsyncGetUserOrder : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetUserOrderOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetUserOrderOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Purchase"))
		static UAsyncGetUserOrder* GetUserOrder(FString OrderNo);

	virtual void Activate() override;
private:
	FString OrderNo;
};
	
	
	
