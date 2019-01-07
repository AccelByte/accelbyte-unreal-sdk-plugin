// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCategoryApi.h"
#include "AccelByteError.h"
#include "AccelByteRegistry.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Api
{

void Category::GetRootCategories(const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/categories?language=%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *Language);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetRootCategoriesResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Category::GetCategory(const FString& CategoryPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s?language=%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetCategoryResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Category::GetChildCategories(const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/children?language=%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetChildCategoriesResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void Category::GetDescendantCategories(const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/descendants?language=%s"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetDescendantCategoriesResponse, OnSuccess, OnError), FPlatformTime::Seconds());
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
