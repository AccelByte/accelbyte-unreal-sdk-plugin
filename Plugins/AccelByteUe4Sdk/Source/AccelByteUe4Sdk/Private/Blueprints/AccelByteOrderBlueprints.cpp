// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteOrderBlueprints.h"
#include "AccelByteSettings.h"
#include "AccelByteOrderApi.h"
#include "AccelByteCredentials.h"
#include "AccelByteRegistry.h"

using AccelByte::Api::Order;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsOrder::CreateNewOrder(const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Order.CreateNewOrder(OrderCreate, AccelByte::THandler<FAccelByteModelsOrderInfo>::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrder(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Order.GetUserOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrders(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Order.GetUserOrders(Page, Size, THandler<FAccelByteModelsOrderInfoPaging>::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfoPaging& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::FulfillOrder(const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Order.FulfillOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrderHistory(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Order.GetUserOrderHistory(OrderNo, THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
