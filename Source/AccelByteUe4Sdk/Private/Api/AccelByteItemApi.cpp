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
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{
}

Item::~Item(){}

FString EAccelByteItemTypeToString(EAccelByteItemType const& EnumValue) {
	UEnum const* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteItemType"), true);

	if (!EnumPtr)
	{
		return "Invalid";
	}

	return EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue));
}

FString EAccelByteItemStatusToString(EAccelByteItemStatus const& EnumValue) {
	UEnum const* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteItemStatus"), true);

	if (!EnumPtr)
	{
		return "Invalid";
	}

	return EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue));
}

FString EAccelByteAppTypeToString(EAccelByteAppType const& EnumValue)
{
	UEnum const* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteAppType"), true);

	if (!EnumPtr)
	{
		return "Invalid";
	}

	return EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue));
}

void Item::GetItemById(FString const& ItemId, FString const& Language, FString const& Region, THandler<FAccelByteModelsPopulatedItemInfo> const& OnSuccess,
	FErrorHandler const& OnError, const FString& StoreId, bool PopulateBundle)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *ItemId);
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{TEXT("region"), Region},
		{TEXT("language"), Language},
		{TEXT("storeId"), StoreId},
		{TEXT("populateBundle"), PopulateBundle ? "true" : "false"}
	});
	Url.Append(QueryParams);

	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::GetItemByAppId(FString const& AppId, FString const& Language, FString const& Region, THandler<FAccelByteModelsItemInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byAppId"), *SettingsRef.PlatformServerUrl, *SettingsRef.PublisherNamespace);
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{TEXT("appId"), AppId},
		{TEXT("region"), Region},
		{TEXT("language"), Language}, 
	});
	Url.Append(QueryParams); 

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

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
	
void Item::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria, int32 const& Offset, int32 const& Limit,
	THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError, TArray<EAccelByteItemListSortBy> SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	FString SortByQueryParamValue = TEXT("");
	if (SortBy.Num() > 0 )
	{ 
		for (int i = 0; i < SortBy.Num(); i++)
		{
			FString sortByString = ConvertItemSortByToString(SortBy[i]);
			FString sortByStringAppend = FString::Printf(TEXT(",%s"), *ConvertItemSortByToString(SortBy[i]));
			SortByQueryParamValue.Append((i == 0) ? sortByString : sortByStringAppend);
		}
	} 
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
    FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace);
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("categoryPath"), FGenericPlatformHttp::UrlEncode(ItemCriteria.CategoryPath) },
		{ TEXT("region"), ItemCriteria.Region },
		{ TEXT("language"), ItemCriteria.Language },
		{ TEXT("itemType"), ItemCriteria.ItemType != EAccelByteItemType::NONE ? EAccelByteItemTypeToString(ItemCriteria.ItemType) : TEXT("") },
		{ TEXT("appType"), ItemCriteria.AppType != EAccelByteAppType::NONE ? EAccelByteAppTypeToString(ItemCriteria.AppType) : TEXT("") },
		{ TEXT("tags"), FAccelByteUtilities::CreateQueryParamValueFromArray(ItemCriteria.Tags)  },
		{ TEXT("features"), FAccelByteUtilities::CreateQueryParamValueFromArray(ItemCriteria.Features) },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("sortBy"), SortByQueryParamValue },
	});
	Url.Append(QueryParams);
 
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::SearchItem(FString const& Language, FString const& Keyword, int32 const& Offset, int32 const& Limit, FString const& Region, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/search"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace);
	FString QueryParams (FAccelByteUtilities::CreateQueryParams({
		{TEXT("language"), Language},
		{TEXT("keyword"), FGenericPlatformHttp::UrlEncode(Keyword)},
		{TEXT("region"), Region},
		{TEXT("offset"), FString::FromInt(Offset)},
		{TEXT("limit"), FString::FromInt(Limit)}
	}));
	Url.Append(QueryParams);
	
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::GetItemBySku(FString const& Sku, FString const& Language, FString const& Region, THandler<FAccelByteModelsItemInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if(Sku.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::StatusBadRequest), TEXT("Bad request, Sku number can not empty"));
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/bySku"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace);
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{TEXT("sku"), Sku},
		{TEXT("language"), Language},
		{TEXT("region"), Region}
	}); 
	Url.Append(QueryParams); 

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::BulkGetLocaleItems(const TArray<FString>& ItemIds, const FString& Region, const FString& Language, THandler<TArray<FAccelByteModelsItemInfo>> const& OnSuccess,
	FErrorHandler const& OnError, const FString& StoreId)
{
	FReport::Log(FString(__FUNCTION__)); 
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
    FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/locale/byIds"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace);
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("storeId"), StoreId },
		{ TEXT("itemIds"), FAccelByteUtilities::CreateQueryParamValueFromArray(ItemIds) },
		{ TEXT("region"), Region },
		{ TEXT("language"), Language }, 
	});
	Url.Append(QueryParams);
 
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
} // Namespace Api
} // Namespace AccelByte