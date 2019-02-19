// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteOrderApi.h"
#include "AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace Api
{

void Order::CreateNewOrder(const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(),  *FRegistry::Credentials.GetUserId());
	FString Verb			= TEXT("POST");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(OrderCreate, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(CreateNewOrderResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Order::GetUserOrder(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *OrderNo);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetUserOrderResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Order::GetUserOrders(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	Url.Append(FString::Printf(TEXT("?page=%d"), Page));
	Url.Append(FString::Printf(TEXT("&size=%d"), Size));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetUserOrdersResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Order::FulfillOrder(const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s/fulfill"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *OrderNo);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(FulfillOrderResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Order::GetUserOrderHistory(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s/history"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId(), *OrderNo);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetUserOrderHistoryResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Order::CreateNewOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateNewOrderSuccess OnSuccess, FErrorHandler OnError)
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
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Order::GetUserOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrderSuccess OnSuccess, FErrorHandler OnError)
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
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Order::GetUserOrdersResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrdersSuccess OnSuccess, FErrorHandler OnError)
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
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Order::FulfillOrderResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FFulfillOrderSuccess OnSuccess, FErrorHandler OnError)
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
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Order::GetUserOrderHistoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserOrderHistorySuccess OnSuccess, FErrorHandler OnError)
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
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
