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
	: CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
	, HttpRef(InHttpRef)
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

void ServerGameTelemetry::Send(FAccelByteModelsTelemetryBody TelemetryBody, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if(TelemetryBody.EventTimestamp.GetTicks() == 0)
	{
		TelemetryBody.EventTimestamp = FDateTime::UtcNow();
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

	SendProtectedEvents(TelemetryBodies, FVoidHandler::CreateLambda([Jobs]()
	{
		for (auto& Job : *Jobs)
		{
			auto _ = Job->OnSuccess.ExecuteIfBound();
		}
	}), FErrorHandler::CreateLambda([Jobs](int32 Code, FString Message)
	{
		for (auto& Job : *Jobs)
		{
			auto _ = Job->OnError.ExecuteIfBound(Code, Message);
		}
	}));

	return true;
}

void ServerGameTelemetry::SendProtectedEvents(TArray<FAccelByteModelsTelemetryBody> Events, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/protected/events"), *SettingsRef.GameTelemetryServerUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (const auto& Event : Events)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		JsonObject->SetStringField("EventNamespace", Event.EventNamespace);
		JsonObject->SetStringField("EventName", Event.EventName);
		JsonObject->SetObjectField("Payload", Event.Payload);
		JsonObject->SetStringField("EventTimestamp", Event.EventTimestamp.ToIso8601());

		JsonArray.Add(MakeShared<FJsonValueObject>(JsonObject));
	}
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonArray, Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} 
}