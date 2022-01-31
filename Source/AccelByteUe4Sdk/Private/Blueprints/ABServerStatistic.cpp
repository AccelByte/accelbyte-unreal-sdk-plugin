// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerStatistic.h"

void UABServerStatistic::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
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
	FDErrorHandler OnError)
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
			}));
}

void UABServerStatistic::GetUserStatItems(
	FString const& UserId,
	TArray<FString> const& StatCodes,
	TArray<FString> const& Tags,
	FDUserStatItemPagingSlicedDelegate OnSuccess,
	FDErrorHandler OnError)
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
