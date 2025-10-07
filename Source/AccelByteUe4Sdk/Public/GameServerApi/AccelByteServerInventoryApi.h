// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteInventoryModels.h"

constexpr int32 MaxSaveInventoryItems = 10;

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Provide APIs to access Inventory service.
 */
class ACCELBYTEUE4SDK_API ServerInventory : public FServerApiBase
{
public:
	ServerInventory(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetrySchedulerBase & InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~ServerInventory();

#pragma region V1 Inventory

	/**
	 * @brief Retrieve all inventories in the current namespace.
	 *
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserInventoriesPagingResponse.
	 * @param OnError A delegate executed when the request fails.
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryConfigurationCode The code for a specific inventory configuration.
	 * @param SortBy The criteria for sorting the list of user inventories. Possible values: "createdAt", "updatedAt", "Configuration". Default: "createdAt:desc".
	 * @param Limit The number of items per page to retrieve. Default: 20.
	 * @param Offset The page number to retrieve. Default: 0.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetInventories(THandler<FAccelByteModelsUserInventoriesPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& UserId = TEXT("")
		, FString const& InventoryConfigurationCode = TEXT("")
		, EAccelByteInventoriesSortBy SortBy = EAccelByteInventoriesSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Create an inventory for a user with a specific configuration.
	 *
	 * @param CreateInventoryRequest A request containing the user ID and inventory configuration code for the target.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserInventoryResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateInventory(FAccelByteModelsCreateInventoryRequest const& CreateInventoryRequest
		, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve detailed information about a user's inventory.
	 *
	 * @param InventoryId The ID of the user's inventory.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserInventoryResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetInventory(FString const& InventoryId
		, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update a user's inventory.
	 *
	 * Notes:
	 *   - A positive value will increase MaxSlots from the existing value.
	 *   - A negative value will decrease MaxSlots from the existing value.
	 *   - Limited slots cannot be changed to unlimited, and vice versa.
	 *
	 * @param InventoryId The ID of the user's inventory.
	 * @param UpdateInventoryRequest A request to update the inventory.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserInventoryResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateInventory(FString const& InventoryId
		, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
		, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update all inventories related to a specific inventory configuration code.
	 *
	 * Notes:
	 *   - A positive value will increase MaxSlots from the existing value.
	 *   - A negative value will decrease MaxSlots from the existing value.
	 *   - Limited slots cannot be changed to unlimited, and vice versa.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryConfigurationCode The code of the inventory configuration.
	 * @param UpdateInventoryRequest A request to update the inventories.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUpdateAllInventoriesResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateAllUserInventories(FString const& UserId
		, FString const& InventoryConfigurationCode
		, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
		, THandler<TArray<FAccelByteModelsUserInventoryResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a user's inventory based on the ID.
	 *
	 * @param InventoryId The ID of the user's inventory.
	 * @param DeleteInventoryRequest A request to delete the inventory.
	 * @param OnSuccess A delegate executed when the request succeeds.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteInventory(FString const& InventoryId
		, FAccelByteModelsDeleteInventoryRequest const& DeleteInventoryRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

#pragma endregion

#pragma region V1 Item

	/**
	 * @brief Retrieve all inventory items based on the inventory ID.
	 *
	 * @param InventoryId The ID of the user's inventory.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserItemsPagingResponse.
	 * @param OnError A delegate executed when the request fails.
	 * @param SortBy The criteria for sorting the list of user inventories. Possible values: "createdAt", "updatedAt". Default: "createdAt:desc".
	 * @param Limit The number of items per page to retrieve. Default: 20.
	 * @param Offset The page number to retrieve. Default: 0.
	 * @param SourceItemId The ID of the source item.
	 * @param Tags The tags of the user's items.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetInventoryAllItems(FString const& InventoryId
		, THandler<FAccelByteModelsUserItemsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUserItemsSortBy SortBy = EAccelByteUserItemsSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0
		, FString const& SourceItemId = TEXT("")
		, FString const& Tags = TEXT(""));

	/**
	 * @brief Retrieve detailed information about a user's inventory item based on the inventory ID.
	 *
	 * @param InventoryId The ID of the user's inventory.
	 * @param SlotId The ID of the item slot.
	 * @param SourceItemId The ID of the source item.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserItemResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetInventoryItem(FString const& InventoryId
		, FString const& SlotId
		, FString const& SourceItemId
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Consume a user's inventory item.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryId The ID of the user's inventory.
	 * @param ConsumedItemsRequest A request containing the quantity, slot ID, and source item ID for the target.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserItemResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ConsumeInventoryItem(FString const& UserId
		, FString const& InventoryId
		, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk update a user's inventory items.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryId The ID of the user's inventory.
	 * @param UpdatedItemsRequest A request containing the attributes of inventory items to update.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is a TArray of FAccelByteModelsUpdateUserInventoryItemResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkUpdateInventoryItems(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsUpdateInventoryItemRequest> const& UpdatedItemsRequest
		, THandler<TArray<FAccelByteModelsUpdateUserInventoryItemResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Save an item to a user's inventory.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryId The ID of the user's inventory.
	 * @param SaveItemRequest A request containing the item details to save.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is a TArray of FAccelByteModelsUserItemResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveInventoryItemByInventoryId(FString const& UserId
		, FString const& InventoryId
		, FAccelByteModelsSaveInventoryItemByInventoryIdRequest const& SaveItemRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk delete items from a user's inventory.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryId The ID of the user's inventory.
	 * @param DeletedItemsRequest A request containing the attributes of the inventory items to delete.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is a TArray of FAccelByteModelsDeleteUserInventoryItemResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkDeleteUserInventoryItems(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
		, THandler<TArray<FAccelByteModelsDeleteUserInventoryItemResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Save an item to a user's inventory based on the configuration code.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param SaveItemRequest A request containing the item details to save.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is FAccelByteModelsUserItemResponse.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveInventoryItem(FString const& UserId
		, FAccelByteModelsSaveInventoryItemRequest const& SaveItemRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk save items to a user's inventory.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param SaveItemsRequests An array of requests, each containing the details of an item to save.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is a TArray of FAccelByteModelsBulkSaveInventoryItems.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkSaveInventoryItems(FString const& UserId
		, TArray<FAccelByteModelsSaveInventoryItemRequest> const& SaveItemsRequests
		, THandler<TArray<FAccelByteModelsBulkSaveInventoryItems>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk save items to a user's inventory based on the inventory ID.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param InventoryId The ID of the user's inventory.
	 * @param SaveItemsRequests An array of requests, each containing the details of an item to save.
	 * @param OnSuccess A delegate executed when the request succeeds. The result is a TArray of FAccelByteModelsBulkSaveInventoryItems.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkSaveInventoryItemsByInventoryId(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsSaveInventoryItemByInventoryIdRequest> const& SaveItemsRequests
		, THandler<TArray<FAccelByteModelsBulkSaveInventoryItems>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sync a user's entitlement from the e-commerce service to their inventory for items that do not yet exist in the user's inventory.
	 *
	 * @param UserId The ID of the inventory owner.
	 * @param OnSuccess A delegate executed when the request succeeds.
	 * @param OnError A delegate executed when the request fails.
	 *
	 * @return An AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SyncUserEntitlement(FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

#pragma endregion

private:
	ServerInventory() = delete;
	ServerInventory(ServerInventory const&) = delete;
	ServerInventory(ServerInventory&&) = delete;

	static constexpr int32 InventoryItemsLimit = 40;

	static FString ConvertInventoriesSortByToString(EAccelByteInventoriesSortBy SortBy);
	static FString ConvertInventoryItemsSortByToString(EAccelByteUserItemsSortBy SortBy);
};

} // Namespace GameServerApi
} // Namespace AccelByte
