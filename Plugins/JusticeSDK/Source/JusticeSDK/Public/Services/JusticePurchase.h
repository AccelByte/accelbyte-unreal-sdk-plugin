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

DECLARE_DELEGATE_ThreeParams(FCreateNewOrderCompleteDelegate, bool, FString, OrderInfo);

class JUSTICESDK_API JusticePurchase
{

public:
	static void CreateNewOrder(FString itemId, int Price, int DiscountedPrice, FString Currency, FString StoreId, FCreateNewOrderCompleteDelegate OnComplete);
private:
	static void OnCreateNewOrderComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FCreateNewOrderCompleteDelegate OnComplete, FString itemId, int Price, int DiscountedPrice, FString Currency, FString StoreId);
};

