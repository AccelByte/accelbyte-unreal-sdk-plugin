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

void Item::GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria, int32 const& Offset, int32 const& Limit, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
    FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace);
    bool bIsNotFirst = false;

	if (!ItemCriteria.CategoryPath.IsEmpty())
    {
        bIsNotFirst = true; Url.Append("?");
        Url.Append(FString::Printf(TEXT("categoryPath=%s"), *FGenericPlatformHttp::UrlEncode(ItemCriteria.CategoryPath)));
    }
    if (!ItemCriteria.Region.IsEmpty())
    {
        if (bIsNotFirst)
        {
            Url.Append("&");
        }
        else
        {
            bIsNotFirst = true; Url.Append("?");
        }
        Url.Append(FString::Printf(TEXT("region=%s"), *ItemCriteria.Region));
    }
    if (!ItemCriteria.Language.IsEmpty())
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		Url.Append(FString::Printf(TEXT("language=%s"), *ItemCriteria.Language));
	}	
	if (ItemCriteria.ItemType != EAccelByteItemType::NONE)
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		Url.Append(FString::Printf(TEXT("itemType=%s"), *EAccelByteItemTypeToString(ItemCriteria.ItemType)));
	}
	if (ItemCriteria.AppType != EAccelByteAppType::NONE)
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		Url.Append(FString::Printf(TEXT("appType=%s"), *EAccelByteAppTypeToString(ItemCriteria.AppType)));
	}
	if (ItemCriteria.Tags.Num() > 0)
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		for (int i = 0; i < ItemCriteria.Tags.Num(); i++)
		{
			Url.Append((i == 0) ? TEXT("tags=") : TEXT(",")).Append(ItemCriteria.Tags[i]);
		}
	}
	if (ItemCriteria.Features.Num() > 0)
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		for (int i = 0; i < ItemCriteria.Features.Num(); i++)
		{
			Url.Append((i == 0) ? TEXT("features=") : TEXT(",")).Append(ItemCriteria.Features[i]);
		}
	}
	if (Offset > 0)
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		Url.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit > 0)
	{
		if (bIsNotFirst)
		{
			Url.Append("&");
		}
		else
		{
			bIsNotFirst = true; Url.Append("?");
		}
		Url.Append(FString::Printf(TEXT("limit=%d"), Limit));
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
	
} // Namespace Api
} // Namespace AccelByte