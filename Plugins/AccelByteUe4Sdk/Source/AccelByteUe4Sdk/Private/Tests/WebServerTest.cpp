// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerCustomWebServerApi.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"


using AccelByte::FErrorHandler;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteWebServerTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteWebServerTest);

const int32 AutomationFlagMaskWebServer = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

TSharedPtr<GameServerApi::ServerCustomWebServer> ServerCustomWebServer;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(WebServerInvalidReq, "AccelByte.Tests.WebServer.InvalidRequest", AutomationFlagMaskWebServer);
bool WebServerInvalidReq::RunTest(const FString& Parameters)
{
	const uint16 Port = 8080;
	bool bWebServerSuccess = false;
	FRegistry::ServerCustomWebServer.SetDSClaimDelegate(THandler<FAccelByteModelsDSMMessage>::CreateLambda([](const FAccelByteModelsDSMMessage Result)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Success"));
	}));
	FRegistry::ServerCustomWebServer.SetErrorDelegate(FErrorHandler::CreateLambda([&bWebServerSuccess](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bWebServerSuccess = true;
	}));
	FRegistry::ServerCustomWebServer.Start(Port);

	FAccelByteModelsDSMMessage ReqBody;
	ReqBody.Msg_type = "Test";
	WebServer_Request(Port, "/get", "POST", ReqBody, FSimpleDelegate::CreateLambda([]()
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Success"));
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
	FlushHttpRequests();
	Waiting(bWebServerSuccess, "Waiting...");
	FRegistry::ServerCustomWebServer.Stop();
	check(bWebServerSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(WebServerValidReq, "AccelByte.Tests.WebServer.ValidRequest", AutomationFlagMaskWebServer);
bool WebServerValidReq::RunTest(const FString& Parameters)
{
	const uint16 Port = 8080;
	bool bWebServerSuccess = false;
	FRegistry::ServerCustomWebServer.SetDSClaimDelegate(THandler<FAccelByteModelsDSMMessage>::CreateLambda([&bWebServerSuccess](const FAccelByteModelsDSMMessage Result)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Success"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Req MessageType: %s"), *Result.Msg_type);
		bWebServerSuccess = true;
	}));
	FRegistry::ServerCustomWebServer.SetErrorDelegate(FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
	FRegistry::ServerCustomWebServer.Start(Port);

	FAccelByteModelsDSMMessage ReqBody;
	ReqBody.Msg_type = "Test";
	WebServer_Request(Port, "/claim", "POST", ReqBody, FSimpleDelegate::CreateLambda([]()
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Success"));
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
	FlushHttpRequests();
	Waiting(bWebServerSuccess, "Waiting...");
	FRegistry::ServerCustomWebServer.Stop();
	check(bWebServerSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(WebServerConnectTwice, "AccelByte.Tests.WebServer.ConnectTwice", AutomationFlagMaskWebServer);
bool WebServerConnectTwice::RunTest(const FString& Parameters)
{
	const uint16 Port = 8080;
	bool bWebServerSuccess = false;
	UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Set WebServer Delegate"));
	FRegistry::ServerCustomWebServer.SetDSClaimDelegate(THandler<FAccelByteModelsDSMMessage>::CreateLambda([&bWebServerSuccess](const FAccelByteModelsDSMMessage Result)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Success"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Req MessageType: %s"), *Result.Msg_type);
		bWebServerSuccess = true;
	}));
	FRegistry::ServerCustomWebServer.SetErrorDelegate(FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));

	UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Connect"));
	FRegistry::ServerCustomWebServer.Start(Port);

	FAccelByteModelsDSMMessage ReqBody;
	ReqBody.Msg_type = "Test";
	WebServer_Request(Port, "/claim", "POST", ReqBody, FSimpleDelegate::CreateLambda([]()
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Success"));
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
	FlushHttpRequests();
	Waiting(bWebServerSuccess, "Waiting...");
	FRegistry::ServerCustomWebServer.Stop();
	check(bWebServerSuccess);

	bWebServerSuccess = false;
	UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Set WebServer Delegate"));
	FRegistry::ServerCustomWebServer.SetDSClaimDelegate(THandler<FAccelByteModelsDSMMessage>::CreateLambda([&bWebServerSuccess](const FAccelByteModelsDSMMessage Result)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Success"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Req MessageType: %s"), *Result.Msg_type);
		bWebServerSuccess = true;
	}));
	FRegistry::ServerCustomWebServer.SetErrorDelegate(FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));

	UE_LOG(LogAccelByteWebServerTest, Log, TEXT("WebServer Connect"));
	FRegistry::ServerCustomWebServer.Start(Port);
	WebServer_Request(Port, "/claim", "POST", ReqBody, FSimpleDelegate::CreateLambda([]()
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Success"));
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Request Error"));
		UE_LOG(LogAccelByteWebServerTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
	FlushHttpRequests();
	Waiting(bWebServerSuccess, "Waiting...");
	FRegistry::ServerCustomWebServer.Stop();
	check(bWebServerSuccess);
	return true;
}