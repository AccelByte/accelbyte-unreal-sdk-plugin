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

DECLARE_DELEGATE_ThreeParams(FGetRootCategoryCompleteDelegate, bool, FString, TArray<Category>);
DECLARE_DELEGATE_ThreeParams(FItemCompleteDelegate, bool, FString, TArray<ItemInfo>);

class JUSTICESDK_API JusticeCatalog
{
public:
	static void GetRootCategory(FString Language, FGetRootCategoryCompleteDelegate OnComplete);
	static void GetCategory(FString ParentPath, FString Language, FGetRootCategoryCompleteDelegate OnComplete);
	static void GetItemByQuery(FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size, FItemCompleteDelegate OnComplete);

private:
	static void OnGetRootCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete, FString Language);
	static void OnGetCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete, FString ParentPath, FString Language);
	static void OnGetItemByQueryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FItemCompleteDelegate OnComplete, FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size);
};