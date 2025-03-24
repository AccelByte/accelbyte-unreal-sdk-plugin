// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteQosManagerApi.h"
#include "IPAddress.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteDefines.h"
#include "Core/AccelByteMessagingSystem.h"
#include "Models/AccelByteQosModels.h"
#include "Templates/SharedPointer.h"

#define ACCELBYTE_MAX_PING 9999.0

namespace AccelByte
{
namespace Api
{

/**
 * @brief Qos to manage specific QoS (Latencies/Ping) Server(s).
 */
class ACCELBYTEUE4SDK_API Qos : public TSharedFromThis<Qos, ESPMode::ThreadSafe>
{
public:
	Qos(Credentials& NewCredentialsRef, const Settings& NewSettingsRef, FAccelByteMessagingSystem& InMessagingSystemRef, const QosManagerWPtr QosManagerWeak, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~Qos();

	void SetApiClient(TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient);

	/**
	 * @brief Check server latencies (ping) per-region, with optional polling.
	 * - Optional Pulling (default off), unless .ini polling Settings.QosLatencyPollIntervalSecs > 0.
	 * - Latencies will be cached, then optionally polled (using cached QoS Servers).
	 *   - Set @ Settings ini (# seconds) via QosLatencyPollIntervalSecs.
	 * - QoS Servers will optionally be polled.
	 *   - Set @ Settings ini (# seconds) via QosServerLatencyPollIntervalSecs.
	 *  
	 * @param OnSuccess If polling, this will also call every ping update success.
	 * @param OnError If polling, this will also call every ping update error.
	 */
	void GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Check server latencies (ping) per-region that have status ACTIVE
	 * @param OnSuccess If polling, this will also call every ping update success.
	 * @param OnError If polling, this will also call every ping update error.
	 */
	void GetActiveServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Start the latency polling schedulers
	 */
	void StartLatencyPollers();

	/**
	 * @brief Wrapper for RemoveFromTicker() for PollLatenciesHandle and PollServerLatenciesHandle.
	 */
	void StopLatencyPollers();

	/**
	 * @brief Check whether latency polling schedulers are active
	 */
	bool AreLatencyPollersActive();

	/**
	 * @brief Get cached latencies data
	 */
	const TArray<TPair<FString, float>>& GetCachedLatencies();

	/**
	* @brief Startup module
	*/
	void Startup();
	
private:
	// Constructor
	FCredentialsRef CredentialsRef;
	const Settings& SettingsRef;
	QosManagerWPtr QosManagerWeak;
	FAccelByteMessagingSystemWPtr MessagingSystemWPtr;
	TWeakPtr<FApiClient, ESPMode::ThreadSafe> ApiClient;

	static FAccelByteModelsQosServerList QosServers;
	static TArray<TPair<FString, float>> Latencies;
	static TMap<FString, TSharedPtr<FInternetAddr>> ResolvedAddresses;

	static constexpr float ResolveServerAddressDelay = 0.5;//in seconds
	
	/**
	 * @brief Get Latencies from cached regions, every x seconds.
	 * - Default 0 (off).
	 */
	static FDelegateHandleAlias PollLatenciesHandle;

	/** @brief Static cleanup handler for Tickers (Latencies Pollers) */
	static void RemoveFromTicker(FDelegateHandleAlias& Handle);

	/**
	 * @brief For each server, ping them and record add to Latency TArray.
	 * - Then, ping each region and cache returned Latencies.
	 * @param QosServerList
	 * @param OnSuccess
	 * @param OnError
	 */
	void PingRegionsSetLatencies(const FAccelByteModelsQosServerList& QosServerList
		, const THandler<TArray<TPair<FString, float>>>& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Sets QosServers, using this cache for future calls.
	 * - Optionally (default off), via Settings.QosServerLatencyPollIntervalSecs, a separate
	 *     polling interval can be set to update QosServers for the regions, themselves.
	 *  - Not to be confused with Settings.QosLatencyPollIntervalSecs (uses server cached regions).
	 *
	 * @param bPingRegionsOnSuccess Upon success, we'll call PingRegionsSetLatencies() from the results.
	 * @param OnPingRegionsSuccess Only calls back if bPingRegionsOnSuccess
	 * @param OnError
	 */
	void CallGetQosServers(const bool bPingRegionsOnSuccess
		, const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Poll every x second to refresh Latencies (pings) with cached (via QosServers) regions.
	 * - SecondsPerTick is set from SettingsRef.QosLatencyPollIntervalSecs
	 * - Only polls when QosLatencyPollIntervalSecs polling Settings > 0.
	 * - Default Qos polling intervals: 0 (no polling).
	 * - Explicitly calling this a 2nd+ time will reset the poller without conflict and use the new tick interval.
	 * - Not to be confused with InitGetServerLatenciesScheduler(), that caches the target server regions for Latencies polling.
	 *
	 * @param LatencyPollIntervalSecs the interval for polling the latency in seconds
	 */
	void InitGetLatenciesScheduler(float LatencyPollIntervalSecs);

	/**
	 * @brief Resolves QOS server IP using the platform's socket subsystem.
	 * @param OutServer QOS server structure that will be modified
	 * @return true if resolution was successful, false otherwise
	 */
	bool ResolveQosServerAddress(FAccelByteModelsQosServer& OutServer);

#pragma region MessagingSystem
private:
	FDelegateHandle LobbyConnectedDelegateHandle;
	FOnMessagingSystemReceivedMessage OnLobbyConnectedHandle;

	/**
	 * @brief Send cached latencies to messaging system to notify the latencies is updated.
	 */
	void SendQosLatenciesMessage();

	/**
	 * @brief Handler for receiving lobby connected message from messaging system
	 *
	 * @param Payload the payload of the message
	 */
	void OnLobbyConnected(const FString& Payload);

	bool bQosUpdated{false};

	const float QosUpdateCheckerIntervalSecs = 60.0f;
	FTickerDelegate QosUpdateCheckerTickerDelegate{};
	FDelegateHandleAlias QosUpdateCheckerHandle{};

	bool bIsStarted = false;

	bool CheckQosUpdate(float DeltaTime);
#pragma endregion

	Qos(Qos const&) = delete;
	Qos(Qos&&) = delete;
};

typedef TSharedRef<Qos, ESPMode::ThreadSafe> QosRef;
typedef TSharedPtr<Qos, ESPMode::ThreadSafe> QosPtr;
typedef TWeakPtr<Qos, ESPMode::ThreadSafe> QosWPtr;

} // Namespace Api
} // Namespace AccelByte
