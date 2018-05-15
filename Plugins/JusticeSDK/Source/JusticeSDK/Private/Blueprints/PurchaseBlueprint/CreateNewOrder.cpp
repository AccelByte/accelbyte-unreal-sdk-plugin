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

UAsyncCreateNewOrder * UAsyncCreateNewOrder::CreateNewOrder(FString itemId, int Price, FString Currency)
{
	UAsyncCreateNewOrder* Node = NewObject<UAsyncCreateNewOrder>();
	Node->ItemId = itemId;
	Node->Price = Price;
	Node->Currency = Currency;
	return Node;
}

void UAsyncCreateNewOrder::Activate() 
{
	JusticePurchase::CreateNewOrder(this->ItemId, this->Price, this->Currency, FCreateNewOrderCompleteDelegate::CreateLambda([&](bool isSuccess, FString errorString, OrderInfo order) {
		if (isSuccess)
		{
			UOrderInfo* result = NewObject<UOrderInfo>();
			result->FromOrderInfo(order);
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(result);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(NewObject<UOrderInfo>());
			}
		}
	}));
}