// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
* @brief Ecommerce API to manage user's Ecommerce related things like Entitlement and Wallet.
*/
class ACCELBYTEUE4SDK_API ServerEcommerce : public FServerApiBase
{
public:
	ServerEcommerce(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerEcommerce();

	/**
	 * @brief Get list of user's Entitlement(s).
	 *
	 * @param UserId The user id who have the entitlements
	 * @param bActiveOnly Set as true if you want to search the active only, and vice-versa.
	 * @param EntitlementName The name of the entitlement (optional).
	 * @param ItemIds Item's id (optional).
	 * @param Offset Offset of the list that has been sliced based on Limit parameter (optional, default = 0).
	 * @param Limit The limit of item on page (optional).
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementPagingSlicedResult.
	 * @param OnError This will be called when the operation failed.
	 * @param EntitlementClass Class of the entitlement (optional).
	 * @param AppType This is the type of application that entitled (optional).
	 * @param Features (optional).
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserEntitlements(FString const& UserId
		, bool bActiveOnly
		, FString const& EntitlementName
		, TArray<FString> const& ItemIds
		, int32 Offset
		, int32 Limit
		, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE
		, EAccelByteAppType AppType = EAccelByteAppType::NONE
		, TArray<FString> const& Features = {});

	/**
	 * @brief Get Entitlement's Info by the EntitlementId.
	 *
	 * @param EntitlementId The id of the entitlement.
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementById(FString const& EntitlementId
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get Entitlement info by the UserId and EntitlementId
	 *
	 * @param UserId The user id who have the entitlements
	 * @param EntitlementId The id of the entitlement
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementById(FString const& UserId
		, FString const& EntitlementId
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Granting Entitlement(s) to a user.
	 *
	 * @param UserId The user who will get the entilement(s).
	 * @param EntitlementGrant Consist of the entitlement(s) that will be granted.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsStackableEntitlementInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GrantUserEntitlements(FString const& UserId
		, TArray<FAccelByteModelsEntitlementGrant> const& EntitlementGrant
		, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Credit a user wallet by currency code, if the wallet does not exist, it will create a new wallet.
	 *
	 * @param UserId The user who will receive credit.
	 * @param CurrencyCode The currency code.
	 * @param CreditUserWalletRequest The request to credit a user wallet.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreditUserWallet(FString const& UserId
		, FString const& CurrencyCode
		, FAccelByteModelsCreditUserWalletRequest const& CreditUserWalletRequest
		, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Credit a user wallet by currency code, if the wallet does not exist, it will create a new wallet.
	 *
	 * @param UserId The user who will receive credit.
	 * @param CurrencyCode The currency code.
	 * @param CreditUserWalletRequest The request to credit a user wallet.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletCreditResponse&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreditUserWalletV2(FString const& UserId
		, FString const& CurrencyCode
		, FAccelByteModelsCreditUserWalletRequest const& CreditUserWalletRequest
		, THandler<FAccelByteModelsWalletCreditResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Revoke Entitlement from a User (Many)
	 *
	 * @param UserId The user id who have the entitlements
	 * @param EntitlementIds The ids of the entitlements
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsBulkRevokeEntitlements&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RevokeUserEntitlements(FString const& UserId
		, TArray<FString> const& EntitlementIds
		, THandler<FAccelByteModelsBulkRevokeEntitlements> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Revoke Entitlement from a User (Single)
	 *
	 * @param UserId The user id who have the entitlement
	 * @param EntitlementId The id of the entitlements
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsEntitlementInfo&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RevokeUserEntitlement(FString const& UserId
		, FString const& EntitlementId
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Consume Entitlement from a User
	 *
	 * @param UserId The user id who have the entitlement
	 * @param EntitlementId The id of the entitlements
	 * @param UseCount How many that will be consumed
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsEntitlementInfo&.
	 * @param OnError This will be called when the operation fails.
	 * @param Options Options of consumed entitlements.
	 * @param RequestId Request id(Optional), A unique tracking ID provided by the developer, can just left it empty if they don't want to track
	 * When a request id is submitted, it will return original successful response
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ConsumeUserEntitlement(FString const& UserId
		, FString const& EntitlementId
		, int32 UseCount
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError
		, TArray<FString> Options = {}
		, FString const& RequestId = {});

	/**
	 * @brief Disable Entitlement from a User. Like a revoke, but it can be enabled by calling func EnableUserEntitlement
	 *
	 * @param UserId The user id who have the entitlement
	 * @param EntitlementId The id of the entitlements
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsEntitlementInfo&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DisableUserEntitlement(FString const& UserId
		, FString const& EntitlementId
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Enable Entitlement that has been disabled
	 *
	 * @param UserId The user id who have the entitlement
	 * @param EntitlementId The id of the entitlements
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsBulkRevokeEntitlements&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr EnableUserEntitlement(FString const& UserId
		, FString const& EntitlementId
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user entitlement history.
	 *
	 * @param UserId The user id who have the entitlement history
	 * @param EntitlementId The id of the entitlements
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserEntitlementHistory&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementHistory(FString const& UserId
		, FString const& EntitlementId
		, THandler<TArray<FAccelByteModelsUserEntitlementHistory>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Debit a user wallet by currency code, if the wallet does not exist, it will create a new wallet.
	 *
	 * @param UserId The user who will receive credit.
	 * @param WalletId The wallet id.
	 * @param DebitUserWalletRequest The request to debit a user wallet.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	// This end point will be deprecated, old wallet id means one currency, but now it only means one wallet, so we wanna use currency and source instead it. 
	FAccelByteTaskWPtr DebitUserWallet(FString const& UserId
		, FString const& WalletId
		, FAccelByteModelsDebitUserWalletRequest const& DebitUserWalletRequest
		, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Debit a user wallet by currency code, if the wallet does not exist, it will create a new wallet.
	 *
	 * @param UserId The user who will receive credit.
	 * @param CurrencyCode The Currency Code.
	 * @param DebitUserWalletRequest The request to debit a user wallet.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DebitUserWalletV2(FString const& UserId
		, FString const& CurrencyCode
		, FAccelByteModelsDebitUserWalletRequestV2 const& DebitUserWalletRequest
		, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
		, FErrorHandler const& OnError);


	/**
	 * @brief Pay with user wallet by currency code and client platform, can debit to all available wallet depending on
	 * configuration set in Admin Portal's platform wallet config
	 *
	 * @param UserId The user who will receive credit.
	 * @param CurrencyCode The Currency Code.
	 * @param PaymentRequest The request to debit a user wallet.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsWalletInfo&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PaymentWithUserWallet(FString const& UserId
		, FString const& CurrencyCode
		, FAccelByteModelsPaymentUserWalletRequest const& PaymentRequest
		, THandler<FAccelByteModelsPlatformWallet> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief FulFill item to a user.
	 *
	 * @param UserId The user who will receive credit.
	 * @param FulfillmentRequest The request to fulfill an item to user.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsFulfillmentResult&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr FulfillUserItem(FString const& UserId
		, FAccelByteModelsFulfillmentRequest const& FulfillmentRequest
		, THandler<FAccelByteModelsFulfillmentResult> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get items by multiple Sku.
	 *
	 * @param Skus Sku of the Item.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGetBulkItemsBySkus&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetItemsBySkus(TArray<FString> const& Skus
		, THandler<TArray<FAccelByteModelsBulkGetItemsBySkus>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief List stores in a namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsPlatformStore>&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ListStores(THandler<TArray<FAccelByteModelsPlatformStore>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query items by criteria within a store.
	 *
	 * @param ItemCriteria Struct of item criteria 
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemPagingSlicedResult&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryItemsByCriteria(FAccelByteModelsItemCriteriaV2 const& ItemCriteria
		, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query items by criteria within a store, endpoint version 2.
	 *
	 * @param ItemCriteria Struct of item criteria, FAccelByteModelsItemCriteriaV3
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemPagingSlicedResult&.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryItemsByCriteriaV2(FAccelByteModelsItemCriteriaV3 const& ItemCriteria
		, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Fulfill Rewards to User 
	 *
	 * @param UserId User Id 
	 * @param FulfillRewardsRequest Struct of Fulfill Rewards, FAccelByteModelsFulfillRewards
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation fails.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr FulfillRewards(FString const& UserId
		, FAccelByteModelsFulfillRewards const& FulfillRewardsRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get items by ID. 
	 *
	 * @param ItemIds Array of item IDs.
	 * @param bActiveOnly Active items only.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation fails.
	 * @param StoreId The store ID (optional) if empty it will get from published store.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetItemsById(TArray<FString> const& ItemIds
		, const bool bActiveOnly
		, THandler<TArray<FAccelByteModelsItemInfoV2>> const& OnSuccess
		, FErrorHandler const& OnError
		, const FString& StoreId = TEXT(""));


private:
	ServerEcommerce() = delete;
	ServerEcommerce(ServerEcommerce const&) = delete;
	ServerEcommerce(ServerEcommerce&&) = delete;
};

} // Namespace GameServerApi
} // Namespace AccelByte
