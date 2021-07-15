// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteError.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Wallet API for buying things from the online store; a wallet can be a virtual or real currency.
 */
class ACCELBYTEUE4SDK_API Wallet
{
public:
	Wallet(const Credentials& Credentials, const Settings& Settings);
	~Wallet();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	 * @brief Get user's wallet information for a specific currency code.
	 *
	 * @param CurrencyCode The currency code.
	 * @param OnSuccess This will be called when operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetWalletInfoByCurrencyCode(const FString& CurrencyCode, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError);

private:
	Wallet() = delete;
	Wallet(Wallet const&) = delete;
	Wallet(Wallet&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
