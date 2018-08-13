// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GetUserOrders.h"

UAsyncGetUserOrders * UAsyncGetUserOrders::GetUserOrders(int32 Page, int32 Size)
{
	UAsyncGetUserOrders* Node = NewObject<UAsyncGetUserOrders>();
	Node->Page = Page;
	Node->Size = Size;
	return Node;
}

void UAsyncGetUserOrders::Activate()
{
	JusticePurchase::GetUserOrders(this->Page, this->Size, FGetUserOrdersCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FOrderInfo> Orders) {
		TArray<UOrderInfo*> FinalResult;
		if (bSuccessful)
		{
			for (int32 i = 0; i < Orders.Num(); i++)
			{
				UOrderInfo* Order = UOrderInfo::Deserialize(Orders[i]);
				check(Order);
				FinalResult.Add(Order);
			}
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(FinalResult);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(FinalResult);
			}
		}
	}));
}