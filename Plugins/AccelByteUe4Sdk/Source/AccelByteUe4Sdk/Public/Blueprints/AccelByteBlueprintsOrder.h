// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsOrder.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteBlueprintsOrder.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsOrder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateNewOrderSuccess, const FAccelByteModelsOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void CreateNewOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsOrderCreate& OrderCreate, FCreateNewOrderSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void CreateNewOrderEasy(const FAccelByteModelsOrderCreate& OrderCreate, FCreateNewOrderSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserOrderSuccess, const FAccelByteModelsOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void GetUserOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void GetUserOrderEasy(FString OrderNo, FGetUserOrderSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserOrdersSuccess, const TArray<FAccelByteModelsOrderInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void GetUserOrders(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void GetUserOrdersEasy(int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FFulfillOrderSuccess, const FAccelByteModelsOrderInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void FulfillOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FFulfillOrderSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void FulfillOrderEasy(FString OrderNo, FFulfillOrderSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserOrderHistorySuccess, const TArray<FAccelByteModelsOrderHistoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void GetUserOrderHistory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Order")
	static void GetUserOrderHistoryEasy(FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, FBlueprintError OnError);

};
