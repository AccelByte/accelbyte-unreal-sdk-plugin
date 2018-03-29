// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Private/Models/Category.h"


DECLARE_DELEGATE_TwoParams(FGetWalletBalanceCompleteDelegate, bool, int);


class JUSTICESDK_API JusticeWallet
{

public:
	static void GetWalletBalance(FGetWalletBalanceCompleteDelegate OnComplete);


private:
	static void OnGetWalletBalanceComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetWalletBalanceCompleteDelegate OnComplete);
};

