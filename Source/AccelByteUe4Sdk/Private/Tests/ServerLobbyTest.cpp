// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
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


DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerLobbyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerLobbyTest);

const int32 AutomationFlagMaskServerLobby = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const int STestUserCount = 4;
FString SUserIds[STestUserCount];
Credentials SUserCreds[STestUserCount];
TArray<TSharedPtr<Api::User>> SLobbyUsers;
TArray<TSharedPtr<Api::Lobby>> SLobbies;
FJsonObjectWrapper SPartyStorageData;

bool bSUsersConnected, bSUsersConnectionSuccess, bSUsersConnectionError;
bool bSCreatePartySuccess, bSCreatePartyError, bSInvitePartySuccess, bSGetInvitedNotifSuccess, bSGetInvitedNotifError, bSGetInfoPartySuccess, bSGetInfoPartyError;
bool bSJoinPartySuccess, bSJoinPartyError, bSLeavePartySuccess, bSLeavePartyError, bSKickPartyMemberSuccess, bSKickPartyMemberError, bSKickedFromPartySuccess;
bool bSetSessionAttribute;
bool bSCleanupLeaveParty;

FAccelByteModelsInfoPartyResponse SinfoPartyResponse;
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
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("%s"), *text);
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
		while (SLobbies[i]->IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("disconecting lobby %d"), i);
			FTicker::GetCoreTicker().Tick(.5f);
		}
	}
}

void ResetServerResponses()
{
	bSUsersConnected = false;
	bSUsersConnectionSuccess = false;
	bSUsersConnectionError = false;
	bSCreatePartySuccess = false;
	bSCreatePartyError = false;
	bSInvitePartySuccess = false;
	bSGetInvitedNotifError = false;
	bSGetInvitedNotifSuccess = false;
	bSGetInfoPartySuccess = false;
	bSGetInfoPartyError = false;
	bSJoinPartySuccess = false;
	bSJoinPartyError = false;
	bSLeavePartySuccess = false;
	bSLeavePartyError = false;
	bSetSessionAttribute = false;
	bSKickPartyMemberSuccess = false;
	bSKickPartyMemberError = false; 
	bSKickedFromPartySuccess = false;
	bSCleanupLeaveParty = false;
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

const auto SGetInfoPartyDelegate = Api::Lobby::FPartyInfoResponse::CreateLambda([](FAccelByteModelsInfoPartyResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Info Party Success!"));
	bSGetInfoPartySuccess = true;
	if (!result.PartyId.IsEmpty())
	{
		SinfoPartyResponse = result;
	}
	else
	{
		bSGetInfoPartyError = true;
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

const auto ServerKickPartyMemberDelegate = Api::Lobby::FPartyKickResponse::CreateLambda([](FAccelByteModelsKickPartyMemberResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Kick Party Member Success!"));
	bSKickPartyMemberSuccess = true;
	if (result.Code != "0")
	{
		bSKickPartyMemberError = true;
	}
});


const auto ServerKickedFromPartyDelegate = Api::Lobby::FPartyKickNotif::CreateLambda([](FAccelByteModelsGotKickedFromPartyNotice result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Kicked From Party!"));
	{
		bSKickedFromPartySuccess = true;
	}
});

const auto ServerSetSessionAttributeDelagate = Api::Lobby::FSetSessionAttributeResponse::CreateLambda([](FAccelByteModelsSetSessionAttributesResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Set Session Attribute Success!"));
	{
		bSetSessionAttribute = true;
	}
});

const auto SCleanupLeavePartyDelegate = Api::Lobby::FPartyLeaveResponse::CreateLambda([](FAccelByteModelsLeavePartyResponse result)
{
	UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Cleanup Leave Party Success!"));
	bSCleanupLeaveParty = true;
});

void CleanPartyBeforeTest(const FSimpleDelegate& OnSuccess)
{
	int32 SuccessLeaveCount = 0;
	for (int i = 0; i < SLobbies.Num(); i++)
	{
		bSLeavePartySuccess = false;
		SLobbies[i]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSLeavePartySuccess, "Leaving Party...");
		SuccessLeaveCount++;
	}

	if (SuccessLeaveCount == SLobbies.Num())
	{
		OnSuccess.Execute();
	}
}

void FormParty(const FSimpleDelegate OnSuccess)
{
	FAccelByteModelsPartyGetInvitedNotice invitedToParty[3];

	SLobbyConnect(4);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");


	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetInvitePartyResponseDelegate(SInvitePartyDelegate);
	SLobbies[0]->SetInfoPartyResponseDelegate(SGetInfoPartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating Party...");
	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	for (int i = 1; i < STestUserCount; i++)
	{
		SLobbies[i]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
		SLobbies[i]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
		SLobbies[i]->SetInfoPartyResponseDelegate(SGetInfoPartyDelegate);

		SLobbies[0]->SendInviteToPartyRequest(SUserCreds[i].GetUserId());
		Waiting(bSInvitePartySuccess, "Inviting to Party...");
		bSInvitePartySuccess = false;
		WaitUntil([&]()
		{
			return bSGetInvitedNotifSuccess;
		}, 30, "Waiting for Party Invitation");
		bSGetInvitedNotifSuccess = false;
		invitedToParty[i - 1] = SinvitedToPartyResponse;
	}

	for (int i = 1; i < STestUserCount; i++)
	{
		int32 index = i - 1;
		SLobbies[i]->SendAcceptInvitationRequest(invitedToParty[index].PartyId, invitedToParty[index].InvitationToken);
		Waiting(bSJoinPartySuccess, "Joining a Party...");
		bSJoinPartySuccess = false;
	}

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	if (bSGetInfoPartySuccess && !SinfoPartyResponse.PartyId.IsEmpty() && (SinfoPartyResponse.Members.Num() == STestUserCount))
	{
		bool _ = OnSuccess.ExecuteIfBound();
	}

}

bool ComparePartyData(const FJsonObjectWrapper& PartyData)
{
	TSharedPtr<FJsonObject> ExpectedData;
	ExpectedData = SPartyStorageData.JsonObject;
	TSharedPtr<FJsonObject> ComparedData;
	ComparedData = PartyData.JsonObject;

	if (!ComparedData->HasField("difficulty")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("difficulty field not found")); return false; }
	if (!ComparedData->HasField("dungeon")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("dungeon field not found")); return false; }
	if (!ComparedData->HasField("server")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("server field not found")); return false; }
	if (!ComparedData->HasField("port")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("port field not found")); return false; }
	if (!ComparedData->HasField("multiplier")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("multiplier field not found")); return false; }
	if (!ComparedData->HasField("bosses")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("bosses field not found")); return false; }
	if (!ComparedData->HasField("symbol1")) { UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("symbol1 field not found")); return false; }

	if (!ComparedData->GetStringField("difficulty").Equals(ExpectedData->GetStringField("difficulty"), ESearchCase::CaseSensitive)) 
	{ 
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("difficulty field content not match, got %s expected %s"), *ComparedData->GetStringField("difficulty"), *ExpectedData->GetStringField("difficulty"));  
		return false; 
	}
	if (!ComparedData->GetStringField("dungeon").Equals(ExpectedData->GetStringField("dungeon"), ESearchCase::CaseSensitive))
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("dungeon field content not match, got %s expected %s"), *ComparedData->GetStringField("dungeon"), *ExpectedData->GetStringField("dungeon"));
		return false;
	}
	if (!ComparedData->GetStringField("server").Equals(ExpectedData->GetStringField("server"), ESearchCase::CaseSensitive))
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("server field content not match, got %s expected %s"), *ComparedData->GetStringField("server"), *ExpectedData->GetStringField("server"));
		return false;
	}
	if (!(ComparedData->GetNumberField("port") == ExpectedData->GetNumberField("port")))
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("port field content not match, got %f expected %f"), ComparedData->GetNumberField("port"), ExpectedData->GetNumberField("port"));
		return false;
	}
	if (!(ComparedData->GetNumberField("multiplier") == ExpectedData->GetNumberField("multiplier")))
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("multiplier field content not match, got %f expected %f"), ComparedData->GetNumberField("multiplier"), ExpectedData->GetNumberField("multiplier"));
		return false;
	}
	if (!ComparedData->GetStringField("symbol1").Equals(ExpectedData->GetStringField("symbol1"), ESearchCase::CaseSensitive))
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("difficulty field content not match, got %s expected %s"), *ComparedData->GetStringField("symbol1"), *ExpectedData->GetStringField("symbol1"));
		return false;
	}

	if (!(ComparedData->GetArrayField("bosses").Num() == ExpectedData->GetArrayField("bosses").Num()))
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("bosses field content not match, num got %d expected %d"), ComparedData->GetArrayField("bosses").Num(), ExpectedData->GetArrayField("bosses").Num());
		return false;
	}

	TArray<TSharedPtr<FJsonValue>> ExpectedBosses = ExpectedData->GetArrayField("bosses");
	TArray<TSharedPtr<FJsonValue>> ComparedBosses = ComparedData->GetArrayField("bosses");
	for (int i = 0; i < ExpectedBosses.Num(); i++)
	{
		if (!ExpectedBosses[i]->AsString().Equals(ComparedBosses[i]->AsString(), ESearchCase::CaseSensitive))
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("boss array content not match, got %s expected %s"), *ComparedBosses[i]->AsString(), *ExpectedBosses[i]->AsString());
			return false;
		}
	}

	return true;
}

void OverwritePartyStorage(const FString& PartyId, FJsonObjectWrapper PartyData, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError)
{
	FRegistry::ServerLobby.WritePartyStorage(PartyId, [PartyData](FJsonObjectWrapper) { return PartyData; }, OnSuccess, OnError, 3);
}

void GameClientOverwritePartyStorage(Api::Lobby* lobby, const FString& PartyId, FJsonObjectWrapper PartyData, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError)
{
	lobby->WritePartyStorage(PartyId, [PartyData](FJsonObjectWrapper) { return PartyData; }, OnSuccess, OnError, 3);
}

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

		SLobbyUsers.Add(MakeShared<Api::User>(SUserCreds[i], FRegistry::Settings, FRegistry::HttpRetryScheduler));

		FString Email = FString::Printf(TEXT("serverlobbyUE4Test+%d-%d@example.com"), i, FMath::RandRange(0, 100000000));
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

		Waiting(UsersLoginSuccess[i], "Waiting for Login...");

		SLobbies.Add(MakeShared<Api::Lobby>(SUserCreds[i], FRegistry::Settings, FRegistry::HttpRetryScheduler));
	}

	for (int i = 0; i < STestUserCount; i++)
	{
		AB_TEST_TRUE(UsersLoginSuccess[i]);
	}

	bool bLoginServerSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginServerSuccess]()
	{
		bLoginServerSuccess = true;
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("\t\tClient Successfully Login."));
	}), ServerLobbyErrorHandler);
	Waiting(bLoginServerSuccess, "Waiting for Client Login...");

	SLobbyConnect(4);

	for (int i = 0; i < SLobbies.Num(); i++)
	{
		bSCleanupLeaveParty = false;
		SLobbies[i]->SetLeavePartyResponseDelegate(SCleanupLeavePartyDelegate);
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSCleanupLeaveParty, "Leaving Party...");
	}

	SLobbyDisconnect(4);
	ResetServerResponses();

	// setup custom party storage data
	SPartyStorageData.JsonObject = MakeShared<FJsonObject>();
	SPartyStorageData.JsonObject->SetStringField("difficulty", "easy");
	SPartyStorageData.JsonObject->SetStringField("dungeon", "deep forest");
	SPartyStorageData.JsonObject->SetStringField("server", "192.168.1.1");
	SPartyStorageData.JsonObject->SetNumberField("port", 12345);
	SPartyStorageData.JsonObject->SetNumberField("multiplier", 1.25);
	TArray<TSharedPtr<FJsonValue>> bosses{
			MakeShareable(new FJsonValueString("wrath")),
			MakeShareable(new FJsonValueString("glutonny")),
			MakeShareable(new FJsonValueString("sloth"))
	};
	SPartyStorageData.JsonObject->SetArrayField("bosses", bosses);
	SPartyStorageData.JsonObject->SetStringField("symbol1", "[{(?\r\t!+-@ [] {} ). ; })]////\\\\***...\"\n\n");

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
		bDeleteUsersSuccessful[i] = false;
		DeleteUserById(SUserCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Success"));
			bDeleteUsersSuccessful[i] = true;
		}), ServerLobbyErrorHandler);
		Waiting(bDeleteUsersSuccessful[i], "Waiting for user deletion...");
	}

	for (int i = 0; i < STestUserCount; i++)
	{
		AB_TEST_TRUE(bDeleteUsersSuccessful[i]);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_WritePartyStorage_ValidParty, "AccelByte.Tests.ServerLobby.B.WritePartyStorageValidParty_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_WritePartyStorage_ValidParty::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	AB_TEST_TRUE(bWritePartyStorageSuccess);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_WriteEmptyPartyStorage_ValidParty, "AccelByte.Tests.ServerLobby.B.WriteEmptyPartyStorageValidParty_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_WriteEmptyPartyStorage_ValidParty::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	FJsonObjectWrapper EmptyStorage;
	EmptyStorage.JsonObject = MakeShared<FJsonObject>();
	OverwritePartyStorage(PartyId, EmptyStorage, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	AB_TEST_TRUE(bWritePartyStorageSuccess);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_WritePartyStorage_NonExistParty, "AccelByte.Tests.ServerLobby.B.WritePartyStorageInValidParty_Fail", AutomationFlagMaskServerLobby);
bool PartyStorageTest_WritePartyStorage_NonExistParty::RunTest(const FString& Parameters)
{
	FString PartyId = "InvalidParty01234ID";
	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	bool bWritePartyStorageDone = false;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageDone, &bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
		bWritePartyStorageDone = true;;
	}), FErrorHandler::CreateLambda([&bWritePartyStorageDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Can't write party storage. Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bWritePartyStorageDone = true;
	}));
	Waiting(bWritePartyStorageDone, "Waiting for write result");

	AB_TEST_FALSE(bWritePartyStorageSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_GetPartyStorage_NonExistParty, "AccelByte.Tests.ServerLobby.B.GetPartyStorage_NonExistParty_Fail", AutomationFlagMaskServerLobby);
bool PartyStorageTest_GetPartyStorage_NonExistParty::RunTest(const FString& Parameters)
{
	bool bGetPartyStorageSuccess = false;
	bool bGetPartyStorageDone = false;
	FString DummyPartyId = "thisPartyNeverExisted";

	FAccelByteModelsPartyDataNotif PartyData;
	FRegistry::ServerLobby.GetPartyStorage(DummyPartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageDone,&bGetPartyStorageSuccess](FAccelByteModelsPartyDataNotif result)
	{
		bGetPartyStorageDone = true;
		bGetPartyStorageSuccess = true;
	}),FErrorHandler::CreateLambda([&bGetPartyStorageDone](int32 ErrorCode, FString ErrorMessage)
	{
		bGetPartyStorageDone = true;
	}));
	Waiting(bGetPartyStorageDone, "Waiting for get result");

	AB_TEST_FALSE(bGetPartyStorageSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_GetPartyStorage_ValidPartyButEmptyStorage_Success, "AccelByte.Tests.ServerLobby.B.GetPartyStorage_ValidPartyButEmptyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_GetPartyStorage_ValidPartyButEmptyStorage_Success::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	bool bGetPartyStorageSuccess = false;
	FAccelByteModelsPartyDataNotif PartyData;

	FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &PartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			PartyData = result;
		}), ServerLobbyErrorHandler);
	Waiting(bGetPartyStorageSuccess, "Waiting for get result");
	
	AB_TEST_TRUE(bGetPartyStorageSuccess);
	AB_TEST_EQUAL(PartyData.Custom_attribute.JsonObject->Values.Num(), 0);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_GetPartyStorage_ValidPartyStorage_Success, "AccelByte.Tests.ServerLobby.B.GetPartyStorage_ValidPartyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_GetPartyStorage_ValidPartyStorage_Success::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	AB_TEST_TRUE(bWritePartyStorageSuccess);

	bool bGetPartyStorageSuccess = false;
	FAccelByteModelsPartyDataNotif PartyData;

	FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &PartyData](FAccelByteModelsPartyDataNotif result)
	{
		bGetPartyStorageSuccess = true;
		PartyData = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetPartyStorageSuccess, "Waiting for get result");

	AB_TEST_TRUE(bGetPartyStorageSuccess);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_OverwritePartyStorage_PartyFormed_SuccessGotNotification, "AccelByte.Tests.ServerLobby.B.OverwritePartyStorage_PartyFormed_SuccessGotNotification", AutomationFlagMaskServerLobby);
bool PartyStorageTest_OverwritePartyStorage_PartyFormed_SuccessGotNotification::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	TQueue<FAccelByteModelsPartyDataNotif> PartyDataNotifResults;
	int UpdateCount = 0;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &UpdateCount](FAccelByteModelsPartyDataNotif result)
	{
		UpdateCount++;
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
		PartyDataNotifResults.Enqueue(result);
	});
	for (int i = 0; i < SLobbies.Num(); i++)
	{
		SLobbies[i]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);
	}

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	WaitUntil([&UpdateCount]() { return UpdateCount  == 4; }, 5);

	AB_TEST_TRUE(bSGetInfoPartySuccess);
	AB_TEST_TRUE(bWritePartyStorageSuccess);

	while (!PartyDataNotifResults.IsEmpty())
	{
		FAccelByteModelsPartyDataNotif PartyData;
		PartyDataNotifResults.Dequeue(PartyData);
		AB_TEST_TRUE(ComparePartyData(PartyData.Custom_attribute));
	}

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_OverwritePartyStorage_PartyFormed_SuccessGotNotification_Throttle, "AccelByte.Tests.ServerLobby.B.OverwritePartyStorage_PartyFormed_SuccessGotNotification_Throttle", AutomationFlagMaskServerLobby);
bool PartyStorageTest_OverwritePartyStorage_PartyFormed_SuccessGotNotification_Throttle::RunTest(const FString& Parameters)
{
	SLobbyConnect(1);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Waiting for create party");

	TArray<FAccelByteModelsPartyDataNotif> PartyDataNotifResults;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults](FAccelByteModelsPartyDataNotif result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
		PartyDataNotifResults.Add(result);
	});
	SLobbies[0]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);

	int bSpamUpdateCount = 10;
	int bServerUpdateSuccessCount = 0;
	int bServerUpdateDone = 0;
	for (int i = 0; i < bSpamUpdateCount; i++)
	{
		FJsonObjectWrapper PartyAttribute;
		PartyAttribute.JsonObject = MakeShared<FJsonObject>();
		PartyAttribute.JsonObject->SetNumberField("key", i);
		FAccelByteModelsPartyDataNotif WriteNotif;
		bool bWritePartyStorageSuccess = false;
		OverwritePartyStorage(ScreatePartyResponse.PartyId, PartyAttribute, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif, &bServerUpdateSuccessCount, &bServerUpdateDone](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			WriteNotif = notifResult;
			bWritePartyStorageSuccess = true;
			bServerUpdateSuccessCount++;
			bServerUpdateDone++;
		}), FErrorHandler::CreateLambda(([&bServerUpdateDone](int32 ErrorCode, FString ErrorMessage)
		{
			bServerUpdateDone++;
		})));
		Waiting(bWritePartyStorageSuccess, "Waiting for write result");
	}

	WaitUntil([&bServerUpdateDone, &bSpamUpdateCount]() { return bServerUpdateDone == bSpamUpdateCount; }, 5);

	bSLeavePartySuccess = false;
	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting for leave party");

	bool bIsPartyDataNotifIsEqual = PartyDataNotifResults.Num() == bServerUpdateSuccessCount;
	AB_TEST_TRUE(bIsPartyDataNotifIsEqual);

	for (auto& PartyDataNotif : PartyDataNotifResults)
	{
		AB_TEST_TRUE(PartyDataNotif.Custom_attribute.JsonObject->TryGetField("key") != nullptr);
	}

	SLobbyDisconnect(1);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_AllPartyState_ShouldGetPartyStorageNotif, "AccelByte.Tests.ServerLobby.B.AllPartyState_ShouldGetPartyStorageNotif", AutomationFlagMaskServerLobby);
bool PartyStorageTest_AllPartyState_ShouldGetPartyStorageNotif::RunTest(const FString& Parameters)
{
	int PartySize = 2;
	SLobbyConnect(PartySize);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	SLobbies[1]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
	SLobbies[1]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
	SLobbies[1]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	TArray<FAccelByteModelsPartyDataNotif> PartyDataNotifResults;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults](FAccelByteModelsPartyDataNotif result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
		PartyDataNotifResults.Add(result);
	});
	SLobbies[0]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	AB_TEST_FALSE(bSCreatePartyError);

	WaitUntil([&PartyDataNotifResults]() { return PartyDataNotifResults.Num() > 0; }, 1.5);
	FAccelByteModelsPartyDataNotif PartyDataNotif;
	PartyDataNotif = PartyDataNotifResults[0];
	AB_TEST_EQUAL(PartyDataNotif.Members.Num(), 1);
	// clear array
	PartyDataNotifResults.RemoveAt(0);

	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	Waiting(bSGetInvitedNotifSuccess, "Waiting party invitation");

	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");
	AB_TEST_FALSE(bSJoinPartyError);

	WaitUntil([&PartyDataNotifResults, &PartySize]() { return PartyDataNotifResults.Num() == PartySize; }, 1.5);
	PartyDataNotif = PartyDataNotifResults[1];
	AB_TEST_TRUE(PartyDataNotif.Members.Num() > 1);
	// clear array
	for (int i = (PartyDataNotifResults.Num()-1); i >= 0; i--)
	{
		PartyDataNotifResults.RemoveAt(i);
	}

	bSLeavePartyError = false;
	bSLeavePartySuccess = false;
	SLobbies[1]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting leave party");
	AB_TEST_FALSE(bSLeavePartyError);

	WaitUntil([&PartyDataNotifResults]() { return PartyDataNotifResults.Num() > 0; }, 5.0);
	PartyDataNotif = PartyDataNotifResults[0];
	AB_TEST_EQUAL(PartyDataNotif.Members.Num(), 1);

	bSLeavePartyError = false;
	bSLeavePartySuccess = false;
	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting leave party");
	AB_TEST_FALSE(bSLeavePartyError);

	SLobbyDisconnect(PartySize);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_SomeoneCreateParty_GameClientAndServerGetPartyStorage_Success, "AccelByte.Tests.ServerLobby.B.SomeoneCreateParty_GameClientAndServerGetPartyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_SomeoneCreateParty_GameClientAndServerGetPartyStorage_Success::RunTest(const FString& Parameters)
{
	SLobbyConnect(1);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	FAccelByteModelsPartyDataNotif PartyDataNotifResults;
	bool bSuccessGetPartyDataNotif = false;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &bSuccessGetPartyDataNotif](FAccelByteModelsPartyDataNotif result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
		PartyDataNotifResults = result;
		bSuccessGetPartyDataNotif = true;
	});
	SLobbies[0]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Waiting create party");
	AB_TEST_TRUE(bSCreatePartySuccess);

	FString PartyId = ScreatePartyResponse.PartyId;

	bool bGetPartyStorageSuccess = false;
	FAccelByteModelsPartyDataNotif ClientPartyData;
	SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ClientPartyData](FAccelByteModelsPartyDataNotif result)
	{
		bGetPartyStorageSuccess = true;
		ClientPartyData = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetPartyStorageSuccess, "Waiting for get result");
	AB_TEST_TRUE(ClientPartyData.Members.Contains(SUserIds[0]));
	AB_TEST_TRUE(bGetPartyStorageSuccess);

	bGetPartyStorageSuccess = false;
	FAccelByteModelsPartyDataNotif ServerPartyData;
	FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ServerPartyData](FAccelByteModelsPartyDataNotif result)
	{
		bGetPartyStorageSuccess = true;
		ServerPartyData = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetPartyStorageSuccess, "Waiting for get result");
	AB_TEST_TRUE(ClientPartyData.Members.Contains(SUserIds[0]));
	AB_TEST_TRUE(bGetPartyStorageSuccess);

	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting for leave party");
	
	AB_TEST_TRUE(bSuccessGetPartyDataNotif);
	AB_TEST_TRUE(PartyDataNotifResults.Members.Contains(SUserIds[0]));
	AB_TEST_EQUAL(PartyDataNotifResults.Invitees.Num(), 0);

	SLobbyDisconnect(1);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_SomeoneJoinParty_GameClientAndServerGetPartyStorage_Success, "AccelByte.Tests.ServerLobby.B.SomeoneJoinParty_GameClientAndServerGetPartyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_SomeoneJoinParty_GameClientAndServerGetPartyStorage_Success::RunTest(const FString& Parameters)
{
	int PartySize = 2;
	SLobbyConnect(PartySize);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	SLobbies[1]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
	SLobbies[1]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
	SLobbies[1]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	AB_TEST_TRUE(bSCreatePartySuccess);

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	FString PartyId = ScreatePartyResponse.PartyId;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");

	TArray< FAccelByteModelsPartyDataNotif> PartyDataNotifResults;

	for (int i = 0; i < PartySize; i++)
	{
		int index = i;
		const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &index](FAccelByteModelsPartyDataNotif result)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("user %d Get Party Data Update Notif"), index);
			PartyDataNotifResults.Add(result);
		});
		SLobbies[i]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);
	}
	// assertion get party storage
	{
		bool bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ClientPartyData;
		SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ClientPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ClientPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ServerPartyData;
		FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ServerPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ServerPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		AB_TEST_EQUAL(ClientPartyData.Invitees.Num(), (PartySize - 1));
		AB_TEST_EQUAL(ServerPartyData.Invitees.Num(), (PartySize - 1));
		AB_TEST_EQUAL(ClientPartyData.Members.Num(), 1);
		AB_TEST_EQUAL(ServerPartyData.Members.Num(), 1);
	}

	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");
	AB_TEST_FALSE(bSJoinPartyError);

	// assertion get party storage
	{
		bool bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ClientPartyData;
		SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ClientPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ClientPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ServerPartyData;
		FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ServerPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ServerPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		AB_TEST_EQUAL(ClientPartyData.Invitees.Num(), 0);
		AB_TEST_EQUAL(ServerPartyData.Invitees.Num(), 0);
		AB_TEST_EQUAL(ClientPartyData.Members.Num(), PartySize);
		AB_TEST_EQUAL(ServerPartyData.Members.Num(), PartySize);
	}

	WaitUntil([&PartyDataNotifResults]() { return (PartyDataNotifResults.Num() == 2); }, 10.0);

	for (auto& PartyDataNotif : PartyDataNotifResults)
	{
		AB_TEST_TRUE(ComparePartyData(PartyDataNotif.Custom_attribute));
		AB_TEST_EQUAL(PartyDataNotif.Members.Num(), PartySize);
	}

	for (int i = 0; i < PartySize; i++)
	{
		bSLeavePartyError = false;
		bSLeavePartySuccess = false;
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSLeavePartySuccess, "Waiting leave party");
		AB_TEST_FALSE(bSLeavePartyError);
	}

	SLobbyDisconnect(PartySize);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_SomeoneLeaveParty_GameClientAndServerGetPartyStorage_Success, "AccelByte.Tests.ServerLobby.B.SomeoneLeaveParty_GameClientAndServerGetPartyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_SomeoneLeaveParty_GameClientAndServerGetPartyStorage_Success::RunTest(const FString& Parameters)
{
	int PartySize = 3;
	SLobbyConnect(PartySize);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	for (int i = 1; i < PartySize; i++)
	{
		SLobbies[i]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
		SLobbies[i]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
		SLobbies[i]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	}

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	AB_TEST_FALSE(bSCreatePartyError);

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	FString PartyId = ScreatePartyResponse.PartyId;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");
	bSGetInvitedNotifSuccess = false;
	SLobbies[0]->SendInviteToPartyRequest(SUserIds[2]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");

	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");
	bSJoinPartySuccess = false;
	SLobbies[2]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");

	WaitSecond(3, "delay wait for random partydatanotif");

	TArray<FAccelByteModelsPartyDataNotif> PartyDataNotifResults = {};

	for (int i = 0; i < PartySize; i++)
	{
		int index = i;
		const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &index](FAccelByteModelsPartyDataNotif result)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("user %d Get Party Data Update Notif"), index);
			PartyDataNotifResults.Add(result);
		});
		SLobbies[i]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);
	}

	int LeaveMemberIndex = 1;
	SLobbies[LeaveMemberIndex]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting leave party");

	// assertion get party storage
	{
		bool bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ClientPartyData;
		SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ClientPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ClientPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ServerPartyData;
		FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ServerPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ServerPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		AB_TEST_EQUAL(ClientPartyData.Members.Num(), (PartySize - 1));
		AB_TEST_EQUAL(ServerPartyData.Members.Num(), (PartySize - 1));
		AB_TEST_FALSE(ClientPartyData.Members.Contains(SUserIds[LeaveMemberIndex]));
		AB_TEST_FALSE(ServerPartyData.Members.Contains(SUserIds[LeaveMemberIndex]));
	}

	WaitUntil([&PartyDataNotifResults, &PartySize]() { return (PartyDataNotifResults.Num() == (PartySize - 1)); }, 2.0);

	for (int i = 0; i < PartyDataNotifResults.Num(); i++)
	{
		if (i == LeaveMemberIndex) continue;
		FAccelByteModelsPartyDataNotif PartyDataNotif = PartyDataNotifResults[i];
		AB_TEST_TRUE(ComparePartyData(PartyDataNotif.Custom_attribute));
		AB_TEST_EQUAL(PartyDataNotif.Members.Num(), (PartySize - 1));
	}

	for (int i = 0; i < PartySize; i++)
	{
		if (i == LeaveMemberIndex) continue;
		bSLeavePartyError = false;
		bSLeavePartySuccess = false;
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSLeavePartySuccess, "Waiting leave party", 120);
		AB_TEST_FALSE(bSLeavePartyError);
	}

	SLobbyDisconnect(PartySize);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_SomeoneKicked_GameClientAndServerGetPartyStorage_Success, "AccelByte.Tests.ServerLobby.B.SomeoneKicked_GameClientAndServerGetPartyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_SomeoneKicked_GameClientAndServerGetPartyStorage_Success::RunTest(const FString& Parameters)
{
	int PartySize = 3;
	SLobbyConnect(PartySize);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	for (int i = 1; i < PartySize; i++)
	{
		SLobbies[i]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
		SLobbies[i]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
		SLobbies[i]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	}

	SLobbies[0]->SetInvitePartyKickMemberResponseDelegate(ServerKickPartyMemberDelegate);
	SLobbies[1]->SetPartyKickNotifDelegate(ServerKickedFromPartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	AB_TEST_FALSE(bSCreatePartyError);

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	FString PartyId = ScreatePartyResponse.PartyId;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");
	bSGetInvitedNotifSuccess = false;
	SLobbies[0]->SendInviteToPartyRequest(SUserIds[2]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");

	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");
	bSJoinPartySuccess = false;
	SLobbies[2]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");

	WaitSecond(3, "delay wait for random partydatanotif");

	TArray<FAccelByteModelsPartyDataNotif> PartyDataNotifResults;

	for (int i = 0; i < PartySize; i++)
	{
		int index = i;
		const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &index](FAccelByteModelsPartyDataNotif result)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("user %d Get Party Data Update Notif"), index);
			PartyDataNotifResults.Add(result);
		});
		SLobbies[i]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);
	}

	int KickedMemberIndex = 1;

	SLobbies[0]->SendKickPartyMemberRequest(SUserCreds[KickedMemberIndex].GetUserId());
	Waiting(bSKickPartyMemberSuccess, "kicking party member...");
	Waiting(bSKickedFromPartySuccess, "waiting to get kicked from party...");

	// assertion get party storage
	{
		bool bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ClientPartyData;
		SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ClientPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ClientPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		bGetPartyStorageSuccess = false;
		FAccelByteModelsPartyDataNotif ServerPartyData;
		FRegistry::ServerLobby.GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &ServerPartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			ServerPartyData = result;
		}), ServerLobbyErrorHandler);
		Waiting(bGetPartyStorageSuccess, "Waiting for get result");

		AB_TEST_EQUAL(ClientPartyData.Members.Num(), (PartySize - 1));
		AB_TEST_EQUAL(ServerPartyData.Members.Num(), (PartySize - 1));
		AB_TEST_FALSE(ClientPartyData.Members.Contains(SUserIds[KickedMemberIndex]));
		AB_TEST_FALSE(ServerPartyData.Members.Contains(SUserIds[KickedMemberIndex]));
	}

	WaitUntil([&PartyDataNotifResults, &PartySize]() { return (PartyDataNotifResults.Num() == (PartySize - 1)); }, 10.0);

	for (int i = 0; i < PartyDataNotifResults.Num(); i++)
	{
		if (i == KickedMemberIndex) continue;
		FAccelByteModelsPartyDataNotif PartyDataNotif = PartyDataNotifResults[i];
		AB_TEST_TRUE(ComparePartyData(PartyDataNotif.Custom_attribute));
		AB_TEST_EQUAL(PartyDataNotif.Members.Num(), (PartySize - 1));
	}

	for (int i = 0; i < PartySize; i++)
	{
		if (i == KickedMemberIndex) continue;
		bSLeavePartyError = false;
		bSLeavePartySuccess = false;
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSLeavePartySuccess, "Waiting leave party", 120);
		AB_TEST_FALSE(bSLeavePartyError);
	}

	SLobbyDisconnect(PartySize);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientGetPartyStorage_NonExistParty, "AccelByte.Tests.ServerLobby.B.ClientGetPartyStorage_NonExistParty_Fail", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientGetPartyStorage_NonExistParty::RunTest(const FString& Parameters)
{
	bool bGetPartyStorageSuccess = false;
	bool bGetPartyStorageDone = false;
	FString DummyPartyId = "thisPartyNeverExisted";

	FAccelByteModelsPartyDataNotif PartyData;
	SLobbies[0]->GetPartyStorage(DummyPartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageDone, &bGetPartyStorageSuccess](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageDone = true;
			bGetPartyStorageSuccess = true;
		}), FErrorHandler::CreateLambda([&bGetPartyStorageDone](int32 ErrorCode, FString ErrorMessage)
			{
				bGetPartyStorageDone = true;
			}));
	Waiting(bGetPartyStorageDone, "Waiting for get result");

	AB_TEST_FALSE(bGetPartyStorageSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientGetPartyStorage_ValidPartyButEmptyStorage_Success, "AccelByte.Tests.ServerLobby.B.ClientGetPartyStorage_ValidPartyButEmptyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientGetPartyStorage_ValidPartyButEmptyStorage_Success::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	bool bGetPartyStorageSuccess = false;
	FAccelByteModelsPartyDataNotif PartyData;

	SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &PartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			PartyData = result;
		}), ServerLobbyErrorHandler);
	Waiting(bGetPartyStorageSuccess, "Waiting for get result");

	AB_TEST_TRUE(bGetPartyStorageSuccess);
	AB_TEST_EQUAL(PartyData.Custom_attribute.JsonObject->Values.Num(), 0);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientGetPartyStorage_ValidPartyStorage_Success, "AccelByte.Tests.ServerLobby.B.ClientGetPartyStorage_ValidPartyStorage_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientGetPartyStorage_ValidPartyStorage_Success::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	GameClientOverwritePartyStorage(SLobbies[0].Get(), PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			WriteNotif = notifResult;
			bWritePartyStorageSuccess = true;
		}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	AB_TEST_TRUE(bWritePartyStorageSuccess);

	bool bGetPartyStorageSuccess = false;
	FAccelByteModelsPartyDataNotif PartyData;

	SLobbies[0]->GetPartyStorage(PartyId, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bGetPartyStorageSuccess, &PartyData](FAccelByteModelsPartyDataNotif result)
		{
			bGetPartyStorageSuccess = true;
			PartyData = result;
		}), ServerLobbyErrorHandler);
	Waiting(bGetPartyStorageSuccess, "Waiting for get result");

	AB_TEST_TRUE(bGetPartyStorageSuccess);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientWritePartyStorage_NonExistParty, "AccelByte.Tests.ServerLobby.B.ClientWritePartyStorageInValidParty_Fail", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientWritePartyStorage_NonExistParty::RunTest(const FString& Parameters)
{
	FString PartyId = "InvalidParty2434ID";
	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageFailed = false;
	bool bWritePartyStorageDone = false;
	GameClientOverwritePartyStorage(SLobbies[0].Get(), PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageDone, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			WriteNotif = notifResult;
			bWritePartyStorageDone = true;;
		}), FErrorHandler::CreateLambda([&bWritePartyStorageDone, &bWritePartyStorageFailed](int32 ErrorCode, FString ErrorMessage)
			{
				UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Can't write party storage. Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
				bWritePartyStorageFailed = true;
				bWritePartyStorageDone = true;
			}));
	Waiting(bWritePartyStorageDone, "Waiting for write result");

	AB_TEST_TRUE(bWritePartyStorageFailed);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientWriteEmptyPartyStorage_ValidParty, "AccelByte.Tests.ServerLobby.B.ClientWriteEmptyPartyStorageValidParty_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientWriteEmptyPartyStorage_ValidParty::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	FJsonObjectWrapper EmptyStorage;
	EmptyStorage.JsonObject = MakeShared<FJsonObject>();
	GameClientOverwritePartyStorage(SLobbies[0].Get(), PartyId, EmptyStorage, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			WriteNotif = notifResult;
			bWritePartyStorageSuccess = true;
		}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	AB_TEST_TRUE(bWritePartyStorageSuccess);

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientWritePartyStorage_Success, "AccelByte.Tests.ServerLobby.B.ClientWritePartyStorage", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientWritePartyStorage_Success::RunTest(const FString& Parameters)
{
	SLobbyConnect(1);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Waiting for create party");

	FAccelByteModelsPartyDataNotif PartyDataNotifResult;
	bool bGetPartyDataNotif = false;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResult,&bGetPartyDataNotif](FAccelByteModelsPartyDataNotif result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
		bGetPartyDataNotif = true;
		PartyDataNotifResult = result;
	});
	SLobbies[0]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);

	FString PartyId = ScreatePartyResponse.PartyId;
	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	GameClientOverwritePartyStorage(SLobbies[0].Get(), PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			WriteNotif = notifResult;
			bWritePartyStorageSuccess = true;
		}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");
	Waiting(bGetPartyDataNotif, "Waiting get party data notif");

	AB_TEST_TRUE(bWritePartyStorageSuccess);
	AB_TEST_TRUE(bGetPartyDataNotif);
	AB_TEST_TRUE(ComparePartyData(PartyDataNotifResult.Custom_attribute));

	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting for leave party");

	SLobbyDisconnect(1);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientWritePartyStorageConcurrent, "AccelByte.Tests.ServerLobby.B.ClientWritePartyStorageConcurrent", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientWritePartyStorageConcurrent::RunTest(const FString& Parameters)
{
	int PartySize = 2;
	SLobbyConnect(PartySize);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	SLobbies[1]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
	SLobbies[1]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
	SLobbies[1]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	AB_TEST_FALSE(bSCreatePartyError);

	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");

	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");

	FString PartyId = ScreatePartyResponse.PartyId;
	int WriteSuccessCount = 0;
	int WriteFailCount = 0;
	int ResultCount = 0;

	for (int i = 0; i < PartySize; i++)
	{
		GameClientOverwritePartyStorage(SLobbies[i].Get(), PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&WriteSuccessCount, &ResultCount](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			ResultCount++;
			WriteSuccessCount++;
		}), FErrorHandler::CreateLambda([&WriteFailCount,&ResultCount](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Can't write party storage. Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			ResultCount++;
			WriteFailCount++;
		}));
	}

	WaitUntil([&ResultCount] {return ResultCount == 2; }, 2.5, "Waiting for write result");

	// rarely happen
	bool bWriteSuccessPass = WriteSuccessCount == 1 || WriteSuccessCount == 2;
	bool bWriteFailPass = WriteFailCount == 1 || WriteFailCount == 0;
	AB_TEST_TRUE(bWriteSuccessPass);
	AB_TEST_TRUE(bWriteFailPass);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_ClientOverwritePartyStorage_PartyFormed_SuccessGotNotification, "AccelByte.Tests.ServerLobby.B.ClientOverwritePartyStorage_PartyFormed_SuccessGotNotification", AutomationFlagMaskServerLobby);
bool PartyStorageTest_ClientOverwritePartyStorage_PartyFormed_SuccessGotNotification::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	TQueue<FAccelByteModelsPartyDataNotif> PartyDataNotifResults;
	int UpdateCount = 0;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &UpdateCount](FAccelByteModelsPartyDataNotif result)
		{
			UpdateCount++;
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
			PartyDataNotifResults.Enqueue(result);
		});
	for (int i = 0; i < SLobbies.Num(); i++)
	{
		SLobbies[i]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);
	}

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	GameClientOverwritePartyStorage(SLobbies[0].Get(), PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
			WriteNotif = notifResult;
			bWritePartyStorageSuccess = true;
		}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	WaitUntil([&UpdateCount]() { return UpdateCount == 4; }, 5);

	AB_TEST_TRUE(bSGetInfoPartySuccess);
	AB_TEST_TRUE(bWritePartyStorageSuccess);

	while (!PartyDataNotifResults.IsEmpty())
	{
		FAccelByteModelsPartyDataNotif PartyData;
		PartyDataNotifResults.Dequeue(PartyData);
		AB_TEST_TRUE(ComparePartyData(PartyData.Custom_attribute));
	}

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_GetActivesPartyAfterPartyCreated, "AccelByte.Tests.ServerLobby.B.GetActivesPartyAfterPartyCreated_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_GetActivesPartyAfterPartyCreated::RunTest(const FString& Parameters)
{
	bool bPartyFormed = false;
	FormParty(FSimpleDelegate::CreateLambda([&]() { bPartyFormed = true; }));
	WaitUntil([&]()
	{
		return bPartyFormed;
	}, 30, "Waiting for Party Formed...");

	bSGetInfoPartySuccess = false;
	SLobbies[0]->SendInfoPartyRequest();
	Waiting(bSGetInfoPartySuccess, "Getting Info Party...");

	FString PartyId = SinfoPartyResponse.PartyId;

	FAccelByteModelsPartyDataNotif WriteNotif;
	bool bWritePartyStorageSuccess = false;
	OverwritePartyStorage(PartyId, SPartyStorageData, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &WriteNotif](FAccelByteModelsPartyDataNotif notifResult)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Write Party Storage Success"));
		WriteNotif = notifResult;
		bWritePartyStorageSuccess = true;
	}), ServerLobbyErrorHandler);
	Waiting(bWritePartyStorageSuccess, "Waiting for write result");

	bool bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData;
	FRegistry::ServerLobby.GetActiveParties(9999, 0,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");

	FAccelByteModelsPartyDataNotif PartyDataNotif;
	bool bHasParty = PartiesData.Data.ContainsByPredicate([&WriteNotif, &PartyDataNotif](FAccelByteModelsPartyDataNotif party)
	{
		if (WriteNotif.PartyId == party.PartyId)
		{
			PartyDataNotif = party;
			return true;
		}
		else
			return false;
	});

	AB_TEST_TRUE(bHasParty);
	AB_TEST_TRUE(bGetActivePartiesSuccess);
	AB_TEST_TRUE(ComparePartyData(PartyDataNotif.Custom_attribute));

	bool bCleanUpDone = false;
	CleanPartyBeforeTest(FSimpleDelegate::CreateLambda([&]() { bCleanUpDone = true; }));
	Waiting(bCleanUpDone, "Waiting for clean up done");
	SLobbyDisconnect(4);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_GetActivesParty_CheckLimitAndOffsetParam, "AccelByte.Tests.ServerLobby.B.GetActivesParty_CheckLimitAndOffsetParam", AutomationFlagMaskServerLobby);
bool PartyStorageTest_GetActivesParty_CheckLimitAndOffsetParam::RunTest(const FString& Parameters)
{
	int PartySize = 4;
	SLobbyConnect(PartySize);

	for (int i = 0; i < PartySize; i++)
	{
		SLobbies[i]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
		SLobbies[i]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

		bSLeavePartySuccess = false;
		bSLeavePartyError = false;
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSLeavePartySuccess, "waiting leave party");
		bSCreatePartySuccess = false;
		bSCreatePartyError = false;
		SLobbies[i]->SendCreatePartyRequest();
		Waiting(bSCreatePartySuccess, "waiting leave party");
		AB_TEST_FALSE(bSCreatePartyError);
	}

	bool bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData1;
	FRegistry::ServerLobby.GetActiveParties(2, 0,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData1](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData1 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData2;
	FRegistry::ServerLobby.GetActiveParties(2, 2,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData2](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData2 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	for (int i = 0; i < PartySize; i++)
	{
		bSLeavePartySuccess = false;
		bSLeavePartyError = false;
		SLobbies[i]->SendLeavePartyRequest();
	}

	AB_TEST_EQUAL(PartiesData1.Data.Num(), 2);
	AB_TEST_EQUAL(PartiesData2.Data.Num(), 2);
	TSet<FString> HashSet;
	bool bHasRepetitivePartyId = false;
	for (auto& PartyData : PartiesData1.Data)
	{
		HashSet.Add(PartyData.PartyId, &bHasRepetitivePartyId);
	}
	AB_TEST_FALSE(bHasRepetitivePartyId);

	SLobbyDisconnect(PartySize);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_GetActivesParty_UsingPagination, "AccelByte.Tests.ServerLobby.B.GetActivesParty_UsingPagination", AutomationFlagMaskServerLobby);
bool PartyStorageTest_GetActivesParty_UsingPagination::RunTest(const FString& Parameters)
{
	int PartySize = 4;
	SLobbyConnect(PartySize);

	for (int i = 0; i < PartySize; i++)
	{
		SLobbies[i]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
		SLobbies[i]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

		bSLeavePartySuccess = false;
		bSLeavePartyError = false;
		SLobbies[i]->SendLeavePartyRequest();
		Waiting(bSLeavePartySuccess, "waiting leave party");
		bSCreatePartySuccess = false;
		bSCreatePartyError = false;
		SLobbies[i]->SendCreatePartyRequest();
		Waiting(bSCreatePartySuccess, "waiting leave party");
		AB_TEST_FALSE(bSCreatePartyError);
	}

	// page 1
	bool bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData1;
	FRegistry::ServerLobby.GetActiveParties(1, 0,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData1](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData1 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	// next page (2)
	bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData2;
	FRegistry::ServerLobby.GetActiveParties(PartiesData1.Paging, EAccelBytePaginationType::NEXT,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData2](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData2 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	// first page (1)
	bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData3;
	FRegistry::ServerLobby.GetActiveParties(PartiesData1.Paging, EAccelBytePaginationType::FIRST,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData3](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData3 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	// previous page from page 2 (1)
	bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData4;
	FRegistry::ServerLobby.GetActiveParties(PartiesData2.Paging, EAccelBytePaginationType::PREVIOUS,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData4](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData4 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	// next page from page 2 (3)
	bGetActivePartiesSuccess = false;
	FAccelByteModelsActivePartiesData PartiesData5;
	FRegistry::ServerLobby.GetActiveParties(PartiesData2.Paging, EAccelBytePaginationType::NEXT,
	THandler< FAccelByteModelsActivePartiesData>::CreateLambda([&bGetActivePartiesSuccess, &PartiesData5](FAccelByteModelsActivePartiesData result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Active Party Success"));
		bGetActivePartiesSuccess = true;
		PartiesData5 = result;
	}), ServerLobbyErrorHandler);
	Waiting(bGetActivePartiesSuccess, "Waiting for get result");
	AB_TEST_TRUE(bGetActivePartiesSuccess);

	for (int i = 0; i < PartySize; i++)
	{
		bSLeavePartySuccess = false;
		bSLeavePartyError = false;
		SLobbies[i]->SendLeavePartyRequest();
	}

	AB_TEST_EQUAL(PartiesData1.Data.Num(), 1);
	AB_TEST_EQUAL(PartiesData2.Data.Num(), 1);
	AB_TEST_EQUAL(PartiesData3.Data.Num(), 1);
	AB_TEST_EQUAL(PartiesData4.Data.Num(), 1);
	AB_TEST_EQUAL(PartiesData5.Data.Num(), 1);

	FString FirstPartyId = PartiesData1.Data[0].PartyId;
	AB_TEST_NOT_EQUAL(PartiesData2.Data[0].PartyId, FirstPartyId);
	AB_TEST_EQUAL(PartiesData3.Data[0].PartyId, FirstPartyId);
	AB_TEST_EQUAL(PartiesData4.Data[0].PartyId, FirstPartyId);

	TSet<FString> HashSet;
	bool bHasRepetitivePartyId = false;
	for (auto& PartyData : PartiesData1.Data)
	{
		HashSet.Add(PartyData.PartyId, &bHasRepetitivePartyId);
	}
	AB_TEST_FALSE(bHasRepetitivePartyId);

	SLobbyDisconnect(PartySize);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyStorageTest_WriteRacingCondition_6ServersAtOnce_Success, "AccelByte.Tests.ServerLobby.B.WriteRacingCondition_6ServersAtOnce_Success", AutomationFlagMaskServerLobby);
bool PartyStorageTest_WriteRacingCondition_6ServersAtOnce_Success::RunTest(const FString& Parameters)
{
	SLobbyConnect(1);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting for leave party");

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Waiting for create party");

	TArray< FAccelByteModelsPartyDataNotif> PartyDataNotifResults;
	bool bSuccessGetPartyDataNotif = false;
	const auto PartyDataUpdateResponse = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([&PartyDataNotifResults, &bSuccessGetPartyDataNotif](FAccelByteModelsPartyDataNotif result)
	{
		UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Get Party Data Update Notif"));
		PartyDataNotifResults.Add(result);
		bSuccessGetPartyDataNotif = true;
	});
	SLobbies[0]->SetPartyDataUpdateResponseDelegate(PartyDataUpdateResponse);

	const int ConcurrentServerWriteCount = 6;
	int ServerUpdateSuccessCount = 0;
	int ServerUpdateDone = 0;

	FString PartyId = ScreatePartyResponse.PartyId;
	FJsonObjectWrapper PartyStorageData;
	PartyStorageData.JsonObject = MakeShared<FJsonObject>();
	for (int i = 0; i < ConcurrentServerWriteCount; i++)
	{
		FString Key = FString::Printf(TEXT("key_%d"), i);
		PartyStorageData.JsonObject->SetNumberField(Key, i);
		bool bWritePartyStorageSuccess = false;
		FRegistry::ServerLobby.WritePartyStorage(PartyId, [PartyStorageData](FJsonObjectWrapper) { return PartyStorageData; }, THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([&bWritePartyStorageSuccess, &ServerUpdateSuccessCount, &ServerUpdateDone](FAccelByteModelsPartyDataNotif notifResult)
		{
			ServerUpdateSuccessCount++;
			ServerUpdateDone++;
			bWritePartyStorageSuccess = true;
		}), FErrorHandler::CreateLambda([&ServerUpdateDone](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteServerLobbyTest, Log, TEXT("Can't write party storage. Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			ServerUpdateDone++;
		}));
		Waiting(bWritePartyStorageSuccess, "write party storage successful");
	}

	bSLeavePartySuccess = false;
	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Waiting for leave party");
	WaitUntil([&ConcurrentServerWriteCount, &PartyDataNotifResults]() { return ConcurrentServerWriteCount == PartyDataNotifResults.Num(); }, 2, "Wait until write done");

	AB_TEST_EQUAL(PartyDataNotifResults.Num(), ConcurrentServerWriteCount);
	bool bLatestNotifFound = false;
	for (auto& PartyDataNotif : PartyDataNotifResults)
	{
		if (PartyDataNotif.Custom_attribute.JsonObject->Values.Num() == ConcurrentServerWriteCount)
		{
			bLatestNotifFound = true;
		}
	}
	AB_TEST_TRUE(bLatestNotifFound);

	SLobbyDisconnect(1);
	ResetServerResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerLobbyQueryPartyByUserId, "AccelByte.Tests.ServerLobby.C.QueryPartyByUserId_AllOk", AutomationFlagMaskServerLobby);
bool ServerLobbyQueryPartyByUserId::RunTest(const FString& Parameters)
{
	SLobbyConnect(2);

	SLobbies[0]->SetCreatePartyResponseDelegate(SCreatePartyDelegate);
	SLobbies[0]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);
	SLobbies[1]->SetPartyGetInvitedNotifDelegate(SInvitedToPartyDelegate);
	SLobbies[1]->SetInvitePartyJoinResponseDelegate(SJoinPartyDelegate);
	SLobbies[1]->SetLeavePartyResponseDelegate(SLeavePartyDelegate);

	// Setup, make sure users have no party
	bSLeavePartySuccess = false;
	SLobbies[0]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Lobby 0 waiting setup leave party");
	bSLeavePartySuccess = false;
	SLobbies[1]->SendLeavePartyRequest();
	Waiting(bSLeavePartySuccess, "Lobby 1 waiting setup leave party");
	bSLeavePartySuccess = false;

	SLobbies[0]->SendCreatePartyRequest();
	Waiting(bSCreatePartySuccess, "Creating party");
	AB_TEST_FALSE(bSCreatePartyError);

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

	bSGetInvitedNotifSuccess = false;
	SLobbies[0]->SendInviteToPartyRequest(SUserIds[1]);
	WaitUntil([&]()
	{
		return bSGetInvitedNotifSuccess;
	}, 30, "Waiting party invitation");

	SLobbies[1]->SendAcceptInvitationRequest(*SinvitedToPartyResponse.PartyId, *SinvitedToPartyResponse.InvitationToken);
	Waiting(bSJoinPartySuccess, "Waiting join party");
	AB_TEST_FALSE(bSJoinPartyError);

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
	AB_TEST_TRUE(bSLeavePartySuccess);

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
	AB_TEST_TRUE(bSLeavePartySuccess);

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

	AB_TEST_TRUE(ScreatePartyResponse.PartyId.Equals(queryAfterCreateParty.partyId));

	bool user1FoundAfterJoin = false;
	for (FString id : queryAfterInviteToParty0.members)
	{
		if (id == SUserIds[1])
		{
			user1FoundAfterJoin = true;
		}
	}
	AB_TEST_TRUE(user1FoundAfterJoin);
	AB_TEST_TRUE(ScreatePartyResponse.PartyId.Equals(queryAfterInviteToParty1.partyId));

	AB_TEST_TRUE(queryAfterLeaveNotFound1);
	bool user1FoundAfterLeave = false;
	for (FString id : queryAfterLeaveParty0.members)
	{
		if (id == SUserIds[1])
		{
			user1FoundAfterLeave = true;
		}
	}
	AB_TEST_FALSE(user1FoundAfterLeave);

	AB_TEST_TRUE(queryAfterDisbandPartyNotFound0);

	SLobbyDisconnect(2);
	ResetServerResponses();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerLobbyGetSetSessionAttribute_Ok, "AccelByte.Tests.ServerLobby.C.SessionAttributeGetSet_Ok", AutomationFlagMaskServerLobby);
bool ServerLobbyGetSetSessionAttribute_Ok::RunTest(const FString& Parameters)
{
	// Setup session attribute contents
	const TMap<FString, FString> Attributes = { {"mmr", "50"}, {"sr", "50"} };
	TArray<FString> AttributeKeys = {};
	Attributes.GenerateKeyArray(AttributeKeys);

	// Setup Lobby client
	SLobbyConnect(1);

	// ACT
	// Setup User set Session Attribute
	SLobbies[0]->SetSetSessionAttributeDelegate(ServerSetSessionAttributeDelagate);
	SLobbies[0]->SetSessionAttribute(AttributeKeys[0], Attributes[AttributeKeys[0]]);

	// Setup Server set session attribute
	bool bServerSetSessionAttributeComplete = false;
	FRegistry::ServerLobby.SetSessionAttribute(SUserIds[0], AttributeKeys[1], Attributes[AttributeKeys[1]], FVoidHandler::CreateLambda([&bServerSetSessionAttributeComplete]() {
		bServerSetSessionAttributeComplete = true;
	}), ServerLobbyErrorHandler);
	Waiting(bServerSetSessionAttributeComplete, "Waiting server set session attribute 1");

	// Setup Server get session attribute
	bool bServerGetSessionAttribute = false;
	FAccelByteModelsGetSessionAttributeResponse getSessionAttributeResult;
	FRegistry::ServerLobby.GetSessionAttribute(SUserIds[0], AttributeKeys[0], THandler<FAccelByteModelsGetSessionAttributeResponse>::CreateLambda([&bServerGetSessionAttribute, &getSessionAttributeResult](const FAccelByteModelsGetSessionAttributeResponse& result)
	{
		bServerGetSessionAttribute = true;
		getSessionAttributeResult = result;
	}), ServerLobbyErrorHandler);

	bool bServerGetSessionAttributeAll = false;
	FAccelByteModelsGetSessionAttributeAllResponse getSessionAttributeAllResult;
	FRegistry::ServerLobby.GetSessionAttributeAll(SUserIds[0], THandler<FAccelByteModelsGetSessionAttributeAllResponse>::CreateLambda([&bServerGetSessionAttributeAll, &getSessionAttributeAllResult](const FAccelByteModelsGetSessionAttributeAllResponse& result)
	{
		bServerGetSessionAttributeAll = true;
		getSessionAttributeAllResult = result;
	}), ServerLobbyErrorHandler);

	Waiting(bServerGetSessionAttribute, "Waiting get session attribute response");
	Waiting(bServerGetSessionAttributeAll, "Waiting get session attribute all response");

	// ASSERT
	AB_TEST_TRUE(bServerSetSessionAttributeComplete);
	AB_TEST_TRUE(bServerGetSessionAttribute);
	AB_TEST_EQUAL(getSessionAttributeResult.Key, AttributeKeys[0]);
	AB_TEST_EQUAL(getSessionAttributeResult.Value, Attributes[AttributeKeys[0]]);
	AB_TEST_TRUE(bServerGetSessionAttributeAll);
	
	bool attributesInconsistent = false;
	for (auto attribute : Attributes)
	{
		if (!getSessionAttributeAllResult.Attributes.Contains(attribute.Key))
		{
			attributesInconsistent = true;
			break;
		}

		if (attribute.Value != getSessionAttributeAllResult.Attributes[attribute.Key])
		{
			attributesInconsistent = true;
			break;
		}
	}
	AB_TEST_FALSE(attributesInconsistent);
	
	SLobbyDisconnect(1);
	ResetServerResponses();

	return true;
}
