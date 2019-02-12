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

void Item::GetItemById(const FString& ItemId, const FString& Language, const FString& Region, const FGetItemByIdSuccess& OnSuccess, const FErrorHandler& OnError)
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetItemByIdResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Item::GetItemsByCriteria(const FString& Language, const FString& Region, const FString& CategoryPath, const EAccelByteItemType& ItemType, const EAccelByteItemStatus& Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria?categoryPath=%s&language=%s&region=%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language, *Region);
	if (ItemType != EAccelByteItemType::NONE)
	{
		Url.Append(FString::Printf(TEXT("&itemType=%s"), *EAccelByteItemTypeToString(ItemType)));
	}
	if (Status != EAccelByteItemStatus::NONE)
	{
		Url.Append(FString::Printf(TEXT("&status=%"), *EAccelByteItemStatusToString(Status)));
	}
	Url.Append(FString::Printf(TEXT("&status=%d"), Page));
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetItemsByCriteriaResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Item::GetItemByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetItemByIdSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	FAccelByteModelsItemInfo Output;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Output, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Output);
			return;
		}
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Item::GetItemsByCriteriaResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetItemsByCriteriaSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	FAccelByteModelsItemPagingSlicedResult Output;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Output, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Output);
			return;
		}
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte