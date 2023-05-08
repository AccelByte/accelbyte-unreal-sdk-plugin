// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerStatistic.h"

using namespace AccelByte;

void UABServerStatistic::SetServerApiClient(FServerApiClientPtr const& NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerStatistic::CreateUserStatItems(
	FString const & UserId,
	TArray<FString> const& StatCodes,
	FDArrayBulkStatItemOperationDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerStatistic.CreateUserStatItems(
		UserId,
		StatCodes,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerStatistic::GetAllUserStatItems(
	FString const& UserId,
	FDUserStatItemPagingSlicedDelegate OnSuccess,
	FDErrorHandler OnError,
	int32 Limit,
	int32 Offset,
	EAccelByteStatisticSortBy SortBy)
{
	ApiClientPtr->ServerStatistic.GetAllUserStatItems(
		UserId,
		THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserStatItemPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		Limit,
		Offset,
		SortBy);
}

void UABServerStatistic::GetUserStatItems(
	FString const& UserId,
	TArray<FString> const& StatCodes,
	TArray<FString> const& Tags,
	FDUserStatItemPagingSlicedDelegate OnSuccess,
	FDErrorHandler OnError,
	int32 Limit,
	int32 Offset,
	EAccelByteStatisticSortBy SortBy)
{
	ApiClientPtr->ServerStatistic.GetUserStatItems(
		UserId,
		StatCodes,
		Tags,
		THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserStatItemPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		Limit,
		Offset,
		SortBy);
}

void UABServerStatistic::BulkFetchStatItemsValue(
	FString const& StatCode, 
	TArray<FString> const& UserIds, 
	FDFetchUserStatistic OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerStatistic.BulkFetchStatItemsValue(
		StatCode,
		UserIds,
		THandler<TArray<FAccelByteModelsStatItemValueResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsStatItemValueResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerStatistic::IncrementManyUsersStatItems(
	TArray<FAccelByteModelsBulkUserStatItemInc> const& Data,
	FDArrayBulkStatItemOperationDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerStatistic.IncrementManyUsersStatItems(
		Data,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerStatistic::IncrementUserStatItems(
	FString const& UserId,
	TArray<FAccelByteModelsBulkStatItemInc> const& Data,
	FDArrayBulkStatItemOperationDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerStatistic.IncrementUserStatItems(
		UserId,
		Data,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerStatistic::GetGlobalStatItemsByStatCode(
	FString const& StatCode, 
	FDGlobalStatItemDelegate OnSuccess, 
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerStatistic.GetGlobalStatItemsByStatCode(
		StatCode,
		THandler<FAccelByteModelsGlobalStatItemValueResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsGlobalStatItemValueResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
