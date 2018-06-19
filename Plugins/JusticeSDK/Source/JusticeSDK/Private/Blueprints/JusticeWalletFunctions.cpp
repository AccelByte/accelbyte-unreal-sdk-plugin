// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWalletFunctions.h"
#include "JusticeWallet.h"

void UJusticeWalletFunctions::GetWalletBalance(FString CurrencyCode, FGetWalletCompleteDelegate OnComplete)
{
	JusticeWallet::GetWalletBalance(CurrencyCode, FGetWalletBalanceCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, int32 Balance) {
		OnComplete.ExecuteIfBound(bSuccessful, Balance);
	}));
}