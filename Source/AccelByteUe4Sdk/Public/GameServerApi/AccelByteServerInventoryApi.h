// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteInventoryModels.h"

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
	ServerInventory(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler & InHttpRef);
	~ServerInventory();

#pragma region V1 Inventory

	FAccelByteTaskWPtr GetInventories(THandler<FAccelByteModelsUserInventoriesPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& UserId = TEXT("")
		, FString const& InventoryConfigurationCode = TEXT("")
		, EAccelByteInventoriesSortBy SortBy = EAccelByteInventoriesSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0);

	FAccelByteTaskWPtr CreateInventory(FAccelByteModelsCreateInventoryRequest const& CreateInventoryRequest
		, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr GetInventory(FString const& InventoryId
		, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr UpdateInventory(FString const& InventoryId
		, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
		, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr DeleteInventory(FString const& InventoryId
		, FAccelByteModelsDeleteInventoryRequest const& DeleteInventoryRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

#pragma endregion

#pragma region V1 Item

	FAccelByteTaskWPtr GetInventoryAllItems(FString const& InventoryId
		, THandler<FAccelByteModelsUserItemsPagingResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteUserItemsSortBy SortBy = EAccelByteUserItemsSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0
		, FString const& SourceItemId = TEXT("")
		, FString const& Tags = TEXT(""));

	FAccelByteTaskWPtr GetInventoryItem(FString const& InventoryId
		, FString const& SlotId
		, FString const& SourceItemId
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr ConsumeInventoryItem(FString const& UserId
		, FString const& InventoryId
		, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr BulkUpdateInventoryItems(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsUpdateInventoryItemRequest> const& UpdatedItemsRequest
		, THandler<TArray<FAccelByteModelsUpdateUserInventoryItemResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr SaveInventoryItemByInventoryId(FString const& UserId
		, FString const& InventoryId
		, FAccelByteModelsSaveInventoryItemByInventoryIdRequest const& SaveItemRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr BulkDeleteUserInventoryItems(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
		, THandler<TArray<FAccelByteModelsDeleteUserInventoryItemResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	FAccelByteTaskWPtr SaveInventoryItem(FString const& UserId
		, FAccelByteModelsSaveInventoryItemRequest const& SaveItemRequest
		, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
		, FErrorHandler const& OnError);

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
