// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelBytePresenceBroadcastEventApi.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
namespace Api
{

PresenceBroadcastEvent::PresenceBroadcastEvent(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, CredentialsRef{ InCredentialsRef }
{
	PresenceBroadcastLoginSuccess = CredentialsRef.OnLoginSuccess().AddRaw(this, &PresenceBroadcastEvent::OnLoginSuccess);
}

PresenceBroadcastEvent::~PresenceBroadcastEvent()
{
	Shutdown();
}

bool PresenceBroadcastEvent::IsGamePlatformSent()
{
	if (!CredentialsRef.IsSessionValid())
	{
		bIsGamePlatformSent = false;
	}
	return bIsGamePlatformSent;
}

void PresenceBroadcastEvent::SetGameState(EAccelByteGameState State, FVoidHandler OnSuccess, FErrorHandler OnError, const FString& Description)
{
	GameState = State;
	PresencePayload.Game_state = FAccelByteUtilities::GetUEnumValueAsString(GameState);
	PresencePayload.Game_context = Description;
	SendPresenceBroadcastEvent(PresencePayload, OnSuccess, OnError);
}

void PresenceBroadcastEvent::SetHeartbeatInterval(FTimespan Interval)
{
	if (Interval >= MinimumBroadcastInterval)
	{
		BroadcastInterval = Interval;
	}
	else
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Presence broadcast interval is too small! Set to %f seconds."), MinimumBroadcastInterval.GetTotalSeconds());
		BroadcastInterval = MinimumBroadcastInterval;
	}
}

void PresenceBroadcastEvent::SetOnSendPresenceHeartbeatError(const FErrorHandler& OnError)
{
	OnSendPresenceHeartbeatError = OnError;
}

void PresenceBroadcastEvent::SetOnSendPresenceHeartbeatSuccess(const FVoidHandler& OnSendSuccess)
{
	OnSendPresenceHeartbeatSuccess = OnSendSuccess;
}

void PresenceBroadcastEvent::SendPlatform(FVoidHandler OnSuccess, FErrorHandler OnError)
{
	FAccelBytePresenceBroadcastEventPayload PlatformInfo;
	PlatformInfo.Flight_id = GetFlightId();
	PlatformInfo.Platform_name = FAccelByteUtilities::GetPlatformName();

	SendPresenceBroadcastEvent(PlatformInfo, OnSuccess, OnError);
}

void PresenceBroadcastEvent::AddPresenceData(const FString& Key, const FString& Value)
{
	if (!AdditionalPayload.IsValid())
	{
		return;
	}
	AdditionalPayload->SetStringField(Key, Value);
	PresencePayload.Additional_data.JsonObject = AdditionalPayload;
}

void PresenceBroadcastEvent::RemovePresenceData(const FString& Key)
{
	if (!AdditionalPayload.IsValid())
	{
		return;
	}
	AdditionalPayload->RemoveField(Key);
}

void PresenceBroadcastEvent::Startup()
{
	bIsShuttingDown = false;
}

void PresenceBroadcastEvent::StartHeartbeat()
{
	if (PresenceBroadcastEventHeartbeatTickDelegateHandle.IsValid())
	{
		return;
	}
	PresenceBroadcastEventHeartbeatTickDelegate = FTickerDelegate::CreateRaw(this, &PresenceBroadcastEvent::PeriodicHeartbeat);
	PresenceBroadcastEventHeartbeatTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(PresenceBroadcastEventHeartbeatTickDelegate, static_cast<float>(BroadcastInterval.GetTotalSeconds()));
}

void PresenceBroadcastEvent::StopHeartbeat()
{
	if (PresenceBroadcastEventHeartbeatTickDelegateHandle.IsValid())
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(PresenceBroadcastEventHeartbeatTickDelegateHandle);
		PresenceBroadcastEventHeartbeatTickDelegateHandle.Reset();
	}
}

void PresenceBroadcastEvent::Shutdown()
{
	bIsShuttingDown = true;
	if (UObjectInitialized())
	{
		if (PresenceBroadcastEventHeartbeatTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(PresenceBroadcastEventHeartbeatTickDelegateHandle);
			PresenceBroadcastEventHeartbeatTickDelegateHandle.Reset();
		}
		if (PresenceBroadcastLoginSuccess.IsValid())
		{
			CredentialsRef.OnLoginSuccess().Remove(PresenceBroadcastLoginSuccess);
		}
	}
}

FString PresenceBroadcastEvent::GetFlightId()
{
	if (FlightId.IsEmpty())
	{
		FlightId = FGuid::NewGuid().ToString();
	}
	return FlightId;
}

bool PresenceBroadcastEvent::PeriodicHeartbeat(float DeltaTime)
{
	FReport::Log(FString(__FUNCTION__));
	SendPresenceBroadcastEvent(PresencePayload, OnSendPresenceHeartbeatSuccess, OnSendPresenceHeartbeatError);
	return true;
}

void PresenceBroadcastEvent::OnLoginSuccess(FOauth2Token const& Response)
{
	if (!AdditionalPayload.IsValid())
	{
		AdditionalPayload = MakeShared<FJsonObject>();
	}
	PresencePayload.Flight_id = GetFlightId();
	PresencePayload.Game_state = FAccelByteUtilities::GetUEnumValueAsString(GameState);
	PresencePayload.Platform_name = FAccelByteUtilities::GetPlatformName();

	SendPlatform(FVoidHandler::CreateLambda([this]()
		{
			bIsGamePlatformSent = true;
		}),
		FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Failed to send platform info. Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		}));

	if (SettingsRef.bEnablePresenceBroadcastEventHeartbeat)
	{
		SetHeartbeatInterval(FTimespan::FromSeconds(SettingsRef.PresenceBroadcastEventHeartbeatInterval));
		StartHeartbeat();
	}
}

void PresenceBroadcastEvent::SendPresenceBroadcastEvent(FAccelBytePresenceBroadcastEventPayload const& Events, FVoidHandler OnSuccess, FErrorHandler OnError)
{
	if (bIsShuttingDown)
	{
		return;
	}

	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/protected/events")
		, *SettingsRef.GameTelemetryServerUrl);

	TSharedPtr<FJsonObject> JsonObjectPtr = MakeShared<FJsonObject>();
	JsonObjectPtr->SetStringField("EventNamespace", SettingsRef.PublisherNamespace);
	JsonObjectPtr->SetStringField("EventName", EventName);
	JsonObjectPtr->SetObjectField("Payload", FJsonObjectConverter::UStructToJsonObject(Events));
	JsonObjectPtr->SetNumberField("ClientTimestamp", FDateTime::UtcNow().ToUnixTimestamp());
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObjectPtr, FAccelByteUtilities::FieldRemovalFlagAll);

	FString Content{};
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObjectPtr.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

}
}