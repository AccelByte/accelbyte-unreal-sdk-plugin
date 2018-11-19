// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteItemApi.h"
#include "AccelByteError.h"
#include "JsonUtilities.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

void Item::GetItemById(const FString& ItemId, const FString& Language, const FString& Region, const FGetItemByIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/%s/locale"), *Settings::PlatformServerUrl, *Credentials::Get().GetUserNamespace(), *ItemId);
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
	Request->OnProcessRequestComplete().BindStatic(GetItemByIdResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Item::GetItemsByCriteria(const FString& Language, const FString& Region, const FString& CategoryPath, const FString& ItemType, const FString& Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/byCriteria?categoryPath=%s&language=%s&region=%s"), *Settings::PlatformServerUrl, *Credentials::Get().GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language, *Region);
	if (!ItemType.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&itemType=%s"), *ItemType));
	}
	if (!Status.IsEmpty())
	{
		Url.Append(FString::Printf(TEXT("&status=%"), *Status));
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
	Request->OnProcessRequestComplete().BindStatic(GetItemsByCriteriaResponse, OnSuccess, OnError);
	Request->ProcessRequest();
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
} // GameId AccelByte