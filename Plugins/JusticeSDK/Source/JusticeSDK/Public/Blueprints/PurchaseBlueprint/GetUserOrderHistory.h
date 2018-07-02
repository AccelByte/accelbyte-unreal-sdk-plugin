// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OrderHistoryInfo.h"
#include "GetUserOrderHistory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserOrderHistoryOutputPin, const TArray<UOrderHistoryInfo*>&, OrderHistory);

UCLASS()
class JUSTICESDK_API UAsyncGetUserOrderHistory : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetUserOrderHistoryOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetUserOrderHistoryOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Purchase"))
		static UAsyncGetUserOrderHistory* GetUserOrderHistory(FString OrderNo);

	virtual void Activate() override;
private:
	FString OrderNo;
};
	
	
	
