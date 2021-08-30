// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteRegistry.h"
#include <algorithm>

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpRetry, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpRetry);

using namespace std;

namespace AccelByte
{

const int FHttpRetryScheduler::InitialDelay = 1;
const int FHttpRetryScheduler::MaximumDelay = 30;
const int FHttpRetryScheduler::TotalTimeout = 60;

class FHttpRetryTask : public FAccelByteTask
{
public:
	FHttpRetryTask(const FHttpRequestPtr& HttpRequest, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay);
	virtual ~FHttpRetryTask();

	virtual bool Start() override;
	virtual bool Cancel() override;
	virtual void Tick(double CurrentTime) override;
	virtual bool Finish() override;
private:
	const FHttpRequestPtr Request;
	const FHttpRequestCompleteDelegate CompleteDelegate;
	const double RequestTime;
	double NextRetryTime;
	float NextDelay;
	bool ScheduledRetry;

	EAccelByteTaskState Retry();
	EAccelByteTaskState ScheduleNextRetry();
	bool IsFinished();
};

FHttpRetryTask::FHttpRetryTask(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay)
	: FAccelByteTask()
	, Request(Request)
	, CompleteDelegate(CompleteDelegate)
	, RequestTime(RequestTime)
	, NextRetryTime(NextDelay)
	, NextDelay(NextDelay)
	, ScheduledRetry(false)
{

}

FHttpRetryTask::~FHttpRetryTask()
{

}

bool FHttpRetryTask::Start()
{
	if (!Request.IsValid()) 
	{
		TaskState = EAccelByteTaskState::Failed;
		return false;
	}

	Request->ProcessRequest();
	TaskState = EAccelByteTaskState::Running;
	return FAccelByteTask::Start();
}

bool FHttpRetryTask::Cancel()
{
	if (!Request.IsValid())
	{
		TaskState = EAccelByteTaskState::Failed;
		return false;
	}

	Request->CancelRequest();
	TaskState = EAccelByteTaskState::Cancelled;
	return FAccelByteTask::Cancel();
}

void FHttpRetryTask::Tick(double CurrentTime)
{
	if (TaskState == EAccelByteTaskState::Completed || TaskState == EAccelByteTaskState::Cancelled || TaskState == EAccelByteTaskState::Failed)
	{
		return;
	}

	EAccelByteTaskState NextState = TaskState;
	
	if (!Request.IsValid())
	{
		NextState = EAccelByteTaskState::Failed;
		return;
	}

	TaskTime = CurrentTime;

	if (ScheduledRetry)
	{
		if (TaskTime >= NextRetryTime)
		{
			NextState = Retry();
		}
		else
		{
			return;
		}
	}

	switch (Request->GetStatus())
	{
	case EHttpRequestStatus::Processing:
		if (TaskTime >= RequestTime + FHttpRetryScheduler::TotalTimeout)
		{
			Cancel();
		}
		break;
	case EHttpRequestStatus::Succeeded: //got response
		switch (Request->GetResponse()->GetResponseCode())
		{
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::BadGateway:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
			if (TaskTime < RequestTime + FHttpRetryScheduler::TotalTimeout)
			{
				NextState = ScheduleNextRetry();
			}
			else
			{
				NextState = EAccelByteTaskState::Completed;
			}
			break;
		default:
			NextState = EAccelByteTaskState::Completed;
			break;
		}
		break;
	case EHttpRequestStatus::Failed_ConnectionError: //network error
		if (TaskTime < RequestTime + FHttpRetryScheduler::TotalTimeout)
		{
			NextState = ScheduleNextRetry();
		}
	case EHttpRequestStatus::NotStarted:
		if (TaskTime >= RequestTime + FHttpRetryScheduler::TotalTimeout)
		{
			NextState = EAccelByteTaskState::Completed;
		}
		break;
	case EHttpRequestStatus::Failed: //request cancelled
		NextState = EAccelByteTaskState::Completed;
	default:
		break;
	}

	TaskState = NextState;
}

bool FHttpRetryTask::Finish()
{
	if (TaskState == EAccelByteTaskState::Running)
	{
		Cancel();
	}

	if (TaskState == EAccelByteTaskState::Completed)
	{
		FReport::LogHttpResponse(Request, Request->GetResponse());
		CompleteDelegate.ExecuteIfBound(Request, Request->GetResponse(), IsFinished());
	}
	return FAccelByteTask::Finish();
}

EAccelByteTaskState FHttpRetryTask::Retry()
{
	ScheduledRetry = false;
	if (Start())
	{
		FReport::LogHttpRequest(Request);
	}
	return TaskState;
}

EAccelByteTaskState FHttpRetryTask::ScheduleNextRetry()
{
	NextDelay *= 2;
	NextDelay += FMath::RandRange(-NextDelay, NextDelay) / 4;

	if (NextDelay > FHttpRetryScheduler::MaximumDelay)
	{
		NextDelay = FHttpRetryScheduler::MaximumDelay;
	}

	NextRetryTime = TaskTime + NextDelay;

	if (NextRetryTime > RequestTime + FHttpRetryScheduler::TotalTimeout)
	{
		NextRetryTime = RequestTime + FHttpRetryScheduler::TotalTimeout;
	}

	ScheduledRetry = true;
	return EAccelByteTaskState::Pending;
}

bool FHttpRetryTask::IsFinished()
{
	return Request->GetStatus() == EHttpRequestStatus::Failed ||
		Request->GetStatus() == EHttpRequestStatus::Failed_ConnectionError ||
		Request->GetStatus() == EHttpRequestStatus::Succeeded;
}

FAccelByteTaskPtr FHttpRetryScheduler::ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime)
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
	Task = MakeShared<FHttpRetryTask, ESPMode::ThreadSafe>(Request, CompleteDelegate, RequestTime, InitialDelay);
	Task->Start();

	TaskQueue.Enqueue(Task);

	return Task.ToSharedRef();
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

	for (auto& Task : CompletedTasks)
	{
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
}

}
