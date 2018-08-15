// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/CatalogBlueprint/GetItemByQuery.h"
#include "Services/JusticeCatalog.h"

UGetItemByQuery * UGetItemByQuery::GetItemByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size)
{
	UGetItemByQuery* Node = NewObject<UGetItemByQuery>();
	Node->Language = Language;
	Node->Region = Region;
	Node->CategoryPath = CategoryPath;
	Node->ItemType = ItemType;
	Node->Status = Status;
	Node->Page = Page;
	Node->Size = Size;    
	return Node;
}

void UGetItemByQuery::Activate()
{
	JusticeCatalog::GetItemByQuery(this->Language, this->Region, this->CategoryPath, this->ItemType, this->Status, this->Page, this->Size, FItemCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FItemInfoJustice> Result) {		
        TArray<UItemInfo*> FinalResult;
        if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UItemInfo* item = UItemInfo::Deserialize(Result[i]);				
				check(item);
				FinalResult.Add(item);
			}            
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(FinalResult);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(FinalResult);
			}
		}
	}));
}