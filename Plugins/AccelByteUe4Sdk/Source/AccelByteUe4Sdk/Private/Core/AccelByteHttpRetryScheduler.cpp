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
		RetryList.push_back(MakeShared<FHttpRetryTask>(Request, CompleteDelegate, RequestTime, InitialDelay));
	}

	return bIsStarted;
}

bool FHttpRetryScheduler::PollRetry(double CurrentTime, Credentials& UserCredentials)
{
	if (RetryList.empty())
	{
		return false;
	}

	auto EraseBegin = remove_if(begin(RetryList), end(RetryList),
		[this, &CurrentTime, &UserCredentials](const TSharedRef<FHttpRetryTask>& CurrentTask)
	{
		if (!CurrentTask->Request.IsValid())
		{
			return true;
		}

		if (!HttpRequest::IsFinished(CurrentTask->Request))
		{
			if (CurrentTime < CurrentTask->NextRetryTime)
			{
				return false;
			}
			else if (CurrentTime >= CurrentTask->RequestTime + TotalTimeout)
			{
				CurrentTask->Request->CancelRequest();

				return false;
			}
		}

		switch (CurrentTask->Request->GetStatus())
		{
		case EHttpRequestStatus::Processing:
			ScheduleNextRetry(CurrentTask, CurrentTime);

			return false;
		case EHttpRequestStatus::Succeeded: //got response
			switch (CurrentTask->Request->GetResponse()->GetResponseCode())
			{
			case EHttpResponseCodes::ServerError:
			case EHttpResponseCodes::BadGateway:
			case EHttpResponseCodes::ServiceUnavail:
			case EHttpResponseCodes::GatewayTimeout:
				ScheduleNextRetry(CurrentTask, CurrentTime);
				CurrentTask->Request->ProcessRequest();

				return false;
			case EHttpResponseCodes::Forbidden:
				UserCredentials.ScheduleRefreshToken(CurrentTime);
				CurrentTask->CompleteDelegate.ExecuteIfBound(CurrentTask->Request, CurrentTask->Request->GetResponse(), true);

				return true;
			default:
				CurrentTask->CompleteDelegate.ExecuteIfBound(CurrentTask->Request, CurrentTask->Request->GetResponse(), true);

				return true;
			}
		case EHttpRequestStatus::Failed: //request cancelled
		case EHttpRequestStatus::Failed_ConnectionError: //network error
			CurrentTask->CompleteDelegate.ExecuteIfBound(CurrentTask->Request, nullptr, false);

			return true;

		default:
			return false;
		}
	});

	RetryList.erase(EraseBegin, end(RetryList));
	
	return true;
}

void FHttpRetryScheduler::ScheduleNextRetry(TSharedPtr<FHttpRetryTask> Task, double CurrentTime)
{
	Task->NextDelay *= 2;
	Task->NextDelay += FMath::RandRange(-Task->NextDelay, Task->NextDelay) / 4;

	if (Task->NextDelay > MaximumDelay)
	{
		Task->NextDelay = MaximumDelay;
	}

	Task->NextRetryTime = CurrentTime + Task->NextDelay;

	if (Task->NextRetryTime > Task->RequestTime + TotalTimeout)
	{
		Task->NextRetryTime = Task->RequestTime + TotalTimeout;
	}
}

}