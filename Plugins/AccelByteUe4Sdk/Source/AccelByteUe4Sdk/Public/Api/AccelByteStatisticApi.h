// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteStatisticModels.h"

// Forward declarations
class IWebSocket;

namespace AccelByte
{
	class Credentials;
	class Settings;
	namespace Api
	{

		/**
		 * @brief Statistic API to manage user's in-game profiles's statistic.
		 */
		class ACCELBYTEUE4SDK_API Statistic
		{
		public:
			Statistic(const Credentials& Credentials, const Settings& Settings);
			~Statistic();
		private:
			const Credentials& Credentials;
			const Settings& Settings;

public:
	/**
	* @brief Create a stat item.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsBulkStatItemOperationResult.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateUserStatItems(const TArray<FString>& StatCodes, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError);

	/**
		* @brief Get all this user stat items.
		*
		* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
		* @param OnError This will be called when the operation failed.
	*/
	void GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

	/**
		* @brief Get this user stat items by specifying statCodes and tags to get from. Returned stat items will only contain
		* stat items specified by statCodes and tags (inclusive)
		*
		* @param statCodes Specify statCodes for stat items to get from this method
		* @param tags Specify tags for for stat items to get from this method
		* @param OnSuccess This will be called when the operation succeeded. Thre result is an FAccelByteModelsUserStatItemPagingSlicedResult
		* @param OnError This will be called when the operation failed.
	*/
	void GetUserStatItems(const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Increment this user stat items.
	*
	* @param Data array consist of increased value and stat code.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	* @param OnError This will be called when the operation failed.
	*/
	void IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError);

private:
	Statistic() = delete;
	Statistic(Statistic const&) = delete;
	Statistic(Statistic&&) = delete;
};

	} // Namespace Api
} // Namespace AccelByte
