// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "JusticePlatformFunctions.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Models/OrderInfo.h"
#include "Models/OrderInfoPaging.h"
#include "Models/OrderHistoryInfo.h"

DECLARE_DELEGATE_ThreeParams(FOrderInfoCompleteDelegate, bool, FString, FOrderInfo*);
DECLARE_DELEGATE_ThreeParams(FGetUserOrdersCompleteDelegate, bool, FString, TArray<FOrderInfo>);
DECLARE_DELEGATE_ThreeParams(FGetUserOrderHistoryCompleteDelegate, bool, FString, TArray<FOrderHistoryInfo>);

class JUSTICESDK_API JusticePurchase
{

public:
	static void CreateNewOrder(FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency, FOrderInfoCompleteDelegate OnComplete);
	static void GetUserOrder(FString OrderNo, FOrderInfoCompleteDelegate OnComplete);
	static void GetUserOrders(int32 Page, int32 Size, FGetUserOrdersCompleteDelegate OnComplete);
	static void FulfillOrder(FString OrderNo, FOrderInfoCompleteDelegate OnComplete);
	static void GetUserOrderHistory(FString OrderNo, FGetUserOrderHistoryCompleteDelegate OnComplete);
private:
	static void OnCreateNewOrderResponse(FJusticeResponsePtr Response, FOrderInfoCompleteDelegate OnComplete);
	static void OnGetUserOrderResponse(FJusticeResponsePtr Response, FOrderInfoCompleteDelegate OnComplete);
	static void OnGetUserOrdersResponse(FJusticeResponsePtr Response, FGetUserOrdersCompleteDelegate OnComplete);
	static void OnFulfillOrderResponse(FJusticeResponsePtr Response, FOrderInfoCompleteDelegate OnComplete);
	static void OnGetUserOrderHistory(FJusticeResponsePtr Response, FGetUserOrderHistoryCompleteDelegate OnComplete);
};
