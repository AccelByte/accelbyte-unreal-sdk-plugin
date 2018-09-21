// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Http.h"
#include "AccelByteError.h"
#include "AccelByteModelsCategory.h"

namespace AccelByte
{
namespace Services
{

/**
 * @brief AccelByte catalog service.
 *
 * User should log in before using this. See AccelByteServicesUser to see how to log user in.
 */
class ACCELBYTEUE4SDK_API Category
{
public:
	DECLARE_DELEGATE_OneParam(FGetRootCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief This function gets root categories that exist in the same namespace with user's namespace.
	 *
	 * @param Language Optional.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief This function gets the category from a store in a namespace.
	 *
	 * @param ParentPath Required.
	 * @param Language Optional.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString ParentPath, FString Language, FGetCategoriesSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetChildCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief This function gets the subcategories from a category.
	 *
	 * @param Language Optional.
	 * @param CategoryPath Required. Parent path.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetDescendantCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&);
	/**
	 * @brief Get every subcategories from a category.
	 *
	 * @param Language Optional.
	 * @param CategoryPath Required.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError);

private:
	static void GetRootCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetRootCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetChildCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetChildCategoriesSuccess OnSuccess, ErrorDelegate OnError);
	static void GetDescendantCategoriesResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetDescendantCategoriesSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Services
} // Namespace AccelByte