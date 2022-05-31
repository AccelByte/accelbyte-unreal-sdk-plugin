// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABOrder.h"
#include "Api/AccelByteOrderApi.h"
#include "Core/AccelByteRegistry.h"

void UABOrder::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABOrder::CreateNewOrder(
	FAccelByteModelsOrderCreate const& OrderCreateRequest,
	FDAccelByteModelsOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Order.CreateNewOrder(
		OrderCreateRequest,
		THandler<FAccelByteModelsOrderInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsOrderInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABOrder::CancelOrder(
	FString const& OrderNo,
	FDAccelByteModelsOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Order.CancelOrder(
		OrderNo,
		THandler<FAccelByteModelsOrderInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsOrderInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABOrder::GetUserOrder(
	FString const& OrderNo,
	FDAccelByteModelsOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Order.GetUserOrder(
		OrderNo,
		THandler<FAccelByteModelsOrderInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsOrderInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABOrder::GetUserOrders(
	int32 const& Page,
	int32 const& Size,
	FDAccelByteModelsPagedOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Order.GetUserOrders(
		Page,
		Size,
		THandler<FAccelByteModelsPagedOrderInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsPagedOrderInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABOrder::GetUserOrderHistory(
	FString const& OrderNo,
	FDArrayModelsOrderHistoryInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Order.GetUserOrderHistory(
		OrderNo,
		THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsOrderHistoryInfo> const& Response)
			{
				FArrayModelsOrderHistoryInfo Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
