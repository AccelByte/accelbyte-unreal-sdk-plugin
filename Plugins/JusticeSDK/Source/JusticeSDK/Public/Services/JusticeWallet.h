// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/JusticeLog.h"
#include "Models/Category.h"

DECLARE_DELEGATE_TwoParams(FGetWalletBalanceCompleteDelegate, bool, int32);

/**
 * @brief Justice wallet service.
 * User should log in before using this. See JusticeIdentity to see how to log user in.
 */
class JUSTICESDK_API JusticeWallet
{
public:
/**
 * @brief Get user's wallet information for a specific currency code. It requires CurrencyCode as a parameter. 
 * 
 * @param CurrencyCode Required.
 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is int32, set in FGetWalletBalanceCompleteDelegate callback.
 */
	static void GetWalletBalance(FString CurrencyCode, FGetWalletBalanceCompleteDelegate OnComplete);
	
private:
	static void OnGetWalletBalanceResponse(FJusticeResponsePtr Response, FGetWalletBalanceCompleteDelegate OnComplete);
};

