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
     * @brief Get all stat items from a specified user id and profile id.
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
     * @param OnError This will be called when the operation failed.
    */
    void GetAllProfileStatItems(const FString& ProfileId, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

    /**
     * @brief Get all stat items from a specified user id and profile id and statCode(s).
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
     * @param OnError This will be called when the operation failed.
    */
    void GetProfileStatItemsByStatCodes(const FString& ProfileId, const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

    /**
     * @brief Get all stat items from a specified user id.
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
     * @param OnError This will be called when the operation failed.
    */
    void GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

    /**
     * @brief Get all stat items from a specified user id and statCode(s).
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
     * @param OnError This will be called when the operation failed.
    */
    void GetUserStatItemsByStatCodes(const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Bulk add stat item(s) value from specified profile id(s) by stat code(s).
	*
	* @param data array consist of increased value, profile id, and stat code.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemIncResult.
	* @param OnError This will be called when the operation failed.
	*/
	void BulkAddStatItemValue(const TArray<FAccelByteModelsBulkUserStatItemInc>& data, const THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Bulk add stat item(s) value from a specified user id and profile id by stat code(s).
	*
	* @param ProfileId the profileId that about to update
	* @param data array consist of increased value and stat code.
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemIncResult.
	* @param OnError This will be called when the operation failed.
	*/
	void BulkAddProfileStatItemValue(const FString& ProfileId, const TArray<FAccelByteModelsBulkStatItemInc>& data, const THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Add a stat item value from a specified user id and profile id by stat code.
	*
	* @param ProfileId the profileId that about to update
	* @param statCode the statCode of statItem that about to update.
	* @param inc increased value of the statItem.
	* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsBulkStatItemIncResult.
	* @param OnError This will be called when the operation failed.
	*/
	void AddProfileStatItemValue(const FString& ProfileId, const FString& statCode, const float& inc, const THandler<FAccelByteModelsStatItemIncResult>& OnSuccess, const FErrorHandler& OnError);

    /**
    * @brief Bulk add stat item(s) value from a specified user id by stat code(s).
    *
    * @param ProfileId the profileId that about to update
    * @param data array consist of increased value and stat code.
    * @param OnSuccess This will be called when the operation succeeded. The result is an array of FAccelByteModelsBulkStatItemIncResult.
    * @param OnError This will be called when the operation failed.
    */
    void BulkAddUserStatItemValue(const TArray<FAccelByteModelsBulkStatItemInc>& data, const THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>& OnSuccess, const FErrorHandler& OnError);

    /**
    * @brief Add a stat item value from a specified user id by stat code.
    *
    * @param ProfileId the profileId that about to update
    * @param statCode the statCode of statItem that about to update.
    * @param inc increased value of the statItem.
    * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsBulkStatItemIncResult.
    * @param OnError This will be called when the operation failed.
    */
    void AddUserStatItemValue(const FString& statCode, const float& inc, const THandler<FAccelByteModelsStatItemIncResult>& OnSuccess, const FErrorHandler& OnError);
	
private:
	Statistic() = delete;
	Statistic(Statistic const&) = delete;
	Statistic(Statistic&&) = delete;

    /**
     * @brief Get all stat items from a specified user id and profile id and/or statCode(s).
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
     * @param OnError This will be called when the operation failed.
    */
    static void GetProfileStatItems(const Statistic& Object, const FString& ProfileId, const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

    /**
     * @brief Get all stat items from a specified user id.
     *
     * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
     * @param OnError This will be called when the operation failed.
    */
    static void GetUserStatItems(const Statistic& Object, const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
};

} // Namespace Api
} // Namespace AccelByte
