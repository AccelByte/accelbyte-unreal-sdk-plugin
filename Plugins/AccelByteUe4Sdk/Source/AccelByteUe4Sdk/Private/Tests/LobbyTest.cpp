// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteUserApi.h"
#include "AccelByteRegistry.h"
#include "AccelByteLobbyApi.h"
#include "TestUtilities.h"
#include "FileManager.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Oauth2;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLobbyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLobbyTest);

const int32 AutomationFlagMaskLobby = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto LobbyTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteLobbyTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int TestUserCount = 2;
FString UserIds[TestUserCount];
Credentials UserCreds[TestUserCount];
TArray<TSharedPtr<Api::Lobby>> Lobbies;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSetup, "AccelByte.Tests.Lobby.A.Setup", AutomationFlagMaskLobby);
bool LobbyTestSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess[TestUserCount];
	bool UsersLoginSuccess[TestUserCount];

	User::LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Client Login Success"));
	}), LobbyTestErrorHandler);
	FlushHttpRequests();

	for (int i = 0; i < TestUserCount; i++)
	{
		FString Email = FString::Printf(TEXT("lobbyUE4Test+%d@example.com"), i);
		FString Password = TEXT("Password");
		FString DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);

		AccelByte::Api::User::Register(Email, Password, DisplayName, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
		{
			UsersCreationSuccess[i] = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d is Created"), i, TestUserCount);
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if (Code == EHttpResponseCodes::Conflict)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d is already"), i, TestUserCount);
			}
			else
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d can't be created"), i, TestUserCount);
			}
		}));
		FlushHttpRequests();

		Oauth2::GetAccessTokenWithPasswordGrant(
			FRegistry::Settings.ClientId, 
			FRegistry::Settings.ClientSecret, 
			Email, 
			Password, 
			THandler<FOauth2Token>::CreateLambda([&](const FOauth2Token& Token)
		{
			UsersLoginSuccess[i] = true;
			UserCreds[i].SetUserToken(Token.Access_token, Token.Refresh_token, Token.Expires_in, Token.User_id, Token.Display_name, Token.Namespace);
			UserIds[i] = Token.User_id;
		}), LobbyTestErrorHandler);
		FlushHttpRequests();

		Lobbies.Add(MakeShared<Api::Lobby>(UserCreds[i], FRegistry::Settings));
	}

	check(bClientLoginSuccess);
	for (int i = 0; i < TestUserCount; i++)
	{
		check(UsersLoginSuccess[i]);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestTeardown, "AccelByte.Tests.Lobby.Z.Teardown", AutomationFlagMaskLobby);
bool LobbyTestTeardown::RunTest(const FString& Parameters)
{
	bool bDeleteUsersSuccessful[TestUserCount];
	Lobbies.Reset(0);

	for (int i = 0; i < TestUserCount; i++)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DeleteUserById (%d/%d)"), i + 1, TestUserCount);
		DeleteUserById(UserCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bDeleteUsersSuccessful[i] = true;
		}), LobbyTestErrorHandler);
		FlushHttpRequests();
	}

	for (int i = 0; i < TestUserCount; i++)
	{
		check(bDeleteUsersSuccessful[i]);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnect2Users, "AccelByte.Tests.Lobby.B.2UsersConnect", AutomationFlagMaskLobby);
bool LobbyTestConnect2Users::RunTest(const FString& Parameters)
{
	bool UsersConnected[2];
	bool UsersConnectionResponded[2];
	Lobbies[0]->SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 0 connected!"))
		UsersConnected[0] = true;
		UsersConnectionResponded[0] = true;
	}));
	Lobbies[0]->SetConnectFailedDelegate(FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 0 failed to connect!"))
		UsersConnected[0] = false;
		UsersConnectionResponded[0] = true;
	}));
	Lobbies[1]->SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 1 connected!"))
		UsersConnected[1] = true;
		UsersConnectionResponded[1] = true;
	}));
	Lobbies[1]->SetConnectFailedDelegate(FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 1 failed to connect!"))
		UsersConnected[1] = false;
		UsersConnectionResponded[1] = true;
	}));

	Lobbies[0]->Connect();
	Lobbies[1]->Connect();

	while (!Lobbies[0]->IsConnected() || !UsersConnectionResponded[0])
	{
		FPlatformProcess::Sleep(.2f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Wait user 0"));
		FTicker::GetCoreTicker().Tick(.2f);
	}

	while (!Lobbies[1]->IsConnected() || !UsersConnectionResponded[1]) 
	{
		FPlatformProcess::Sleep(.2f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Wait user 1"));
		FTicker::GetCoreTicker().Tick(.2f);
	}

	for (int i = 0; i < 2; i++)
	{
		check(UsersConnected[i])
		check(UsersConnectionResponded[i])
	}
	Lobbies[0]->Disconnect();
	Lobbies[1]->Disconnect();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnectUser, "AccelByte.Tests.Lobby.B.UserConnect", AutomationFlagMaskLobby);
bool LobbyTestConnectUser::RunTest(const FString& Parameters)
{
	Api::Lobby Lobby(UserCreds[0], FRegistry::Settings);


	bool UsersConnected;
	bool UsersConnectionResponded;
	Lobby.SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 0 connected!"))
		UsersConnected = true;
		UsersConnectionResponded = true;
	}));
	Lobby.SetConnectFailedDelegate(FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 0 failed to connect!"))
		UsersConnected = false;
		UsersConnectionResponded = true;
	}));

	Lobby.Connect();
	while (!Lobby.IsConnected() || !UsersConnectionResponded) 
	{
		FPlatformProcess::Sleep(.2f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Wait user 0"));
		FTicker::GetCoreTicker().Tick(.2f);
	}

	for (int i = 0; i < 2; i++)
	{
		check(UsersConnected)
		check(UsersConnectionResponded)
	}
	Lobby.Disconnect();
	return true;
}

