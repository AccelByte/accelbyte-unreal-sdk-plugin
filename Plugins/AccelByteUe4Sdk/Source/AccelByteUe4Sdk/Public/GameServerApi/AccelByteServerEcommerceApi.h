// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
* @brief Ecommerce API to manage user's Ecommerce related things like Entitlement and Wallet.
*/
class ACCELBYTEUE4SDK_API ServerEcommerce
{
public:
	ServerEcommerce(const ServerCredentials& Credentials, const ServerSettings& Settings);
	~ServerEcommerce();

	/**
	* @brief Get Entitlement's Info by the EntitlementId.
	*
	* @param EntitlementId The id of the entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementById(const FString& Entitlementid, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Granting Entitlement(s) to a user.
	 *
	 * @param UserId The user who will get the entilement(s).
	 * @param EntitlementGrant Consist of the entitlement(s) that will be granted.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsStackableEntitlementInfo.
	 * @param OnError This will be called when the operation failed.
	 */
	void GrantUserEntitlements(const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	 * @brief Credit a user wallet by currency code, if the wallet does not exist, it will create a new wallet.
	 *
	 * @param UserId The user who will receive credit.
	 * @param CurrencyCode The currency code.
	 * @param CreditUserWalletRequest The request to credit a user wallet.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation fails.
	 */
	void CreditUserWallet(const FString& UserId, const FString& CurrencyCode, const FAccelByteModelsCreditUserWalletRequest& CreditUserWalletRequest, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Debit a user wallet by currency code, if the wallet does not exist, it will create a new wallet.
	*
	* @param UserId The user who will receive credit.
	* @param WalletId The wallet id.
	* @param DebitUserWalletRequest The request to debit a user wallet.
	* @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	* @param OnError This will be called when the operation fails.
	*/
	void DebitUserWallet(const FString& UserId, const FString& WalletId, const FAccelByteModelsDebitUserWalletRequest& DebitUserWalletRequest, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError);


private:
	const ServerCredentials& Credentials;
	const ServerSettings& Settings;

	ServerEcommerce() = delete;
	ServerEcommerce(ServerEcommerce const&) = delete;
	ServerEcommerce(ServerEcommerce&&) = delete;
};

} // Namespace GameServerApi
} // Namespace AccelByte
