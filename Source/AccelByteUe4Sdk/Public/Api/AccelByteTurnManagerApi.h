// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
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
class ACCELBYTEUE4SDK_API TurnManager : public FApiBase
{
public:
	TurnManager(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~TurnManager();

	/**
	 * @brief Get List of TURN Server(s).
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServerList.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetTurnServers(const THandler<FAccelByteModelsTurnServerList>& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get closest TURN server(s).
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServer.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetClosestTurnServer(THandler<FAccelByteModelsTurnServer> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get credential to authenticate with the turn server.
	 *
	 * @param Region
	 * @param Ip
	 * @param Port
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServerCredential.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetTurnCredential(FString const& Region
		, FString const& Ip
		, int Port
		, THandler<FAccelByteModelsTurnServerCredential> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send info about P2P connection type and selected turn server region to BE. This called by client.
	 *
	 * @param SelectedTurnServerRegion Selected turn server region
	 * @param P2PConnectionType P2P connection type enum (host, relay, srflx, or prflx)
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendMetric(FString const& SelectedTurnServerRegion
		, EP2PConnectionType P2PConnectionType
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
private:
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
