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
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Wallet::~Wallet()
{}

void Wallet::GetWalletInfoByCurrencyCode(const FString& CurrencyCode
	, const THandler<FAccelByteModelsWalletInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This does not support for multiplatform wallet, use GetWalletInfoByCurrencyCodeV2 instead.");

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *CurrencyCode);

	const TDelegate<void(const FAccelByteModelsWalletInfoResponse&)> OnSuccessHttpClient =
		THandler<FAccelByteModelsWalletInfoResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsWalletInfoResponse const& WalletInfoResponse)
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
			});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void Wallet::GetWalletInfoByCurrencyCodeV2(const FString& CurrencyCode
	, const THandler<FAccelByteModelsWalletInfoResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *CurrencyCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Wallet::ListWalletTransactionsByCurrencyCode(FString const& CurrencyCode
	, THandler<FAccelByteModelsWalletTransactionPaging> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s/transactions?offset=%d&limit=%d")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *CurrencyCode
		, Offset
		, Limit);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
