// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteError.h"
#include "AccelByteCategoryModels.h"

namespace AccelByte
{
namespace Api
{

/**
 * @brief The category has tree data structure.
 * Each category has path, for example "/equipments/armor/legs". Items are grouped into a category. The root is equivalent to "/".
 *
 */
class ACCELBYTEUE4SDK_API Category
{
public:
	DECLARE_DELEGATE_OneParam(FGetRootCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief This function gets root categories that exist in the specified namespace.
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetRootCategoriesEasy(FString Language, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetCategorySuccess, const FAccelByteModelsFullCategoryInfo&);
	/**
	 * @brief This function gets the category from a store in the specified namespace.
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param CategoryPath Category path, e.g., "/equipments/pay2win/super_badass_armor/chest".
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsFullCategoryInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetCategory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString CategoryPath, FString Language, FGetCategorySuccess OnSuccess, ErrorDelegate OnError);
	static void GetCategoryEasy(FString CategoryPath, FString Language, FGetCategorySuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetChildCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief This function gets the subcategories from a category in the specified namespace.
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param CategoryPath Category path, e.g., "/equipments/pay2win/super_badass_armor/chest".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetChildCategoriesEasy(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetDescendantCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief Get every subcategories from a category in the specified namespace.
	 *
	 * @param ServerBaseUrl Your server's base URL.
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param CategoryPath Category path, e.g., "/equipments/pay2win/super_badass_armor/chest".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetDescendantCategoriesEasy(FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError);
private:
	Category() = delete; // static class can't have instance
	Category(Category const&) = delete;
	Category(Category&&) = delete;

	static void GetRootCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetCategoryResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetCategorySuccess OnSuccess, ErrorDelegate OnError);
	static void GetChildCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetDescendantCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Api
} // Namespace AccelByte
