#include "JusticePurchase.h"
#include "Private/Models/OrderCreate.h"

void JusticePurchase::CreateNewOrder(FString itemId, int Price, FString Currency, FCreateNewOrderCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	OrderCreate NewOrderRequest;
	NewOrderRequest.itemId = itemId;
	NewOrderRequest.currencyCode = Currency;
	NewOrderRequest.price = Price;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders"), *BaseURL, *Namespace, *UserID));	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(NewOrderRequest.ToJson());
	Request->OnProcessRequestComplete().BindStatic(JusticePurchase::OnCreateNewOrderComplete, RequestTrace, OnComplete);
	UE_LOG(LogJustice, Log, TEXT("Attemp to Create new order: %s"), *Request->GetURL());

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePurchase::CreateNewOrder failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, OrderInfo());
	}
}

void JusticePurchase::OnCreateNewOrderComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FCreateNewOrderCompleteDelegate OnComplete)
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
					OnComplete.ExecuteIfBound(true, TEXT(""), info);
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
		OnComplete.ExecuteIfBound(false, ErrorStr, OrderInfo());
		return;
	}
}