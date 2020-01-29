// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteStatisticBlueprints.h"
#include "Api/AccelByteStatisticApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FRegistry;
using AccelByte::FErrorHandler;

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

void UAccelByteBlueprintsStatistic::GetUserStatItemsByStatCodes(const TArray<FString>& StatCodes, const FGetUserStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetUserStatItemsByStatCodes(StatCodes, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::GetUserStatItemsByTags(const TArray<FString>& Tags, const FGetUserStatItemsByTagsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetUserStatItemsByTags(Tags, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::BulkAddStatItemValue(const TArray<FAccelByteModelsBulkUserStatItemInc>& data, const FBulkAddStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.BulkAddStatItemValue(data, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::BulkAddUserStatItemValue(const TArray<FAccelByteModelsBulkStatItemInc>& data, const FBulkAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.BulkAddUserStatItemValue(data, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::AddUserStatItemValue(const FString& statCode, const float& inc, const FAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.AddUserStatItemValue(statCode, inc, THandler<FAccelByteModelsStatItemIncResult>::CreateLambda([OnSuccess](const FAccelByteModelsStatItemIncResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}