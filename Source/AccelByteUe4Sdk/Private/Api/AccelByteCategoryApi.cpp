// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCategoryApi.h"
#include "Core/AccelByteError.h"

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
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

Category::~Category()
{}


FAccelByteTaskWPtr Category::GetRootCategories(FString const& Language
	, THandler<TArray<FAccelByteModelsCategoryInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(Language));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Category::GetCategory(FString const& CategoryPath
	, FString const& Language
	, THandler<FAccelByteModelsCategoryInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(CategoryPath)
		, *FGenericPlatformHttp::UrlEncode(Language));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Category::GetChildCategories(FString const& Language
	, FString const& CategoryPath
	, THandler<TArray<FAccelByteModelsCategoryInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/children?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(CategoryPath)
		, *FGenericPlatformHttp::UrlEncode(Language));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Category::GetDescendantCategories(FString const& Language
	, FString const& CategoryPath
	, THandler<TArray<FAccelByteModelsCategoryInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/categories/%s/descendants?language=%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(CategoryPath)
		, *FGenericPlatformHttp::UrlEncode(Language));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
