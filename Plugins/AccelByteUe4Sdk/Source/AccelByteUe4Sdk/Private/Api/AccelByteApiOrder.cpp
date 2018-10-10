// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteApiOrder.h"
#include "AccelByteApiIdentity.h"
#include "AccelByteHttpRetrySystem.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Api
{

void Order::CreateNewOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsOrderCreate& OrderCreate, FCreateNewOrderSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders"), *ServerBaseUrl, *Namespace,  *UserId);
	FString Verb			= TEXT("POST");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(OrderCreate, Content);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		CreateNewOrderResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Order::GetUserOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders/%s"), *ServerBaseUrl, *Namespace, *UserId, *OrderNo);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetUserOrderResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Order::GetUserOrders(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, int32 Page, int32 Size, FGetUserOrdersSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	Url.Append(FString::Printf(TEXT("?page=%d"), Page));
	Url.Append(FString::Printf(TEXT("&size=%d"), Size));

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetUserOrdersResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Order::FulfillOrder(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FFulfillOrderSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders/%s/fulfill"), *ServerBaseUrl, *Namespace, *UserId, *OrderNo);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		FulfillOrderResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Order::GetUserOrderHistory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString OrderNo, FGetUserOrderHistorySuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/orders/%s/history"), *ServerBaseUrl, *Namespace, *UserId, *OrderNo);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetUserOrderHistoryResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Order::CreateNewOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateNewOrderSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOrderInfo Result;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Result);
			return;
		}
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Order::GetUserOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserOrderSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOrderInfo Result;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Result);
			return;
		}
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Order::GetUserOrdersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserOrdersSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOrderInfoPaging Result;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Result);
			return;
		}
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Order::FulfillOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FFulfillOrderSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOrderInfo Result;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Result);
			return;
		}
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Order::GetUserOrderHistoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserOrderHistorySuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TArray<FAccelByteModelsOrderHistoryInfo> Result;
		if (FJsonObjectConverter::JsonArrayStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Result);
			return;
		}
		else
		{
			Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
		}
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
