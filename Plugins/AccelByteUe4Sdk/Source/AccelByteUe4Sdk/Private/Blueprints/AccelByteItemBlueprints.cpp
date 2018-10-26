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

void UAccelByteBlueprintsItem::GetItemById(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& ItemId, const FString& Region, const FString& Language, const FGetItemByIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Item::GetItemById(AccessToken, Namespace, ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([OnSuccess](const FAccelByteModelsItemInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemByIdEasy(const FString& ItemId, const FString& Region, const FString& Language, const FGetItemByIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetItemById(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), ItemId, Region, Language, OnSuccess, OnError);
}


void UAccelByteBlueprintsItem::GetItemsByCriteria(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& Language, const FString& Region, const FString& CategoryPath, const FString& ItemType, const FString& Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Item::GetItemsByCriteria(AccessToken, Namespace, Language, Region, CategoryPath, ItemType, Status, Page, Size, Item::FGetItemsByCriteriaSuccess::CreateLambda([OnSuccess](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemsByCriteriaEasy(const FString& Language, const FString& Region, const FString& CategoryPath, const FString& ItemType, const FString& Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetItemsByCriteria(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Language, Region, CategoryPath, ItemType, Status, Page, Size, OnSuccess, OnError);
}
