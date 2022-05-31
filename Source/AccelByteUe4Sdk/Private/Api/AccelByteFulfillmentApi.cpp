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
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Fulfillment::~Fulfillment()
{}

void Fulfillment::RedeemCode(FString const& Code, FString const& Region, FString const& Language, THandler<FAccelByteModelsFulfillmentResult> OnSuccess, FErrorHandler OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/fulfillment/code"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
	
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
}
