// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeCatalogFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "JusticeCatalog.h"


//void UJusticeCatalogFunctions::GetRootCategory(FGetRootCategoryCompleteDelegate OnComplete)
//{
//}

void UJusticeCatalogFunctions::GetItemByCriteria(FString CategoryPath, FItemCompleteDynamicDelegate OnComplete)
{
	JusticeCatalog::GetItemByCriteria(CategoryPath, FItemCompleteDelegate::CreateLambda([OnComplete](bool isSuccess, FString errorString, TArray<ItemInfo> result) {
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
		OnComplete.Execute(isSuccess, errorString, Result);


	}));
}
