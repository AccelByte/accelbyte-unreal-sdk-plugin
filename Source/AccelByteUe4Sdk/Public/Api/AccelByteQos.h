// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Icmp.h"
#include "Networking.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteSettings.h"
#include "Models/AccelByteQosModels.h"

namespace AccelByte
{
class Settings;
	
namespace Api
{

/**
 * @brief Qos to manage specific QoS (Latencies/Ping) Server(s).
 */
class ACCELBYTEUE4SDK_API Qos
{
public:
	Qos(Credentials& NewCredentialsRef, const Settings& NewSettingsRef);
	~Qos();
	
	/**
	 * @brief The user was just authed:
	 * - Init Qos Scheduler, if enabled via Settings.
	 * @param Response 
	 */
	void OnLoginSuccess(const FOauth2Token& Response);

	/**
	 * @brief [CALLED @ LOGIN SUCCESS] Check server latencies (ping) per-region, with optional polling.
	 * - This particular overload binds no callbacks.
	 * - Optional Pulling (default off), unless .ini polling Settings.QosLatencyPollIntervalSecs > 0.
	 * - Latencies will be cached, then optionally polled (using cached Latencies).
	 *   - Set @ Settings ini (# seconds) via QosLatencyPollIntervalSecs.
	 * - Server Latencies will optionally be polled.
	 *   - Set @ Settings ini (# seconds) via QosServerLatencyPollIntervalSecs.
	 */
	void GetServerLatencies();

	/**
	 * @brief [CALLED @ LOGIN SUCCESS] Check server latencies (ping) per-region, with optional polling.
	 * - Optional Pulling (default off), unless .ini polling Settings.QosLatencyPollIntervalSecs > 0.
	 * - Latencies will be cached, then optionally polled (using cached Latencies).
	 *   - Set @ Settings ini (# seconds) via QosLatencyPollIntervalSecs.
	 * - Server Latencies will optionally be polled.
	 *   - Set @ Settings ini (# seconds) via QosServerLatencyPollIntervalSecs.
	 *  
	 * @param OnSuccess If polling, this will also call every ping update success.
	 * @param OnError If polling, this will also call every ping update error.
	 */
	void GetServerLatencies(
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError);

	/**
	* @brief Check server latencies (ping) per-region that have status ACTIVE
	* @param OnSuccess If polling, this will also call every ping update success.
	* @param OnError If polling, this will also call every ping update error.
	*/
	void GetActiveServerLatencies(
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError);

	/**
	 * @brief For each server, ping them and record add to Latency TArray.
	 * - Then, ping each region and cache returend Latencies.
	 * @param QosServerList 
	 * @param OnSuccess 
	 */
	void PingRegionsSetLatencies(
		const FAccelByteModelsQosServerList& QosServerList,
		const THandler<TArray<TPair<FString, float>>>& OnSuccess);

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
	void CallGetQosServers(
		const bool bPingRegionsOnSuccess,
		const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess,
		const FErrorHandler& OnError);

	/**
	 * @brief Poll every x second to refresh Latencies (pings) with cached (via QosServers) regions.
	 * - SecondsPerTick is set from SettingsRef.QosLatencyPollIntervalSecs
	 * - Only polls when QosLatencyPollIntervalSecs polling Settings > 0.
	 * - Default Qos polling intervals: 0 (no polling).
	 * - Explicitly calling this a 2nd+ time will reset the poller without conflict and use the new tick interval.
	 * - Not to be confused with InitGetServerLatenciesScheduler(), that caches the target server regions for Latencies polling.
	 * 
	 * @param OnTick Sets Latencies with refreshed region latencies pings.
	 * @param OnError For example, if Preferred
	 */
	void InitGetLatenciesScheduler(
		const THandler<TArray<TPair<FString, float>>>& OnTick,
		const FErrorHandler& OnError);

	/**
	 * @brief Poll every x second to refresh target server region Latencies (pings) to later use as cached for Latency pollers/objects.
	 * - SecondsPerTick is set from SettingsRef.QosServerLatencyPollIntervalSecs
	 * - Only polls when QosServerLatencyPollIntervalSecs polling Settings > 0.
	 * - Default Qos server polling intervals: 0 (no polling).
	 * - Explicitly calling this a 2nd+ time will reset the poller without conflict and use the new tick interval.
	 * - Not to be confused with InitGetLatenciesScheduler(), that uses this func's cached target server regions for Latencies polling.
	 * 
	 * @param OnTick Sets Latencies with refreshed region latencies pings.
	 * @param OnError For example, if Preferred
	 */
	void InitGetServerLatenciesScheduler(
		const THandler<TArray<TPair<FString, float>>>& OnTick,
		const FErrorHandler& OnError);

	/** @brief Wrapper for RemoveFromTicker() for PolllatenciesHandle and PollServerLatenciesHandle. */
	void RemoveLatencyPollers();

	const TArray<TPair<FString, float>>& GetCachedLatencies() const;

	/** @brief Cleanup handler for: PollLatenciesHandle, PollServerLatenciesHandle */
	void Shutdown();
	
private:
	// Constructor
	Credentials& CredentialsRef;
	const Settings& SettingsRef;

	

	FAccelByteModelsQosServerList QosServers;
	FTimerHandle InputTimeHandle;
	TArray<TPair<FString, float>> Latencies;
	
	/**
	 * @brief Get Latencies from cached regions, every x seconds.
	 * - Default 0 (off).
	 */
	FDelegateHandle PollLatenciesHandle;

	/**
	 * @brief Get Server region latency targets and cache for Latencies object/handle, every x seconds.
	 * - Default 0 (off).
	 */
	FDelegateHandle PollServerLatenciesHandle;

	/** @brief Static cleanup handler for Tickers (Latencies Pollers) */
	static void RemoveFromTicker(FDelegateHandle& Handle);
	
	Qos(Qos const&) = delete;
	Qos(Qos&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
