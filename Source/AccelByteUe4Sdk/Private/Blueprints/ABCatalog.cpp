// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABCatalog.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Core/AccelByteRegistry.h"

void UABCatalog::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABCatalog::GetRootCategories(FString const& Language
	, FDArrayModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	ApiClientPtr->Category.GetRootCategories(Language
		, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsCategoryInfo> const& Response)
			{
				FArrayModelsCategoryInfoResponse Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

void UABCatalog::GetCategory(FString const& CategoryPath
	, FString const& Language
	, FDModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	ApiClientPtr->Category.GetCategory(CategoryPath
		, Language
		, THandler<FAccelByteModelsCategoryInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsCategoryInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

void UABCatalog::GetChildCategories(FString const& CategoryPath
	, FString const& Language
	, FDArrayModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	ApiClientPtr->Category.GetChildCategories(Language
		, CategoryPath
		, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsCategoryInfo> const& Response)
			{
				FArrayModelsCategoryInfoResponse Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

void UABCatalog::GetDescendantCategories(FString const& CategoryPath
	, FString const& Language
	, FDArrayModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->Category.GetDescendantCategories(Language
		, CategoryPath
		, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsCategoryInfo> const& Response)
			{
				FArrayModelsCategoryInfoResponse Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

void UABCatalog::GetItemById(FString const& ItemId
	, FString const& Language
	, FString const& Region
	, const FString& StoreId
	, bool bPopulateBundle
	, FDModelsPopulatedItemInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	ApiClientPtr->Item.GetItemById(ItemId
		, Language
		, Region
		, THandler<FAccelByteModelsPopulatedItemInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsPopulatedItemInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		, StoreId
		, bPopulateBundle);
}

void UABCatalog::GetItemByAppId(FString const& AppId
	, FString const& Language
	, FString const& Region
	, FDModelsItemInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->Item.GetItemByAppId(AppId
		, Language
		, Region
		, THandler<FAccelByteModelsItemInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsItemInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

void UABCatalog::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria
	, int32 const& Offset
	, int32 const& Limit
	, TArray<EAccelByteItemListSortBy> SortBy
	, FDModelsItemPagingSlicedResultResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->Item.GetItemsByCriteria(ItemCriteria
		, Offset
		, Limit
		, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsItemPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		, SortBy);
}

void UABCatalog::SearchItem(FString const& Language
	, FString const& Keyword
	, int32 const& Offset
	, int32 const& Limit
	, FString const& Region
	, FDModelsItemPagingSlicedResultResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->Item.SearchItem(Language
		, Keyword
		, Offset
		, Limit
		, Region
		, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsItemPagingSlicedResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

void UABCatalog::GetItemDynamicData(FString const& ItemId
	, FDModelsItemDynamicDataResponse OnSuccess
	, FDErrorHandler OnError)
{
	ApiClientPtr->Item.GetItemDynamicData(ItemId
		, THandler<FAccelByteModelsItemDynamicData>::CreateLambda(
			[OnSuccess](FAccelByteModelsItemDynamicData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
}

