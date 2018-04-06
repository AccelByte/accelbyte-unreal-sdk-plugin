// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWalletFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeWallet.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"

void UJusticeWalletFunctions::GetWalletBalance(FGetWalletCompleteDelegate OnComplete)
{
	JusticeWallet::GetWalletBalance(FGetWalletBalanceCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, int balance) {
		OnComplete.Execute(bSuccessful, balance);
	}));
}