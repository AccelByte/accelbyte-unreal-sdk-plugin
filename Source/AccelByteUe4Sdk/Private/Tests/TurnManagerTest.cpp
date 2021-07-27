// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Api/AccelByteQos.h"
#include "Api/AccelByteTurnManagerApi.h"
#include "Core/AccelByteRegistry.h"
#include "TestUtilities.h"
#include "Api/AccelByteUserApi.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteTurnManagerTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteTurnManagerTest);

const int32 AutomationFlagMaskTurnManager = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto TurnManagerTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteTurnManagerTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

struct TurnManagerTestUserInfo
{
	const FString EmailPrefix = "TurnManager_UE4Test";
	const FString EmailSuffix = "@example.com";
	const FString Password = "Password+123";
	const FString Country = "ID";
	const FString DisplayName = "MyDisplayName";
};
const TurnManagerTestUserInfo TurnManagerTestUserInfo_;

TSharedPtr<Api::User> TurnManagerUser;
Credentials TurnManagerUserCred;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TurnManagerSetup, "AccelByte.Tests.TurnManager.A.Setup", AutomationFlagMaskTurnManager);
bool TurnManagerSetup::RunTest(const FString& Parameters)
{
	bool UsersCreationSuccess;
	bool UsersLoginSuccess;
	bool bServerClientLoginSuccess = false;

	// SERVER LOGIN
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bServerClientLoginSuccess]()
        {
            bServerClientLoginSuccess = true;
            UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("\t\tClient Successfully Login."));
        }), TurnManagerTestErrorHandler);
	WaitUntil(bServerClientLoginSuccess, "Waiting for Client Login...");
	FString LoginUserID;
	FString TestEmail = FString::Printf(TEXT("%s%s"), *TurnManagerTestUserInfo_.EmailPrefix, *TurnManagerTestUserInfo_.EmailSuffix);
	TestEmail.ToLowerInline();

	TurnManagerUser = MakeShared<Api::User>(TurnManagerUserCred, FRegistry::Settings, FRegistry::HttpRetryScheduler);

	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7));
	const FString DoB_String = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	bool bUsersCreationDone = false;
	TurnManagerUser->Register(TestEmail, TurnManagerTestUserInfo_.Password, TurnManagerTestUserInfo_.DisplayName, TurnManagerTestUserInfo_.Country, DoB_String, THandler<FRegisterResponse>::CreateLambda([&UsersCreationSuccess, &bUsersCreationDone](const FRegisterResponse& Response)
	{
		UsersCreationSuccess = true;
		UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("Test User is successfuly created."));
		bUsersCreationDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
		{
			UsersCreationSuccess = true;
			UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("Test User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("Test User can't be created: %s"), *Message);
		}
		bUsersCreationDone = true;
	}));
	WaitUntil(bUsersCreationDone, "Waiting for user created...");

	TurnManagerUser->LoginWithUsername(TestEmail, TurnManagerTestUserInfo_.Password, FVoidHandler::CreateLambda([ &UsersLoginSuccess, &LoginUserID]()
		{
			LoginUserID = FRegistry::Credentials.GetUserId();
			UsersLoginSuccess = true;
			UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("\t\tSuccessfully Login."));
		}), TurnManagerTestErrorHandler);
	WaitUntil(UsersLoginSuccess, "Waiting for login with user name...");

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TurnManagerServerListAndLatencies, "AccelByte.Tests.TurnManager.B.GetServerLatencies", AutomationFlagMaskTurnManager);
bool TurnManagerServerListAndLatencies::RunTest(const FString& Parameters)
{
	bool bGetServerLatenciesSuccess = false;
	TArray<TPair<FString, float>> Latencies;
	Api::TurnManager TurnManager(TurnManagerUserCred, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	bool bGetListDone = false;
	int ServerNum = 0;
	TurnManager.GetTurnServers(THandler<FAccelByteModelsTurnServerList>::CreateLambda([&bGetListDone, &ServerNum](const FAccelByteModelsTurnServerList &ResultList)
	{
		UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("Get Server Latencies Success! Count: %d"), ResultList.Servers.Num());
		bGetListDone = true;
		ServerNum = ResultList.Servers.Num();
	}), FErrorHandler::CreateLambda([&bGetListDone, &ServerNum](int32 ErrorCode, const FString& ErrorMessage)
	{
		ServerNum = -1;
		bGetListDone = true;
	}));
	WaitUntil(bGetListDone, "Waiting get server list...");
	AB_TEST_TRUE(ServerNum >= 0);
	if(ServerNum > 0)
	{
		bGetListDone = false;
		bool ClosestError = false;
		TurnManager.GetClosestTurnServer(THandler<FAccelByteModelsTurnServer>::CreateLambda([&bGetListDone](const FAccelByteModelsTurnServer &Result)
		{
			UE_LOG(LogAccelByteTurnManagerTest, Log, TEXT("Closest TURN server REGION: %s, Address: %s:%d"), *Result.Region, *Result.Ip, Result.Port);
			bGetListDone = true;
		}), FErrorHandler::CreateLambda([&bGetListDone, &ClosestError](int32 ErrorCode, const FString& ErrorMessage)
		{
			ClosestError = true;
			bGetListDone = true;
		}));
		WaitUntil(bGetListDone, "Waiting get closest server...");
		AB_TEST_TRUE(!ClosestError);
	}
	return true;
}
