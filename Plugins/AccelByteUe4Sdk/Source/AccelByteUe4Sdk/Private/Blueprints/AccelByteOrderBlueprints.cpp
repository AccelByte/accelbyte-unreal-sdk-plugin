// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteOrderBlueprints.h"
#include "AccelByteSettings.h"
#include "AccelByteOrderApi.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Order;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsOrder::CreateNewOrder(const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::CreateNewOrder(OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrder(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::GetUserOrder(OrderNo, Order::FGetUserOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrders(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::GetUserOrders(Page, Size, Order::FGetUserOrdersSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfoPaging& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::FulfillOrder(const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::FulfillOrder(OrderNo, Order::FFulfillOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrderHistory(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::GetUserOrderHistory(OrderNo, Order::FGetUserOrderHistorySuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
