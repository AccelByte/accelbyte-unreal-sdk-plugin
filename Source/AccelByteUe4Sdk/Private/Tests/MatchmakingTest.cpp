// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "UserTestAdmin.h"
#include "MatchmakingTestAdmin.h"
#include "Core/AccelByteMultiRegistry.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteMatchmakingTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteMatchmakingTest);

const auto MatchmakingTestErrorHandler = FErrorHandler::CreateLambda(
	[](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	});

BEGIN_DEFINE_SPEC(
	MatchmakingTest,
	"AccelByte.Tests.Lobby.MatchmakingTestNew",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

const int UserNum {2};
TArray<FTestUser> TestUsers;
TArray<FApiClientPtr> ApiClientPtrs;

FString ChannelName;
FString ChannelNameSubGameMode;
const TArray<FString> SubGameModeNames {"1v1", "4pffa"};
FString ChannelNameExtraAttributeTest;
FString ChannelNameNewSessionTest;
FString ChannelNameTempParty2;
FString ChannelNameTest3v3;

bool bDoneSetup {false};
FString ActiveLocalServerName {};

END_DEFINE_SPEC(MatchmakingTest)

#pragma region utilities
enum EResponseStatus
{
	None = 0,
	Success = 1,
	Error = 2
};

bool MatchmakingTestSetupUser(
	const int InUserNum,
	TArray<FTestUser>& OutTestUsers,
	TArray<FApiClientPtr>& OutApiClientPtrs)
{
	// Setup, Register and Login user.
	const bool bSetupUsers = SetupTestUsers(InUserNum, OutTestUsers);

	// Get ref to the client pointers since this is used more often than raw Users
	OutApiClientPtrs.Reset();
	for (const FTestUser& User : OutTestUsers)
	{
		OutApiClientPtrs.Emplace(FMultiRegistry::GetApiClient(User.Email));
	}
			
	// Multi-user login
	FThreadSafeCounter LoginSuccessCounter {0};
	bool bLoginError {false};

	const FVoidHandler OnLoginSuccess = FVoidHandler::CreateLambda(
		[&LoginSuccessCounter]()
	{
		LoginSuccessCounter.Increment();
	});

	const FErrorHandler OnLoginFailed = FErrorHandler::CreateLambda(
		[&bLoginError](int Code, const FString& Message)
	{
		UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("User login error with code %d, message %s"), Code, *Message);
		bLoginError = true;
	});
	
	for (const auto& User : OutTestUsers)
	{
		const FApiClientPtr ClientPtr {FMultiRegistry::GetApiClient(User.Email)};
		ClientPtr->User.LoginWithUsername(User.Email, User.Password, OnLoginSuccess, OnLoginFailed);
	}

	// Wait until either all login success or there is login failure
	WaitUntil([&LoginSuccessCounter, &bLoginError, &OutTestUsers]()
	{
		if(bLoginError || (LoginSuccessCounter.GetValue() == OutTestUsers.Num()))
		{
			return true;
		}

		return false;
	}, "Waiting Login users...");
	
	return !bLoginError;
}

bool MatchmakingTestDeleteUser(TArray<FTestUser> Users)
{
	bool bDeleteError {false};
	for(FTestUser User : Users)
	{
		if(!DeleteTestUser(User))
		{
			UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("Error deleting user %s"), *User.Email);
		}
	}

	return true;
}

bool MatchmakingTestConnectUser(
	int InUserNum,
	const TArray<FApiClientPtr>& InClientPtrs)
{
	if(InUserNum > InClientPtrs.Num())
	{
		UE_LOG(LogAccelByteMatchmakingTest,
			Display,
			TEXT("Error connecting lobby, request connect count (%d) is more than registered user count (%d)"),
			InUserNum,
			InClientPtrs.Num());
		return false;
	}
	
	for(int i = 0; i < InUserNum; i++)
	{
		InClientPtrs[i]->Lobby.Connect();

		WaitUntil([&InClientPtrs, i]()
		{
			return InClientPtrs[i]->Lobby.IsConnected();
		},  FString::Printf(TEXT("Waiting connecting lobby index %d"), i));
	}

	return true;
}

bool MatchmakingTestResetDelegatesAndDisconnectLobby(TArray<FApiClientPtr>& ClientPtrs)
{
	bool bLeavePartyDone {false};
	const auto OnLeavePartyResponse = Api::Lobby::FPartyLeaveResponse::CreateLambda(
		[&](const FAccelByteModelsLeavePartyResponse& Result)
		{
			bLeavePartyDone = true;
		});
	
	for(int i = 0; i < ClientPtrs.Num(); i++)
	{
		if(!ClientPtrs[i]->Lobby.IsConnected())
		{
			continue;
		}
		
		// leave party
		bLeavePartyDone = false;
		ClientPtrs[i]->Lobby.SetLeavePartyResponseDelegate(OnLeavePartyResponse);
		ClientPtrs[i]->Lobby.SendLeavePartyRequest();

		WaitUntil([&](){return bLeavePartyDone;}, "leaving party on beforeEach");

		// unbind events
		UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("UnbindEvent lobby index %d"), i);
		ClientPtrs[i]->Lobby.UnbindEvent();

		// disconnect
		UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("Disconnecting lobby index %d"), i);
		ClientPtrs[i]->Lobby.Disconnect();
		WaitUntil([&]()
		{
			return !ClientPtrs[i]->Lobby.IsConnected();
		}, "Waiting disconnect lobby...");

		UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("lobby index %d disconnected"), i);
	}

	return true;
}

bool MatchmakingTestInviteAndJoinParty(const FTestUser& Inviter, const FTestUser& Invitee)
{
	const FApiClientPtr InviterClientPtr = FMultiRegistry::GetApiClient(Inviter.Email);
	FApiClientPtr InviteeClientPtr = FMultiRegistry::GetApiClient(Invitee.Email);

	EResponseStatus InviteStatus {};
	EResponseStatus JoinStatus {};
	
	const auto OnInviteResponded = Api::Lobby::FPartyInviteResponse::CreateLambda(
		[&InviteStatus, &Inviter, &Invitee](const FAccelByteModelsPartyInviteResponse& Result)
		{
			if(Result.Code == "0")
			{
				InviteStatus = EResponseStatus::Success;
			}
			else
			{
				InviteStatus = EResponseStatus::Error;
			}

			UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("%s invite to %s status %s"), *Inviter.Email,
				*Invitee.Email, (InviteStatus == EResponseStatus::Success? "Success" : "Error"));
		});

	const auto OnInviteReceived = Api::Lobby::FPartyGetInvitedNotif::CreateLambda(
		[&InviteeClientPtr, &Invitee](const FAccelByteModelsPartyGetInvitedNotice& Notif)
		{
			UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("%s received invite notice"), *Invitee.Email);
			
			InviteeClientPtr->Lobby.SendAcceptInvitationRequest(Notif.PartyId, Notif.InvitationToken);
		});

	const auto OnAcceptInvitationResponded = Api::Lobby::FPartyJoinResponse::CreateLambda(
		[&JoinStatus, &Invitee](const FAccelByteModelsPartyJoinResponse& Result)
		{
			if(Result.Code == "0")
			{
				JoinStatus = EResponseStatus::Success;
			}
			else
			{
				JoinStatus = EResponseStatus::Error;
			}

			UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("%s join status %s"), *Invitee.Email,
				(JoinStatus == EResponseStatus::Success? "Success" : "Error"));
		});

	InviteeClientPtr->Lobby.SetPartyGetInvitedNotifDelegate(OnInviteReceived);
	InviteeClientPtr->Lobby.SetInvitePartyJoinResponseDelegate(OnAcceptInvitationResponded);

	InviterClientPtr->Lobby.SetInvitePartyResponseDelegate(OnInviteResponded);
	InviterClientPtr->Lobby.SendInviteToPartyRequest(InviteeClientPtr->CredentialsRef->GetUserId());

	WaitUntil([&InviteStatus, &JoinStatus]()
	{
		// Wait until invite failed (early exit) OR Join received a response
		return InviteStatus == ::EResponseStatus::Error || JoinStatus != EResponseStatus::None;
	}, FString::Printf(TEXT("Wait inviting %s to party"), *Invitee.Email));

	InviteeClientPtr->Lobby.SetPartyGetInvitedNotifDelegate({});
	InviteeClientPtr->Lobby.SetInvitePartyJoinResponseDelegate({});

	InviterClientPtr->Lobby.SetInvitePartyResponseDelegate({});

	return InviteStatus == EResponseStatus::Success && JoinStatus == EResponseStatus::Success;
}

bool MatchmakingTestFormParty(const FTestUser& Leader, const TArray<FTestUser>& Members = {})
{
	// Fetch Client ptrs to interact with each user's lobby
	FApiClientPtr LeaderClientPtr = FMultiRegistry::GetApiClient(Leader.Email);
	TArray<FApiClientPtr> MemberClientPtrs;
	for(FTestUser Member : Members)
	{
		MemberClientPtrs.Emplace(FMultiRegistry::GetApiClient(Member.Email));
	}

	EResponseStatus CreatePartyStatus {EResponseStatus::None};
	const auto OnCreatePartyResponse = Api::Lobby::FPartyCreateResponse::CreateLambda(
		[&CreatePartyStatus](const FAccelByteModelsCreatePartyResponse& Result)
		{
			if(Result.Code == "0")
			{
				CreatePartyStatus = EResponseStatus::Success;
			}
			else
			{
			CreatePartyStatus = EResponseStatus::Error;
			}

			UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Party %s"),
				(CreatePartyStatus == EResponseStatus::Success? "Success" : "Error"));
		});
	
	// register delegates for creating party flow
	// leader => create party response
	LeaderClientPtr->Lobby.SetCreatePartyResponseDelegate(OnCreatePartyResponse);
	LeaderClientPtr->Lobby.SendCreatePartyRequest();

	WaitUntil([&CreatePartyStatus]()
	{
		return CreatePartyStatus != EResponseStatus::None;
	}, "Waiting create party...");

	LeaderClientPtr->Lobby.SetCreatePartyResponseDelegate({});
	
	// early exit if create party failed
	if(CreatePartyStatus == EResponseStatus::Error)
	{
		UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("Create party error"));
		return false;
	}

	for(const auto& Member : Members)
	{
		if(!MatchmakingTestInviteAndJoinParty(Leader, Member))
		{
			// early exit if inviting to party failed
			UE_LOG(LogAccelByteMatchmakingTest, Display, TEXT("Inviting %s to party failed"), *Member.Email);
			return false;
		}
	}

	return true;
}

bool StartLocalDS(const FString& ServerName)
{
	FServerApiClientPtr ServerApiClient = FMultiRegistry::GetServerApiClient();

	bool bLoggedIn {false};

	ServerApiClient->ServerOauth2.LoginWithClientCredentials(
	FVoidHandler::CreateLambda([&bLoggedIn]()
	{
		bLoggedIn = true;
	}),
	MatchmakingTestErrorHandler);

	WaitUntil(bLoggedIn, "Waiting localDS logging in...");

	bool bServerRegistered {false};

	bool canBind = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString LocalIPStr = LocalIp->IsValid() ? LocalIp->ToString(false) : "";
	
	ServerApiClient->ServerDSM.RegisterLocalServerToDSM(
		LocalIPStr,
		7777,
		ServerName,
		FVoidHandler::CreateLambda([&bServerRegistered]()
		{
			bServerRegistered = true;
		}),
		MatchmakingTestErrorHandler);

	WaitUntil(bServerRegistered, "Waiting localDS register to DSMC...");
	return true;
}

bool DeregisterLocalServer(FString& InOutServerName)
{
	if(InOutServerName.IsEmpty())
	{
		return true;
	}

	FServerApiClientPtr ServerApiClient = FMultiRegistry::GetServerApiClient();

	if(ServerApiClient->ServerCredentialsRef->GetClientAccessToken().IsEmpty())
	{
		bool bLoginSuccess {false};
		bool bLoginFailed {false};
		ServerApiClient->ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda(
			[&bLoginSuccess]()
		{
			bLoginSuccess = true;
		}), FErrorHandler::CreateLambda([&bLoginFailed](const int Code, const FString& Message)
		{
			UE_LOG(LogAccelByteMatchmakingTest, Warning,
				TEXT("Error login server with client credentials, code %d message %s"), Code, *Message);
			bLoginFailed = true;
		}));

		WaitUntil([&]()
		{
			return bLoginSuccess || bLoginFailed;
		});

		if(bLoginFailed)
		{
			return false;
		}

		InOutServerName = "";
		return true;
	}

	bool bDeregisterSuccess {false};
	bool bDeregisterError {false};
	ServerApiClient->ServerDSM.DeregisterLocalServerFromDSM(InOutServerName,
		FVoidHandler::CreateLambda([&bDeregisterSuccess]()
		{
			bDeregisterSuccess = true;
		}),
		FErrorHandler::CreateLambda([&bDeregisterError](const int Code, const FString& Message)
		{
			bDeregisterError = true;
		}));

	WaitUntil([&]()
	{
		return bDeregisterError || bDeregisterSuccess;
	});

	return bDeregisterSuccess && !bDeregisterError;
}

#pragma endregion // utilites

void MatchmakingTest::Define()
{
	Describe("With all game modes setup", [this]()
	{
		BeforeEach([this]()
		{
			if(bDoneSetup)
			{
				return true;
			}
			
			// setup users
			MatchmakingTestSetupUser(4, TestUsers, ApiClientPtrs);
			
			// setup matchmaking game modes
			// Create matchmaking channel
			{
				ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

				bool bCreateMatchmakingChannelSuccess = false;
				AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				}), MatchmakingTestErrorHandler);
				
				WaitUntil([&]()
				{
					return bCreateMatchmakingChannelSuccess;
				}, "Waiting for Create Matchmaking channel...", 60);
			}

			// Create matchmaking channel with sub gamemode
			{
				ChannelNameSubGameMode = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

				FAllianceRule AllianceRule;
				AllianceRule.min_number = 3;
				AllianceRule.max_number = 3;
				AllianceRule.player_min_number = 1;
				AllianceRule.player_max_number = 1;

				FAllianceRule AllianceRuleSub;
				AllianceRuleSub.min_number = 2;
				AllianceRuleSub.max_number = 2;
				AllianceRuleSub.player_min_number = 1;
				AllianceRuleSub.player_max_number = 1;

				FAllianceRule AllianceRuleSub4;
				AllianceRuleSub4.min_number = 4;
				AllianceRuleSub4.max_number = 4;
				AllianceRuleSub4.player_min_number = 1;
				AllianceRuleSub4.player_max_number = 1;
			
				TArray<FSubGameMode> SubGameModes;
				SubGameModes.Add({SubGameModeNames[0], AllianceRuleSub});
				SubGameModes.Add({SubGameModeNames[1], AllianceRuleSub4});
			
				bool bCreateMatchmakingChannelSuccess = false;
				AdminCreateMatchmakingChannel(ChannelNameSubGameMode, AllianceRule, {}, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				}), MatchmakingTestErrorHandler, false, SubGameModes);
				
				WaitUntil([&]()
				{
					return bCreateMatchmakingChannelSuccess;
				}, "Waiting for Create Matchmaking channel...", 60);
			}

			// create matchmaking channel for extra attribute matchmaking
			{
				ChannelNameExtraAttributeTest = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
			
				FAllianceRule AllianceRule;
				AllianceRule.min_number = 2;
				AllianceRule.max_number = 2;
				AllianceRule.player_min_number = 1;
				AllianceRule.player_max_number = 1;

				// Create Matchmaking rule with name mmr that has distance of at least 10 apart.
				FMatchingRule MmrRule;
				MmrRule.attribute = "mmr";
				MmrRule.criteria = "distance";
				MmrRule.reference = 10;
				TArray<FMatchingRule> MatchingRules;
				MatchingRules.Add(MmrRule);

				bool bCreateMatchmakingChannelSuccess = false;
				AdminCreateMatchmakingChannel(ChannelNameExtraAttributeTest, AllianceRule, MatchingRules,
					FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				}), MatchmakingTestErrorHandler);

				WaitUntil([&]()
				{
					return bCreateMatchmakingChannelSuccess;
				}, "Create Matchmaking channel...", 60);
			}

			// create matchmaking channel for new session only matchmaking
			{
				ChannelNameNewSessionTest = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
			
				FAllianceRule AllianceRule;
				AllianceRule.min_number = 2;
				AllianceRule.max_number = 3;
				AllianceRule.player_min_number = 1;
				AllianceRule.player_max_number = 1;

				bool bCreateMatchmakingChannelSuccess = false;
				AdminCreateMatchmakingChannel(ChannelNameNewSessionTest, AllianceRule,
					FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				}), MatchmakingTestErrorHandler, true);

				WaitUntil([&]()
				{
					return bCreateMatchmakingChannelSuccess;
				}, "Create Matchmaking channel...", 60);
			}

			// Create matchmaking channel for temp party of 2
			{
				ChannelNameTempParty2 = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
				FAllianceRule AllianceRule;
				AllianceRule.min_number = 2;
				AllianceRule.max_number = 2;
				AllianceRule.player_min_number = 1;
				AllianceRule.player_max_number = 2;

				bool bCreateMatchmakingChannelSuccess = false;
				AdminCreateMatchmakingChannel(ChannelNameTempParty2, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				}), MatchmakingTestErrorHandler);
				WaitUntil([&]()
				{
					return bCreateMatchmakingChannelSuccess;
				}, "Create Matchmaking channel...", 60);
			}

			// Create matchmaking channel for 3v3
			{
				ChannelNameTest3v3 = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
				bool bCreateMatchmakingChannelSuccess = false;
				FAllianceRule AllianceRule;
				AllianceRule.min_number = 3;
				AllianceRule.max_number = 3;
				AllianceRule.player_min_number = 1;
				AllianceRule.player_max_number = 1;

				AdminCreateMatchmakingChannel(ChannelNameTest3v3, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				}), MatchmakingTestErrorHandler);

				WaitUntil([&]()
				{
					return bCreateMatchmakingChannelSuccess;
				}, "Create Matchmaking channel...", 60);
			}

			DelaySeconds(60, "Waiting 60 sec for lobby to refresh cached game mode from matchmaking service...");

			bDoneSetup = true;;
			return true;
		});

		AfterEach([this]()
		{
			// reset lobbies and disconnect connected lobby
			MatchmakingTestResetDelegatesAndDisconnectLobby(ApiClientPtrs);

			DeregisterLocalServer(ActiveLocalServerName);
		});
			
		It("Should do matchmaking flow with only game mode parameter", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			FAccelByteModelsMatchmakingNotice MMNotice;
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MMNotice = Notice;
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};
			FAccelByteModelsDsNotice DsNotice;
			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter, &DsNotice](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
						DsNotice = Notice;
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Start matchmaking process
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));

			AB_TEST_FALSE(MMNotice.MatchId.IsEmpty());
			AB_TEST_EQUAL(MMNotice.GameMode, ChannelName);
			AB_TEST_EQUAL(MMNotice.Joinable, false);
			AB_TEST_TRUE(MMNotice.MatchingAllies.Data.Num() > 0);

			bool bUserFound {false};
			for(auto Ally : MMNotice.MatchingAllies.Data)
			{
				for(auto Party : Ally.Matching_parties)
				{
					for(auto Member : Party.Party_members)
					{
						if(Member.User_id == ApiClientPtrs[0]->CredentialsRef->GetUserId())
						{
							bUserFound = true;
							break;
						}
					}
					if(bUserFound)
						break;
				}
				if(bUserFound)
					break;
			}
			AB_TEST_TRUE(bUserFound);
			
			return true;
		});

		It("Should do matchmaking flow with optional server name only", [this]()
		{
			// Start a localDS
			ActiveLocalServerName = "UEMatchmakingTest" + FGuid::NewGuid().ToString().Right(6);
			if(!StartLocalDS(ActiveLocalServerName))
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Failed to register local DS"));
				return false;
			};
			
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.ServerName = ActiveLocalServerName;
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));
			
			return true;
		});

		It("Should do matchmaking flow with optional client version only", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Start matchmaking process
			const FString ClientVersion = "1.0.0";
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.ClientVersion = ClientVersion;
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));
			
			return true;
		});

		It("Should do matchmaking flow with optional latencies only", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Fetch latencies
			TArray<TPair<FString, float>> Latencies;
			ApiClientPtrs[0]->Qos.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda(
				[&Latencies](const TArray<TPair<FString, float>>& Result)
				{
					Latencies = Result;
				}), MatchmakingTestErrorHandler);
			
			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.Latencies = Latencies;
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));
			
			return true;
		});

		It("Should do matchmaking flow with optional party attributes only", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			FAccelByteModelsMatchmakingNotice MMDDoneNotice;
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MMDDoneNotice = Notice;
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Set Party Attributes
			TMap<FString, FString> PartyAttribute;
			PartyAttribute.Emplace("Test", "Temp");
			
			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.PartyAttributes = PartyAttribute;
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));

			for (auto Ally : MMDDoneNotice.MatchingAllies.Data)
			{
				for (auto Party : Ally.Matching_parties)
				{
					for (auto partyAttribute : PartyAttribute)
					{
						FString Value;
						bool bKeyFound = Party.Party_attributes.JsonObject->TryGetStringField(partyAttribute.Key, Value);
						AB_TEST_TRUE(bKeyFound);
						if (bKeyFound)
						{
							AB_TEST_EQUAL(*Value, partyAttribute.Value);
							UE_LOG(LogAccelByteMatchmakingTest, Log, TEXT("Party Attribute, Key: %s | Value: %s"), *partyAttribute.Key, *Value);
						}
						else
						{
							UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Party Attribute, Key: %s | not found"), *partyAttribute.Key);
						}
					}
				}
			}
			
			return true;
		});

		It("Should do matchmaking flow with optional temp party only", [this]()
		{
			// connect lobby (4)
			const int NumberOfUser = 4;
			const int NumberOfParty = 2;
			MatchmakingTestConnectUser(NumberOfUser, ApiClientPtrs);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			for(int i = 0; i < NumberOfUser; i++)
			{
				ApiClientPtrs[i]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			}
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};

			for(int i = 0; i < NumberOfUser; i++)
			{
				int Index = i;
				ApiClientPtrs[Index]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&, Index](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[Index]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			}
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			for(int i = 0; i < NumberOfUser; i++)
			{
				ApiClientPtrs[i]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			}
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			for(int i = 0; i < NumberOfUser; i++)
			{
				ApiClientPtrs[i]->Lobby.SetDsNotifDelegate(OnDsNotice);
			}

			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams0;
			OptionalParams0.TempPartyUserIds = {ApiClientPtrs[0]->CredentialsRef->GetUserId(), ApiClientPtrs[1]->CredentialsRef->GetUserId()};

			Api::FMatchmakingOptionalParams OptionalParams2;
			OptionalParams2.TempPartyUserIds = {ApiClientPtrs[2]->CredentialsRef->GetUserId(), ApiClientPtrs[3]->CredentialsRef->GetUserId()};
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelNameTempParty2, OptionalParams0);
			ApiClientPtrs[2]->Lobby.SendStartMatchmaking(ChannelNameTempParty2, OptionalParams2);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= NumberOfUser);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == NumberOfParty);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == NumberOfUser);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == NumberOfUser);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= NumberOfUser));
			
			return true;
		});

		It("Should do matchmaking flow with optional extra attributes only", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Set Session Attribute
			FThreadSafeCounter SetSessionAttributeSuccessCount {0};
			bool bSetSessionAttributeError {false};
			const auto OnSetSessionAttributeResponse = Api::Lobby::FSetSessionAttributeResponse::CreateLambda(
				[&bSetSessionAttributeError, &SetSessionAttributeSuccessCount]
				(const FAccelByteModelsSetSessionAttributesResponse& Result)
				{
					if(Result.Code == "0")
					{
						SetSessionAttributeSuccessCount.Increment();
					}
					else
					{
						bSetSessionAttributeError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetSetSessionAttributeDelegate(OnSetSessionAttributeResponse);
			ApiClientPtrs[1]->Lobby.SetSetSessionAttributeDelegate(OnSetSessionAttributeResponse);

			const FString AttributeName {"MMR"};
			
			ApiClientPtrs[0]->Lobby.SetSessionAttribute(AttributeName, "10");
			ApiClientPtrs[1]->Lobby.SetSessionAttribute(AttributeName, "20");

			WaitUntil([&]()
			{
				return bSetSessionAttributeError || SetSessionAttributeSuccessCount.GetValue() == 2;
			}, "Waiting set session attribute");
			
			if(bSetSessionAttributeError)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Failed to set session attribute"));
				return false;
			}
			
			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.ExtraAttributes = {AttributeName};
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelNameExtraAttributeTest, OptionalParams);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelNameExtraAttributeTest, OptionalParams);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));
			
			return true;
		});

		It("Should do matchmaking flow with optional new session only true only", [this]()
		{
			// connect lobby (4)
			MatchmakingTestConnectUser(4, ApiClientPtrs);

			// create party
			for(int i = 0; i < 4; i++)
			{
				MatchmakingTestFormParty(TestUsers[i]);
			}
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			for(int i = 0; i < 4; i++)
			{
				ApiClientPtrs[i]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			}
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			FThreadSafeCounter MatchmakingNotifTimeoutCount {0};
			TArray<FString> MatchIds {};

			for(int i = 0; i < 4; i++)
			{
				int Index = i;
				ApiClientPtrs[Index]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&, Index](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[Index]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
						MatchIds.Emplace(Notice.MatchId);
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifTimeoutCount.Increment();
					}
				}));
			}

			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			for(int i = 0; i < 4; i++)
			{
				ApiClientPtrs[i]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			}
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			for(int i = 0; i < 4; i++)
			{
				ApiClientPtrs[i]->Lobby.SetDsNotifDelegate(OnDsNotice);
			}

			// Prepare local DS
			ActiveLocalServerName = "UEMatchmakingTest" + FGuid::NewGuid().ToString().Right(6);
			if(!StartLocalDS(ActiveLocalServerName))
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Failed to register local DS"));
				return false;
			};
			
			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.NewSessionOnly = true;
			OptionalParams.ServerName = ActiveLocalServerName;
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest, OptionalParams);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest, OptionalParams);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| (MatchmakingNotifTimeoutCount.GetValue() > 0)
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			if(MatchmakingNotifTimeoutCount.GetValue() > 0)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("First matchmaking session got timeout, should be done"));
				return false;
			}

			// Reset mm notif timeout count and ds notif ready count for next mm attempt
			MatchmakingNotifTimeoutCount.Reset();
			DsNotifReadyCounter.Reset();

			DelaySeconds(5, "Waiting for 5 sec");

			// set session as joinable flow
			// fetch session Id (result will have value when ds is busy)
			FString SessionId;
			FMultiRegistry::GetServerApiClient()->ServerDSM.GetSessionId(
				THandler<FAccelByteModelsServerSessionResponse>::CreateLambda([&SessionId]
					(const FAccelByteModelsServerSessionResponse& Result)
				{
					SessionId = Result.Session_id;
				}), MatchmakingTestErrorHandler);

			WaitUntil([&SessionId]()
			{
				return !SessionId.IsEmpty();
			}, "Waiting fetching session Id");

			// Fetch session status
			FAccelByteModelsMatchmakingResult MMStatus;
			bool bQuerySessionFinished {false};
			FMultiRegistry::GetServerApiClient()->ServerMatchmaking.QuerySessionStatus(
				SessionId,
				THandler<FAccelByteModelsMatchmakingResult>::CreateLambda(
					[&MMStatus, &bQuerySessionFinished]
					(const FAccelByteModelsMatchmakingResult& Result)
					{
						MMStatus = Result;
						bQuerySessionFinished = true;
					}),
				MatchmakingTestErrorHandler
			);

			WaitUntil(bQuerySessionFinished, "Waiting query session status");

			// Enqueue joinable session using fetched session status
			bool bSessionEnqueued {false};
			FMultiRegistry::GetServerApiClient()->ServerMatchmaking.EnqueueJoinableSession(
				MMStatus,
				FVoidHandler::CreateLambda([&bSessionEnqueued]()
				{
					bSessionEnqueued = true;
				}),
				MatchmakingTestErrorHandler
			);

			WaitUntil(bSessionEnqueued, "Waiting session enqueued");

			ApiClientPtrs[2]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest, OptionalParams);
			ApiClientPtrs[3]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest, OptionalParams);

			WaitUntil([&]()
			{
				return DsNotifReadyCounter.GetValue() == 2 || MatchmakingNotifTimeoutCount.GetValue() > 0;
			});

			// latest matchmaking attempt result in timeout and did not receive any ds notif
			AB_TEST_TRUE(DsNotifReadyCounter.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifTimeoutCount.GetValue() == 0);
			AB_TEST_TRUE(MatchIds[0] != MatchIds[2]);
			
			return true;
		});

		It("Should do matchmaking flow with optional sub game mode only", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParamsAllGameModeNames;
			OptionalParamsAllGameModeNames.SubGameModes = SubGameModeNames;

			Api::FMatchmakingOptionalParams OptionalParamsOneGameModeName;
			OptionalParamsOneGameModeName.SubGameModes = {SubGameModeNames[0]};
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelNameSubGameMode, OptionalParamsAllGameModeNames);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelNameSubGameMode, OptionalParamsOneGameModeName);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));
			
			return true;
		});

		It("Should do matchmaking flow, 1 person did not send consent, trigger mm timeout and can do matchmaking again", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0]);
			MatchmakingTestFormParty(TestUsers[1]);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool bMatchmakingError {false};
			
			FString Lobby0MatchId {};
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						Lobby0MatchId = Notice.MatchId;
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						bMatchmakingError = true;
					}
				}));

			FString Lobby1MatchId {};
			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						Lobby1MatchId = Notice.MatchId;
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						bMatchmakingError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);

			// Rematchmaking
			FThreadSafeCounter RematchmakingNotifCount {0};
			const auto OnRematchmakingNotif = Api::Lobby::FRematchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsRematchmakingNotice& Notice)
				{
					RematchmakingNotifCount.Increment();
				});

			ApiClientPtrs[0]->Lobby.SetRematchmakingNotifDelegate(OnRematchmakingNotif);
			ApiClientPtrs[1]->Lobby.SetRematchmakingNotifDelegate(OnRematchmakingNotif);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Start matchmaking process
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError || MatchmakingNotifDoneCount.GetValue() == 2;
			});

			if(StartMatchmakingResponseError)
				return false;

			// Lobby 0 send ready consent, lobby 1 doesn't
			ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Lobby0MatchId);

			// wait until ready consent is error OR got mm notif with status timeout
			WaitUntil([&]()
			{
				return ReadyConsentError || RematchmakingNotifCount.GetValue() == 2;
			});

			if(ReadyConsentError)
				return false;

			if(RematchmakingNotifCount.GetValue() < 2)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Error expecting to receive 2 rematchmaking notif, but got %d"), RematchmakingNotifCount.GetValue());
				return false;
			}

			// reset matchmaking notif resets
			MatchmakingNotifDoneCount .Reset();
			RematchmakingNotifCount.Reset();

			// Restart matchmaking process
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelName);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifDoneCount.GetValue() == 2;
			});

			if(StartMatchmakingResponseError || RematchmakingNotifCount.GetValue() > 0)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Start matchmaking after timeout failed, expected to succeed"));
				return false;
			}
			
			ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Lobby0MatchId);
			ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Lobby1MatchId);

			WaitUntil([&]()
			{
				return ReadyConsentError || DsNotifReadyCounter.GetValue() >= 2;
			});

			if(ReadyConsentError)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Start matchmaking after timeout failed, sending ready consent error"));
				return false;
			}
			
			return true;
		});

		It("Should do matchmaking flow with all optional filled", [this]()
		{
			// Start a localDS
			ActiveLocalServerName = "UEMatchmakingTest" + FGuid::NewGuid().ToString().Right(6);
			if(!StartLocalDS(ActiveLocalServerName))
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Failed to register local DS"));
				return false;
			};
			
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			ApiClientPtrs[1]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			bool MatchmakingNotifError {false};
			
			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[0]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));

			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						ApiClientPtrs[1]->Lobby.SendReadyConsentRequest(Notice.MatchId);
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifError = true;
					}
				}));
			
			// readyconsentnotif
			FThreadSafeCounter ReadyConsentSuccessCount {0};
			bool ReadyConsentError {false};

			const auto OnReadyConsentResponse = Api::Lobby::FReadyConsentResponse::CreateLambda(
				[&ReadyConsentSuccessCount, &ReadyConsentError](const FAccelByteModelsReadyConsentRequest& Result)
				{
					if(Result.Code == "0")
					{
						ReadyConsentSuccessCount.Increment();
					}
					else
					{
						ReadyConsentError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			ApiClientPtrs[1]->Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponse);
			
			// dsnotif

			FThreadSafeCounter DsNotifReadyCounter {0};

			const auto OnDsNotice = Api::Lobby::FDsNotif::CreateLambda(
				[&DsNotifReadyCounter](const FAccelByteModelsDsNotice& Notice)
				{
					if(Notice.Status == "READY" || Notice.Status == "BUSY")
					{
						DsNotifReadyCounter.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetDsNotifDelegate(OnDsNotice);
			ApiClientPtrs[1]->Lobby.SetDsNotifDelegate(OnDsNotice);

			// Get Latencies
			TArray<TPair<FString, float>> Latencies;
			bool bLatenciesGet {false};
			ApiClientPtrs[0]->Qos.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda(
				[&Latencies, &bLatenciesGet](const TArray<TPair<FString, float>>& Result)
				{
					bLatenciesGet = true;
					Latencies = Result;
				}),
				MatchmakingTestErrorHandler);

			WaitUntil(bLatenciesGet, "Waiting get latencies");

			// Set extra attributes
			FThreadSafeCounter SetSessionAttributeSuccessCount {0};
			bool bSetSessionAttributeError {false};
			const auto OnSetSessionAttributeResponse = Api::Lobby::FSetSessionAttributeResponse::CreateLambda(
				[&bSetSessionAttributeError, &SetSessionAttributeSuccessCount]
				(const FAccelByteModelsSetSessionAttributesResponse& Result)
				{
					if(Result.Code == "0")
					{
						SetSessionAttributeSuccessCount.Increment();
					}
					else
					{
						bSetSessionAttributeError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetSetSessionAttributeDelegate(OnSetSessionAttributeResponse);
			ApiClientPtrs[1]->Lobby.SetSetSessionAttributeDelegate(OnSetSessionAttributeResponse);

			const FString AttributeName {"MMR"};
			const FString AttributeName2 {"MMR2"}; // intentionally not set to backend
			
			ApiClientPtrs[0]->Lobby.SetSessionAttribute(AttributeName, "10");
			ApiClientPtrs[1]->Lobby.SetSessionAttribute(AttributeName, "20");

			WaitUntil([&]()
			{
				return bSetSessionAttributeError || SetSessionAttributeSuccessCount.GetValue() == 2;
			}, "Waiting set session attribute");
			
			if(bSetSessionAttributeError)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Failed to set session attribute"));
				return false;
			}
			
			const TMap<FString, FString> PartyAttributes {{"Test", "Value"}, {"Test2", "Value2"}}; 

			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParamsAllGameModeNames;
			OptionalParamsAllGameModeNames.SubGameModes = SubGameModeNames;
			OptionalParamsAllGameModeNames.TempPartyUserIds = {ApiClientPtrs[0]->CredentialsRef->GetUserId()};
			OptionalParamsAllGameModeNames.Latencies = Latencies;
			OptionalParamsAllGameModeNames.ClientVersion = "0.0.0";
			OptionalParamsAllGameModeNames.PartyAttributes = PartyAttributes;
			OptionalParamsAllGameModeNames.ExtraAttributes = {AttributeName, AttributeName2};
			OptionalParamsAllGameModeNames.ServerName = ActiveLocalServerName;
			OptionalParamsAllGameModeNames.NewSessionOnly = true;

			Api::FMatchmakingOptionalParams OptionalParamsOneGameModeName;
			OptionalParamsOneGameModeName.SubGameModes = {SubGameModeNames[0]};
			OptionalParamsOneGameModeName.TempPartyUserIds = {ApiClientPtrs[1]->CredentialsRef->GetUserId()};
			OptionalParamsOneGameModeName.Latencies = Latencies;
			OptionalParamsOneGameModeName.ClientVersion = "0.0.0";
			OptionalParamsOneGameModeName.PartyAttributes = PartyAttributes;
			OptionalParamsOneGameModeName.ExtraAttributes = {AttributeName, AttributeName2};
			OptionalParamsOneGameModeName.ServerName = ActiveLocalServerName;
			OptionalParamsOneGameModeName.NewSessionOnly = true;
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelNameSubGameMode, OptionalParamsAllGameModeNames);
			ApiClientPtrs[1]->Lobby.SendStartMatchmaking(ChannelNameSubGameMode, OptionalParamsOneGameModeName);

			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| MatchmakingNotifError
				|| ReadyConsentError
				|| (DsNotifReadyCounter.GetValue() >= 2);
			});

			AB_TEST_FALSE(StartMatchmakingResponseError);
			AB_TEST_FALSE(MatchmakingNotifError);
			AB_TEST_FALSE(ReadyConsentError);
			AB_TEST_TRUE(StartMatchmakingResponseSuccessCount.GetValue() == 2);
			AB_TEST_TRUE(MatchmakingNotifDoneCount.GetValue() == 2);
			AB_TEST_TRUE(ReadyConsentSuccessCount.GetValue() == 2);
			AB_TEST_TRUE((DsNotifReadyCounter.GetValue() >= 2));
			
			return true;
		});

		It("Should do matchmaking flow, then cancel default", [this]()
		{
			// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);

			// create party
			MatchmakingTestFormParty(TestUsers[0], {TestUsers[1]});
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			FThreadSafeCounter MatchmakingNotifTimeoutCount {0};
			FThreadSafeCounter MatchmakingNotifCancelCount {0};

			const auto OnMatchmakingNotif = Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifTimeoutCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Cancel)
					{
						MatchmakingNotifCancelCount.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(OnMatchmakingNotif);
			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(OnMatchmakingNotif);

			// cancel matchmaking response
			bool bMatchmakingCancelSuccess {false};
			bool bMatchmakingCancelError {false};
			const auto OnCancelMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&](const FAccelByteModelsMatchmakingResponse& Result)
				{
					if(Result.Code == "0")
					{
						bMatchmakingCancelSuccess = true;
						return;
					}

					bMatchmakingCancelError = true;
				}
			);
			ApiClientPtrs[0]->Lobby.SetCancelMatchmakingResponseDelegate(OnCancelMatchmakingResponse);
			
			// Start matchmaking process
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName);

			// Wait until receive start matchmaking response
			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| StartMatchmakingResponseSuccessCount.GetValue() > 0;
			});

			if(StartMatchmakingResponseError)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Start matchmaking error"));
				return false;
			}

			// cancel the matchmaking
			ApiClientPtrs[0]->Lobby.SendCancelMatchmaking(ChannelName);

			WaitUntil([&]()
			{
				return bMatchmakingCancelError
				|| MatchmakingNotifTimeoutCount.GetValue() > 0
				|| MatchmakingNotifCancelCount.GetValue() >= 2;
			});

			AB_TEST_FALSE(bMatchmakingCancelError);
			AB_TEST_TRUE(bMatchmakingCancelSuccess);
			AB_TEST_FALSE(MatchmakingNotifTimeoutCount.GetValue() > 0);
			AB_TEST_TRUE(MatchmakingNotifCancelCount.GetValue() >= 2);

			return true;
		});

		It("Should do matchmaking flow, then cancel with temp party true", [this]()
		{
						// connect lobby (2)
			MatchmakingTestConnectUser(2, ApiClientPtrs);
			
			// subscribe lobby delegates to matchmaking flow
			// startmatchmakingresponse
			FThreadSafeCounter StartMatchmakingResponseSuccessCount {0};
			bool StartMatchmakingResponseError {false};
			const auto OnStartMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&StartMatchmakingResponseSuccessCount, &StartMatchmakingResponseError]
				(const FAccelByteModelsMatchmakingResponse& result)
				{
					if(result.Code == "0")
					{
						StartMatchmakingResponseSuccessCount.Increment();
					}
					else
					{
						StartMatchmakingResponseError = true;
					}
				});

			ApiClientPtrs[0]->Lobby.SetStartMatchmakingResponseDelegate(OnStartMatchmakingResponse);
			
			// matchmakingnotif
			FThreadSafeCounter MatchmakingNotifDoneCount {0};
			FThreadSafeCounter MatchmakingNotifTimeoutCount {0};
			FThreadSafeCounter MatchmakingNotifCancelCount {0};

			const auto OnMatchmakingNotif = Api::Lobby::FMatchmakingNotif::CreateLambda(
				[&](const FAccelByteModelsMatchmakingNotice& Notice)
				{
					if(Notice.Status == EAccelByteMatchmakingStatus::Done)
					{
						MatchmakingNotifDoneCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Timeout)
					{
						MatchmakingNotifTimeoutCount.Increment();
					}
					else if(Notice.Status == EAccelByteMatchmakingStatus::Cancel)
					{
						MatchmakingNotifCancelCount.Increment();
					}
				});

			ApiClientPtrs[0]->Lobby.SetMatchmakingNotifDelegate(OnMatchmakingNotif);
			ApiClientPtrs[1]->Lobby.SetMatchmakingNotifDelegate(OnMatchmakingNotif);

			// cancel matchmaking response
			bool bMatchmakingCancelSuccess {false};
			bool bMatchmakingCancelError {false};const auto OnCancelMatchmakingResponse = Api::Lobby::FMatchmakingResponse::CreateLambda(
				[&](const FAccelByteModelsMatchmakingResponse& Result)
				{
					if(Result.Code == "0")
					{
						bMatchmakingCancelSuccess = true;
						return;
					}

					bMatchmakingCancelError = true;
				}
			);
			ApiClientPtrs[0]->Lobby.SetCancelMatchmakingResponseDelegate(OnCancelMatchmakingResponse);
			
			// Start matchmaking process
			Api::FMatchmakingOptionalParams OptionalParams;
			OptionalParams.TempPartyUserIds = {
				ApiClientPtrs[0]->CredentialsRef->GetUserId(),
				ApiClientPtrs[1]->CredentialsRef->GetUserId()
			};
			
			ApiClientPtrs[0]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);

			// Wait until receive start matchmaking response
			WaitUntil([&]()
			{
				return StartMatchmakingResponseError
				|| StartMatchmakingResponseSuccessCount.GetValue() > 0;
			});

			if(StartMatchmakingResponseError)
			{
				UE_LOG(LogAccelByteMatchmakingTest, Error, TEXT("Start matchmaking error"));
				return false;
			}

			// cancel the matchmaking
			ApiClientPtrs[0]->Lobby.SendCancelMatchmaking(ChannelName, true);

			WaitUntil([&]()
			{
				return bMatchmakingCancelError
				|| MatchmakingNotifTimeoutCount.GetValue() > 0
				|| MatchmakingNotifCancelCount.GetValue() >= 2;
			});

			AB_TEST_FALSE(bMatchmakingCancelError);
			AB_TEST_TRUE(bMatchmakingCancelSuccess);
			AB_TEST_FALSE(MatchmakingNotifTimeoutCount.GetValue() > 0);
			AB_TEST_TRUE(MatchmakingNotifCancelCount.GetValue() >= 2);

			return true;
		});

		It("Z_CleanUp", [this]()
		{
			// Clean users
			MatchmakingTestDeleteUser(TestUsers);

			// clean matchmaking game modes
			FThreadSafeCounter DeletedChannelCounter {0};
			const auto OnDeleteMatchmakingChannelSuccess = FSimpleDelegate::CreateLambda(
				[&DeletedChannelCounter]()
				{
					DeletedChannelCounter.Increment();
				});

			const FString MatchmakingChannelNames[]
			{
				ChannelName, ChannelNameTest3v3, ChannelNameTempParty2,
				ChannelNameExtraAttributeTest, ChannelNameNewSessionTest, ChannelNameSubGameMode
			};

			for(FString ChannelToDelete : MatchmakingChannelNames)
			{
						
				const auto OnDeleteMatchmakingChannelFailed = FErrorHandler::CreateLambda(
					[&ChannelToDelete](int Code, const FString& Message)
					{
						UE_LOG(LogAccelByteMatchmakingTest, Warning, TEXT("Error deleting matchmaking channel name %s with code %d, message %s"), *ChannelToDelete, Code, *Message);
					});
						
				AdminDeleteMatchmakingChannel(ChannelToDelete, OnDeleteMatchmakingChannelSuccess, OnDeleteMatchmakingChannelFailed);
			}

			bDoneSetup = false;
			return true;
		});
	});
}