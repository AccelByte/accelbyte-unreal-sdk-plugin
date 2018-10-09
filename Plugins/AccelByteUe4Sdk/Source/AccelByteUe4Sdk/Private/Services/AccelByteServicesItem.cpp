// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteServicesItem.h"
#include "AccelByteError.h"
#include "AccelByteHttpRetrySystem.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Services
{

void Item::GetItemById(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString ItemId, FString Region, FString Language, FGetItemByIdSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/%s/locale"), *ServerBaseUrl, *Namespace, *ItemId);
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
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetItemByIdResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Item::GetItemsByCriteria(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FGetItemsByCriteriaSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/byCriteria?categoryPath=%s&language=%s&region=%s"), *ServerBaseUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language, *Region);
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

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetItemsByCriteriaResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Item::GetItemByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetItemByIdSuccess OnSuccess, ErrorDelegate OnError)
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
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Item::GetItemsByCriteriaResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetItemsByCriteriaSuccess OnSuccess, ErrorDelegate OnError)
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
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Services
} // Namespace AccelByte