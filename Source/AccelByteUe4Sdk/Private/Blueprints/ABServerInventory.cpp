// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerInventory.h"

using namespace AccelByte;

void UABServerInventory::SetServerApiClient(FServerApiClientPtr const& NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerInventory::GetInventories(FDModelsPaginatedUserInventoriesResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, FString const& UserId
	, FString const& InventoryConfigurationCode
	, EAccelByteInventoriesSortBy SortBy
	, int32 Limit
	, int32 Offset)
{
	ApiClientPtr->ServerInventory.GetInventories(
		THandler<FAccelByteModelsUserInventoriesPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserInventoriesPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		UserId, InventoryConfigurationCode, SortBy, Limit, Offset);
}

void UABServerInventory::CreateInventory(FAccelByteModelsCreateInventoryRequest const& CreateInventoryRequest
	, FDModelsUserInventoryResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.CreateInventory(
		CreateInventoryRequest, 
		THandler<FAccelByteModelsUserInventoryResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserInventoryResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::GetInventory(FString const& InventoryId
	, FDModelsUserInventoryResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.GetInventory(
		InventoryId,
		THandler<FAccelByteModelsUserInventoryResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserInventoryResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::UpdateInventory(FString const& InventoryId
	, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
	, FDModelsUserInventoryResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.UpdateInventory(
		InventoryId,
		UpdateInventoryRequest,
		THandler<FAccelByteModelsUserInventoryResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserInventoryResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::UpdateAllUserInventories(FString const& UserId
	, FString const& InventoryConfigurationCode
	, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
	, FDModelsUpdateAllInventoriesResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.UpdateAllUserInventories(
		UserId,
		InventoryConfigurationCode,
		UpdateInventoryRequest,
		THandler<TArray<FAccelByteModelsUserInventoryResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUserInventoryResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::DeleteInventory(FString const& InventoryId
	, FAccelByteModelsDeleteInventoryRequest const& DeleteInventoryRequest
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.DeleteInventory(
		InventoryId,
		DeleteInventoryRequest,
		FVoidHandler::CreateLambda([&OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::GetInventoryAllItems(FString const& InventoryId
	, FDModelsPaginatedUserItemsResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, EAccelByteUserItemsSortBy const& SortBy
	, int32 Limit
	, int32 Offset
	, FString const& SourceItemId
	, FString const& Tags)
{
	ApiClientPtr->ServerInventory.GetInventoryAllItems(
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
		), SortBy, Offset, Limit, SourceItemId, Tags);
}

void UABServerInventory::GetInventoryItem(FString const& InventoryId
	, FString const& SlotId
	, FString const& SourceItemId
	, FDModelsUserItemResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.GetInventoryItem(
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
			}));
}

void UABServerInventory::ConsumeInventoryItem(FString const& UserId
	, FString const& InventoryId
	, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
	, FDModelsUserItemResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.ConsumeInventoryItem(
		UserId,
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
			}));
}

void UABServerInventory::BulkUpdateInventoryItems(FString const& UserId
	, FString const& InventoryId
	, TArray<FAccelByteModelsUpdateInventoryItemRequest> const& UpdatedItemsRequest
	, FDModelsUpdateInventoryItemsResponses const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.BulkUpdateInventoryItems(
		UserId,
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
			}));
}

void UABServerInventory::SaveInventoryItemByInventoryId(FString const& UserId
	, FString const& InventoryId
	, FAccelByteModelsSaveInventoryItemByInventoryIdRequest const& SaveItemRequest
	, FDModelsUserItemResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.SaveInventoryItemByInventoryId(
		UserId,
		InventoryId,
		SaveItemRequest,
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
			}));
}

void UABServerInventory::BulkDeleteUserInventoryItems(FString const& UserId
	, FString const& InventoryId
	, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
	, FDModelsDeleteUserItemsResponses const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.BulkDeleteUserInventoryItems(
		UserId,
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
			}));
}

void UABServerInventory::SaveInventoryItem(FString const& UserId
	, FAccelByteModelsSaveInventoryItemRequest const& SaveItemRequest
	, FDModelsUserItemResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.SaveInventoryItem(
		UserId,
		SaveItemRequest,
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
			}));
}

void UABServerInventory::BulkSaveInventoryItems(FString const& UserId
	, TArray<FAccelByteModelsSaveInventoryItemRequest> const& SaveItemsRequests
	, FDModelsSaveInventoryItemsResponses const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.BulkSaveInventoryItems(
		UserId,
		SaveItemsRequests,
		THandler<TArray<FAccelByteModelsBulkSaveInventoryItems>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkSaveInventoryItems> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::BulkSaveInventoryItemsByInventoryId(FString const& UserId
	, FString const& InventoryId
	, TArray<FAccelByteModelsSaveInventoryItemByInventoryIdRequest> const& SaveItemsRequests
	, FDModelsSaveInventoryItemsResponses const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.BulkSaveInventoryItemsByInventoryId(
		UserId,
		InventoryId,
		SaveItemsRequests,
		THandler<TArray<FAccelByteModelsBulkSaveInventoryItems>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkSaveInventoryItems> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerInventory::SyncUserEntitlement(FString const& UserId
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerInventory.SyncUserEntitlement(
		UserId,
		FVoidHandler::CreateLambda([&OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
