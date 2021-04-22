// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerEcommerce::ServerEcommerce(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
{}

ServerEcommerce::~ServerEcommerce()
{}

void ServerEcommerce::GetUserEntitlementById(const FString& Entitlementid, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/entitlements/%s"), *Settings.PlatformServerUrl, *Credentials.GetClientNamespace(), *Entitlementid);

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::GrantUserEntitlements(const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements"), *Settings.PlatformServerUrl, *Credentials.GetClientNamespace(), *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents = "[";
	for (int i =0; i < EntitlementGrant.Num(); i++)
	{
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(EntitlementGrant[i], Content);
		Contents += Content;
		if (i < EntitlementGrant.Num() - 1)
		{
			Contents += ",";
		}
		else
		{
			Contents += "]";
		}
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::CreditUserWallet(const FString& UserId, const FString& CurrencyCode, const FAccelByteModelsCreditUserWalletRequest& CreditUserWalletRequest, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit"), *Settings.PlatformServerUrl, *Credentials.GetClientNamespace(), *UserId, *CurrencyCode);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(CreditUserWalletRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::DebitUserWallet(const FString& UserId, const FString& WalletId,
	const FAccelByteModelsDebitUserWalletRequest& DebitUserWalletRequest,
	const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/debit"), *Settings.PlatformServerUrl, *Credentials.GetClientNamespace(), *UserId, *WalletId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(DebitUserWalletRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
} // Namespace GameServerApi
} // Namespace AccelByte
