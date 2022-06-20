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
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"

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
	ServerGameTelemetry(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
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

	/**
	* @brief Flush pending telemetry events
	*/
	void Flush();

private:
	struct FJob
	{
		FAccelByteModelsTelemetryBody TelemetryBody;
		FVoidHandler OnSuccess;
		FErrorHandler OnError;

		FJob(FAccelByteModelsTelemetryBody TelemetryBody, FVoidHandler OnSuccess, FErrorHandler OnError)
			: TelemetryBody{TelemetryBody}
			, OnSuccess{OnSuccess}
			, OnError{OnError}
		{
		}
	};

	void SendProtectedEvents(TArray<FAccelByteModelsTelemetryBody> Events, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	bool PeriodicTelemetry(float DeltaTime);

	ServerGameTelemetry() = delete;
	ServerGameTelemetry(ServerGameTelemetry const&) = delete;
	ServerGameTelemetry(ServerGameTelemetry&&) = delete;

	ServerCredentials const& CredentialsRef;
	ServerSettings const& SettingsRef;
	FHttpRetryScheduler& HttpRef;

	FTimespan TelemetryInterval = FTimespan(0, 1, 0);
	TSet<FString> ImmediateEvents;
	TQueue<TSharedPtr<FJob>> JobQueue;
	bool bTelemetryJobStarted = false;
	const FTimespan MINIMUM_INTERVAL_TELEMETRY = FTimespan(0, 0, 5);
	FTickerDelegate GameTelemetryTickDelegate;
	FDelegateHandleAlias GameTelemetryTickDelegateHandle;
};

} // Namespace Api
} // Namespace AccelByte
