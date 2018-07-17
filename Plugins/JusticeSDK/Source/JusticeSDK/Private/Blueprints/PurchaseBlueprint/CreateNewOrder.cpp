// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "CreateNewOrder.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Models/OrderCreate.h"
#include "JusticeSDK.h"
#include "JusticePurchase.h"

UAsyncCreateNewOrder * UAsyncCreateNewOrder::CreateNewOrder(FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency)
{
	UAsyncCreateNewOrder* Node = NewObject<UAsyncCreateNewOrder>();
	Node->ItemID = ItemID;
	Node->Price = Price;
	Node->DiscountedPrice = DiscountedPrice;
	Node->Currency = Currency;
	return Node;
}

void UAsyncCreateNewOrder::Activate() 
{
	JusticePurchase::CreateNewOrder(this->ItemID, this->Price, this->DiscountedPrice, this->Currency, FCreateNewOrderCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, OrderInfo Order) {
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