// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteUtilities.h"

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

typedef FHttpRetryScheduler::FBearerAuthRejectedRefresh FBearerAuthRejectedRefresh;

FHttpRetryScheduler::FHttpRetryScheduler()
	: TaskQueue()
{}

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

	FReport::LogHttpRequest(Request);

	FVoidHandler OnBearerAuthReject = FVoidHandler::CreateLambda([&]() {BearerAuthRejected(); });

	Task = MakeShared<FHttpRetryTask, ESPMode::ThreadSafe>
		( Request
		, CompleteDelegate
		, RequestTime
		, InitialDelay
		, OnBearerAuthReject
		, BearerAuthRejectedRefresh
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
		if (UAccelByteBlueprintsSettings::IsHttpCacheEnabled() && HttpCache.TryRetrieving(Request, CachedResponse))
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

void FHttpRetryScheduler::SetBearerAuthRejectedDelegate(FBearerAuthRejected BearerAuthRejected)
{
	if (BearerAuthRejectedDelegate.IsBound()) 
	{
		BearerAuthRejectedDelegate.Unbind();
	}

	BearerAuthRejectedDelegate = BearerAuthRejected;
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

void FHttpRetryScheduler::BearerAuthRejected()
{
	if (State != EState::Paused)
	{
		HttpCache.ClearCache();
		BearerAuthRejectedDelegate.ExecuteIfBound();
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
	if (BearerAuthRejectedRefresh.IsBound()) 
	{
		BearerAuthRejectedRefresh.Broadcast(AccessToken);
		BearerAuthRejectedRefresh.Clear();
	}

	if (State == EState::Paused) 
	{
		State = EState::Initialized;
	}
}

bool FHttpRetryScheduler::PollRetry(double Time)
{
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

	const bool bIsHttpCacheEnabled = UAccelByteBlueprintsSettings::IsHttpCacheEnabled();
	for (auto& Task : RemovedTasks)
	{
		if (bIsHttpCacheEnabled && Task->State() == EAccelByteTaskState::Completed)
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
	InitializeRateLimit();
	
	PollRetryHandle = FTickerAlias::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateLambda([this](float DeltaTime)
        {
            PollRetry(FPlatformTime::Seconds());

            return true;
        }),
        0.2f);

	State = EState::Initialized;
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTP Retry Scheduler has been INITIALIZED"));

	HttpCache.ClearCache();
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

}
