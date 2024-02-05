// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAMSApi.h"
#include "Core/AccelByteReport.h"

AccelByte::Api::AMS::AMS(Credentials const& InCredentialsRef
						, Settings const& InSettingsRef
						, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

AccelByte::Api::AMS::~AMS()
{}

void AccelByte::Api::AMS::GetAccount(THandler<FAccelByteModelsAMSGetAccountResponse> const& OnSuccess,
	FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/{namespace}/account")
		, *SettingsRef.AMSServerUrl);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
