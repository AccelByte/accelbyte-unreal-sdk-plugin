// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "TestUtilities.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteSessionBrowserApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Models/AccelByteSessionBrowserModels.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteSessionBrowserTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteSessionBrowserTest);

const int32 AutomationFlagMaskSessionBrowser = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto SessionBrowserTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogAccelByteSessionBrowserTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

struct SessionBrowserTestUserInfo
{
	const FString EmailPrefix = "SessionBrowser_UE4Test";
	const FString EmailSuffix = "@example.com";
	const FString Password = "Password+123";
	const FString Country = "ID";
	const FString DisplayName = "SessionBrowserBoy";
};
const SessionBrowserTestUserInfo SessionBrowserTestUserInfo_;

const int SessionBrowserUserCount = 2;
TArray<FString> TestSessionBrowserUsersID;
TArray<FString> TestSessionID;
Credentials SessionBrowserUserCreds[SessionBrowserUserCount];
TArray<TSharedPtr<Api::User>> SessionBrowserUsers;
TArray<TSharedPtr<Api::Lobby>> SessionBrowserLobbies;
TArray<TSharedPtr<Api::SessionBrowser>> SessionBrowsers;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SessionBrowserSetup, "AccelByte.Tests.SessionBrowser.A.Setup", AutomationFlagMaskSessionBrowser);
bool SessionBrowserSetup::RunTest(const FString& Parameters)
{
	TArray<bool> UsersCreationSuccess;
	UsersCreationSuccess.Init(false, SessionBrowserUserCount);

	TArray<bool> UsersLoginSuccess;
	UsersLoginSuccess.Init(false, SessionBrowserUserCount);

	TArray<FString> TestEmails;
	TestEmails.Init("", SessionBrowserUserCount);

	TArray<bool> UserStatCodeInitSuccess;
	UserStatCodeInitSuccess.Init(false, SessionBrowserUserCount);

	bool bServerClientLoginSuccess = false;

	// SERVER LOGIN
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bServerClientLoginSuccess]()
        {
            bServerClientLoginSuccess = true;
            UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("\t\tClient Successfully Login."));
        }), SessionBrowserTestErrorHandler);
	Waiting(bServerClientLoginSuccess, "Waiting for Client Login...");
	TArray<FString> LoginUserIDs;
	for (int i = 0; i < SessionBrowserUserCount; i++)
	{
		// USER CREATION
		UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("USER CREATION...%d out of %d"), i + 1, SessionBrowserUserCount);

		TestEmails[i] = FString::Printf(TEXT("%s+%d%s"), *SessionBrowserTestUserInfo_.EmailPrefix, i, *SessionBrowserTestUserInfo_.EmailSuffix);
		TestEmails[i].ToLowerInline();

		SessionBrowserUsers.Add(MakeShared<Api::User>(SessionBrowserUserCreds[i], FRegistry::Settings));

		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7));
		const FString DoB_String = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		bool bUsersCreationDone = false;
		SessionBrowserUsers[i]->Register(TestEmails[i], SessionBrowserTestUserInfo_.Password, SessionBrowserTestUserInfo_.DisplayName, SessionBrowserTestUserInfo_.Country, DoB_String, THandler<FRegisterResponse>::CreateLambda([&i, &UsersCreationSuccess, &bUsersCreationDone](const FRegisterResponse& Response)
		{
			UsersCreationSuccess[i] = true;
			UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("Test User is successfuly created."));
			bUsersCreationDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("Test User is already created."));
			}
			else
			{
				UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("Test User can't be created"));
			}
			bUsersCreationDone = true;
		}));
		Waiting(bUsersCreationDone, "Waiting for user created...");

		// USER LOG IN
		UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("LOG USER IN...%d out of %d"), i + 1, SessionBrowserUserCount);

		SessionBrowserUsers[i]->LoginWithUsername(
			TestEmails[i],
			SessionBrowserTestUserInfo_.Password,
			FVoidHandler::CreateLambda([&i, &UsersLoginSuccess, &LoginUserIDs]()
		{
			LoginUserIDs.Add(FRegistry::Credentials.GetUserId());
			UsersLoginSuccess[i] = true;
			UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("\t\tSuccessfully Login."));
		}), SessionBrowserTestErrorHandler);
		Waiting(UsersLoginSuccess[i], "Waiting for login with user name...");
		UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

		//User connect to lobby
		SessionBrowserLobbies.Add(MakeShared<Api::Lobby>(SessionBrowserUserCreds[i], FRegistry::Settings));
		SessionBrowserLobbies[i]->Connect();
		while (!SessionBrowserLobbies[i]->IsConnected())
		{
			FPlatformProcess::Sleep(.1f);
			UE_LOG(LogAccelByteSessionBrowserTest, Log, TEXT("Connect the signaling: Wait user %d"), i);
			FTicker::GetCoreTicker().Tick(.1f);
		}

		SessionBrowsers.Add(MakeShared<Api::SessionBrowser>(SessionBrowserUserCreds[i], FRegistry::Settings));
	}
	TestSessionBrowserUsersID = LoginUserIDs;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SessionBrowserTearDown, "AccelByte.Tests.SessionBrowser.Z.Teardown", AutomationFlagMaskSessionBrowser);
bool SessionBrowserTearDown::RunTest(const FString& Parameters)
{
	for (auto& SessionID : TestSessionID)
	{
		bool bDeleteSessionBrowser = false;
		FRegistry::SessionBrowser.RemoveGameSession(SessionID, THandler<FAccelByteModelsSessionBrowserData>::CreateLambda([&bDeleteSessionBrowser](const FAccelByteModelsSessionBrowserData &Data)
		{
			bDeleteSessionBrowser = true;
		}), SessionBrowserTestErrorHandler);
		Waiting(bDeleteSessionBrowser, "Waiting for session browser deletion...");
	}
	for (auto &Lobby : SessionBrowserLobbies)
	{
		Lobby->Disconnect();
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SessionBrowserCRUD, "AccelByte.Tests.SessionBrowser.B.CRUD", AutomationFlagMaskSessionBrowser);
bool SessionBrowserCRUD::RunTest(const FString& Parameters)
{
	bool bCreated = false;
	FString SessionType("p2p");
	FString GameMode("FFA");
	FString GameMap("Karimun Java Island");
	FString GameVersion("1.0.1");
	FAccelByteModelsSessionBrowserData Result;
	int UpdatedPlayerCount = 4;
	int UpdatedMaxPlayer = 8;
	auto SettingJson = MakeShared<FJsonObject>();
	SettingJson->SetStringField(TEXT("CUSTOM1"), TEXT("CUSTOM1"));
	SettingJson->SetNumberField(TEXT("CUSTOM2"), 20);
	
	//Create game session
	SessionBrowsers[0]->CreateGameSession(GameMode, GameMap, GameVersion, 1, 8, SettingJson,
		THandler<FAccelByteModelsSessionBrowserData>::CreateLambda([&bCreated, &Result](const FAccelByteModelsSessionBrowserData &Data)
	{
		bCreated = true;
		Result = Data;
	}), SessionBrowserTestErrorHandler);
	Waiting(bCreated, "Waiting for session browser created...");
	check(bCreated);

	//update game session
	bool bUpdated = false;
	FAccelByteModelsSessionBrowserData ResultUpdated;
	SessionBrowsers[0]->UpdateGameSession(Result.Session_id, UpdatedMaxPlayer, UpdatedPlayerCount,
        THandler<FAccelByteModelsSessionBrowserData>::CreateLambda([&bUpdated, &ResultUpdated](const FAccelByteModelsSessionBrowserData &Data)
    {
        bUpdated = true;
        ResultUpdated = Data;
    }), SessionBrowserTestErrorHandler);
	Waiting(bUpdated, "Waiting for session browser updated...");
	check(bUpdated);
	check(ResultUpdated.Game_session_setting.Max_player == UpdatedMaxPlayer);
	check(ResultUpdated.Game_session_setting.Current_player == UpdatedPlayerCount);

	//Query game session
	bool bQueried = false;
	bool bFounded = false;
	FAccelByteModelsSessionBrowserGetResult QueryResult;
	FAccelByteModelsSessionBrowserData Founded;
	SessionBrowsers[1]->GetGameSessions(SessionType, GameMode,
        THandler<FAccelByteModelsSessionBrowserGetResult>::CreateLambda([&bQueried, &QueryResult](const FAccelByteModelsSessionBrowserGetResult &QueryData)
    {
        bQueried = true;
        QueryResult = QueryData;
    }), SessionBrowserTestErrorHandler);
	Waiting(bUpdated, "Waiting for session browser queried...");
	check(bQueried);
	check(QueryResult.Sessions.Num() > 0);

	for(int i = 0; i < QueryResult.Sessions.Num(); i++)
	{
		if(QueryResult.Sessions[i].Session_id == ResultUpdated.Session_id)
		{
			bFounded = true;
			Founded = QueryResult.Sessions[i];
		}
	}
	check(Founded.Game_session_setting.Settings.JsonObject->GetStringField("CUSTOM1").Equals(TEXT("CUSTOM1")));
	check(Founded.Game_session_setting.Settings.JsonObject->GetIntegerField("CUSTOM2") == 20);
	check(bFounded);
	check(Founded.Game_session_setting.Current_player == ResultUpdated.Game_session_setting.Current_player);

	//Remove game session
	bool bRemoved = false;
	SessionBrowsers[0]->RemoveGameSession(Result.Session_id,
        THandler<FAccelByteModelsSessionBrowserData>::CreateLambda([&bRemoved](const FAccelByteModelsSessionBrowserData &Data)
    {
        bRemoved = true;
    }), SessionBrowserTestErrorHandler);
	Waiting(bRemoved, "Waiting for session browser removed...");
	check(bUpdated);
	check(ResultUpdated.Game_session_setting.Max_player == UpdatedMaxPlayer);
	check(ResultUpdated.Game_session_setting.Current_player == UpdatedPlayerCount);

	//Check again if game session really removed
	SessionBrowsers[1]->GetGameSessions(SessionType, GameMode,
        THandler<FAccelByteModelsSessionBrowserGetResult>::CreateLambda([&bQueried, &QueryResult](const FAccelByteModelsSessionBrowserGetResult &QueryData)
    {
        bQueried = true;
        QueryResult = QueryData;
    }), SessionBrowserTestErrorHandler);
	Waiting(bUpdated, "Waiting for session browser queried...");
	check(bQueried);

	bFounded = false;
	for(int i = 0; i < QueryResult.Sessions.Num(); i++)
	{
		if(QueryResult.Sessions[i].Session_id == ResultUpdated.Session_id)
		{
			bFounded = true;
		}
	}
	check(!bFounded);
	
	return true;
}