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

void Item::GetItemById(FString const& ItemId
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsPopulatedItemInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& StoreId
	, bool bPopulateBundle
	, bool AutoCalcEstimatedPrice)
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

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("storeId"), StoreId },
		{ TEXT("populateBundle"), bPopulateBundle ? TEXT("true"):TEXT("false") },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
	
	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
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

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("appId"), AppId }
	};

	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

void Item::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria
	, int32 const& Offset
	, int32 const& Limit
	, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<EAccelByteItemListSortBy> SortBy
	, FString const& StoreId
	, bool AutoCalcEstimatedPrice)
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

	TArray<FString> SortByStringArray = {};
	if (SortBy.Num() > 0 )
	{		 
		for (auto SortByEnum : SortBy)
		{
			if (SortByEnum != EAccelByteItemListSortBy::NONE)
			{
				SortByStringArray.Add(FAccelByteUtilities::ConvertItemSortByToString(SortByEnum));
			}
		} 
	}
	
	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("categoryPath"), ItemCriteria.CategoryPath },
		{ TEXT("region"), ItemCriteria.Region },
		{ TEXT("language"), ItemCriteria.Language },
		{ TEXT("itemType"), ItemCriteria.ItemType != EAccelByteItemType::NONE ?
				FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.ItemType) : TEXT("") },
		{ TEXT("appType"), ItemCriteria.AppType != EAccelByteAppType::NONE ?
				FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.AppType) : TEXT("")  },
		{ TEXT("tags"), FString::Join(ItemCriteria.Tags, TEXT(",")) },
		{ TEXT("features"), FString::Join(ItemCriteria.Features, TEXT(",")) },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("sortBy"), FString::Join(SortByStringArray, TEXT(",")) },
		{ TEXT("storeId"), StoreId },
		{ TEXT("includeSubCategoryItem"), ItemCriteria.IncludeSubCategoryItem ? TEXT("true") : TEXT("false") },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
	
	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

void Item::SearchItem(FString const& Language
	, FString const& Keyword
	, int32 const& Offset
	, int32 const& Limit
	, FString const& Region
	, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, bool AutoCalcEstimatedPrice)
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
	
	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("keyword"), Keyword },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};

	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

void Item::GetItemBySku(FString const& Sku
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsItemInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, bool AutoCalcEstimatedPrice)
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

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("sku"), Sku },
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
	
	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

void Item::BulkGetLocaleItems(TArray<FString> const& ItemIds
	, FString const& Region
	, FString const& Language
	, THandler<TArray<FAccelByteModelsItemInfo>> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& StoreId
	, bool AutoCalcEstimatedPrice)
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

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("itemIds"), FString::Join(ItemIds, TEXT(",")) },
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("storeId"), StoreId },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
 
	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
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

	HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

void Item::GetListAllStores(THandler<TArray<FAccelByteModelsPlatformStore>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return;
	}	

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/stores")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

void Item::GetEstimatedPrice(const TArray<FString>& ItemIds, const FString& Region, THandler<TArray<FAccelByteModelsEstimatedPrices>> const& OnSuccess
	, FErrorHandler const& OnError)
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

	// There will only one published stored in a live game environment. 
	// Here the StoreId is intended to be set as empty string, then the Platform Service will fill by default value/published store Id. 
	const FString& StoreId = TEXT("");
	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("storeId"), StoreId },
		{ TEXT("itemIds"), FString::Join(ItemIds, TEXT(",")) },
		{ TEXT("region"), Region },
	};
	
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/estimatedPrice")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(Verb, Url, QueryParams, FString(), OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte