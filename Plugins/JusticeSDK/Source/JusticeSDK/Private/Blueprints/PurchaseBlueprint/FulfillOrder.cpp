// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "FulfillOrder.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Models/OrderInfo.h"
#include "JusticeSDK.h"
#include "JusticePurchase.h"

UAsyncFulfillOrder * UAsyncFulfillOrder::FulfillOrder(FString OrderNo)
{
	UAsyncFulfillOrder* Node = NewObject<UAsyncFulfillOrder>();
	Node->OrderNo = OrderNo;
	return Node;
}

void UAsyncFulfillOrder::Activate()
{
	JusticePurchase::FulfillOrder(this->OrderNo, FOrderInfoCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, FOrderInfo* Order) {
		if (bSuccessful)
		{			
			UOrderInfo* Result = UOrderInfo::Deserialize(Order);
			check(Result);
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(Result);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(nullptr);
			}
		}
	}));
}