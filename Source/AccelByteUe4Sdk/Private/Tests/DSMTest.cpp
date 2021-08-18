// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Misc/AutomationTest.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteEnvironment.h"
#include "Models/AccelByteDSMModels.h"
#include "TestUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

#define ENABLE_RUN_COULD_SERVER_TESTS 0

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteDSMTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteDSMTest);

const int32 AutomationFlagMaskDSM = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto DSMTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteDSMTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

#if ENABLE_RUN_COULD_SERVER_TESTS
//need to add arg -provider=baremetal when run the test, need to run on cloud
IMPLEMENT_SIMPLE_AUTOMATION_TEST(DSMRegisterStaticServer, "AccelByte.Tests.DSM.A.RegisterStaticServer", AutomationFlagMaskDSM);
bool DSMRegisterStaticServer::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Login Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterServerToDSM(7777, FVoidHandler::CreateLambda([&bServerRegisterSuccess]()
	{
		bServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bServerRegisterSuccess, "Waiting for register server Url...");
	AB_TEST_TRUE(bServerRegisterSuccess);

	bool bServerShutdownSuccess = false;
	FRegistry::ServerDSM.SendShutdownToDSM(false, "", FVoidHandler::CreateLambda([&bServerShutdownSuccess]()
	{
		bServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Shutdown Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bServerShutdownSuccess, "Waiting for shutdown server Url...");
	AB_TEST_TRUE(bServerShutdownSuccess);
	return true;
}

//need to add arg -provider=baremetal when run the test, need to run on cloud
IMPLEMENT_SIMPLE_AUTOMATION_TEST(DSMRegisterAsTwoDifferentServerFailed, "AccelByte.Tests.DSM.B.RegisterTwice", AutomationFlagMaskDSM);
bool DSMRegisterAsTwoDifferentServerFailed::RunTest(const FString& Parameters)
{
	FString ServerName = TEXT("AccelByteTesting");
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Login Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bLocalServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM("127.0.0.1", 7777, ServerName, FVoidHandler::CreateLambda([&bLocalServerRegisterSuccess]()
	{
		bLocalServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Local Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bLocalServerRegisterSuccess, "Waiting for register local server Url...");
	AB_TEST_TRUE(bLocalServerRegisterSuccess);

	bool bServerRegisterFailed = false;
	FRegistry::ServerDSM.RegisterServerToDSM(7777, FVoidHandler::CreateLambda([]()
	{
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Cloud Success..!"));
	}), FErrorHandler::CreateLambda([&bServerRegisterFailed](int32 ErrorCode, const FString& ErrorMessage)
	{
		bServerRegisterFailed = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Cloud Failed..! Code: %d | Message: %s"), ErrorCode, *ErrorMessage);
	}));
	Waiting(bServerRegisterFailed, "Waiting for register server Url...");
	AB_TEST_TRUE(bServerRegisterFailed);

	bool bLocalServerShutdownSuccess = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM(ServerName, FVoidHandler::CreateLambda([&bLocalServerShutdownSuccess]()
	{
		bLocalServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Deregister Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bLocalServerShutdownSuccess, "Waiting for deregister server Url...");
	AB_TEST_TRUE(bLocalServerShutdownSuccess);

	bool bCloudServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterServerToDSM(7777, FVoidHandler::CreateLambda([&bCloudServerRegisterSuccess]()
	{
		bCloudServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Cloud Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bCloudServerRegisterSuccess, "Waiting for register server Url...");
	AB_TEST_TRUE(bCloudServerRegisterSuccess);

	bool bCloudServerShutdownSuccess = false;
	FRegistry::ServerDSM.SendShutdownToDSM(false, "", FVoidHandler::CreateLambda([&bCloudServerShutdownSuccess]()
	{
		bCloudServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Shutdown Success..!"));
	}), DSMTestErrorHandler);
	Waiting(bCloudServerShutdownSuccess, "Waiting for shutdown server Url...");
	AB_TEST_TRUE(bCloudServerShutdownSuccess);
	return true;
}
#endif

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DSMRegisterLocalServer, "AccelByte.Tests.DSM.A.RegisterLocalServer", AutomationFlagMaskDSM);
bool DSMRegisterLocalServer::RunTest(const FString& Parameters)
{
	FString ServerName = TEXT("AccelByteTesting");
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Login Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM("127.0.0.1", 7777, ServerName, FVoidHandler::CreateLambda([&bServerRegisterSuccess]()
	{
		bServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Local Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bServerRegisterSuccess, "Waiting for register local server Url...");
	AB_TEST_TRUE(bServerRegisterSuccess);

	bool bServerShutdownSuccess = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM( ServerName, FVoidHandler::CreateLambda([&bServerShutdownSuccess]()
	{
		bServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Deregister Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bServerShutdownSuccess, "Waiting for deregister server Url...");
	AB_TEST_TRUE(bServerShutdownSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DSMRegisterLocalServerWithPublicIP, "AccelByte.Tests.DSM.A.RegisterLocalServerWithPublicIp", AutomationFlagMaskDSM);
bool DSMRegisterLocalServerWithPublicIP::RunTest(const FString& Parameters)
{
	FString ServerName = TEXT("AccelByteTesting");
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Login Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM(7777, ServerName, FVoidHandler::CreateLambda([&bServerRegisterSuccess]()
	{
		bServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Local with public IP Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bServerRegisterSuccess, "Waiting for Register Local with public IP...");
	AB_TEST_TRUE(bServerRegisterSuccess);

	bool bServerShutdownSuccess = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM(ServerName, FVoidHandler::CreateLambda([&bServerShutdownSuccess]()
	{
		bServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Deregister Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bServerShutdownSuccess, "Waiting for deregister server Url...");
	AB_TEST_TRUE(bServerShutdownSuccess);
	return true;
}