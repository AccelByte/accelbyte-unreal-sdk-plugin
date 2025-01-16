// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABStatistic.h"

using namespace AccelByte;

void UABStatistic::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABStatistic::CreateUserStatItems(
	TArray<FString> const& StatCodes,
	FDArrayModelsBulkStatItemOperationResultResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto StatisticPtr = ApiClientPtr->GetStatisticApi().Pin();
	if (StatisticPtr.IsValid())
	{
		StatisticPtr->CreateUserStatItems(
			StatCodes,
			THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda(
				[OnSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> const& Response)
				{
					FArrayModelsBulkStatItemOperationResultResponse Result;
					Result.Content = Response;
					OnSuccess.ExecuteIfBound(Result);
				}
			),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			)
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABStatistic::GetAllUserStatItems(
	FDModelsUserStatItemPagingSlicedResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset,
	EAccelByteStatisticSortBy SortBy
)
{
	const auto StatisticPtr = ApiClientPtr->GetStatisticApi().Pin();
	if (StatisticPtr.IsValid())
	{
		StatisticPtr->GetAllUserStatItems(
		THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserStatItemPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Limit,
		Offset,
		SortBy
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABStatistic::GetUserStatItems(
	TArray<FString> const& StatCodes,
	TArray<FString> const& Tags,
	FDModelsUserStatItemPagingSlicedResultResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset,
	EAccelByteStatisticSortBy SortBy
)
{
	const auto StatisticPtr = ApiClientPtr->GetStatisticApi().Pin();
	if (StatisticPtr.IsValid())
	{
		StatisticPtr->GetUserStatItems(
		StatCodes,
		Tags,
		THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserStatItemPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Limit,
		Offset,
		SortBy
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABStatistic::IncrementUserStatItems(
	TArray<FAccelByteModelsBulkStatItemInc> const& Data,
	FDArrayModelsBulkStatItemOperationResultResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto StatisticPtr = ApiClientPtr->GetStatisticApi().Pin();
	if (StatisticPtr.IsValid())
	{
		StatisticPtr->IncrementUserStatItems(
		Data,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> const& Response)
			{
				FArrayModelsBulkStatItemOperationResultResponse Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABStatistic::GetGlobalStatItemsByStatCode(
	FString const& StatCode,
	FDModelsGlobalStatItemDelegate const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto StatisticPtr = ApiClientPtr->GetStatisticApi().Pin();
	if (StatisticPtr.IsValid())
	{
		StatisticPtr->GetGlobalStatItemsByStatCode(
		StatCode,
		THandler<FAccelByteModelsGlobalStatItemValueResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsGlobalStatItemValueResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABStatistic::BulkFetchStatItemsValue(
	const FString StatCode, 
	TArray<FString> const& UserIds, 
	FDModelsStatItemValueResponses const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto StatisticPtr = ApiClientPtr->GetStatisticApi().Pin();
	if (StatisticPtr.IsValid())
	{
		StatisticPtr->BulkFetchStatItemsValue(
		StatCode,
		UserIds,
		THandler<TArray<FAccelByteModelsStatItemValueResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsStatItemValueResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
