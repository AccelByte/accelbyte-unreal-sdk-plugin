// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Http.h"
#include "AccelByteError.h"
#include "Models/AccelByteModelsItem.h"

namespace AccelByte
{
namespace Services
{

/**
 * @brief AccelByte item service.
 *
 * User should log in before using this. See AccelByteServicesUser to see how to log user in.
 */
class ACCELBYTEUE4SDK_API Item
{
public:
	DECLARE_DELEGATE_OneParam(FGetItemByIdSuccess, const FAccelByteModelsItemInfo&);
	/**
	 * @brief Get an item information from a store within the user's namespace.
	 *
	 * @param ItemId Required.
	 * @param Region Required.
	 * @param Language Optional.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const FAccelByteModelsItemInfo&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetItemById(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString ItemId, FString Region, FString Language, FGetItemByIdSuccess OnSuccess, ErrorDelegate OnError);

	DECLARE_DELEGATE_OneParam(FGetItemsByCriteriaSuccess, const TArray<FAccelByteModelsItemInfo>&);
	/**
	 * @brief Get an array of items with specific criteria/filter from a store within user's namespace.
	 *
	 * @param Language Optional.
	 * @param Region Required.
	 * @param CategoryPath Required.
	 * @param ItemType Required.
	 * @param Status Required.
	 * @param Page Required.
	 * @param Size Required.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<ItemInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetItemsByCriteria(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FGetItemsByCriteriaSuccess OnSuccess, ErrorDelegate OnError);

private:
	static void GetItemByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetItemByIdSuccess OnSuccess, ErrorDelegate OnError);
	static void GetItemsByCriteriaResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetItemsByCriteriaSuccess OnSuccess, ErrorDelegate OnError);
};

} // Namespace Services
} // Namespace AccelByte
