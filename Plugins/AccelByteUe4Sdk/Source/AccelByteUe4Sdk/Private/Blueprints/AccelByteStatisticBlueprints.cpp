// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteStatisticBlueprints.h"
#include "Api/AccelByteStatisticApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FRegistry;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsStatistic::GetAllProfileStatItems(const FString& ProfileId, const FGetAllProfileStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetAllProfileStatItems(ProfileId, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::GetProfileStatItemsByStatCodes(const FString& ProfileId, const TArray<FString>& StatCodes, const FGetProfileStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetProfileStatItemsByStatCodes(ProfileId, StatCodes, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
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

void UAccelByteBlueprintsStatistic::BulkAddStatItemValue(const TArray<FAccelByteModelsBulkUserStatItemInc>& data, const FBulkAddStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
    FRegistry::Statistic.BulkAddStatItemValue(data, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemIncResult>& Result)
    {
        OnSuccess.ExecuteIfBound(Result);
    }),
        FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
    {
        OnError.ExecuteIfBound(Code, Message);
    }));
}

void UAccelByteBlueprintsStatistic::BulkAddProfileStatItemValue(const FString& ProfileId, const TArray<FAccelByteModelsBulkStatItemInc>& data, const FBulkAddProfileStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
    FRegistry::Statistic.BulkAddProfileStatItemValue(ProfileId, data, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemIncResult>& Result)
    {
        OnSuccess.ExecuteIfBound(Result);
    }),
        FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
    {
        OnError.ExecuteIfBound(Code, Message);
    }));
}

void UAccelByteBlueprintsStatistic::AddProfileStatItemValue(const FString& ProfileId, const FString& statCode, const float& inc, const FAddProfileStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
    FRegistry::Statistic.AddProfileStatItemValue(ProfileId, statCode, inc, THandler<FAccelByteModelsStatItemIncResult>::CreateLambda([OnSuccess](const FAccelByteModelsStatItemIncResult& Result)
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
    FRegistry::Statistic.BulkAddUserStatItemValue(data, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsBulkStatItemIncResult>& Result)
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