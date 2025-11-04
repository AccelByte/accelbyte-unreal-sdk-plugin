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
class ACCELBYTEUE4SDK_API TurnManager 
	: public FApiBase
	, public TSharedFromThis<TurnManager, ESPMode::ThreadSafe>
{
public:
	TurnManager(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	TurnManager(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);
	
	virtual ~TurnManager();

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
	 * @brief Get List of TURN Server(s).
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServerList.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetTurnServersV2(const THandler<FAccelByteModelsTurnServerList>& OnSuccess
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
	 * @brief Get closest TURN server(s).
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsTurnServer.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetClosestTurnServerV2(THandler<FAccelByteModelsTurnServer> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get turn server metrics by specific region.
	 *
	 * @param Region Specified region to get the server metrics
	 * @param OnSuccess This will be called when the operation succeeded. The result is the latency of specified region.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetTurnServerLatencyByRegion(const FString& Region
		, THandler<int32> const& OnSuccess
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
	 * @param Latency It will send to turn server metrics if specified
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendMetric(FString const& SelectedTurnServerRegion
		, EP2PConnectionType P2PConnectionType
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Latency = INDEX_NONE);

	/**
	 * @brief Sets QosServers, using this cache for future calls.
	 *
	 * @param OnPingRegionsSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetTurnServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Get cached latencies data
	 */
	static const TArray<TPair<FString, float>>& GetCachedLatencies();
	
private:
	FAccelByteModelsTurnServer ClosestServer;
	float FastestPing;
	int Counter;

	TurnManager() = delete;
	TurnManager(TurnManager const&) = delete;
	TurnManager(TurnManager&&) = delete;

	FString GetTurnManagerServerUrl() const;

	void PingRegionsSetLatencies(const FAccelByteModelsTurnServerList& TurnServerList
		, const THandler<TArray<TPair<FString, float>>>& OnSuccess
		, const FErrorHandler& OnError) const;

	static FAccelByteModelsTurnServerList TurnServers;
	static TArray<TPair<FString, float>> Latencies;
};

using TurnManagerRef = TSharedRef<TurnManager, ESPMode::ThreadSafe>;
using TurnManagerPtr = TSharedPtr<TurnManager, ESPMode::ThreadSafe>;
using TurnManagerWPtr = TWeakPtr<TurnManager, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
