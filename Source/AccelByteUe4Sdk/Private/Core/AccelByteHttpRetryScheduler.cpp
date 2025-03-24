// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"

#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteUtilities.h"

#include "Dom/JsonObject.h"
#include <algorithm>

DEFINE_LOG_CATEGORY(LogAccelByteHttpRetry);

using namespace std;

namespace AccelByte
{

int FHttpRetryScheduler::InitialDelay = 1;
int FHttpRetryScheduler::MaximumDelay = 30;
int FHttpRetryScheduler::TotalTimeout = 60;
int FHttpRetryScheduler::PauseTimeout = 60;
int FHttpRetryScheduler::RateLimit = FHttpRetryScheduler::DefaultRateLimit;

FString FHttpRetryScheduler::HeaderNamespace = TEXT("");
FString FHttpRetryScheduler::HeaderSDKVersion = TEXT("");
FString FHttpRetryScheduler::HeaderOSSVersion = TEXT("");
FString FHttpRetryScheduler::HeaderGameClientVersion = TEXT("");

TMap<EHttpResponseCodes::Type, FHttpRetryScheduler::FHttpResponseCodeHandler> FHttpRetryScheduler::ResponseCodeDelegates{};

FHttpRetryScheduler::FHttpRetryScheduler()
	: TaskQueue()
{}

FHttpRetryScheduler::FHttpRetryScheduler(const SettingsWPtr InSettingsWeak)
: TaskQueue()
, SettingsWeak(InSettingsWeak)
{
	const SettingsPtr Settings = SettingsWeak.Pin();
	if(Settings.IsValid())
	{
		HttpCache.SetCacheType(Settings->HttpCacheType);
	}
}

FHttpRetryScheduler::~FHttpRetryScheduler()
{
	TaskQueue.Empty();
	RequestsBucket.Empty();
}

void FHttpRetryScheduler::InitializeRateLimit()
{
	FAccelByteUtilities::LoadABConfigFallback(TEXT("HTTP"), TEXT("RateLimit"), RateLimit);
	if (RateLimit <= 0)
	{
		RateLimit = DefaultRateLimit;
	}
}

FAccelByteTaskPtr FHttpRetryScheduler::ProcessRequest
	( FHttpRequestPtr Request
	, FHttpRequestCompleteDelegate const& CompleteDelegate
	, double RequestTime )
{
	FAccelByteTaskPtr Task(nullptr);
	if (State == EState::ShuttingDown)
	{
		UE_LOG(LogAccelByteHttpRetry, Warning, TEXT("Cannot process request, HTTP Retry Scheduler is SHUTTING DOWN"));
		return Task;
	}
	if (State == EState::Uninitialized)
	{
		UE_LOG(LogAccelByteHttpRetry, Warning, TEXT("Cannot process request, HTTP Retry Scheduler is UNINITIALIZED"));
		return Task;
	}
	if (!FAccelByteNetUtilities::IsValidUrl(Request->GetURL()))
	{
		UE_LOG(LogAccelByteHttpRetry, Warning, TEXT("Cannot process request, Request URL (%s) is not valid"), *Request->GetURL());
		CompleteDelegate.ExecuteIfBound(Request, Request->GetResponse(), false);
		return Task;
	}

	FReport::LogHttpRequest(Request);

	// AccelByteTracing track http request 
	ACCELBYTE_SERVICE_LOGGING_HTTP_REQUEST(Request);

	Task = MakeShared<FHttpRetryTask, ESPMode::ThreadSafe>
		( Request
		, CompleteDelegate
		, RequestTime
		, InitialDelay
		, AsShared()
		, FHttpRetryScheduler::ResponseCodeDelegates );

	FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(Task));

	//Http header
	Request->SetHeader("Namespace", HeaderNamespace);
	Request->SetHeader("Game-Client-Version", HeaderGameClientVersion);
	Request->SetHeader("AccelByte-SDK-Version", HeaderSDKVersion);
	Request->SetHeader("AccelByte-OSS-Version", HeaderOSSVersion);

	if (State == EState::Paused && Request->GetHeader("Authorization").Contains("Bearer"))
	{
		HttpRetryTaskPtr->Pause();
	}
	else
	{
		FHttpResponsePtr CachedResponse;
		SettingsPtr Settings = SettingsWeak.Pin();
		if (Settings.IsValid() && Settings->bEnableHttpCache && HttpCache.TryRetrieving(Request, CachedResponse))
		{
			HttpRetryTaskPtr->FinishFromCached(CachedResponse);
		}
		else
		{
			uint32 AvailableToken = RateLimit;
			double ResetTokenTime = RequestTime + 1.0f; //Reset every second
			bool bCancelRequest = false;
			RequestBucketLock.Lock();
			if (FRequestBucket* LastRequest = RequestsBucket.Find(Request->GetURL()))
			{
				if (RequestTime < LastRequest->ResetTokenTime)
				{
					if (LastRequest->AvailableToken <= 0)
					{
						UE_LOG(LogAccelByteHttpRetry, Warning, TEXT("Cannot process request, rate limit reached %s"), *Request->GetURL());
						Task->Cancel();
						bCancelRequest = true;
					}
					else
					{
						--LastRequest->AvailableToken;
					}
				}
				else
				{
					LastRequest->AvailableToken = --AvailableToken;
					LastRequest->ResetTokenTime = ResetTokenTime;
				}
			}
			else
			{
				RequestsBucket.Emplace(Request->GetURL(), FRequestBucket{ --AvailableToken, ResetTokenTime});
			}
			RequestBucketLock.Unlock();
			if (!bCancelRequest)
			{
				Task->Start();
			}
		}

	}
	TaskQueue.Enqueue(Task);

	return Task;
}

FAccelByteTaskPtr FHttpRetryScheduler::ProcessRequest
	(FHttpRequestPtr Request
	, TSharedPtr<FJsonObject> Content
	, const FHttpRequestCompleteDelegate& CompleteDelegate
	, double RequestTime
	, bool bOmitBlankValues)
{
	if (Request->GetVerb().Equals(TEXT("GET"), ESearchCase::IgnoreCase) || !Content.IsValid())
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("JSON object is not valid!"));
		return ProcessRequest(Request, CompleteDelegate, RequestTime);
	}

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4)
	FAccelByteUtilities::FixupAllJsonKeys(Content, FAccelByteUtilities::JsonKeyToActualValue);
#endif

	FString JSONString = ParseUStructToJsonString(Content, bOmitBlankValues);

	Request->SetContentAsString(JSONString);

	return ProcessRequest(Request, CompleteDelegate, RequestTime);
}

void FHttpRetryScheduler::SetBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected)
{
	AddBearerAuthRejectedDelegate(BearerAuthRejected);
}

FDelegateHandle FHttpRetryScheduler::AddBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected)
{
	FScopeLock Lock(&LockBearerAuthRejected);
	return BearerAuthRejectedMulticast.Add(BearerAuthRejected);
}

bool FHttpRetryScheduler::RemoveBearerAuthRejectedDelegate(FDelegateHandle const& BearerAuthRejectedHandle)
{
	return BearerAuthRejectedMulticast.Remove(BearerAuthRejectedHandle);
}

FDelegateHandle FHttpRetryScheduler::AddBearerAuthRefreshedDelegate(FBearerAuthRefreshed const& BearerAuthRefreshed)
{
	FScopeLock Lock(&LockBearerAuthRefreshed);
	return BearerAuthRefreshedMulticast.Add(BearerAuthRefreshed);
}

bool FHttpRetryScheduler::RemoveBearerAuthRefreshedDelegate(FDelegateHandle const& BearerAuthRefreshedHandle)
{
	return BearerAuthRefreshedMulticast.Remove(BearerAuthRefreshedHandle);
}

void FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode, FHttpResponseCodeHandler const& Handler)
{
	FHttpRetryScheduler::ResponseCodeDelegates.Emplace(StatusCode, Handler);
}

bool FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode)
{
	bool bResult = false;

	if (FHttpRetryScheduler::ResponseCodeDelegates.Contains(StatusCode))
	{
		FHttpRetryScheduler::ResponseCodeDelegates.Remove(StatusCode);
		bResult = true;
	}
	return bResult;
}

FString FHttpRetryScheduler::ParseUStructToJsonString(const TSharedPtr<FJsonObject>& JsonObject, bool bOmitBlankValues)
{
	FString JsonString;

	if (!JsonObject.IsValid())
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HttpClient Request UStructToJsonObject failed!"));

		return JsonString;
	}

	// Omit blank JSON string values if opted into by the caller
	if (bOmitBlankValues)
	{
		FAccelByteUtilities::RemoveEmptyStrings(JsonObject);
	}

	// Finally, write the JSON object to a string
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HttpClient Request FJsonSerializer::Serialize failed!"));

		return JsonString;
	}

	return JsonString;
}

void FHttpRetryScheduler::BearerAuthRejected()
{
	if (State != EState::Paused)
	{
		HttpCache.ClearCache();
		BearerAuthRejectedMulticast.Broadcast();
	}
}

void FHttpRetryScheduler::PauseBearerAuthRequest()
{
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTP Retry Scheduler PAUSED"));
	State = EState::Paused;
	TQueue<FAccelByteTaskPtr, EQueueMode::Spsc> TempQueue;
	
	while (FAccelByteTaskPtr *TaskPtr = TaskQueue.Peek())
	{
		FAccelByteTaskPtr Task = *TaskPtr;
		
		TaskQueue.Pop();
		Task->Pause();
		TempQueue.Enqueue(Task);
	}

	while (FAccelByteTaskPtr *TaskPtr = TempQueue.Peek())
	{
		FAccelByteTaskPtr Task = *TaskPtr;
		
		TempQueue.Pop();
		TaskQueue.Enqueue(Task);
	}
}
  
void FHttpRetryScheduler::ResumeBearerAuthRequest(const FString& AccessToken)
{
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTP Retry Scheduler RESUME"));
	if (BearerAuthRefreshedMulticast.IsBound()) 
	{
		BearerAuthRefreshedMulticast.Broadcast(AccessToken);
		BearerAuthRefreshedMulticast.Clear();
	}

	if (State == EState::Paused) 
	{
		State = EState::Initialized;
	}
}

bool FHttpRetryScheduler::PollRetry(double Time)
{
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelBytePollRetryScheduler"));
#endif
	if (TaskQueue.IsEmpty())
	{
		return false;
	}

	TArray<FAccelByteTaskPtr> RemovedTasks;
	bool Loop = true;
	do
	{
		FAccelByteTaskPtr *TaskPtr = TaskQueue.Peek();
		if (TaskPtr != nullptr)
		{
			FAccelByteTaskPtr Task = *TaskPtr;

			if (Task->Time() < Time)
			{
				TaskQueue.Pop();

				bool WillBeRemoved = false;
				Task->Tick(Time);

				switch (Task->State())
				{
				case EAccelByteTaskState::Completed:
				case EAccelByteTaskState::Cancelled:
				case EAccelByteTaskState::Failed:
					RemovedTasks.Add(Task);
					WillBeRemoved = true;
					break;
				default:
					break;
				}

				if (!WillBeRemoved)
				{
					TaskQueue.Enqueue(Task);
				}
			}
			else
			{
				Loop = false;
			}
		}
		else
		{
			Loop = false;
		}
	} while (Loop);

	const SettingsPtr Settings = SettingsWeak.Pin();
	for (auto& Task : RemovedTasks)
	{
		if (Settings.IsValid() && Settings->bEnableHttpCache && Task->State() == EAccelByteTaskState::Completed)
		{
			FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(Task));
			HttpCache.TryStoring(HttpRetryTaskPtr->GetHttpRequest());
		}
		
		Task->Finish();
	}

	return true;
}

void FHttpRetryScheduler::Startup()
{
	if(State == EState::Initialized)
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTP Retry Scheduler is initialized, skipping startup process"));
		return;
	}
	
	InitializeRateLimit();
	
	PollRetryHandle = FTickerAlias::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateThreadSafeSP(AsShared(), &FHttpRetryScheduler::Tick),
        0.2f);

	State = EState::Initialized;
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTP Retry Scheduler has been INITIALIZED"));
}

void FHttpRetryScheduler::Shutdown()
{
	State = EState::ShuttingDown;

	if (PollRetryHandle.IsValid())
	{
		// Core ticker by this point in engine shutdown has already been torn down - only remove ticker if this is not an engine shutdown
		if (!IsEngineExitRequested())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(PollRetryHandle);
		}
		PollRetryHandle.Reset();
	}

	// flush http requests
	if (!TaskQueue.IsEmpty())
	{
		// Don't flush if we're on the game thread, and we're not exiting, as it causes stutters.
		if (IsInGameThread() && !IsEngineExitRequested())
		{
			TaskQueue.Empty();
			return;
		}

		double MaxFlushTimeSeconds = -1.0;
		GConfig->GetDouble(TEXT("HTTP"), TEXT("MaxFlushTimeSeconds"), MaxFlushTimeSeconds, GEngineIni);

		if (MaxFlushTimeSeconds <= 0)
		{
			UE_LOG(LogAccelByteHttpRetry, Log, TEXT("HTTP MaxFlushTimeSeconds is not configured, it may prevent the shutdown, until all requests flushed"));
		}

		// try flush once
#if (ENGINE_MAJOR_VERSION==5 && ENGINE_MINOR_VERSION>=3)
		FHttpModule::Get().GetHttpManager().Shutdown();
#elif (ENGINE_MAJOR_VERSION==5 && ENGINE_MINOR_VERSION>=0)
		FHttpModule::Get().GetHttpManager().Flush(EHttpFlushReason::Shutdown);
#else
		FHttpModule::Get().GetHttpManager().Flush(true);
#endif
		FHttpModule::Get().GetHttpManager().Tick(0);

		// cancel unfinished http requests, so don't hinder the shutdown
		TaskQueue.Empty();
	}
}

bool FHttpRetryScheduler::Tick(float DeltaTime)
{
	PollRetry(FPlatformTime::Seconds());
	return true;
}

}
