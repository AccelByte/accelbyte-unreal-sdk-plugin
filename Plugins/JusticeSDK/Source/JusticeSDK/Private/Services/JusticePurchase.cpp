#include "JusticePurchase.h"
#include "Models/OrderCreate.h"
#include "Services/JusticeIdentity.h"

void JusticePurchase::CreateNewOrder(FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency, FOrderInfoCompleteDelegate OnComplete)
{
	FOrderCreate NewOrderRequest;
	NewOrderRequest.ItemID = ItemID;
	NewOrderRequest.CurrencyCode = Currency;
	NewOrderRequest.Price = Price;
	NewOrderRequest.DiscountedPrice = DiscountedPrice;
	NewOrderRequest.Quantity = 1;
	NewOrderRequest.ReturnURL = TEXT("https://api.justice.accelbyte.net/");

	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders?isSandbox=true"), *FJusticeBaseURL, *FJusticeUserToken->Namespace,  *FJusticeUserID);
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

void JusticePurchase::OnCreateNewOrderResponse(FJusticeHttpResponsePtr Response, FOrderInfoCompleteDelegate OnComplete)
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
		FOrderInfo* Info = new FOrderInfo();		
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
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
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
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
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

void JusticePurchase::GetUserOrder(FString OrderNo, FOrderInfoCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders/%s"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FJusticeUserID, *OrderNo);
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
		FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrderResponse, OnComplete));
}

void JusticePurchase::OnGetUserOrderResponse(FJusticeHttpResponsePtr Response, FOrderInfoCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get user's order Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	case EHttpResponseCodes::Created:
	{
		FOrderInfo* Info = new FOrderInfo();
		if (Info->FromJson(Response->Content))
		{
			OnComplete.ExecuteIfBound(true, TEXT(""), Info);
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
					OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrderResponse, OnComplete));
				return;
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
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrderResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT(" Get User Order Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}

void JusticePurchase::GetUserOrders(int32 Page, int32 Size, FGetUserOrdersCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FJusticeUserID);
	FString Verb = GET;
	FString ContentType = TYPE_JSON;
	FString Accept = TYPE_JSON;
	FString Payload = TEXT("");
	URL.Append(FString::Printf(TEXT("?page=%d"), Page));
	URL.Append(FString::Printf(TEXT("&size=%d"), Size));

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrdersResponse, OnComplete));
}

void JusticePurchase::OnGetUserOrdersResponse(FJusticeHttpResponsePtr Response, FGetUserOrdersCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get user's orders Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FOrderInfo>());
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	case EHttpResponseCodes::Created:
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->Content);
		TArray<FOrderInfo> ArrayResult;

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FOrderInfoPaging Result;
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
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderInfo>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrdersResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderInfo>());
				return;
			}
		}));
		return;
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
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderInfo>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrdersResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT(" Get User Orders Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderInfo>());
		return;
	}
}

void JusticePurchase::FulfillOrder(FString OrderNo, FOrderInfoCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders/%s/fulfill"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FJusticeUserID, *OrderNo);
	FString Verb = PUT;
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
		FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnFulfillOrderResponse, OnComplete));
}

void JusticePurchase::OnFulfillOrderResponse(FJusticeHttpResponsePtr Response, FOrderInfoCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Fulfill Order Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	case EHttpResponseCodes::Created:
	{
		FOrderInfo* Info = new FOrderInfo();
		if (Info->FromJson(Response->Content))
		{
			OnComplete.ExecuteIfBound(true, TEXT(""), Info);
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
					OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnFulfillOrderResponse, OnComplete));
				return;
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
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnFulfillOrderResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT(" Fulfill Order Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}

void JusticePurchase::GetUserOrderHistory(FString OrderNo, FGetUserOrderHistoryCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders/%s/history"), *FJusticeBaseURL, *FJusticeUserToken->Namespace, *FJusticeUserID, *OrderNo);
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
		FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrderHistory, OnComplete));
}

void JusticePurchase::OnGetUserOrderHistory(FJusticeHttpResponsePtr Response, FGetUserOrderHistoryCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get User Order Histroy Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, TArray<FOrderHistoryInfo>());
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	case EHttpResponseCodes::Created:
	{
		FString ResponseStr = Response->Content;
		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
		TArray<FOrderHistoryInfo> Result;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
			for (int32 i = 0; i != JsonArray.Num(); i++)
			{
				FOrderHistoryInfo OrderHistory;
				if (OrderHistory.FromJson(JsonArray[i]->AsObject()))
				{
					Result.Add(OrderHistory);
				}
			}
			OnComplete.ExecuteIfBound(true, ErrorStr, Result);
		}
		else
		{
			ErrorStr = TEXT("unable to deserialize response from server");
		}
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderHistoryInfo>());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrderHistory, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderHistoryInfo>());
				return;
			}
		}));
		return;
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
			OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderHistoryInfo>());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePurchase::OnGetUserOrderHistory, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT(" Get User Order History Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FOrderHistoryInfo>());
		return;
	}
}
