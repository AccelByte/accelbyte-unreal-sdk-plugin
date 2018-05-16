// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticePlatformFunctions.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/JusticeLog.h"
#include "Models/Category.h"

DECLARE_DELEGATE_TwoParams(FGetWalletBalanceCompleteDelegate, bool, int);

class JUSTICESDK_API JusticeWallet
{
public:
	static void GetWalletBalance(FString CurrencyCode, FGetWalletBalanceCompleteDelegate OnComplete);
	
private:
	static void OnGetWalletBalanceComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetWalletBalanceCompleteDelegate OnComplete);
};

