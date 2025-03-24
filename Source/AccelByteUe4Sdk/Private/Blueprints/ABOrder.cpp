// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABOrder.h"
#include "Api/AccelByteOrderApi.h"


using namespace AccelByte;

void UABOrder::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABOrder::CreateNewOrder(
	FAccelByteModelsOrderCreate const& OrderCreateRequest,
	FDAccelByteModelsOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto OrderPtr = ApiClientPtr->GetOrderApi().Pin();
	if (OrderPtr.IsValid())
	{
		OrderPtr->CreateNewOrder(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABOrder::CancelOrder(
	FString const& OrderNo,
	FDAccelByteModelsOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto OrderPtr = ApiClientPtr->GetOrderApi().Pin();
	if (OrderPtr.IsValid())
	{
		OrderPtr->CancelOrder(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABOrder::GetUserOrder(
	FString const& OrderNo,
	FDAccelByteModelsOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto OrderPtr = ApiClientPtr->GetOrderApi().Pin();
	if (OrderPtr.IsValid())
	{
		OrderPtr->GetUserOrder(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABOrder::GetUserOrders(
	int32 const& Page,
	int32 const& Size,
	FDAccelByteModelsPagedOrderInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto OrderPtr = ApiClientPtr->GetOrderApi().Pin();
	if (OrderPtr.IsValid())
	{
		OrderPtr->GetUserOrders(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABOrder::GetUserOrderHistory(
	FString const& OrderNo,
	FDArrayModelsOrderHistoryInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto OrderPtr = ApiClientPtr->GetOrderApi().Pin();
	if (OrderPtr.IsValid())
	{
		OrderPtr->GetUserOrderHistory(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABOrder::PreviewUserOrder(const FAccelByteModelsUserPreviewOrderRequest& OrderPreviewRequest,
	const FDAccelByteModelsPreviewOrderResponse& OnSuccess, const FDErrorHandler& OnError)
{
	const auto OrderPtr = ApiClientPtr->GetOrderApi().Pin();
	if (OrderPtr.IsValid())
	{
		OrderPtr->PreviewUserOrder(OrderPreviewRequest, THandler<FAccelByteModelsUserPreviewOrderResponse>::CreateLambda(
		[OnSuccess](const FAccelByteModelsUserPreviewOrderResponse& Result)
		{
			OnSuccess.ExecuteIfBound(Result);
		}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
