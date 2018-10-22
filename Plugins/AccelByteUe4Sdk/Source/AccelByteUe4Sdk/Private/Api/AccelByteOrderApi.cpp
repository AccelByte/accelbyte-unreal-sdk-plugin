// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteOrderApi.h"
#include "AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

void Order::CreateNewOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders"), *Settings::PlatformServerUrl, *Namespace,  *UserId);
	FString Verb			= TEXT("POST");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(OrderCreate, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(CreateNewOrderResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Order::CreateNewOrderEasy(const FAccelByteModelsOrderCreate& OrderCreate, const FCreateNewOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	CreateNewOrder(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderCreate, OnSuccess, OnError);
}

void Order::GetUserOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s"), *Settings::PlatformServerUrl, *Namespace, *UserId, *OrderNo);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetUserOrderResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Order::GetUserOrderEasy(const FString& OrderNo, const FGetUserOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetUserOrder(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, OnSuccess, OnError);
}

void Order::GetUserOrders(const FString& AccessToken, const FString& Namespace, const FString& UserId, int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders"), *Settings::PlatformServerUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	Url.Append(FString::Printf(TEXT("?page=%d"), Page));
	Url.Append(FString::Printf(TEXT("&size=%d"), Size));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetUserOrdersResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Order::GetUserOrdersEasy(int32 Page, int32 Size, const FGetUserOrdersSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetUserOrders(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Page, Size, OnSuccess, OnError);
}

void Order::FulfillOrder(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s/fulfill"), *Settings::PlatformServerUrl, *Namespace, *UserId, *OrderNo);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(FulfillOrderResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Order::FulfillOrderEasy(const FString& OrderNo, const FFulfillOrderSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FulfillOrder(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, OnSuccess, OnError);
}

void Order::GetUserOrderHistory(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/orders/%s/history"), *Settings::PlatformServerUrl, *Namespace, *UserId, *OrderNo);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetUserOrderHistoryResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Order::GetUserOrderHistoryEasy(const FString& OrderNo, const FGetUserOrderHistorySuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetUserOrderHistory(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, OnSuccess, OnError);
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
