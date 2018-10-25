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

void UAccelByteBlueprintsOrder::CreateNewOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::CreateNewOrder(AccessToken, Namespace, UserId, OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::CreateNewOrderEasy(const FAccelByteModelsOrderCreate & OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	CreateNewOrder(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderCreate, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::GetUserOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::GetUserOrder(AccessToken, Namespace, UserId, OrderNo, Order::FGetUserOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrderEasy(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetUserOrder(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::GetUserOrders(const FString& AccessToken, const FString& Namespace, const FString& UserId, int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::GetUserOrders(AccessToken, Namespace, UserId, Page, Size, Order::FGetUserOrdersSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfoPaging& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrdersEasy(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetUserOrders(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Page, Size, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::FulfillOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::FulfillOrder(AccessToken, Namespace, UserId, OrderNo, Order::FFulfillOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::FulfillOrderEasy(const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FulfillOrder(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::GetUserOrderHistory(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Order::GetUserOrderHistory(AccessToken, Namespace, UserId, OrderNo, Order::FGetUserOrderHistorySuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrderHistoryEasy(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetUserOrderHistory(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, OnSuccess, OnError);
}
