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
		: Request{ InRequest }
		, CompleteDelegate{ InCompleteDelegate }
		, RequestTime{ InRequestTime }
		, PauseTime{}
		, PauseDuration{}
		, NextRetryTime{ InNextDelay }
		, NextDelay{ InNextDelay }
		, HttpRetrySchedulerWPtr{ InHttpRetrySchedulerWPtr }
	{
		TaskTime = RequestTime;
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
			HttpRetrySchedulerPtr->RemoveBearerAuthRejectedDelegate(BearerAuthRejectedHandle);
			if (BearerAuthRefreshedHandle.IsValid())
			{
				HttpRetrySchedulerPtr->RemoveBearerAuthRefreshedDelegate(BearerAuthRefreshedHandle);
				BearerAuthRefreshedHandle.Reset();
			}
		}

#if UE_BUILD_SHIPPING

		if (!ensure(Request.IsValid()))
		{
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Error during HTTP Retry Task Destructor: Request is invalid"));
			return;
		}

		const bool bIsTaskStateFinishProperly = (TaskState != EAccelByteTaskState::Completed) && (TaskState != EAccelByteTaskState::Failed) && (TaskState != EAccelByteTaskState::Cancelled);
		if (ensure(bIsTaskStateFinishProperly))
		{
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Error during HTTP Retry Task Destructor: Task state is not ready to proceed"));
			return;
		}
#endif

		if (Request.IsValid() && Request->OnProcessRequestComplete().IsBound())
		{
			Request->OnProcessRequestComplete().Unbind();
		}
	}

	bool FHttpRetryTask::Start()
	{
		if (!Request.IsValid())
		{
			TaskState = EAccelByteTaskState::Failed;
			return false;
		}

		if (Request->OnProcessRequestComplete().IsBound())
		{
			Request->OnProcessRequestComplete().Unbind();
		}
		Request->OnProcessRequestComplete().BindThreadSafeSP(this, &FHttpRetryTask::OnProcessRequestComplete);


		auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
		if (HttpRetrySchedulerPtr.IsValid())
		{
			BearerAuthRejectedHandle = HttpRetrySchedulerPtr->AddBearerAuthRejectedDelegate(
				FHttpRetryScheduler::FBearerAuthRejected::CreateThreadSafeSP(AsShared(), &FHttpRetryTask::OnBearerAuthRejected));
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
			TaskState = EAccelByteTaskState::Paused;
		}
		PauseTime = TaskTime;

		return FAccelByteTask::Pause();
	}

	TMap<FString, FString> FHttpRetryTask::GetResponseHeader()
	{
		TMap<FString, FString> Result;

		if(!Request.IsValid())
		{
			return Result;
		}

		const FHttpResponsePtr Response = Request->GetResponse();
		if(!Response.IsValid())
		{
			return Result;
		}

		const FString HeaderContent = Response->GetHeader(HEADER_FEATURE_FLAG);

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
			return Result;
		}

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

		if (TaskState == EAccelByteTaskState::Completed || TaskState == EAccelByteTaskState::Cancelled || TaskState == EAccelByteTaskState::Failed)
		{
			return;
		}

		EAccelByteTaskState NextState = TaskState;
	
		if (!Request.IsValid())
		{
			TaskState = EAccelByteTaskState::Failed;
			return;
		}

		TaskTime = CurrentTime;

		if (TaskState == EAccelByteTaskState::Paused)
		{
			double DeltaTime = TaskTime - PauseTime;
			PauseDuration += DeltaTime;
			PauseTime = TaskTime;

			if (PauseDuration > FHttpRetryScheduler::PauseTimeout)
			{
				UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTPRetryTask pause timeout of %d seconds reached! Cancelling..."), FHttpRetryScheduler::PauseTimeout);
				Cancel();
			}
			return;
		}
		else if (TaskState == EAccelByteTaskState::Retrying)
		{
			if (TaskTime < NextRetryTime)
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
			const FHttpResponsePtr Response = Request->GetResponse();
			if (Response.IsValid())
			{
				const int32 ResponseCode = Response->GetResponseCode();
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
			TaskState = EAccelByteTaskState::Failed;
			NextState = TaskState;
			CheckRetry(NextState);
		}break;
#else
		case EHttpRequestStatus::Failed_ConnectionError: //network error
		case EHttpRequestStatus::Failed: //request failed
		{
			TaskState = EAccelByteTaskState::Failed;
			NextState = TaskState;
			CheckRetry(NextState);
		}break;
#endif
		default:
			break;
		}
		TaskState = NextState;
	}

	bool FHttpRetryTask::Finish()
	{
		FString State = FAccelByteUtilities::GetUEnumValueAsString(TaskState);
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("HTTPRetryTask finished with status: %s"), *State);

		if (TaskState == EAccelByteTaskState::Running)
		{
			Cancel();
		}

		if (TaskState == EAccelByteTaskState::Completed || TaskState == EAccelByteTaskState::Cancelled || TaskState == EAccelByteTaskState::Failed)
		{
			auto Response = Request->GetResponse();
			FReport::LogHttpResponse(Request, Response);
			CompleteDelegate.ExecuteIfBound(Request, Response, IsFinished());
		}

		return FAccelByteTask::Finish();
	}

	bool FHttpRetryTask::FinishFromCached(const FHttpResponsePtr& Response)
	{
		TaskState = EAccelByteTaskState::Completed;
	
		FReport::LogHttpResponse(Request, Response);
		CompleteDelegate.ExecuteIfBound(Request, Response, true /*IsFinished()*/);

		bIsResponseFromCache = true;

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
		if (TaskState != EAccelByteTaskState::Paused)
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
		TaskState = EAccelByteTaskState::Retrying;
		NextRetryTime = FPlatformTime::Seconds();
		auto HttpRetrySchedulerPtr = HttpRetrySchedulerWPtr.Pin();
		if (HttpRetrySchedulerPtr.IsValid())
		{
			HttpRetrySchedulerPtr->RemoveBearerAuthRefreshedDelegate(BearerAuthRefreshedHandle);
			BearerAuthRefreshedHandle.Reset();
		}
	}

	EAccelByteTaskState FHttpRetryTask::HandleDefaultRetry(int32 StatusCode)
	{
		UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Handle Default Retry with Status Code %d"), StatusCode);

		return EAccelByteTaskState::Retrying;
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
		return TaskState;
	}

	EAccelByteTaskState FHttpRetryTask::ScheduleNextRetry()
	{
		NextDelay *= 2;
		NextDelay += FMath::RandRange((float)-NextDelay, (float)NextDelay) / 4;

		if (NextDelay > FHttpRetryScheduler::MaximumDelay)
		{
			NextDelay = FHttpRetryScheduler::MaximumDelay;
		}

		NextRetryTime = TaskTime + NextDelay;

		if (NextRetryTime > RequestTime + PauseDuration + FHttpRetryScheduler::TotalTimeoutIncludingRetries)
		{
			NextRetryTime = RequestTime + PauseDuration + FHttpRetryScheduler::TotalTimeoutIncludingRetries;
		}

		Request->OnRequestWillRetry().ExecuteIfBound(Request, Request->GetResponse(), NextRetryTime);

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
		return TaskTime >= RequestTime + PauseDuration + FHttpRetryScheduler::TotalTimeoutIncludingRetries;
	}

	void FHttpRetryTask::OnProcessRequestComplete(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse,
		bool bConnectedSuccessfully)
	{
		if (InRequest.IsValid() && InResponse.IsValid())
		{
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Got %s %s HTTP Request to: %s; Response: HTTP %d"), (bConnectedSuccessfully ? TEXT("SUCCESSFUL") : TEXT("FAILED")), *InRequest->GetVerb(), *InRequest->GetURL(), InResponse->GetResponseCode());
		}
		else if (InResponse.IsValid()) {
			UE_LOG(LogAccelByteHttpRetry, Verbose, TEXT("Got %s %s HTTP Request to: %s; Response: INVALID HTTP RESPONSE"), (bConnectedSuccessfully ? TEXT("SUCCESSFUL") : TEXT("FAILED")), *InRequest->GetVerb(), *InRequest->GetURL());
		}
		else {
		}
		SetResponseTime(FDateTime::UtcNow());
	}
}
