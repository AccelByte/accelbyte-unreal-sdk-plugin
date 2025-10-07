// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteStatisticModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

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
class ACCELBYTEUE4SDK_API ServerStatistic : public FServerApiBase
{
public:
	ServerStatistic(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerStatistic();

	/**
	 * @brief Create stat items for a user.
	 *
	 * @param UserId
	 * @param StatCodes
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateUserStatItems(FString const& UserId
		, TArray<FString> const& StatCodes
		, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get all stat items from a specified user id.
	 *
	 * @param UserId
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 * @param SortBy Sorting method for the statistic result.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAllUserStatItems(FString const& UserId
		, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::UPDATED_AT_ASC);

	/**
	 * @brief Get a user stat items by specifying statCodes and tags to get from. Returned stat items will only contain
	 * stat items specified by statCodes and tags (inclusive)
	 *
	 * @param UserId
	 * @param StatCodes Specify statCodes for stat items to get from this method
	 * @param Tags Specify tags for for stat items to get from this method
	 * @param OnSuccess Specify tags for for stat items to get from this method
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 * @param SortBy Sorting method for the statistic result.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserStatItems(FString const& UserId
		, TArray<FString> const& StatCodes
		, TArray<FString> const& Tags
		, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::UPDATED_AT_ASC );

	/**
	 * @brief Get a user stat items within the cycle
	 *
	 * @param CycleId the CycleId where the stat items belong to
	 * @param TargetUserId the user Id to be queried
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserStatCycleItemPagingSlicedResult
	 * @param OnError This will be called when the operation failed
	 * @param StatCodes Optional. the StatCode of the user stat item. Determine which stat code to get with the cycle id.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserStatCycleItems(FString const& CycleId
		, FString const& TargetUserId
		, THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, TArray<FString> const& StatCodes = {}
		, int32 Limit = 20
		, int32 Offset = 0
	);

	/**
	 * @brief Bulk add stat item(s) value from specified user id(s) by stat code(s).
	 *
	 * @param Data array consist of increased value, user id, and stat code.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr IncrementManyUsersStatItems(TArray<FAccelByteModelsBulkUserStatItemInc> const& Data
		, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Increment stat items of a user (could be negative)
	 *
	 * @param UserId
	 * @param Data array consist of increased value and stat code.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr IncrementUserStatItems(FString const& UserId
		, TArray<FAccelByteModelsBulkStatItemInc> const& Data
		, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk fetch multiple user's stat item values for a given namespace and statCode.
	 * If stat item does not exist, will return default value.
	 *
	 * @param StatCode This is the StatCode that will be stored in the slot.
	 * @param UserIds This is the UserId array that will be stored in the slot.
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsFetchUser.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkFetchUserStatItemValues(FString const& StatCode
		, TArray<FString> const& UserIds
		, FString const& AdditionalKey
		, THandler<TArray<FAccelByteModelsFetchUser>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk fetch multiple user's stat item values for a given namespace and statCode.
	 *
	 * @param StatCode This is the StatCode that will be stored in the slot.
	 * @param UserIds This is the UserId array that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkFetchStatItemsValue(FString const& StatCode
		, TArray<FString> const& UserIds
		, THandler<TArray<FAccelByteModelsStatItemValueResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk update multiple user's statitems value with specific update strategy.
	 *
	 * @param BulkUpdateMultipleUserStatItems This is the BulkUpdateMultipleUserStatItem array that will be stored in the slot. 
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkUpdateMultipleUserStatItemsValue(TArray<FAccelByteModelsUpdateUserStatItem> const& BulkUpdateMultipleUserStatItems
		, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk reset user's statitem values for given namespace and user.
	 *
	 * @param UserId This is the UserId that will be stored in the slot.
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param BulkUserStatItems This is the BulkUserStatItem array that will be stored in the slot. 
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkResetUserStatItemsValues(FString const& UserId
		, FString const& AdditionalKey
		, TArray<FAccelByteModelsUserStatItem> const& BulkUserStatItems
		, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk reset multiple user's statitems value.
	 * User's statitem value will be reset to the default value defined in the statistic configuration.
	 *
	 * @param UserStatItemValue Array of UserId and StatCode.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsUpdateUserStatItemsResponse>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkResetMultipleUserStatItemsValue(TArray<FAccelByteModelsResetUserStatItemValue> const& UserStatItemValue
		, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	* @brief Bulk update user's statitems value for given namespace and user with specific update strategy.
	*
	* @param UserId UserId.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param BulkUpdateUserStatItems This is the BulkUpdateUserStatItem array that will be stored in the slot.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	* @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkUpdateUserStatItemValue(FString const& UserId
		, FString const& AdditionalKey
		, TArray<FAccelByteModelsUpdateUserStatItemWithStatCode> const& BulkUpdateUserStatItems
		, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update user's statitem value for a given namespace and user with a certain update strategy.
	 *
	 * @param UserId UserId.
	 * @param StatCode StatCode.
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param UpdateUserStatItemValue This is the FAccelByteModelsUpdateUserStatItem that will be stored in the slot. 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserStatItemValue(FString const& UserId
		, FString const& StatCode
		, FString const& AdditionalKey
		, FAccelByteModelsUpdateUserStatItem const& UpdateUserStatItemValue
		, THandler<FAccelByteModelsUpdateUserStatItemValueResponse> const& OnSuccess
		, FErrorHandler const& OnError);

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
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteUserStatItems(FString const& UserId
		, FString const& StatCode
		, FString const& AdditionalKey
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get global stat items by specifying statCodes.
	 *
	 * @param StatCode The StatCode of the global stat items
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsGlobalStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGlobalStatItemsByStatCode(FString const& StatCode
		, THandler<FAccelByteModelsGlobalStatItemValueResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief List global stat items. 
	 * 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsGlobalStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ListGlobalStatItems(THandler<FAccelByteModelsGlobalStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<FString> const& StatCodes = {}
	, int32 Limit = 20
	, int32 Offset = 0);

private:
	ServerStatistic() = delete;
	ServerStatistic(ServerStatistic const&) = delete;
	ServerStatistic(ServerStatistic&&) = delete;

	static FString ConvertUserStatisticSortByToString(const EAccelByteStatisticSortBy& SortBy);
};

} // Namespace GameServerApi
} // Namespace AccelByte
