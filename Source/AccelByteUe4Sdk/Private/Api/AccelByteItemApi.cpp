// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteItemApi.h"
#include "Core/AccelByteError.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{
Item::Item(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

Item::~Item(){}

FString Item::ConvertItemSortByToString(EAccelByteItemListSortBy const& SortBy)
{ 
	switch (SortBy)
	{
	case EAccelByteItemListSortBy::NAME:
		return FGenericPlatformHttp::UrlEncode(TEXT("name"));
	case EAccelByteItemListSortBy::NAME_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("name:desc"));
	case EAccelByteItemListSortBy::CREATEDAT:
		return FGenericPlatformHttp::UrlEncode(TEXT("createdAt"));
	case EAccelByteItemListSortBy::CREATEDAT_ASC:
		return FGenericPlatformHttp::UrlEncode(TEXT("createdAt:asc"));
	case EAccelByteItemListSortBy::CREATEDAT_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("createdAt:desc"));
	case EAccelByteItemListSortBy::UPDATEDAT:
		return FGenericPlatformHttp::UrlEncode(TEXT("updatedAt"));
	case EAccelByteItemListSortBy::UPDATEDAT_ASC:
		return FGenericPlatformHttp::UrlEncode(TEXT("updatedAt:asc"));
	case EAccelByteItemListSortBy::UPDATEDAT_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("updatedAt:desc"));
	case EAccelByteItemListSortBy::DISPLAYORDER:
		return FGenericPlatformHttp::UrlEncode(TEXT("displayOrder"));
	case EAccelByteItemListSortBy::DISPLAYORDER_ASC:
		return FGenericPlatformHttp::UrlEncode(TEXT("displayOrder:asc"));
	case EAccelByteItemListSortBy::DISPLAYORDER_DESC:
		return FGenericPlatformHttp::UrlEncode(TEXT("displayOrder:desc"));
	}
	return TEXT("");
}

void Item::GetItemById(FString const& ItemId
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsPopulatedItemInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& StoreId
	, bool bPopulateBundle)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}
	
	if (ItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemId can not be empty."));
		return;
	}
	
	const FString Verb            = TEXT("GET");
	const FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *ItemId);

	TMap<FString, FString> Params;

	if (!Region.IsEmpty())
	{
		Params.Add(TEXT("region"), *Region);
	}
	if (!Language.IsEmpty())
	{
		Params.Add(TEXT("language"), Language);
	}
	if (!StoreId.IsEmpty())
	{
		Params.Add(TEXT("storeId"), *StoreId);
	}
	if (bPopulateBundle)
	{
		Params.Add(TEXT("populateBundle"), TEXT("true"));
	}
	
	HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
}

void Item::GetItemByAppId(FString const& AppId
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsItemInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SettingsRef.PublisherNamespace.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Publisher Namespace isn't specified in the configuration file."));
		return;
	}
	
	if (AppId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, AppId can not be empty."));
		return;
	}
	
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byAppId")
		, *SettingsRef.PlatformServerUrl
		, *SettingsRef.PublisherNamespace);

	TMap<FString, FString> Params;

	Params.Add(TEXT("appId"), AppId);

	if (!Region.IsEmpty())
	{
		Params.Add(TEXT("region"), *Region);
	}
	if (!Language.IsEmpty())
	{
		Params.Add(TEXT("language"), Language);
	}

	HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
}

void Item::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria
	, int32 const& Offset
	, int32 const& Limit
	, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<EAccelByteItemListSortBy> SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}
	
	const FString Verb = TEXT("GET");
    const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria")
    	, *SettingsRef.PlatformServerUrl
    	, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Params;

	if (!ItemCriteria.CategoryPath.IsEmpty())
	{
		Params.Add(TEXT("categoryPath"), ItemCriteria.CategoryPath);
	}
	if (!ItemCriteria.Region.IsEmpty())
	{
		Params.Add(TEXT("region"), *ItemCriteria.Region);
	}
	if (!ItemCriteria.Language.IsEmpty())
	{
		Params.Add(TEXT("language"), ItemCriteria.Language);
	}
	if (ItemCriteria.ItemType != EAccelByteItemType::NONE)
	{
		Params.Add(TEXT("itemType"), FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.ItemType));
	}
	if (ItemCriteria.AppType != EAccelByteAppType::NONE)
	{
		Params.Add(TEXT("appType"), FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.AppType));
	}
	if (ItemCriteria.Tags.Num() > 0)
	{
		TArray<FString> Tags;
		for (auto& Tag : ItemCriteria.Tags)
		{
			Tags.Add(Tag);
		}
		Params.Add(TEXT("tags"), FString::Join(Tags, TEXT(",")));
	}
	if (ItemCriteria.Features.Num() > 0)
	{
		TArray<FString> Features;
		for (auto& Feature : ItemCriteria.Features)
		{
			Features.Add(Feature);
		}
		Params.Add(TEXT("features"), FString::Join(Features, TEXT(",")));
	}
	if (Offset > 0)
	{
		Params.Add(TEXT("offset"), FString::Printf(TEXT("%d"), Offset));
	}
	if (Limit > 0)
	{
		Params.Add(TEXT("limit"), FString::Printf(TEXT("%d"), Limit));
	}
	if (SortBy.Num() > 0 )
	{
		TArray<FString> SortByStrings;
		for (auto SortByEnum : SortBy)
		{
			SortByStrings.Add(ConvertItemSortByToString(SortByEnum));
		}
		Params.Add(TEXT("sortBy"), FString::Join(SortByStrings, TEXT(",")));
	} 

	HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
}

void Item::SearchItem(FString const& Language
	, FString const& Keyword
	, int32 const& Offset
	, int32 const& Limit
	, FString const& Region
	, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}
	
	if (Language.IsEmpty() || Keyword.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Language and Keyword can not be empty"));
		return;
	}
	
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/search")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace());
	
	TMap<FString, FString> Params;
	
	Params.Add(TEXT("language"), Language);
	Params.Add(TEXT("keyword"), Keyword);
	
	if (!Region.IsEmpty())
	{
		Params.Add(TEXT("region"), *Region);
	}
	if (Offset > 0)
	{
		Params.Add(TEXT("offset"), FString::Printf(TEXT("%d"), Offset));
	}
	if (Limit > 0)
	{
		Params.Add(TEXT("limit"), FString::Printf(TEXT("%d"), Limit));
	}

	HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
}

void Item::GetItemBySku(FString const& Sku, FString const& Language, FString const& Region, THandler<FAccelByteModelsItemInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}
	
	if(Sku.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Sku number can not be empty."));
		return;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/bySku")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Params;
	
	Params.Add(TEXT("sku"), Sku);

	if (!Region.IsEmpty())
	{
		Params.Add(TEXT("region"), *Region);
	}
	if (!Language.IsEmpty())
	{
		Params.Add(TEXT("language"), Language);
	}

	HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
}

void Item::BulkGetLocaleItems(TArray<FString> const& ItemIds
	, FString const& Region
	, FString const& Language
	, THandler<TArray<FAccelByteModelsItemInfo>> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& StoreId)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}

	if (ItemIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemIds can not be empty"));
		return;
	}
	
	FString Verb = TEXT("GET");
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/locale/byIds")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Params;

	Params.Add(TEXT("itemIds"), FString::Join(ItemIds, TEXT(",")));

	if (!Region.IsEmpty())
	{
		Params.Add(TEXT("region"), *Region);
	}
	if (!Language.IsEmpty())
	{
		Params.Add(TEXT("language"), Language);
	}
	if (!StoreId.IsEmpty())
	{
		Params.Add(TEXT("storeId"), *StoreId);
	}
 
	HttpClient.ApiRequest(Verb, Url, Params, OnSuccess, OnError);
}

void Item::GetItemDynamicData(FString const& ItemId
	, THandler<FAccelByteModelsItemDynamicData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}
	
	if(ItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemId can not be empty."));
		return;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/dynamic")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *ItemId);

	HttpClient.ApiRequest(Verb, Url, OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte