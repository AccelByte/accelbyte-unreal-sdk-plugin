// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGameTelemetryApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
GameTelemetry::GameTelemetry(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
	, ShuttingDown(false)
{
}

GameTelemetry::~GameTelemetry()
{
	Shutdown();
}

void GameTelemetry::SetBatchFrequency(FTimespan Interval)
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

void GameTelemetry::SetImmediateEventList(TArray<FString> const& EventNames)
{
	ImmediateEvents = TSet<FString>(EventNames);
}

void GameTelemetry::Send(FAccelByteModelsTelemetryBody TelemetryBody, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	if (ShuttingDown)
	{
		return;
	}

	FReport::Log(FString(__FUNCTION__));

	if(TelemetryBody.EventTimestamp.GetTicks() == 0)
	{
		TelemetryBody.EventTimestamp = FDateTime::UtcNow();
	}

	if (ImmediateEvents.Contains(TelemetryBody.EventName))
	{
		SendProtectedEvents({ TelemetryBody }, OnSuccess, OnError);
	}
	else
	{
		JobQueue.Enqueue(TTuple<FAccelByteModelsTelemetryBody, FVoidHandler, FErrorHandler>{ TelemetryBody, OnSuccess, OnError });
		if (bTelemetryJobStarted == false)
		{
			bTelemetryJobStarted = true;
			GameTelemetryTickDelegate = FTickerDelegate::CreateRaw(this, &GameTelemetry::PeriodicTelemetry);
			GameTelemetryTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(GameTelemetryTickDelegate, static_cast<float>(TelemetryInterval.GetTotalSeconds()));
		}
	}
}

void GameTelemetry::Flush()
{
	PeriodicTelemetry(0);
}

void GameTelemetry::Startup()
{
	ShuttingDown = false;
}

void GameTelemetry::Shutdown()
{
	ShuttingDown = true;
	if(UObjectInitialized())
	{
		if (GameTelemetryTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(GameTelemetryTickDelegateHandle);
			GameTelemetryTickDelegateHandle.Reset();
		}
		// flush events
		Flush();
	}
}

bool GameTelemetry::PeriodicTelemetry(float DeltaTime)
{
	if (!JobQueue.IsEmpty())
	{
		FReport::Log(FString(__FUNCTION__));

		TArray<FAccelByteModelsTelemetryBody> TelemetryBodies;
		TArray<FVoidHandler> OnSuccessCallbacks;
		TArray<FErrorHandler> OnErrorCallbacks;
		while (!JobQueue.IsEmpty())
		{
			TTuple<FAccelByteModelsTelemetryBody, FVoidHandler, FErrorHandler> DequeueResult;
			if (JobQueue.Dequeue(DequeueResult))
			{
				TelemetryBodies.Add(DequeueResult.Get<0>());
				OnSuccessCallbacks.Add(DequeueResult.Get<1>());
				OnErrorCallbacks.Add(DequeueResult.Get<2>());
			}
		}

		SendProtectedEvents(
			TelemetryBodies,
			FVoidHandler::CreateLambda(
				[OnSuccessCallbacks]()
				{
					for (auto& OnSuccessCallback : OnSuccessCallbacks)
					{
						OnSuccessCallback.ExecuteIfBound();
					}
				}),
			FErrorHandler::CreateLambda(
				[OnErrorCallbacks](int32 Code, FString Message)
				{
					for (auto& OnErrorCallback : OnErrorCallbacks)
					{
						OnErrorCallback.ExecuteIfBound(Code, Message);
					}
				}));
	}
	return true;
}

void GameTelemetry::SendProtectedEvents(TArray<FAccelByteModelsTelemetryBody> Events, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	if (ShuttingDown)
	{
		return;
	}

	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/protected/events"), *SettingsRef.GameTelemetryServerUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (auto const& Event : Events)
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