// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GetUserOrderHistory.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Models/OrderHistoryInfo.h"
#include "JusticeSDK.h"
#include "JusticePurchase.h"

UAsyncGetUserOrderHistory * UAsyncGetUserOrderHistory::GetUserOrderHistory(FString OrderNo)
{
	UAsyncGetUserOrderHistory* Node = NewObject<UAsyncGetUserOrderHistory>();
	Node->OrderNo = OrderNo;
	return Node;
}

void UAsyncGetUserOrderHistory::Activate()
{
	JusticePurchase::GetUserOrderHistory(this->OrderNo, FGetUserOrderHistoryCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<OrderHistoryInfo> OrderHistories) {
		TArray<UOrderHistoryInfo*> Result = TArray<UOrderHistoryInfo*>();
		if (bSuccessful)
		{	
			if (OnSuccess.IsBound())
			{
				for (int32 i = 0; i < OrderHistories.Num(); i++)
				{
					UOrderHistoryInfo* OrderHistory = UOrderHistoryInfo::Deserialize(OrderHistories[i]);
					check(OrderHistory);
					Result.Add(OrderHistory);
				}
				OnSuccess.Broadcast(Result);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(TArray<UOrderHistoryInfo*>());
			}
		}
	}));
}