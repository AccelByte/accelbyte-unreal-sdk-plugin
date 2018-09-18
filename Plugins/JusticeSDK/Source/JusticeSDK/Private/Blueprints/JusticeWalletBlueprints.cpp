// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWalletBlueprints.h"
#include "JusticeWallet.h"

#pragma region GetWalletBalance
UGetWalletBalance * UGetWalletBalance::GetWalletBalance(FString Currency)
{
	UGetWalletBalance* Node = NewObject<UGetWalletBalance>();
	Node->Currency = Currency;
	return Node;
}

void UGetWalletBalance::Activate()
{
	JusticeWallet::GetWalletBalance(this->Currency, FGetWalletBalanceCompleteDelegate::CreateLambda([&](bool bSuccessful, int32 Balance) {
		if (bSuccessful)
		{
			OnSuccess.Broadcast(Balance);
		}
		else
		{
			OnFailed.Broadcast(0);
		}
	}));
}
#pragma endregion GetWalletBalance