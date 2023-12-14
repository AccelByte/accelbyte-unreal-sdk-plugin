// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "GameServerApi/AccelByteServerBaseAnalyticsApi.h"
#include "Models/AccelByteGameStandardEventModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Send game standard telemetry data securely and the server should be logged in first.
 */
class ACCELBYTEUE4SDK_API ServerGameStandardEvent : public ServerBaseAnalytics
{
public:
	ServerGameStandardEvent(ServerCredentials& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);

	/**
	 * @brief Send/enqueue a single authorized telemetry data of a game standard event.
	 * Server should be logged in.
	 *
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelByteGameStandardEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendGameStandardEventData(TSharedRef<T> const& Payload, 
									FVoidHandler const& OnSuccess, 
									FErrorHandler const& OnError, 
									FDateTime const& ClientTimestamp = FDateTime::UtcNow())
	{
		if (!TIsDerivedFrom<T, FAccelByteModelsGameStandardEvent>::IsDerived)
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("Payload must derive from FAccelByteModelsGameStandardEvent!"));
			return;
		}
		else
		{
			Payload->GameStandardEventName = Payload->GetGameStandardEventName();
			SendEventData(Payload, OnSuccess, OnError, ClientTimestamp);
		}
	}

	void SendGameStandardEventData(TSharedRef<FAccelByteModelsCachedGameStandardEventPayload> const& Payload, 
									FVoidHandler const& OnSuccess, 
									FErrorHandler const& OnError, 
									FDateTime const& ClientTimestamp = FDateTime::UtcNow());

private:
	ServerGameStandardEvent() = delete;
	ServerGameStandardEvent(ServerGameStandardEvent const&) = delete;
	ServerGameStandardEvent(ServerGameStandardEvent&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
