// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "AccelByteError.h"
#include "Models/AccelByteItemModels.h"

namespace AccelByte
{
namespace Api
{

/**
 * @brief Item API for buying things from the online store. An item represents a single product sold in the online store. Each category has items inside it. You can get a list of items by criteria or by its ID.
 */
class ACCELBYTEUE4SDK_API Item
{
public:
	/**
	 * @brief Get one item information from an online store.
	 *
	 * @param ItemId The item ID.
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN". 
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetItemById(const FString& ItemId, const FString& Language, const FString& Region, const THandler<FAccelByteModelsItemInfo>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Get an array of items with specific criteria/filter from online store.
	 *
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN". 
	 * @param CategoryPath Item's category path.
	 * @param ItemType Type of the item.
	 * @param Status Status of the item.
	 * @param Page Page number.
	 * @param Size Page size.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsItemInfo>&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetItemsByCriteria(const FString& Language, const FString& Region, const FString& CategoryPath, const EAccelByteItemType& ItemType, const EAccelByteItemStatus& Status, int32 Page, int32 Size, const THandler<FAccelByteModelsItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

private:
	Item() = delete; // static class can't have instance
	Item(Item const&) = delete;
	Item(Item&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
