// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteConfigurationsApi.h"
#include "Core/AccelByteReport.h"

AccelByte::Api::Configurations::Configurations(Credentials const& InCredentialsRef
												, Settings const& InSettingsRef
												, FHttpRetryScheduler& InHttpRef)
		: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

AccelByte::Api::Configurations::~Configurations()
{}

void AccelByte::Api::Configurations::GetAll(const THandler<TArray<FAccelByteModelsConfiguration>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/configs")
		, *SettingsRef.ConfigServerUrl);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void AccelByte::Api::Configurations::Get(const FString& Key
	, const THandler<FAccelByteModelsConfiguration>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key for common config can't be empty"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/configs/%s")
		, *SettingsRef.ConfigServerUrl
		, *Key);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
