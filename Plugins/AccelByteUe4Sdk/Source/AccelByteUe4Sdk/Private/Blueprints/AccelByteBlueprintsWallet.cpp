// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsWallet.h"
#include "AccelByteSettings.h"
#include "AccelByteApiWallet.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Wallet;
using AccelByte::ErrorDelegate;
using AccelByte::Settings;
using AccelByte::CredentialStore;

void UAccelByteBlueprintsWallet::GetWalletInfoByCurrencyCode(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString CurrencyCode, FGetWalletByCurrencyCodeSuccess OnSuccess, FBlueprintError OnError)
{
	Wallet::GetWalletInfoByCurrencyCode(ServerBaseUrl, AccessToken, Namespace, UserId, CurrencyCode, Wallet::FGetWalletByCurrencyCodeSuccess::CreateLambda([OnSuccess](const FAccelByteModelsWalletInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsWallet::GetWalletInfoByCurrencyCodeEasy(FString CurrencyCode, FGetWalletByCurrencyCodeSuccess OnSuccess, FBlueprintError OnError)
{
	GetWalletInfoByCurrencyCode(Settings::ServerBaseUrl, CredentialStore.GetUserAccessToken(), CredentialStore.GetUserNamespace(), CredentialStore.GetUserId(), CurrencyCode, OnSuccess, OnError);
}
