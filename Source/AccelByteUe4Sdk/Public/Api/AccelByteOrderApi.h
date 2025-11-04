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
class ACCELBYTEUE4SDK_API Order 
	: public FApiBase
	, public TSharedFromThis<Order, ESPMode::ThreadSafe>
{
public:
	Order(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	Order(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~Order();

	/**
	 * @brief Create order to purchase something from the store. 
	 * 
	 * @param OrderCreateRequest Requst parameter.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateNewOrder(FAccelByteModelsOrderCreate const& OrderCreateRequest
		, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Cancel Order after ord created with a specific Order number
	 *
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelOrder(FString const& OrderNo
		, THandler<FAccelByteModelsOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's order information. 
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserOrder(FString const& OrderNo
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
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserOrders(int32 Page
		, int32 Size
		, THandler<FAccelByteModelsPagedOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query all of user's orders that have been created with paging.
	 *
	 * @param UserOrderRequest The request to query user orders, contains some parameters.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsOrderInfoPaging.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserOrders(FAccelByteModelsUserOrdersRequest const& UserOrderRequest
		, THandler<FAccelByteModelsPagedOrderInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Order number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is TArray<FAccelByteModelsOrderHistoryInfo>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserOrderHistory(FString const& OrderNo
		, THandler<TArray<FAccelByteModelsOrderHistoryInfo>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Preview order price with discount code(s).
	 * 
	 * @param PreviewOrderRequest Preview order request.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * The result is FAccelByteModelsUserPreviewOrderResponse.
	 * @param OnError This will be called when the operation failed.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PreviewUserOrder(FAccelByteModelsUserPreviewOrderRequest const& PreviewOrderRequest
		, THandler<FAccelByteModelsUserPreviewOrderResponse> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	Order() = delete;
	Order(Order const&) = delete;
	Order(Order&&) = delete;
};

using OrderRef = TSharedRef<Order, ESPMode::ThreadSafe>;
using OrderPtr = TSharedPtr<Order, ESPMode::ThreadSafe>;
using OrderWPtr = TWeakPtr<Order, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
