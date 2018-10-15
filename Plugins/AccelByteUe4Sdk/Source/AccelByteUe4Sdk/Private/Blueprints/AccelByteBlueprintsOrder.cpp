// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsOrder.h"
#include "AccelByteSettings.h"
#include "AccelByteApiOrder.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Order;
using AccelByte::ErrorDelegate;
using AccelByte::Settings;
using AccelByte::CredentialStore;

void UAccelByteBlueprintsOrder::CreateNewOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsOrderCreate& OrderCreate, FCreateNewOrderSuccess OnSuccess, FBlueprintError OnError)
{
	Order::CreateNewOrder(ServerBaseUrl, AccessToken, Namespace, UserId, OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::CreateNewOrderEasy(const FAccelByteModelsOrderCreate & OrderCreate, FCreateNewOrderSuccess OnSuccess, FBlueprintError OnError)
{
	CreateNewOrder(Settings::ServerBaseUrl, CredentialStore.GetUserAccessToken(), CredentialStore.GetUserNamespace(), CredentialStore.GetUserId(), OrderCreate, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::GetUserOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderSuccess OnSuccess, FBlueprintError OnError)
{
	Order::GetUserOrder(ServerBaseUrl, AccessToken, Namespace, UserId, OrderNo, Order::FGetUserOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrderEasy(FString OrderNo, FGetUserOrderSuccess OnSuccess, FBlueprintError OnError)
{
	GetUserOrder(Settings::ServerBaseUrl, CredentialStore.GetUserAccessToken(), CredentialStore.GetUserNamespace(), CredentialStore.GetUserId(), OrderNo, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::GetUserOrders(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, FBlueprintError OnError)
{
	Order::GetUserOrders(ServerBaseUrl, AccessToken, Namespace, UserId, Page, Size, Order::FGetUserOrdersSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfoPaging& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrdersEasy(int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, FBlueprintError OnError)
{
	GetUserOrders(Settings::ServerBaseUrl, CredentialStore.GetUserAccessToken(), CredentialStore.GetUserNamespace(), CredentialStore.GetUserId(), Page, Size, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::FulfillOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FFulfillOrderSuccess OnSuccess, FBlueprintError OnError)
{
	Order::FulfillOrder(ServerBaseUrl, AccessToken, Namespace, UserId, OrderNo, Order::FFulfillOrderSuccess::CreateLambda([OnSuccess](const FAccelByteModelsOrderInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::FulfillOrderEasy(FString OrderNo, FFulfillOrderSuccess OnSuccess, FBlueprintError OnError)
{
	FulfillOrder(Settings::ServerBaseUrl, CredentialStore.GetUserAccessToken(), CredentialStore.GetUserNamespace(), CredentialStore.GetUserId(), OrderNo, OnSuccess, OnError);
}

void UAccelByteBlueprintsOrder::GetUserOrderHistory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, FBlueprintError OnError)
{
	Order::GetUserOrderHistory(ServerBaseUrl, AccessToken, Namespace, UserId, OrderNo, Order::FGetUserOrderHistorySuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsOrder::GetUserOrderHistoryEasy(FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, FBlueprintError OnError)
{
	GetUserOrderHistory(Settings::ServerBaseUrl, CredentialStore.GetUserAccessToken(), CredentialStore.GetUserNamespace(), CredentialStore.GetUserId(), OrderNo, OnSuccess, OnError);
}
