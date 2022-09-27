// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteWalletApi.h"

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Wallet::Wallet(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Wallet::~Wallet()
{}

void Wallet::GetWalletInfoByCurrencyCode(const FString& CurrencyCode, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This does not support for multiplatform wallet, use GetWalletInfoByCurrencyCodeV2 instead.");

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *CurrencyCode);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(THandler<FAccelByteModelsWalletInfoResponse>::CreateLambda([OnSuccess](FAccelByteModelsWalletInfoResponse const& WalletInfoResponse)
	{
		FAccelByteModelsWalletInfo WalletInfo{};
		if (WalletInfoResponse.WalletInfos.Num() > 0)
		{
			WalletInfo = *WalletInfoResponse.WalletInfos.GetData();
		}
		else
		{
			WalletInfo.Id = WalletInfoResponse.Id;
			WalletInfo.Namespace = WalletInfoResponse.Namespace;
			WalletInfo.UserId = WalletInfoResponse.UserId;
			WalletInfo.CurrencyCode = WalletInfoResponse.CurrencyCode;
			WalletInfo.CurrencySymbol = WalletInfoResponse.CurrencySymbol;
			WalletInfo.Balance = WalletInfoResponse.Balance;
			WalletInfo.Status = WalletInfoResponse.Status;
		}
		OnSuccess.ExecuteIfBound(WalletInfo);
	}),
	OnError), FPlatformTime::Seconds());
}

void Wallet::GetWalletInfoByCurrencyCodeV2(const FString& CurrencyCode, const THandler<FAccelByteModelsWalletInfoResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *CurrencyCode);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Wallet::ListWalletTransactionsByCurrencyCode(FString const& CurrencyCode, THandler<FAccelByteModelsWalletTransactionPaging> const& OnSuccess, FErrorHandler const& OnError, int32 Offset, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s/transactions?offset=%d&limit=%d"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *CurrencyCode, Offset, Limit);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
