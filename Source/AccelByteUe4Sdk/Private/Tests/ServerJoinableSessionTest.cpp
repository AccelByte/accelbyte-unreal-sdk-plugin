// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteQos.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
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
using AccelByte::Api::Lobby;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteJoinableSessionTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteJoinableSessionTest);

const int32 AutomationFlagMaskJoinable = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto JoinableSessionTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteJoinableSessionTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int ActiveUserCount = 2;
const float DSNotifWaitTime = 130;
TArray<FTestUser> ActiveUsers;
TArray<FApiClientPtr> ActiveApiClients;
FString JoinableChannelName = "";
FString NonJoinableChannelName = "";
FString LocalDSPodName = "";
bool isDSRegistered = false;
bool isSessionQueued = false;
FAccelByteModelsMatchmakingResult DSGetMatchData;

void ActiveLobbyConnect(int UserCount)
{
	if (UserCount > ActiveUserCount)
	{
		UserCount = ActiveUserCount;
	}
	if (UserCount <= 0)
	{
		UserCount = 1;
	}
	for (int Index = 0; Index < UserCount; Index++)
	{
		FApiClientPtr ApiClient = ActiveApiClients[Index];
		if (!ApiClient->Lobby.IsConnected())
		{
			ApiClient->Lobby.Connect();
		}
		
		while (!ApiClient->Lobby.IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("LobbyConnect: Wait user %s to connect")
				, *ApiClient->CredentialsRef->GetUserId());
			FTicker::GetCoreTicker().Tick(.5f);
		}
	}
}

void ActiveLobbyDisconnect(int UserCount)
{
	if (UserCount > ActiveUserCount)
	{
		UserCount = ActiveUserCount;
	}
	if (UserCount <= 0)
	{
		UserCount = 1;
	}
	for (int Index = 0; Index < UserCount; Index++)
	{
		FApiClientPtr ApiClient = ActiveApiClients[Index];
		ApiClient->Lobby.UnbindEvent();
		ApiClient->Lobby.Disconnect();
		while (ApiClient->Lobby.IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Lobby Disconnect: Waiting user %s to disconnect")
				, *ApiClient->CredentialsRef->GetUserId());
			FTicker::GetCoreTicker().Tick(.5f);
		}
	}
}

bool TestCleanUp()
{
	ActiveLobbyDisconnect(ActiveUserCount);

	if (isSessionQueued && !DSGetMatchData.Match_id.IsEmpty())
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Dequeueing Joinable Session"));

		bool bDequeueSuccess = false;
		FRegistry::ServerMatchmaking.DequeueJoinableSession(DSGetMatchData.Match_id, FVoidHandler::CreateLambda([&bDequeueSuccess]()
		{
			bDequeueSuccess = true;
		}), JoinableSessionTestErrorHandler);

		WaitUntil(bDequeueSuccess, "Dequeueing Joinable Session...");

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

		WaitUntil(bUnregisterLocalServerSuccess, "Deregistering Local Server...");

		isDSRegistered = false;
		DSGetMatchData = FAccelByteModelsMatchmakingResult();
	}

	return !isSessionQueued && !isDSRegistered;
}

bool RegisterDS()
{
	//GameServer SDK / Local DS
	LocalDSPodName = "ue4SdkTestJoinableServerName" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			})
		, JoinableSessionTestErrorHandler);

	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	bool canBind = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString LocalIPStr = LocalIp->IsValid() ? LocalIp->ToString(false) : "";

	bool bRegisterLocalServerToDSMDone = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM(LocalIPStr
		, 7777
		, LocalDSPodName
		, FVoidHandler::CreateLambda([&bRegisterLocalServerToDSMDone]()
			{
				bRegisterLocalServerToDSMDone = true;
			})
		, JoinableSessionTestErrorHandler);

	WaitUntil(bRegisterLocalServerToDSMDone, "Local DS Register To DSM");

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
		WaitUntil(bEnqueueResultSuccess, "Wait enqueue joinable session");

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
		WaitUntil(bGetSessionId, "Waiting GetSessionId");
		return !sessionId.IsEmpty();
	});

	bool bQuerySessionStatusDone = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(sessionId, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&bQuerySessionStatusDone](const FAccelByteModelsMatchmakingResult& result)
	{
		DSGetMatchData = result;
		bQuerySessionStatusDone = true;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionStatusDone, "Waiting QuerySessionStatus");

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

		FTestUser TestUser{i};
		TestUser.Email = FString::Printf(TEXT("joinablelobbyUE4Test+%d-%d@example.com"), i, FMath::RandRange(0, 100000000));
		TestUser.Email.ToLowerInline();
		TestUser.Password = TEXT("123Password123");
		TestUser.DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);
		TestUser.Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
		TestUser.DateOfBirth = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		UsersCreationSuccess[i] = RegisterTestUser(TestUser);

		UsersLoginSuccess[i] = LoginTestUser(TestUser);

		ActiveUsers.Add(TestUser);

		ActiveApiClients.Add(FMultiRegistry::GetApiClient(TestUser.Email));
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
	AdminCreateMatchmakingChannel(JoinableChannelName
		, AllianceRule
		, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
		{
			bCreateMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Create Matchmaking Channel Success..!"));
		})
		, JoinableSessionTestErrorHandler
		, true);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Joinable Matchmaking channel...", 60);
	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);

	NonJoinableChannelName = "ue4sdktestnonjoinable" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(NonJoinableChannelName
		, AllianceRule
		, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
		{
			bCreateMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Create Matchmaking Channel Success..!"));
		})
		, JoinableSessionTestErrorHandler);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create NonJoinable Matchmaking channel...", 60);
	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);

	DelaySeconds(60.0f, TEXT("Waiting 60s for lobby service to refresh matchmaking game mode cache from MM service"));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestTeardown, "AccelByte.Tests.Joinable.Z.Teardown", AutomationFlagMaskJoinable);
bool JoinableSessionTestTeardown::RunTest(const FString& Parameters)
{
	//Delete Users
	ActiveLobbyDisconnect(ActiveUserCount);

	AB_TEST_TRUE(TeardownTestUsers(ActiveUsers));

	//Delete Matchmaking Channel
	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(JoinableChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);

	bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(NonJoinableChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestTwoPartyMatchmake, "AccelByte.Tests.Joinable.B.TwoPartyMatchmake_JoinableSession_SameSession", AutomationFlagMaskJoinable);
bool JoinableSessionTestTwoPartyMatchmake::RunTest(const FString& Parameters)
{
	//Arrange
	ActiveLobbyConnect(2);

	FString AMatchId = "";
	FString BMatchId = "";

	ActiveApiClients[0]->Lobby.SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	ActiveApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		ActiveApiClients[0]->Lobby.SendReadyConsentRequest(Result.MatchId);
	}));

	ActiveApiClients[1]->Lobby.SetDsNotifDelegate(THandler<FAccelByteModelsDsNotice>::CreateLambda([&BMatchId](const FAccelByteModelsDsNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("B Received DS Update, MatchID %s"),  *Result.MatchId);
		BMatchId = Result.MatchId;
	}));

	bool leavePartyDoneA = false;
	ActiveApiClients[0]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	ActiveApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneA, "wait leaving party A");

	bool leavePartyDoneB = false;
	ActiveApiClients[1]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneB](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneB = true;
	}));
	ActiveApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneB, "wait leaving party B");

	// register local DS
	isDSRegistered = RegisterDS();
	WaitUntil(isDSRegistered, "Waiting DS Register...");

	bool bGetServerInfoDone = false;
	FAccelByteModelsServerInfo ServerInfo;
	FRegistry::ServerDSM.GetServerInfo(THandler<FAccelByteModelsServerInfo>::CreateLambda(
			[&bGetServerInfoDone, &ServerInfo](const FAccelByteModelsServerInfo& result)
			{
				ServerInfo = result;
				bGetServerInfoDone = true;
				UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Server Info Found, Pod Name %s"), *result.Pod_name);
			})
		, FErrorHandler::CreateLambda([&bGetServerInfoDone](int32 ErrorCode, FString ErrorMessage)
			{
				bGetServerInfoDone = true;
				UE_LOG(LogAccelByteJoinableSessionTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			})
		);
	WaitUntil(bGetServerInfoDone, "Waiting Get Server Info");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	ActiveApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	ActiveApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "A Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, "Waiting receive DS Notif", DSNotifWaitTime);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	WaitUntil(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	WaitUntil(bRegisterSessionSuccess, "Waiting Session Register...");

	DelaySeconds(3, "Waiting backend session sync");

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bIsCreatePartySuccess = false;
	ActiveApiClients[1]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[1]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby B creating party");

	FAccelByteModelsMatchmakingResponse BMatchmakingResult;
	bIsStartMatchmakingSuccess = false;
	ActiveApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&BMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		BMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[1]->Lobby.SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "B Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&BMatchId]()
	{
		return !BMatchId.IsEmpty();
	}, "", 20);

	for (int i = ActiveApiClients.Num() - 1; i >= 0; i--)
	{
		ActiveApiClients[i]->Lobby.SendLeavePartyRequest();
		ActiveApiClients[i]->Lobby.Disconnect();
	}

	AB_TEST_FALSE(ServerInfo.Pod_name.IsEmpty());
	AB_TEST_FALSE(AMatchId.IsEmpty());
	AB_TEST_FALSE(BMatchId.IsEmpty());
	AB_TEST_EQUAL(AMatchId, BMatchId);

	return TestCleanUp();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestAddRemovePlayerManual, "AccelByte.Tests.Joinable.B.AddRemovePlayerManual_JoinableSession_AllOk", AutomationFlagMaskJoinable);
bool JoinableSessionTestAddRemovePlayerManual::RunTest(const FString& Parameters)
{
	//Arrange
	ActiveLobbyConnect(1);

	FString AMatchId = "";

	ActiveApiClients[0]->Lobby.SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	ActiveApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		ActiveApiClients[0]->Lobby.SendReadyConsentRequest(Result.MatchId);
	}));

	bool leavePartyDoneA = false;
	ActiveApiClients[0]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	ActiveApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneA, "wait leaving party A");

	// register local DS
	isDSRegistered = RegisterDS();
	WaitUntil(isDSRegistered, "Waiting DS Register...");
	
	bool bGetServerInfoDone = false;
	FAccelByteModelsServerInfo ServerInfo;
	FRegistry::ServerDSM.GetServerInfo(THandler<FAccelByteModelsServerInfo>::CreateLambda(
			[&bGetServerInfoDone, &ServerInfo](const FAccelByteModelsServerInfo& result)
			{
				ServerInfo = result;
				bGetServerInfoDone = true;
				UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Server Info Found, Pod Name %s"), *result.Pod_name);
			})
		, FErrorHandler::CreateLambda([&bGetServerInfoDone](int32 ErrorCode, FString ErrorMessage)
			{
				bGetServerInfoDone = true;
				UE_LOG(LogAccelByteJoinableSessionTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			})
		);
	WaitUntil(bGetServerInfoDone, "Waiting Get Server Info");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	ActiveApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	ActiveApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, "Waiting for DSNotif", DSNotifWaitTime);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	WaitUntil(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	WaitUntil(bRegisterSessionSuccess, "Waiting Session Register...");

	DelaySeconds(3, "Waiting backend syncing session");

	bool bAddUserSuccess = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUsers[1].UserId, FVoidHandler::CreateLambda([&bAddUserSuccess]()
	{
		bAddUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bAddUserSuccess, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUsers[1].UserId, FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionSuccess, "Waiting for Query Status...");
	
	for (int i = ActiveApiClients.Num() - 1; i >= 0; i--)
	{
		ActiveApiClients[i]->Lobby.SendLeavePartyRequest();
		ActiveApiClients[i]->Lobby.Disconnect();
	}

	// Assertions

	AB_TEST_FALSE(ServerInfo.Pod_name.IsEmpty());
	AB_TEST_FALSE(AMatchId.IsEmpty());

	bool userIdExist = false;
	for(auto ally : DSGetMatchData.Matching_allies)
	{
		for(auto party : ally.Matching_parties)
		{
			for(auto member : party.Party_members)
			{
				if (member.User_id == ActiveUsers[1].UserId)
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
				if (member.User_id == ActiveUsers[1].UserId)
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
				if (member.User_id == ActiveUsers[1].UserId)
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
	ActiveLobbyConnect(2);

	FString AMatchId = "";

	ActiveApiClients[0]->Lobby.SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	ActiveApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		ActiveApiClients[0]->Lobby.SendReadyConsentRequest(Result.MatchId);
	}));

	bool leavePartyDoneA = false;
	ActiveApiClients[0]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	ActiveApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneA, "wait leaving party A");

	bool leavePartyDoneB = false;
	ActiveApiClients[1]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneB](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneB = true;
	}));
	ActiveApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneB, "wait leaving party B");

	// register local DS
	isDSRegistered = RegisterDS();
	WaitUntil(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	ActiveApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	ActiveApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, "Waiting for DSNotif", DSNotifWaitTime);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	WaitUntil(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	WaitUntil(bRegisterSessionSuccess, "Waiting Session Register...");

	DelaySeconds(3, "Waiting backend to sync session");

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bIsCreatePartySuccess = false;
	ActiveApiClients[1]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[1]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby B creating party");

	bool bAddUserSuccess = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUsers[1].UserId, FVoidHandler::CreateLambda([&bAddUserSuccess]()
	{
		bAddUserSuccess = true;
	}), JoinableSessionTestErrorHandler, BCreatePartyResult.PartyId);
	WaitUntil(bAddUserSuccess, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUsers[1].UserId, FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionSuccess, "Waiting for Query Status...");

	for (int i = ActiveApiClients.Num() - 1; i >= 0; i--)
	{
		ActiveApiClients[i]->Lobby.SendLeavePartyRequest();
		ActiveApiClients[i]->Lobby.Disconnect();
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
				if (member.User_id == ActiveUsers[1].UserId)
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
				if (member.User_id == ActiveUsers[1].UserId)
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
				if (member.User_id == ActiveUsers[1].UserId)
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
	ActiveLobbyConnect(ActiveUserCount);

	FString AMatchId = "";
	FString BMatchId = "";

	ActiveApiClients[0]->Lobby.SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	ActiveApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		ActiveApiClients[0]->Lobby.SendReadyConsentRequest(Result.MatchId);
	}));

	ActiveApiClients[1]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		ActiveApiClients[1]->Lobby.SendReadyConsentRequest(Result.MatchId);
	}));

	ActiveApiClients[1]->Lobby.SetDsNotifDelegate(THandler<FAccelByteModelsDsNotice>::CreateLambda([&BMatchId](const FAccelByteModelsDsNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("B Received DS Update, MatchID %s"), *Result.MatchId);
		BMatchId = Result.MatchId;
	}));

	bool leavePartyDoneA = false;
	ActiveApiClients[0]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneA](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneA = true;
	}));
	ActiveApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneA, "wait leaving party A");

	bool leavePartyDoneB = false;
	ActiveApiClients[1]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDoneB](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDoneB = true;
	}));
	ActiveApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDoneB, "wait leaving party B");

	// register local DS
	isDSRegistered = RegisterDS();
	WaitUntil(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	ActiveApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	ActiveApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, "Waiting for DSNotif", DSNotifWaitTime);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	WaitUntil(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	WaitUntil(bRegisterSessionSuccess, "Waiting Session Register...");

	DelaySeconds(3, "Waiting backend to sync session");

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bIsCreatePartySuccess = false;
	ActiveApiClients[1]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[1]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby B creating party");

	FAccelByteModelsMatchmakingResponse BMatchmakingResult;
	bIsStartMatchmakingSuccess = false;
	ActiveApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&BMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		BMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[1]->Lobby.SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&BMatchId]()
	{
		return !BMatchId.IsEmpty();
	}, "", 20);


	for (int i = ActiveApiClients.Num() - 1; i >= 0; i--)
	{
		ActiveApiClients[i]->Lobby.SendLeavePartyRequest();
		ActiveApiClients[i]->Lobby.Disconnect();
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
	ActiveLobbyConnect(1);

	FString AMatchId = "";

	ActiveApiClients[0]->Lobby.SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&AMatchId](const FAccelByteModelsReadyConsentNotice& Result)
	{
		AMatchId = Result.MatchId;
	}));

	ActiveApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("A Received Matchmaking Completed, MatchID %s"), *Result.MatchId);
		ActiveApiClients[0]->Lobby.SendReadyConsentRequest(Result.MatchId);
	}));

	bool leavePartyDone = false;
	ActiveApiClients[0]->Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&leavePartyDone](const FAccelByteModelsLeavePartyResponse& result)
	{
		leavePartyDone = true;
	}));
	ActiveApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(leavePartyDone, "wait leaving party");

	// register local DS
	isDSRegistered = RegisterDS();
	WaitUntil(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bIsCreatePartySuccess = false;
	ActiveApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bIsCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bIsCreatePartySuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bIsCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bIsStartMatchmakingSuccess = false;
	ActiveApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bIsStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bIsStartMatchmakingSuccess = true;
	}));
	ActiveApiClients[0]->Lobby.SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	WaitUntil(bIsStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	AB_TEST_TRUE(bIsStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, "Waiting for ds notif", DSNotifWaitTime);

	// DS set query session, enqueue joinable session
	bool bGetMatchDataComplete = false;
	bGetMatchDataComplete = GetMatchData();
	WaitUntil(bGetMatchDataComplete, "Waiting get match data");

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData);
	WaitUntil(bRegisterSessionSuccess, "Waiting Session Register...");

	DelaySeconds(3, "Waiting backend to sync session");

	bool bAddUserSuccess = false;
	bool bAddUserDone = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUsers[1].UserId, FVoidHandler::CreateLambda([&bAddUserSuccess, &bAddUserDone]()
	{
		bAddUserSuccess = true;
		bAddUserDone = true;
	}), FErrorHandler::CreateLambda([&bAddUserDone](int32 Code, const FString& Message)
	{
		bAddUserDone = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Add User Error: %d | %s"), Code, *Message);
	}));
	WaitUntil(bAddUserDone, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Match_id, ActiveUsers[0].UserId, FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Match_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	WaitUntil(bQuerySessionSuccess, "Waiting for Query Status...");

	for (int i = ActiveApiClients.Num() - 1; i >= 0; i--)
	{
		ActiveApiClients[i]->Lobby.SendLeavePartyRequest();
		ActiveApiClients[i]->Lobby.Disconnect();
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
				if (member.User_id == ActiveUsers[1].UserId)
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
				if (member.User_id == ActiveUsers[1].UserId)
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
				if (member.User_id == ActiveUsers[0].UserId)
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