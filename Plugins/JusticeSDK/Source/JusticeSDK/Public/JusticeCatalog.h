// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Private/Models/Category.h"
#include "Private/Models/ItemInfo.h"
#include "Private/Models/ItemPagingSlicedResult.h"

DECLARE_DELEGATE_ThreeParams(FGetRootCategoryCompleteDelegate, bool, FString, TArray<Category>);
DECLARE_DELEGATE_ThreeParams(FItemCompleteDelegate, bool, FString, TArray<ItemInfo>);

class JUSTICESDK_API JusticeCatalog
{

public:
	static void GetRootCategory(FGetRootCategoryCompleteDelegate OnComplete);
	static void GetSubCategory(FString ParentPath, FGetRootCategoryCompleteDelegate OnComplete);
	static void GetItemByCriteria(FString CategoryPath, FItemCompleteDelegate OnComplete);


private:
	static void OnGetRootCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete);	
	static void OnGetItemByCriteriaComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FItemCompleteDelegate OnComplete);
};

