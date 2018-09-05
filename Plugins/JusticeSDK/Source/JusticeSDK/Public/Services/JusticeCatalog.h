// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Misc/ConfigCacheIni.h"
#include "JusticePlatformFunctions.h"
#include "JusticeSDK.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/JusticeLog.h"
#include "Models/Category.h"
#include "Models/ItemInfo.h"
#include "Models/ItemPagingSlicedResult.h"

DECLARE_DELEGATE_ThreeParams(FCategoryDefaultCompleteDelegate, bool, FString, TArray<FCategory>);
DECLARE_DELEGATE_ThreeParams(FGetItemCompleteDelegate, bool, FString, FItemInfoJustice*);
DECLARE_DELEGATE_ThreeParams(FItemCompleteDelegate, bool, FString, TArray<FItemInfoJustice>);

class JUSTICESDK_API JusticeCatalog
{
public:
	static void GetRootCategory(FString Language, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetCategory(FString ParentPath, FString Language, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetChildCategory(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetDescendantCategory(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetItem(FString ItemID, FString Region, FString Language, FGetItemCompleteDelegate OnComplete);
	static void GetItemByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FItemCompleteDelegate OnComplete);

private:
	static void OnGetRootCategoryResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetCategoryResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetChildCategoryResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetDescendantCategoryResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetItemResponse(FJusticeResponsePtr Response, FGetItemCompleteDelegate OnComplete);
	static void OnGetItemByQueryResponse(FJusticeResponsePtr Response, FItemCompleteDelegate OnComplete);
};