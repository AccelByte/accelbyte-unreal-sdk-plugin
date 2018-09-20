// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Models/OrderInfo.h"
#include "Models/OrderInfoPaging.h"
#include "Models/OrderHistoryInfo.h"
#include "Models/OAuthTokenJustice.h"

DECLARE_DELEGATE_ThreeParams(FOrderInfoCompleteDelegate, bool, FString, FOrderInfo*);
DECLARE_DELEGATE_ThreeParams(FGetUserOrdersCompleteDelegate, bool, FString, TArray<FOrderInfo>);
DECLARE_DELEGATE_ThreeParams(FGetUserOrderHistoryCompleteDelegate, bool, FString, TArray<FOrderHistoryInfo>);


/**
 * @brief Justice purchasing service.
 * User should log in before using this. See JusticeIdentity to see how to log user in.
 */
class JUSTICESDK_API JusticePurchase
{

public:
	/**
	 * @brief Create order to purchase something from the store. 
	 * 
	 * @param Token  Required. User access token.
	 * @param Quantity Required.
	 * @param ItemID Required.
	 * @param Price Required.
	 * @param DiscountedPrice Required.
	 * @param Currency Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is OrderInfo, set in FOrderInfoCompleteDelegate callback.
	 */
	static void CreateNewOrder(FOAuthTokenJustice Token, int32 Quantity, FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency, FOrderInfoCompleteDelegate OnComplete);

	/**
	 * @brief Get user's order information. It requires the order's number (OrderNo) as parameter.
	 * 
	 * @param OrderNo Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is OrderInfo, set in FOrderInfoCompleteDelegate callback.
	 */
	static void GetUserOrder(FString OrderNo, FOrderInfoCompleteDelegate OnComplete);

	/**
	 * @brief Get all of user's orders that have been created. 
	 * 
	 * @param Page Optional.
	 * @param Size Optional.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is TArray<OrderInfo>, set in FGetUserOrdersCompleteDelegate callback.
	 */
	static void GetUserOrders(int32 Page, int32 Size, FGetUserOrdersCompleteDelegate OnComplete);

	/**
	 * @brief Fulfill user's order.
	 * 
	 * @param OrderNo Required.
	 * @param OnComplete OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is OrderInfo, set in FOrderInfoCompleteDelegate callback.
	 */
	static void FulfillOrder(FString OrderNo, FOrderInfoCompleteDelegate OnComplete);

	/**
	 * @brief  Get the history of the created orders.
	 * 
	 * @param OrderNo Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 * The result is TArray<OrderHistoryInfo>, set in FGetUserOrderHistoryCompleteDelegate callback.
	 */
	static void GetUserOrderHistory(FString OrderNo, FGetUserOrderHistoryCompleteDelegate OnComplete);
private:
	static void OnCreateNewOrderResponse(FJusticeResponsePtr Response, FOrderInfoCompleteDelegate OnComplete);
	static void OnGetUserOrderResponse(FJusticeResponsePtr Response, FOrderInfoCompleteDelegate OnComplete);
	static void OnGetUserOrdersResponse(FJusticeResponsePtr Response, FGetUserOrdersCompleteDelegate OnComplete);
	static void OnFulfillOrderResponse(FJusticeResponsePtr Response, FOrderInfoCompleteDelegate OnComplete);
	static void OnGetUserOrderHistory(FJusticeResponsePtr Response, FGetUserOrderHistoryCompleteDelegate OnComplete);
};
