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

void JusticeCatalog::GetRootCategory(FString Language, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories?language=%s"), *FJusticeBaseURL, *FJusticeNamespace, *Language);
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
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetRootCategoryComplete, OnComplete));
}

void JusticeCatalog::OnGetRootCategoryComplete(FJusticeHttpResponsePtr Response, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Root Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<Category>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;
		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<Category> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int itCatagory = 0; itCatagory != JsonArray.Num(); itCatagory++)
			{
				Category categoryObj;
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
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), TArray<Category>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetRootCategoryComplete, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<Category>());
		return;
	}
}

void JusticeCatalog::GetCategory(FString ParentPath, FString Language, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s?language=%s"), *FJusticeBaseURL, *FJusticeNamespace, *ParentPath, *Language);
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
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetCategoryComplete, OnComplete));
}

void JusticeCatalog::OnGetCategoryComplete(FJusticeHttpResponsePtr Response, FGetRootCategoryCompleteDelegate OnComplete)
{	
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<Category>());
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
		TArray<Category> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int itCatagory = 0; itCatagory != JsonArray.Num(); itCatagory++)
			{
				Category categoryObj;
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
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:	
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), TArray<Category>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetCategoryComplete, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<Category>());
		return;
	}
}

void JusticeCatalog::GetItemByQuery(FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size, FItemCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/byCriteria?categoryPath=%s&language=%s&region=%s"), *FJusticeBaseURL, *FJusticeNamespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *language, *region);
	if (!itemType.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("&itemType=%s"), *itemType));
	}
	if (!status.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("&status=%"), *status));
	}
	URL.Append(FString::Printf(TEXT("&status=%d"), page));
	if (size > 0)
	{
		URL.Append(FString::Printf(TEXT("&size=%d"), size));
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
		FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemByQueryComplete, OnComplete));
}

void JusticeCatalog::OnGetItemByQueryComplete(FJusticeHttpResponsePtr Response, FItemCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Category Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<ItemInfo>());
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;
		UE_LOG(LogJustice, Log, TEXT("OnGetItemByCriteriaComplete : %s"), *ResponseStr);
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<ItemInfo> ArrayResult;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			ItemPagingSlicedResult Result;
			if (Result.FromJson(JsonObject))
			{
				for (int i = 0; i < Result.Data.Num(); i++)
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
			FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (IsSuccess)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), TArray<ItemInfo>());
						return;
					}
					//retry 
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemByQueryComplete, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("You token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<ItemInfo>());
				return;
			}
		}));
		break;
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), TArray<ItemInfo>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeCatalog::OnGetItemByQueryComplete, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnGetItemByQueryComplete Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<ItemInfo>());
		return;
	}
}