// Copyright (c) 2018 inXile entertainment, Inc. All rights reserved.

#include "AsyncCreateNewOrder.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Private/Models/OrderCreate.h"
#include "JusticeSDK.h"


//UAsyncServerBrowser* UAsyncCreateNewOrder::GetServerList(FString Region) {
//	UAsyncServerBrowser* Node = NewObject<UAsyncServerBrowser>();
//	Node->Region = Region;
//	return Node;
//}
UAsyncCreateNewOrder * UAsyncCreateNewOrder::CreateNewOrder(FString itemId, int Price, FString Currency)
{
	UAsyncCreateNewOrder* Node = NewObject<UAsyncCreateNewOrder>();
	Node->ItemId = itemId;
	Node->Price = Price;
	Node->Currency = Currency;
	return Node;
}


void UAsyncCreateNewOrder::Activate() {
	//TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	//TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	//FString ErrorStr;

	//FString BaseURL;
	//if (!GConfig->GetString(TEXT("InxileServerBrowser"), TEXT("BaseURL"), BaseURL, GEngineIni))
	//{
	//	UE_LOG(LogJustice, Error, TEXT("Missing BaseURL= in [InxileServerBrowser] of DefaultEngine.ini"));
	//}

	//FString UserToken = FJusticeSDKModule::Get().UserToken->AccessToken;
	//Request->SetURL(BaseURL + TEXT("/serverbrowser/public/servers/region/" + Region));
	//Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(UserToken));
	//Request->SetVerb(TEXT("GET"));
	//Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	//UE_LOG(LogServerBrowser, VeryVerbose, TEXT("Attemp to call Player Profile: %s"), *Request->GetURL());

	//Request->OnProcessRequestComplete().BindUObject(this, &UAsyncServerBrowser::OnRequestComplete, FJusticeSDKModule::Get().UserToken->UserId, RequestTrace);
	//if (!Request->ProcessRequest())
	//{
	//	ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	//}
	//if (!ErrorStr.IsEmpty())
	//{
	//	UE_LOG(LogServerBrowser, Warning, TEXT("UFJusticeComponent::RequestCurrentPlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	//	ExecuteOnFailed();
	//}


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
	UE_LOG(LogJustice, Log, TEXT("item: %s  Price: %d   Currency:%s"), *ItemId, Price, *Currency);

	OrderCreate newOrder;
	newOrder.itemId = ItemId;
	newOrder.currencyCode = Currency;
	newOrder.price = Price;
	FString Payload = newOrder.ToJson(false);

	UE_LOG(LogJustice, Log, TEXT("Payload: %s"), *Payload);

	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindUObject(this, &UAsyncCreateNewOrder::OnRequestComplete, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePurchase::CreateNewOrder failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		//OnComplete.Execute(false, ErrorStr, OrderInfo());
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
	//FString ErrorStr;
	//if (!bSuccessful || !Response.IsValid())
	//{
	//	ErrorStr = TEXT("request failed");
	//}

	//if (!ErrorStr.IsEmpty())
	//{
	//	UE_LOG(LogServerBrowser, Error, TEXT("Get Server List Error : %s"), *ErrorStr);
	//	ExecuteOnFailed();
	//	return;
	//}

	//if (Response->GetResponseCode() == EHttpResponseCodes::Ok) {

	//	FString ResponseStr = Response->GetContentAsString();
	//	TArray< TSharedPtr<FJsonValue> > JsonArray;
	//	TSharedPtr<FJsonObject> JsonObject;
	//	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
	//	TArray<FServerItemHelper> ServerStructList;

	//	FJsonObjectConverter::JsonArrayStringToUStruct(ResponseStr, &ServerStructList, 0, 0);
	//	for (int i = 0; i < ServerStructList.Num(); i++)
	//	{
	//		FServerItemHelper item = ServerStructList[i];

	//		UServerItem* newItem = NewObject<UServerItem>();
	//		newItem->LoadFromStruct(item);
	//		ServerList.Add(newItem);
	//	}

	//	ExecuteOnSuccess();
	//}
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
					//OnComplete.Execute(false, ErrorStr, info);
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
		//OnComplete.Execute(false, ErrorStr, OrderInfo());
		ExecuteOnFailed();
		return;
	}


}

