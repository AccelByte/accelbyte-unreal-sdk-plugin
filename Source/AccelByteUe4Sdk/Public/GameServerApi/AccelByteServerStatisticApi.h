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
	class ServerCredentials;
	class ServerSettings;
	namespace GameServerApi
	{

		/**
		 * @brief Statistic API to manage user's in-game profiles's statistic.
		 */
		class ACCELBYTEUE4SDK_API ServerStatistic
		{
		public:
			ServerStatistic(const ServerCredentials& Credentials, const ServerSettings& Settings);
			~ServerStatistic();
		private:
			const ServerCredentials& Credentials;
			const ServerSettings& Settings;

		public:

public:
	
	/**
		* @brief Create stat items for a user.
		*
		* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
		* @param OnError This will be called when the operation failed.
	*/
	void CreateUserStatItems(const FString& UserId, const TArray<FString>& StatCodes, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError);

	/**
		* @brief Get all stat items from a specified user id.
		*
		* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
		* @param OnError This will be called when the operation failed.
	*/
	void GetAllUserStatItems(const FString& UserId, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

	/**
		* @brief Get a user stat items by specifying statCodes and tags to get from. Returned stat items will only contain
		* stat items specified by statCodes and tags (inclusive)
		*
		* @param UserId
		* @param StatCodes Specify statCodes for stat items to get from this method
		* @param Tags Specify tags for for stat items to get from this method
		* @param OnSuccess Specify tags for for stat items to get from this method
		* @param OnError This will be called when the operation failed.
	*/
	void GetUserStatItems(const FString& UserId, const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);


	/**
	* @brief Bulk add stat item(s) value from specified user id(s) by stat code(s).
	*
	* @param Data array consist of increased value, user id, and stat code.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	* @param OnError This will be called when the operation failed.
	*/
	void IncrementManyUsersStatItems(const TArray<FAccelByteModelsBulkUserStatItemInc>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Increment stat items of a user (could be negative)
	*
	* @param Data array consist of increased value and stat code.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	* @param OnError This will be called when the operation failed.
	*/
	void IncrementUserStatItems(const FString& UserId, const TArray<FAccelByteModelsBulkStatItemInc>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError);

private:
	ServerStatistic() = delete;
	ServerStatistic(ServerStatistic const&) = delete;
	ServerStatistic(ServerStatistic&&) = delete;
};

	} // Namespace Api
} // Namespace AccelByte
