// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
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
class ACCELBYTEUE4SDK_API Statistic
{
public:
	Statistic(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~Statistic();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

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
		* @param StatCodes Specify statCodes for stat items to get from this method
		* @param Tags Specify tags for for stat items to get from this method
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
	void ListUserStatItems(const TArray<FString>& StatCodes,  const TArray<FString>& Tags,  const FString& AdditionalKey, 
		const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Public bulk update user's statitems value for given namespace and user with specific update strategy.
	*
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param BulkUpdateUserStatItems This is the BulkUpdateUserStatItem array that will be stored in the slot.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsUpdateUserStatItemsResponse.
	* @param OnError This will be called when the operation failed.
	*/		
	void BulkUpdateUserStatItemsValue(const FString& AdditionalKey, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems,
		const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Public update user's statitem value for a given namespace and user with a certain update strategy.
	*
	* @param StatCode StatCode.
	* @param AdditionalKey This is the AdditionalKey that will be stored in the slot.
	* @param UpdateUserStatItem This is the UpdateUserStatItem that will be stored in the slot.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUpdateUserStatItemValueResponse.
	* @param OnError This will be called when the operation failed.
	*/	
	void UpdateUserStatItemsValue(const FString& StatCode, const FString& AdditionalKey, const FAccelByteModelsPublicUpdateUserStatItem& UpdateUserStatItem,
		const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess, const FErrorHandler& OnError);

private:
	Statistic() = delete;
	Statistic(Statistic const&) = delete;
	Statistic(Statistic&&) = delete;
};
} // Namespace Api
} // Namespace AccelByte
