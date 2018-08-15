// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/CatalogBlueprint/GetItem.h"
#include "Services/JusticeCatalog.h"

UGetItem * UGetItem::GetItem(FString ItemID, FString Region, FString Language)
{
	UGetItem* Node = NewObject<UGetItem>();
	Node->Language = Language;
	Node->Region = Region;
	Node->ItemID = ItemID;
	return Node;
}

void UGetItem::Activate()
{
	JusticeCatalog::GetItem(this->ItemID, this->Region, this->Language, FGetItemCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, FItemInfoJustice* Result) {
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
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(ItemResult);
			}
		}
	}));
}