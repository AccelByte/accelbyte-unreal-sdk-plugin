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
 * @brief Item API for buying things from the online store. An item represents a single product sold in the online store. Each category has items inside it. You can get a list of items by criteria or by its ID.
 */
class ACCELBYTEUE4SDK_API Item
{
public:
	Item(const Credentials& Credentials, const Settings& Settings);
	~Item();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	 * @brief Get one item information from an online store.
	 *
	 * @param ItemId The item ID.
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN". 
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPopulatedItemInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetItemById(const FString& ItemId, const FString& Language, const FString& Region, const THandler<FAccelByteModelsPopulatedItemInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get one item information from an online store.
	 *
	 * @param AppId The App ID.
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPopulatedItemInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	 void GetItemByAppId(const FString & AppId, const FString & Language, const FString & Region, const THandler<FAccelByteModelsItemInfo>& OnSuccess, const FErrorHandler & OnError);

	/**
	 * @brief Get an array of items with specific criteria/filter from online store.
	 *
	 * @param ItemCriteria should be contain some parameters for query.
	 * @param Offset Page number.
	 * @param Limit Page size.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemPagingSlicedResult&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetItemsByCriteria(const FAccelByteModelsItemCriteria& ItemCriteria, const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Search items by keyword in title, description and long description from published store. Language constrained. If item does not exist in the specified region, default region item will be returned.
	 * 
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Keyword Item's keyword in title or description or long description.
	 * @param Offset Offset item.
	 * @param Limit Limit item.
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemPagingSlicedResult&.
	 * @param OnError This will be called when the operation failed.
	 */
	void SearchItem(const FString& Language, const FString& Keyword, const int32& Offset, const int32& Limit, const FString& Region, const THandler<FAccelByteModelsItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

private:
	Item() = delete;
	Item(Item const&) = delete;
	Item(Item&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
