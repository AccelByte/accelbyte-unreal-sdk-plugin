// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteStatisticModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{

/**
 * @brief Statistic API to manage user's in-game profiles's statistic.
 */
class ACCELBYTEUE4SDK_API Statistic : public FApiBase
{
public:
	Statistic(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Statistic();

	/**
	 * @brief Create a stat item.
	 *
	 * @param StatCodes Specify statCodes for stat items to get from this method
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsBulkStatItemOperationResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateUserStatItems(TArray<FString> const& StatCodes
		, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get all this user stat items.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 * @param SortBy The container to store sortby.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAllUserStatItems(THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::UPDATED_AT_ASC );

	/**
	 * @brief Get this user stat items by specifying statCodes and tags to get from. Returned stat items will only contain
	 * stat items specified by statCodes and tags (inclusive)
	 *
	 * @param StatCodes Specify statCodes for stat items to get from this method
	 * @param Tags Specify tags for for stat items to get from this method
	 * @param OnSuccess This will be called when the operation succeeded. Thre result is an FAccelByteModelsUserStatItemPagingSlicedResult
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 * @param SortBy The container to store sortby.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserStatItems(TArray<FString> const& StatCodes
		, TArray<FString> const& Tags
		, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::UPDATED_AT_ASC );

	/**
	 * @brief Get user's specified stat items and specifying statCodes and tags to get from. Returned stat items will only contain
	 * stat items specified by statCodes and tags (inclusive)
	 *
	 * @param UserId The Id of a user that will be given his stat items 
	 * @param StatCodes Specify statCodes for stat items to get from this method
	 * @param Tags Specify tags for for stat items to get from this method
	 * @param OnSuccess This will be called when the operation succeeded. Thre result is an FAccelByteModelsUserStatItemPagingSlicedResult
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
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
	 * @brief Increment this user stat items.
	 *
	 * @param Data array consist of increased value and stat code.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr IncrementUserStatItems(TArray<FAccelByteModelsBulkStatItemInc> const& Data
		, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
		, FErrorHandler const& OnError);
 
	/**
	 * @brief Public list all statItems of user.
	 *	NOTE:
	 *		If stat code does not exist, will ignore this stat code.
	 *		If stat item does not exist, will return default value
	 *
	 * @param StatCodes StatCodes.
	 * @param Tags This is the Tag array that will be stored in the slot.
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsFetchUser.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ListUserStatItems(TArray<FString> const& StatCodes
		, TArray<FString> const& Tags
		, FString const& AdditionalKey 
		, THandler<TArray<FAccelByteModelsFetchUser>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Public bulk update user's statitems value for given namespace and user with specific update strategy.
	 *
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param BulkUpdateUserStatItems This is the BulkUpdateUserStatItem array that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkUpdateUserStatItemsValue(FString const& AdditionalKey
		, TArray<FAccelByteModelsUpdateUserStatItemWithStatCode> const& BulkUpdateUserStatItems
		, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Public update user's statitem value for a given namespace and user with a certain update strategy.
	 *
	 * @param StatCode StatCode.
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param UpdateUserStatItem This is the UpdateUserStatItem that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserStatItemsValue(FString const& StatCode
		, FString const& AdditionalKey
		, FAccelByteModelsPublicUpdateUserStatItem const& UpdateUserStatItem
		, THandler<FAccelByteModelsUpdateUserStatItemValueResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Public bulk fetch multiple user's statitem value for a given namespace and statCode.
	 *
	 * @param StatCode StatCode.
	 * @param UserIds List of UserIds.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsStatItemValueResponse>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkFetchStatItemsValue(FString const& StatCode
		, TArray<FString> const& UserIds  
		, THandler<TArray<FAccelByteModelsStatItemValueResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Reset user's statitem value for a given namespace and user.
	 * User's statitem value will be reset to the default value defined in the statistic configuration.
	 *
	 * @param StatCode StatCode.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ResetUserStatItemValue(FString const& StatCode
		, THandler<FAccelByteModelsUpdateUserStatItemValueResponse> const& OnSuccess
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
	 * @brief Get user stat items with cycle
	 *
	 * @param CycleId the CycleId where the stat items belong to
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserStatCycleItemPagingSlicedResult
	 * @param OnError This will be called when the operation failed
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 * @param StatCodes Optional. the StatCode of the user stat item. Determine which stat code to get with the cycle id.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserStatCycleItems(FString const& CycleId
		, THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0
		, TArray<FString> const& StatCodes = {});

	/**
	 * @brief Get Statistic Cycle Config
	 * 
	 * @param CycleId The CycleId of the config
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsStatCycleConfig
	 * @param OnError This will be called when the operation failed
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetStatCycleConfig(FString const& CycleId
		, THandler<FAccelByteModelsStatCycleConfig> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get List of Statistic Cycle Config
	 * 
	 * @param CycleType 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsStatCycleConfig
	 * @param OnError This will be called when the operation failed
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetListStatCycleConfigs(EAccelByteCycle CycleType
		, THandler<FAccelByteModelsStatCycleConfigPagingResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Get user's own statistic item
	 * 
	 * @param StatCodes Array of statistic codes to be retrieved
	 * @param Tags Array of tags which the statistic items to be retrieved have
	 * @param OnSuccess Thil will be called when the operation succeeded. The result is FAccelByteModelsUserStatItemPagingSlicedResult
	 * @param OnError This will be called when the operation failed
	 * @param SortBy 
	 * @param Limit Page size, default value is 20
	 * @param Offset Page number, default value is 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMyStatItems(TArray<FString> const& StatCodes
		, TArray<FString> const& Tags
		, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::NONE
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Get user's own statistic value
	 * 
	 * @param StatCodes Array of statistic codes for statistic value to be retrieved
	 * @param Tags Array of tags that statistic item to be retrieved has
	 * @param AdditionalKey Additional key 
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsFetchUser>
	 * @param OnError This will be called when the operation failed
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetMyStatItemValues(TArray<FString> const& StatCodes
		, TArray<FString> const& Tags
		, FString const& AdditionalKey
		, THandler<TArray<FAccelByteModelsFetchUser>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get user's own statistic cycle item
	 * 
	 * @param CycleId The cycle id to which the stat item belong
	 * @param StatCodes Array of statistic codes for statistic cycle item to be retrieved 
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserStatCycleItemPagingSlicedResult
	 * @param OnError This will be called when the operation failed
	 * @param SortBy 
	 * @param Limit Page size, default value is 20
	 * @param Offset Page number, default value is 0
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	 FAccelByteTaskWPtr GetMyStatCycleItems(FString const& CycleId
		, TArray<FString> const& StatCodes
		, THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::NONE
		, int32 Limit = 20
		, int32 Offset = 0);

#if !UE_BUILD_SHIPPING
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
#endif // !UE_BUILD_SHIPPING
	
private:
	Statistic() = delete;
	Statistic(Statistic const&) = delete;
	Statistic(Statistic&&) = delete;
	
	static FString ConvertUserStatisticSortByToString(EAccelByteStatisticSortBy SortBy);
};
} // Namespace Api
} // Namespace AccelByte
