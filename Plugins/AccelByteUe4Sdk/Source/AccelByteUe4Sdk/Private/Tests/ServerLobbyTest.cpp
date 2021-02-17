// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerLobby.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteServerCredentials.h"
#include "Models/AccelByteLobbyModels.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerLobbyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerLobbyTest);

const int32 AutomationFlagMaskServerLobby = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

const int STestUserCount = 4;
FString SUserIds[STestUserCount];
Credentials SUserCreds[STestUserCount];
TArray<TSharedPtr<Api::User>> SLobbyUsers;
TArray<TSharedPtr<Api::Lobby>> SLobbies;

bool bSUsersConnected, bSUsersConnectionSuccess, bSUsersConnectionError;
bool bSCreatePartySuccess, bSCreatePartyError, bSInvitePartySuccess, bSGetInvitedNotifSuccess, bSGetInvitedNotifError;
bool bSJoinPartySuccess, bSJoinPartyError, bSLeavePartySuccess, bSLeavePartyError;

FAccelByteModelsPartyGetInvitedNotice SinvitedToPartyResponse;
FAccelByteModelsPartyJoinReponse SjoinPartyResponse;
FAccelByteModelsCreatePartyResponse ScreatePartyResponse;

void SLobbyConnect(int userCount)
{
	if (userCount > STestUserCount)
	{
		userCount = STestUserCount;
	}
	if (userCount <= 0)
	{
		userCount = 1;
	}
	for (int i = 0; i < userCount; i++)
	{
		if (!SLobbies[i]->IsConnected())
		{
			SLobbies[i]->Connect();
		}
		FString text = FString::Printf(TEXT("LobbyConnect: Wait user %d"), i);
		while (!SLobbies[i]->IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogTemp, Log, TEXT("%s"), *text);
			FTicker::GetCoreTicker().Tick(.5f);
		}
	}
}

void SLobbyDisconnect(int userCount)
{
	if (userCount > STestUserCount)
	{
		userCount = STestUserCount;
	}
	if (userCount <= 0)
	{
		userCount = 1;
	}
	for (int i = 0; i < userCount; i++)
	{
		SLobbies[i]->UnbindEvent();
		SLobbies[i]->Disconnect();
	}
}

void ResetResponses()
{
	bSUsersConnected = false;
	bSUsersConnectionSuccess = false;
	bSUsersConnectionError = false;
	bSCreatePartySuccess = false;
	bSCreatePartyError = false;
	bSInvitePartySuccess = false;
	bSJoinPartySuccess = false;
	bSJoinPartyError = false;
	bSLeavePartySuccess = false;
	bSLeavePartyError = false;
}

const auto ServerLobbyErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteServerLobbyTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const auto SConnectSuccessDelegate = Api::Lobby::FConnectSuccess::CreateLambda([]()
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("User connected!"));
	bSUsersConnected = true;
	bSUsersConnectionSuccess = true;
});

const auto SConnectFailedDelegate = FErrorHandler::CreateLambda([](int32 Code, FString Message)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("User failed to connect!"));
	bSUsersConnected = false;
	bSUsersConnectionSuccess = true;
});

const auto SLeavePartyDelegate = Api::Lobby::FPartyLeaveResponse::CreateLambda([](FAccelByteModelsLeavePartyResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Leave Party Success!"));
	bSLeavePartySuccess = true;
	if (result.Code != "0")
	{
		bSLeavePartyError = true;
	}
});

const auto SCreatePartyDelegate = Api::Lobby::FPartyCreateResponse::CreateLambda([](FAccelByteModelsCreatePartyResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Create Party Success!"));
	bSCreatePartySuccess = true;
	ScreatePartyResponse = result;
	if (result.PartyId.IsEmpty())
	{
		bSCreatePartyError = true;
	}
});

const auto SInvitePartyDelegate = Api::Lobby::FPartyInviteResponse::CreateLambda([](FAccelByteModelsPartyInviteResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Invite Party Success!"));
	bSInvitePartySuccess = true;
});

const auto SInvitedToPartyDelegate = Api::Lobby::FPartyGetInvitedNotif::CreateLambda([](FAccelByteModelsPartyGetInvitedNotice result)
{
	SinvitedToPartyResponse = result;
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Invitation!"));
	bSGetInvitedNotifSuccess = true;
	if (result.PartyId.IsEmpty())
	{
		bSGetInvitedNotifError = true;
	}
});

const auto SJoinPartyDelegate = Api::Lobby::FPartyJoinResponse::CreateLambda([](FAccelByteModelsPartyJoinReponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Join Party Success! Member : %d"), result.Members.Num());
	SjoinPartyResponse = result;
	bSJoinPartySuccess = true;
	if (result.Code != "0")
	{
		bSJoinPartyError = false;
	}
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerLobbyTestSetup, "AccelByte.Tests.ServerLobby.A.Setup", AutomationFlagMaskServerLobby);
bool ServerLobbyTestSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess[STestUserCount];
	bool UsersLoginSuccess[STestUserCount];

	for (int i = 0; i < STestUserCount; i++)
	{
		UsersCreationSuccess[i] = false;
		UsersLoginSuccess[i] = false;
		bClientLoginSuccess = false;

		SLobbyUsers.Add(MakeShared<Api::User>(SUserCreds[i], FRegistry::Settings));

		FString Email = FString::Printf(TEXT("lobbyUE4Test+%d@example.com"), i);
		Email.ToLowerInline();
		FString Password = TEXT("123Password123");
		FString DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);
		FString Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
		const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		SLobbyUsers[i]->Register(
			Email,
			Password,
			DisplayName,
			Country,
			format,
			THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
		{
			UsersCreationSuccess[i] = true;
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Test Lobby Server User %d/%d is Created"), i, STestUserCount);

		}),
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Code=%d"), Code);
			if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException || (ErrorCodes)Code == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Test Lobby Server User %d/%d is already"), i, STestUserCount);
			}
			else
			{
				UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Test Lobby Server User %d/%d can't be created"), i, STestUserCount);
			}
		}));

		FlushHttpRequests();
		Waiting(UsersCreationSuccess[i], "Waiting for user created...");

		SLobbyUsers[i]->LoginWithUsername(
			Email,
			Password,
			FVoidHandler::CreateLambda([&]()
		{
			UsersLoginSuccess[i] = true;
			SUserIds[i] = SUserCreds[i].GetUserId();
		}),
			ServerLobbyErrorHandler);

		FlushHttpRequests();
		Waiting(UsersLoginSuccess[i], "Waiting for Login...");

		SLobbies.Add(MakeShared<Api::Lobby>(SUserCreds[i], FRegistry::Settings));
	}

	for (int i = 0; i < STestUserCount; i++)
	{
		check(UsersLoginSuccess[i]);
	}

	bool bLoginServerSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginServerSuccess]()
	{
		bLoginServerSuccess = true;
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("\t\tClient Successfully Login."));
	}), ServerLobbyErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginServerSuccess, "Waiting for Client Login...");

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerLobbyTestTeardown, "AccelByte.Tests.ServerLobby.Z.Teardown", AutomationFlagMaskServerLobby);
bool ServerLobbyTestTeardown::RunTest(const FString& Parameters)
{
	bool bDeleteUsersSuccessful[STestUserCount];
	SLobbies.Reset(0);

	for (int i = 0; i < STestUserCount; i++)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("DeleteUserById (%d/%d)"), i + 1, STestUserCount);
		DeleteUserById(SUserCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Success"));
			bDeleteUsersSuccessful[i] = true;
		}), ServerLobbyErrorHandler);
		FlushHttpRequests();
		Waiting(bDeleteUsersSuccessful[i], "Waiting for user deletion...");
	}

	for (int i = 0; i < STestUserCount; i++)
	{
		check(bDeleteUsersSuccessful[i]);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerLobbyQueryPartyByUserId, "AccelByte.Tests.ServerLobby.B.QueryPartyByUserId_AllOk", AutomationFlagMaskServerLobby);
bool ServerLobbyQueryPartyByUserId::RunTest(const FString& Parameters)
{
	SLobbyConnect(2);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	SLobbies[1]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
	SLobbies[1]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
	SLobbies[1]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	check(!bSCreatePartyError);

	FAccelByteModelsDataPartyResponse queryAfterCreateParty;
	bool queryAfterCreatePartySuccess = false;
	FRegistry::ServerLobby.GetPartyDataByUserId(
		SUserIds[0], 
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda([&](const FAccelByteModelsDataPartyResponse& result)
		{
			queryAfterCreatePartySuccess = true;
			queryAfterCreateParty = result;

		}),
		ServerLobbyErrorHandler);
	Waiting(queryAfterCreatePartySuccess, "Waiting query after creating party");


	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	Waiting(bSGetInvitedNotifSuccess, "Waiting party invitation");
	
	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");
	check(!bSJoinPartyError);

	FlushHttpRequests();
	FAccelByteModelsDataPartyResponse queryAfterInviteToParty0;
	bool queryAfterInviteToParty0Success = false;
	FRegistry::ServerLobby.GetPartyDataByUserId(
		SUserIds[0], 
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda([&](const FAccelByteModelsDataPartyResponse& result)
		{
			queryAfterInviteToParty0Success = true;
			queryAfterInviteToParty0 = result;
		}),
		ServerLobbyErrorHandler);

	FlushHttpRequests();
	FAccelByteModelsDataPartyResponse queryAfterInviteToParty1;
	bool queryAfterInviteToParty1Success = false;
	FRegistry::ServerLobby.GetPartyDataByUserId(
		SUserIds[1], 
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda([&](const FAccelByteModelsDataPartyResponse& result)
		{
			queryAfterInviteToParty1Success = true;
			queryAfterInviteToParty1 = result;
		}),
		ServerLobbyErrorHandler);

	Waiting(queryAfterInviteToParty0Success, "Waiting query after invite to party 0");
	Waiting(queryAfterInviteToParty1Success, "Waiting query after invite to party 1");


	SLobbies[1]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting 1 leave party");
	check(!bSLeavePartyError);

	FlushHttpRequests();
	FAccelByteModelsDataPartyResponse queryAfterLeaveParty0;
	bool queryAfterLeaveParty0Success = false;
	FRegistry::ServerLobby.GetPartyDataByUserId(
		SUserIds[0], 
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda([&](const FAccelByteModelsDataPartyResponse& result)
		{
			queryAfterLeaveParty0Success = true;
			queryAfterLeaveParty0 = result;
		}),
		ServerLobbyErrorHandler);

	FlushHttpRequests();
	FAccelByteModelsDataPartyResponse queryAfterLeaveParty1;
	bool queryAfterLeaveNotFound1 = false;
	bool queryAfterLeaveComplete1 = false;
	FRegistry::ServerLobby.GetPartyDataByUserId(
		SUserIds[1], 
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda([&](const FAccelByteModelsDataPartyResponse& result)
		{
			queryAfterLeaveComplete1 = true;
			queryAfterLeaveParty1 = result;
		}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
		{
			queryAfterLeaveComplete1 = true;
			queryAfterLeaveNotFound1 = true;
		})
	);
	Waiting(queryAfterLeaveParty0Success, "Waiting query after leave party 0");
	Waiting(queryAfterLeaveComplete1, "Waiting query after leave party 1");

	bSLeavePartyError = false;
	bSLeavePartySuccess = false;
	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting 0 leave party");
	check(!bSLeavePartyError);

	FlushHttpRequests();
	FAccelByteModelsDataPartyResponse queryAfterDisbandParty;
	bool queryAfterDisbandPartyNotFound0 = false;
	bool queryAfterDisbandComplete = false;
	FRegistry::ServerLobby.GetPartyDataByUserId(
		SUserIds[0],
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda([&](const FAccelByteModelsDataPartyResponse& result)
		{
			queryAfterDisbandComplete = true;
			queryAfterDisbandParty = result;
		}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
		{
			queryAfterDisbandComplete = true;
			queryAfterDisbandPartyNotFound0 = true;
		})
	);
	Waiting(queryAfterDisbandComplete, "Waiting 0 disband party");

	check(ScreatePartyResponse.PartyId.Equals(queryAfterCreateParty.partyId));

	bool user1FoundAfterJoin = false;
	for (FString id : queryAfterInviteToParty0.members)
	{
		if (id == SUserIds[1])
		{
			user1FoundAfterJoin = true;
		}
	}
	check(user1FoundAfterJoin);
	check(ScreatePartyResponse.PartyId.Equals(queryAfterInviteToParty1.partyId));

	check(queryAfterLeaveNotFound1);
	bool user1FoundAfterLeave = false;
	for (FString id : queryAfterLeaveParty0.members)
	{
		if (id == SUserIds[1])
		{
			user1FoundAfterLeave = true;
		}
	}
	check(!user1FoundAfterLeave);

	check(queryAfterDisbandPartyNotFound0);

	SLobbyDisconnect(2);
	ResetResponses();

	return true;
}