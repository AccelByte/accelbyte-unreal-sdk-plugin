// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
FHttpRetryTask::FHttpRetryTask(
	FHttpRequestPtr& InRequest,
	FHttpRequestCompleteDelegate const& InCompleteDelegate,
	double InRequestTime,
	double InNextDelay,
	FHttpRetrySchedulerWPtr const& InHttpRetrySchedulerWPtr,
	TMap<EHttpResponseCodes::Type, FHttpRetryScheduler::FHttpResponseCodeHandler> HandlerDelegates)
	: CompleteDelegate{ InCompleteDelegate }
	, HttpRetrySchedulerWPtr{ InHttpRetrySchedulerWPtr }
	, Request{ InRequest }
	, RequestTime{ InRequestTime }
	, PauseTime{}
	, PauseDuration{}
	, NextRetryTime{ InNextDelay }
	, NextDelay{ InNextDelay }
{
	TaskTime.store(RequestTime, std::memory_order_release);
	InitializeDefaultDelegates();
	for (auto& Handler : HandlerDelegates)
	{
		ResponseCodeDelegates.Emplace(Handler.Key, Handler.Value);
	}
}

FHttpRetryTask::~FHttpRetryTask()
{
	auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
	if (HttpRetrySchedulerPtr.IsValid())
	{
		{
			FWriteScopeLock Lock(BearerAuthRejectedHandleMtx);
			if(BearerAuthRejectedHandle.IsValid())
			{
				HttpRetrySchedulerPtr->RemoveBearerAuthRejectedDelegate(BearerAuthRejectedHandle);
				BearerAuthRejectedHandle.Reset();
			}
		}
		{
			FWriteScopeLock Lock(BearerAuthRefreshedHandleMtx);
			if (BearerAuthRefreshedHandle.IsValid())
			{
				HttpRetrySchedulerPtr->RemoveBearerAuthRefreshedDelegate(BearerAuthRefreshedHandle);
				BearerAuthRefreshedHandle.Reset();
			}
		}
	}

#if UE_BUILD_SHIPPING

	if (!ensure(Request.IsValid()))
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Error during HTTP Retry Task Destructor: Request is invalid"));
		return;
	}

	const EAccelByteTaskState CurrentTaskState = GetTaskState();
	const bool bIsTaskStateFinishProperly = (CurrentTaskState != EAccelByteTaskState::Completed) && (CurrentTaskState != EAccelByteTaskState::Failed) && (CurrentTaskState != EAccelByteTaskState::Cancelled);
	if (ensure(bIsTaskStateFinishProperly))
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Error during HTTP Retry Task Destructor: Task state is not ready to proceed"));
		return;
	}
#endif

	if (Request.IsValid())
	{
		if (Request->OnProcessRequestComplete().IsBound())
		{
			Request->OnProcessRequestComplete().Unbind();
		}

		if (Request->OnRequestWillRetry().IsBound())
		{
			Request->OnRequestWillRetry().Unbind();
		}

		if (Request->GetStatus() != EHttpRequestStatus::Succeeded)
		{
			Request->CancelRequest();
		}
	}
}

bool FHttpRetryTask::Start()
{
	if (!Request.IsValid())
	{
		SetTaskState(EAccelByteTaskState::Failed);
		return false;
	}

	{
		FWriteScopeLock Lock(ResponseMtx);
		if (Response.IsValid())
		{
			Response.Reset();
		}
	}

	if (Request->OnProcessRequestComplete().IsBound())
	{
		Request->OnProcessRequestComplete().Unbind();
	}
	Request->OnProcessRequestComplete().BindThreadSafeSP(this, &FHttpRetryTask::OnProcessRequestComplete);

	auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
	if (HttpRetrySchedulerPtr.IsValid())
	{
		FWriteScopeLock Lock(BearerAuthRejectedHandleMtx);
		if (BearerAuthRejectedHandle.IsValid())
		{
			HttpRetrySchedulerPtr->RemoveBearerAuthRejectedDelegate(BearerAuthRejectedHandle);
			BearerAuthRejectedHandle.Reset();
		}
		BearerAuthRejectedHandle = HttpRetrySchedulerPtr->AddBearerAuthRejectedDelegate(
			FHttpRetryScheduler::FBearerAuthRejected::CreateThreadSafeSP(AsShared(), &FHttpRetryTask::OnBearerAuthRejected));
	}

	Request->ProcessRequest();
	SetTaskState(EAccelByteTaskState::Running);
	return FAccelByteTask::Start();
}

bool FHttpRetryTask::Cancel()
{
	if (!Request.IsValid())
	{
		SetTaskState(EAccelByteTaskState::Failed);
		return false;
	}

	Request->CancelRequest();
	SetTaskState(EAccelByteTaskState::Cancelled);

	return FAccelByteTask::Cancel();
}

EAccelByteTaskState FHttpRetryTask::Pause()
{
	SetTaskState(EAccelByteTaskState::Paused);
	PauseTime.store(TaskTime.load(std::memory_order_acquire), std::memory_order_release);
	return FAccelByteTask::Pause();
}

TMap<FString, FString> FHttpRetryTask::GetResponseHeader()
{
	FString HeaderContent;
	{
		FReadScopeLock Lock(ResponseMtx);
		if(!Request.IsValid() || !Response.IsValid())
		{
			return TMap<FString, FString>();
		}
		HeaderContent = Response->GetHeader(HEADER_FEATURE_FLAG);
	}

	/***	Header feature flag value format Per 20 Sept 2024
		*		https://accelbyte.atlassian.net/wiki/spaces/JCP/pages/3410165897/Feature+Flags+HTTP+Response+Header+Specification
		*		consist of key=value pair
		*		Keys cannot contain uppercase characters. Keys and values are separated by "=" (without whitespace)
		*			- example key1=value1
		*		Multiple pairs are separated by comma followed by space ", "
		*			- example multi values: key1=value1, key2=value2
		*		The Value will be URL encoded to avoid mismatch the separator and other special characters
		*			- example key1=a,b,c, key2=http://localhost 
		*			- will become key1=a%2Cb%2Cc, key2=http%3A%2F%2Flocalhost
	***/

	TArray<FString> FeatureFlagArray;
	if(HeaderContent.ParseIntoArray(FeatureFlagArray, TEXT(", "), true) == 0)
	{
		return TMap<FString, FString>();
	}

	TMap<FString, FString> Result;
	for (const FString& FlagString : FeatureFlagArray)
	{
		TArray<FString> KeyVal;
		if(FlagString.ParseIntoArray(KeyVal, TEXT("=")) >= 2)
		{
			FString DecodedVal = FPlatformHttp::UrlDecode(KeyVal[1]);
			Result.Emplace(KeyVal[0], KeyVal[1]);
		}
	}
	return Result;
}

void FHttpRetryTask::Tick(double CurrentTime)
{
	FAccelByteTask::Tick(CurrentTime);
	auto CurrentTaskState = GetTaskState();
	if (CurrentTaskState == EAccelByteTaskState::Completed 
		|| CurrentTaskState == EAccelByteTaskState::Cancelled 
		|| CurrentTaskState == EAccelByteTaskState::Failed)
	{
		return;
	}

	EAccelByteTaskState NextState = CurrentTaskState;
	
	if (!Request.IsValid())
	{
		SetTaskState(EAccelByteTaskState::Failed);
		return;
	}
	
	CurrentTaskState = GetTaskState();
	TaskTime.store(CurrentTime, std::memory_order_release);
	if (CurrentTaskState == EAccelByteTaskState::Paused)
	{
		double DeltaTime = TaskTime.load(std::memory_order_acquire) - PauseTime.load(std::memory_order_acquire);
		double PauseDurationBuf = PauseDuration.load(std::memory_order_acquire);
		PauseDurationBuf += DeltaTime;
		PauseDuration.store(PauseDurationBuf, std::memory_order_release);

		PauseTime.store(TaskTime.load(std::memory_order_acquire), std::memory_order_release);
		if (PauseDurationBuf > FHttpRetryScheduler::PauseTimeout)
		{
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTPRetryTask pause timeout of %d seconds reached! Cancelling..."), FHttpRetryScheduler::PauseTimeout);
			Cancel();
		}
		return;
	}
	else if (CurrentTaskState == EAccelByteTaskState::Retrying)
	{
		if (TaskTime.load(std::memory_order_acquire) < NextRetryTime.load(std::memory_order_acquire))
		{
			return;
		}
		NextState = Retry();
	}
		
	const EHttpRequestStatus::Type RequestStatus = Request->GetStatus();
	switch (RequestStatus)
	{
	case EHttpRequestStatus::Processing: //Do nothing, request is still processing
		break;
	case EHttpRequestStatus::Succeeded: //got response
	{
		NextState = EAccelByteTaskState::Completed;
		FHttpResponsePtr CurrentResponse;
		{
			FReadScopeLock Lock(ResponseMtx);
			if (Response.IsValid())
			{
				CurrentResponse = Response;
			}
		}
		if (CurrentResponse.IsValid())
		{
			const int32 ResponseCode = CurrentResponse->GetResponseCode();
			if (ResponseCodeDelegates.Contains(ResponseCode))
			{
				NextState = ResponseCodeDelegates[ResponseCode].Execute(ResponseCode);
			}

			if (NextState == EAccelByteTaskState::Pending 
				|| NextState == EAccelByteTaskState::Running 
				|| NextState == EAccelByteTaskState::Retrying)
			{
				if (!CheckRetry(NextState))
				{
					NextState = EAccelByteTaskState::Completed;
				}
			}
		}
	}break;
	// Whatever the failure reason, try to retry. HTTP 4xx doesn't count as failure from http client perspective.
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
	case EHttpRequestStatus::Failed:
	{
		SetTaskState(EAccelByteTaskState::Failed);
		NextState = GetTaskState();
		CheckRetry(NextState);
	}break;
#else
	case EHttpRequestStatus::Failed_ConnectionError: //network error
	case EHttpRequestStatus::Failed: //request failed
	{
		SetTaskState(EAccelByteTaskState::Failed);
		NextState = GetTaskState();
		CheckRetry(NextState);
	}break;
#endif
	default:
		break;
	}
	SetTaskState(NextState);
}

bool FHttpRetryTask::Finish()
{
	FString State = FAccelByteUtilities::GetUEnumValueAsString(GetTaskState());
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTPRetryTask finished with status: %s"), *State);

	if (GetTaskState() == EAccelByteTaskState::Running)
	{
		Cancel();
	}
	auto CurrentTaskState = GetTaskState();
	if (CurrentTaskState == EAccelByteTaskState::Completed 
		|| CurrentTaskState == EAccelByteTaskState::Cancelled 
		|| CurrentTaskState == EAccelByteTaskState::Failed)
	{
		FHttpResponsePtr CurrentResponse;
		{
			FReadScopeLock Lock(ResponseMtx);
			CurrentResponse = Response;
		}
		FReport::LogHttpResponse(Request, CurrentResponse);
		CompleteDelegate.ExecuteIfBound(Request, CurrentResponse, IsFinished());
	}

	return FAccelByteTask::Finish();
}

bool FHttpRetryTask::FinishFromCached(const FHttpResponsePtr& InResponse)
{
	SetTaskState(EAccelByteTaskState::Completed);
	{
		FWriteScopeLock Lock(ResponseMtx);
		Response = InResponse;
	}
	bIsResponseFromCache.store(true, std::memory_order_release);
	return FAccelByteTask::Finish();
}

void FHttpRetryTask::InitializeDefaultDelegates()
{
	ResponseCodeDelegates = {
		{
			EHttpResponseCodes::TooManyRequests,
			FHttpRetryScheduler::FHttpResponseCodeHandler::CreateRaw(this, &FHttpRetryTask::HandleDefaultRetry)
		},
		{
			EHttpResponseCodes::RetryWith,
			FHttpRetryScheduler::FHttpResponseCodeHandler::CreateRaw(this, &FHttpRetryTask::HandleDefaultRetry)
		},
		{
			EHttpResponseCodes::ServerError,
			FHttpRetryScheduler::FHttpResponseCodeHandler::CreateRaw(this, &FHttpRetryTask::HandleDefaultRetry)
		},
		{
			EHttpResponseCodes::BadGateway,
			FHttpRetryScheduler::FHttpResponseCodeHandler::CreateRaw(this, &FHttpRetryTask::HandleDefaultRetry)
		},
		{
			EHttpResponseCodes::ServiceUnavail,
			FHttpRetryScheduler::FHttpResponseCodeHandler::CreateRaw(this, &FHttpRetryTask::HandleDefaultRetry)
		},
		{
			EHttpResponseCodes::GatewayTimeout,
			FHttpRetryScheduler::FHttpResponseCodeHandler::CreateRaw(this, &FHttpRetryTask::HandleDefaultRetry)
		}
	};

	ResponseCodeDelegates.Emplace(EHttpResponseCodes::Denied, 
		FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
		[this](int32 StatusCode) -> EAccelByteTaskState {
			EAccelByteTaskState Result = EAccelByteTaskState::Failed;
#if !UE_SERVER
			if (IsRefreshable())
			{
				UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Denied and will refresh with Status Code %d"), StatusCode);
				Result = TriggerBearerAuthRejected();
			}
#endif
			return Result;
		}));
}

EAccelByteTaskState FHttpRetryTask::TriggerBearerAuthRejected()
{
	auto Result = EAccelByteTaskState::Failed;
	auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
	if (HttpRetrySchedulerPtr.IsValid())
	{
		HttpRetrySchedulerPtr->BearerAuthRejected();
		Result = Pause();
	}
	return Result;
}

void FHttpRetryTask::OnBearerAuthRejected()
{
	auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
	if (HttpRetrySchedulerPtr.IsValid())
	{
		FWriteScopeLock Lock(BearerAuthRefreshedHandleMtx);
		if (BearerAuthRefreshedHandle.IsValid())
		{
			HttpRetrySchedulerPtr->RemoveBearerAuthRefreshedDelegate(BearerAuthRefreshedHandle);
			BearerAuthRefreshedHandle.Reset();
		}
		BearerAuthRefreshedHandle = HttpRetrySchedulerPtr->AddBearerAuthRefreshedDelegate(
			FHttpRetryScheduler::FBearerAuthRefreshed::CreateThreadSafeSP(AsShared(), &FHttpRetryTask::OnBearerAuthRefreshed));
	}
}

void FHttpRetryTask::OnBearerAuthRefreshed(const FString& AccessToken)
{
	bIsBeenRunFromPause = true;
	if (GetTaskState() != EAccelByteTaskState::Paused)
	{
		return;
	}

	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("BearerAuthUpdated"));
	if (AccessToken.IsEmpty())
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Bearer token is empty, Task will be canceled"));
		Cancel();
		return;
	}
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	Request->SetHeader(TEXT("Authorization"), Authorization);

	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Bearer token updated, Task will be resumed"));
	SetTaskState(EAccelByteTaskState::Retrying);
	NextRetryTime.store(FPlatformTime::Seconds(), std::memory_order_release);
	auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
	if (HttpRetrySchedulerPtr.IsValid())
	{
		FWriteScopeLock Lock(BearerAuthRefreshedHandleMtx);
		HttpRetrySchedulerPtr->RemoveBearerAuthRefreshedDelegate(BearerAuthRefreshedHandle);
		BearerAuthRefreshedHandle.Reset();
	}
}

EAccelByteTaskState FHttpRetryTask::HandleDefaultRetry(int32 StatusCode)
{
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Handle Default Retry with Status Code %d"), StatusCode);

	return EAccelByteTaskState::Retrying;
}

void FHttpRetryTask::SetResponseTime(FDateTime InResponseTime) 
{ 
	FWriteScopeLock Lock(ResponseTimeMtx);
	ResponseTime = InResponseTime; 
}

FDateTime FHttpRetryTask::GetResponseTime() const 
{ 
	FReadScopeLock Lock(ResponseTimeMtx);
	return ResponseTime; 
}

bool FHttpRetryTask::IsResponseFromCache() 
{ 
	return bIsResponseFromCache.load(std::memory_order_acquire); 
}

bool FHttpRetryTask::CheckRetry(EAccelByteTaskState& Out)
{
	bool WillRetry = false;

	if (!IsTimedOut())
	{
		Out = ScheduleNextRetry();
		WillRetry = true;
	}

	return WillRetry;
}

EAccelByteTaskState FHttpRetryTask::Retry()
{
	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Retrying HTTP Request to : %s"), *Request->GetURL());
	if (Start())
	{
		FReport::LogHttpRequest(Request);
	}
	return GetTaskState();
}

EAccelByteTaskState FHttpRetryTask::ScheduleNextRetry()
{
	auto NextDelayBuf = NextDelay.load(std::memory_order_acquire);
	NextDelayBuf *= 2;
	NextDelayBuf += FMath::RandRange(static_cast<float>(-NextDelayBuf), static_cast<float>(NextDelayBuf)) / 4;

	if (NextDelayBuf > FHttpRetryScheduler::MaximumDelay)
	{
		NextDelayBuf = FHttpRetryScheduler::MaximumDelay;
	}
	NextDelay.store(NextDelayBuf, std::memory_order_release);

	auto NextRetryTimeBuf = NextRetryTime.load(std::memory_order_acquire);
	NextRetryTimeBuf = TaskTime.load(std::memory_order_acquire) + NextDelay.load(std::memory_order_acquire);

	if (NextRetryTimeBuf > RequestTime + PauseDuration.load(std::memory_order_acquire) + FHttpRetryScheduler::TotalTimeoutIncludingRetries)
	{
		NextRetryTimeBuf = RequestTime + PauseDuration.load(std::memory_order_acquire) + FHttpRetryScheduler::TotalTimeoutIncludingRetries;
	}
	NextRetryTime.store(NextRetryTimeBuf, std::memory_order_release);
	FHttpResponsePtr CurrentResponse;
	{
		FReadScopeLock Lock(ResponseMtx);
		CurrentResponse = Response;
	}
	Request->OnRequestWillRetry().ExecuteIfBound(Request, CurrentResponse, NextRetryTime.load(std::memory_order_acquire));
	return EAccelByteTaskState::Retrying;
}

bool FHttpRetryTask::IsFinished()
{
	return Request->GetStatus() == EHttpRequestStatus::Failed ||
#if !(ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4)
		Request->GetStatus() == EHttpRequestStatus::Failed_ConnectionError ||
#endif
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

	UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Is the request refreshable %d"), bIsRefreshable);
	return bIsRefreshable;
}

bool FHttpRetryTask::IsTimedOut()
{
	return TaskTime.load(std::memory_order_acquire) >= RequestTime + PauseDuration.load(std::memory_order_acquire) + FHttpRetryScheduler::TotalTimeoutIncludingRetries;
}

void FHttpRetryTask::OnProcessRequestComplete(FHttpRequestPtr InRequest
	, FHttpResponsePtr InResponse
	, bool bConnectedSuccessfully)
{
	if (InRequest.IsValid())
	{
		if (InResponse.IsValid())
		{
			{
				FWriteScopeLock Lock(ResponseMtx);
				Response = InResponse;
			}
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Got %s %s HTTP Request to: %s; Response: HTTP %d")
				, (bConnectedSuccessfully ? TEXT("SUCCESSFUL") : TEXT("FAILED"))
				, *InRequest->GetVerb()
				, *InRequest->GetURL()
				, InResponse->GetResponseCode());
		}
		else
		{
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Got %s %s HTTP Request to: %s; Response: INVALID HTTP RESPONSE")
				, (bConnectedSuccessfully ? TEXT("SUCCESSFUL") : TEXT("FAILED"))
				, *InRequest->GetVerb()
				, *InRequest->GetURL());
		}
	}
	else 
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Got %s HTTP Request due to Invalid Request and Response")
			, (bConnectedSuccessfully ? TEXT("SUCCESSFUL") : TEXT("FAILED")));
	}
	SetResponseTime(FDateTime::UtcNow());
}
} // namespace AccelByte
