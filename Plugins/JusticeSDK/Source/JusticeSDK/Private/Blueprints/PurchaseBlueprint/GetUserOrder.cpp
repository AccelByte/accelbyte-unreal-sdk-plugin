// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GetUserOrder.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Models/OrderInfo.h"
#include "JusticeSDK.h"
#include "JusticePurchase.h"

UAsyncGetUserOrder * UAsyncGetUserOrder::GetUserOrder(FString OrderNo)
{
	UAsyncGetUserOrder* Node = NewObject<UAsyncGetUserOrder>();
	Node->OrderNo = OrderNo;
	return Node;
}

void UAsyncGetUserOrder::Activate()
{
	JusticePurchase::GetUserOrder(this->OrderNo, FOrderInfoCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, FOrderInfo Order) {
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