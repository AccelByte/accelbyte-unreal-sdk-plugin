// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteError.h"
#include "AccelByteStatisticModels.h"

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
	 * @brief Get all stat items from the specified user id and profile id.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemPagingSlicedResult.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetAllStatItems(const FString& ProfileId, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Get stat item(s) from the specified user id and profile id by the stat code(s).
	*
	* @param statCodes stat codes.
	* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsUserStatItemInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void GetStatItemsByStatCodes(const FString& ProfileId, TArray<FString> StatCodes, const THandler<TArray<FAccelByteModelsUserStatItemInfo>>& OnSUccess, const FErrorHandler& OnError);
	
private:
	Statistic() = delete;
	Statistic(Statistic const&) = delete;
	Statistic(Statistic&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
