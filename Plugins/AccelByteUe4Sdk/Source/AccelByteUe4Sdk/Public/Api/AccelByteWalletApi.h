// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "AccelByteWalletModels.h"
#include "AccelByteError.h"

namespace AccelByte
{
namespace Api
{

/**
 * @brief Wallet API for buying things from the online store; a wallet can be a virtual or real currency.
 */
class ACCELBYTEUE4SDK_API Wallet
{
public:
	/**
	 * @brief Get user's wallet information for a specific currency code.
	 *
	 * @param CurrencyCode The currency code.
	 * @param OnSuccess This will be called when operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetWalletInfoByCurrencyCode(const FString& CurrencyCode, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError);

private:
	Wallet() = delete; // static class can't have instance
	Wallet(Wallet const&) = delete;
	Wallet(Wallet&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
