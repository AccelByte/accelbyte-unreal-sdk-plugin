// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAMSApi.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"

namespace AccelByte
{
namespace Api
{

AMS::AMS(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{
}

AMS::AMS(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform)
{
}

AMS::~AMS()
{
}

FAccelByteTaskWPtr AMS::GetAccount(THandler<FAccelByteModelsAMSGetAccountResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/{namespace}/account")
		, *SettingsRef.AMSServerUrl);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

}
}