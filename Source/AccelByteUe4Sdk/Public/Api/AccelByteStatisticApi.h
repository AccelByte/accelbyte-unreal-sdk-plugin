// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	 */
	void CreateUserStatItems(const TArray<FString>& StatCodes
		, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Get all this user stat items.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
	 * @param OnError This will be called when the operation failed.
	 * @param Limit Page size, default value is 20.
	 * @param Offset Page number, default value is 0.
	 */
	void GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
		, const FErrorHandler& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

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
	 */
	void GetUserStatItems(const TArray<FString>& StatCodes
		, const TArray<FString>& Tags
		, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
		, const FErrorHandler& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

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
	 */
	void GetUserStatItems(const FString& UserId
		, const TArray<FString>& StatCodes
		, const TArray<FString>& Tags
		, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
		, const FErrorHandler& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	/**
	 * @brief Increment this user stat items.
	 *
	 * @param Data array consist of increased value and stat code.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemOperationResult.
	 * @param OnError This will be called when the operation failed.
	 */
	void IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data
		, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
		, const FErrorHandler& OnError);
 
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
	 */	
	void ListUserStatItems(const TArray<FString>& StatCodes
		, const TArray<FString>& Tags
		, const FString& AdditionalKey 
		, const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Public bulk update user's statitems value for given namespace and user with specific update strategy.
	 *
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param BulkUpdateUserStatItems This is the BulkUpdateUserStatItem array that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	 * @param OnError This will be called when the operation failed.
	 */		
	void BulkUpdateUserStatItemsValue(const FString& AdditionalKey
		, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems
		, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Public update user's statitem value for a given namespace and user with a certain update strategy.
	 *
	 * @param StatCode StatCode.
	 * @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	 * @param UpdateUserStatItem This is the UpdateUserStatItem that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 */	
	void UpdateUserStatItemsValue(const FString& StatCode
		, const FString& AdditionalKey
		, const FAccelByteModelsPublicUpdateUserStatItem& UpdateUserStatItem
		, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Public bulk fetch multiple user's statitem value for a given namespace and statCode.
	 *
	 * @param StatCode StatCode.
	 * @param UserIds List of UserIds.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsStatItemValueResponse>.
	 * @param OnError This will be called when the operation failed.
	 */
	void BulkFetchStatItemsValue(const FString StatCode
		, const TArray<FString>& UserIds  
		, const THandler<TArray<FAccelByteModelsStatItemValueResponse>>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Reset user's statitem value for a given namespace and user.
	 * User's statitem value will be reset to the default value defined in the statistic configuration.
	 *
	 * @param StatCode StatCode.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void ResetUserStatItemValue(const FString& StatCode
		, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Get global stat items by specifying statCodes. 
	 * 
	 * @param StatCode The StatCode of the global stat items
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsGlobalStatItemValueResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetGlobalStatItemsByStatCode(const FString& StatCode
		, const THandler<FAccelByteModelsGlobalStatItemValueResponse>& OnSuccess
		, const FErrorHandler& OnError);

#if !UE_BUILD_SHIPPING
	/**
	 * @brief Bulk reset multiple user's statitems value.
	 * User's statitem value will be reset to the default value defined in the statistic configuration.
	 *
	 * @param UserStatItemValue Array of UserId and StatCode.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsUpdateUserStatItemsResponse>.
	 * @param OnError This will be called when the operation failed.
	 */
	void BulkResetMultipleUserStatItemsValue(const TArray<FAccelByteModelsResetUserStatItemValue>& UserStatItemValue
		, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
		, const FErrorHandler& OnError);
#endif // !UE_BUILD_SHIPPING
	
private:
	Statistic() = delete;
	Statistic(Statistic const&) = delete;
	Statistic(Statistic&&) = delete;
};
} // Namespace Api
} // Namespace AccelByte
