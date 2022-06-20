// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteItemBlueprints.h"
#include "Core/AccelByteSettings.h"
#include "Api/AccelByteItemApi.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Item;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsItem::GetItemById(const FString& ItemId, const FString& Region, const FString& Language, const FGetItemByIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Item.GetItemById(ItemId, Language, Region, THandler<FAccelByteModelsPopulatedItemInfo>::CreateLambda([OnSuccess](const FAccelByteModelsPopulatedItemInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemByAppId(const FString& AppId, const FString& Region, const FString& Language, const FGetItemByAppIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Item.GetItemByAppId(AppId, Language, Region, THandler<FAccelByteModelsItemInfo>::CreateLambda([OnSuccess](const FAccelByteModelsItemInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemsByCriteria(const FAccelByteModelsItemCriteria& ItemCriteria, const int32& Offset, const int32& Limit, TArray<EAccelByteItemListSortBy> SortBy,
	const FGetItemsByCriteriaSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, Offset, Limit, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), SortBy);
}

void UAccelByteBlueprintsItem::SearchItem(const FString& Language, const FString& Keyword, int32 Page, int32 Size, const FString& Region, const FSearchItemSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Item.SearchItem(Language, Keyword, Page, Size, Region, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
