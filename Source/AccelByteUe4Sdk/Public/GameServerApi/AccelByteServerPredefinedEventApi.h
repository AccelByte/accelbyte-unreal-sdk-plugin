// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "GameServerApi/AccelByteServerBaseAnalyticsApi.h"
#include "Models/AccelBytePredefinedEventModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Send predefined telemetry data securely and the server should be logged in first.
 */
class ACCELBYTEUE4SDK_API ServerPredefinedEvent : public ServerBaseAnalytics
{
public:
	ServerPredefinedEvent(ServerCredentials& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);

	/**
	 * @brief Send/enqueue a single authorized telemetry data of a predefined event.
	 * Server should be logged in.
	 *
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelBytePredefinedEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendPredefinedEventData(TSharedRef<T> const& Payload, 
									FVoidHandler const& OnSuccess, 
									FErrorHandler const& OnError, 
									FDateTime const& ClientTimestamp = FDateTime::UtcNow())
	{
		if (!ServerSettingsRef.bSendPredefinedEvent)
		{
			return;
		}

		if (!TIsDerivedFrom<T, FAccelByteModelsPredefinedEvent>::IsDerived)
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("Payload must derive from FAccelByteModelsPredefinedEvent!"));
			return;
		}
		else
		{
			Payload->PreDefinedEventName = Payload->GetPreDefinedEventName();
			SendEventData(Payload, OnSuccess, OnError, ClientTimestamp);
		}
	}

	void SendPredefinedEventData(TSharedRef<FAccelByteModelsCachedPredefinedEventPayload> const& Payload, 
									FVoidHandler const& OnSuccess, 
									FErrorHandler const& OnError, 
									FDateTime const& ClientTimestamp = FDateTime::UtcNow());

private:
	ServerPredefinedEvent() = delete;
	ServerPredefinedEvent(ServerPredefinedEvent const&) = delete;
	ServerPredefinedEvent(ServerPredefinedEvent&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
