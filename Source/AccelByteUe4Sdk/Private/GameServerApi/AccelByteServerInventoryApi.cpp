// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerInventoryApi.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerInventory::ServerInventory(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{}

ServerInventory::~ServerInventory()
{}

FString ServerInventory::ConvertInventoriesSortByToString(EAccelByteInventoriesSortBy SortBy)
{
	switch (SortBy)
	{
	case EAccelByteInventoriesSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteInventoriesSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteInventoriesSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteInventoriesSortBy::UPDATED_AT:
		return TEXT("updatedAt");
	case EAccelByteInventoriesSortBy::UPDATED_AT_ASC:
		return TEXT("updatedAt:asc");
	case EAccelByteInventoriesSortBy::UPDATED_AT_DESC:
		return TEXT("updatedAt:desc");
	case EAccelByteInventoriesSortBy::CONFIGURATION_CODE:
		return TEXT("inventoryConfigurationCode");
	case EAccelByteInventoriesSortBy::CONFIGURATION_CODE_ASC:
		return TEXT("inventoryConfigurationCode:asc");
	case EAccelByteInventoriesSortBy::CONFIGURATION_CODE_DESC:
		return TEXT("inventoryConfigurationCode:desc");
	default:
		return TEXT("");
	}
}

FString ServerInventory::ConvertInventoryItemsSortByToString(EAccelByteUserItemsSortBy SortBy)
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

FAccelByteTaskWPtr ServerInventory::GetInventories(THandler<FAccelByteModelsUserInventoriesPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& UserId
	, FString const& InventoryConfigurationCode
	, EAccelByteInventoriesSortBy SortBy
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace()));

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), ConvertInventoriesSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("inventoryConfigurationCode"), InventoryConfigurationCode.IsEmpty() ? TEXT("") : InventoryConfigurationCode },
		{ TEXT("userId"), UserId.IsEmpty() ? TEXT("") : UserId }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::CreateInventory(FAccelByteModelsCreateInventoryRequest const& CreateInventoryRequest
	, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CreateInventoryRequest.UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, CreateInventoryRequest.UserId is empty."));
		return nullptr;
	}
	if (CreateInventoryRequest.InventoryConfigurationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, CreateInventoryRequest.InventoryConfigurationCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, CreateInventoryRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::GetInventory(FString const& InventoryId
	, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories/%s")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::UpdateInventory(FString const& InventoryId
	, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
	, THandler<FAccelByteModelsUserInventoryResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories/%s")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateInventoryRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::UpdateAllUserInventories(FString const& UserId
	, FString const& InventoryConfigurationCode
	, FAccelByteModelsUpdateInventoryRequest const& UpdateInventoryRequest
	, THandler<TArray<FAccelByteModelsUserInventoryResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (InventoryConfigurationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryConfigurationCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/inventoryConfigurations/%s/inventories")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(InventoryConfigurationCode));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateInventoryRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::DeleteInventory(FString const& InventoryId
	, FAccelByteModelsDeleteInventoryRequest const& DeleteInventoryRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories/%s")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, DeleteInventoryRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::GetInventoryAllItems(FString const& InventoryId
	, THandler<FAccelByteModelsUserItemsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUserItemsSortBy SortBy
	, int32 Limit
	, int32 Offset
	, FString const& SourceItemId
	, FString const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories/%s/items")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), FAccelByteInventoryUtilities::ConvertUserItemsSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sourceItemId"), SourceItemId.IsEmpty() ? TEXT("") : SourceItemId },
		{ TEXT("tags"), Tags.IsEmpty() ? TEXT("") : Tags }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::GetInventoryItem(FString const& InventoryId
	, FString const& SlotId
	, FString const& SourceItemId
	, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}
	if (SlotId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SlotId is empty."));
		return nullptr;
	}
	if (SourceItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SourceItemId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/inventories/%s/slots/%s/sourceItems/%s")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(InventoryId)
		, *FGenericPlatformHttp::UrlEncode(SlotId)
		, *FGenericPlatformHttp::UrlEncode(SourceItemId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::ConsumeInventoryItem(FString const& UserId
	, FString const& InventoryId
	, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
	, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}
	if (ConsumedItemsRequest.Qty == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ConsumedItemsRequest.Qty cannot be 0."));
		return nullptr;
	}
	if (ConsumedItemsRequest.SourceItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ConsumedItemsRequest.SourceItemId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/inventories/%s/consume")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ConsumedItemsRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::BulkUpdateInventoryItems(FString const& UserId
	, FString const& InventoryId
	, TArray<FAccelByteModelsUpdateInventoryItemRequest> const& UpdatedItemsRequest
	, THandler<TArray<FAccelByteModelsUpdateUserInventoryItemResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}
	if (UpdatedItemsRequest.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, UpdatedItemsRequest cannot be 0."));
		return nullptr;
	}
	if (UpdatedItemsRequest.Num() > InventoryItemsLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("UpdatedItemsRequest cannot exceed %d!"), InventoryItemsLimit));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/inventories/%s/items")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateInventoryItemRequest>(UpdatedItemsRequest, Content);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::SaveInventoryItemByInventoryId(FString const& UserId
	, FString const& InventoryId
	, FAccelByteModelsSaveInventoryItemByInventoryIdRequest const& SaveItemRequest
	, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}
	if (SaveItemRequest.SlotId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SlotId is empty."));
		return nullptr;
	}
	if (SaveItemRequest.SourceItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SourceItemId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/inventories/%s/items")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, SaveItemRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::BulkDeleteUserInventoryItems(FString const& UserId
	, FString const& InventoryId
	, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
	, THandler<TArray<FAccelByteModelsDeleteUserInventoryItemResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryId is empty."));
		return nullptr;
	}
	if (DeletedItemsRequest.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, DeletedItemsRequest cannot be 0."));
		return nullptr;
	}
	if (DeletedItemsRequest.Num() > InventoryItemsLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Items cannot exceed %d!"), InventoryItemsLimit));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/inventories/%s/items")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsDeleteUserInventoryItemsRequest>(DeletedItemsRequest, Content);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::SaveInventoryItem(FString const& UserId
	, FAccelByteModelsSaveInventoryItemRequest const& SaveItemRequest
	, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (SaveItemRequest.SlotId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SlotId is empty."));
		return nullptr;
	}
	if (SaveItemRequest.SourceItemId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SourceItemId is empty."));
		return nullptr;
	}
	if (SaveItemRequest.InventoryConfigurationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, InventoryConfigurationCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/items")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, SaveItemRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::BulkSaveInventoryItems(FString const& UserId
	, TArray<FAccelByteModelsSaveInventoryItemRequest> const& SaveItemsRequests
	, THandler<TArray<FAccelByteModelsBulkSaveInventoryItems>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (SaveItemsRequests.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SaveItemsRequests is empty."));
		return nullptr;
	}
	else if (SaveItemsRequests.Num() > MaxSaveInventoryItems)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Maximum SaveItemsRequests is 10 for each call."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/items/bulk")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsSaveInventoryItemRequest>(SaveItemsRequests, Content);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::BulkSaveInventoryItemsByInventoryId(FString const& UserId
	, FString const& InventoryId
	, TArray<FAccelByteModelsSaveInventoryItemByInventoryIdRequest> const& SaveItemsRequests
	, THandler<TArray<FAccelByteModelsBulkSaveInventoryItems>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (SaveItemsRequests.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, SaveItemsRequests is empty."));
		return nullptr;
	}
	else if (SaveItemsRequests.Num() > MaxSaveInventoryItems)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Maximum SaveItemsRequests is 10 for each call."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/inventories/%s/items/bulk")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(InventoryId));

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsSaveInventoryItemByInventoryIdRequest>(SaveItemsRequests, Content);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerInventory::SyncUserEntitlement(FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/items/entitlements/sync")
		, *ServerSettingsRef.InventoryServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
