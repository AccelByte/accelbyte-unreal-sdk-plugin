// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "Api/AccelByteGameTelemetryApi.h"
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
class ACCELBYTEUE4SDK_API PredefinedEvent : public GameTelemetry
{
public:
	PredefinedEvent(Credentials& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~PredefinedEvent();

	/**
	 * @brief Send/enqueue a single authorized telemetry data of a predefined event.
	 * User should be logged in.
	 *
	 * @param EventNamespace Namespace of the event.
	 * @param EventName Name of the event.
	 * @param Payload The data to be send, each event expected to have different payload body, see Models/AccelBytePredefinedEventModels.h
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param ClientTimestamp Timestamp when the event is triggered, the default will be FDateTime::UtcNow()
	 */
	template<typename T>
	void SendPredefinedEventData(const TSharedRef<T>& Payload, FVoidHandler const& OnSuccess, FErrorHandler const& OnError, FDateTime const& ClientTimestamp = FDateTime::UtcNow())
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
			const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Payload.Get());
			if (JsonObject.IsValid())
			{
				FAccelByteModelsTelemetryBody Body;
				Body.EventNamespace = EventNamespace;
				Body.EventName = Payload->GetEventName();
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
	}

	void SendPredefinedEventData(const TSharedRef<FAccelByteModelsCachedEventPayload>& Payload, FVoidHandler const& OnSuccess, FErrorHandler const& OnError, FDateTime const& ClientTimestamp = FDateTime::UtcNow());

protected:
	const FString EventNamespace = TEXT("io.accelbyte.intelligence.predefinedevents");

private:
	using GameTelemetry::SetImmediateEventList;
	using GameTelemetry::Send;

	PredefinedEvent() = delete;
	PredefinedEvent(PredefinedEvent const&) = delete;
	PredefinedEvent(PredefinedEvent&&) = delete;

};

} // Namespace Api
} // Namespace AccelByte
