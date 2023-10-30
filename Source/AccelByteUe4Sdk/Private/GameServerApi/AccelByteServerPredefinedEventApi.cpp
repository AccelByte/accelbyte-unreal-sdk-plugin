// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerPredefinedEventApi.h"

namespace AccelByte
{
namespace GameServerApi
{
	
ServerPredefinedEvent::ServerPredefinedEvent(ServerCredentials& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: ServerGameTelemetry(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

ServerPredefinedEvent::~ServerPredefinedEvent()
{
	ServerGameTelemetry::~ServerGameTelemetry();
}

const FString ServerPredefinedEvent::GetEventNamespace()
{
	if (EventNamespace.IsEmpty())
	{
		EventNamespace = ServerCredentialsRef.GetNamespace();
	}
	return EventNamespace;
}

void ServerPredefinedEvent::SendPredefinedEventData(TSharedRef<FAccelByteModelsCachedEventPayload> const& Payload, FVoidHandler const& OnSuccess, FErrorHandler const& OnError, FDateTime const& ClientTimestamp)
{
	if (!ServerSettingsRef.bSendPredefinedEvent)
	{
		return;
	}

	UEnum* EnumPtr = StaticEnum<EAccelBytePredefinedEventName>();
	int64 ValueInt = 0;

	if (EnumPtr != nullptr)
	{
		ValueInt = EnumPtr->GetValueByNameString(Payload->GetPreDefinedEventName());

		if (ValueInt == INDEX_NONE)
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("EventName is not listed on EAccelBytePredefinedEventName!"));
			return;
		}
	}
	else
	{
		OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::UnknownError, TEXT("Failed to process the request, Unknown Error!"));
		return;
	}

	Payload->Payload.EventNamespace = GetEventNamespace();
	Payload->Payload.EventName = EventName;
	if (Payload->Payload.ClientTimestamp.GetTicks() == 0)
	{
		Payload->Payload.ClientTimestamp = ClientTimestamp;
	}

	Send(Payload->Payload, OnSuccess, OnError);
}

} 
}