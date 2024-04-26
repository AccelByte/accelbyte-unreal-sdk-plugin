// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteInventoryModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{
/**
 * @brief Provide APIs to access Inventory service.
 */
class ACCELBYTEUE4SDK_API Inventory : public FApiBase
{
public:
	Inventory(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Inventory();

#pragma region V1 Inventory

	/**
	 * @brief Get a list of inventory configurations in a namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsInventoryConfigurationsPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy The sorting criteria for the list of inventory configurations. Value: createdAt, updatedAt, code, maxItems. default = createdAt:desc.
	 * @param Limit Number of content per page to retrieve. Default value : 20
	 * @param Offset Number of page to retrieve. Default value : 0
	 * @param InventoryConfigurationCode The code of a certain inventory configuration.
	 */
	void GetInventoryConfigurations(THandler<FAccelByteModelsInventoryConfigurationsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteInventoryConfigurationSortBy const& SortBy = EAccelByteInventoryConfigurationSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0
		, FString const& InventoryConfigurationCode = TEXT(""));

	/**
	 * @brief Get a list of inventory tags in a namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsInventoryTagPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy The sorting criteria for the list of inventory types. Value: createdAt, name. default = createdAt:desc.
	 * @param Limit Number of content per page to retrieve. Default value : 20
	 * @param Offset Number of page to retrieve. Default value : 0
	 */
	void GetInventoryTags(THandler<FAccelByteModelsInventoryTagPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteInventoryUtilitiesSortBy const& SortBy = EAccelByteInventoryUtilitiesSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Get a list of user inventories in a namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserInventoriesPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy The sorting criteria for the list of user inventories. Value: createdAt, updatedAt. default = createdAt:desc.
	 * @param Limit Number of content per page to retrieve. Default value : 20
	 * @param Offset Number of page to retrieve. Default value : 0
	 * @param InventoryConfigurationCode The code of a certain inventory configuration.
	 */
	void GetUserInventories(THandler<FAccelByteModelsUserInventoriesPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUserInventoriesSortBy const& SortBy = EAccelByteUserInventoriesSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0
		, FString const& InventoryConfigurationCode = TEXT(""));

#pragma endregion

#pragma region V1 Item

	/**
	 * @brief Get a list of item types in a namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsItemTypePagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy The sorting criteria for the list of inventory types. Value: createdAt, name. default = createdAt:desc.
	 * @param Limit Number of content per page to retrieve. Default value : 20
	 * @param Offset Number of page to retrieve. Default value : 0
	 */
	void GetItemTypes(THandler<FAccelByteModelsItemTypePagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteInventoryUtilitiesSortBy const& SortBy = EAccelByteInventoryUtilitiesSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Get a list of user items from specific user inventory.
	 *
	 * @param InventoryId The id of user's inventory.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserItemsPagingResponse.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy The sorting criteria for the list of inventory types. Value: createdAt, updatedAt, quantity. default = createdAt:desc.
	 * @param Limit Number of content per page to retrieve. Default value : 20
	 * @param Offset Number of page to retrieve. Default value : 0
	 * @param SourceItemId The id of source item.
	 * @param Tags The Tags of user's item.
	 * @param Quantity The preferred minimum quantity of an item.
	 */
	void GetUserInventoryAllItems(FString const& InventoryId
		, THandler<FAccelByteModelsUserItemsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUserItemsSortBy const& SortBy = EAccelByteUserItemsSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0
		, FString const& SourceItemId = TEXT("")
		, FString const& Tags = TEXT("")
		, int32 Quantity = 1);

	/**
	 * @brief Get a specific item from specific user inventory.
	 *
	 * @param InventoryId The id of user's inventory.
	 * @param SlotId The id of item slot.
	 * @param SourceItemId The id of item source.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserItemResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserInventoryItem(FString const& InventoryId
		, FString const& SlotId
		, FString const& SourceItemId
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk update user inventory items.
	 *
	 * @param InventoryId The id of user's inventory.
	 * @param UpdatedItemsRequest An array of items that will be updated.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsUpdateUserInventoryItemResponse>.
	 * @param OnError This will be called when the operation failed.
	 */
	void BulkUpdateInventoryItems(FString const& InventoryId
		, TArray<FAccelByteModelsUpdateUserInventoryItemRequest> const& UpdatedItemsRequest
		, THandler<TArray<FAccelByteModelsUpdateUserInventoryItemResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk delete user inventory items.
	 *
	 * @param InventoryId The id of user's inventory.
	 * @param DeletedItemsRequest An array of items that will be deleted.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsDeleteUserInventoryItemResponse>.
	 * @param OnError This will be called when the operation failed.
	 */
	void BulkDeleteInventoryItems(FString const& InventoryId
		, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
		, THandler<TArray<FAccelByteModelsDeleteUserInventoryItemResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk delete user inventory items.
	 *
	 * @param TargetInventoryId The target id of user's inventory.
	 * @param MoveItemsRequest Variable contains an array of items that will be moved as well as the inventory destination id.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsMoveUserItemsBetweenInventoriesResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void MoveItemsBetweenInventories(FString const& TargetInventoryId
		, FAccelByteModelsMoveUserItemsBetweenInventoriesRequest const& MoveItemsRequest
		, THandler<FAccelByteModelsMoveUserItemsBetweenInventoriesResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Consume user inventory item.
	 *
	 * @param InventoryId The id of user's inventory.
	 * @param ConsumedItemsRequest Variable contains an array of items that will be consumed.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserItemResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void ConsumeUserInventoryItem(FString const& InventoryId
		, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

#pragma endregion

private:
	Inventory() = delete;
	Inventory(Inventory const&) = delete;
	Inventory(Inventory&&) = delete;

	static constexpr int32 InventoryItemsLimit = 40;

	static FString ConvertInventoryConfigurationSortByToString(EAccelByteInventoryConfigurationSortBy const& SortBy);
	static FString ConvertUserInventoriesSortByToString(EAccelByteUserInventoriesSortBy const& SortBy);
	static FString ConvertItemTypeSortByToString(EAccelByteInventoryUtilitiesSortBy const& SortBy);
	static FString ConvertUserItemsSortByToString(EAccelByteUserItemsSortBy const& SortBy);

};

}
}