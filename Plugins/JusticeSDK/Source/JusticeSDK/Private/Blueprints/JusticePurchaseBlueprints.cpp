// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePurchaseBlueprints.h"
#include "JusticePurchase.h"

#pragma region CreateNewOrder
UCreateNewOrder * UCreateNewOrder::CreateNewOrder(int32 Quantity, FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency)
{
	UCreateNewOrder* Node = NewObject<UCreateNewOrder>();
	Node->Quantity = Quantity;
	Node->ItemID = ItemID;
	Node->Price = Price;
	Node->DiscountedPrice = DiscountedPrice;
	Node->Currency = Currency;
	return Node;
}

void UCreateNewOrder::Activate()
{
	JusticePurchase::CreateNewOrder(*FJusticeSDKModule::Get().UserToken.Get(), this->Quantity, this->ItemID, this->Price, this->DiscountedPrice, this->Currency, FOrderInfoCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, FOrderInfo* Order) {
		if (bSuccessful)
		{
			UOrderInfo* Result = UOrderInfo::Deserialize(Order);
			check(Result);
			OnSuccess.Broadcast(Result);
		}
		else
		{
			OnFailed.Broadcast(nullptr);
		}
	}));
}
#pragma endregion CreateNewOrder

#pragma region FulfillOrder
UFulfillOrder * UFulfillOrder::FulfillOrder(FString OrderNo)
{
	UFulfillOrder* Node = NewObject<UFulfillOrder>();
	Node->OrderNo = OrderNo;
	return Node;
}

void UFulfillOrder::Activate()
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
			OnFailed.Broadcast(nullptr);
		}
	}));
}
#pragma endregion FulfillOrder

#pragma region GetUserOrder
UGetUserOrder * UGetUserOrder::GetUserOrder(FString OrderNo)
{
	UGetUserOrder* Node = NewObject<UGetUserOrder>();
	Node->OrderNo = OrderNo;
	return Node;
}

void UGetUserOrder::Activate()
{
	JusticePurchase::GetUserOrder(this->OrderNo, FOrderInfoCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, FOrderInfo* Order) {
		if (bSuccessful)
		{
			UOrderInfo* Result = UOrderInfo::Deserialize(Order);
			check(Result);
			OnSuccess.Broadcast(Result);
		}
		else
		{
			OnFailed.Broadcast(nullptr);
		}
	}));
}
#pragma endregion GetUserOrder

#pragma region GetUserOrderHistory
UGetUserOrderHistory * UGetUserOrderHistory::GetUserOrderHistory(FString OrderNo)
{
	UGetUserOrderHistory* Node = NewObject<UGetUserOrderHistory>();
	Node->OrderNo = OrderNo;
	return Node;
}

void UGetUserOrderHistory::Activate()
{
	JusticePurchase::GetUserOrderHistory(this->OrderNo, FGetUserOrderHistoryCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FOrderHistoryInfo> OrderHistories) {
		TArray<UOrderHistoryInfo*> Result = TArray<UOrderHistoryInfo*>();
		if (bSuccessful)
		{
			for (int32 i = 0; i < OrderHistories.Num(); i++)
			{
				UOrderHistoryInfo* OrderHistory = UOrderHistoryInfo::Deserialize(OrderHistories[i]);
				check(OrderHistory);
				Result.Add(OrderHistory);
			}
			OnSuccess.Broadcast(Result);
		}
		else
		{
			OnFailed.Broadcast(TArray<UOrderHistoryInfo*>());
		}
	}));
}
#pragma endregion GetUserOrderHistory

#pragma region GetUserOrders
UGetUserOrders * UGetUserOrders::GetUserOrders(int32 Page, int32 Size)
{
	UGetUserOrders* Node = NewObject<UGetUserOrders>();
	Node->Page = Page;
	Node->Size = Size;
	return Node;
}

void UGetUserOrders::Activate()
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
			OnSuccess.Broadcast(FinalResult);
		}
		else
		{
			OnFailed.Broadcast(FinalResult);
		}
	}));
}
#pragma endregion GetUserOrders
