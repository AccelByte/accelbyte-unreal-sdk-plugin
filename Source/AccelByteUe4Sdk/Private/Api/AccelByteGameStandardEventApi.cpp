// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGameStandardEventApi.h"

namespace AccelByte
{
namespace Api
{
	
GameStandardEvent::GameStandardEvent(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: BaseAnalytics(InCredentialsRef, InSettingsRef, InHttpRef, TEXT("GameStandardEvent"), true, true)
{}

FString GameStandardEvent::GetTelemetryKey()
{
	FString Environment = *SettingsRef.SettingsEnvironment;
	FString UserId = *CredentialsRef->GetUserId();
	if (UserId.IsEmpty())
	{
		return UserId;
	}
	FString TelemetryKey = FString::Printf(TEXT("%sGameStandardClientEvent_%s"), *Environment, *UserId);
	return TelemetryKey;
}

void GameStandardEvent::SendGameStandardEventData(TSharedRef<FAccelByteModelsCachedGameStandardEventPayload> const& Payload
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FDateTime const& ClientTimestamp)
{

	UEnum* EnumPtr = StaticEnum<EAccelByteGameStandardEventName>();
	int64 ValueInt = 0;

	if (EnumPtr != nullptr)
	{
		ValueInt = EnumPtr->GetValueByNameString(Payload->GetGameStandardEventName());

		if (ValueInt == INDEX_NONE)
		{
			OnError.ExecuteIfBound((int32)AccelByte::ErrorCodes::InvalidRequest, TEXT("EventName is not listed on EAccelByteGameStandardEventName!"));
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