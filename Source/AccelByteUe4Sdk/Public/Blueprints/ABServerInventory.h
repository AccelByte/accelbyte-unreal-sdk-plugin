// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "ABInventory.h"
#include "Models/AccelByteInventoryModels.h"
#include "ABServerInventory.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUpdateInventoryItemsResponses, TArray<FAccelByteModelsUpdateUserInventoryItemResponse>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUpdateAllInventoriesResponse, TArray<FAccelByteModelsUserInventoryResponse>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsSaveInventoryItemsResponses, TArray<FAccelByteModelsBulkSaveInventoryItems>, Response);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABServerInventory : public UObject
{
	GENERATED_BODY()

public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

#pragma region V1 Inventory

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void GetInventories(FDModelsPaginatedUserInventoriesResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, FString const& UserId = TEXT("")
		, FString const& InventoryConfigurationCode = TEXT("")
		, EAccelByteInventoriesSortBy SortBy = EAccelByteInventoriesSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void CreateInventory(FAccelByteModelsCreateInventoryRequest const& CreateInventoryRequest
		, FDModelsUserInventoryResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void GetInventory(FString const& InventoryId
		, FDModelsUserInventoryResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void UpdateInventory(FString const& InventoryId
		, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
		, FDModelsUserInventoryResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void UpdateAllUserInventories(FString const& UserId
		, FString const& InventoryConfigurationCode
		, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
		, FDModelsUpdateAllInventoriesResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void DeleteInventory(FString const& InventoryId
		, FAccelByteModelsDeleteInventoryRequest const& DeleteInventoryRequest
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

#pragma endregion

#pragma region V1 Item

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void GetInventoryAllItems(FString const& InventoryId
		, FDModelsPaginatedUserItemsResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, EAccelByteUserItemsSortBy const& SortBy = EAccelByteUserItemsSortBy::CREATED_AT_DESC
		, int32 Limit = 20
		, int32 Offset = 0
		, FString const& SourceItemId = TEXT("")
		, FString const& Tags = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void GetInventoryItem(FString const& InventoryId
		, FString const& SlotId
		, FString const& SourceItemId
		, FDModelsUserItemResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void ConsumeInventoryItem(FString const& UserId
		, FString const& InventoryId
		, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
		, FDModelsUserItemResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void BulkUpdateInventoryItems(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsUpdateInventoryItemRequest> const& UpdatedItemsRequest
		, FDModelsUpdateInventoryItemsResponses const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void SaveInventoryItemByInventoryId(FString const& UserId
		, FString const& InventoryId
		, FAccelByteModelsSaveInventoryItemByInventoryIdRequest const& SaveItemRequest
		, FDModelsUserItemResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void BulkDeleteUserInventoryItems(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
		, FDModelsDeleteUserItemsResponses const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void SaveInventoryItem(FString const& UserId
		, FAccelByteModelsSaveInventoryItemRequest const& SaveItemRequest
		, FDModelsUserItemResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void BulkSaveInventoryItems(FString const& UserId
		, TArray<FAccelByteModelsSaveInventoryItemRequest> const& SaveItemsRequests
		, FDModelsSaveInventoryItemsResponses const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void BulkSaveInventoryItemsByInventoryId(FString const& UserId
		, FString const& InventoryId
		, TArray<FAccelByteModelsSaveInventoryItemByInventoryIdRequest> const& SaveItemsRequests
		, FDModelsSaveInventoryItemsResponses const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Inventory | Api")
	void SyncUserEntitlement(FString const& UserId
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

#pragma endregion

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
