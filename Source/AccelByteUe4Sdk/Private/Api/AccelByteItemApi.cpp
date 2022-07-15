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

void Item::GetItemById(FString const& ItemId, FString const& Language, FString const& Region, THandler<FAccelByteModelsPopulatedItemInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *ItemId);
	if (!Region.IsEmpty() || !Language.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("?")));
		if (!Region.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("region=%s"), *Region));
			if (!Language.IsEmpty())
			{
				Url.Append(FString::Printf(TEXT("&language=%s"), *Language));
			}
		}
		else if (!Language.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("language=%s"), *Language));
		}
	}

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
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byAppId?appId=%s"), *SettingsRef.PlatformServerUrl, *SettingsRef.PublisherNamespace, *AppId);
	if (!Region.IsEmpty() || !Language.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&")));
		if (!Region.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("region=%s"), *Region));
			if (!Language.IsEmpty())
			{
				Url.Append(FString::Printf(TEXT("&language=%s"), *Language));
			}
		}
		else if (!Language.IsEmpty())
		{
			Url.Append(FString::Printf(TEXT("language=%s"), *Language));
		}
	}

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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
    FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace);

	FString Query = TEXT("");
	if (!ItemCriteria.CategoryPath.IsEmpty())
    { 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("categoryPath=%s"), *FGenericPlatformHttp::UrlEncode(ItemCriteria.CategoryPath))); 
    }
    if (!ItemCriteria.Region.IsEmpty())
    { 
    	Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
        Query.Append(FString::Printf(TEXT("region=%s"), *ItemCriteria.Region));
    }
    if (!ItemCriteria.Language.IsEmpty())
	{ 
    	Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&")); 
		Query.Append(FString::Printf(TEXT("language=%s"), *ItemCriteria.Language));
	}	
	if (ItemCriteria.ItemType != EAccelByteItemType::NONE)
	{ 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("itemType=%s"), *EAccelByteItemTypeToString(ItemCriteria.ItemType)));
	}
	if (ItemCriteria.AppType != EAccelByteAppType::NONE)
	{ 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&")); 
		Query.Append(FString::Printf(TEXT("appType=%s"), *EAccelByteAppTypeToString(ItemCriteria.AppType)));
	}
	if (ItemCriteria.Tags.Num() > 0)
	{ 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("tags=%s"), *FString::Join(ItemCriteria.Tags, TEXT(","))));
	}
	if (ItemCriteria.Features.Num() > 0)
	{ 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("features=%s"), *FString::Join(ItemCriteria.Features, TEXT(","))));
	}
	if (Offset > 0)
	{ 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit > 0)
	{ 
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
	}	
	if (SortBy.Num() > 0 )
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		TArray<FString> QuerySortBy;
		for (int i = 0; i < SortBy.Num(); i++)
		{
			QuerySortBy.Add(ConvertItemSortByToString(SortBy[i]));
		}
		Query.Append(FString::Printf(TEXT("sortBy=%s"), *FString::Join(QuerySortBy, TEXT(","))));
	}
	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"),*Query));
	
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
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/search?language=%s&keyword=%s"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace, *Language, *FGenericPlatformHttp::UrlEncode(Keyword));
	if (!Region.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&region=%s"), *Region));
	}
	if (Offset > 0)
	{
		Url.Append(FString::Printf(TEXT("&offset=%d"), Offset));
	}
	if (Limit > 0)
	{
		Url.Append(FString::Printf(TEXT("&limit=%d"), Limit));
	}
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
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/bySku?sku=%s"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace, *Sku);
	if (!Language.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&language=%s"), *Language));
	}
	if (!Region.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&region=%s"), *Region));
	}

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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/dynamic")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *ItemId);

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
	
} // Namespace Api
} // Namespace AccelByte