// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "Api/AccelByteBaseAnalyticsApi.h"
#include "Core/AccelByteSettings.h"
#include "Models/AccelBytePredefinedEventModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Send predefined telemetry data securely and the user should be logged in first.
 */
class ACCELBYTEUE4SDK_API PredefinedEvent : public BaseAnalytics
{
public:
	PredefinedEvent(Credentials& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);

	/**
	 * @brief Send/enqueue a single authorized telemetry data of a predefined event.
	 * User should be logged in.
	 *
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelBytePredefinedEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendPredefinedEventData(TSharedRef<T> const& Payload
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FDateTime const& ClientTimestamp = FDateTime::UtcNow())
	{
		if (!SettingsRef.bSendPredefinedEvent)
		{
			Shutdown();
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

	void SendPredefinedEventData(TSharedRef<FAccelByteModelsCachedPredefinedEventPayload> const& Payload
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FDateTime const& ClientTimestamp = FDateTime::UtcNow());

private:
	PredefinedEvent() = delete;
	PredefinedEvent(PredefinedEvent const&) = delete;
	PredefinedEvent(PredefinedEvent&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
