// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteStatisticBlueprints.h"
#include "AccelByteStatisticApi.h"
#include "AccelByteRegistry.h"

using AccelByte::FRegistry;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsStatistic::GetAllStatItems(const FString& ProfileId, const FGetAllStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetAllStatItems(ProfileId, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

void UAccelByteBlueprintsStatistic::GetStatItemsByStatCodes(const FString& ProfileId, TArray<FString> StatCodes, const FGetStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Statistic.GetStatItemsByStatCodes(ProfileId, StatCodes, THandler< TArray<FAccelByteModelsUserStatItemInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsUserStatItemInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}

