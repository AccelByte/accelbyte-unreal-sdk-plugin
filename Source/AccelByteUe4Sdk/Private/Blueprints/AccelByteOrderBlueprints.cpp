// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteOrderBlueprints.h"
#include "Core/AccelByteSettings.h"
#include "Api/AccelByteOrderApi.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"

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

void UAccelByteBlueprintsOrder::CancelOrder(const FString& OrderNo, const FCancelOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Order.CancelOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
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
	FRegistry::Order.GetUserOrders(Page, Size, THandler<FAccelByteModelsPagedOrderInfo>::CreateLambda([OnSuccess](const FAccelByteModelsPagedOrderInfo& Result)
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
