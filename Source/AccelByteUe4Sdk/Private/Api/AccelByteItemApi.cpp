// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteItemApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteUtilities.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Api
{
Item::Item(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{
}

Item::Item(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform)
{
}

Item::~Item()
{
}

FAccelByteTaskWPtr Item::GetItemById(FString const& ItemId
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsPopulatedItemInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& StoreId
	, bool bPopulateBundle
	, bool AutoCalcEstimatedPrice)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}
	
	if (ItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemId can not be empty."));
		return nullptr;
	}
	
	const FString Verb            = TEXT("GET");
	const FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ItemId));

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("storeId"), StoreId },
		{ TEXT("populateBundle"), bPopulateBundle ? TEXT("true"):TEXT("false") },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
	
	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetItemByAppId(FString const& AppId
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsItemInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SettingsRef.PublisherNamespace.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Publisher Namespace isn't specified in the configuration file."));
		return nullptr;
	}
	
	if (AppId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, AppId can not be empty."));
		return nullptr;
	}
	
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byAppId")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(SettingsRef.PublisherNamespace));

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("appId"), AppId }
	};

	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria
	, int32 Offset
	, int32 Limit
	, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<EAccelByteItemListSortBy> const& SortBy
	, FString const& StoreId
	, bool AutoCalcEstimatedPrice)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}
	
	const FString Verb = TEXT("GET");
    const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria")
    	, *SettingsRef.PlatformServerUrl
    	, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

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
	
	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::SearchItem(FString const& Language
	, FString const& Keyword
	, int32 Offset
	, int32 Limit
	, FString const& Region
	, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, bool AutoCalcEstimatedPrice)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}
	
	if (Language.IsEmpty() || Keyword.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Language and Keyword can not be empty"));
		return nullptr;
	}
	
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/search")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));
	
	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("keyword"), Keyword },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};

	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetItemBySku(FString const& Sku
	, FString const& Language
	, FString const& Region
	, THandler<FAccelByteModelsItemInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, bool AutoCalcEstimatedPrice)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}
	
	if(Sku.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Sku number can not be empty."));
		return nullptr;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/bySku")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("sku"), Sku },
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
	
	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::BulkGetLocaleItems(TArray<FString> const& ItemIds
	, FString const& Region
	, FString const& Language
	, THandler<TArray<FAccelByteModelsItemInfo>> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& StoreId
	, bool AutoCalcEstimatedPrice)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}

	if (ItemIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemIds can not be empty"));
		return nullptr;
	}
	
	FString Verb = TEXT("GET");
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/locale/byIds")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("itemIds"), FString::Join(ItemIds, TEXT(",")) },
		{ TEXT("region"), Region },
		{ TEXT("language"), Language },
		{ TEXT("storeId"), StoreId },
		{ TEXT("autoCalcEstimatedPrice"), AutoCalcEstimatedPrice ? TEXT("true") : TEXT("false")}
	};
 
	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetItemDynamicData(FString const& ItemId
	, THandler<FAccelByteModelsItemDynamicData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}
	
	if(ItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemId can not be empty."));
		return nullptr;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/dynamic")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ItemId));

	return HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetListAllStores(THandler<TArray<FAccelByteModelsPlatformStore>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	}	

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/stores")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetEstimatedPrice(const TArray<FString>& ItemIds
	, FString const& Region
	, THandler<TArray<FAccelByteModelsEstimatedPrices>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetNamespace().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("Not logged in, Namespace is empty due to failed login."));
		return nullptr;
	} 

	if (ItemIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ItemIds can not be empty"));
		return nullptr;
	}

	// There will only one published stored in a live game environment. 
	// Here the StoreId is intended to be set as empty string, then the Platform Service will fill by default value/published store Id. 
	FString const& StoreId = TEXT("");
	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("storeId"), StoreId },
		{ TEXT("itemIds"), FString::Join(ItemIds, TEXT(",")) },
		{ TEXT("region"), Region },
	};
	
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/estimatedPrice")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(Verb, Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Item::GetItemMappings(EAccelBytePlatformMapping Platform
	, THandler<FAccelByteModelsItemMappingsResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/iap/item/mapping")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams;
	if(Platform != EAccelBytePlatformMapping::NONE)
	{
		QueryParams.Add(TEXT("platform"),ConvertPlatformMappingToString(Platform));
	}

	THandler<FAccelByteModelsItemMappingsResponseInternal> OnSuccessInternal = THandler<FAccelByteModelsItemMappingsResponseInternal>::CreateLambda(
		[OnSuccess](const FAccelByteModelsItemMappingsResponseInternal& Result)
		{
			FAccelByteModelsItemMappingsResponse CorrectedResult;
			for (const auto& ResultItem : Result.Data)
			{
				FAccelByteModelsItemMapping Item;
				Item.ItemIdentity = ResultItem.ItemIdentity;
				Item.ItemIdentityType = ResultItem.ItemIdentityType;
				Item.PlatformProductId = ResultItem.PlatformProductId;
				Item.Platform = static_cast<EAccelBytePlatformMapping>(ResultItem.Platform);

				CorrectedResult.Data.Add(Item);
			}
			
			OnSuccess.ExecuteIfBound(CorrectedResult);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessInternal, OnError);
}

FString Item::ConvertPlatformMappingToString(EAccelBytePlatformMapping Platform)
{
	switch (Platform)
	{
	case EAccelBytePlatformMapping::APPLE:
		return TEXT("APPLE");
	case EAccelBytePlatformMapping::GOOGLE:
		return TEXT("GOOGLE");
	case EAccelBytePlatformMapping::PLAYSTATION:
		return TEXT("PLAYSTATION");
	case EAccelBytePlatformMapping::STEAM:
		return TEXT("STEAM");
	case EAccelBytePlatformMapping::XBOX:
		return TEXT("XBOX");
	case EAccelBytePlatformMapping::EPIC_GAMES:
		return TEXT("EPICGAMES");
	case EAccelBytePlatformMapping::OCULUS:
		return TEXT("OCULUS");
	case EAccelBytePlatformMapping::TWITCH:
		return TEXT("TWITCH");
	case EAccelBytePlatformMapping::STADIA:
		return TEXT("STADIA");
	default:
		return TEXT("");
	}
}
} // Namespace Api
} // Namespace AccelByte