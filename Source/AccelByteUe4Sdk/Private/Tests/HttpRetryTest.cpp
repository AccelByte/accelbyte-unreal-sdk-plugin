// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include <chrono>
#include <vector>

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "TestUtilities.h"
#include "Containers/Ticker.h"
#include "ParseErrorTest.h"
#include "GenericPlatform/GenericPlatformProcess.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Api/AccelByteOrderApi.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::UserProfile;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpRetryTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpRetryTest);

class FHttpRetrySchedulerTestingMode : public FHttpRetryScheduler
{
public:
	FHttpRetrySchedulerTestingMode()
		: FHttpRetryScheduler()
	{
		// Set this state to initialized without Startup() function that make the ticker duplication
	}

	virtual ~FHttpRetrySchedulerTestingMode()
	{
	}

	virtual void Startup() override
	{
		FHttpRetryScheduler::Startup();
	}
	
	void Startup(const FDelegateHandle& InTickDelegate)
	{
		PollRetryHandle = InTickDelegate;
		State = EState::Initialized;
		UE_LOG(LogAccelByteHttpRetryTest, Verbose, TEXT("HTTP Retry Scheduler Testing Mode has been INITIALIZED"));
	}

	virtual void Shutdown() override
	{
		// flush http requests
		if (!TaskQueue.IsEmpty())
		{
			// cancel unfinished http requests, so don't hinder the shutdown
			TaskQueue.Empty();
		}

		FHttpRetryScheduler::Shutdown();
	}
};

class FHttpTestWorker : public FRunnable
{
public:
	FHttpTestWorker(const FString& ThreadName
		, const TSharedRef<FHttpRetryScheduler>& Scheduler
		, const FHttpRequestPtr& Request
		, const FHttpRequestCompleteDelegate& CompleteDelegate
		, double RequestTime
		, double Delay = 0.0f)
		: Scheduler(Scheduler)
		, Request(Request)
		, RequestCompleteDelegate(CompleteDelegate)
		, RequestTime(RequestTime)
		, DelayTime(Delay)
	{
		Thread = FRunnableThread::Create(this, *ThreadName);
	}

	virtual ~FHttpTestWorker()
	{
		delete Thread;
		Thread = nullptr;
	}

	virtual uint32 Run() override
	{
		FPlatformProcess::Sleep(DelayTime);
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		FString ThreadName = FThreadManager::Get().GetThreadName(ThreadId);
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("[Async] Start Sending Request to URL=%s on Thread=%s"), *Request->GetURL(), *ThreadName);
		Scheduler->ProcessRequest(Request, RequestCompleteDelegate, RequestTime + DelayTime);
		return 0;
	}

	void EnsureCompletion()
	{
		Stop();
		Thread->WaitForCompletion();
	}

private:
	const TSharedRef<FHttpRetryScheduler> Scheduler;
	const FHttpRequestPtr Request;
	const FHttpRequestCompleteDelegate RequestCompleteDelegate;
	const double RequestTime;
	const double DelayTime;
	FRunnableThread* Thread;
};

DECLARE_DELEGATE_FiveParams(FHttpRetryTestDelegate, const FString& /*Url*/, bool& /*bRequestCompleted*/, bool& /*bRequestSucceeded*/, int& /*NumRequestRetry*/, FAccelByteTaskPtr& /*AccelByteTask*/);

BEGIN_DEFINE_SPEC(FHttpRetryTestSpec, "AccelByte.Tests.Core.HttpRetry",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask |
	EAutomationTestFlags::CriticalPriority)
	FTicker Ticker = FTicker::GetCoreTicker();
	FHttpRetrySchedulerTestingMode Scheduler;
	FDelegateHandle TickDelegate;
	double CurrentTime = 0.0f;
	int Timeout = FHttpRetryScheduler::TotalTimeout;
	float SpecifiedDeltaTime = 0.2f;
	FString BaseUrl = TEXT("https://httpbin.org");
	FHttpRetryTestDelegate TestDelegate;
END_DEFINE_SPEC(FHttpRetryTestSpec)

void FHttpRetryTestSpec::Define()
{
	int32 StatusCode = 0;

	TickDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([this](float)->bool
			{
				CurrentTime = FPlatformTime::Seconds();
				UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Poll Retry"), CurrentTime);
				Scheduler.PollRetry(CurrentTime);

				return true;
			}),
		SpecifiedDeltaTime);

	TestDelegate = FHttpRetryTestDelegate::CreateLambda(
		[this](const FString& Url, bool& bRequestCompleted, bool& bRequestSucceeded, int& NumRequestRetry, FAccelByteTaskPtr& AccelByteTask)
		{
			double Timer = 0.0f;

			{
				auto Request = FHttpModule::Get().CreateRequest();
				Request->SetURL(Url);
				Request->SetVerb(TEXT("GET"));

				//Hijack OnRequestWillRetry to peek if it's retried or not
				Request->OnRequestWillRetry().BindLambda(
					[this, &NumRequestRetry](FHttpRequestPtr, FHttpResponsePtr, float)
					{
						NumRequestRetry++;
						UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Retried %d times"), CurrentTime, NumRequestRetry);
					});

				const auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
					[this, &bRequestCompleted, &bRequestSucceeded](FHttpRequestPtr, FHttpResponsePtr Response, bool)
					{
						UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Completed"), CurrentTime);
						bRequestCompleted = true;
						bRequestSucceeded = (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()));
					});

				AccelByteTask = Scheduler.ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);
			}

			while (!bRequestCompleted && Timer <= Timeout + 1)
			{
				Timer += SpecifiedDeltaTime;
				Ticker.Tick(SpecifiedDeltaTime);
				FPlatformProcess::Sleep(SpecifiedDeltaTime);
			}
		});

	Describe("while sending a single request", [this]()
		{
			LatentBeforeEach([this](const FDoneDelegate& Done)
				{
					Timeout = 9;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					CurrentTime = FPlatformTime::Seconds();
					Scheduler.Startup(TickDelegate);
					Done.Execute();
				});

			LatentIt("should receive successful response when sending request with a valid URL"
				//, EAsyncExecution::ThreadPool
				, [this](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/get"), *BaseUrl)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_TRUE(bRequestSucceeded);

					return true;
				});

			// Disabled due to inconsistent behavior from the ISP which will affect the test significantly
			xLatentIt("should be immediately completed when sending request with an invalid URL"
				//, EAsyncExecution::ThreadPool
				, [this](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(TEXT("http://accelbyte.example")
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_EQUAL(AccelByteTask->State(), EAccelByteTaskState::Completed);

					return true;
				});

			LatentIt("should be cancelled when not receiving any response until timed out"
				//, EAsyncExecution::ThreadPool
				, [this](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/delay/10"), *BaseUrl)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(AccelByteTask->State(), EAccelByteTaskState::Cancelled);

					return true;
				});

			LatentIt("should retry sending requests when having network error"
				//, EAsyncExecution::ThreadPool
				, [this](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(TEXT("http://localhost:11223/connection_refused")
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					return true;
				});

			LatentAfterEach([this](const FDoneDelegate& Done)
				{
					Timeout = 60;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = Timeout;
					Scheduler.Shutdown();
					Done.Execute();
				});
		});

	Describe("while using default behaviors", [this, &StatusCode]()
		{
			LatentBeforeEach([this](const FDoneDelegate& Done)
				{
					Timeout = 15;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					CurrentTime = FPlatformTime::Seconds();
					Scheduler.Startup(TickDelegate);
					Done.Execute();
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::RetryWith);
			LatentIt(FString::Printf(TEXT("should retry sending requests when receiving status code %d (RetryWith)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::ServerError);
			LatentIt(FString::Printf(TEXT("should retry sending requests when receiving status code %d (ServerError)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::BadGateway);
			LatentIt(FString::Printf(TEXT("should retry sending requests when receiving status code %d (BadGateway)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::ServiceUnavail);
			LatentIt(FString::Printf(TEXT("should retry sending requests when receiving status code %d (ServiceUnavail)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::GatewayTimeout);
			LatentIt(FString::Printf(TEXT("should retry sending requests when receiving status code %d (GatewayTimeout)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::BadRequest);
			LatentIt(FString::Printf(TEXT("should not retry sending requests when receiving status code %d (BadRequest)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::NotFound);
			LatentIt(FString::Printf(TEXT("should not retry sending requests when receiving status code %d (NotFound)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					return true;
				});

			StatusCode = static_cast<int32>(EHttpResponseCodes::Type::Denied);
			LatentIt(FString::Printf(TEXT("should not retry sending requests when receiving status code %d (Denied)"), StatusCode)
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					return true;
				});

			LatentAfterEach([this](const FDoneDelegate& Done)
				{
					Timeout = 60;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = Timeout;
					Scheduler.Shutdown();
					Done.Execute();
				});
		});

	StatusCode = static_cast<int32>(EHttpResponseCodes::Type::ServerError);
	Describe(FString::Printf(TEXT("while using modified behaviors by replacing handler for %d (ServerError)"), StatusCode), [this, StatusCode]()
		{
			LatentBeforeEach([this](const FDoneDelegate& Done)
				{
					Timeout = 15;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					CurrentTime = FPlatformTime::Seconds();
					Scheduler.Startup(TickDelegate);
					Done.Execute();
				});

			LatentIt("should not retry sending requests when handler returns Task Completed"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Completed;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentIt("should not retry sending requests when handler returns Task Cancelled"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Cancelled;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentIt("should not retry sending requests when handler returns Task Failed"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Failed;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentIt("should retry sending requests when handler returns Task Retrying"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Retrying;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentIt("should retry sending requests when handler returns Task Running"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Running;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentIt("should retry sending requests when handler returns Task Pending"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Retrying;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_TRUE(NumRequestRetry > 0);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentIt("should be paused until timed oud when handler returns Task Paused"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this, StatusCode](const FDoneDelegate& Done)->bool
				{
					FHttpRetryScheduler::SetHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode), FHttpRetryScheduler::FHttpResponseCodeHandler::CreateLambda(
						[this](int32)->EAccelByteTaskState
						{
							return EAccelByteTaskState::Paused;
						}));

					bool bRequestCompleted = false;
					bool bRequestSucceeded = false;
					int NumRequestRetry = 0;
					int ExpectedRetry = 0;
					FAccelByteTaskPtr AccelByteTask;

					TestDelegate.Execute(FString::Printf(TEXT("%s/status/%d"), *BaseUrl, StatusCode)
						, bRequestCompleted
						, bRequestSucceeded
						, NumRequestRetry
						, AccelByteTask);

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent a Request and %d Completed"), CurrentTime, bRequestCompleted);
					Done.Execute();

					AB_TEST_TRUE(bRequestCompleted);
					AB_TEST_FALSE(bRequestSucceeded);
					AB_TEST_EQUAL(NumRequestRetry, ExpectedRetry);

					FHttpRetryScheduler::RemoveHttpResponseCodeHandlerDelegate(static_cast<EHttpResponseCodes::Type>(StatusCode));

					return true;
				});

			LatentAfterEach([this](const FDoneDelegate& Done)
				{
					Timeout = 60;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = Timeout;
					Scheduler.Shutdown();
					Done.Execute();
				});
		});

	Describe("while sending multiple requests at once", [this]()
		{
			LatentBeforeEach([this](const FDoneDelegate& Done)
				{
					Timeout = 15;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					CurrentTime = FPlatformTime::Seconds();
					Scheduler.Startup(TickDelegate);
					Done.Execute();
				});

			LatentIt("should received successful responses when sending multiple requests with all valid URLs"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this](const FDoneDelegate& Done)
				{
					int RequestCompleted = 0;
					int RequestSucceeded = 0;
					constexpr int RequestCount = 15;

					for (int index = 0; index < RequestCount; index++)
					{
						auto Request = FHttpModule::Get().CreateRequest();
						Request->SetURL(FString::Printf(TEXT("%s/get?id=%d"), *BaseUrl, index));
						Request->SetVerb(TEXT("GET"));

						auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
							[this, &RequestCompleted, &RequestSucceeded](FHttpRequestPtr Request, FHttpResponsePtr Response, bool)
							{
								UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Completed, URL=%s"), CurrentTime, *Request->GetURL());
								RequestCompleted++;
								if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
								{
									RequestSucceeded++;
								}
							});

						Scheduler.ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);
					}

					double Timer = 0.0f;

					while (RequestCompleted < RequestCount && Timer <= Timeout + 1)
					{
						Timer += SpecifiedDeltaTime;
						Ticker.Tick(SpecifiedDeltaTime);
						FPlatformProcess::Sleep(SpecifiedDeltaTime);
					}

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent %d Requests and %d Completed"), CurrentTime, RequestCount, RequestCompleted);
					Done.Execute();

					AB_TEST_EQUAL(RequestCompleted, RequestCount);
					AB_TEST_EQUAL(RequestSucceeded, RequestCount);

					return true;
				});

			LatentIt("should received some successful responses when sending multiple requests with some invalid URLs"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this](const FDoneDelegate& Done)
				{
					int RequestCompleted = 0;
					int RequestSucceeded = 0;
					int RequestFailed = 0;
					constexpr int RequestCount = 12;

					for (int index = 0; index < RequestCount; index++)
					{
						auto Request = FHttpModule::Get().CreateRequest();
						switch (index % 4)
						{
						case 0:
							Request->SetURL(FString::Printf(TEXT("%s/get?id=%d"), *BaseUrl, index));

							break;
						case 1:
							//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
							//BadRequest 400, delayed by 4s
							Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=4s&id=%d"), index));

							break;
						case 2:
							//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
							//BadRequest 400, delayed by 6s
							Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=6s&id=%d"), index));

							break;
						case 3:
							//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
							//Sucess 200, delayed by 8s
							Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c37fc0330000054001f659d?mocky-delay=8s&id=%d"), index));

							break;
						}
						Request->SetVerb(TEXT("GET"));

						auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
							[this, &RequestCompleted, &RequestSucceeded, &RequestFailed](FHttpRequestPtr Request, FHttpResponsePtr Response, bool)
							{
								UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Completed, URL=%s"), CurrentTime, *Request->GetURL());
								RequestCompleted++;
								if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
								{
									RequestSucceeded++;
								}
								else
								{
									RequestFailed++;
								}
							});

						Scheduler.ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);
					}

					double Timer = 0.0f;

					while (RequestCompleted < RequestCount && Timer <= Timeout + 1)
					{
						Timer += SpecifiedDeltaTime;
						Ticker.Tick(SpecifiedDeltaTime);
						FPlatformProcess::Sleep(SpecifiedDeltaTime);
					}

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent %d Requests and %d Completed"), CurrentTime, RequestCount, RequestCompleted);
					Done.Execute();

					AB_TEST_EQUAL(RequestCompleted, RequestCount);
					AB_TEST_EQUAL(RequestSucceeded, RequestCount / 2);
					AB_TEST_EQUAL(RequestFailed, RequestCount / 2);

					return true;
				});

			LatentIt("should received successful responses when sending multiple requests with all valid URLs and Concurrency"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this](const FDoneDelegate& Done)
				{
					int RequestCompleted = 0;
					int RequestSucceeded = 0;
					constexpr int RequestCount = 15;

					TArray<TSharedPtr<FHttpTestWorker>> Tasks;

					for (int index = 0; index < RequestCount; index++)
					{
						auto Request = FHttpModule::Get().CreateRequest();
						Request->SetURL(FString::Printf(TEXT("%s/get?id=%d"), *BaseUrl, index));
						Request->SetVerb(TEXT("GET"));

						auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
							[this, &RequestCompleted, &RequestSucceeded](FHttpRequestPtr Request, FHttpResponsePtr Response, bool)
							{
								UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Completed, URL=%s"), CurrentTime, *Request->GetURL());
								RequestCompleted++;
								if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
								{
									RequestSucceeded++;
								}
							});

						FString ThreadName = FString::Printf(TEXT("Concurrency_%03d"), index);
						TSharedPtr<FHttpTestWorker> Task = MakeShared<FHttpTestWorker>(ThreadName
							, MakeShareable(&Scheduler, [](FHttpRetryScheduler*) {})
							, Request
							, RequestCompleteDelegate
							, CurrentTime
							, 2.0f);
						Tasks.Add(Task);
					}

					double Timer = 0.0f;

					while (RequestCompleted < RequestCount && Timer <= Timeout + 1)
					{
						Timer += SpecifiedDeltaTime;
						Ticker.Tick(SpecifiedDeltaTime);
						FPlatformProcess::Sleep(SpecifiedDeltaTime);
					}

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent %d Requests and %d Completed"), CurrentTime, RequestCount, RequestCompleted);
					Done.Execute();

					AB_TEST_EQUAL(RequestCompleted, RequestCount);
					AB_TEST_EQUAL(RequestSucceeded, RequestCount);

					for (auto& Task : Tasks)
					{
						Task->EnsureCompletion();
					}

					return true;
				});

			LatentIt("should received some successful responses when sending multiple requests with some invalid URLs and Concurrency"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this](const FDoneDelegate& Done)
				{
					int RequestCompleted = 0;
					int RequestSucceeded = 0;
					int RequestFailed = 0;
					constexpr int RequestCount = 12;

					TArray<TSharedPtr<FHttpTestWorker>> Tasks;

					for (int index = 0; index < RequestCount; index++)
					{
						float DelayTime = 0.0f;
						auto Request = FHttpModule::Get().CreateRequest();
						switch (index % 4)
						{
						case 0:
							Request->SetURL(FString::Printf(TEXT("%s/get?id=%d"), *BaseUrl, index));
							DelayTime = 8.0;

							break;
						case 1:
							//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
							//BadRequest 400, delayed by 4s
							Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=4s&id=%d"), index));
							DelayTime = 4.0f;

							break;
						case 2:
							//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
							//BadRequest 400, delayed by 6s
							Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=6s&id=%d"), index));
							DelayTime = 1.8f;

							break;
						case 3:
							//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
							//Sucess 200, delayed by 8s
							Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c37fc0330000054001f659d?mocky-delay=8s&id=%d"), index));
							
							break;
						}
						Request->SetVerb(TEXT("GET"));

						auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
							[this, &RequestCompleted, &RequestSucceeded, &RequestFailed](FHttpRequestPtr Request, FHttpResponsePtr Response, bool)
							{
								UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Completed, URL=%s"), CurrentTime, *Request->GetURL());
								RequestCompleted++;
								if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
								{
									RequestSucceeded++;
								}
								else
								{
									RequestFailed++;
								}
							});

						FString ThreadName = FString::Printf(TEXT("Concurrency_%03d"), index);
						TSharedPtr<FHttpTestWorker> Task = MakeShared<FHttpTestWorker>(ThreadName
							, MakeShareable(&Scheduler, [](FHttpRetryScheduler*) {})
							, Request
							, RequestCompleteDelegate
							, CurrentTime
							, DelayTime);
						Tasks.Add(Task);
					}

					double Timer = 0.0f;

					while (RequestCompleted < RequestCount && Timer <= Timeout + 1)
					{
						Timer += SpecifiedDeltaTime;
						Ticker.Tick(SpecifiedDeltaTime);
						FPlatformProcess::Sleep(SpecifiedDeltaTime);
					}

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Sent %d Requests and %d Completed"), CurrentTime, RequestCount, RequestCompleted);
					Done.Execute();

					AB_TEST_EQUAL(RequestCompleted, RequestCount);
					AB_TEST_EQUAL(RequestSucceeded, RequestCount / 2);
					AB_TEST_EQUAL(RequestFailed, RequestCount / 2);

					for (auto& Task : Tasks)
					{
						Task->EnsureCompletion();
					}

					return true;
				});

			LatentIt("should received successful responses when sending multiple chain requests with valid URLs"
				//, EAsyncExecution::ThreadPool
				, FTimespan(0, 1, 10)
				, [this](const FDoneDelegate& Done)
				{
					auto SendMockRequest = [this](int Id, const FHttpRequestCompleteDelegate& OnCompleted)
					{
						auto Request = FHttpModule::Get().CreateRequest();
						Request->SetURL(FString::Printf(TEXT("%s/delay/1?id=%d"), *BaseUrl, Id));
						Request->SetVerb(TEXT("GET"));

						Scheduler.ProcessRequest(Request, OnCompleted, CurrentTime);
						FHttpModule::Get().GetHttpManager().Flush(false);
					};

					auto TickerDelegate = Ticker.AddTicker(
						FTickerDelegate::CreateLambda([this](float DeltaTime)
							{
								UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Poll Http Module"), CurrentTime);
								FHttpModule::Get().GetHttpManager().Tick(DeltaTime);

								return true;
							}),
						SpecifiedDeltaTime);

					int RequestCompleted = 0;
					int RequestCount = 10;
					using Del = FHttpRequestCompleteDelegate;

					SendMockRequest(0, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
						{
							UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 0"), CurrentTime);
							RequestCompleted++;
							SendMockRequest(1, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
								{
									UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 1"), CurrentTime);
									RequestCompleted++;
									SendMockRequest(2, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
										{
											UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 2"), CurrentTime);
											RequestCompleted++;
											SendMockRequest(3, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
												{
													UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 3"), CurrentTime);
													RequestCompleted++;
													SendMockRequest(4, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
														{
															UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 4"), CurrentTime);
															RequestCompleted++;
														}));
												}));
										}));
								}));
						}));
					FHttpModule::Get().GetHttpManager().Flush(false);

					SendMockRequest(5, Del::CreateLambda([&RequestCompleted](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
						{
							RequestCompleted++;
						}));
					FHttpModule::Get().GetHttpManager().Flush(false);

					SendMockRequest(6, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
						{
							RequestCompleted++;
							SendMockRequest(7, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
								{
									RequestCompleted++;
									SendMockRequest(8, Del::CreateLambda([this, &RequestCompleted, &SendMockRequest](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
										{
											RequestCompleted++;
										}));
								}));
						}));
					FHttpModule::Get().GetHttpManager().Flush(false);

					SendMockRequest(9, Del::CreateLambda([&RequestCompleted](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
						{
							RequestCompleted++;
						}));
					FHttpModule::Get().GetHttpManager().Flush(false);

					double Timer = 0.0f;

					while (RequestCompleted < RequestCount && Timer <= Timeout + 1)
					{
						Timer += SpecifiedDeltaTime;
						Ticker.Tick(SpecifiedDeltaTime);
						FPlatformProcess::Sleep(SpecifiedDeltaTime);
					}

					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("CurrentTime=%.4f - Request Completed %d"), CurrentTime, RequestCompleted);
					Done.Execute();

					AB_TEST_EQUAL(RequestCompleted, RequestCount);

					Ticker.RemoveTicker(TickerDelegate);

					return true;
				});

			LatentAfterEach([this](const FDoneDelegate& Done)
				{
					Timeout = 60;
					FHttpRetryScheduler::MaximumDelay = FGenericPlatformMath::CeilToInt(Timeout / 2.0f);
					FHttpRetryScheduler::TotalTimeout = Timeout;
					FHttpRetryScheduler::PauseTimeout = Timeout;
					Scheduler.Shutdown();
					Done.Execute();
				});
		});
}