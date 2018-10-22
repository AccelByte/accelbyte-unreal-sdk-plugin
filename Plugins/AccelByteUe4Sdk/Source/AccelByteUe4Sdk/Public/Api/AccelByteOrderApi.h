// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "AccelByteError.h"
#include "AccelByteOrderModels.h"

namespace AccelByte
{
namespace Api
{

/**
 * @brief Order is used to purchase something from the online store.
 */
class ACCELBYTEUE4SDK_API Order
{
public:
	DECLARE_DELEGATE_OneParam(FCreateNewOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Create order to purchase something from the store. 
	 * 
	 * @param OrderCreateRequest Requst parameter.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	static void CreateNewOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsOrderCreate& OrderCreateRequest, FCreateNewOrderSuccess OnSuccess, FErrorDelegate OnError);
	static void CreateNewOrderEasy(const FAccelByteModelsOrderCreate& OrderCreate, FCreateNewOrderSuccess OnSuccess, FErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Get user's order information. 
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	static void GetUserOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderSuccess OnSuccess, FErrorDelegate OnError);
	static void GetUserOrderEasy(FString OrderNo, FGetUserOrderSuccess OnSuccess, FErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrdersSuccess, const FAccelByteModelsOrderInfoPaging&);
	/**
	 * @brief Get all of user's orders that have been created with paging.
	 * 
	 * @param Page Page number.
	 * @param Size Items per page.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfoPaging.
	 */
	static void GetUserOrders(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, FErrorDelegate OnError);
	static void GetUserOrdersEasy(int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, FErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FFulfillOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Fulfill an order if the order is charged but the fulfillment fail.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsOrderInfo&.
	 */
	static void FulfillOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FFulfillOrderSuccess OnSuccess, FErrorDelegate OnError);
	static void FulfillOrderEasy(FString OrderNo, FFulfillOrderSuccess OnSuccess, FErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrderHistorySuccess, const TArray<FAccelByteModelsOrderHistoryInfo>&);
	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<FAccelByteModelsOrderHistoryInfo>.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetUserOrderHistory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, FErrorDelegate OnError);
	static void GetUserOrderHistoryEasy(FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, FErrorDelegate OnError);
private:
	Order() = delete; // static class can't have instance
	Order(Order const&) = delete;
	Order(Order&&) = delete;

	static void CreateNewOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateNewOrderSuccess OnSuccess, FErrorDelegate OnError);
	static void GetUserOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrderSuccess OnSuccess, FErrorDelegate OnError);
	static void GetUserOrdersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrdersSuccess OnSuccess, FErrorDelegate OnError);
	static void FulfillOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FFulfillOrderSuccess OnSuccess, FErrorDelegate OnError);
	static void GetUserOrderHistoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrderHistorySuccess OnSuccess, FErrorDelegate OnError);
};

} // Namespace Api
} // Namespace AccelByte
