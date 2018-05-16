// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GetWalletBalance.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "JusticeSDK.h"
#include "JusticeWallet.h"

UAsyncGetWalletBalance * UAsyncGetWalletBalance::GetWalletBalance(FString Currency)
{
	UAsyncGetWalletBalance* Node = NewObject<UAsyncGetWalletBalance>();
	Node->Currency = Currency;
	return Node;
}

void UAsyncGetWalletBalance::Activate() 
{
	JusticeWallet::GetWalletBalance(this->Currency, FGetWalletBalanceCompleteDelegate::CreateLambda([&](bool isSuccess, int balance) {
		if (isSuccess)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(balance);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(0);
			}
		}
	}));
}