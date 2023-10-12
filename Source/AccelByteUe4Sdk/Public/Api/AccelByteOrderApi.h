// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Order is used to purchase something from the online store.
 */
class ACCELBYTEUE4SDK_API Order : public FApiBase
{
public:
	Order(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Order();

	/**
	 * @brief Create order to purchase something from the store. 
	 * 
	 * @param OrderCreateRequest Requst parameter.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	void CreateNewOrder(FAccelByteModelsOrderCreate const& OrderCreateRequest
		, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Cancel Order after ord created with a specific Order number
	 *
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void CancelOrder(FString const& OrderNo
		, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's order information. 
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed. 
	 */
	void GetUserOrder(FString const& OrderNo
		, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get all of user's orders that have been created with paging.
	 * @obsolete This method will be removed in the future, so please use QueryUserOrders(const FAccelByteModelsUserOrdersRequest& UserOrderRequest, 
	 *	  const THandler<FAccelByteModelsPagedOrderInfo>& OnSuccess, const FErrorHandler& OnError);
	 * 
	 * @param Page Page number.
	 * @param Size Items per page.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfoPaging.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserOrders(int32 Page
		, int32 Size
		, THandler<FAccelByteModelsPagedOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query all of user's orders that have been created with paging.
	 *
	 * @param UserOrderRequest The request to query user orders, contains some parameters.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfoPaging.
	 * @param OnError This will be called when the operation failed.
     */
	void QueryUserOrders(const FAccelByteModelsUserOrdersRequest& UserOrderRequest
		, const THandler<FAccelByteModelsPagedOrderInfo>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<FAccelByteModelsOrderHistoryInfo>.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserOrderHistory(FString const& OrderNo
		, THandler<TArray<FAccelByteModelsOrderHistoryInfo>> const& OnSuccess
		, FErrorHandler const& OnError);
private:
	Order() = delete;
	Order(Order const&) = delete;
	Order(Order&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
