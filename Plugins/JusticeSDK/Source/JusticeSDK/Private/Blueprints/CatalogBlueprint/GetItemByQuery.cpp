// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/CatalogBlueprint/GetItemByQuery.h"
#include "Services/JusticeCatalog.h"

UGetItemByQuery * UGetItemByQuery::GetItemByQuery(FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size)
{
	UGetItemByQuery* Node = NewObject<UGetItemByQuery>();
    Node->Language = language;
    Node->Region = region;
    Node->CategoryPath = CategoryPath;
    Node->ItemType = itemType;
    Node->Status = status;
    Node->Page = page;
    Node->Size = size;    
	return Node;
}

void UGetItemByQuery::Activate()
{
	JusticeCatalog::GetItemByQuery(this->Language, this->Region, this->CategoryPath, this->ItemType, this->Status, this->Page, this->Size, FItemCompleteDelegate::CreateLambda([&](bool IsSuccess, FString errorString, TArray<ItemInfo> result) {		
        TArray<UItemInfo*> Result;
        if (IsSuccess)
		{
			for (int i = 0; i < result.Num(); i++)
			{
				UItemInfo* item = UItemInfo::Deserialize(result[i]);				
				check(item);
				Result.Add(item);
			}            
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(Result);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(Result);
			}
		}
	}));
}