// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
	FHttpRetryTask::FHttpRetryTask(FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay, const FVoidHandler& OnBearerAuthRejectDelegate, FBearerAuthRejectedRefresh& BearerAuthRejectedRefresh)
		: FAccelByteTask()
		, Request(Request)
		, CompleteDelegate(CompleteDelegate)
		, RequestTime(RequestTime)
		, NextRetryTime(NextDelay)
		, NextDelay(NextDelay)
		, ScheduledRetry(false)
		, OnBearerAuthRejectDelegate(OnBearerAuthRejectDelegate)
		, BearerAuthRejectedRefresh(BearerAuthRejectedRefresh)
		, BearerAuthRejectedRefreshHandle()
		, bIsBeenRunFromPause(false)
	{

	}

	FHttpRetryTask::~FHttpRetryTask()
	{
		BearerAuthRejectedRefresh.Remove(BearerAuthRejectedRefreshHandle);
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
		if (TaskState != EAccelByteTaskState::Paused)
		{
			BearerAuthRejectedRefreshHandle = BearerAuthRejectedRefresh.Add(THandler<FString>::CreateLambda([&](const FString& AccessToken) 
			{
				BearerAuthUpdated(AccessToken);
			}));
			OnBearerAuthRejectDelegate.ExecuteIfBound();
		}

		return EAccelByteTaskState::Paused;
	}

	void FHttpRetryTask::Tick(double CurrentTime)
	{
		FAccelByteTask::Tick(CurrentTime);

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
#if !UE_SERVER
					if (IsRefreshable())
					{
						NextState = Pause();
						break;
					}
#endif
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

	bool FHttpRetryTask::FinishFromCached(const FHttpResponsePtr& Response)
	{
		TaskState = EAccelByteTaskState::Completed;
	
		FReport::LogHttpResponse(Request, Response);
		CompleteDelegate.ExecuteIfBound(Request, Response, true /*IsFinished()*/);

		return FAccelByteTask::Finish();
	}

	void FHttpRetryTask::BearerAuthUpdated(const FString& AccessToken)
	{
		bIsBeenRunFromPause = true;

		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("BearerAuthUpdated"));
		if (AccessToken.IsEmpty())
		{
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Bearer token is empty, Task is canceled"));
			Cancel();
			return;
		}
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

	bool FHttpRetryTask::IsRefreshable()
	{
		bool bIsRefreshable = false;
		FString Authorization = Request->GetHeader("Authorization");
		Authorization = Authorization.TrimStartAndEnd();
		if (Authorization.Contains("Bearer", ESearchCase::IgnoreCase) 
			&& !Authorization.Equals("Bearer", ESearchCase::IgnoreCase)
			&& !bIsBeenRunFromPause)
		{
			bIsRefreshable = true;
		}
		return bIsRefreshable;
	}

}
