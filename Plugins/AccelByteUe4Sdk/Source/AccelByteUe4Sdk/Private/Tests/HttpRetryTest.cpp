// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>
#include <ctime>
#include <queue>
#include <vector>

#include "AutomationTest.h"
#include "FileManager.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Runtime/Core/Public/Containers/Ticker.h"

#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteOrderApi.h"
#include "AccelByteRegistry.h"
#include "AccelByteUserApi.h"
#include "AccelByteUserProfileApi.h"
#include "AccelByteUserProfileModels.h"

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
void ResetSettings();

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_GotError500Twice_RetryTwice, "AccelByte.Tests.Core.HttpRetry.ProcessRequest_GotError500Twice_RetryTwice", AutomationFlagMaskHttpRetry);
bool ProcessRequest_GotError500Twice_RetryTwice::RunTest(const FString& Parameter)
{
	FRegistry::Settings.IamServerUrl = "http://accelbyte.example";
	FRegistry::Settings.ClientId = "ClientID";
	FRegistry::Settings.ClientSecret = "ClientSecret";
	FRegistry::Settings.Namespace = "game01";
	FRegistry::Settings.PublisherNamespace = "publisher01";
	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 3600.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

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
		UE_LOG(LogAccelByteHttpRetryTest, Warning, TEXT("%.4f Request Retried"), CurrentTime);
		NumRequestRetry++;
	});
	
	//Official OnProcessRequestComplete
	auto RequestCompleteDelegate = FHttpRequestCompleteDelegate::CreateLambda(
	[&RequestCompleted, &CurrentTime]
	(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed"), CurrentTime);
		RequestCompleted = true;
	});


	CurrentTime = 10.0;
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 16)
	{
		CurrentTime += 0.5;
		Ticker.Tick(0.5);
		FPlatformProcess::Sleep(0.5);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f NumRequestRetry=%d"), CurrentTime, NumRequestRetry);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed=%d"), CurrentTime, RequestCompleted);
	check(!RequestCompleted);
	check(NumRequestRetry >= 2);

	FRegistry::Credentials.ForgetAll();

	ResetSettings();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_GotError403_RefreshToken, "AccelByte.Tests.Core.HttpRetry.ProcessRequest_GotError403_RefreshToken", AutomationFlagMaskHttpRetry);
bool ProcessRequest_GotError403_RefreshToken::RunTest(const FString& Parameter)
{
	//Mock IamServerUrl path prefix
	//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
	//Sucess 200, with json for refresh token:
	//{
	//	"access_token": "some_access_token",
	//	"bans" : []
	//	"display_name": "some_display_name",
	//	"expires_in" : 10,
	//	"jflgs" : 0,
	//	"namespace" : "string",
	//	"permissions" : [],
	//	"platform_id": "string",
	//	"platform_user_id" : "string",
	//	"refresh_token" : "some_refresh_token",
	//	"roles" : [],
	//	"token_type" : "string",
	//	"user_id" : "string"
	//}
	
	FRegistry::Settings.IamServerUrl = "http://www.mocky.io/v2/5c3b2f592e00000f00648857";
	FRegistry::Settings.ClientId = "ClientID";
	FRegistry::Settings.ClientSecret = "ClientSecret";
	FRegistry::Settings.Namespace = "game01";
	FRegistry::Settings.PublisherNamespace = "publisher01";
	FRegistry::Credentials.SetClientCredentials(TEXT("client_id"), TEXT("client_secret"));
	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 100.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

			return true;
		}),
		0.2f);

	auto Request = FHttpModule::Get().CreateRequest();
	//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
	//Forbidden 403
	Request->SetURL("http://www.mocky.io/v2/5c38c0853100007800a99210");
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

	CurrentTime = 10.0;
	check(FRegistry::Credentials.GetTokenState() == Credentials::ETokenState::Valid);
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 15)
	{
		CurrentTime += 0.2;
		Ticker.Tick(0.2);
		FPlatformProcess::Sleep(0.2);
	}

	check(Request->GetResponse()->GetResponseCode() == 403);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("Token State %d"), static_cast<int32>(FRegistry::Credentials.GetTokenState()));
	check(FRegistry::Credentials.GetUserAccessToken().Compare(TEXT("some_access_token")) == 0);
	check(FRegistry::Credentials.GetUserRefreshToken().Compare(TEXT("some_refresh_token")) == 0);

	FRegistry::Credentials.ForgetAll();

	ResetSettings();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(Credentials_OnExpired_Refreshed, "AccelByte.Tests.Core.HttpRetry.Credentials_OnExpired_Refreshed", AutomationFlagMaskHttpRetry);
bool Credentials_OnExpired_Refreshed::RunTest(const FString& Parameter)
{
	//Mock IamServerUrl path prefix
	//Mocky is free third party mock HTTP server. If some day, it disappears, we should use mock http request and response
	//Sucess 200, with json for refresh token:
	//{
	//	"access_token": "some_access_token",
	//	"bans" : []
	//	"display_name": "some_display_name",
	//	"expires_in" : 10,
	//	"jflgs" : 0,
	//	"namespace" : "string",
	//	"permissions" : [],
	//	"platform_id": "string",
	//	"platform_user_id" : "string",
	//	"refresh_token" : "some_refresh_token",
	//	"roles" : [],
	//	"token_type" : "string",
	//	"user_id" : "string"
	//}

	FRegistry::Settings.IamServerUrl = "http://www.mocky.io/v2/5c3b2f592e00000f00648857";
	FRegistry::Settings.ClientId = "ClientID";
	FRegistry::Settings.ClientSecret = "ClientSecret";
	FRegistry::Settings.Namespace = "game01";
	FRegistry::Settings.PublisherNamespace = "publisher01";
	FRegistry::Credentials.SetClientCredentials(TEXT("client_id"), TEXT("client_secret"));
	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 20.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
	{
		UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
		FRegistry::Credentials.PollRefreshToken(CurrentTime);

		return true;
	}),
		0.2f);

	CurrentTime = 10.0;
	check(FRegistry::Credentials.GetTokenState() == Credentials::ETokenState::Valid);

	while (CurrentTime < 25.0)
	{
		CurrentTime += 0.2;
		Ticker.Tick(0.2);
		FPlatformProcess::Sleep(0.2);
	}

	check(FRegistry::Credentials.GetTokenState() == Credentials::ETokenState::Valid);
	check(FRegistry::Credentials.GetUserAccessToken().Compare(TEXT("some_access_token")) == 0);
	check(FRegistry::Credentials.GetUserRefreshToken().Compare(TEXT("some_refresh_token")) == 0);

	FRegistry::Credentials.ForgetAll();
	check(FRegistry::Credentials.GetTokenState() == Credentials::ETokenState::Invalid);

	ResetSettings();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_NoConnection_RequestImmediatelyCompleted, "AccelByte.Disabled.Core.HttpRetry.ProcessRequest_NoConnection_RequestImmediatelyCompleted", AutomationFlagMaskHttpRetry);
bool ProcessRequest_NoConnection_RequestImmediatelyCompleted::RunTest(const FString& Parameter)
{
	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 100.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

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

	CurrentTime = 10.0;
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 70.3 && !RequestCompleted)
	{
		CurrentTime += 0.2;
		Ticker.Tick(0.2);
		FPlatformProcess::Sleep(0.2);
	}

	check(RequestCompleted);
	check(!Request->GetResponse().IsValid());
	check(Request->GetStatus() == EHttpRequestStatus::Failed_ConnectionError);

	FRegistry::Credentials.ForgetAll();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequest_NoResponseFor60s_RequestCancelled, "AccelByte.Tests.Core.HttpRetry.ProcessRequest_NoResponseFor60s_RequestCancelled", AutomationFlagMaskHttpRetry);
bool ProcessRequest_NoResponseFor60s_RequestCancelled::RunTest(const FString& Parameter)
{
	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 100.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

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

	CurrentTime = 10.0;
	Scheduler->ProcessRequest(Request, RequestCompleteDelegate, CurrentTime);

	while (CurrentTime < 70.3 && !RequestCompleted)
	{
		CurrentTime += 0.2;
		Ticker.Tick(0.2);
		FPlatformProcess::Sleep(0.1); //lying to Scheduler, it's actually 30s instead of 60s. So mocky still hold the response, while Scheduler thinks that it's timeout.
	}

	check(RequestCompleted);
	check(!Request->GetResponse().IsValid());
	check(Request->GetStatus() == EHttpRequestStatus::Failed);

	FRegistry::Credentials.ForgetAll();

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessManyRequests_WithValidURL_AllCompleted, "AccelByte.Tests.Core.HttpRetry.ProcessManyRequests_WithValidURL_AllCompleted", AutomationFlagMaskHttpRetry);
bool ProcessManyRequests_WithValidURL_AllCompleted::RunTest(const FString& Parameter)
{

	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 100.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

			return true;
		}),
		0.2f);

	int RequestCompleted = 0;

	for (int i = 0; i < 15; i++)
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

	CurrentTime = 10.0;

	while (CurrentTime < 15.0)
	{
		CurrentTime += 0.2;
		Ticker.Tick(0.2);
		FPlatformProcess::Sleep(0.2);
	}

	check(RequestCompleted == 15);

	FRegistry::Credentials.ForgetAll();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessManyRequests_WithSomeInvalidURLs_AllCompleted, "AccelByte.Tests.Core.HttpRetry.ProcessManyRequests_WithSomeInvalidURLs_AllCompleted", AutomationFlagMaskHttpRetry);
bool ProcessManyRequests_WithSomeInvalidURLs_AllCompleted::RunTest(const FString& Parameter)
{

	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 100.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
	auto Ticker = FTicker::GetCoreTicker();
	double CurrentTime;

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

			return true;
		}),
		0.2f);

	int RequestCompleted = 0;
	int RequestSucceeded = 0;
	int RequestFailed = 0;
	vector<FHttpRequestPtr> CancelledRequests;

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
			CancelledRequests.push_back(Request);

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

	while (CurrentTime < 15.0)
	{
		CurrentTime += 0.5;
		Ticker.Tick(0.5);
		FPlatformProcess::Sleep(0.5);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	check(RequestFailed == 0);
	check(RequestSucceeded == 3);

	for (const auto& request : CancelledRequests)
	{
		request->CancelRequest();
	}

	while (CurrentTime < 25.0)
	{
		CurrentTime += 0.5;
		Ticker.Tick(0.5);
		FPlatformProcess::Sleep(0.5);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	check(RequestFailed == 3);
	check(RequestSucceeded == 6);

	while (CurrentTime < 35.0)
	{
		CurrentTime += 0.5;
		Ticker.Tick(0.5);
		FPlatformProcess::Sleep(0.5);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Failed %d"), CurrentTime, RequestFailed);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Succeeded %d"), CurrentTime, RequestSucceeded);
	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	check(RequestFailed == 3);
	check(RequestSucceeded == 9);

	FRegistry::Credentials.ForgetAll();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ProcessRequestsChain_WithValidURLs_AllCompleted, "AccelByte.Tests.Core.HttpRetry.ProcessRequestsChain_WithValidURLs_AllCompleted", AutomationFlagMaskHttpRetry);
bool ProcessRequestsChain_WithValidURLs_AllCompleted::RunTest(const FString& Parameter)
{

	FRegistry::Credentials.SetUserToken(TEXT("user_access_token"), TEXT("user_refresh_token"), 100.0, TEXT("Id"), "user_display_name", FRegistry::Settings.Namespace);
	auto Scheduler = MakeShared<FHttpRetryScheduler>();
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
	};

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Retry"), CurrentTime);
			Scheduler->PollRetry(CurrentTime, FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Refresh Token"), CurrentTime);
			FRegistry::Credentials.PollRefreshToken(CurrentTime);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([Scheduler, &CurrentTime](float DeltaTime)
		{
			UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Poll Http Module"), CurrentTime);
			FHttpModule::Get().GetHttpManager().Tick(DeltaTime);

			return true;
		}),
		0.2f);


	CurrentTime = 10.0;
	int RequestCompleted = 0;
	using Del = FHttpRequestCompleteDelegate;

	SendMockyRequest(0, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		RequestCompleted++;
		SendMockyRequest(1, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			RequestCompleted++;
			SendMockyRequest(2, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
			{
				RequestCompleted++;
				SendMockyRequest(3, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
				{
					RequestCompleted++;
					SendMockyRequest(4, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
					{
						RequestCompleted++;
					}));
				}));
			}));
		}));
	}));

	SendMockyRequest(5, Del::CreateLambda([&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		RequestCompleted++;
	}));

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

	SendMockyRequest(9, Del::CreateLambda([&RequestCompleted, &Scheduler, &CurrentTime](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		RequestCompleted++;
	}));

	while (CurrentTime < 20.0)
	{
		CurrentTime += 0.5;
		Ticker.Tick(0.5);
		FPlatformProcess::Sleep(0.5);
	}

	UE_LOG(LogAccelByteHttpRetryTest, Log, TEXT("%.4f Request Completed %d"), CurrentTime, RequestCompleted);
	check(RequestCompleted == 10)

	FRegistry::Credentials.ForgetAll();

	ResetSettings();
	return true;
}

void ResetSettings()
{
	FRegistry::Settings.ClientId = GetDefault<UAccelByteSettings>()->ClientId;
	FRegistry::Settings.ClientSecret = GetDefault<UAccelByteSettings>()->ClientSecret;
	FRegistry::Settings.Namespace = GetDefault<UAccelByteSettings>()->Namespace;
	FRegistry::Settings.PublisherNamespace = GetDefault<UAccelByteSettings>()->PublisherNamespace;
	FRegistry::Settings.RedirectURI = GetDefault<UAccelByteSettings>()->RedirectURI;
	FRegistry::Settings.BaseUrl = GetDefault<UAccelByteSettings>()->BaseUrl;
	FRegistry::Settings.IamServerUrl = GetDefault<UAccelByteSettings>()->IamServerUrl;
	FRegistry::Settings.PlatformServerUrl = GetDefault<UAccelByteSettings>()->PlatformServerUrl;
	FRegistry::Settings.LobbyServerUrl = GetDefault<UAccelByteSettings>()->LobbyServerUrl;
	FRegistry::Settings.BasicServerUrl = GetDefault<UAccelByteSettings>()->BasicServerUrl;
	FRegistry::Settings.CloudStorageServerUrl = GetDefault<UAccelByteSettings>()->CloudStorageServerUrl;
	FRegistry::Settings.GameProfileServerUrl = GetDefault<UAccelByteSettings>()->GameProfileServerUrl;
	FRegistry::Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
}