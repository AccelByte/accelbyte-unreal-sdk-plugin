// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
	FHttpRetryTask::FHttpRetryTask(
		FHttpRequestPtr& InRequest,
		const FHttpRequestCompleteDelegate& InCompleteDelegate,
		double InRequestTime,
		double InNextDelay,
		const FVoidHandler& InOnBearerAuthRejectDelegate,
		FBearerAuthRejectedRefresh& InBearerAuthRejectedRefresh,
		TMap<EHttpResponseCodes::Type, FHttpRetryScheduler::FHttpResponseCodeHandler> HandlerDelegates)
		: Request{ InRequest }
		, CompleteDelegate{ InCompleteDelegate }
		, RequestTime{ InRequestTime }
		, PauseTime{}
		, PauseDuration{}
		, NextRetryTime{ InNextDelay }
		, NextDelay{ InNextDelay }
		, OnBearerAuthRejectDelegate{ InOnBearerAuthRejectDelegate }
		, BearerAuthRejectedRefresh{ InBearerAuthRejectedRefresh }
	{
		Request->OnProcessRequestComplete().BindRaw(this, &FHttpRetryTask::OnProcessRequestComplete);
		
		TaskTime = RequestTime;
		InitializeDefaultDelegates();
		for (auto& Handler : HandlerDelegates)
		{
			ResponseCodeDelegates.Emplace(Handler.Key, Handler.Value);
		}
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
			TaskState = EAccelByteTaskState::Paused;
			BearerAuthRejectedRefreshHandle = BearerAuthRejectedRefresh.Add(THandler<FString>::CreateLambda([&](const FString& AccessToken) 
			{
				BearerAuthUpdated(AccessToken);
			}));
			OnBearerAuthRejectDelegate.ExecuteIfBound();
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
		case EHttpRequestStatus::Processing:
			if (IsTimedOut()) 
			{
				Cancel();
				NextState = TaskState;
			}
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
			break;
		}
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4
		case EHttpRequestStatus::Failed:
		{
			const EHttpFailureReason FailureReason = Request->GetFailureReason();
			if (FailureReason == EHttpFailureReason::ConnectionError)
			{
				CheckRetry(NextState);
			}
			else
			{
				Cancel();
				NextState = TaskState;
			}
			break;
		}
#else
		case EHttpRequestStatus::Failed_ConnectionError: //network error
			CheckRetry(NextState);
			break;
		case EHttpRequestStatus::Failed: //request cancelled
			Cancel();
			NextState = TaskState;
#endif
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

		if (TaskState == EAccelByteTaskState::Completed || TaskState == EAccelByteTaskState::Cancelled || TaskState == EAccelByteTaskState::Failed)
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
					Result = Pause();
				}
#endif
				return Result;
			}));
	}

	void FHttpRetryTask::BearerAuthUpdated(const FString& AccessToken)
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

		if (NextRetryTime > RequestTime + PauseDuration + FHttpRetryScheduler::TotalTimeout)
		{
			NextRetryTime = RequestTime + PauseDuration + FHttpRetryScheduler::TotalTimeout;
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
		return TaskTime >= RequestTime + PauseDuration + FHttpRetryScheduler::TotalTimeout;
	}

	void FHttpRetryTask::OnProcessRequestComplete(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse,
		bool bConnectedSuccessfully)
	{
		SetResponseTime(FDateTime::UtcNow());
	}
}
