// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGameTelemetryApi.h"
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Api
{
	
GameTelemetry::GameTelemetry(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, bool bInCacheEvent
	, bool bInRetryOnFailed)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, CredentialsRef{InCredentialsRef.AsShared()}
	, ShuttingDown(false)
	, bCacheEvent(bInCacheEvent)
	, bRetryOnFailed(bInRetryOnFailed)
{
	GameTelemetryLoginSuccess = CredentialsRef->OnLoginSuccess().AddRaw(this, &GameTelemetry::OnLoginSuccess);
	GameTelemetryLogoutSuccess = CredentialsRef->OnLogoutSuccess().AddRaw(this, &GameTelemetry::OnLogoutSuccess);
	bCacheEvent = SettingsRef.bEnableGameTelemetryCache;
}

GameTelemetry::~GameTelemetry()
{
	Shutdown();
}

void GameTelemetry::OnLoginSuccess(FOauth2Token const& Response)
{
	if (bCacheEvent)
	{
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]()
			{
				LoadCachedEvents();
			});
	}
}

void GameTelemetry::OnLogoutSuccess()
{
	EventPtrArrayAccumulation.Empty();
	EventPtrArray.Empty();
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
	if (bTelemetryJobStarted)
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(GameTelemetryTickDelegateHandle);
		GameTelemetryTickDelegateHandle.Reset();
		GameTelemetryTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(GameTelemetryTickDelegate, static_cast<float>(TelemetryInterval.GetTotalSeconds()));
	}
}

void GameTelemetry::SetImmediateEventList(TArray<FString> const& EventNames)
{
	ImmediateEvents = TSet<FString>(EventNames);
}

void GameTelemetry::SetCriticalEventList(TArray<FString> const& EventNames)
{
	CriticalEvents = TSet<FString>(EventNames);
}

void GameTelemetry::Send(FAccelByteModelsTelemetryBody TelemetryBody
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (ShuttingDown)
	{
		return;
	}

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
		SendProtectedEvents({ MakeShared<FAccelByteModelsTelemetryBody>(TelemetryBody) }, OnSuccess, OnError);
	}
	else
	{
		TSharedPtr<FAccelByteModelsTelemetryBody> TelemetryPtr = MakeShared<FAccelByteModelsTelemetryBody>(TelemetryBody);
		JobQueue.Enqueue(TTuple<TSharedPtr<FAccelByteModelsTelemetryBody>, FVoidHandler, FErrorHandler>{ TelemetryPtr, OnSuccess, OnError });
		if (bCacheEvent || CriticalEvents.Contains(TelemetryBody.EventName))
		{
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [TelemetryPtr, this]()
				{
					AppendEventToCache(TelemetryPtr);
				});
		}
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
	if (bTelemetryJobStarted)
	{
		PeriodicTelemetry(0);
	}
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
		
		if (GameTelemetryLoginSuccess.IsValid())
		{
			CredentialsRef->OnLoginSuccess().Remove(GameTelemetryLoginSuccess);
		}

		if (GameTelemetryLogoutSuccess.IsValid())
		{
			CredentialsRef->OnLogoutSuccess().Remove(GameTelemetryLogoutSuccess);
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

		TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> TelemetryBodies;
		TArray<FVoidHandler> OnSuccessCallbacks;
		TArray<FErrorHandler> OnErrorCallbacks;
		while (!JobQueue.IsEmpty())
		{
			TTuple<TSharedPtr<FAccelByteModelsTelemetryBody>, FVoidHandler, FErrorHandler> DequeueResult;
			if (JobQueue.Dequeue(DequeueResult))
			{
				TelemetryBodies.Add(DequeueResult.Get<0>());
				OnSuccessCallbacks.Add(DequeueResult.Get<1>());
				OnErrorCallbacks.Add(DequeueResult.Get<2>());
			}
		}

		SendProtectedEvents(TelemetryBodies
			, FVoidHandler::CreateLambda(
				[this, OnSuccessCallbacks, TelemetryBodies]()
				{
					AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [TelemetryBodies, this]()
						{
							RemoveEventsFromCache(TelemetryBodies);
						});
					for (auto& OnSuccessCallback : OnSuccessCallbacks)
					{
						OnSuccessCallback.ExecuteIfBound();
					}
				})
			, FErrorHandler::CreateLambda(
				[this, OnSuccessCallbacks, OnErrorCallbacks, TelemetryBodies](int32 Code, FString Message)
				{

					if (bRetryOnFailed && Code != (int32)ErrorCodes::StatusUnprocessableEntity)
					{
						for (int i = 0; i < TelemetryBodies.Num(); i++)
						{
							JobQueue.Enqueue(TTuple<TSharedPtr<FAccelByteModelsTelemetryBody>, FVoidHandler, FErrorHandler>
							{ 
								TelemetryBodies[i], 
								i < OnSuccessCallbacks.Num() ? OnSuccessCallbacks[i] : FVoidHandler{}, 
								i < OnErrorCallbacks.Num() ? OnErrorCallbacks[i] : FErrorHandler{} 
							});
						}
					}
					else
					{
						for (auto& OnErrorCallback : OnErrorCallbacks)
						{
							OnErrorCallback.ExecuteIfBound(Code, Message);
						}
					}
				}));
	}
	return true;
}

void GameTelemetry::SendProtectedEvents(TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> const& Events
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{

	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/protected/events")
		, *SettingsRef.GameTelemetryServerUrl);

	FString Content = TEXT("");
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (auto const& Event : Events)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		JsonObject->SetStringField("EventNamespace", Event->EventNamespace);
		JsonObject->SetStringField("EventName", Event->EventName);
		JsonObject->SetObjectField("Payload", Event->Payload);
		JsonObject->SetStringField("ClientTimestamp", FAccelByteUtilities::GetCurrentServerTime().ToIso8601());
		JsonObject->SetStringField("flightId", FAccelByteUtilities::GetFlightId());
		JsonObject->SetStringField("DeviceType", FAccelByteUtilities::GetPlatformName());

		JsonArray.Add(MakeShared<FJsonValueObject>(JsonObject));
	}
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonArray, Writer);

	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	if (ShuttingDown && !bCacheEvent)
	{
		if (CredentialsRef->IsSessionValid())
		{
			HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, FVoidHandler{}, FErrorHandler{});
		}
	}
	else
	{
		HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
	}
}

//Should be called from async task
void GameTelemetry::LoadCachedEvents()
{
	FString TelemetryKey = GetTelemetryKey();
	if (TelemetryKey.IsEmpty())
	{
		return;
	}

	bCacheUpdated = true;

	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->GetItem(TelemetryKey
		, THandler<TPair<FString, FString>>::CreateLambda(
			[this](TPair<FString, FString> Pair)
			{
				if (Pair.Key.IsEmpty() || Pair.Value.IsEmpty())
				{
					return;
				}
				TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> EventList;
				if (EventsJsonToArray(Pair.Value, EventList))
				{
					SendProtectedEvents(EventList
						, FVoidHandler::CreateLambda(
							[this, EventList]()
							{
								AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [EventList, this]()
									{
										RemoveEventsFromCache(EventList);
									});
							})
						, FErrorHandler::CreateLambda([this, EventList](int32 Code, FString Message)
							{
								if (bRetryOnFailed && Code != (int32)ErrorCodes::StatusUnprocessableEntity)
								{
									for (int i = 0; i < EventList.Num(); i++)
									{
										JobQueue.Enqueue(TTuple<TSharedPtr<FAccelByteModelsTelemetryBody>, FVoidHandler, FErrorHandler>
										{ EventList[i], FVoidHandler{}, FErrorHandler{} });
									}
								}
							})
					);
				}
			})
		, FAccelByteUtilities::GetCacheFilenameTelemetry());
}

//Should be called from async task
void GameTelemetry::AppendEventToCache(TSharedPtr<FAccelByteModelsTelemetryBody> Telemetry)
{
	FString TelemetryKey = GetTelemetryKey();
	if (TelemetryKey.IsEmpty())
	{
		return;
	}
	FScopeLock ScopeLock(&EventPtrArrayLock);
	int AddedIndex = EventPtrArray.Add(Telemetry);
	
	//Mimicking GameTelemetry::JobArrayQueueAsJsonString
	{
		//Create new array if empty, or open the end of the bracket
		if (EventPtrArrayAccumulation.IsEmpty())
		{
			EventPtrArrayAccumulation += "{\"telemetry\":[";
		}
		else
		{
			bool bCharRemoved = false;
			EventPtrArrayAccumulation.RemoveFromEnd("]}");
			EventPtrArrayAccumulation += ",";
		}
	
		{
			TSharedRef<FJsonObject> JsonObj = MakeShared<FJsonObject>();
			JsonObj->SetStringField("EventName", EventPtrArray[AddedIndex]->EventName);
			JsonObj->SetStringField("EventNamespace", EventPtrArray[AddedIndex]->EventNamespace);
			JsonObj->SetObjectField("Payload", EventPtrArray[AddedIndex]->Payload);
			JsonObj->SetNumberField("ClientTimestamp", EventPtrArray[AddedIndex]->ClientTimestamp.ToUnixTimestamp());
			TSharedRef<FJsonValueObject> JsonValue = MakeShared<FJsonValueObject>(JsonObj);
			FString WrittenResult{};
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&WrittenResult);
			if (FJsonSerializer::Serialize(JsonObj, Writer))
			{
				EventPtrArrayAccumulation+=(WrittenResult);
			}
			EventPtrArrayAccumulation += "]}";
		}
	}

	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->SaveItemOverwiteEntireFile(TelemetryKey
		, EventPtrArrayAccumulation
		, THandler<bool>::CreateLambda([](bool IsSuccess){})
		, FAccelByteUtilities::GetCacheFilenameTelemetry());

	bCacheUpdated = true;
}

//Should be called from async task
void GameTelemetry::RemoveEventsFromCache(TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> const& Events)
{
	FString TelemetryKey = GetTelemetryKey();
	if (TelemetryKey.IsEmpty())
	{
		return;
	}

	if (!bCacheUpdated)
	{
		return;
	}

	if (EventPtrArray.Num() != 0)
	{
		FScopeLock ScopeLock(&EventPtrArrayLock);
		for (const auto& Event : Events)
		{
			EventPtrArray.Remove(Event);
		}
	}
	EventPtrArrayAccumulation.Empty();

	FString TelemetryValues;
	JobArrayQueueAsJsonString(TelemetryValues);
	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->SaveItem(TelemetryKey
		, TelemetryValues
		, THandler<bool>::CreateLambda([](bool IsSuccess) {})
		, FAccelByteUtilities::GetCacheFilenameTelemetry());

	bCacheUpdated = false;
}

bool GameTelemetry::JobArrayQueueAsJsonString(FString& OutJsonString)
{
	TSharedRef<FJsonObject> TelemetryObj = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> EventsObjArray;
	TArray<TSharedPtr<FAccelByteModelsTelemetryBody>> TempEventPtrArray;
	{
		FScopeLock ScopeLock(&EventPtrArrayLock);
		TempEventPtrArray = EventPtrArray;
	}
	for (auto& EventPtr : TempEventPtrArray)
	{
		if (EventPtr.IsValid())
		{
			TSharedPtr<FJsonObject> JsonObj = MakeShared<FJsonObject>();
			JsonObj->SetStringField("EventName", EventPtr->EventName);
			JsonObj->SetStringField("EventNamespace", EventPtr->EventNamespace);
			JsonObj->SetObjectField("Payload", EventPtr->Payload);
			JsonObj->SetNumberField("ClientTimestamp", EventPtr->ClientTimestamp.ToUnixTimestamp());
			TSharedRef<FJsonValueObject> JsonValue = MakeShared<FJsonValueObject>(JsonObj);
			EventsObjArray.Add(JsonValue);
		}
	}
	TelemetryObj->SetArrayField("telemetry", EventsObjArray);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	if (!FJsonSerializer::Serialize(TelemetryObj, Writer))
	{
		return false;
	}
	return true;
}

const FString GameTelemetry::GetEventNamespace()
{
	if (EventNamespace.IsEmpty())
	{
		EventNamespace = CredentialsRef->GetNamespace();
	}
	return EventNamespace;
}

bool GameTelemetry::EventsJsonToArray(FString& InJsonString
	, TArray<TSharedPtr<FAccelByteModelsTelemetryBody>>& OutArray)
{
	TSharedPtr<FJsonObject> JsonObjectPtr{ MakeShared<FJsonObject>() };

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(InJsonString), JsonObjectPtr))
	{
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> const* ArrayValue;
	if (!JsonObjectPtr->TryGetArrayField(TEXT("telemetry"), ArrayValue))
	{
		return false;
	}

	if (ArrayValue->Num() == 0)
	{
		return false;
	}

	for (auto& ArrayItem : *ArrayValue)
	{
		auto& JsonObj = ArrayItem->AsObject();
		FString EventName;
		TSharedPtr<FJsonObject> const* Payload = nullptr;
		int32 ClientTimestamp = 0;
		FAccelByteModelsTelemetryBody TelemetryBody;
		TelemetryBody.EventName = JsonObj->GetStringField(TEXT("EventName"));
		TelemetryBody.EventNamespace = JsonObj->GetStringField(TEXT("EventNamespace"));
		TelemetryBody.Payload = JsonObj->GetObjectField(TEXT("Payload"));
		ClientTimestamp = JsonObj->GetIntegerField(TEXT("ClientTimestamp"));
		TelemetryBody.ClientTimestamp = FDateTime::FromUnixTimestamp(ClientTimestamp);
		OutArray.Add(MakeShared<FAccelByteModelsTelemetryBody>(TelemetryBody));
	}
	return true;
}

FString GameTelemetry::GetTelemetryKey()
{
	FString UserId = *CredentialsRef->GetUserId();
	if (UserId.IsEmpty())
	{
		return UserId;
	}
	FString TelemetryKey = FString::Printf(TEXT("TELEMETRY_%s"), *UserId);
	return TelemetryKey;
}

} 
}