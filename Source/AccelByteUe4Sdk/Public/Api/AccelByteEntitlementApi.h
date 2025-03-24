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
class ACCELBYTEUE4SDK_API Entitlement : public FApiBase, public TSharedFromThis<Entitlement, ESPMode::ThreadSafe>
{
public:
	Entitlement(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~Entitlement();

public:

	/**
	 * @brief Get current user entitlements history.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserEntitlementHistoryPagingResult&.
	 * @param OnError This will be called when the operation fails.
	 * @param EntitlementClass Class of the entitlement (optional).
	 * @param StartDate Defines the preferred starting date or time to query user entitlement history (optional).
	 * @param EndDate Defines the preferred ending  date or time to query user entitlement history (optional).
	 * @param Limit Number of content per page to retrieve. Default value : 20
	 * @param Offset Number of page to retrieve. Default value : 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCurrentUserEntitlementHistory(THandler<FAccelByteModelsUserEntitlementHistoryPagingResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteEntitlementClass const& EntitlementClass = EAccelByteEntitlementClass::NONE
		, FDateTime const& StartDate = 0
		, FDateTime const& EndDate = 0
		, int32 Limit = 20
		, int32 Offset = 0);

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
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserEntitlements(FString const& EntitlementName
		, FString const& ItemId
		, int32 Offset
		, int32 Limit
		, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE
		, EAccelByteAppType AppType = EAccelByteAppType::NONE
		, TArray<FString> const& Features = {});

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
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserEntitlements(FString const& EntitlementName
		, TArray<FString> const& ItemIds
		, int32 Offset
		, int32 Limit
		, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE
		, EAccelByteAppType AppType = EAccelByteAppType::NONE
		, TArray<FString> const& Features = {});

	/**
	 * @brief Get user's Entitlement by the EntitlementId.
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
	 * @brief Get user's entitlements based on the Entitlement Ids.
	 *
	 * @param EntitlementIds The IDs of the entitlements to retrieve.
	 * @param OnSuccess Callback invoked when the operation succeeds. The result is const TArray<FAccelByteModelsEntitlementInfo>.
	 * @param OnError Callback invoked when the operation fails.
	 * @param bAvailablePlatformOnly If true, only checks for available platform entitlements; defaults to true.
	 *
	 * @note
	 * - This operation depends on the entitlement origin feature being enabled by the admin.
	 * - If bAvailablePlatformOnly is true, the backend returns entitlements specific to the current user's IAM client platform.
	 * - If bAvailablePlatformOnly is false, the backend returns all entitlements, regardless of platform availability.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementByIds(TArray<FString> const& EntitlementIds
		, THandler<TArray<FAccelByteModelsEntitlementInfo>> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bAvailablePlatformOnly = true);

	/**
	 * @brief Get user's Entitlement ownership of the AppId.
	 *
	 * @param AppId The App ID.
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	 * @param OnError This will be called when the operation failed.
	 * @param bUsePublisherNamespace check user entitlement in publisher namespace, by default is true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipByAppId(FString const& AppId
		, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bUsePublisherNamespace = true);
	
	/**
	 * @brief Get user's Entitlement ownership of the sku.
	 *
	 * @param Sku The item's SKU.
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	 * @param OnError This will be called when the operation failed.
	 * @param bUsePublisherNamespace check user entitlement in publisher namespace, by default is true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipBySku(FString const& Sku
		, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bUsePublisherNamespace = true);

	/**
	 * @brief Get user's Entitlement ownership of the ItemId.
	 *
	 * @param ItemId The item's ItemId.
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	 * @param OnError This will be called when the operation failed.
	 * @param bUsePublisherNamespace check user entitlement in publisher namespace, by default is false (to preserve the previous behaviour).
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipByItemId(FString const& ItemId
		, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bUsePublisherNamespace = false);

	/**
	 * @brief Get user's Entitlement ownership for multiple checks.
	 *
	 * @param ItemIds The item's IDs to check.
	 * @param AppIds the app IDs to check.
	 * @param Skus the skus to check
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership will return true if any one parameters are owned.
	 * @param OnError This will be called when the operation failed.
	 * @param bUsePublisherNamespace check user entitlement in publisher namespace, by default is true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipAny(TArray<FString> const& ItemIds
		, TArray<FString> const& AppIds
		, TArray<FString> const& Skus
		, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bUsePublisherNamespace = true);

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
	 * @param bUsePublisherNamespace check user entitlement in publisher namespace, by default is true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipViaToken(FString const& PublicKey
		, TArray<FString> const& ItemIds
		, TArray<FString> const& AppIds
		, TArray<FString> const& Skus
		, THandler<FAccelByteModelsEntitlementOwnershipDetails> const& OnSuccess
		, FErrorHandler const& OnError
		, bool VerifySignature = true
		, bool VerifyExpiration = true
		, FString const& VerifySub = TEXT("")
		, bool bUsePublisherNamespace = true);
	
	/**
	 * @brief Get user entitlement ownership token if any of item IDs, app IDs, or SKUs are true
	 *
	 * @param ItemIds The item's IDs to check.
	 * @param AppIds the app IDs to check.
	 * @param Skus the skus to check
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsOwnershipToken.
	 * @param OnError This will be called when the operation failed.
	 * @param bUsePublisherNamespace check user entitlement in publisher namespace, by default is true.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipTokenOnly(TArray<FString> const& ItemIds
		, TArray<FString> const& AppIds
		, TArray<FString> const& Skus
		, THandler<FAccelByteModelsOwnershipToken> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bUsePublisherNamespace = true);
	
	/**
	 * @brief Consume a use entitlement.
	 *
	 * @param EntitlementId The id of the entitlement.
	 * @param UseCount Number of consumed entitlement.
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param Options Options of consumed entitlements.
	 * @param RequestId Request id(Optional), A unique tracking ID provided by the developer, can just left it empty if they don't want to track
	 * When a request id is submitted, it will return original successful response
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ConsumeUserEntitlement(FString const& EntitlementId
		, int32 UseCount
		, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError
		, TArray<FString> const& Options = {}
		, FString const& RequestId = TEXT(""));
	
	/**
	 * @brief Create Distribution Receiver.
	 *
	 * @param ExtUserId The external user id is a random string that can be generated by yourself for the receiver.
	 * @param Attributes That contain of ServerId, ServerName, CharacterId, and CharacterName.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateDistributionReceiver(FString const& ExtUserId
		, FAccelByteModelsAttributes const& Attributes
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete Distribution Receiver.
	 *
	 * @param ExtUserId The external user id that want to be deleted in receiver.
	 * @param UserId The user id from the game.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteDistributionReceiver(FString const& ExtUserId
		, FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get Distribution Receiver.
	 *
	 * @param PublisherNamespace The namespace of the publisher or namespace of the game.
	 * @param PublisherUserId The user id from the publisher or user id from the game.
	 * @param OnSuccess This will be called when the operation success. The result is const TArray<FAccelByteModelsDistributionReceiver>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetDistributionReceiver(FString const& PublisherNamespace
		, FString const& PublisherUserId
		, THandler<TArray<FAccelByteModelsDistributionReceiver>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Update Distribution Receiver.
	 *
	 * @param ExtUserId The external user id that has been created before for receiver.
	 * @param Attributes That contain of ServerId, ServerName, CharacterId, and CharacterName.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateDistributionReceiver(FString const& ExtUserId
		, FAccelByteModelsAttributes const& Attributes
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync entitlement from 3rd party platform purchase.
	 *
	 * @param PlatformType The platform type identifier for sync entitlement.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncPlatformPurchase(EAccelBytePlatformSync PlatformType
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Sync entitlement from 3rd party platform purchase.
	 *
	 * @param PlatformType The platform type identifier for sync entitlement.
	 * @param PSSyncModel Contains ServiceLabel needed for PlayStation DLC sync
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncPlatformPurchase(EAccelBytePlatformSync PlatformType
		, FAccelByteModelsPlayStationDLCSync const& PSSyncModel
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync entitlement from 3rd party platform purchase.
	 *
	 * @param EntitlementSyncBase Base information needed by some platforms.
	 * @param PlatformType The platform type identifier for sync entitlement.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncPlatformPurchase(FAccelByteModelsEntitlementSyncBase const& EntitlementSyncBase
		, EAccelBytePlatformSync PlatformType
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync (Verify and fulfil) item entitlement from Google Play platform purchase.
	 *
	 * @param SyncRequest That contain of OrderId, PackageName, ProductId, PurchaseTime, and PurchaseToken to verify and sync item user bought from Google Play. 
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncMobilePlatformPurchaseGoogle(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync (Verify and fulfil) item entitlement from Google Play item platform purchase.
	 *
	 * @param SyncRequest That contain of OrderId, PackageName, ProductId, PurchaseTime, and PurchaseToken to verify and sync item user bought from Google Play. 
	 * @param OnSuccess This will be called when the operation success. The result is boolean with true value.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncMobilePlatformPurchaseGooglePlay(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest
		, THandler<FAccelByteModelsPlatformSyncMobileGoogleResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync (Verify and fulfil) item entitlement from Apple Store platform purchase.
	 *
	 * @param SyncRequest That contain of ProductId, TransactionId, ReceiptData, and ExcludeOldTransactions to verify and sync item user bought from Apple Store. 
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncMobilePlatformPurchaseApple(FAccelByteModelsPlatformSyncMobileApple const& SyncRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Xbox inventory's DLC items.
	 *
	 * @param XboxDLCSync Contains XSTSToken needed for Xbox DLC sync
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncXBoxDLC(FAccelByteModelsXBoxDLCSync const& XboxDLCSync
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Steam DLC.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncSteamDLC(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize with DLC entitlements in PSN Store.
	 *
	 * @param PSSyncModel Contains ServiceLabel needed for PlayStation DLC sync
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncPSNDLC(FAccelByteModelsPlayStationDLCSync const& PSSyncModel
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Twitch drop entitlement.
	 * 
	 * @param TwitchDropModel Contains GameId, Region, and Language.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncTwitchDropEntitlement(FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Epic Game Durable/dlc Items.
	 * 
	 * @param EpicGamesJwtToken Epic game Jwt token.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncEpicGameDurableItems(FString const& EpicGamesJwtToken
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Validate user item purchase condition.
	 *
	 * @param Items Items to be validated.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ValidateUserItemPurchaseCondition(TArray<FString> const& Items
		, THandler<TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user entitlement ownership by itemIds.
	 *
	 * @param Ids ItemsIds.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserEntitlementOwnershipByItemIds(TArray<FString> const& Ids
		, THandler<TArray<FAccelByteModelsEntitlementOwnershipItemIds>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize with DLC entitlements in PSN Store.
	 *
	 * @param PlaystationModel Playstation's DLC Model Sync Request  
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncWithDLCEntitlementInPSNStore(FAccelByteModelsPlayStationDLCSync const& PlaystationModel
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize with entitlements in PSN Store.
	 *
	 * @param PlaystationModel Playstation's IAP Model Sync Request  
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncWithEntitlementInPSNStore(FAccelByteModelsPlayStationIAPSync const& PlaystationModel
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sell user entitlement.
	 *
	 * @param EntitlementId The id of the entitlement.
	 * @param UseCount The count be consumed.
	 * @param RequestId The id of the request.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SellUserEntitlement(FString const& EntitlementId
		, int32 UseCount
		, FString const& RequestId
		, THandler<FAccelByteModelsSellItemEntitlementInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Oculus Consumable Entitlement/IAP.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncOculusConsumableEntitlements(THandler<TArray<FAccelByteModelsSyncOculusConsumableEntitlementInfo>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Oculus Downloadable Content/DLC.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncOculusDLC(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize with DLC entitlements in PSN Store with multiple service labels.
	 *
	 * @param PlaystationModel Contains ServiceLabel needed for PlayStation DLC Multiple Service sync.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncDLCPSNMultipleService(FAccelByteModelsMultipleServicePSNDLCSync const& PlaystationModel
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Synchronize with IAP entitlements in PSN Store with multiple service labels.
	 *
	 * @param PlaystationModel Contains ServiceLabel needed for PlayStation IAP Multiple Service sync.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncEntitlementPSNMultipleService(FAccelByteModelsMultipleServicePSNIAPSync const& PlaystationModel
		, THandler<TArray<FAccelByteModelsMultipleServicePlayStationInfo>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query Platform's subscription items
	 * 
	 * @param PlatformType The platform type identifier for subscription query
	 * @param Request Query parameters for the request
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryUserSubcriptions(EAccelBytePlatformSync PlatformType
		, FAccelByteModelsThirdPartyUserSubscriptionQueryRequest const& Request
		, THandler<FAccelByteModelsThirdPartyUserSubscriptions> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize Steam IAP using a specific order ID. Only to be used when Steam sync mode is set to TRANSACTION
	 * in the admin portal.
	 *
	 * @param Request Data to attach to this request, such as Steam order ID
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncSteamIAPTransaction(FAccelByteModelsSyncSteamIAPTransactionRequest const& Request
		, THandler<FAccelByteModelsSyncSteamIAPTransactionResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Synchronize abornal Steam IAP purchases. Such as when a user purchases an item from Steam, but has not
	 * yet linked an AGS account. Only to be used when Steam sync mode is set to TRANSACTION in the admin portal.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncSteamAbnormalIAPTransaction(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
private:
	Entitlement() = delete;
	Entitlement(Entitlement const&) = delete;
	Entitlement(Entitlement&&) = delete;
};

typedef TSharedRef<Entitlement, ESPMode::ThreadSafe> EntitlementRef;
typedef TSharedPtr<Entitlement, ESPMode::ThreadSafe> EntitlementPtr;
typedef TWeakPtr<Entitlement, ESPMode::ThreadSafe> EntitlementWPtr;

} // Namespace Api
}
