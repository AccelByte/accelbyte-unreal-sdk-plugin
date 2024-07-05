// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteInventoryApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{

Inventory::Inventory(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Inventory::~Inventory(){}

FString Inventory::ConvertInventoryConfigurationSortByToString(EAccelByteInventoryConfigurationSortBy SortBy)
{
	switch (SortBy)
	{
	case EAccelByteInventoryConfigurationSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteInventoryConfigurationSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteInventoryConfigurationSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteInventoryConfigurationSortBy::UPDATED_AT:
		return TEXT("updatedAt");
	case EAccelByteInventoryConfigurationSortBy::UPDATED_AT_ASC:
		return TEXT("updatedAt:asc");
	case EAccelByteInventoryConfigurationSortBy::UPDATED_AT_DESC:
		return TEXT("updatedAt:desc");
	case EAccelByteInventoryConfigurationSortBy::CODE:
		return TEXT("code");
	case EAccelByteInventoryConfigurationSortBy::CODE_ASC:
		return TEXT("code:asc");
	case EAccelByteInventoryConfigurationSortBy::CODE_DESC:
		return TEXT("code:desc");
	default:
		return TEXT("");
	}
}

FString Inventory::ConvertUserInventoriesSortByToString(EAccelByteUserInventoriesSortBy SortBy)
{
	switch (SortBy)
	{
	case EAccelByteUserInventoriesSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteUserInventoriesSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteUserInventoriesSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteUserInventoriesSortBy::UPDATED_AT:
		return TEXT("updatedAt");
	case EAccelByteUserInventoriesSortBy::UPDATED_AT_ASC:
		return TEXT("updatedAt:asc");
	case EAccelByteUserInventoriesSortBy::UPDATED_AT_DESC:
		return TEXT("updatedAt:desc");
	default:
		return TEXT("");
	}
}

FString Inventory::ConvertItemTypeSortByToString(EAccelByteInventoryUtilitiesSortBy SortBy)
{
	switch (SortBy)
	{
	case EAccelByteInventoryUtilitiesSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteInventoryUtilitiesSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteInventoryUtilitiesSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteInventoryUtilitiesSortBy::NAME:
		return TEXT("name");
	case EAccelByteInventoryUtilitiesSortBy::NAME_ASC:
		return TEXT("name:asc");
	case EAccelByteInventoryUtilitiesSortBy::NAME_DESC:
		return TEXT("name:desc");
	default:
		return TEXT("");
	}
}

FAccelByteTaskWPtr Inventory::GetInventoryConfigurations(THandler<FAccelByteModelsInventoryConfigurationsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteInventoryConfigurationSortBy SortBy
	, int32 Limit
	, int32 Offset
	, FString const& InventoryConfigurationCode)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/inventoryConfigurations")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), ConvertInventoryConfigurationSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("code"), InventoryConfigurationCode.IsEmpty() ? TEXT("") : InventoryConfigurationCode }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::GetInventoryTags(THandler<FAccelByteModelsInventoryTagPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteInventoryUtilitiesSortBy SortBy
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), ConvertItemTypeSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::GetUserInventories(THandler<FAccelByteModelsUserInventoriesPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUserInventoriesSortBy SortBy
	, int32 Limit
	, int32 Offset
	, FString const& InventoryConfigurationCode)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), ConvertUserInventoriesSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("inventoryConfigurationCode"), InventoryConfigurationCode.IsEmpty() ? TEXT("") : InventoryConfigurationCode }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::GetItemTypes(THandler<FAccelByteModelsItemTypePagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteInventoryUtilitiesSortBy SortBy
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/itemtypes")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), ConvertItemTypeSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::GetUserInventoryAllItems(FString const& InventoryId
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

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories/%s/items")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace()
		, *InventoryId);

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), FAccelByteInventoryUtilities::ConvertUserItemsSortByToString(SortBy) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sourceItemId"), SourceItemId.IsEmpty() ? TEXT("") : SourceItemId },
		{ TEXT("tags"), Tags.IsEmpty() ? TEXT("") : Tags }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::GetUserInventoryItem(FString const& InventoryId
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

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories/%s/slots/%s/sourceItems/%s")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace()
		, *InventoryId
		, *SlotId
		, *SourceItemId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::BulkUpdateInventoryItems(FString const& InventoryId
	, TArray<FAccelByteModelsUpdateUserInventoryItemRequest> const& UpdatedItemsRequest
	, THandler<TArray<FAccelByteModelsUpdateUserInventoryItemResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

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

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories/%s/items")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace()
		, *InventoryId);

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserInventoryItemRequest>(UpdatedItemsRequest, Content);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::BulkDeleteInventoryItems(FString const& InventoryId
	, TArray<FAccelByteModelsDeleteUserInventoryItemsRequest> const& DeletedItemsRequest
	, THandler<TArray<FAccelByteModelsDeleteUserInventoryItemResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

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

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories/%s/items")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace()
		, *InventoryId);

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsDeleteUserInventoryItemsRequest>(DeletedItemsRequest, Content);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::MoveItemsBetweenInventories(FString const& TargetInventoryId
	, FAccelByteModelsMoveUserItemsBetweenInventoriesRequest const& MoveItemsRequest
	, THandler<FAccelByteModelsMoveUserItemsBetweenInventoriesResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (TargetInventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, TargetInventoryId is empty."));
		return nullptr;
	}
	if (MoveItemsRequest.Items.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, MoveItemsRequest.Items cannot be 0."));
		return nullptr;
	}
	if (MoveItemsRequest.Items.Num() > InventoryItemsLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("MoveItemsRequest.Items cannot exceed %d!"), InventoryItemsLimit));
		return nullptr;
	}
	if (MoveItemsRequest.SrcInventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, MoveItemsRequest.SrcInventoryId is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories/%s/items/movement")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace()
		, *TargetInventoryId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, MoveItemsRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr Inventory::ConsumeUserInventoryItem(FString const& InventoryId
	, FAccelByteModelsConsumeUserItemsRequest const& ConsumedItemsRequest
	, THandler<FAccelByteModelsUserItemResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (InventoryId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, TargetInventoryId is empty."));
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

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/inventories/%s/consume")
		, *SettingsRef.InventoryServerUrl
		, *CredentialsRef->GetNamespace()
		, *InventoryId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ConsumedItemsRequest, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte