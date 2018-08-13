// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalog.h"
#include "Misc/ConfigCacheIni.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "JusticeSDK.h"
#include "Services/JusticeIdentity.h"
#include "JusticeLog.h"

void JusticeCatalog::GetRootCategory(FString Language, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories?language=%s"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *Language);
	FString Verb			= GET;
	FString ContentType		= TYPE_JSON;
	FString Accept			= TYPE_JSON;
	FString Payload			= TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetRootCategoryResponse, OnComplete));
}

void JusticeCatalog::OnGetRootCategoryResponse(FJusticeHttpResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Root Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FCategory>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;
		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<FCategory> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int32 itCatagory = 0; itCatagory != JsonArray.Num(); itCatagory++)
			{
				FCategory categoryObj;
				if (categoryObj.FromJson(JsonArray[itCatagory]->AsObject()))
				{
					Result.Add(categoryObj);
				}
			}
			OnComplete.ExecuteIfBound(true, ErrorStr, Result);
		}
		else
		{
			ErrorStr = TEXT("unable to deserlize response from server");
		}
		break;
	}
	case EHttpResponseCodes::Denied:
	{
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetRootCategoryResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
				return;
			}
		}));
		return;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetRootCategoryResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
		return;
	}
}

void JusticeCatalog::GetCategory(FString ParentPath, FString Language, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s?language=%s"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FGenericPlatformHttp::UrlEncode(ParentPath), *Language);
	FString Verb			= GET;
	FString ContentType		= TYPE_JSON;
	FString Accept			= TYPE_JSON;
	FString Payload			= TEXT("");

	FJusticeHTTP::CreateRequest(Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetCategoryResponse, OnComplete));
}

void JusticeCatalog::OnGetCategoryResponse(FJusticeHttpResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete)
{	
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FCategory>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;
		UE_LOG(LogJustice, Error, TEXT("OnGetRootCategoryComplete : %s"), *ResponseStr);

		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<FCategory> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int32 itCatagory = 0; itCatagory != JsonArray.Num(); itCatagory++)
			{
				FCategory categoryObj;
				if (categoryObj.FromJson(JsonArray[itCatagory]->AsObject()))
				{
					Result.Add(categoryObj);
				}
			}
			OnComplete.ExecuteIfBound(true, ErrorStr, Result);
		}
		else
		{
			ErrorStr = TEXT("unable to deserlize response from server");
		}
		break;
	}
	case EHttpResponseCodes::Denied:
	{
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetCategoryResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
				return;
			}
		}));
		return;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:	
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetCategoryResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
		return;
	}
}

void JusticeCatalog::GetChildCategory(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s/children?language=%s"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
	FString Verb = GET;
	FString ContentType = TYPE_JSON;
	FString Accept = TYPE_JSON;
	FString Payload = TEXT("");

	FJusticeHTTP::CreateRequest(Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetChildCategoryResponse, OnComplete));
}

void JusticeCatalog::OnGetChildCategoryResponse(FJusticeHttpResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Child Categories Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FCategory>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;

		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<FCategory> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int32 itCatagory = 0; itCatagory != JsonArray.Num(); itCatagory++)
			{
				FCategory categoryObj;
				if (categoryObj.FromJson(JsonArray[itCatagory]->AsObject()))
				{
					Result.Add(categoryObj);
				}
			}
			OnComplete.ExecuteIfBound(true, ErrorStr, Result);
		}
		else
		{
			ErrorStr = TEXT("unable to deserialize response from server");
		}
		break;
	}
	case EHttpResponseCodes::Denied:
	{
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetChildCategoryResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
				return;
			}
		}));
		return;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetChildCategoryResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpected response Code=%d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Children Categories Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
		return;
	}
}

void JusticeCatalog::GetDescendantCategory(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s/descendants?language=%s"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
	FString Verb = GET;
	FString ContentType = TYPE_JSON;
	FString Accept = TYPE_JSON;
	FString Payload = TEXT("");

	FJusticeHTTP::CreateRequest(Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetDescendantCategoryResponse, OnComplete));
}

void JusticeCatalog::OnGetDescendantCategoryResponse(FJusticeHttpResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Descendant Categories Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FCategory>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;
		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<FCategory> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int32 itCatagory = 0; itCatagory != JsonArray.Num(); itCatagory++)
			{
				FCategory categoryObj;
				if (categoryObj.FromJson(JsonArray[itCatagory]->AsObject()))
				{
					Result.Add(categoryObj);
				}
			}
			OnComplete.ExecuteIfBound(true, ErrorStr, Result);
		}
		else
		{
			ErrorStr = TEXT("unable to deserialize response from server");
		}
		break;
	}
	case EHttpResponseCodes::Denied:
	{
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetDescendantCategoryResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
				return;
			}
		}));
		return;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetDescendantCategoryResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpected response Code=%d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Descendant Categories Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FCategory>());
		return;
	}
}

void JusticeCatalog::GetItem(FString ItemID, FString Region, FString Language, FGetItemCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/%s/locale"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *ItemID);
	if (!Region.IsEmpty() || !Language.IsEmpty()) 
	{
		URL.Append(FString::Printf(TEXT("?")));
		if (!Region.IsEmpty())
		{
			URL.Append(FString::Printf(TEXT("region=%s"), *Region));
			if (!Language.IsEmpty())
			{
				URL.Append(FString::Printf(TEXT("&language=%s"), *Language));
			}
		}
		else if (!Language.IsEmpty())
		{
			URL.Append(FString::Printf(TEXT("language=%s"), *Language));
		}
	}

	FString Verb = GET;
	FString ContentType = TYPE_JSON;
	FString Accept = TYPE_JSON;
	FString Payload = TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemResponse, OnComplete));
}

void JusticeCatalog::OnGetItemResponse(FJusticeHttpResponsePtr Response, FGetItemCompleteDelegate OnComplete)
{
	FString ErrorStr;
	FItemInfoJustice Result;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, Result);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->Content);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (Result.FromJson(JsonObject))
			{
				OnComplete.ExecuteIfBound(true, TEXT(""), Result);
			}
			else
			{
				ErrorStr = TEXT("unable to deserialize response from server");
			}
		}
		else
		{
			ErrorStr = TEXT("unable to deserialize response from server");
		}
		break;
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, Result);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, Result);
				return;
			}
		}));
		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, Result);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnGetItemResponse Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, Result);
		return;
	}
}

void JusticeCatalog::GetItemByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FItemCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/byCriteria?categoryPath=%s&language=%s&region=%s"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language, *Region);
	if (!ItemType.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("&itemType=%s"), *ItemType));
	}
	if (!Status.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("&status=%"), *Status));
	}
	URL.Append(FString::Printf(TEXT("&status=%d"), Page));
	if (Size > 0)
	{
		URL.Append(FString::Printf(TEXT("&size=%d"), Size));
	}
	
	FString Verb		= GET;
	FString ContentType = TYPE_JSON;
	FString Accept		= TYPE_JSON;
	FString Payload		= TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemByQueryResponse, OnComplete));
}

void JusticeCatalog::OnGetItemByQueryResponse(FJusticeHttpResponsePtr Response, FItemCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FItemInfoJustice>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->Content);
		TArray<FItemInfoJustice> ArrayResult;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FItemPagingSlicedResult Result;
			if (Result.FromJson(JsonObject))
			{
				for (int32 i = 0; i < Result.Data.Num(); i++)
				{
					ArrayResult.Add(Result.Data[i]);						
				}
				OnComplete.ExecuteIfBound(true, TEXT(""), ArrayResult);
			}
			else 
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
		}
		else
		{
			ErrorStr = TEXT("unable to deserlize response from server");
		}
		break; 
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FItemInfoJustice>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemByQueryResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FItemInfoJustice>());
				return;
			}
		}));
		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FItemInfoJustice>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemByQueryResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnGetItemByQueryResponse Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FItemInfoJustice>());
		return;
	}
}