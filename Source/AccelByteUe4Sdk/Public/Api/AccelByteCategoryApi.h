// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Category API for buying things from the online store.
 * The category has tree data structure.
 * Each category has path, for example "/equipments/armor/legs". Items are grouped into a category. The root is equivalent to "/".
 */
class ACCELBYTEUE4SDK_API Category
{
public:
	Category(const Credentials& Credentials, const Settings& Settings);
	~Category();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	 * @brief This function gets root categories that exist in the specified namespace.
	 *
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsCategoryInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetRootCategories(const FString& Language, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief This function gets the category from a store in the specified namespace.
	 *
	 * @param CategoryPath Category path, e.g., "/equipments/pay2win/super_badass_armor/chest".
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsCategoryInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetCategory(const FString& CategoryPath, const FString& Language, const THandler<FAccelByteModelsCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief This function gets the subcategories from a category in the specified namespace.
	 *
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param CategoryPath Category path, e.g., "/equipments/pay2win/super_badass_armor/chest".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsCategoryInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetChildCategories(const FString& Language, const FString& CategoryPath, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get every subcategories from a category in the specified namespace.
	 *
	 * @param Language ISO 639-1 language tag, e.g., "en", "zh".
	 * @param CategoryPath Category path, e.g., "/equipments/pay2win/super_badass_armor/chest".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsCategoryInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetDescendantCategories(const FString& Language, const FString& CategoryPath, const THandler<TArray<FAccelByteModelsCategoryInfo>>& OnSuccess, const FErrorHandler& OnError);
private:
	Category() = delete;
	Category(Category const&) = delete;
	Category(Category&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
