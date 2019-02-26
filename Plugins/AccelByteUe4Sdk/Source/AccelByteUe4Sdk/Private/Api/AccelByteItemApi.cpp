// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteItemApi.h"
#include "AccelByteError.h"
#include "JsonUtilities.h"
#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace Api
{

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

void Item::GetItemById(const FString& ItemId, const FString& Language, const FString& Region, const THandler<FAccelByteModelsItemInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *ItemId);
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

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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

void Item::GetItemsByCriteria(const FString& Language, const FString& Region, const FString& CategoryPath, const EAccelByteItemType& ItemType, const EAccelByteItemStatus& Status, int32 Page, int32 Size, const THandler<FAccelByteModelsItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria?categoryPath=%s&region=%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Settings.Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Region);
	if (!Language.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&language=%s"), *Language));
	}	
	if (ItemType != EAccelByteItemType::NONE)
	{
		Url.Append(FString::Printf(TEXT("&itemType=%s"), *EAccelByteItemTypeToString(ItemType)));
	}
	Url.Append(FString::Printf(TEXT("&page=%d"), Page));
	if (Size > 0)
	{
		Url.Append(FString::Printf(TEXT("&size=%d"), Size));
	}
	FString Verb		= TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept		= TEXT("application/json");
	FString Content		= TEXT("");

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