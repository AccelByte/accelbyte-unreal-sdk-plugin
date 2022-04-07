// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryTask.h"
#include <algorithm>

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpRetry, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpRetry);

using namespace std;

namespace AccelByte
{

const int FHttpRetryScheduler::InitialDelay = 1;
const int FHttpRetryScheduler::MaximumDelay = 30;
const int FHttpRetryScheduler::TotalTimeout = 60;

typedef FHttpRetryScheduler::FBearerAuthRejectedRefresh FBearerAuthRejectedRefresh;

FHttpRetryScheduler::FHttpRetryScheduler()
	: TaskQueue()
{}

FHttpRetryScheduler::~FHttpRetryScheduler()
{}

FAccelByteTaskPtr FHttpRetryScheduler::ProcessRequest(FHttpRequestPtr Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime)
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

	Task = MakeShared<FHttpRetryTask, ESPMode::ThreadSafe>(Request, CompleteDelegate, RequestTime, InitialDelay, OnBearerAuthReject, BearerAuthRejectedRefresh);

	FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(Task));
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
			Task->Start();
		}

	}
	TaskQueue.Enqueue(Task);

	return Task.ToSharedRef();
}

void FHttpRetryScheduler::SetBearerAuthRejectedDelegate(FBearerAuthRejected BearerAuthRejected)
{
	if (BearerAuthRejectedDelegate.IsBound()) 
	{
		BearerAuthRejectedDelegate.Unbind();
	}

	BearerAuthRejectedDelegate = BearerAuthRejected;
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

	TArray<FAccelByteTaskPtr> CompletedTasks;
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
					CompletedTasks.Add(Task);
				case EAccelByteTaskState::Failed:
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
	for (auto& Task : CompletedTasks)
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
	PollRetryHandle = FTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateLambda([this](float DeltaTime)
        {
            PollRetry(FPlatformTime::Seconds());

            return true;
        }),
        0.2f);

	State = EState::Initialized;
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTP Retry Scheduler has been INITIALIZED"));
}

void FHttpRetryScheduler::Shutdown()
{
	State = EState::ShuttingDown;

	if (PollRetryHandle.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(PollRetryHandle);
		PollRetryHandle.Reset();
	}

	// flush http requests
	if (!TaskQueue.IsEmpty())
	{
		double MaxFlushTimeSeconds = -1.0;
		GConfig->GetDouble(TEXT("HTTP"), TEXT("MaxFlushTimeSeconds"), MaxFlushTimeSeconds, GEngineIni);

		if (MaxFlushTimeSeconds <= 0)
		{
			UE_LOG(LogAccelByteHttpRetry, Log, TEXT("HTTP MaxFlushTimeSeconds is not configured, it may prevent the shutdown, until all requests flushed"));
		}

		// try flush once
		FHttpModule::Get().GetHttpManager().Flush(true);
		FHttpModule::Get().GetHttpManager().Tick(0);

		// cancel unfinished http requests, so don't hinder the shutdown
		TaskQueue.Empty();
	};

	HttpCache.ClearCache();
}

}
