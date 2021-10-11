// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteTurnModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief TurnManager API to manage Qos Server(s).
*/
class ACCELBYTEUE4SDK_API TurnManager
{
public:
	TurnManager(const Credentials& Credentials, const Settings& Settings, FHttpRetryScheduler& HttpRef);
	~TurnManager();

	/**
	* @brief Get List of TURN Server(s).
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServerList.
	* @param OnError This will be called when the operation failed.
	*/
	void GetTurnServers(const THandler<FAccelByteModelsTurnServerList>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Get closest TURN server(s).
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServer.
	* @param OnError This will be called when the operation failed.
	*/
	void GetClosestTurnServer(const THandler<FAccelByteModelsTurnServer>& OnSuccess, const FErrorHandler& OnError);
	
private:
	FHttpRetryScheduler& HttpRef;
	const Credentials& Credentials;
	const Settings& Settings;
	FAccelByteModelsTurnServer ClosestServer;
	float FastestPing;
	int Counter;

	TurnManager() = delete;
	TurnManager(TurnManager const&) = delete;
	TurnManager(TurnManager&&) = delete;

	FString GetTurnManagerServerUrl() const;
};

} // Namespace Api
} // Namespace AccelByte
