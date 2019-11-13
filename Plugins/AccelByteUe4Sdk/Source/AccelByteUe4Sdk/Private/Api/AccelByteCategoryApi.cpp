// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCategoryApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Category::Category(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting){}

Category::~Category(){}

void Category::GetRootCategories(const FString& Language, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/categories?language=%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Language);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request,  CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Category::GetCategory(const FString& CategoryPath, const FString& Language, const THandler<FAccelByteModelsCategoryInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s?language=%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Category::GetChildCategories(const FString& Language, const FString& CategoryPath, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/children?language=%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Category::GetDescendantCategories(const FString& Language, const FString& CategoryPath, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/descendants?language=%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(CategoryPath), *Language);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
