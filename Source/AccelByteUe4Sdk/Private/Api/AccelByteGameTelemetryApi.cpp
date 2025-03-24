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
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, nullptr)
	, CredentialsRef{InCredentialsRef.AsShared()}
	, ShuttingDown(false)
	, bCacheEvent(bInCacheEvent)
	, bRetryOnFailed(bInRetryOnFailed)
{
	bCacheEvent = SettingsRef.bEnableGameTelemetryCache;
}

GameTelemetry::GameTelemetry(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient
	, bool bInCacheEvent
	, bool bInRetryOnFailed)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
	, CredentialsRef{InCredentialsRef.AsShared()}
	, ShuttingDown(false)
	, bCacheEvent(bInCacheEvent)
	, bRetryOnFailed(bInRetryOnFailed)
{
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
		GameTelemetryWPtr GameTelemetryWeak = AsShared();
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [GameTelemetryWeak]()
			{
				const auto GameTelemetryApi = GameTelemetryWeak.Pin();
				if (GameTelemetryApi.IsValid())
				{
					GameTelemetryApi->LoadCachedEvents();
				}
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
		SendProtectedEvents({ MakeShared<FAccelByteModelsTelemetryBody, ESPMode::ThreadSafe>(TelemetryBody) }, OnSuccess, OnError);
	}
	else
	{
		TelemetryBodyPtr TelemetryPtr = MakeShared<FAccelByteModelsTelemetryBody, ESPMode::ThreadSafe>(TelemetryBody);
		JobQueue.Enqueue(TTuple<TelemetryBodyPtr, FVoidHandler, FErrorHandler>{ TelemetryPtr, OnSuccess, OnError });
		if (bCacheEvent || CriticalEvents.Contains(TelemetryBody.EventName))
		{
			GameTelemetryWPtr GameTelemetryWeak = AsShared();
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [TelemetryPtr, GameTelemetryWeak]()
				{
					const auto GameTelemetryApi = GameTelemetryWeak.Pin();
					if (GameTelemetryApi.IsValid())
					{
						GameTelemetryApi->AppendEventToCache(TelemetryPtr);
					}
				});
		}
		if (bTelemetryJobStarted == false)
		{
			bTelemetryJobStarted = true;
			GameTelemetryTickDelegate = FTickerDelegate::CreateThreadSafeSP(AsShared(), &GameTelemetry::PeriodicTelemetry);
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
	GameTelemetryLoginSuccess = CredentialsRef->OnLoginSuccess().AddThreadSafeSP(AsShared(), &GameTelemetry::OnLoginSuccess);
	GameTelemetryLogoutSuccess = CredentialsRef->OnLogoutSuccess().AddThreadSafeSP(AsShared(), &GameTelemetry::OnLogoutSuccess);
	ShuttingDown = false;
}

void GameTelemetry::Shutdown()
{
	if(UObjectInitialized() && !ShuttingDown)
	{
		ShuttingDown = true;
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
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelBytePeriodicTelemetry"));
#endif
	if (!JobQueue.IsEmpty())
	{
		FReport::Log(FString(__FUNCTION__));

		TArray<TelemetryBodyPtr> TelemetryBodies;
		TArray<FVoidHandler> OnSuccessCallbacks;
		TArray<FErrorHandler> OnErrorCallbacks;
		while (!JobQueue.IsEmpty())
		{
			TTuple<TelemetryBodyPtr, FVoidHandler, FErrorHandler> DequeueResult;
			if (JobQueue.Dequeue(DequeueResult))
			{
				TelemetryBodies.Add(DequeueResult.Get<0>());
				OnSuccessCallbacks.Add(DequeueResult.Get<1>());
				OnErrorCallbacks.Add(DequeueResult.Get<2>());
			}
		}
		GameTelemetryWPtr GameTelemetryWeak = AsShared();
		SendProtectedEvents(TelemetryBodies
			, FVoidHandler::CreateLambda(
				[GameTelemetryWeak, OnSuccessCallbacks, TelemetryBodies]()
				{
					AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [TelemetryBodies, GameTelemetryWeak]()
						{
							const auto GameTelemetryApi = GameTelemetryWeak.Pin();
							if (GameTelemetryApi.IsValid())
							{
								GameTelemetryApi->RemoveEventsFromCache(TelemetryBodies);
							}
						});
					for (auto& OnSuccessCallback : OnSuccessCallbacks)
					{
						OnSuccessCallback.ExecuteIfBound();
					}
				})
			, FErrorHandler::CreateLambda(
				[GameTelemetryWeak, OnSuccessCallbacks, OnErrorCallbacks, TelemetryBodies](int32 Code, FString Message)
				{
					const auto GameTelemetryApi = GameTelemetryWeak.Pin();
					if (GameTelemetryApi.IsValid() 
						&& GameTelemetryApi->bRetryOnFailed 
						&& Code != (int32)ErrorCodes::StatusUnprocessableEntity)
					{
						for (int i = 0; i < TelemetryBodies.Num(); i++)
						{
							GameTelemetryApi->JobQueue.Enqueue(TTuple<TelemetryBodyPtr, FVoidHandler, FErrorHandler>
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

void GameTelemetry::SendProtectedEvents(TArray<TelemetryBodyPtr> const& Events
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{

	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/protected/events")
		, *SettingsRef.GameTelemetryServerUrl);

	FString ClientTimestamp;
	const FApiClientPtr ApiClientPtr = ApiClient.Pin();
	if(ApiClientPtr.IsValid())
	{
		FAccelByteTimeManagerPtr TimeManagerPtr = ApiClientPtr->GetTimeManager().Pin();
		if(TimeManagerPtr.IsValid())
		{
			ClientTimestamp = TimeManagerPtr->GetCurrentServerTime().ToIso8601();
		}
	}

	FString Content = TEXT("");
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (auto const& Event : Events)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		JsonObject->SetStringField("EventNamespace", Event->EventNamespace);
		JsonObject->SetStringField("EventName", Event->EventName);
		JsonObject->SetObjectField("Payload", Event->Payload);
		JsonObject->SetStringField("ClientTimestamp", ClientTimestamp);
		JsonObject->SetStringField("FlightId", FAccelByteUtilities::GetFlightId());
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

	GameTelemetryWPtr GameTelemetryWeak = AsShared();
	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->GetItem(TelemetryKey
		, THandler<TPair<FString, FString>>::CreateLambda(
			[GameTelemetryWeak](TPair<FString, FString> Pair)
			{
				if (Pair.Key.IsEmpty() || Pair.Value.IsEmpty())
				{
					return;
				}
				const auto GameTelemetryApi = GameTelemetryWeak.Pin();
				TArray<TelemetryBodyPtr> EventList;
				if (GameTelemetryApi.IsValid()
					&& GameTelemetryApi->EventsJsonToArray(Pair.Value, EventList))
				{
					GameTelemetryApi->SendProtectedEvents(EventList
						, FVoidHandler::CreateLambda(
							[GameTelemetryWeak, EventList]()
							{
								AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [EventList, GameTelemetryWeak]()
									{
										const auto GameTelemetryApi = GameTelemetryWeak.Pin();
										if (GameTelemetryApi.IsValid())
										{
											GameTelemetryApi->RemoveEventsFromCache(EventList);
										}
									});
							})
						, FErrorHandler::CreateLambda([GameTelemetryWeak, EventList](int32 Code, FString Message)
							{
								const auto GameTelemetryApi = GameTelemetryWeak.Pin();
								if (GameTelemetryApi.IsValid()
									&& GameTelemetryApi->bRetryOnFailed 
									&& Code != (int32)ErrorCodes::StatusUnprocessableEntity)
								{
									for (int i = 0; i < EventList.Num(); i++)
									{
										GameTelemetryApi->JobQueue.Enqueue(TTuple<TelemetryBodyPtr, FVoidHandler, FErrorHandler>
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
void GameTelemetry::AppendEventToCache(TelemetryBodyPtr Telemetry)
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
void GameTelemetry::RemoveEventsFromCache(TArray<TelemetryBodyPtr> const& Events)
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
	TArray<TelemetryBodyPtr> TempEventPtrArray;
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
	, TArray<TelemetryBodyPtr>& OutArray)
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
		OutArray.Add(MakeShared<FAccelByteModelsTelemetryBody, ESPMode::ThreadSafe>(TelemetryBody));
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

void GameTelemetry::ShouldCacheEnabled(bool bShouldCache)
{
	bCacheEvent = bShouldCache;
}

bool GameTelemetry::IsCacheEnabled() const
{
	return bCacheEvent;
}

} 
}