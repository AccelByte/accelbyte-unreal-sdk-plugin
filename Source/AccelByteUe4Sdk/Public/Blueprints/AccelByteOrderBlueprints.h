// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteOrderBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsOrder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateNewOrderSuccess, const FAccelByteModelsOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api ")
	static void CreateNewOrder(const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCancelOrderSuccess, const FAccelByteModelsOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api")
	static void CancelOrder(const FString& OrderNo, const FCancelOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserOrderSuccess, const FAccelByteModelsOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api ")
	static void GetUserOrder(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserOrdersSuccess, const FAccelByteModelsPagedOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api ")
	static void GetUserOrders(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserOrderHistorySuccess, const TArray<FAccelByteModelsOrderHistoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api ")
	static void GetUserOrderHistory(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};
