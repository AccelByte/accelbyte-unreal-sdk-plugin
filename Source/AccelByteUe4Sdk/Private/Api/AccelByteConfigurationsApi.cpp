// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteConfigurationsApi.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
namespace Api
{

Configurations::Configurations(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

Configurations::~Configurations()
{}

FAccelByteTaskWPtr Configurations::GetAll(THandler<TArray<FAccelByteModelsConfiguration>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/configs")
		, *SettingsRef.ConfigServerUrl
		, *FGenericPlatformHttp::UrlEncode(SettingsRef.Namespace));

	return HttpClient.Request(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr Configurations::Get(FString const& Key
	, THandler<FAccelByteModelsConfiguration> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key for common config can't be empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/configs/%s")
		, *SettingsRef.ConfigServerUrl
		, *FGenericPlatformHttp::UrlEncode(SettingsRef.Namespace)
		, *FGenericPlatformHttp::UrlEncode(Key));

	return HttpClient.Request(TEXT("GET"), Url, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
