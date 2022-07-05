// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
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
	Item(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Item();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
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
	void GetItemById(FString const& ItemId, FString const& Language, FString const& Region, THandler<FAccelByteModelsPopulatedItemInfo> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get one item information from an online store.
	 *
	 * @param AppId The App ID.
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPopulatedItemInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	 void GetItemByAppId(FString const& AppId, FString const& Language, FString const& Region, THandler<FAccelByteModelsItemInfo> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get an array of items with specific criteria/filter from online store.
	 *
	 * @param ItemCriteria should be contain some parameters for query.
	 * @param Offset Page number.
	 * @param Limit Page size.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemPagingSlicedResult&.
	 * @param OnError This will be called when the operation failed.
	 * @param SortBy Make sure to always use more than one sort if the first sort is not an unique value for example, if you wish to sort by displayOrder,
	 * make sure to include other sort such as name or createdAt after the first sort, eg: displayOrder:asc,name:asc
	 * if it leave with empty array, it will be set to default value : name:asc,displayOrder:asc
	 */
	void GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria, int32 const& Offset, int32 const& Limit,
		THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError, TArray<EAccelByteItemListSortBy> SortBy = { });

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
	void SearchItem(FString const& Language, FString const& Keyword, int32 const& Offset, int32 const& Limit, FString const& Region, THandler<FAccelByteModelsItemPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError);

	/**
	* @brief Get Item information by SKU number from an online store.
	*
	* @param Sku should contain specific number of item Sku.
	* @param Language ISO 639-1 language tag, e.g., "en, "zh".
	* @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN".
	* @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemInfo&.
	* @param OnError This will be called when the operation failed.
	*/
	void GetItemBySku(FString const& Sku, FString const& Language,FString const& Region, THandler<FAccelByteModelsItemInfo> const& OnSuccess, FErrorHandler const& OnError);
	
private:
	Item() = delete;
	Item(Item const&) = delete;
	Item(Item&&) = delete;

	FString ConvertItemSortByToString(EAccelByteItemListSortBy const& SortBy);
};

} // Namespace Api
} // Namespace AccelByte
