// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteOauth2Api.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteQos.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "TestUtilities.h"
#include "MatchmakingTestAdmin.h"

#include <IPAddress.h>
#include <SocketSubsystem.h>
#include <IWebSocket.h>


using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Oauth2;
using AccelByte::Api::Lobby;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteJoinableSessionTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteJoinableSessionTest);

const int32 AutomationFlagMaskJoinable = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto JoinableSessionTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteJoinableSessionTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int ActiveUserCount = 2;
FString ActiveUserIds[ActiveUserCount];
Credentials ActiveUserCreds[ActiveUserCount];
TArray<TSharedPtr<Api::User>> ActiveUsers;
TArray<TSharedPtr<Api::Lobby>> ActiveLobbies;
FString JoinableChannelName = "";
FString NonJoinableChannelName = "";
FString LocalDSPodName = "";
bool isDSRegistered = false;
bool isSessionQueued = false;
FAccelByteModelsMatchmakingResult DSGetMatchData;

TSharedPtr<Api::Lobby> CreateLobby(Credentials Creds)
{
	TSharedPtr<Api::Lobby> lobby =  MakeShared<Api::Lobby>(Creds, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	lobby->Connect();
	ActiveLobbies.Add(lobby);
	return lobby;
}

bool TestCleanUp()
{
	for(auto lobby : ActiveLobbies)
	{

		if (lobby->IsConnected())
		{
			UE_LOG(LogAccelByteJoinableSessionTest, Warning, TEXT("[LOBBY] Dangling websocket connection, previous test are not closing WS connection, please disconnect it at the of of the tests, disconnecting..."));
			lobby->Disconnect();
		}

	}
	ActiveLobbies.Empty();

	if (isSessionQueued && !DSGetMatchData.Match_id.IsEmpty())
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Dequeueing Joinable Session"));

		bool bDequeueSuccess = false;
		FRegistry::ServerMatchmaking.DequeueJoinableSession(DSGetMatchData.Match_id, FVoidHandler::CreateLambda([&bDequeueSuccess]()
		{
			bDequeueSuccess = true;
		}), JoinableSessionTestErrorHandler);

		Waiting(bDequeueSuccess, "Dequeueing Joinable Session...");

		isSessionQueued = false;
	}

	if (isDSRegistered)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Cleaning Registered Local DS"));

		bool bUnregisterLocalServerSuccess = false;
		FRegistry::ServerDSM.DeregisterLocalServerFromDSM(LocalDSPodName, FVoidHandler::CreateLambda([&bUnregisterLocalServerSuccess]()
		{
			bUnregisterLocalServerSuccess = true;
		}), JoinableSessionTestErrorHandler);

		Waiting(bUnregisterLocalServerSuccess, "Deregistering Local Server...");

		isDSRegistered = false;
		DSGetMatchData = FAccelByteModelsMatchmakingResult();
	}

	return ((ActiveLobbies.Num() == 0) && !isSessionQueued && !isDSRegistered);
}

bool RegisterDS()
{
	//GameServer SDK / Local DS
	LocalDSPodName = "ue4SdkTestJoinableServerName" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
	{
		bServerLoginWithClientCredentialsDone = true;
	}),
		JoinableSessionTestErrorHandler);

	Waiting(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	bool canBind = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString LocalIPStr = LocalIp->IsValid() ? LocalIp->ToString(false) : "";

	bool bRegisterLocalServerToDSMDone = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM(
		LocalIPStr,
		7777,
		LocalDSPodName,
		FVoidHandler::CreateLambda([&bRegisterLocalServerToDSMDone]()
	{
		bRegisterLocalServerToDSMDone = true;
	}),
		JoinableSessionTestErrorHandler);

	Waiting(bRegisterLocalServerToDSMDone, "Local DS Register To DSM");
	return bRegisterLocalServerToDSMDone;
}

bool RegisterSessionAsync(const FAccelByteModelsMatchmakingResult matchmakingResult)
{
	UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Session is %s"), matchmakingResult.Joinable ? TEXT("JOINABLE") : TEXT("NonJoinable"));

	if (matchmakingResult.Joinable)
	{
		bool bEnqueueResultSuccess = false;
		FRegistry::ServerMatchmaking.EnqueueJoinableSession(matchmakingResult, FVoidHandler::CreateLambda([&bEnqueueResultSuccess]()
		{
			bEnqueueResultSuccess = true;
		}), JoinableSessionTestErrorHandler);
		Waiting(bEnqueueResultSuccess, "Wait enqueue joinable session");

		if (!bEnqueueResultSuccess) { return false; }
		isSessionQueued = true;
		return bEnqueueResultSuccess;
	}
	else
	{
		return true;
	}
}

bool GetMatchData()
{
	// DS set query session, enqueue joinable session
	FString sessionId;
	WaitUntil([&sessionId]()
	{
		bool bGetSessionId = false;
		FRegistry::ServerDSM.GetSessionId(THandler<FAccelByteModelsServerSessionResponse>::CreateLambda([&bGetSessionId, &sessionId](const FAccelByteModelsServerSessionResponse& result)
		{
			bGetSessionId = true;
			sessionId = result.Session_id;
		}), JoinableSessionTestErrorHandler);
		Waiting(bGetSessionId, "Waiting GetSessionId");
		return !sessionId.IsEmpty();
	});

	bool bQuerySessionStatusDone = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(sessionId, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&bQuerySessionStatusDone](const FAccelByteModelsMatchmakingResult& result)
	{
		DSGetMatchData = result;
		bQuerySessionStatusDone = true;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionStatusDone, "Waiting QuerySessionStatus");

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestSetup, "AccelByte.Tests.Joinable.A.Setup", AutomationFlagMaskJoinable);
bool JoinableSessionTestSetup::RunTest(const FString& Parameters)
{
	//Create Users
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess[ActiveUserCount];
	bool UsersLoginSuccess[ActiveUserCount];

	for (int i = 0; i < ActiveUserCount; i++)
	{
		UsersCreationSuccess[i] = false;
		UsersLoginSuccess[i] = false;
		bClientLoginSuccess = false;

		ActiveUsers.Add(MakeShared<Api::User>(ActiveUserCreds[i], FRegistry::Settings, FRegistry::HttpRetryScheduler));

		FString Email = FString::Printf(TEXT("joinablelobbyUE4Test+%d-%d@example.com"), i, FMath::RandRange(0, 100000000));
		Email.ToLowerInline();
		FString Password = TEXT("123Password123");
		FString DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);
		FString Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
		const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		ActiveUsers[i]->Register(
			Email, 
			Password, 
			DisplayName, 
			Country, 
			format, 
			THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Test Lobby User %d/%d is Created"), i, ActiveUserCount);

			}), 
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Code=%d"), Code);
				if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException || (ErrorCodes)Code == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
				{
					UsersCreationSuccess[i] = true;
					UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Test Lobby User %d/%d is already"), i, ActiveUserCount);
				}
				else
				{
					UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Test Lobby User %d/%d can't be created"), i, ActiveUserCount);
				}
			}));
		
		Waiting(UsersCreationSuccess[i],"Waiting for user created...");

		ActiveUsers[i]->LoginWithUsername(
			Email,
			Password,
			FVoidHandler::CreateLambda([&]()
			{
				UsersLoginSuccess[i] = true;
				ActiveUserIds[i] = ActiveUserCreds[i].GetUserId();
			}), 
			JoinableSessionTestErrorHandler);
		
		Waiting(UsersLoginSuccess[i],"Waiting for Login...");
	}
	
	for (int i = 0; i < ActiveUserCount; i++)
	{
		AB_TEST_TRUE(UsersLoginSuccess[i]);
	}

	//Create Matchmaking Channel
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 1;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 2;

	JoinableChannelName = "ue4sdktestjoinable" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(JoinableChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler, true);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, 60, "Create Joinable Matchmaking channel...");
	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);

	NonJoinableChannelName = "ue4sdktestnonjoinable" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(NonJoinableChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, 60, "Create NonJoinable Matchmaking channel...");
	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestTeardown, "AccelByte.Tests.Joinable.Z.Teardown", AutomationFlagMaskJoinable);
bool JoinableSessionTestTeardown::RunTest(const FString& Parameters)
{
	//Delete Users
	bool bDeleteUsersSuccessful[ActiveUserCount];
	ActiveLobbies.Reset(0);

	for (int i = 0; i < ActiveUserCount; i++)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("DeleteUserById (%d/%d)"), i + 1, ActiveUserCount);
		DeleteUserById(ActiveUserCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Success"));
			bDeleteUsersSuccessful[i] = true;
		}), JoinableSessionTestErrorHandler);
		Waiting(bDeleteUsersSuccessful[i],"Waiting for user deletion...");
	}

	for (int i = 0; i < ActiveUserCount; i++)
	{
		AB_TEST_TRUE(bDeleteUsersSuccessful[i]);
	}

	//Delete Matchmaking Channel
	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(JoinableChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	Waiting(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);

	bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(NonJoinableChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	Waiting(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestTwoPartyMatchmake, "AccelByte.Tests.Joinable.B.TwoPartyMatchmake_JoinableSession_SameSession", AutomationFlagMaskJoinable);
bool JoinableSessionTestTwoPartyMatchmake::RunTest(const FString& Parameters)
{
	//Arrange
	TSharedPtr<Lobby> lobbyA = CreateLobby(ActiveUserCreds[0]);
	TSharedPtr<Lobby> lobbyB = CreateLobby(ActiveUserCreds[1]);

	WaitUntil([&lobbyA]()
	{
		return lobbyA->IsConnected();
	}, 10, "Lobby Connect...");

	FString AMatchId = "";
	FString BMatchId = "";

	lobbyA->SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	lobbyA->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&lobbyA](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		lobbyA->SendReadyConsentRequest(Result.MatchId);
	}));

	lobbyB->SetDsNotifDelegate(THandler<FAccelByteModelsDsNotice>::CreateLambda([&BMatchId](const FAccelByteModelsDsNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("B Received DS Update, MatchID %s"),  *Result.MatchId);
		BMatchId = Result.MatchId;
	}));

	bool leavePartyDoneA = false;
	lobbyA->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	lobbyA->SendLeavePartyRequest();
	Waiting(leavePartyDoneA, "wait leaving party A");

	bool leavePartyDoneB = false;
	lobbyB->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneB](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneB = true;
	}));
	lobbyB->SendLeavePartyRequest();
	Waiting(leavePartyDoneB, "wait leaving party B");

	// register local DS
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "A Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 15);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	Waiting(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	Waiting(bRegisterSessionSuccess, "Waiting Session Register...");

	WaitSecond(3, "Waiting backend session sync");

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bIsCreatePartySuccess = false;
	lobbyB->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyB->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby B creating party");

	FAccelByteModelsMatchmakingResponse BMatchmakingResult;
	bIsStartMatchmakingSuccess = false;
	lobbyB->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&BMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		BMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyB->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "B Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&BMatchId]()
	{
		return !BMatchId.IsEmpty();
	}, 20);


	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	AB_TEST_FALSE(AMatchId.IsEmpty());
	AB_TEST_FALSE(BMatchId.IsEmpty());
	AB_TEST_EQUAL(AMatchId, BMatchId);

	return TestCleanUp();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestAddRemovePlayerManual, "AccelByte.Tests.Joinable.B.AddRemovePlayerManual_JoinableSession_AllOk", AutomationFlagMaskJoinable);
bool JoinableSessionTestAddRemovePlayerManual::RunTest(const FString& Parameters)
{
	//Arrange
	TSharedPtr<Lobby> lobbyA = CreateLobby(ActiveUserCreds[0]);

	WaitUntil([&lobbyA]()
	{
		return lobbyA->IsConnected();
	}, 10, "Lobby Connect...");

	FString AMatchId = "";

	lobbyA->SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	lobbyA->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&lobbyA](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		lobbyA->SendReadyConsentRequest(Result.MatchId);
	}));

	bool leavePartyDoneA = false;
	lobbyA->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	lobbyA->SendLeavePartyRequest();
	Waiting(leavePartyDoneA, "wait leaving party A");

	// register local DS
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 15);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	Waiting(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	Waiting(bRegisterSessionSuccess, "Waiting Session Register...");

	WaitSecond(3, "Waiting backend syncing session");

	bool bAddUserSuccess = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bAddUserSuccess]()
	{
		bAddUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	Waiting(bAddUserSuccess, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	Waiting(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");
	
	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	// Assertions

	AB_TEST_FALSE(AMatchId.IsEmpty());

	bool userIdExist = false;
	for(auto ally : DSGetMatchData.Matching_allies)
	{
		for(auto party : ally.Matching_parties)
		{
			for(auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist) break;
			}
			if (userIdExist) break;
		}
		if (userIdExist) break;
	}
	AB_TEST_FALSE(userIdExist);
	AB_TEST_TRUE(bAddUserSuccess);

	userIdExist = false;
	for (auto ally : mmResultAfterAddUser.Matching_allies)
	{
		for (auto party : ally.Matching_parties)
		{
			for (auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist) break;
			}
			if (userIdExist) break;
		}
		if (userIdExist) break;
	}
	AB_TEST_TRUE(userIdExist);
	AB_TEST_TRUE(bRemoveUserSuccess);

	userIdExist = false;
	for (auto ally : mmResultAfterRemoveUser.Matching_allies)
	{
		for (auto party : ally.Matching_parties)
		{
			for (auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist) break;
			}
			if (userIdExist) break;
		}
		if (userIdExist) break;
	}
	AB_TEST_FALSE(userIdExist);
	return TestCleanUp();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestAddRemovePlayerPartyParam, "AccelByte.Tests.Joinable.B.AddRemovePlayerManual_JoinableSession_WithOptionalPartyParam_AllOk", AutomationFlagMaskJoinable);
bool JoinableSessionTestAddRemovePlayerPartyParam::RunTest(const FString& Parameters)
{
	//Arrange
	TSharedPtr<Lobby> lobbyA = CreateLobby(ActiveUserCreds[0]);
	TSharedPtr<Lobby> lobbyB = CreateLobby(ActiveUserCreds[1]);

	WaitUntil([&lobbyA]()
	{
		return lobbyA->IsConnected();
	}, 10, "Lobby Connect...");

	FString AMatchId = "";

	lobbyA->SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	lobbyA->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&lobbyA](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		lobbyA->SendReadyConsentRequest(Result.MatchId);
	}));

	bool leavePartyDoneA = false;
	lobbyA->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	lobbyA->SendLeavePartyRequest();
	Waiting(leavePartyDoneA, "wait leaving party A");

	bool leavePartyDoneB = false;
	lobbyB->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneB](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneB = true;
	}));
	lobbyB->SendLeavePartyRequest();
	Waiting(leavePartyDoneB, "wait leaving party B");

	// register local DS
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 15);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	Waiting(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	Waiting(bRegisterSessionSuccess, "Waiting Session Register...");

	WaitSecond(3, "Waiting backend to sync session");

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bIsCreatePartySuccess = false;
	lobbyB->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyB->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby B creating party");

	bool bAddUserSuccess = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bAddUserSuccess]()
	{
		bAddUserSuccess = true;
	}), JoinableSessionTestErrorHandler, BCreatePartyResult.PartyId);
	Waiting(bAddUserSuccess, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	Waiting(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	// Assertions
	AB_TEST_FALSE(AMatchId.IsEmpty());
	bool userIdExist = false;
	bool partyIdExist = false;
	for(auto ally : DSGetMatchData.Matching_allies)
	{
		for(auto party : ally.Matching_parties)
		{
			if (party.Party_id == BCreatePartyResult.PartyId)
			{
				partyIdExist = true;
				break;
			}
			for(auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist || partyIdExist) break;
			}
			if (userIdExist || partyIdExist) break;
		}
		if (userIdExist || partyIdExist) break;
	}
	AB_TEST_FALSE(partyIdExist);
	AB_TEST_FALSE(userIdExist);

	AB_TEST_TRUE(bAddUserSuccess);
	userIdExist = false;
	partyIdExist = false;
	for(auto ally : mmResultAfterAddUser.Matching_allies)
	{
		for(auto party : ally.Matching_parties)
		{
			if (party.Party_id == BCreatePartyResult.PartyId)
			{
				partyIdExist = true;
			}
			for(auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
				}
				if (userIdExist && partyIdExist) break;
			}
			if (userIdExist && partyIdExist) break;
		}
		if (userIdExist && partyIdExist) break;
	}
	AB_TEST_TRUE(partyIdExist);
	AB_TEST_TRUE(userIdExist);

	AB_TEST_TRUE(bRemoveUserSuccess);
	userIdExist = false;
	partyIdExist = false;
	for(auto ally : mmResultAfterRemoveUser.Matching_allies)
	{
		for(auto party : ally.Matching_parties)
		{
			if (party.Party_id == BCreatePartyResult.PartyId)
			{
				partyIdExist = true;
				break;
			}
			for(auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist || partyIdExist) break;
			}
			if (userIdExist || partyIdExist) break;
		}
		if (userIdExist || partyIdExist) break;
	}
	AB_TEST_FALSE(partyIdExist);
	AB_TEST_FALSE(userIdExist);
	return TestCleanUp();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestNonJoinable, "AccelByte.Tests.Joinable.B.TwoPartyMatchmake_NonJoinableSession_DifferentSession", AutomationFlagMaskJoinable);
bool JoinableSessionTestNonJoinable::RunTest(const FString& Parameters)
{
	//Arrange
	TSharedPtr<Lobby> lobbyA = CreateLobby(ActiveUserCreds[0]);
	TSharedPtr<Lobby> lobbyB = CreateLobby(ActiveUserCreds[1]);

	WaitUntil([&lobbyA, &lobbyB]()
	{
		return lobbyA->IsConnected() && lobbyB->IsConnected();
	}, 10, "Lobby Connect...");

	FString AMatchId = "";
	FString BMatchId = "";

	lobbyA->SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	lobbyA->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&lobbyA](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		lobbyA->SendReadyConsentRequest(Result.MatchId);
	}));

	lobbyB->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&lobbyB](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		lobbyB->SendReadyConsentRequest(Result.MatchId);
	}));

	lobbyB->SetDsNotifDelegate(THandler<FAccelByteModelsDsNotice>::CreateLambda([&BMatchId](const FAccelByteModelsDsNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("B Received DS Update, MatchID %s"), *Result.MatchId);
		BMatchId = Result.MatchId;
	}));

	bool leavePartyDoneA = false;
	lobbyA->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	lobbyA->SendLeavePartyRequest();
	Waiting(leavePartyDoneA, "wait leaving party A");

	bool leavePartyDoneB = false;
	lobbyB->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneB](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneB = true;
	}));
	lobbyB->SendLeavePartyRequest();
	Waiting(leavePartyDoneB, "wait leaving party B");

	// register local DS
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 15);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	Waiting(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	Waiting(bRegisterSessionSuccess, "Waiting Session Register...");

	WaitSecond(3, "Waiting backend to sync session");

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bIsCreatePartySuccess = false;
	lobbyB->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyB->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby B creating party");

	FAccelByteModelsMatchmakingResponse BMatchmakingResult;
	bIsStartMatchmakingSuccess = false;
	lobbyB->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&BMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		BMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyB->SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&BMatchId]()
	{
		return !BMatchId.IsEmpty();
	}, 20);


	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	AB_TEST_FALSE(AMatchId.IsEmpty());
	AB_TEST_FALSE(BMatchId.IsEmpty());
	AB_TEST_NOT_EQUAL(AMatchId, BMatchId);

	return TestCleanUp();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestAddRemovePlayerNonJoinable, "AccelByte.Tests.Joinable.B.AddRemovePlayerManual_NonJoinableSession_AddFailed_RemoveOk", AutomationFlagMaskJoinable);
bool JoinableSessionTestAddRemovePlayerNonJoinable::RunTest(const FString& Parameters)
{
	//Arrange
	TSharedPtr<Lobby> lobbyA = CreateLobby(ActiveUserCreds[0]);

	WaitUntil([&lobbyA]()
	{
		return lobbyA->IsConnected();
	}, 10, "Lobby Connect...");

	FString AMatchId = "";

	lobbyA->SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	lobbyA->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&lobbyA](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		lobbyA->SendReadyConsentRequest(Result.MatchId);
	}));

	bool leavePartyDone = false;
	lobbyA->SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDone](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDone = true;
	}));
	lobbyA->SendLeavePartyRequest();
	Waiting(leavePartyDone, "wait leaving party");

	// register local DS
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	Waiting(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 15);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	Waiting(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	Waiting(bRegisterSessionSuccess, "Waiting Session Register...");

	WaitSecond(3, "Waiting backend to sync session");

	bool bAddUserSuccess = false;
	bool bAddUserDone = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bAddUserSuccess, &bAddUserDone]()
	{
		bAddUserSuccess = true;
		bAddUserDone = true;
	}), FErrorHandler::CreateLambda([&bAddUserDone](int32 Code, const FString& Message)
	{
		bAddUserDone = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Add User Error: %d | %s"), Code, *Message);
	}));
	Waiting(bAddUserDone, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUserCreds[0].GetUserId(), FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	Waiting(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	// Assertions

	AB_TEST_FALSE(AMatchId.IsEmpty());

	bool userIdExist = false;
	for (auto ally : DSGetMatchData.Matching_allies)
	{
		for (auto party : ally.Matching_parties)
		{
			for (auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist) break;
			}
			if (userIdExist) break;
		}
		if (userIdExist) break;
	}
	AB_TEST_FALSE(userIdExist);
	AB_TEST_FALSE(bAddUserSuccess);
	AB_TEST_TRUE(bAddUserDone);

	userIdExist = false;
	for (auto ally : mmResultAfterAddUser.Matching_allies)
	{
		for (auto party : ally.Matching_parties)
		{
			for (auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[1].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist) break;
			}
			if (userIdExist) break;
		}
		if (userIdExist) break;
	}
	AB_TEST_FALSE(userIdExist);
	AB_TEST_TRUE(bRemoveUserSuccess);

	userIdExist = false;
	for (auto ally : mmResultAfterRemoveUser.Matching_allies)
	{
		for (auto party : ally.Matching_parties)
		{
			for (auto member : party.Party_members)
			{
				if (member.User_id == ActiveUserCreds[0].GetUserId())
				{
					userIdExist = true;
					break;
				}
				if (userIdExist) break;
			}
			if (userIdExist) break;
		}
		if (userIdExist) break;
	}
	AB_TEST_FALSE(userIdExist);
	return TestCleanUp();
}