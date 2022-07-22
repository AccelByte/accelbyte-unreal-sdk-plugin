// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteStatisticModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"

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
			ServerStatistic(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
			~ServerStatistic();
		private:
			ServerCredentials const& CredentialsRef;
			ServerSettings const& SettingsRef;
			FHttpRetryScheduler& HttpRef;

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

	/**
	* @brief Bulk fetch multiple user's stat item values for a given namespace and statCode.
	* If stat item does not exist, will return default value.
	*
	* @param StatCode This is the StatCode that will be stored in the slot.
	* @param UserIds This is the UserId array that will be stored in the slot.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsFetchUser.
	* @param OnError This will be called when the operation failed.
	*/
	void BulkFetchUserStatItemValues(const FString& StatCode, const TArray<FString>& UserIds, const FString& AdditionalKey,
		const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Bulk update multiple user's statitems value with specific update strategy.
	*
	* @param BulkUpdateMultipleUserStatItems This is the BulkUpdateMultipleUserStatItem array that will be stored in the slot. 
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	* @param OnError This will be called when the operation failed.
	*/ 
	void BulkUpdateMultipleUserStatItemsValue(const TArray<FAccelByteModelsUpdateUserStatItem>& BulkUpdateMultipleUserStatItems,
		const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Bulk reset user's statitem values for given namespace and user.
	*
	* @param UserId This is the UserId that will be stored in the slot.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param BulkUserStatItems This is the BulkUserStatItem array that will be stored in the slot. 
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	* @param OnError This will be called when the operation failed.
	*/ 
	void BulkResetUserStatItemsValues(const FString& UserId, const FString& AdditionalKey, const TArray<FAccelByteModelsUserStatItem>& BulkUserStatItems,
		const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Bulk update user's statitems value for given namespace and user with specific update strategy.
	*
	* @param UserId UserId.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param BulkUpdateUserStatItems This is the BulkUpdateUserStatItem array that will be stored in the slot.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	* @param OnError This will be called when the operation failed.
	*/ 
	void BulkUpdateUserStatItemValue(const FString& UserId, const FString& AdditionalKey, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems,
		const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Update user's statitem value for a given namespace and user with a certain update strategy.
	*
	* @param UserId UserId.
	* @param StatCode StatCode.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param UpdateUserStatItemValue This is the FAccelByteModelsUpdateUserStatItem that will be stored in the slot. 
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	* @param OnError This will be called when the operation failed.
	*/ 			
	void UpdateUserStatItemValue(const FString& UserId,  const FString& StatCode, const FString& AdditionalKey, const FAccelByteModelsUpdateUserStatItem& UpdateUserStatItemValue,
		const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Delete user's stat items for given namespace, statCode, and user Id.
	*	If query param additionalKey is provided, it will delete user stat items of specific key (i.e. characterName).
	*	Otherwise, it will delete all stat items related to the user Id..
	*
	* @param UserId This is the UserId that will be stored in the slot.
	* @param StatCode This is the StatCode that will be stored in the slot.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param OnSuccess This will be called when the operation succeeded.  
	* @param OnError This will be called when the operation failed.
	*/ 				
	void DeleteUserStatItems(const FString& UserId, const FString& StatCode, const FString& AdditionalKey,
		const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
private:
	ServerStatistic() = delete;
	ServerStatistic(ServerStatistic const&) = delete;
	ServerStatistic(ServerStatistic&&) = delete;
};

	} // Namespace Api
} // Namespace AccelByte
