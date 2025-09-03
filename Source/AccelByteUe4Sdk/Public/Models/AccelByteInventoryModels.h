// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteInventoryModels.generated.h"

#pragma region V1 Enum

UENUM(BlueprintType)
enum class EAccelByteInventoryConfigurationSortBy : uint8
{
	NONE = 0,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	UPDATED_AT,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC,
	CODE,
	CODE_ASC,
	CODE_DESC
};

UENUM(BlueprintType)
enum class EAccelByteInventoryUtilitiesSortBy : uint8
{
	NONE = 0,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	NAME,
	NAME_ASC,
	NAME_DESC
};

UENUM(BlueprintType)
enum class EAccelByteUserInventoriesSortBy : uint8
{
	NONE = 0,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	UPDATED_AT,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC
};

UENUM(BlueprintType)
enum class EAccelByteUserItemsSortBy : uint8
{
	NONE = 0,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	UPDATED_AT,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC,
};

UENUM(BlueprintType)
enum class EAccelByteInventoriesSortBy : uint8
{
	NONE = 0,
	CREATED_AT,
	CREATED_AT_ASC,
	CREATED_AT_DESC,
	UPDATED_AT,
	UPDATED_AT_ASC,
	UPDATED_AT_DESC,
	CONFIGURATION_CODE, // Inventory Configuration Code
	CONFIGURATION_CODE_ASC, // Inventory Configuration Code Ascending
	CONFIGURATION_CODE_DESC // Inventory Configuration Code Descending
};

#pragma endregion

class ACCELBYTEUE4SDK_API FAccelByteInventoryUtilities
{
public:
	static FString ConvertUserItemsSortByToString(EAccelByteUserItemsSortBy SortBy)
	{
		switch (SortBy)
		{
		case EAccelByteUserItemsSortBy::CREATED_AT:
			return TEXT("createdAt");
		case EAccelByteUserItemsSortBy::CREATED_AT_ASC:
			return TEXT("createdAt:asc");
		case EAccelByteUserItemsSortBy::CREATED_AT_DESC:
			return TEXT("createdAt:desc");
		case EAccelByteUserItemsSortBy::UPDATED_AT:
			return TEXT("updatedAt");
		case EAccelByteUserItemsSortBy::UPDATED_AT_ASC:
			return TEXT("updatedAt:asc");
		case EAccelByteUserItemsSortBy::UPDATED_AT_DESC:
			return TEXT("updatedAt:desc");
		default:
			return TEXT("");
		}
	}
};

#pragma region V1 Struct Models
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsInventoryConfigurationResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	int32 InitialMaxSlots{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	int32 MaxInstancesPerUser{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	int32 MaxUpgradeSlots{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FString Name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FString Status{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configuration")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsInventoryConfigurationsPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configurations Paging")
	TArray<FAccelByteModelsInventoryConfigurationResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Configurations Paging")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemTypeResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Type")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Type")
	FString Name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Type")
	FString Namespace{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemTypePagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Type Paging")
	TArray<FAccelByteModelsItemTypeResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Type Paging")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsInventoryTagResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Tag")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Tag")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Tag")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Tag")
	FString Owner{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsInventoryTagPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Tags Paging")
	TArray<FAccelByteModelsInventoryTagResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Tags Paging")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserInventoryResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FString InventoryConfigurationCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FString InventoryConfigurationId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	int32 MaxSlots{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	int32 MaxUpgradeSlots{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	int32 UsedCountSlots{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventory Data")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserInventoriesPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	TArray<FAccelByteModelsUserInventoryResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserItemResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FJsonObjectWrapper CustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString InventoryId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FJsonObjectWrapper ItemInfo{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	bool PlatformAvailable{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	int32 Qty{0}; // Quantity
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FJsonObjectWrapper ServerCustomAttributes{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString SlotId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	int32 SlotUsed{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString Source{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString SourceItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Item Data")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserItemsPagingResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	TArray<FAccelByteModelsUserItemResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserInventoryItemErrorDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item Error Details")
	int32 ErrorCode{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item Error Details")
	FString ErrorMessage{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSaveInventoryItemsErrorDetails : public FAccelByteModelsUpdateUserInventoryItemErrorDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Inventory Items Error Details")
	FString SlotId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Inventory Items Error Details")
	FString SourceItemId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserInventoryItemRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FJsonObjectWrapper CustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FString SlotId{}; // Not Mandatory

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FString SourceItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	TArray<FString> Tags{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateUserInventoryItemResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FAccelByteModelsUpdateUserInventoryItemErrorDetails ErrorDetails{}; // It will be blank if Success = true

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FString SlotId{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FString SourceItemId{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	bool Success{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeleteUserInventoryItemsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Delete User Inventory Items")
	FString SlotId{}; // Not Mandatory

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Delete User Inventory Items")
	FString SourceItemId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeleteUserInventoryItemResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FAccelByteModelsUpdateUserInventoryItemErrorDetails ErrorDetails{}; // It will be blank if Success = true

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FString SlotId{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	FString SourceItemId{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update User Inventorie Item")
	bool Success{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMoveUserItemDataRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	int32 Qty{0}; // Quantity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	FString SlotId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	FString SourceItemId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMoveUserItemsBetweenInventoriesRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	TArray<FAccelByteModelsMoveUserItemDataRequest> Items{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	FString SrcInventoryId{}; // Source Inventory Id
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMoveUserItemDataResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	int32 Qty{0}; // Quantity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	FString SlotId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	FString SourceItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Item Data")
	FString Id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMoveUserItemsBetweenInventoriesResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	FString DstInventoryId{}; // Destination Inventory Id

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	TArray<FAccelByteModelsMoveUserItemDataResponse> Items{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Move User Items Between Inventories")
	FString SrcInventoryId{}; // Source Inventory Id
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsConsumeUserItemsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Consume User Item")
	int32 Qty{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Consume User Item")
	FString SlotId{}; // Not Mandatory

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Consume User Item")
	FString SourceItemId{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreateInventoryRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Create Inventory")
	FString InventoryConfigurationCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Create Inventory")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateInventoryRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Update Inventory")
	int32 IncMaxSlots{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeleteInventoryRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Delete Inventory")
	FString Message{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateInventoryItemRequest : public FAccelByteModelsUpdateUserInventoryItemRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Delete Inventory")
	FJsonObjectWrapper ServerCustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Delete Inventory")
	FString Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSaveInventoryItemByInventoryIdRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	FJsonObjectWrapper customAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	int32 Qty{ 0 }; // Quantity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	FJsonObjectWrapper serverCustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	FString SlotId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	int32 SlotUsed{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	FString Source{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	FString SourceItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Item For Specific Inventory")
	FString Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSaveInventoryItemRequest : public FAccelByteModelsSaveInventoryItemByInventoryIdRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Save Inventory Item")
	FString InventoryConfigurationCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkSaveInventoryItems
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	FAccelByteModelsSaveInventoryItemsErrorDetails ErrorDetails{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	FAccelByteModelsUserItemResponse Item{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Inventory | Models | Inventories Data")
	bool Success{ false };
};
#pragma endregion