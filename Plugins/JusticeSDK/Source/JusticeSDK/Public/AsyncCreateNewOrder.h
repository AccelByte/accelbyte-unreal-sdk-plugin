// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Private/HTTPJustice.h"
#include "Private/AWSXRayJustice.h"
#include "Private/Models/OrderInfo.h"
#include "AsyncCreateNewOrder.generated.h"

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

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "ServerList"))
		static UAsyncCreateNewOrder* CreateNewOrder(FString itemId, int Price, FString Currency);

	virtual void Activate() override;

private:
	UFUNCTION()
		void ExecuteOnSuccess();

	UFUNCTION()
		void ExecuteOnFailed();

	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace);

private:
	FString ItemId;
	int Price;
	FString Currency;
	UOrderInfo* OrderInformation;
};
	
	
	
