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

typedef FHttpRetryScheduler::FBearerAuthRejectedRefresh FBearerAuthRejectedRefresh;

class FHttpRetryTask : public FAccelByteTask
{

public:
	FHttpRetryTask(const FHttpRequestPtr& HttpRequest, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay, const FVoidHandler& OnBearerAuthRejectDelegate, FBearerAuthRejectedRefresh& BearerAuthRejectedRefresh);
	virtual ~FHttpRetryTask();

	virtual bool Start() override;
	virtual bool Cancel() override;
	virtual void Tick(double CurrentTime) override;
	virtual bool Finish() override;

	EAccelByteTaskState Pause();

private:
	const FHttpRequestPtr Request;
	const FHttpRequestCompleteDelegate CompleteDelegate;
	const double RequestTime;
	double NextRetryTime;
	float NextDelay;
	bool ScheduledRetry;
	const FVoidHandler OnBearerAuthRejectDelegate;
	FBearerAuthRejectedRefresh& BearerAuthRejectedRefresh;
	bool bIsBeenRunFromPause;

	void BearerAuthUpdated(const FString& AccessToken);
	bool CheckRetry(EAccelByteTaskState& Out);
	EAccelByteTaskState Retry();
	EAccelByteTaskState ScheduleNextRetry();
	bool IsFinished();
};

FHttpRetryTask::FHttpRetryTask(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay, const FVoidHandler& OnBearerAuthRejectDelegate, FBearerAuthRejectedRefresh& BearerAuthRejectedRefresh)
	: FAccelByteTask()
	, Request(Request)
	, CompleteDelegate(CompleteDelegate)
	, RequestTime(RequestTime)
	, NextRetryTime(NextDelay)
	, NextDelay(NextDelay)
	, ScheduledRetry(false)
	, OnBearerAuthRejectDelegate(OnBearerAuthRejectDelegate)
	, BearerAuthRejectedRefresh(BearerAuthRejectedRefresh)
	, bIsBeenRunFromPause(false)
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

EAccelByteTaskState FHttpRetryTask::Pause()
{
	if (bIsBeenRunFromPause) 
	{
		return EAccelByteTaskState::Completed;
	}
	
	if (TaskState != EAccelByteTaskState::Paused)
	{
		OnBearerAuthRejectDelegate.ExecuteIfBound();
		BearerAuthRejectedRefresh.Add(THandler<FString>::CreateLambda([&](const FString& AccessToken) 
			{
				BearerAuthUpdated(AccessToken);
			}));
	}

	return EAccelByteTaskState::Paused;
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

	if (ScheduledRetry && TaskState != EAccelByteTaskState::Paused)
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
		
	EHttpRequestStatus::Type RequestStatus = Request->GetStatus();
	switch (RequestStatus)
	{
	case EHttpRequestStatus::Processing:
		if (TaskTime >= RequestTime + FHttpRetryScheduler::TotalTimeout) 
		{
			Cancel();
		}
		break;
	case EHttpRequestStatus::Succeeded: //got response
	{
		const FHttpResponsePtr Response = Request->GetResponse();
		if (Response.IsValid())
		{
			int32 ResponseCode = Response->GetResponseCode();
			switch (ResponseCode)
			{
			case EHttpResponseCodes::ServerError:
			case EHttpResponseCodes::BadGateway:
			case EHttpResponseCodes::ServiceUnavail:
			case EHttpResponseCodes::GatewayTimeout:
				if (!CheckRetry(NextState)) 
				{
					NextState = EAccelByteTaskState::Completed;
				}
				break;
			case EHttpResponseCodes::Denied:
				if (Request->GetHeader("Authorization").Contains("Bearer")) 
				{
					NextState = Pause();
				}
				break;
			default:
				NextState = EAccelByteTaskState::Completed;
				break;
			}
		}
		else 
		{
			NextState = EAccelByteTaskState::Completed;
		}
		break;
	}
	case EHttpRequestStatus::Failed_ConnectionError: //network error
		CheckRetry(NextState);
		break;
	case EHttpRequestStatus::Failed: //request cancelled
		NextState = EAccelByteTaskState::Cancelled;
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

	if (TaskState == EAccelByteTaskState::Completed || TaskState == EAccelByteTaskState::Cancelled)
	{
		FReport::LogHttpResponse(Request, Request->GetResponse());
		CompleteDelegate.ExecuteIfBound(Request, Request->GetResponse(), IsFinished());
	}
	return FAccelByteTask::Finish();
}

void FHttpRetryTask::BearerAuthUpdated(const FString& AccessToken)
{
	bIsBeenRunFromPause = true;

	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("BearerAuthUpdated"));
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	ScheduledRetry = true;

	TaskState = EAccelByteTaskState::Pending;
	NextRetryTime = FPlatformTime::Seconds();
	ScheduledRetry = true;
}

bool FHttpRetryTask::CheckRetry(EAccelByteTaskState& Out)
{
	bool WillRetry = false;

	if (TaskTime < RequestTime + FHttpRetryScheduler::TotalTimeout)
	{
		Out = ScheduleNextRetry();
		WillRetry = true;
	}

	return WillRetry;
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

FHttpRetryScheduler::FHttpRetryScheduler()
	: TaskQueue()
{}

FHttpRetryScheduler::~FHttpRetryScheduler()
{}

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

	FVoidHandler OnBearerAuthReject = FVoidHandler::CreateLambda([&]() {BearerAuthRejected(); });

	Task = MakeShared<FHttpRetryTask, ESPMode::ThreadSafe>(Request, CompleteDelegate, RequestTime, InitialDelay, OnBearerAuthReject, BearerAuthRejectedRefresh);

	if (State == EState::Paused && Request->GetHeader("Authorization").Contains("Bearer"))
	{
		StaticCastSharedPtr<FHttpRetryTask>(Task)->Pause();
	}
	else 
	{
		Task->Start();
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
	if (BearerAuthRejectedRefresh.IsBound()) {
		BearerAuthRejectedRefresh.Broadcast(AccessToken);
		BearerAuthRejectedRefresh.Clear();
	}
	State = EState::Initialized;
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
