#include "JusticePurchase.h"
#include "Models/OrderCreate.h"

void JusticePurchase::CreateNewOrder(FString itemId, int Price, int DiscountedPrice, FString Currency, FString StoreId, FCreateNewOrderCompleteDelegate OnComplete)
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
	NewOrderRequest.discountedPrice = DiscountedPrice;

	NewOrderRequest.storeId = StoreId;
	NewOrderRequest.quantity = 1;
	NewOrderRequest.returnUrl = TEXT("https://api.justice.accelbyte.net/");

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders"), *BaseURL, *Namespace, *UserID));	
	Request->SetHeader(TEXT("Authorization"), FJusticeHTTP::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(NewOrderRequest.ToJson());
	Request->OnProcessRequestComplete().BindStatic(JusticePurchase::OnCreateNewOrderComplete, RequestTrace, OnComplete, itemId, Price, DiscountedPrice, Currency, StoreId);
	UE_LOG(LogJustice, Log, TEXT("Attemp to Create new order: %s"), *Request->GetURL());

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		//UE_LOG(LogJustice, Warning, TEXT("JusticePurchase::CreateNewOrder failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, OrderInfo());
	}
}

void JusticePurchase::OnCreateNewOrderComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FCreateNewOrderCompleteDelegate OnComplete, FString itemId, int Price, int DiscountedPrice, FString Currency, FString StoreId)
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
			break;
		}		
		case EHttpResponseCodes::Denied:
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			JusticePurchase::CreateNewOrder(itemId, Price, DiscountedPrice, Currency, StoreId, OnComplete);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d, Content=%s"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnCreateNewOrderComplete Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, OrderInfo());
		return;
	}
}