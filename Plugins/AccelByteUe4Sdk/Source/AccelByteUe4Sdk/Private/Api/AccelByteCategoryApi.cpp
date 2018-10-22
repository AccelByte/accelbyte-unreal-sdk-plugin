// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCategoryApi.h"
#include "AccelByteError.h"
#include "JsonUtilities.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

namespace AccelByte
{
namespace Api
{

void Category::GetRootCategories(const FString& AccessToken, const FString& Namespace, const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/categories?language=%s"), *Settings::PlatformServerUrl, *Namespace, *Language);
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
	Request->OnProcessRequestComplete().BindStatic(GetRootCategoriesResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Category::GetRootCategoriesEasy(const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetRootCategories(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, OnSuccess, OnError);
}

void Category::GetCategory(const FString& AccessToken, const FString& Namespace, const FString& CategoryPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s?language=%s"), *Settings::PlatformServerUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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
	Request->OnProcessRequestComplete().BindStatic(GetCategoryResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Category::GetCategoryEasy(const FString& CategoryPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetCategory(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), CategoryPath, Language, OnSuccess, OnError);
}

void Category::GetChildCategories(const FString& AccessToken, const FString& Namespace, const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/children?language=%s"), *Settings::PlatformServerUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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
	Request->OnProcessRequestComplete().BindStatic(GetChildCategoriesResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Category::GetChildCategoriesEasy(const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetChildCategories(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}

void Category::GetDescendantCategories(const FString& AccessToken, const FString& Namespace, const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/descendants?language=%s"), *Settings::PlatformServerUrl, *Namespace, *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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
	Request->OnProcessRequestComplete().BindStatic(GetDescendantCategoriesResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Category::GetDescendantCategoriesEasy(const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetDescendantCategories(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Category::GetRootCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetRootCategoriesSuccess OnSuccess, FErrorHandler OnError)
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

void Category::GetCategoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetCategorySuccess OnSuccess, FErrorHandler OnError)
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

void Category::GetChildCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetChildCategoriesSuccess OnSuccess, FErrorHandler OnError)
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

void Category::GetDescendantCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetDescendantCategoriesSuccess OnSuccess, FErrorHandler OnError)
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
