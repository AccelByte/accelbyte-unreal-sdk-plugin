// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Core/AccelByteError.h"
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
class ACCELBYTEUE4SDK_API Entitlement
{
public:
	Entitlement(const Credentials& Credentials, const Settings& Settings);
	~Entitlement();
private:
	const Credentials& Credentials;
	const Settings& Settings;
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
	void QueryUserEntitlements(const FString& EntitlementName, const FString& ItemId, const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType);

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
	*/
	void QueryUserEntitlements(const FString& EntitlementName, const TArray<FString>& ItemIds, const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType);

	/**
	* @brief Get user's Entitlement by the EntitlementId.
	*
	* @param EntitlementId The id of the entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementById(const FString& Entitlementid, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Get user's Entitlement ownership of the AppId.
	*
	* @param AppId The App ID.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipByAppId(const FString& AppId, const THandler<FAccelByteModelsEntitlementOwnership>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Get user's Entitlement ownership of the sku.
	*
	* @param Sku The item's SKU.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipBySku(const FString & Sku, const THandler<FAccelByteModelsEntitlementOwnership>& OnSuccess, const FErrorHandler & OnError);

	/**
	* @brief Get user's Entitlement ownership for multiple checks.
	*
	* @param ItemIds The item's IDs to check.
	* @param AppIds the app IDs to check.
	* @param Skus the skus to check
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementOwnership will return true if any one parameters are owned.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementOwnershipAny(const TArray<FString> ItemIds, const TArray<FString> AppIds, const TArray<FString> Skus, const THandler<FAccelByteModelsEntitlementOwnership> OnSuccess, const FErrorHandler & Error);

	/**
	* @brief Consume a use entitlement.
	*
	* @param EntitlementId The id of the entitlement.
	* @param UseCount Number of consumed entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void ConsumeUserEntitlement(const FString& EntitlementId, const int32& UseCount, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Create Distribution Receiver.
	*
	* @param ExtUserId The external user id is a random string that can be generated by yourself for the receiver.
	* @param Attributes That contain of ServerId, ServerName, CharacterId, and CharacterName.
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateDistributionReceiver(const FString& ExtUserId, const FAccelByteModelsAttributes Attributes, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Delete Distribution Receiver.
	*
	* @param ExtUserId The external user id that want to be deleted in receiver.
	* @param UserId The user id from the game.
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void DeleteDistributionReceiver(const FString& ExtUserId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Get Distribution Receiver.
	*
	* @param PublisherNamespace The namespace of the publisher or namespace of the game.
	* @param PublisherUserId The user id from the publisher or user id from the game.
	* @param OnSuccess This will be called when the operation success. The result is const TArray<FAccelByteModelsDistributionReceiver>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetDistributionReceiver(const FString& PublisherNamespace, const FString& PublisherUserId, const THandler<TArray<FAccelByteModelsDistributionReceiver>>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Update Distribution Receiver.
	*
	* @param ExtUserId The external user id that has been created before for receiver.
	* @param Attributes That contain of ServerId, ServerName, CharacterId, and CharacterName.
	* @param OnSuccess This will be called when the operation success.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateDistributionReceiver(const FString& ExtUserId, const FAccelByteModelsAttributes Attributes, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Sync entitlement from 3rd party platform purchase.
	 *
	 * @param PlatformType The platform type identifier for sync entitlement.
	 * @param OnSuccess This will be called when the operation success.
	 * @param OnError This will be called when the operation failed.
	 */
	void SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
private:
	Entitlement() = delete;
	Entitlement(Entitlement const&) = delete;
	Entitlement(Entitlement&&) = delete;
};

} // Namespace Api
}
