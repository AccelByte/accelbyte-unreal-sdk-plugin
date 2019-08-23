// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteOrderModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Order is used to purchase something from the online store.
 */
class ACCELBYTEUE4SDK_API Order
{
public:
	Order(const Credentials& Credentials, const Settings& Settings);
	~Order();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	 * @brief Create order to purchase something from the store. 
	 * 
	 * @param OrderCreateRequest Requst parameter.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	void CreateNewOrder(const FAccelByteModelsOrderCreate& OrderCreateRequest, const THandler<FAccelByteModelsOrderInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get user's order information. 
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	void GetUserOrder(const FString& OrderNo, const THandler<FAccelByteModelsOrderInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get all of user's orders that have been created with paging.
	 * 
	 * @param Page Page number.
	 * @param Size Items per page.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfoPaging.
	 */
	void GetUserOrders(int32 Page, int32 Size, const THandler<FAccelByteModelsOrderInfoPaging>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Fulfill an order if the order is charged but the fulfillment fail.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsOrderInfo&.
	 */
	void FulfillOrder(const FString& OrderNo, const THandler<FAccelByteModelsOrderInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<FAccelByteModelsOrderHistoryInfo>.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserOrderHistory(const FString& OrderNo, const THandler<TArray<FAccelByteModelsOrderHistoryInfo>>& OnSuccess, const FErrorHandler& OnError);
private:
	Order() = delete;
	Order(Order const&) = delete;
	Order(Order&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
