// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteFulfillmentApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Fulfillment::Fulfillment(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Fulfillment::~Fulfillment()
{}

void Fulfillment::RedeemCode(FString const& Code
	, FString const& Region
	, FString const& Language
	, THandler<FAccelByteModelsFulfillmentResult> OnSuccess
	, FErrorHandler OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/fulfillment/code")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	FString Content = FString::Printf(TEXT("{\"code\":\"%s\""), *Code);
	if(!Region.IsEmpty())
	{
		Content += FString::Printf(TEXT(", \"region\":\"%s\""), *Region);
	}
	if (!Language.IsEmpty())
	{
		Content += FString::Printf(TEXT(", \"language\":\"%s\""), *Language);
	}
	Content += "}";

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

} // Namespace Api
}
