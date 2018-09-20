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

/**
 * @brief Justice catalog service.
 * User should log in before using this. See JusticeIdentity to see how to log user in.
 */
class JUSTICESDK_API JusticeCatalog
{
public:
	/**
	 * @brief This function gets root categories that exist in the same namespace with user's namespace.
	 * @param Language Optional.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is TArray<Category> in FCategoryDefaultCompleteDelegate.
	 */
	static void GetRootCategories(FString Language, FCategoryDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function gets the category from a store in a namespace.
	 * @param ParentPath Required.
	 * @param Language Optional.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is TArray<Category> in FCategoryDefaultCompleteDelegate.
	 */
	static void GetCategories(FString ParentPath, FString Language, FCategoryDefaultCompleteDelegate OnComplete);

	/**
	 * @brief This function gets the subcategories from a category.
	 * @param Language Optional.
	 * @param CategoryPath Required. Parent path.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is TArray<Category> in FCategoryDefaultCompleteDelegate.
	 */
	static void GetChildCategories(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete);

	/**
	 * @brief Get every subcategories from a category.
	 * 
	 * @param Language Optional.
	 * @param CategoryPath Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is TArray<Category> in FCategoryDefaultCompleteDelegate.
	 */
	static void GetDescendantCategories(FString Language, FString CategoryPath, FCategoryDefaultCompleteDelegate OnComplete);

	/**
	 * @brief Get an item information from a store within the user's namespace.
	 * @param ItemID Required.
	 * @param Region Required.
	 * @param Language Optional.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is TArray<ItemInfo> in FGetItemCompleteDelegate.
	 */
	static void GetItemById(FString ItemID, FString Region, FString Language, FGetItemCompleteDelegate OnComplete);

	/**
	 * @brief Get an array of items with specific criteria/filter from a store within user's namespace.
	 * @param Language Optional.
	 * @param Region Required.
	 * @param CategoryPath Required.
	 * @param ItemType Required.
	 * @param Status Required.
	 * @param Page Required.
	 * @param Size Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is TArray<ItemInfo> in FGetItemCompleteDelegate.
	 */
	static void GetItemsByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FItemCompleteDelegate OnComplete);

private:
	static void OnGetRootCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetChildCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetDescendantCategoriesResponse(FJusticeResponsePtr Response, FCategoryDefaultCompleteDelegate OnComplete);
	static void OnGetItemByIdResponse(FJusticeResponsePtr Response, FGetItemCompleteDelegate OnComplete);
	static void OnGetItemsByQueryResponse(FJusticeResponsePtr Response, FItemCompleteDelegate OnComplete);
};