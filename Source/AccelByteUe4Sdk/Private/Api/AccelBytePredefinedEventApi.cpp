// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelBytePredefinedEventApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteApiClient.h"

namespace AccelByte
{
namespace Api
{
	
PredefinedEvent::PredefinedEvent(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: BaseAnalytics(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient, TEXT("PreDefinedEvent"), false)
{
	if (!SettingsRef.bSendPredefinedEvent)
	{
		Shutdown();
	}
}

PredefinedEvent::PredefinedEvent(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: BaseAnalytics(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform, TEXT("PreDefinedEvent"), false)
{
	if (!SettingsRef.bSendPredefinedEvent)
	{
		Shutdown();
	}
}

void PredefinedEvent::SendPredefinedEventData(const TSharedRef<FAccelByteModelsCachedPredefinedEventPayload>& Payload
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FDateTime const& ClientTimestamp)
{
	if (!SettingsRef.bSendPredefinedEvent)
	{
		Shutdown();
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

	SendEventData(Payload->Payload, OnSuccess, OnError, ClientTimestamp);
}

} 
}