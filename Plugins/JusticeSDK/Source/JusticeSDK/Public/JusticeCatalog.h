// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Private/Models/Category.h"
#include "Private/Models/JusticeItem.h"


DECLARE_DELEGATE_ThreeParams(FGetRootCategoryCompleteDelegate, bool, FString, TArray<Category>);
DECLARE_DELEGATE_ThreeParams(FItemCompleteDelegate, bool, FString, TArray<JusticeItem>);


class JUSTICESDK_API JusticeCatalog
{

public:
	static void GetRootCategory(FGetRootCategoryCompleteDelegate OnComplete);
	static void GetSubCategory(FString ParentPath, FGetRootCategoryCompleteDelegate OnComplete);
	static void GetItemByCategory(FString CategoryPath, FItemCompleteDelegate OnComplete);


private:
	static void OnGetRootCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete);
	static void OnGetSubCategoryComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetRootCategoryCompleteDelegate OnComplete);
};

