// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteItemBlueprints.h"
#include "AccelByteSettings.h"
#include "AccelByteItemApi.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Item;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsItem::GetItemById(const FString& ItemId, const FString& Region, const FString& Language, const FGetItemByIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Item::GetItemById(ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([OnSuccess](const FAccelByteModelsItemInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemsByCriteria(const FString& Language, const FString& Region, const FString& CategoryPath, EAccelByteItemType ItemType, EAccelByteItemStatus Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Item::GetItemsByCriteria(Language, Region, CategoryPath, ItemType, Status, Page, Size, Item::FGetItemsByCriteriaSuccess::CreateLambda([OnSuccess](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
