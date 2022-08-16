// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief Entitlement API for checking user's ownership. User can query a list of item that belongs to him/her.
*/
class ACCELBYTEUE4SDK_API Entitlement : public FApiBase
{
public:
	Entitlement(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Entitlement();

public:
	/**
	* @brief Get list of user's Entitlement(s).
	*
	* @param EntitlementName The name of the entitlement (optional).
	* @param ItemId Item's id (optional).
	* @param Offset Offset of the list that has been sliced based on Limit parameter (optional, default = 0).
	* @param Limit The limit of item on page (optional).
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementPagingSlicedResult.
	* @param OnError This will be called when the operation failed.
	* @param EntitlementClass Class of the entitlement (optional).
	* @param AppType This is the type of application that entitled (optional).
	*/
	void QueryUserEntitlements(FString const& EntitlementName, FString const& ItemId, int32 const& Offset, int32 const& Limit, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType, TArray<FString> const& Features = {});

	/**
	* @brief Get list of user's Entitlement(s).
	*
	* @param EntitlementName The name of the entitlement (optional).
	* @param ItemIds Item's id (optional).
	* @param Offset Offset of the list that has been sliced based on Limit parameter (optional, default = 0).
	* @param Limit The limit of item on page (optional).
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementPagingSlicedResult.
	* @param OnError This will be called when the operation failed.
	* @param EntitlementClass Class of the entitlement (optional).
	* @param AppType This is the type of application that entitled (optional).
	* @param Features The feature array.
	*/
	void QueryUserEntitlements(FString const& EntitlementName, TArray<FString> const& ItemIds, int32 const& Offset, int32 const& Limit, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType, TArray<FString> const& Features = {});

	/**
	* @brief Get user's Entitlement by the EntitlementId.
	*
	* @param EntitlementId The id of the entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementById(FString const& EntitlementId, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get user's Entitlement ownership of the AppId.
	*
	* @param AppId The App ID.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipByAppId(FString const& AppId, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	* @brief Get user's Entitlement ownership of the sku.
	*
	* @param Sku The item's SKU.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipBySku(FString const& Sku, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get user's Entitlement ownership of the ItemId.
	*
	* @param ItemId The item's ItemId.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipByItemId(FString const& ItemId, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get user's Entitlement ownership for multiple checks.
	*
	* @param ItemIds The item's IDs to check.
	* @param AppIds the app IDs to check.
	* @param Skus the skus to check
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership will return true if any one parameters are owned.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipAny(TArray<FString> const ItemIds, TArray<FString> const AppIds, TArray<FString> const Skus, THandler<FAccelByteModelsEntitlementOwnership> const OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get user entitlement ownership if any of item IDs, app IDs, or SKUs are true
	*
	* @param PublicKey The Public Key that will be used to verify token
	* @param ItemIds The item's IDs to check.
	* @param AppIds the app IDs to check.
	* @param Skus the skus to check
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsOwnershipToken.
	* @param OnError This will be called when the operation failed.
	* @param VerifySignature Optional param, if true will verify the token's signature.
	* @param VerifyExpiration Optional param, if true will verify the token's expiration.
	* @param VerifySub Optional param, verify the token's sub field content.
	*/
	void GetUserEntitlementOwnershipViaToken(const FString& PublicKey, const TArray<FString>& ItemIds, const TArray<FString>& AppIds, const TArray<FString>& Skus, const THandler<FAccelByteModelsEntitlementOwnershipDetails>& OnSuccess, const FErrorHandler& OnError, const bool VerifySignature = true, const bool VerifyExpiration = true, const FString& VerifySub="");
	
	/**
	* @brief Get user entitlement ownership token if any of item IDs, app IDs, or SKUs are true
	*
	* @param ItemIds The item's IDs to check.
	* @param AppIds the app IDs to check.
	* @param Skus the skus to check
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsOwnershipToken.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipTokenOnly(const TArray<FString>& ItemIds, const TArray<FString>& AppIds, const TArray<FString>& Skus, const THandler<FAccelByteModelsOwnershipToken>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Consume a use entitlement.
	*
	* @param EntitlementId The id of the entitlement.
	* @param UseCount Number of consumed entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	* @param Options Options of consumed entitlements.
	*/
	void ConsumeUserEntitlement(FString const& EntitlementId, int32 const& UseCount, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess, FErrorHandler const& OnError,
		TArray<FString> Options = {});
	
	/**
	* @brief Create Distribution Receiver.
	*
	* @param ExtUserId The external user id is a random string that can be generated by yourself for the receiver.
	* @param Attributes That contain of ServerId, ServerName, CharacterId, and CharacterName.
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateDistributionReceiver(FString const& ExtUserId, FAccelByteModelsAttributes const Attributes, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Delete Distribution Receiver.
	*
	* @param ExtUserId The external user id that want to be deleted in receiver.
	* @param UserId The user id from the game.
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void DeleteDistributionReceiver(FString const& ExtUserId, FString const& UserId, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get Distribution Receiver.
	*
	* @param PublisherNamespace The namespace of the publisher or namespace of the game.
	* @param PublisherUserId The user id from the publisher or user id from the game.
	* @param OnSuccess This will be called when the operation success. The result is const TArray<FAccelByteModelsDistributionReceiver>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetDistributionReceiver(FString const& PublisherNamespace, FString const& PublisherUserId, THandler<TArray<FAccelByteModelsDistributionReceiver>> const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	* @brief Update Distribution Receiver.
	*
	* @param ExtUserId The external user id that has been created before for receiver.
	* @param Attributes That contain of ServerId, ServerName, CharacterId, and CharacterName.
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateDistributionReceiver(FString const& ExtUserId, FAccelByteModelsAttributes const Attributes, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Sync entitlement from 3rd party platform purchase.
	 *
	 * @param PlatformType The platform type identifier for sync entitlement.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 */
	void SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Sync entitlement from 3rd party platform purchase.
	 *
	 * @param PlatformType The platform type identifier for sync entitlement.
	 * @param PSSyncModel Contains ServiceLabel needed for PlayStation DLC sync
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 */
	void SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, FAccelByteModelsPlayStationDLCSync const& PSSyncModel, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Sync (Verify and fulfil) item entitlement from Google Play platform purchase.
	*
	* @param SyncRequest That contain of OrderId, PackageName, ProductId, PurchaseTime, and PurchaseToken to verify and sync item user bought from Google Play. 
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncMobilePlatformPurchaseGoogle(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Sync (Verify and fulfil) item entitlement from Google Play item platform purchase.
	*
	* @param SyncRequest That contain of OrderId, PackageName, ProductId, PurchaseTime, and PurchaseToken to verify and sync item user bought from Google Play. 
	* @param OnSuccess This will be called when the operation success. The result is boolean with true value.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncMobilePlatformPurchaseGooglePlay(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest, THandler<FAccelByteModelsPlatformSyncMobileGoogleResponse> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Sync (Verify and fulfil) item entitlement from Apple Store platform purchase.
	*
	* @param SyncRequest That contain of ProductId, TransactionId, ReceiptData, and ExcludeOldTransactions to verify and sync item user bought from Apple Store. 
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncMobilePlatformPurchaseApple(FAccelByteModelsPlatformSyncMobileApple const& SyncRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Synchronize Xbox inventory's DLC items.
	*
	* @param XboxDLCSync Contains XSTSToken needed for Xbox DLC sync
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncXBoxDLC(FAccelByteModelsXBoxDLCSync const& XboxDLCSync, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Synchronize Steam DLC.
	*
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncSteamDLC(FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Synchronize with DLC entitlements in PSN Store.
	*
	* @param PSSyncModel Contains ServiceLabel needed for PlayStation DLC sync
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncPSNDLC(FAccelByteModelsPlayStationDLCSync const& PSSyncModel, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Synchronize Twitch drop entitlement.
	* 
	* @param TwitchDropModel Contains GameId, Region, and Language.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncTwitchDropEntitlement(FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Synchronize Epic Game Durable/dlc Items.
	* 
	* @param EpicGamesJwtToken Epic game Jwt token.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void SyncEpicGameDurableItems(FString const& EpicGamesJwtToken, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	* @brief Validate user item purchase condition.
	*
	* @param Items Items to be validated.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void ValidateUserItemPurchaseCondition(TArray<FString> const& Items, THandler<TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get user entitlement ownership by itemIds.
	*
	* @param Ids ItemsIds.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/	
	void GetUserEntitlementOwnershipByItemIds(TArray<FString> const& Ids, THandler<TArray<FAccelByteModelsEntitlementOwnershipItemIds>> const& OnSuccess, FErrorHandler const& OnError);

	
private:
	Entitlement() = delete;
	Entitlement(Entitlement const&) = delete;
	Entitlement(Entitlement&&) = delete;
};

} // Namespace Api
}
