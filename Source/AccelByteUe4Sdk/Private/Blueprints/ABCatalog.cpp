// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABCatalog.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteCategoryApi.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCatalog, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCatalog);

using namespace AccelByte;

void UABCatalog::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABCatalog::GetRootCategories(FString const& Language
	, FDArrayModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Category = ApiClientPtr->GetCategoryApi().Pin();
	if (Category.IsValid())
	{
		Category->GetRootCategories(Language
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Category API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Category API already destroyed!"));
	}
}

void UABCatalog::GetCategory(FString const& CategoryPath
	, FString const& Language
	, FDModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Category = ApiClientPtr->GetCategoryApi().Pin();
	if (Category.IsValid())
	{
		Category->GetCategory(CategoryPath
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Category API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Category API already destroyed!"));
	}
}

void UABCatalog::GetChildCategories(FString const& CategoryPath
	, FString const& Language
	, FDArrayModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Category = ApiClientPtr->GetCategoryApi().Pin();
	if (Category.IsValid())
	{
		Category->GetChildCategories(Language
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Category API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Category API already destroyed!"));
	}
}

void UABCatalog::GetDescendantCategories(FString const& CategoryPath
	, FString const& Language
	, FDArrayModelsCategoryInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Category = ApiClientPtr->GetCategoryApi().Pin();
	if (Category.IsValid())
	{
		Category->GetDescendantCategories(Language
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Category API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Category API already destroyed!"));
	}
}

void UABCatalog::GetItemById(FString const& ItemId
	, FString const& Language
	, FString const& Region
	, const FString& StoreId
	, bool bPopulateBundle
	, FDModelsPopulatedItemInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Item = ApiClientPtr->GetItemApi().Pin();
	if (Item.IsValid())
	{
		Item->GetItemById(ItemId
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Item API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Item API already destroyed!"));
	}
}

void UABCatalog::GetItemByAppId(FString const& AppId
	, FString const& Language
	, FString const& Region
	, FDModelsItemInfoResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Item = ApiClientPtr->GetItemApi().Pin();
	if (Item.IsValid())
	{
		Item->GetItemByAppId(AppId
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Item API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Item API already destroyed!"));
	}
}

void UABCatalog::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria
	, int32 const& Offset
	, int32 const& Limit
	, TArray<EAccelByteItemListSortBy> SortBy
	, FDModelsItemPagingSlicedResultResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Item = ApiClientPtr->GetItemApi().Pin();
	if (Item.IsValid())
	{
		Item->GetItemsByCriteria(ItemCriteria
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Item API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Item API already destroyed!"));
	}
}

void UABCatalog::SearchItem(FString const& Language
	, FString const& Keyword
	, int32 const& Offset
	, int32 const& Limit
	, FString const& Region
	, FDModelsItemPagingSlicedResultResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Item = ApiClientPtr->GetItemApi().Pin();
	if (Item.IsValid())
	{
		Item->SearchItem(Language
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Item API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Item API already destroyed!"));
	}
}

void UABCatalog::GetItemDynamicData(FString const& ItemId
	, FDModelsItemDynamicDataResponse OnSuccess
	, FDErrorHandler OnError)
{
	auto Item = ApiClientPtr->GetItemApi().Pin();
	if (Item.IsValid())
	{
		Item->GetItemDynamicData(ItemId
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
	else
	{
		UE_LOG(LogAccelByteCatalog, Warning, TEXT("Invalid Item API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Item API already destroyed!"));
	}
}

