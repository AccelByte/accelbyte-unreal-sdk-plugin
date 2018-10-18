// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCategoryApi.h"
#include "AccelByteError.h"
#include "JsonUtilities.h"
#include "AccelByteHttpRetrySystem.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

namespace AccelByte
{
namespace Api
{

void Category::GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories?language=%s"), *ServerBaseUrl, *Namespace, *Language);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content			= TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetRootCategoriesResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Category::GetRootCategoriesEasy(FString Language, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	GetRootCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, OnSuccess, OnError);
}

void Category::GetCategory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString CategoryPath, FString Language, FGetCategorySuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s?language=%s"), *ServerBaseUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content			= TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetCategoryResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Category::GetCategoryEasy(FString CategoryPath, FString Language, FGetCategorySuccess OnSuccess, ErrorDelegate OnError)
{
	GetCategory(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), CategoryPath, Language, OnSuccess, OnError);
}

void Category::GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s/children?language=%s"), *ServerBaseUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetChildCategoriesResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Category::GetChildCategoriesEasy(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	GetChildCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}

void Category::GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/categories/%s/descendants?language=%s"), *ServerBaseUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetDescendantCategoriesResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Category::GetDescendantCategoriesEasy(FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	GetDescendantCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Category::GetRootCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	TArray<FAccelByteModelsFullCategoryInfo> Result;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
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

void Category::GetCategoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetCategorySuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	FAccelByteModelsFullCategoryInfo Result;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
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

void Category::GetChildCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TArray<FAccelByteModelsFullCategoryInfo> Result;
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

void Category::GetDescendantCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	TArray<FAccelByteModelsFullCategoryInfo> Result;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
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
