// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCurrencyApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

Currency::Currency(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{
}

Currency::Currency(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform)
{
}

Currency::~Currency()
{}

FAccelByteTaskWPtr Currency::GetCurrencyList(FString const& Namespace
	, THandler<TArray<FAccelByteModelsCurrencyList>> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteCurrencyType CurrencyType)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/currencies")
		, *SettingsRef.PlatformServerUrl
		, *FGenericPlatformHttp::UrlEncode(Namespace));

	TMultiMap<FString, FString> QueryParams = {};
	if (CurrencyType != EAccelByteCurrencyType::NONE)
	{
		QueryParams.Add(TEXT("currencyType"), *FAccelByteUtilities::GetUEnumValueAsString(CurrencyType));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
