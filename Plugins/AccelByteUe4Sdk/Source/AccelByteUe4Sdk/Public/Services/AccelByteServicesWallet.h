// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "AccelByteModelsWallet.h"
#include "AccelByteError.h"

namespace AccelByte
{
namespace Services
{

/**
 * @brief Justice wallet service.
 * User should log in before using this. See JusticeIdentity to see how to log user in.
 */
class ACCELBYTEUE4SDK_API Wallet
{
public:
	DECLARE_DELEGATE_OneParam(FGetWalletByCurrencyCodeSuccess, const FAccelByteModelsWalletInfo&);
	/**
	 * @brief Get user's wallet information for a specific currency code.
	 *
	 * @param CurrencyCode Required.
	 * @param OnSuccess Required, but can be nullptr. This will be called when operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetWalletInfoByCurrencyCode(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString CurrencyCode, FGetWalletByCurrencyCodeSuccess OnSuccess, ErrorDelegate OnError);

private:
	static void GetWalletInfoByCurrencyCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetWalletByCurrencyCodeSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Services
} // Namespace AccelByte
