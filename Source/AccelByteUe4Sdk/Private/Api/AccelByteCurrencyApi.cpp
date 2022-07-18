// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCurrencyApi.h"

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h" 
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

Currency::Currency(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Currency::~Currency()
{}

void Currency::GetCurrencyList(const FString& Namespace, const THandler<TArray<FAccelByteModelsCurrencyList>>& OnSuccess, const FErrorHandler& OnError,
	EAccelByteCurrencyType CurrencyType)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/currencies"), *SettingsRef.PlatformServerUrl, *Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	if (CurrencyType != EAccelByteCurrencyType::NONE)
	{
		FString Query = FString::Printf(TEXT("currencyType=%s"), *FAccelByteUtilities::GetUEnumValueAsString(CurrencyType));
		Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"),*Query));
	}
	
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
} // Namespace AccelByte
