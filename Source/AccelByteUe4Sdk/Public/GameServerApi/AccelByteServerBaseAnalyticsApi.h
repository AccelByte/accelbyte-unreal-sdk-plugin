// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Send telemetry data securely and the server should be logged in first.
 */
class ACCELBYTEUE4SDK_API ServerBaseAnalytics : public ServerGameTelemetry
{
protected:
	ServerBaseAnalytics(ServerCredentials& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, FString const& InEventName
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient);

	/**
	 * @brief Send/enqueue a single authorized telemetry data of an event.
	 * Server should be logged in.
	 *
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelBytePredefinedEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendEventData(TSharedRef<T> const& Payload, 
						FVoidHandler const& OnSuccess, 
						FErrorHandler const& OnError, 
						FDateTime const& ClientTimestamp = FDateTime::UtcNow())
	{
		const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Payload.Get());
		if (JsonObject.IsValid())
		{
			FAccelByteModelsTelemetryBody Body;
			Body.EventName = EventName;
			Body.Payload = JsonObject;
			Body.ClientTimestamp = ClientTimestamp;

			Send(Body, OnSuccess, OnError);
		}
		else
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("Failed to convert UStruct to Json!"));
			return;
		}
	}

	void SendEventData(FAccelByteModelsTelemetryBody Payload, 
						FVoidHandler const& OnSuccess, 
						FErrorHandler const& OnError, 
						FDateTime const& ClientTimestamp = FDateTime::UtcNow());

	const FString EventName;
	using FServerApiBase::ServerCredentialsRef;
	using ServerGameTelemetry::Flush;
	using ServerGameTelemetry::SetImmediateEventList;

private:
	using ServerGameTelemetry::Send;
	ServerBaseAnalytics() = delete;
	ServerBaseAnalytics(ServerBaseAnalytics const&) = delete;
	ServerBaseAnalytics(ServerBaseAnalytics&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
