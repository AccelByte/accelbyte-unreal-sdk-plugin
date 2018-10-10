// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "AccelByteError.h"
#include "AccelByteModelsOrder.h"

namespace AccelByte
{
namespace Api
{

/**
 * @brief AccelByte purchasing service.
 * User should log in before using this. See AccelByteServicesUser to see how to log user in.
 */
class ACCELBYTEUE4SDK_API Order
{
public:
	DECLARE_DELEGATE_OneParam(FCreateNewOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Create order to purchase something from the store. 
	 * 
	 * @param Token  User access token.
	 * @param Quantity Required.
	 * @param ItemId Required.
	 * @param Price Required.
	 * @param DiscountedPrice Required.
	 * @param Currency Required.
	 * @param ReturnUrl Required valid URL.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is OrderInfo, set in FOrderInfoCompleteDelegate callback.
	 */
	static void CreateNewOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsOrderCreate& OrderCreate, FCreateNewOrderSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Get user's order information. It requires the order's number (OrderNo) as parameter.
	 * 
	 * @param OrderNo Required.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is OrderInfo, set in FOrderInfoCompleteDelegate callback.
	 */
	static void GetUserOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrdersSuccess, const FAccelByteModelsOrderInfoPaging&);
	/**
	 * @brief Get all of user's orders that have been created. 
	 * 
	 * @param Page Optional.
	 * @param Size Optional.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<OrderInfo>, set in FGetUserOrdersCompleteDelegate callback.
	 */
	static void GetUserOrders(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FFulfillOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Fulfill user's order.
	 * 
	 * @param OrderNo Required.
	 * @param OnSuccess OnSuccess This will be called when the operation succeeded.
	 * The result is OrderInfo, set in FOrderInfoCompleteDelegate callback.
	 */
	static void FulfillOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FFulfillOrderSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrderHistorySuccess, const TArray<FAccelByteModelsOrderHistoryInfo>&);
	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Required.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<OrderHistoryInfo>, set in FGetUserOrderHistoryCompleteDelegate callback.
	 */
	static void GetUserOrderHistory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, ErrorDelegate OnError);
private:
	static void CreateNewOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateNewOrderSuccess OnSuccess, ErrorDelegate OnError);
	static void GetUserOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserOrderSuccess OnSuccess, ErrorDelegate OnError);
	static void GetUserOrdersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserOrdersSuccess OnSuccess, ErrorDelegate OnError);
	static void FulfillOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FFulfillOrderSuccess OnSuccess, ErrorDelegate OnError);
	static void GetUserOrderHistoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserOrderHistorySuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Api
} // Namespace AccelByte
