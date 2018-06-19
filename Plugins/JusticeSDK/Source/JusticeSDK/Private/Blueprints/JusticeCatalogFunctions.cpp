// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalogFunctions.h"
#include "JusticeCatalog.h"

void UJusticeCatalogFunctions::GetItemByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FItemCompleteDynamicDelegate OnComplete)
{
	JusticeCatalog::GetItemByQuery(Language, Region, CategoryPath, ItemType, Status, Page, Size, FItemCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorString, TArray<ItemInfo> Result) {
		TArray<UItemInfo*> FinalResult;
		if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UItemInfo* Item = UItemInfo::Deserialize(Result[i]);
				check(Item);
				FinalResult.Add(Item);
			}
		}
		OnComplete.ExecuteIfBound(bSuccessful, ErrorString, FinalResult);
	}));
}