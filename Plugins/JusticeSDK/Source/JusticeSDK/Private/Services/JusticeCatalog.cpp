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


// Get Item By Query Retry Task
class FGetItemByQueryRetryTask : public FJusticeRetryTask
{
public:
	FGetItemByQueryRetryTask(FString language, 
		FString region, 
		FString CategoryPath, 
		FString itemType, 
		FString status, 
		int page, 
		int size, 
		FItemCompleteDelegate onComplete,
		int nextRetry = 1,
		int totalElapsedWait = 0) : FJusticeRetryTask(nextRetry, totalElapsedWait),
		OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticeCatalog::GetItemByQuery(Language,
			Region,
			CategoryPath,
			ItemType,
			Status,
			Page,
			Size, 
			FItemCompleteDelegate::CreateLambda([&](bool IsSucessful, FString ErrorString, TArray<ItemInfo> Result) {
			if (IsSucessful)
			{
				OnComplete.ExecuteIfBound(IsSucessful, ErrorString, Result);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"), TArray<ItemInfo>());
				}
				else
				{
					FGetItemByQueryRetryTask* RetryTask = new FGetItemByQueryRetryTask(Language,
						Region,
						CategoryPath,
						ItemType,
						Status,
						Page,
						Size, 
						OnComplete,
						GetLastWait() * 2, // wait more longer for next retry
						GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));
	}
private:
	FString Language;
	FString Region;
	FString CategoryPath;
	FString ItemType;
	FString Status;
	int Page;
	int Size;
	FItemCompleteDelegate OnComplete;
};


void JusticeCatalog::GetRootCategory(FString Language, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories?language=%s"), *BaseURL, *Namespace, *Language));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	UE_LOG(LogJustice, VeryVerbose, TEXT("Attemp to call GetRootCategory: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(JusticeCatalog::OnGetRootCategoryComplete, RequestTrace, OnComplete, Language);
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

void JusticeCatalog::OnGetRootCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete, FString Language)
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
			JusticeCatalog::GetRootCategory(Language, OnComplete);
			return;
		}
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



void JusticeCatalog::GetCategory(FString ParentPath, FString Language, FGetRootCategoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s?language=%s"), *BaseURL, *Namespace, *ParentPath, *Language));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(JusticeCatalog::OnGetCategoryComplete, RequestTrace, OnComplete, ParentPath, Language);
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

void JusticeCatalog::OnGetCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete, FString ParentPath, FString Language)
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
			JusticeCatalog::GetCategory(ParentPath, Language, OnComplete);
			return;
		}
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

void JusticeCatalog::GetItemByQuery(FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size, FItemCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;


	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/items/byCriteria?categoryPath=%s&language=%s&region=%s"), *BaseURL, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *language, *region);
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
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());	
	Request->OnProcessRequestComplete().BindStatic(JusticeCatalog::OnGetItemByQueryComplete, RequestTrace, OnComplete, language, region, CategoryPath, itemType, status, page, size);
	UE_LOG(LogJustice, VeryVerbose, TEXT("Attemp to call GetRootCategory: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Log, TEXT("JusticeCatalog::GetRootCategory failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<ItemInfo>());
	}
}

void JusticeCatalog::OnGetItemByQueryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FItemCompleteDelegate OnComplete, FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size)
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

						//UE_LOG(LogJustice, Log, TEXT("Item Currency And Value:%d %s  "), Result.Data[i].PriceInfo.Value, *Result.Data[i].PriceInfo.CurrencyCode);
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
			JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete, language, region, CategoryPath, itemType, status, page, size, &ErrorStr](bool IsSuccess,
				FString ErrorStr,
				UOAuthTokenJustice* Token) {
				if (IsSuccess)
				{
					if (Token->Bans.Num() > 0)
					{
						FString bansList = FString::Join(Token->Bans, TEXT(","));
						ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					}
					else
					{
						FGetItemByQueryRetryTask* RetryTask = new FGetItemByQueryRetryTask(language, region, CategoryPath, itemType, status, page, size, OnComplete);
						FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
						return;
					}
				}
				else
				{
					//add to queue: refresh token, if success then retry (register new player)
					FGetItemByQueryRetryTask* RetryTask = new FGetItemByQueryRetryTask(language, region, CategoryPath, itemType, status, page, size, OnComplete);
					FRefreshTokenRetryTask* RefreshTokenTask = new FRefreshTokenRetryTask(RetryTask);
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RefreshTokenTask);
					return;
				}
			}));
			break;
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			FGetItemByQueryRetryTask* RetryTask = new FGetItemByQueryRetryTask(language, region, CategoryPath, itemType, status, page, size, OnComplete);
			FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnGetItemByQueryComplete Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<ItemInfo>());
		return;
	}
}