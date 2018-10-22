// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteItemBlueprints.h"
#include "AccelByteSettings.h"
#include "AccelByteItemApi.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Item;
using AccelByte::FErrorDelegate;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsItem::GetItemById(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString ItemId, FString Region, FString Language, FGetItemByIdSuccess OnSuccess, FBlueprintError OnError)
{
	Item::GetItemById(ServerBaseUrl, AccessToken, Namespace, ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([OnSuccess](const FAccelByteModelsItemInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemByIdEasy(FString ItemId, FString Region, FString Language, FGetItemByIdSuccess OnSuccess, FBlueprintError OnError)
{
	GetItemById(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), ItemId, Region, Language, OnSuccess, OnError);
}


void UAccelByteBlueprintsItem::GetItemsByCriteria(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FGetItemsByCriteriaSuccess OnSuccess, FBlueprintError OnError)
{
	Item::GetItemsByCriteria(ServerBaseUrl, AccessToken, Namespace, Language, Region, CategoryPath, ItemType, Status, Page, Size, Item::FGetItemsByCriteriaSuccess::CreateLambda([OnSuccess](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsItem::GetItemsByCriteriaEasy(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FGetItemsByCriteriaSuccess OnSuccess, FBlueprintError OnError)
{
	GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Language, Region, CategoryPath, ItemType, Status, Page, Size, OnSuccess, OnError);
}
