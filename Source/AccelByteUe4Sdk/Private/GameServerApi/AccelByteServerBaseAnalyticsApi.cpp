// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerBaseAnalyticsApi.h"

namespace AccelByte
{
namespace GameServerApi
{
	
ServerBaseAnalytics::ServerBaseAnalytics(ServerCredentials& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, FString const& InEventName)
	: ServerGameTelemetry(InCredentialsRef, InSettingsRef, InHttpRef)
	, EventName(InEventName)
{
}

void ServerBaseAnalytics::SendEventData(FAccelByteModelsTelemetryBody Payload, 
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