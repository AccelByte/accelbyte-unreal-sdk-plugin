// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "Blueprints/ABInventory.h"

using namespace AccelByte;

void UABInventory::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABInventory::GetInventoryConfigurations(
	FDModelsPaginatedInventoryTypesResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteInventoryConfigurationSortBy const& SortBy,
	int32 const& Limit,
	int32 const& Offset,
	FString const& InventoryConfigurationCode
)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->GetInventoryConfigurations(
		THandler<FAccelByteModelsInventoryConfigurationsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsInventoryConfigurationsPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		SortBy,
		Limit,
		Offset,
		InventoryConfigurationCode
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::GetInventoryTags(
	FDModelsPaginatedInventoryTagsResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteInventoryUtilitiesSortBy const& SortBy,
	int32 const& Limit,
	int32 const& Offset)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->GetInventoryTags(
		THandler<FAccelByteModelsInventoryTagPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsInventoryTagPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		SortBy,
		Limit,
		Offset
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::GetUserInventories(
	FDModelsPaginatedUserInventoriesResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	EAccelByteUserInventoriesSortBy const& SortBy, 
	int32 const& Limit, 
	int32 const& Offset, 
	FString const& InventoryConfigurationCode)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->GetUserInventories(
		THandler<FAccelByteModelsUserInventoriesPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserInventoriesPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		SortBy,
		Limit,
		Offset,
		InventoryConfigurationCode
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::GetItemTypes(
	FDModelsPaginatedItemTypesResponse const& OnSuccess,
	FDErrorHandler const& OnError, 
	EAccelByteInventoryUtilitiesSortBy const& SortBy, 
	int32 const& Limit, 
	int32 const& Offset)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->GetItemTypes(
		THandler<FAccelByteModelsItemTypePagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsItemTypePagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		SortBy,
		Limit,
		Offset
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::GetUserInventoryAllItems(
	FString const& InventoryId, 
	FDModelsPaginatedUserItemsResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	EAccelByteUserItemsSortBy const& SortBy, 
	int32 Limit, 
	int32 Offset, 
	FString const& SourceItemId, 
	FString const& Tags)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->GetUserInventoryAllItems(
		InventoryId,
		THandler<FAccelByteModelsUserItemsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserItemsPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		SortBy,
		Limit,
		Offset,
		SourceItemId,
		Tags
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::GetUserInventoryItem(
	FString const& InventoryId, 
	FString const& SlotId,
	FString const& SourceItemId,
	FDModelsUserItemResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->GetUserInventoryItem(
		InventoryId,
		SlotId,
		SourceItemId,
		THandler<FAccelByteModelsUserItemResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserItemResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::BulkUpdateInventoryItems(
	FString const& InventoryId, 
	TArray<FAccelByteModelsUpdateUserInventoryItemRequest> const& UpdatedItemsRequest, 
	FDModelsUpdateUserItemsResponses const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->BulkUpdateInventoryItems(
		InventoryId,
		UpdatedItemsRequest,
		THandler<TArray<FAccelByteModelsUpdateUserInventoryItemResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUpdateUserInventoryItemResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::BulkDeleteInventoryItems(
	FString const& InventoryId,
	TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest,
	FDModelsDeleteUserItemsResponses const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->BulkDeleteInventoryItems(
		InventoryId,
		DeletedItemsRequest,
		THandler<TArray<FAccelByteModelsDeleteUserInventoryItemResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsDeleteUserInventoryItemResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::MoveItemsBetweenInventories(
	FString const& TargetInventoryId, 
	FAccelByteModelsMoveUserItemsBetweenInventoriesRequest const& MoveItemsRequest,
	FDModelsMoveUserItemsBetweenInventoriesResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->MoveItemsBetweenInventories(
		TargetInventoryId,
		MoveItemsRequest,
		THandler<FAccelByteModelsMoveUserItemsBetweenInventoriesResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsMoveUserItemsBetweenInventoriesResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABInventory::ConsumeUserInventoryItem(
	FString const& InventoryId,
	FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest,
	FDModelsUserItemResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto InventoryPtr = ApiClientPtr->GetInventoryApi().Pin();
	if (InventoryPtr.IsValid())
	{
		InventoryPtr->ConsumeUserInventoryItem(
		InventoryId,
		ConsumedItemsRequest,
		THandler<FAccelByteModelsUserItemResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserItemResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}