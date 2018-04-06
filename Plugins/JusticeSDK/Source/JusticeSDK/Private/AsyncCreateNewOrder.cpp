// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AsyncCreateNewOrder.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Private/Models/OrderCreate.h"
#include "JusticeSDK.h"

UAsyncCreateNewOrder * UAsyncCreateNewOrder::CreateNewOrder(FString itemId, int Price, FString Currency)
{
	UAsyncCreateNewOrder* Node = NewObject<UAsyncCreateNewOrder>();
	Node->ItemId = itemId;
	Node->Price = Price;
	Node->Currency = Currency;
	return Node;
}

void UAsyncCreateNewOrder::Activate() {
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	//{{justice_url}}/platform/public/namespaces/{ {namespace}} / users / { {user_id}} / orders
	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId + TEXT("/orders"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	UE_LOG(LogJustice, Log, TEXT("Attemp to Create new order: %s"), *Request->GetURL());

	OrderCreate newOrder;
	newOrder.itemId = ItemId;
	newOrder.currencyCode = Currency;
	newOrder.price = Price;
	FString Payload = newOrder.ToJson(false);
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindUObject(this, &UAsyncCreateNewOrder::OnRequestComplete, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePurchase::CreateNewOrder failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());		
		ExecuteOnFailed();
	}

}

void UAsyncCreateNewOrder::ExecuteOnSuccess()
{
	OnSuccess.Broadcast(OrderInformation);
}

void UAsyncCreateNewOrder::ExecuteOnFailed()
{
	OnFailed.Broadcast(NewObject<UOrderInfo>());
}

void UAsyncCreateNewOrder::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
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
		case EHttpResponseCodes::Created:
		{
			FString ResponseStr = Response->GetContentAsString();
			UE_LOG(LogJustice, Log, TEXT("OnCreateNewOrderComplete : %s"), *ResponseStr);

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				OrderInfo info;
				if (info.FromJson(JsonObject))
				{
					OrderInformation = NewObject<UOrderInfo>();
					OrderInformation->FromOrderInfo(info);
					ExecuteOnSuccess();
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
		UE_LOG(LogJustice, Error, TEXT("OnCreateNewOrderComplete Error : %s"), *ErrorStr);
		ExecuteOnFailed();
		return;
	}

}

