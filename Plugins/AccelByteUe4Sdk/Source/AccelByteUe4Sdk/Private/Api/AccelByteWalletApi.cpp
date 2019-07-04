// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteWalletApi.h"

#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Wallet::Wallet(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Settings) : Credentials(Credentials), Settings(Settings){}

Wallet::~Wallet(){}

void Wallet::GetWalletInfoByCurrencyCode(const FString& CurrencyCode, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s"), *Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *Credentials.GetUserId(), *CurrencyCode);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content			= TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
