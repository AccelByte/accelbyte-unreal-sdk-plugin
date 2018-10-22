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
	static void CreateNewOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsOrderCreate& OrderCreateRequest, const FCreateNewOrderSuccess& OnSuccess, const FErrorHandler& OnError);
	static void CreateNewOrderEasy(const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Get user's order information. 
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	static void GetUserOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetUserOrderEasy(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrdersSuccess, const FAccelByteModelsOrderInfoPaging&);
	/**
	 * @brief Get all of user's orders that have been created with paging.
	 * 
	 * @param Page Page number.
	 * @param Size Items per page.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfoPaging.
	 */
	static void GetUserOrders(const FString& AccessToken, const FString& Namespace, const FString& UserId, int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetUserOrdersEasy(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FFulfillOrderSuccess, const FAccelByteModelsOrderInfo&);
	/**
	 * @brief Fulfill an order if the order is charged but the fulfillment fail.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsOrderInfo&.
	 */
	static void FulfillOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FErrorHandler& OnError);
	static void FulfillOrderEasy(const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetUserOrderHistorySuccess, const TArray<FAccelByteModelsOrderHistoryInfo>&);
	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<FAccelByteModelsOrderHistoryInfo>.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetUserOrderHistory(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetUserOrderHistoryEasy(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FErrorHandler& OnError);
private:
	Order() = delete; // static class can't have instance
	Order(Order const&) = delete;
	Order(Order&&) = delete;

	static void CreateNewOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateNewOrderSuccess OnSuccess, FErrorHandler OnError);
	static void GetUserOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrderSuccess OnSuccess, FErrorHandler OnError);
	static void GetUserOrdersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrdersSuccess OnSuccess, FErrorHandler OnError);
	static void FulfillOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FFulfillOrderSuccess OnSuccess, FErrorHandler OnError);
	static void GetUserOrderHistoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrderHistorySuccess OnSuccess, FErrorHandler OnError);
};

} // Namespace Api
} // Namespace AccelByte
