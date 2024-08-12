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

FAccelByteTaskWPtr Wallet::GetWalletInfoByCurrencyCode(FString const& CurrencyCode
	, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
	, FErrorHandler const& OnError)
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
					WalletInfo.DuplicateTimeLimitedBalancesExpirationInfo();
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr Wallet::GetWalletInfoByCurrencyCodeV2(FString const& CurrencyCode
	, THandler<FAccelByteModelsWalletInfoResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *CurrencyCode);

	const TDelegate<void(const FAccelByteModelsWalletInfoResponse&)> OnSuccessHttpClient =
		THandler<FAccelByteModelsWalletInfoResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsWalletInfoResponse const& WalletInfoResponse)
			{
				FAccelByteModelsWalletInfoResponse Result = WalletInfoResponse;
				for (int i = 0; i < Result.WalletInfos.Num(); i++)
				{
					Result.WalletInfos[i].DuplicateTimeLimitedBalancesExpirationInfo();
				}
				OnSuccess.ExecuteIfBound(Result);
			});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr Wallet::ListWalletTransactionsByCurrencyCode(FString const& CurrencyCode
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
