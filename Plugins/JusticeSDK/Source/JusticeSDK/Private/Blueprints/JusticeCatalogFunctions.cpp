// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalogFunctions.h"
#include "JusticeCatalog.h"

void UJusticeCatalogFunctions::GetItemByQuery(FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size, FItemCompleteDynamicDelegate OnComplete)
{
	JusticeCatalog::GetItemByQuery(language, region, CategoryPath, itemType, status, page, size, FItemCompleteDelegate::CreateLambda([OnComplete](bool isSuccess, FString errorString, TArray<ItemInfo> result) {
		TArray<UItemInfo*> Result;
		if (isSuccess)
		{
			for (int i = 0; i < result.Num(); i++)
			{
				UItemInfo* item = NewObject<UItemInfo>();
				item->FromItemInfo(result[i]);
				Result.Add(item);
			}
		}
		OnComplete.ExecuteIfBound(isSuccess, errorString, Result);
	}));
}