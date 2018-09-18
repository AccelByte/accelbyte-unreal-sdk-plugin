// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalogBlueprints.h"
#include "JusticeCatalog.h"

#pragma region GetChildCategories

UGetChildCategories * UGetChildCategories::GetChildCategories(FString Language, FString CategoryPath)
{
	UGetChildCategories* Node = NewObject<UGetChildCategories>();
	Node->Language = Language;
	Node->CategoryPath = CategoryPath;
	return Node;
}

void UGetChildCategories::Activate()
{
	JusticeCatalog::GetChildCategories(this->Language, this->CategoryPath, FCategoryDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FCategory> Result) {
		TArray<UCategory*> ChildCategories;
		if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UCategory* ChildCategory = UCategory::Deserialize(Result[i]);
				check(ChildCategory);
				ChildCategories.Add(ChildCategory);
			}
			OnSuccess.Broadcast(ChildCategories);
		}
		else
		{
			OnFailed.Broadcast(ChildCategories);
		}
	}));
}

#pragma endregion GetChildCategories

#pragma region GetDescendantCategories

UGetDescendantCategories * UGetDescendantCategories::GetDescendantCategories(FString Language, FString CategoryPath)
{
	UGetDescendantCategories* Node = NewObject<UGetDescendantCategories>();
	Node->Language = Language;
	Node->CategoryPath = CategoryPath;
	return Node;
}

void UGetDescendantCategories::Activate()
{
	JusticeCatalog::GetDescendantCategories(this->Language, this->CategoryPath, FCategoryDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FCategory> Result) {
		TArray<UCategory*> DescendantCategories;
		if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UCategory* DescendantCategory = UCategory::Deserialize(Result[i]);
				check(DescendantCategory);
				DescendantCategories.Add(DescendantCategory);
			}
			OnSuccess.Broadcast(DescendantCategories);
		}
		else
		{
			OnFailed.Broadcast(DescendantCategories);
		}
	}));
}

#pragma endregion GetDescendantCategories

#pragma region GetItemById

UGetItemById * UGetItemById::GetItemById(FString ItemID, FString Region, FString Language)
{
	UGetItemById* Node = NewObject<UGetItemById>();
	Node->Language = Language;
	Node->Region = Region;
	Node->ItemID = ItemID;
	return Node;
}

void UGetItemById::Activate()
{
	JusticeCatalog::GetItemById(this->ItemID, this->Region, this->Language, FGetItemCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, FItemInfoJustice* Result) {
		UItemInfo* ItemResult = NewObject<UItemInfo>();
		if (bSuccessful)
		{
			ItemResult = UItemInfo::Deserialize(Result);
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(ItemResult);
			}
		}
		else
		{
		OnFailed.Broadcast(ItemResult);
		}
	}));
}

#pragma endregion GetItemById

#pragma region GetItemsByQuery

UGetItemsByQuery * UGetItemsByQuery::GetItemsByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size)
{
	UGetItemsByQuery* Node = NewObject<UGetItemsByQuery>();
	Node->Language = Language;
	Node->Region = Region;
	Node->CategoryPath = CategoryPath;
	Node->ItemType = ItemType;
	Node->Status = Status;
	Node->Page = Page;
	Node->Size = Size;
	return Node;
}

void UGetItemsByQuery::Activate()
{
	JusticeCatalog::GetItemsByQuery(this->Language, this->Region, this->CategoryPath, this->ItemType, this->Status, this->Page, this->Size, FItemCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FItemInfoJustice> Result) {
		TArray<UItemInfo*> FinalResult;
		if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UItemInfo* item = UItemInfo::Deserialize(Result[i]);
				check(item);
				FinalResult.Add(item);
			}
			OnSuccess.Broadcast(FinalResult);
		}
		else
		{
			OnFailed.Broadcast(FinalResult);
		}
	}));
}

#pragma endregion GetItemsByQuery
