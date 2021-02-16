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
#include "HAL/FileManager.h"

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
	UE_LOG(LogAccelByteJoinableSessionTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
FAccelByteModelsMatchRequest DSGetMatchData;

TSharedPtr<Api::Lobby> CreateLobby(Credentials Creds)
{
	TSharedPtr<Api::Lobby> lobby =  MakeShared<Api::Lobby>(Creds, FRegistry::Settings);
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

	if (isSessionQueued && !DSGetMatchData.Session_id.IsEmpty())
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Dequeueing Joinable Session"));

		bool bDequeueSuccess = false;
		FRegistry::ServerMatchmaking.DequeueJoinableSession(DSGetMatchData.Session_id, FVoidHandler::CreateLambda([&bDequeueSuccess]()
		{
			bDequeueSuccess = true;
		}), JoinableSessionTestErrorHandler);

		FlushHttpRequests();
		Waiting(bDequeueSuccess, "Dequeueing Joinable Session...");

		isSessionQueued = false;
	}

	if (isDSRegistered)
	{
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Cleaning Registered Local DS"));

		bool bDderegisterLocalServerSuccess = false;
		FRegistry::ServerDSM.DeregisterLocalServerFromDSM(LocalDSPodName, FVoidHandler::CreateLambda([&bDderegisterLocalServerSuccess]()
		{
			bDderegisterLocalServerSuccess = true;
		}), JoinableSessionTestErrorHandler);

		FlushHttpRequests();
		Waiting(bDderegisterLocalServerSuccess, "Deregistering Local Server...");

		isDSRegistered = false;
		DSGetMatchData = FAccelByteModelsMatchRequest();
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

	FlushHttpRequests();
	Waiting(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	bool canBind = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString LocalIPStr = LocalIp->IsValid() ? LocalIp->ToString(false) : "";

	FRegistry::ServerDSM.SetOnMatchRequest(THandler<FAccelByteModelsMatchRequest>::CreateLambda([](const FAccelByteModelsMatchRequest& MatchRequest)
	{
		DSGetMatchData = MatchRequest;
	}));

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

	FlushHttpRequests();
	Waiting(bRegisterLocalServerToDSMDone, "Local DS Register To DSM");
	return bRegisterLocalServerToDSMDone;
}

bool RegisterSessionAsync(const FString MatchId)
{
	bool bQuerySessionSuccess = false;
	FAccelByteModelsMatchmakingResult SessionQueryResult;
	FRegistry::ServerMatchmaking.QuerySessionStatus(MatchId, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&SessionQueryResult, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		SessionQueryResult = Result;
		bQuerySessionSuccess = true;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Wait query session");

	check(bQuerySessionSuccess);

	UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Session is %s"), SessionQueryResult.Joinable ? TEXT("JOINABLE") : TEXT("NonJoinable"));

	if (SessionQueryResult.Joinable)
	{
		bool bEnqueueResultSuccess = false;
		FRegistry::ServerMatchmaking.EnqueueJoinableSession(SessionQueryResult, FVoidHandler::CreateLambda([&bEnqueueResultSuccess]()
		{
			bEnqueueResultSuccess = true;
		}), JoinableSessionTestErrorHandler);
		FlushHttpRequests();
		Waiting(bEnqueueResultSuccess, "Wait enqueue joinable session");

		check(bEnqueueResultSuccess);
		isSessionQueued = true;
		return bEnqueueResultSuccess;
	}
	else
	{
		return true;
	}
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

		ActiveUsers.Add(MakeShared<Api::User>(ActiveUserCreds[i], FRegistry::Settings));

		FString Email = FString::Printf(TEXT("lobbyUE4Test+%d@example.com"), i);
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
		
		FlushHttpRequests();
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
		
		FlushHttpRequests();
		Waiting(UsersLoginSuccess[i],"Waiting for Login...");
	}
	
	for (int i = 0; i < ActiveUserCount; i++)
	{
		check(UsersLoginSuccess[i]);
	}

	//Create Matchmaking Channel
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 1;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 2;

	JoinableChannelName = "ue4sdktestjoinable" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	Matchmaking_Create_Matchmaking_Channel(JoinableChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler, true);
	FlushHttpRequests();
	Waiting(bCreateMatchmakingChannelSuccess, "Create Joinable Matchmaking channel...");
	check(bCreateMatchmakingChannelSuccess);

	NonJoinableChannelName = "ue4sdktestnonjoinable" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bCreateMatchmakingChannelSuccess = false;
	Matchmaking_Create_Matchmaking_Channel(NonJoinableChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bCreateMatchmakingChannelSuccess, "Create NonJoinable Matchmaking channel...");
	check(bCreateMatchmakingChannelSuccess);

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
		FlushHttpRequests();
		Waiting(bDeleteUsersSuccessful[i],"Waiting for user deletion...");
	}

	for (int i = 0; i < ActiveUserCount; i++)
	{
		check(bDeleteUsersSuccessful[i]);
	}

	//Delete Matchmaking Channel
	bool bDeleteMatchmakingChannelSuccess = false;
	Matchmaking_Delete_Matchmaking_Channel(JoinableChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	check(bDeleteMatchmakingChannelSuccess);

	bDeleteMatchmakingChannelSuccess = false;
	Matchmaking_Delete_Matchmaking_Channel(NonJoinableChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	check(bDeleteMatchmakingChannelSuccess);

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

	// register local DS & set heartbeat
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 10);

	// DS set query session, enqueue joinable session
	WaitUntil([]()
	{
		FPlatformProcess::Sleep(1.f);
		FRegistry::ServerDSM.PollHeartBeat();
		FlushHttpRequests();
		FTicker::GetCoreTicker().Tick(1.f);
		return !DSGetMatchData.Session_id.IsEmpty();
	}, 10);
	
	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData.Session_id);
	Waiting(bRegisterSessionSuccess, "Waiting Sesstion Register...");

	WaitUntil([](){return false;}, 3);

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bCreatePartySuccess = false;
	lobbyB->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyB->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby B creating party");

	FAccelByteModelsMatchmakingResponse BMatchmakingResult;
	bStartMatchmakingSuccess = false;
	lobbyB->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&BMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		BMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyB->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

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

	check(!AMatchId.IsEmpty());
	check(!BMatchId.IsEmpty());
	check(AMatchId == BMatchId);

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

	// register local DS & set heartbeat
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 10);

	// DS set query session, enqueue joinable session
	WaitUntil([]()
	{
		FPlatformProcess::Sleep(1.f);
		FRegistry::ServerDSM.PollHeartBeat();
		FTicker::GetCoreTicker().Tick(1.f);
		return !DSGetMatchData.Session_id.IsEmpty();
	}, 10);

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData.Session_id);
	Waiting(bRegisterSessionSuccess, "Waiting Sesstion Register...");

	WaitUntil([]()
	{
		return false;
	}, 3);

	bool bAddUserSuccess = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Session_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bAddUserSuccess]()
	{
		bAddUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bAddUserSuccess, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Session_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Session_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Session_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");
	
	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	// Assertions

	check(!AMatchId.IsEmpty());

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
	check(!userIdExist);
	check(bAddUserSuccess);

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
	check(userIdExist);
	check(bRemoveUserSuccess);

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
	check(!userIdExist);
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

	// register local DS & set heartbeat
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 10);

	// DS set query session, enqueue joinable session
	WaitUntil([]()
	{
		FPlatformProcess::Sleep(1.f);
		FRegistry::ServerDSM.PollHeartBeat();
		FlushHttpRequests();
		FTicker::GetCoreTicker().Tick(1.f);
		return !DSGetMatchData.Session_id.IsEmpty();
	}, 10);

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData.Session_id);
	Waiting(bRegisterSessionSuccess, "Waiting Sesstion Register...");

	WaitUntil([]()
	{
		return false;
	}, 3);

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bCreatePartySuccess = false;
	lobbyB->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyB->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby B creating party");

	bool bAddUserSuccess = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Session_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bAddUserSuccess]()
	{
		bAddUserSuccess = true;
	}), JoinableSessionTestErrorHandler, BCreatePartyResult.PartyId);
	FlushHttpRequests();
	Waiting(bAddUserSuccess, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Session_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Session_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Session_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	// Assertions
	check(!AMatchId.IsEmpty());
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
	check(!partyIdExist);
	check(!userIdExist);

	check(bAddUserSuccess);
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
	check(partyIdExist);
	check(userIdExist);

	check(bRemoveUserSuccess);
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
	check(!partyIdExist);
	check(!userIdExist);
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

	// register local DS & set heartbeat
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 10);

	// DS set query session, enqueue joinable session
	WaitUntil([]()
	{
		FPlatformProcess::Sleep(1.f);
		FRegistry::ServerDSM.PollHeartBeat();
		FTicker::GetCoreTicker().Tick(1.f);
		return !DSGetMatchData.Session_id.IsEmpty();
	}, 10);

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData.Session_id);
	Waiting(bRegisterSessionSuccess, "Waiting Sesstion Register...");

	WaitUntil([]()
	{
		return false;
	}, 3);

	// player B complete matchmaking with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse BCreatePartyResult;
	bCreatePartySuccess = false;
	lobbyB->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&BCreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		BCreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyB->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby B creating party");

	FAccelByteModelsMatchmakingResponse BMatchmakingResult;
	bStartMatchmakingSuccess = false;
	lobbyB->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&BMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		BMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyB->SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

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

	check(!AMatchId.IsEmpty());
	check(!BMatchId.IsEmpty());
	check(AMatchId != BMatchId);

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

	// register local DS & set heartbeat
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(NonJoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 10);

	// DS set query session, enqueue joinable session
	WaitUntil([]()
	{
		FPlatformProcess::Sleep(1.f);
		FRegistry::ServerDSM.PollHeartBeat();
		FTicker::GetCoreTicker().Tick(1.f);
		return !DSGetMatchData.Session_id.IsEmpty();
	}, 10);

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(DSGetMatchData.Session_id);
	Waiting(bRegisterSessionSuccess, "Waiting Sesstion Register...");

	WaitUntil([]()
	{
		return false;
	}, 3);

	bool bAddUserSuccess = false;
	bool bAddUserDone = false;
	FRegistry::ServerMatchmaking.AddUserToSession(DSGetMatchData.Game_mode, DSGetMatchData.Session_id, ActiveUserCreds[1].GetUserId(), FVoidHandler::CreateLambda([&bAddUserSuccess, &bAddUserDone]()
	{
		bAddUserSuccess = true;
		bAddUserDone = true;
	}), FErrorHandler::CreateLambda([&bAddUserDone](int32 Code, const FString& Message)
	{
		bAddUserDone = true;
		UE_LOG(LogAccelByteJoinableSessionTest, Log, TEXT("Add User Error: %d | %s"), Code, *Message);
	}));
	FlushHttpRequests();
	Waiting(bAddUserDone, "Waiting Add User...");

	FAccelByteModelsMatchmakingResult mmResultAfterAddUser;
	bool bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Session_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterAddUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterAddUser = Result;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	bool bRemoveUserSuccess = false;
	FRegistry::ServerMatchmaking.RemoveUserFromSession(DSGetMatchData.Game_mode, DSGetMatchData.Session_id, ActiveUserCreds[0].GetUserId(), FVoidHandler::CreateLambda([&bRemoveUserSuccess]()
	{
		bRemoveUserSuccess = true;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bRemoveUserSuccess, "Waiting for Remove User...");


	FAccelByteModelsMatchmakingResult mmResultAfterRemoveUser;
	bQuerySessionSuccess = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(DSGetMatchData.Session_id, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&mmResultAfterRemoveUser, &bQuerySessionSuccess](const FAccelByteModelsMatchmakingResult& Result)
	{
		bQuerySessionSuccess = true;
		mmResultAfterRemoveUser = Result;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bQuerySessionSuccess, "Waiting for Query Status...");

	for (int i = ActiveLobbies.Num() - 1; i >= 0; i--)
	{
		ActiveLobbies[i]->SendLeavePartyRequest();
		ActiveLobbies[i]->Disconnect();
		ActiveLobbies.RemoveAt(i);
	}

	// Assertions

	check(!AMatchId.IsEmpty());

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
	check(!userIdExist);
	check(!bAddUserSuccess);
	check(bAddUserDone);

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
	check(!userIdExist);
	check(bRemoveUserSuccess);

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
	check(!userIdExist);
	return TestCleanUp();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(JoinableSessionTestActivateSessionPolling, "AccelByte.Tests.Joinable.B.Activate_Session_Polling", AutomationFlagMaskJoinable);
bool JoinableSessionTestActivateSessionPolling::RunTest(const FString& Parameters)
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

	// register local DS & set heartbeat
	isDSRegistered = RegisterDS();
	Waiting(isDSRegistered, "Waiting DS Register...");

	// player A complete matchmaking flow with joinable gamemode channel
	FAccelByteModelsCreatePartyResponse ACreatePartyResult;
	bool bCreatePartySuccess = false;
	lobbyA->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&ACreatePartyResult, &bCreatePartySuccess](const FAccelByteModelsCreatePartyResponse& Result)
	{
		ACreatePartyResult = Result;
		bCreatePartySuccess = true;
	}));
	lobbyA->SendCreatePartyRequest();
	Waiting(bCreatePartySuccess, "Lobby A creating party");

	FAccelByteModelsMatchmakingResponse AMatchmakingResult;
	bool bStartMatchmakingSuccess = false;
	lobbyA->SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&AMatchmakingResult, &bStartMatchmakingSuccess](const FAccelByteModelsMatchmakingResponse& Result)
	{
		AMatchmakingResult = Result;
		bStartMatchmakingSuccess = true;
	}));
	lobbyA->SendStartMatchmaking(JoinableChannelName, LocalDSPodName);
	Waiting(bStartMatchmakingSuccess, "Waiting for StartMatchmaking...");
	check(bStartMatchmakingSuccess);

	WaitUntil([&AMatchId]()
	{
		return !AMatchId.IsEmpty();
	}, 10);

	// DS set query session, enqueue joinable session
	WaitUntil([]()
	{
		FPlatformProcess::Sleep(1.f);
		FRegistry::ServerDSM.PollHeartBeat();
		FTicker::GetCoreTicker().Tick(1.f);
		FlushHttpRequests();
		return !DSGetMatchData.Session_id.IsEmpty();
	}, 10);

	FString heartbeatSessionId = DSGetMatchData.Session_id;

	int QueryCount = 0;
	const int Interval = 5;
	FAccelByteModelsMatchmakingResult MatchStatusResponse;

	auto onSessionStatusUpdated = THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&QueryCount, &MatchStatusResponse](const FAccelByteModelsMatchmakingResult& Result)
	{
		QueryCount++;
		MatchStatusResponse = Result;
		FlushHttpRequests();
	});

	FRegistry::ServerMatchmaking.ActivateSessionStatusPolling(heartbeatSessionId, onSessionStatusUpdated, JoinableSessionTestErrorHandler);

	WaitUntil([]()
	{
		return false;
	}, Interval*3);

	bool bRegisterSessionSuccess = false;
	bRegisterSessionSuccess = RegisterSessionAsync(heartbeatSessionId);
	Waiting(bRegisterSessionSuccess, "Waiting Session Register...");

	WaitUntil([]()
	{
		return false;
	}, 3);

	FRegistry::ServerMatchmaking.DeactivateStatusPolling();
	check(QueryCount >= 3);
	check(MatchStatusResponse.Status == EAccelByteMatchmakingSessionStatus::SessionInQueue);

	WaitUntil([]()
	{
		return false;
	}, 5);

	FRegistry::ServerMatchmaking.ActivateSessionStatusPolling(heartbeatSessionId, onSessionStatusUpdated, JoinableSessionTestErrorHandler);

	WaitUntil([]()
	{
		return false;
	}, Interval * 3);

	bool bDequeueSuccess = false;
	FRegistry::ServerMatchmaking.DequeueJoinableSession(heartbeatSessionId, FVoidHandler::CreateLambda([&bDequeueSuccess]()
	{
		bDequeueSuccess = true;
	}), JoinableSessionTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDequeueSuccess, "Dequeue Session...");

	WaitUntil([]()
	{
		return false;
	}, 3);

	FRegistry::ServerMatchmaking.DeactivateStatusPolling();
	check(QueryCount >= 6);
	check(MatchStatusResponse.Status == EAccelByteMatchmakingSessionStatus::Done);

	return TestCleanUp();
}