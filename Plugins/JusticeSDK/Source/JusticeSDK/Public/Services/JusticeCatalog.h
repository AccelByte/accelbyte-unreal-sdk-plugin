// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Misc/ConfigCacheIni.h"
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
DECLARE_DELEGATE_ThreeParams(FItemIDCompleteDelegate, bool, FString, FItemInfoJustice*);

class JUSTICESDK_API JusticeCatalog
{
public:
	static void GetRootCategories(FString Language, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetCategories(FString ParentPath, FString Language, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetChildCategories(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetDescendantCategories(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete);
	static void GetItemById(FString ItemID, FString Region, FString Language, FGetItemCompleteDelegate OnComplete);
	static void GetItemsByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FItemCompleteDelegate OnComplete);

private:
	static void OnGetRootCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetChildCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetDescendantCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetItemByIdResponse(FJusticeResponsePtr Response, FGetItemCompleteDelegate OnComplete);
	static void OnGetItemsByQueryResponse(FJusticeResponsePtr Response, FItemCompleteDelegate OnComplete);
};