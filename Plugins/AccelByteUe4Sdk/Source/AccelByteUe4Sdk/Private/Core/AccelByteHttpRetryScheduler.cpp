// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteHttpRetryScheduler.h"
#include <algorithm>

using namespace std;

namespace AccelByte
{

namespace HttpRequest
{
	bool IsFinished(const FHttpRequestPtr& Request)
	{
		return Request->GetStatus() == EHttpRequestStatus::Failed ||
			Request->GetStatus() == EHttpRequestStatus::Failed_ConnectionError ||
			Request->GetStatus() == EHttpRequestStatus::Succeeded;
	}
}

const int FHttpRetryScheduler::InitialDelay = 1;
const int FHttpRetryScheduler::MaximumDelay = 30;
const int FHttpRetryScheduler::TotalTimeout = 60;

FHttpRetryScheduler::FHttpRetryTask::FHttpRetryTask(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay)
	: Request(Request)
	, CompleteDelegate(CompleteDelegate)
	, RequestTime(RequestTime)
	, NextDelay(NextDelay)
	, NextRetryTime(RequestTime + NextDelay)
{
}

bool FHttpRetryScheduler::ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime)
{
	bool bIsStarted = Request->ProcessRequest();

	if (bIsStarted)
	{
		RetryList.Add(MakeShared<FHttpRetryTask>(Request, CompleteDelegate, RequestTime, InitialDelay));
	}

	return bIsStarted;
}

bool FHttpRetryScheduler::PollRetry(double CurrentTime, Credentials& UserCredentials)
{
	if (RetryList.Num() == 0)
	{
		return false;
	}

	TArray<TSharedRef<FHttpRetryTask>> CompletedTasks;

	RetryList.RemoveAll([CurrentTime, &UserCredentials, &CompletedTasks](const TSharedRef<FHttpRetryTask>& CurrentTask)
	{
		if (!CurrentTask->Request.IsValid())
		{
			return true;
		}

		switch (CurrentTask->Request->GetStatus())
		{
		case EHttpRequestStatus::Processing:
			if (CurrentTime >= CurrentTask->RequestTime + FHttpRetryScheduler::TotalTimeout)
			{
				CurrentTask->Request->CancelRequest();
			}
			else if (CurrentTime >= CurrentTask->NextRetryTime)
			{
				CurrentTask->ScheduleNextRetry(CurrentTime);
			}

			return false;
		case EHttpRequestStatus::Succeeded: //got response
			switch (CurrentTask->Request->GetResponse()->GetResponseCode())
			{
			case EHttpResponseCodes::ServerError:
			case EHttpResponseCodes::BadGateway:
			case EHttpResponseCodes::ServiceUnavail:
			case EHttpResponseCodes::GatewayTimeout:
				CurrentTask->ScheduleNextRetry(CurrentTime);
				CurrentTask->Request->ProcessRequest();

				return false;
			case EHttpResponseCodes::Forbidden:
				UserCredentials.ScheduleRefreshToken(CurrentTime);
				CompletedTasks.Add(CurrentTask);

				return true;
			default:
				CompletedTasks.Add(CurrentTask);

				return true;
			}
		case EHttpRequestStatus::Failed: //request cancelled
		case EHttpRequestStatus::Failed_ConnectionError: //network error
		case EHttpRequestStatus::NotStarted:
			CompletedTasks.Add(CurrentTask);

			return true;
		}

		return false;
	});

	for (const auto& Task : CompletedTasks)
	{
		Task->CompleteDelegate.ExecuteIfBound(Task->Request, Task->Request->GetResponse(), Task->Request->GetResponse().IsValid());
	}

	return true;
}

void FHttpRetryScheduler::FHttpRetryTask::ScheduleNextRetry(double CurrentTime)
{
	NextDelay *= 2;
	NextDelay += FMath::RandRange(-NextDelay, NextDelay) / 4;

	if (NextDelay > FHttpRetryScheduler::MaximumDelay)
	{
		NextDelay = FHttpRetryScheduler::MaximumDelay;
	}

	NextRetryTime = CurrentTime + NextDelay;

	if (NextRetryTime > RequestTime + FHttpRetryScheduler::TotalTimeout)
	{
		NextRetryTime = RequestTime + FHttpRetryScheduler::TotalTimeout;
	}
}

}