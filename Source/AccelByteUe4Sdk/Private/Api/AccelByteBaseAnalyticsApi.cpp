// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteBaseAnalyticsApi.h"

namespace AccelByte
{
namespace Api
{
	
BaseAnalytics::BaseAnalytics(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, FString const& InEventName
	, bool bInCacheEvent
	, bool bInRetryOnFailed)
	: GameTelemetry(InCredentialsRef, InSettingsRef, InHttpRef, bInCacheEvent, bInRetryOnFailed)
	, EventName(InEventName)
{}

BaseAnalytics::BaseAnalytics(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient
	, FString const& InEventName
	, bool bInCacheEvent
	, bool bInRetryOnFailed)
	: GameTelemetry(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient, bInCacheEvent, bInRetryOnFailed)
	, EventName(InEventName)
{}

void BaseAnalytics::SendEventData(const TSharedPtr<FJsonObject>& Payload, FVoidHandler const& OnSuccess, FErrorHandler const& OnError, FDateTime const& ClientTimestamp)
{
	if (Payload.IsValid())
	{
		FAccelByteModelsTelemetryBody Body;
		Body.EventName = EventName;
		Body.Payload = Payload;
		Body.ClientTimestamp = ClientTimestamp;

		Send(Body, OnSuccess, OnError);
	}
	else
	{
		OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("Failed to convert UStruct to Json!"));
		return;
	}
}

void BaseAnalytics::SendEventData(FAccelByteModelsTelemetryBody Payload,
									FVoidHandler const& OnSuccess, 
									FErrorHandler const& OnError, 
									FDateTime const& ClientTimestamp)
{
	Payload.EventName = EventName;
	if (Payload.ClientTimestamp.GetTicks() == 0)
	{
		Payload.ClientTimestamp = ClientTimestamp;
	}

	Send(Payload, OnSuccess, OnError);
}

} 
}