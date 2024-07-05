// Copyright (c) 2023 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteInventoryModels.h"
#include "ABInventory.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedInventoryTypesResponse, FAccelByteModelsInventoryConfigurationsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedItemTypesResponse, FAccelByteModelsItemTypePagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedInventoryTagsResponse, FAccelByteModelsInventoryTagPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserInventoryResponse, FAccelByteModelsUserInventoryResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedUserInventoriesResponse, FAccelByteModelsUserInventoriesPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedUserItemsResponse, FAccelByteModelsUserItemsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserItemResponse, FAccelByteModelsUserItemResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUpdateUserItemsResponses, TArray<FAccelByteModelsUpdateUserInventoryItemResponse>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsDeleteUserItemsResponses, TArray<FAccelByteModelsDeleteUserInventoryItemResponse>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsMoveUserItemsBetweenInventoriesResponse, FAccelByteModelsMoveUserItemsBetweenInventoriesResponse, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABInventory : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

#pragma region V1 Inventory

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void GetInventoryConfigurations(
		FDModelsPaginatedInventoryTypesResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteInventoryConfigurationSortBy const& SortBy = EAccelByteInventoryConfigurationSortBy::CREATED_AT_DESC,
		int32 const& Limit = 20,
		int32 const& Offset = 0,
		FString const& InventoryConfigurationCode = TEXT("")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void GetInventoryTags(
		FDModelsPaginatedInventoryTagsResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteInventoryUtilitiesSortBy const& SortBy = EAccelByteInventoryUtilitiesSortBy::CREATED_AT_DESC,
		int32 const& Limit = 20,
		int32 const& Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void GetUserInventories(
		FDModelsPaginatedUserInventoriesResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteUserInventoriesSortBy const& SortBy = EAccelByteUserInventoriesSortBy::CREATED_AT_DESC,
		int32 const& Limit = 20,
		int32 const& Offset = 0,
		FString const& InventoryConfigurationCode = TEXT("")
	);

#pragma endregion

#pragma region V1 Item

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void GetItemTypes(
		FDModelsPaginatedItemTypesResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteInventoryUtilitiesSortBy const& SortBy = EAccelByteInventoryUtilitiesSortBy::CREATED_AT_DESC,
		int32 const& Limit = 20,
		int32 const& Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void GetUserInventoryAllItems(
		FString const& InventoryId,
		FDModelsPaginatedUserItemsResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteUserItemsSortBy const& SortBy = EAccelByteUserItemsSortBy::CREATED_AT_DESC,
		int32 Limit = 20,
		int32 Offset = 0,
		FString const& SourceItemId = TEXT(""),
		FString const& Tags = TEXT("")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void GetUserInventoryItem(FString const& InventoryId,
		FString const& SlotId,
		FString const& SourceItemId,
		FDModelsUserItemResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void BulkUpdateInventoryItems(FString const& InventoryId,
		TArray<FAccelByteModelsUpdateUserInventoryItemRequest> const& UpdatedItemsRequest,
		FDModelsUpdateUserItemsResponses const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void BulkDeleteInventoryItems(FString const& InventoryId,
		TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> DeletedItemsRequest,
		FDModelsDeleteUserItemsResponses const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void MoveItemsBetweenInventories(FString const& TargetInventoryId,
		FAccelByteModelsMoveUserItemsBetweenInventoriesRequest const& MoveItemsRequest,
		FDModelsMoveUserItemsBetweenInventoriesResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Inventory | Api")
	void ConsumeUserInventoryItem(FString const& InventoryId,
		FAccelByteModelsConsumeUserItemsRequest ConsumedItemsRequest,
		FDModelsUserItemResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

#pragma endregion

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};