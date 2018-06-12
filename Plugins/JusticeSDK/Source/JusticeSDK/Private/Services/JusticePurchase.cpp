#include "JusticePurchase.h"
#include "Models/OrderCreate.h"
#include "Services/JusticeIdentity.h"

void JusticePurchase::CreateNewOrder(FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency, FString StoreId, FCreateNewOrderCompleteDelegate OnComplete)
{
	OrderCreate NewOrderRequest;
	NewOrderRequest.ItemID = ItemID;
	NewOrderRequest.CurrencyCode = Currency;
	NewOrderRequest.Price = Price;
	NewOrderRequest.DiscountedPrice = DiscountedPrice;
	NewOrderRequest.StoreID = StoreId;
	NewOrderRequest.Quantity = 1;
	NewOrderRequest.ReturnURL = TEXT("https://api.justice.accelbyte.net/");

	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders"), *FJusticeBaseURL, *FJusticeNamespace,  *FJusticeUserID);
	FString Verb			= POST;
	FString ContentType		= TYPE_JSON;
	FString Accept			= TYPE_JSON;
	FString Payload			= NewOrderRequest.ToJson();

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnCreateNewOrderResponse, OnComplete));
}

void JusticePurchase::OnCreateNewOrderResponse(FJusticeHttpResponsePtr Response, FCreateNewOrderCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Create new order Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	case EHttpResponseCodes::Created:
	{
		OrderInfo* Info = new OrderInfo();		
		if (Info->FromJson(Response->Content))
		{
			OnComplete.ExecuteIfBound(true, TEXT(""), Info);
		}
		else
		{
			ErrorStr = TEXT("unable to deserlize response from server");
		}
		break;
	}		
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
						return;
					}
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnCreateNewOrderResponse, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), nullptr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnCreateNewOrderResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT(" Create NewOrder Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}