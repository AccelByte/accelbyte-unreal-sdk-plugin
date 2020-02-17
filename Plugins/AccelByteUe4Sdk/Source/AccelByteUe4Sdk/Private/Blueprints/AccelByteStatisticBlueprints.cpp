// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteStatisticBlueprints.h"
#include "Api/AccelByteStatisticApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FRegistry;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsStatistic::CreateUserStatItems(const TArray<FString>& StatCodes, const FCreateUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.CreateUserStatItems(
		StatCodes,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
		{
			OnSuccess.ExecuteIfBound(Result);
		}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
}

void UAccelByteBlueprintsStatistic::GetAllUserStatItems(const FGetAllUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetAllUserStatItems(THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::GetUserStatItems(const TArray<FString>& StatCodes, const TArray<FString>& Tags, const FGetUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetUserStatItems(
		StatCodes,
		Tags,
		THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
		{
			OnSuccess.ExecuteIfBound(Result);
		}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
}

void UAccelByteBlueprintsStatistic::IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data, const FBulkAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.IncrementUserStatItems(Data, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}