// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>
#include <vector>

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "TestUtilities.h"
#include "Runtime/Core/Public/Containers/Ticker.h"
#include "ParseErrorTest.h"

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

static const int32 AutomationFlagMaskHttpRetry = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

using namespace std;

static Settings OriginalSettings;
void ResetSettings();

class FHttpRetrySchedulerTestingMode: public FHttpRetryScheduler{
public:
	FHttpRetrySchedulerTestingMode()
	{
		// Set this state to initialized without Startup() function that make the ticker duplication
		State = EState::Initialized;
	};
};

class FHttpTestWorker : public FRunnable
{
public:
	FHttpTestWorker(const FString& ThreadName, const TSharedRef<FHttpRetryScheduler>& Scheduler, const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double Delay = 0.0f)
		: Scheduler(Scheduler)
		, Request(Request)
		, RequestCompleteDelegate(CompleteDelegate)
		, RequestTime(RequestTime)
		, DelayTime(Delay)
	{ 
		Thread = FRunnableThread::Create(this, *ThreadName);
	}

	~FHttpTestWorker()
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(HttpRetryBackupSettings, "AccelByte.Tests.Core.HttpRetry.A.BackupSettings", AutomationFlagMaskHttpRetry);
bool HttpRetryBackupSettings::RunTest(const FString& Parameter)
{
	OriginalSettings = FRegistry::Settings;
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_GotError500Twice_RetryTwice, "AccelByte.Tests.Core.HttpRetry.ProcessRequest_GotError500Twice_RetryTwice", AutomationFlagMaskHttpRetry);
bool ProcessRequest_GotError500Twice_RetryTwice::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime);

			return true;
		}),
		0.2f);


	auto Request = FHttpModule::Get().CreateRequest();
	//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
	//Error 503 Service Unavailable, with response content:
	//{
	//	"numericErrorCode": 33333,
	//		"errorCode" : "httpRetryErrorResponse",
	//		"errorMessage" : "This endpoint will always answer with this exact response"
	//}

	Request->SetURL("http://www.mocky.io/v2/5d07138a3000001224051d6f");
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("text/html; charset=utf-8"));
	bool RequestCompleted = false;
	int NumRequestRetry = 0;

	//Hijack OnProcessRequestComplete to peek if it's retried or not
	Request->OnProcessRequestComplete().BindLambda(
	[&NumRequestRetry, &CurrentTime]
	(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		NumRequestRetry++;
		UE_LOG(LogAccelByteHttpRetryTest, Warning, TEXT("%.4f Request Retried %d times"), CurrentTime, NumRequestRetry);
	});
	
	//Official OnProcessRequestComplete
	auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
	[&RequestCompleted, &CurrentTime]
	(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed"), CurrentTime);
		RequestCompleted = true;
	});

	float SpecifiedDeltaTime = 0.5f;
	CurrentTime = 10.0;
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 16)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f NumRequestRetry=%d"), CurrentTime, NumRequestRetry);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed=%d"), CurrentTime, RequestCompleted);
	AB_TEST_FALSE(RequestCompleted);
	AB_TEST_TRUE(NumRequestRetry >= 2);

	FRegistry::Credentials.ForgetAll();
	
	Ticker.RemoveTicker(TickerDelegate);
	ResetSettings();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_NetworkError_Retry, "AccelByte.Tests.Core.HttpRetry.ProcessRequest_NetworkError_Retry", AutomationFlagMaskHttpRetry);
bool ProcessRequest_NetworkError_Retry::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
		Scheduler->PollRetry(CurrentTime);

		return true;
	}),
		0.2f);


	auto Request = FHttpModule::Get().CreateRequest();

	Request->SetURL("http://localhost:11223/connection_refused");
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("text/html; charset=utf-8"));
	bool RequestCompleted = false;
	int NumRequestRetry = 0;

	//Hijack OnProcessRequestComplete to peek if it's retried or not
	Request->OnProcessRequestComplete().BindLambda(
		[&NumRequestRetry, &CurrentTime]
	(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		NumRequestRetry++;
		UE_LOG(LogAccelByteHttpRetryTest, Warning, TEXT("%.4f Request Retried %d times"), CurrentTime, NumRequestRetry);
	});

	//Official OnProcessRequestComplete
	auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
		[&RequestCompleted, &CurrentTime]
	(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed"), CurrentTime);
		RequestCompleted = true;
	});

	float SpecifiedDeltaTime = 0.5f;
	CurrentTime = 10.0;
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 30 && NumRequestRetry < 2)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f NumRequestRetry=%d"), CurrentTime, NumRequestRetry);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed=%d"), CurrentTime, RequestCompleted);
	AB_TEST_FALSE(RequestCompleted);
	AB_TEST_TRUE(NumRequestRetry >= 2);

	FRegistry::Credentials.ForgetAll();

	Ticker.RemoveTicker(TickerDelegate);
	ResetSettings();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_NoConnection_RequestImmediatelyCompleted, "AccelByte.DisabledTests.Core.HttpRetry.ProcessRequest_NoConnection_RequestImmediatelyCompleted", AutomationFlagMaskHttpRetry);
bool ProcessRequest_NoConnection_RequestImmediatelyCompleted::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime);

			return true;
		}),
		0.2f);

	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("http://www.accelbyte.example"); //Address that should not exist forever
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("text/html; charset=utf-8"));
	bool RequestCompleted = false;
	auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda([&RequestCompleted, &CurrentTime](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (Response.IsValid()) 
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed, Status %d"), CurrentTime, Response->GetResponseCode());
		}
		else
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed, NoResponse"), CurrentTime);
		}

		RequestCompleted = true;
	});

	float SpecifiedDeltaTime = 0.2f;
	CurrentTime = 10.0;
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 70.3 && !RequestCompleted)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	AB_TEST_TRUE(RequestCompleted);
	AB_TEST_FALSE(Request->GetResponse().IsValid());
	AB_TEST_EQUAL(Request->GetStatus(), EHttpRequestStatus::Failed_ConnectionError);

	Ticker.RemoveTicker(TickerDelegate);
	FRegistry::Credentials.ForgetAll();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_NoResponseFor60s_RequestCancelled, "AccelByte.Tests.Core.HttpRetry.ProcessRequest_NoResponseFor60s_RequestCancelled", AutomationFlagMaskHttpRetry);
bool ProcessRequest_NoResponseFor60s_RequestCancelled::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime);

			return true;
		}),
		0.2f);

	auto Request = FHttpModule::Get().CreateRequest();
	//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
	//Sucess 200, delayed by 60s
	Request->SetURL("http://www.mocky.io/v2/5c37fc0330000054001f659d?mocky-delay=60s");
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("text/html; charset=utf-8"));
	bool RequestCompleted = false;
	
	auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda([&RequestCompleted, &CurrentTime](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (Response.IsValid())
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed, Status %d"), CurrentTime, Response->GetResponseCode());
		}
		else
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed, NoResponse"), CurrentTime);
		}

		RequestCompleted = true;
	});

	float SpecifiedDeltaTime = 0.2f;
	CurrentTime = 10.0;
	auto AccelByteTask = Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 70.3 && !RequestCompleted)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime / 2); //lying to Scheduler, it's actually 30s instead of 60s. So mocky still hold the response, while Scheduler thinks that it's timeout.
	}

	AB_TEST_TRUE(RequestCompleted);
	AB_TEST_FALSE(Request->GetResponse().IsValid());
	AB_TEST_EQUAL(Request->GetStatus(), EHttpRequestStatus::Failed);
	AB_TEST_EQUAL(AccelByteTask->State(), EAccelByteTaskState::Cancelled);

	FRegistry::Credentials.ForgetAll();
	Ticker.RemoveTicker(TickerDelegate);
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessManyRequests_WithValidURL_AllCompleted, "AccelByte.Tests.Core.HttpRetry.ProcessManyRequests_WithValidURL_AllCompleted", AutomationFlagMaskHttpRetry);
bool ProcessManyRequests_WithValidURL_AllCompleted::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime);

			return true;
		}),
		0.2f);

	int RequestCompleted = 0;
	int RequestCount = 15;

	for (int i = 0; i < RequestCount; i++)
	{
		auto Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(FString::Printf(TEXT("http://www.example.com/?id=%d"), i));
		Request->SetVerb(TEXT("GET"));

		auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda([&CurrentTime, &RequestCompleted](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed, Status %d, URL=%s"), CurrentTime, Response->GetResponseCode(), *Request->GetURL());
			RequestCompleted++;

		});

		Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);
	}

	float SpecifiedDeltaTime = 0.2f;
	CurrentTime = 10.0;

	while (CurrentTime < 15.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	AB_TEST_EQUAL(RequestCompleted, RequestCount);

	FRegistry::Credentials.ForgetAll();
	Ticker.RemoveTicker(TickerDelegate);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessManyRequests_WithSomeInvalidURLs_AllCompleted, "AccelByte.Tests.Core.HttpRetry.ProcessManyRequests_WithSomeInvalidURLs_AllCompleted", AutomationFlagMaskHttpRetry);
bool ProcessManyRequests_WithSomeInvalidURLs_AllCompleted::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime);

			return true;
		}),
		0.2f);

	int RequestCompleted = 0;
	int RequestSucceeded = 0;
	int RequestFailed = 0;
	TArray<FHttpRequestPtr> CancelledRequests;

	for (int i = 0; i < 12; i++)
	{
		auto Request = FHttpModule::Get().CreateRequest();

		switch (i % 4)
		{
		case 0:
			Request->SetURL(FString::Printf(TEXT("http://www.example.com/?id=%d"), i));

			break;
		case 1:
			//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
			//BadRequest 400, delayed by 10s
			Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=10s&id=%d"), i));

			break;
		case 2:
			//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
			//BadRequest 400, delayed by 10s
			Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=10s&id=%d"), i));
			CancelledRequests.Add(Request);

			break;
		case 3:
			//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
			//Sucess 200, delayed by 20s
			Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c37fc0330000054001f659d?mocky-delay=20s&id=%d"), i));

			break;
		}

		Request->SetVerb(TEXT("GET"));

		auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda([&RequestCompleted, &RequestSucceeded, &RequestFailed](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			RequestCompleted++;

			if (Request->GetStatus() == EHttpRequestStatus::Succeeded)
			{
				UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("RequestStatus = %s, ResponseCode=%d, URL=%s"), ToString(Request->GetStatus()), Response->GetResponseCode(), *Request->GetURL());
				RequestSucceeded++;
			}
			else
			{
				UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("RequestStatus=%s, URL=%s"), ToString(Request->GetStatus()), *Request->GetURL());
				RequestFailed++;
			}

		});

		Scheduler->ProcessRequest(Request, RequestCompleteDelegate, 10.01);
	}

	CurrentTime = 10.0;
	float SpecifiedDeltaTime = 0.5f;

	while (CurrentTime < 15.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	AB_TEST_EQUAL(RequestFailed, 0);
	AB_TEST_EQUAL(RequestSucceeded, 3);

	for (const auto& request : CancelledRequests)
	{
		request->CancelRequest();
	}

	while (CurrentTime < 25.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	AB_TEST_EQUAL(RequestFailed, 3);
	AB_TEST_EQUAL(RequestSucceeded, 6);

	while (CurrentTime < 35.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	AB_TEST_EQUAL(RequestFailed, 3);
	AB_TEST_EQUAL(RequestSucceeded, 9);

	FRegistry::Credentials.ForgetAll();
	Ticker.RemoveTicker(TickerDelegate);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequestsChain_WithValidURLs_AllCompleted, "AccelByte.Tests.Core.HttpRetry.ProcessRequestsChain_WithValidURLs_AllCompleted", AutomationFlagMaskHttpRetry);
bool ProcessRequestsChain_WithValidURLs_AllCompleted::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto& Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	auto SendMockyRequest = [&](int Id, const FHttpRequestCompleteDelegate& OnCompleted)
	{
		//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
		//Sucess 200, delayed by 2s
		auto Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c6cc89d370000d808fa2fdd?mocky-delay=1s&id=%d"), Id));
		Request->SetVerb(TEXT("GET"));
		Scheduler->ProcessRequest(Request, OnCompleted, CurrentTime);
		FHttpModule::Get().GetHttpManager().Flush(false);
	};

	auto TickerDelegate1 = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime);

			return true;
		}),
		0.2f);

	auto TickerDelegate2 = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Http Module"), CurrentTime);
			FHttpModule::Get().GetHttpManager().Tick(DeltaTime);

			return true;
		}),
		0.2f);

	float SpecifiedDeltaTime = 0.5f;
	CurrentTime = 10.0;
	int RequestCompleted = 0;
	using Del = FHttpRequestCompleteDelegate;

	SendMockyRequest(0, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 0"), CurrentTime);
		RequestCompleted++;
		SendMockyRequest(1, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 1"), CurrentTime);
			RequestCompleted++;
			SendMockyRequest(2, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
			{
				UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 2"), CurrentTime);
				RequestCompleted++;
				SendMockyRequest(3, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
				{
					UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 3"), CurrentTime);
					RequestCompleted++;
					SendMockyRequest(4, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
					{
						UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Callback Request 4"), CurrentTime);
						RequestCompleted++;
					}));
				}));
			}));
		}));
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);

	SendMockyRequest(5, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		RequestCompleted++;
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);

	SendMockyRequest(6, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		RequestCompleted++;
		SendMockyRequest(7, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			RequestCompleted++;
			SendMockyRequest(8, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
			{
				RequestCompleted++;
			}));
		}));
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);

	SendMockyRequest(9, Del::CreateLambda([&RequestCompleted, &Scheduler, &CurrentTime](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		RequestCompleted++;
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);

	while (CurrentTime < 50.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	AB_TEST_EQUAL(RequestCompleted, 10);

	FRegistry::Credentials.ForgetAll();
	Ticker.RemoveTicker(TickerDelegate1);
	Ticker.RemoveTicker(TickerDelegate2);

	ResetSettings();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(Concurrency_ProcessManyRequests_WithSomeInvalidURLs_AllCompleted, "AccelByte.Tests.Core.HttpRetry.Concurrency.ProcessManyRequests_WithSomeInvalidURLs_AllCompleted", AutomationFlagMaskHttpRetry);
bool Concurrency_ProcessManyRequests_WithSomeInvalidURLs_AllCompleted::RunTest(const FString& Parameter)
{
	auto Scheduler = MakeShared<FHttpRetrySchedulerTestingMode>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;
	TArray<TSharedPtr<FHttpTestWorker>> Tasks;

	auto TickerDelegate = Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
		Scheduler->PollRetry(CurrentTime);

		return true;
	}),
		0.2f);

	int TotalRequests = 24;
	int RequestCompleted = 0;
	int RequestSucceeded = 0;
	int RequestFailed = 0;
	int FirstPass = 0;

	TArray<FHttpRequestPtr> CancelledRequests;

	for (int count = 0; count < TotalRequests; count++)
	{
		FString URL = TEXT("");
		auto Request = FHttpModule::Get().CreateRequest();
		int modulo = count % 4;
		double DelayTime = (double)modulo;

		switch (modulo)
		{
		case 0:
			Request->SetURL(FString::Printf(TEXT("http://www.example.com/?id=%d"), count));
			FirstPass++;

			break;
		case 1:
			//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
			//BadRequest 400, delayed by 10s
			Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=10s&id=%d"), count));

			break;
		case 2:
			//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
			//BadRequest 400, delayed by 10s
			Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c38bc153100006c00a991ed?mocky-delay=10s&id=%d"), count));
			CancelledRequests.Add(Request);

			break;
		case 3:
			//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
			//Sucess 200, delayed by 20s
			Request->SetURL(FString::Printf(TEXT("http://www.mocky.io/v2/5c37fc0330000054001f659d?mocky-delay=20s&id=%d"), count));

			break;
		}

		Request->SetVerb(TEXT("GET"));

		auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda([&RequestCompleted, &RequestSucceeded, &RequestFailed](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			RequestCompleted++;

			if (Request->GetStatus() == EHttpRequestStatus::Succeeded)
			{
				UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("RequestStatus=%s, ResponseCode=%d, URL=%s"), ToString(Request->GetStatus()), Response->GetResponseCode(), *Request->GetURL());
				RequestSucceeded++;
			}
			else
			{
				UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("RequestStatus=%s, URL=%s"), ToString(Request->GetStatus()), *Request->GetURL());
				RequestFailed++;
			}

		});

		FString ThreadName = FString::Printf(TEXT("Concurrency_%03d"), count);
		TSharedPtr<FHttpTestWorker> Task = MakeShared<FHttpTestWorker>(ThreadName, Scheduler, Request, RequestCompleteDelegate, 10.01, DelayTime);
		Tasks.Add(Task);
	}

	CurrentTime = 10.0;
	float SpecifiedDeltaTime = 0.5f; 
	
	while (CurrentTime < 15.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	AB_TEST_EQUAL(RequestFailed, 0);
	AB_TEST_EQUAL(RequestSucceeded, FirstPass);

	for (const auto& Request : CancelledRequests)
	{
		Request->CancelRequest();
	}

	while (CurrentTime < 40.0)
	{
		CurrentTime += SpecifiedDeltaTime;
		Ticker.Tick(SpecifiedDeltaTime);
		FPlatformProcess::Sleep(SpecifiedDeltaTime);
	}

	int FailedNum = CancelledRequests.Num();
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	AB_TEST_EQUAL(RequestFailed, FailedNum);
	AB_TEST_EQUAL(RequestSucceeded, TotalRequests - FailedNum);

	FRegistry::Credentials.ForgetAll();
	Ticker.RemoveTicker(TickerDelegate);

	for (auto& Task : Tasks)
	{
		Task->EnsureCompletion();
	}

	return true;
}

void ResetSettings()
{
	FRegistry::Settings = OriginalSettings;
	FRegistry::Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
}
