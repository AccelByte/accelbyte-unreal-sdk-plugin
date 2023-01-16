// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCategoryApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Category::Category(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Category::~Category()
{}

void Category::GetRootCategories(const FString& Language
	, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(Language));

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Category::GetCategory(const FString& CategoryPath
	, const FString& Language
	, const THandler<FAccelByteModelsCategoryInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(CategoryPath)
		, *FGenericPlatformHttp::UrlEncode(Language));

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Category::GetChildCategories(const FString& Language
	, const FString& CategoryPath
	, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/children?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(CategoryPath)
		, *FGenericPlatformHttp::UrlEncode(Language));

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Category::GetDescendantCategories(const FString& Language
	, const FString& CategoryPath
	, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/descendants?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(CategoryPath)
		, *FGenericPlatformHttp::UrlEncode(Language));

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
