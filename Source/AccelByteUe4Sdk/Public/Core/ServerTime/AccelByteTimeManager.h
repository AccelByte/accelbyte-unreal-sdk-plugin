// Copyright (c) 2020-2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/Timespan.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteSettings.h"
#include "Models/AccelByteGeneralModels.h"

namespace AccelByte
{

/**
 * @brief Utility class to manage and synchronize Server Time from the Backend services.
 */
class ACCELBYTEUE4SDK_API FAccelByteTimeManager
{
public:
	FAccelByteTimeManager(AccelByte::FHttpRetryScheduler& InHttpRef, FString const& InBasicServerUrl);
	FAccelByteTimeManager(const FString& InBasicServerUrl);
	virtual ~FAccelByteTimeManager();

	/**
	 * @brief Reset any cached Server Time in the Time Manager.
	 */
	virtual void Reset();

	/**
	 * @brief Get current Server Time based on the back calculation from the cached Server Time value,
	 *			or sync with the Backend services if the cached Server Time is out of sync and store it to the cache.
	 * 
	 * @param OnSuccess		Success delegate.
	 * @param OnError		Error delegate
	 * @param bForceSync	Flag to forcefully sync the Server Time with the Backend services.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	virtual FAccelByteTaskWPtr GetServerTime(THandler<FTime> const& OnSuccess
		, FErrorHandler const& OnError
		, bool bForceSync = false);

	/**
	 * @brief Get cached Server Time value, if there is no cached Server Time then return 1970-01-01 00:00:00.
	 * 
	 * @return Cached Server Time in FDateTime
	 */
	virtual FDateTime GetCachedServerTime() const;

	/**
	 * @brief Get current Server Time value based on back calculation from the cached Server Time value, if there
	 *			is no cached Server Time value then return 1970-01-01 00:00:00.
	 * 
	 * @return Current Server Time in FDateTime.
	 */
	virtual FDateTime GetCurrentServerTime();

	/**
	 * @brief Check whether the time manager is in sync with server time or not.
	 * 
	 * @return True if the time manager is in sync with server time. 
	 */
	virtual bool IsInSync();

protected:
	/**
	 * @brief Back calculate current Server Time from the last time when the Server Time is sync'd.
	 * 
	 * @return The timespan from the last time the Server Time is sync'd in FTimespan.
	 */
	virtual FTimespan BackCalculateServerTime();

private:
	bool bUseSharedResources{ false };
	FHttpRetrySchedulerRef HttpRef{};

	/** Critical sections for thread safe operation of ServerTime */
	mutable FCriticalSection ServerTimeLock;

	FDateTime CachedServerTime{ FDateTime::MinValue() };
	FDateTime CurrentServerTime{ FDateTime::MinValue() };
	FDateTime ServerTimeLastUpdated{ FDateTime::MinValue() };
	FThreadSafeCounter ReferenceCount{0};
	
	FAccelByteTaskWPtr AccelByteGetServerTimeTaskWPtr;
	FString BasicServerUrl;
};

using FAccelByteTimeManagerPtr = TSharedPtr<FAccelByteTimeManager, ESPMode::ThreadSafe>;
using FAccelByteTimeManagerWPtr = TWeakPtr<FAccelByteTimeManager, ESPMode::ThreadSafe>;
}