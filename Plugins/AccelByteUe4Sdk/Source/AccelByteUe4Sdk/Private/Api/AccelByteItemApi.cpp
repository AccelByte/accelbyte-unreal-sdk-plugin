// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteItemApi.h"
#include "Core/AccelByteError.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Item::Item(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Settings) : Credentials(Credentials), Settings(Settings){}

Item::~Item(){}

FString EAccelByteItemTypeToString(const EAccelByteItemType& EnumValue) {
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteItemType"), true);
	if (!EnumPtr) return "Invalid";

	return EnumPtr->GetNameStringByValue((int64)EnumValue);
}

FString EAccelByteItemStatusToString(const EAccelByteItemStatus& EnumValue) {
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteItemStatus"), true);
	if (!EnumPtr) return "Invalid";

	return EnumPtr->GetNameStringByValue((int64)EnumValue);
}

FString EAccelByteAppTypeToString(const EAccelByteAppType& EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteAppType"), true);
	if (!EnumPtr) return "Invalid";

	return EnumPtr->GetNameStringByValue((int64)EnumValue);
}

void Item::GetItemById(const FString& ItemId, const FString& Language, const FString& Region, const THandler<FAccelByteModelsPopulatedItemInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *ItemId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::GetItemByAppId(const FString& AppId, const FString& Language, const FString& Region, const THandler<FAccelByteModelsItemInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byAppId?appId=%s"), *Settings.PlatformServerUrl, *Settings.PublisherNamespace, *AppId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::GetItemsByCriteria(const FAccelByteModelsItemCriteria& ItemCriteria, const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
    FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria"), *Settings.PlatformServerUrl, *Settings.Namespace);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::SearchItem(const FString& Language, const FString& Keyword, const int32& Offset, const int32& Limit, const FString& Region, const THandler<FAccelByteModelsItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/items/search?language=%s&keyword=%s"), *Settings.PlatformServerUrl, *Settings.Namespace, *Language, *FGenericPlatformHttp::UrlEncode(Keyword));
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte