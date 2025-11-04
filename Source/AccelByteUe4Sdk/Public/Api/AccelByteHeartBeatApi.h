// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"

namespace AccelByte
{
namespace Api
{

class ACCELBYTEUE4SDK_API HeartBeat 
	: public FApiBase
{
public:
	DECLARE_DELEGATE(FHeartBeatResponse);

	HeartBeat(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	HeartBeat(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~HeartBeat();

	/**
	* @brief Startup heart beat module
	*/
	void Startup();

	/**
	* @brief Shutdown heart beat module
	*/
	void Shutdown();

	/**
	* @brief set heart beat response
	*
	* @param OnHeartBeatResponse set delegate .
	* @param OnError Delegate that will be called when operation failed.
	*/
	void SetHeartBeatResponseDelegate(FHeartBeatResponse OnHeartBeatResponse
		, FErrorHandler OnError = {});

	/** Enables or disables heart beat */
	void SetHeartBeatEnabled(bool bEnable);

	// Check whether heart beat is enabled
	bool IsHeartBeatEnabled() const { return bEnableHeartBeat; }

private:
	bool SendHeartBeatEvent(float DeltaTime);
	void StartHeartBeatScheduler();
	void StopHeartBeatScheduler();

	HeartBeat() = delete;
	HeartBeat(HeartBeat const&) = delete;
	HeartBeat(HeartBeat&&) = delete;

	bool bEnableHeartBeat = false;
	bool bShuttingDown = false;
	bool bHeartBeatJobStarted = false;

	FTimespan const HeartBeatInterval = FTimespan(0, 0, 60);

	FTickerDelegate HeartBeatTickDelegate;
	FDelegateHandleAlias HeartBeatTickDelegateHandle;
	FHeartBeatResponse HeartBeatResponseDelegate;
	FErrorHandler OnHeartBeatError;

	FString HeartBeatData;
};

} // Namespace Api
} // Namespace AccelByte
