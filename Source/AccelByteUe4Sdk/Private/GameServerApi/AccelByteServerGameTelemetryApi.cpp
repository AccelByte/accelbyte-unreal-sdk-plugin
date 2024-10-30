// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerGameTelemetryApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerGameTelemetry::ServerGameTelemetry(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerGameTelemetry::~ServerGameTelemetry()
{
	if(UObjectInitialized() && GameTelemetryTickDelegateHandle.IsValid())
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(GameTelemetryTickDelegateHandle);
		GameTelemetryTickDelegateHandle.Reset();
	}
}

void ServerGameTelemetry::SetBatchFrequency(FTimespan Interval)
{
	if (Interval >= MINIMUM_INTERVAL_TELEMETRY)
	{
		TelemetryInterval = Interval;
	}
	else
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Telemetry schedule interval is too small! Set to %f seconds."), MINIMUM_INTERVAL_TELEMETRY.GetTotalSeconds());
		TelemetryInterval = MINIMUM_INTERVAL_TELEMETRY;
	}
}

void ServerGameTelemetry::SetImmediateEventList(const TArray<FString>& EventNames)
{
	ImmediateEvents = TSet<FString>(EventNames);
}

void ServerGameTelemetry::Send(FAccelByteModelsTelemetryBody TelemetryBody
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if(TelemetryBody.ClientTimestamp.GetTicks() == 0)
	{
		TelemetryBody.ClientTimestamp = FDateTime::UtcNow();
	}

	if (TelemetryBody.EventNamespace.IsEmpty())
	{
		TelemetryBody.EventNamespace = GetEventNamespace();
	}
	
	if (ImmediateEvents.Contains(TelemetryBody.EventName))
	{
		SendProtectedEvents({TelemetryBody}, OnSuccess, OnError);
	}
	else
	{
		JobQueue.Enqueue(MakeShared<FJob>(TelemetryBody, OnSuccess, OnError ));
		if (!bTelemetryJobStarted)
		{
			bTelemetryJobStarted = true;
			GameTelemetryTickDelegate = FTickerDelegate::CreateRaw(this, &ServerGameTelemetry::PeriodicTelemetry);
			GameTelemetryTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(GameTelemetryTickDelegate, static_cast<float>(TelemetryInterval.GetTotalSeconds()));
		}
	}
}

void ServerGameTelemetry::Flush()
{
	PeriodicTelemetry(0);
}

bool ServerGameTelemetry::PeriodicTelemetry(float DeltaTime)
{
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelByteServerPeriodicTelemetry"));
#endif
	FReport::Log(FString(__FUNCTION__));

	if (JobQueue.IsEmpty()) { return true; }

	TArray<FAccelByteModelsTelemetryBody> TelemetryBodies;
	TSharedRef<TArray<TSharedPtr<FJob>>> Jobs = MakeShared<TArray<TSharedPtr<FJob>>>();
	TSharedPtr<FJob> DequeueResult;
	while (JobQueue.Dequeue(DequeueResult))
	{
		TelemetryBodies.Add(DequeueResult->TelemetryBody);
		Jobs->Add(DequeueResult);
	}

	SendProtectedEvents(TelemetryBodies
		, FVoidHandler::CreateLambda(
			[Jobs]()
			{
				for (auto& Job : *Jobs)
				{
					auto _ = Job->OnSuccess.ExecuteIfBound();
				}
			})
		, FErrorHandler::CreateLambda(
			[Jobs](int32 Code, FString Message)
			{
				for (auto& Job : *Jobs)
				{
					auto _ = Job->OnError.ExecuteIfBound(Code, Message);
				}
			}));

	return true;
}

const FString ServerGameTelemetry::GetEventNamespace()
{
	if (EventNamespace.IsEmpty())
	{
		EventNamespace = ServerCredentialsRef->GetNamespace();
	}
	return EventNamespace;
}

void ServerGameTelemetry::SendProtectedEvents(TArray<FAccelByteModelsTelemetryBody> Events
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/protected/events")
		, *ServerSettingsRef.GameTelemetryServerUrl);

	FString Content = TEXT("");

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (const auto& Event : Events)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		JsonObject->SetStringField("EventNamespace", Event.EventNamespace);
		JsonObject->SetStringField("EventName", Event.EventName);
		JsonObject->SetObjectField("Payload", Event.Payload);
		JsonObject->SetStringField("ClientTimestamp", FAccelByteUtilities::GetCurrentServerTime().ToIso8601());
		JsonObject->SetStringField("FlightId", FAccelByteUtilities::GetFlightId());
		JsonObject->SetStringField("DeviceType", FAccelByteUtilities::GetPlatformName());

		JsonArray.Add(MakeShared<FJsonValueObject>(JsonObject));
	}
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonArray, Writer);

	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

} 
}