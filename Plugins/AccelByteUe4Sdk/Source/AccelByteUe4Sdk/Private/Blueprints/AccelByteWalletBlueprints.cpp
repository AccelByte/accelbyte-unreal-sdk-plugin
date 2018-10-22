// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteWalletBlueprints.h"
#include "AccelByteSettings.h"
#include "AccelByteWalletApi.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Wallet;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsWallet::GetWalletInfoByCurrencyCode(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& CurrencyCode, const FGetWalletByCurrencyCodeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Wallet::GetWalletInfoByCurrencyCode(AccessToken, Namespace, UserId, CurrencyCode, Wallet::FGetWalletByCurrencyCodeSuccess::CreateLambda([OnSuccess](const FAccelByteModelsWalletInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsWallet::GetWalletInfoByCurrencyCodeEasy(const FString& CurrencyCode, const FGetWalletByCurrencyCodeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetWalletInfoByCurrencyCode(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), CurrencyCode, OnSuccess, OnError);
}
