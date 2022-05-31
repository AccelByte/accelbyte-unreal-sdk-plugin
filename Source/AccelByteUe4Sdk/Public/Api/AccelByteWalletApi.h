// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"

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
	Wallet(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Wallet();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
public:
	/**
	 * @brief Get user's wallet information for a specific currency code.
	 *
	 * @param CurrencyCode The currency code.
	 * @param OnSuccess This will be called when operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetWalletInfoByCurrencyCode(FString const& CurrencyCode, THandler<FAccelByteModelsWalletInfo> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get user's wallet transactions list for a specific currency code.
	 * 
	 * @param CurrencyCode The currency code.
	 * @param OnSuccess This will be called when operation succeeded. The result is const FAccelByteModelsWalletTransactionPaging&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset Offset of the list that has been sliced based on Limit parameter (optional, default = 0).
	 * @param Limit The limit of item on page (optional, default = 20).
	*/
	void ListWalletTransactionsByCurrencyCode(FString const& CurrencyCode, THandler<FAccelByteModelsWalletTransactionPaging> const& OnSuccess, FErrorHandler const& OnError, int32 Offset = 0, int32 Limit = 20);

private:
	Wallet() = delete;
	Wallet(Wallet const&) = delete;
	Wallet(Wallet&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
