// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalog.h"
#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"



void JusticeCatalog::GetRootCategory(FGetRootCategoryCompleteDelegate OnComplete)
{

	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	//{{justice_url}}/platform/public/namespaces/{{namespace}}/categories
	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/categories"));
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
		OnComplete.Execute(false, ErrorStr, TArray<Category>());
	}

}

void JusticeCatalog::GetSubCategory(FString ParentPath, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	//{{justice_url}}/platform/public/namespaces/{{namespace}}/categories
	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/categories"));
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
		OnComplete.Execute(false, ErrorStr, TArray<Category>());
	}
}

void JusticeCatalog::GetItemByCategory(FString CategoryPath, FItemCompleteDelegate OnComplete)
{
}

void JusticeCatalog::OnGetRootCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete)
{
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
				OnComplete.Execute(true, ErrorStr, Result);
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
		OnComplete.Execute(false, ErrorStr, TArray<Category>());
		return;
	}
}

void JusticeCatalog::OnGetSubCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete)
{

}
