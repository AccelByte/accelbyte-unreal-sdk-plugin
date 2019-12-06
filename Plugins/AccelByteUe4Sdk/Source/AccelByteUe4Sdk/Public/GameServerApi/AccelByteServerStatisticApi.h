// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
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

			void CreateUserStatItem(const FString& UserId, const FString& StatCode, const THandler< FAccelByteModelsUserStatItemInfo>& OnSuccess, const FErrorHandler& OnError);

			void BulkCreateUserStatItems(const FString& UserId, const TArray<FAccelByteModelsBulkStatItemCreate>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Get all stat items from a specified user id.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
			 * @param OnError This will be called when the operation failed.
			*/
			void GetAllUserStatItems(const FString& UserId, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Get all stat items from a specified user id and statCode(s).
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
			 * @param OnError This will be called when the operation failed.
			*/
			void GetUserStatItemsByStatCodes(const FString& UserId, const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Get all stat items from a specified user id and tag(s).
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
			 * @param OnError This will be called when the operation failed.
			*/
			void GetUserStatItemsByTags(const FString& UserId, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);


		private:
			ServerStatistic() = delete;
			ServerStatistic(ServerStatistic const&) = delete;
			ServerStatistic(ServerStatistic&&) = delete;

			/**
			 * @brief Get all stat items from a specified user id.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
			 * @param OnError This will be called when the operation failed.
			*/
			void GetUserStatItems(const FString& UserId, const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
		};

	} // Namespace Api
} // Namespace AccelByte
