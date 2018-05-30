// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalog.h"
#include "Misc/ConfigCacheIni.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"

void JusticeCatalog::GetRootCategory(FGetRootCategoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	UE_LOG(LogJustice, VeryVerbose, TEXT("Attemp to call GetRootCategory: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(JusticeCatalog::OnGetRootCategoryComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeCatalog::GetRootCategory failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<Category>());
	}
}

void JusticeCatalog::GetSubCategory(FString ParentPath, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s"), *BaseURL, *Namespace, *ParentPath));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(JusticeCatalog::OnGetRootCategoryComplete, RequestTrace, OnComplete);
	UE_LOG(LogJustice, VeryVerbose, TEXT("Attemp to call GetRootCategory: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeCatalog::GetRootCategory failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<Category>());
	}
}

void JusticeCatalog::GetItemByCriteria(FString CategoryPath, FItemCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/byCriteria?categoryPath=%s"), *BaseURL, *Namespace, *CategoryPath));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());	
	Request->OnProcessRequestComplete().BindStatic(JusticeCatalog::OnGetItemByCriteriaComplete, RequestTrace, OnComplete);
	UE_LOG(LogJustice, VeryVerbose, TEXT("Attemp to call GetRootCategory: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeCatalog::GetRootCategory failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<ItemInfo>());
	}
}

void JusticeCatalog::OnGetItemByCriteriaComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FItemCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();
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

						UE_LOG(LogJustice, Log, TEXT("Item Currency And Value:%d %s  "), Result.Data[i].PriceInfo.Value, *Result.Data[i].PriceInfo.CurrencyCode);
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
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<ItemInfo>());
		return;
	}
}

void JusticeCatalog::OnGetRootCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();
			UE_LOG(LogJustice, Error, TEXT("OnGetRootCategoryComplete : %s"), *ResponseStr);

			//UUserProfileJustice* pUserProfileJustice = NewObject<UUserProfileJustice>();
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
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<Category>());
		return;
	}
}