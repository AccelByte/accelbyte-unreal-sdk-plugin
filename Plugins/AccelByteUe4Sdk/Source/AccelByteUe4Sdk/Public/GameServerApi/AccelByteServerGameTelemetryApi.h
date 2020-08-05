// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "Models/AccelByteGameTelemetryModels.h"
#include "Core/AccelByteError.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Send telemetry data securely and server should be logged in first.
 */
class ACCELBYTEUE4SDK_API ServerGameTelemetry
{
public:
	ServerGameTelemetry(const ServerCredentials& Credentials, const ServerSettings& Settings);
	~ServerGameTelemetry();

	/**
	 * @brief Set the interval of sending telemetry event to the backend.
	 * By default it sends the queued events once a minute. 
	 * Should not be less than 5 seconds.
	 *
	 * @param Interval The interval between telemetry event
	 */
	void SetBatchFrequency(FTimespan Interval);

	/**
	 * @brief Set list of event that need to be sent immediately without the needs to jobQueue it.
	 *
	 * @param EventNames FString Array of payload EventName.
	 */
	void SetImmediateEventList(const TArray<FString>& EventNames);

	/**
	 * @brief Send/enqueue a single authorized telemetry data.
	 * Server should be logged in. See DedicatedServer::LoginWithClientCredentials()
	 *
	 * @param TelemetryBody Telemetry request with arbitrary payload.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void Send(FAccelByteModelsTelemetryBody TelemetryBody, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

private:
	void SendProtectedEvents(TArray<FAccelByteModelsTelemetryBody> Events, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	bool PeriodicTelemetry(float DeltaTime);

	ServerGameTelemetry() = delete;
	ServerGameTelemetry(ServerGameTelemetry const&) = delete;
	ServerGameTelemetry(ServerGameTelemetry&&) = delete;

	const ServerCredentials& Credentials;
	const ServerSettings& Settings;

	FTimespan TelemetryInterval = FTimespan(0, 1, 0);
	TSet<FString> ImmediateEvents;
	TQueue<TTuple<FAccelByteModelsTelemetryBody, FVoidHandler, FErrorHandler>> JobQueue;
	bool bTelemetryJobStarted = false;
	const FTimespan MINIMUM_INTERVAL_TELEMETRY = FTimespan(0, 0, 5);
	FTickerDelegate GameTelemetryTickDelegate;
	FDelegateHandle GameTelemetryTickDelegateHandle;
};

} // Namespace Api
} // Namespace AccelByte
