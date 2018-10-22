// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
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
 * @brief An item represents a single product sold in the online store. Each category has items inside it. You can get a list of items by criteria or by its ID.
 */
class ACCELBYTEUE4SDK_API Item
{
public:
	DECLARE_DELEGATE_OneParam(FGetItemByIdSuccess, const FAccelByteModelsItemInfo&);
	/**
	 * @brief Get one item information from an online store.
	 *
	 * @param AccessToken Access token.
	 * @param Namespace Target.
	 * @param ItemId The item ID.
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN". 
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsItemInfo&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetItemById(const FString& AccessToken, const FString& Namespace, const FString& ItemId, const FString& Language, const FString& Region, const FGetItemByIdSuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetItemByIdEasy(const FString& ItemId, const FString& Language, const FString& Region, const FGetItemByIdSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetItemsByCriteriaSuccess, const FAccelByteModelsItemPagingSlicedResult&);
	/**
	 * @brief Get an array of items with specific criteria/filter from online store.
	 *
	 * @param AccessToken Access token.
	 * @param Namespace Target.
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
	static void GetItemsByCriteria(const FString& AccessToken, const FString& Namespace, const FString& Language, const FString& Region, const FString& CategoryPath, const FString& ItemType, const FString& Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetItemsByCriteriaEasy(const FString& Language, const FString& Region, const FString& CategoryPath, const FString& ItemType, const FString& Status, int32 Page, int32 Size, const FGetItemsByCriteriaSuccess& OnSuccess, const FErrorHandler& OnError);

private:
	Item() = delete; // static class can't have instance
	Item(Item const&) = delete;
	Item(Item&&) = delete;

	static void GetItemByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetItemByIdSuccess OnSuccess, FErrorHandler OnError);
	static void GetItemsByCriteriaResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetItemsByCriteriaSuccess OnSuccess, FErrorHandler OnError);
};

} // Namespace Api
} // GameId AccelByte
