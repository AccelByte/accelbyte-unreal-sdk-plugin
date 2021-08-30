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
	WaitUntil(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterServerToDSM(7777, FVoidHandler::CreateLambda([&bServerRegisterSuccess]()
	{
		bServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bServerRegisterSuccess, "Waiting for register server Url...");
	AB_TEST_TRUE(bServerRegisterSuccess);

	bool bServerShutdownSuccess = false;
	FRegistry::ServerDSM.SendShutdownToDSM(false, "", FVoidHandler::CreateLambda([&bServerShutdownSuccess]()
	{
		bServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Shutdown Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bServerShutdownSuccess, "Waiting for shutdown server Url...");
	AB_TEST_TRUE(bServerShutdownSuccess);
	return true;
}

//need to add arg -provider=baremetal when run the test, need to run on cloud
IMPLEMENT_SIMPLE_AUTOMATION_TEST(DSMAutoShutdown, "AccelByte.unTests.DSM.A.AutoShutdown", AutomationFlagMaskDSM);
bool DSMAutoShutdown::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			bClientLoginSuccess = true;
			UE_LOG(LogAccelByteDSMTest, Log, TEXT("Login Success..!"));
		}), DSMTestErrorHandler);
	FlushHttpRequests();
	WaitUntil(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bServerRegisterSuccess = false;
	FRegistry::ServerDSM.ConfigureAutoShutdown(5, 10);

	bool bAutoShutdownSuccess = false;
	FRegistry::ServerDSM.SetOnAutoShutdownResponse(FVoidHandler::CreateLambda([&bAutoShutdownSuccess]()
		{
			bAutoShutdownSuccess = true;
			UE_LOG(LogAccelByteDSMTest, Log, TEXT("Auto Shutting down!"));
		}));
	FRegistry::ServerDSM.RegisterServerToDSM(7777, FVoidHandler::CreateLambda([&bServerRegisterSuccess]()
		{
			bServerRegisterSuccess = true;
			UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Local Success..!"));
		}), DSMTestErrorHandler);
	FlushHttpRequests();
	WaitUntil(bServerRegisterSuccess, "Waiting for register local server Url...");
	AB_TEST_TRUE(bServerRegisterSuccess);

	bool bLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			bLoginSuccess = true;
		}), DSMTestErrorHandler);

	WaitUntil(bLoginSuccess, "Waiting for Login...");
	FlushHttpRequests();

	FRegistry::Lobby.Connect();
	while (!FRegistry::Lobby.IsConnected())
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Waiting for lobby connection..."));
		FTicker::GetCoreTicker().Tick(.5f);
	}

	bool bCreatePartySuccess = false;
	bool bCreatePartyError = false;
	FRegistry::Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartySuccess, &bCreatePartyError](const FAccelByteModelsCreatePartyResponse& Result)
		{
			bCreatePartySuccess = true;
			if (Result.PartyId.IsEmpty())
			{
				bCreatePartyError = true;
			}
		}));

	bool bGetInfoPartySuccess = false;
	bool bGetInfoPartyError = false;
	FRegistry::Lobby.SetInfoPartyResponseDelegate(THandler<FAccelByteModelsInfoPartyResponse>::CreateLambda([&bGetInfoPartySuccess, &bGetInfoPartyError](const FAccelByteModelsInfoPartyResponse& Result)
		{
			bGetInfoPartySuccess = true;
			if (Result.PartyId.IsEmpty())
			{
				bGetInfoPartyError = true;
			}
		}));

	bool bLeavePartySuccess = false;
	bool bLeavePartyError = false;
	FRegistry::Lobby.SetLeavePartyResponseDelegate(THandler<FAccelByteModelsLeavePartyResponse>::CreateLambda([&bLeavePartySuccess, &bLeavePartyError](const FAccelByteModelsLeavePartyResponse& Result)
		{
			bLeavePartySuccess = true;
			if (Result.Code != "0")
			{
				bLeavePartyError = true;
			}
		}));

	bool bReadyConsentResponseSuccess = false;
	FRegistry::Lobby.SetReadyConsentResponseDelegate(THandler<FAccelByteModelsReadyConsentRequest>::CreateLambda([&bReadyConsentResponseSuccess](const FAccelByteModelsReadyConsentRequest& Result)
		{
			bReadyConsentResponseSuccess = true;
		}));

	bool bReadyConsentNotifSuccess = false;
	bool bReadyConsentNotifError = false;
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse;
	FRegistry::Lobby.SetReadyConsentNotifDelegate(THandler<FAccelByteModelsReadyConsentNotice>::CreateLambda([&bReadyConsentNotifSuccess, &bReadyConsentNotifError, &readyConsentNoticeResponse](const FAccelByteModelsReadyConsentNotice& Result)
		{
			readyConsentNoticeResponse = Result;
			bReadyConsentNotifSuccess = true;
			if (Result.MatchId.IsEmpty())
			{
				bReadyConsentNotifError = true;
			}
		}));

	bool bDsNotifSuccess = false;
	bool bDsNotifError = false;
	FRegistry::Lobby.SetDsNotifDelegate(THandler<FAccelByteModelsDsNotice>::CreateLambda([&bDsNotifSuccess, &bDsNotifError](const FAccelByteModelsDsNotice& Result)
		{
			if (Result.Status == "READY" || Result.Status == "BUSY")
			{
				bDsNotifSuccess = true;
			}
			if (Result.MatchId.IsEmpty())
			{
				bDsNotifError = true;
			}
		}));

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse;
	bool bMatchmakingNotifSuccess = false;
	bool bMatchmakingNotifError = false;
	FRegistry::Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
		{
			UE_LOG(LogAccelByteDSMTest, Log, TEXT("Get Matchmaking Notification!"));
			matchmakingNotifResponse = result;
			bMatchmakingNotifSuccess = true;
			if (result.MatchId.IsEmpty())
			{
				bMatchmakingNotifError = true;
			}
		}));

	bool bStartMatchmakingSuccess = false;
	bool bStartMatchmakingError = false;
	FRegistry::Lobby.SetStartMatchmakingResponseDelegate(THandler<FAccelByteModelsMatchmakingResponse>::CreateLambda([&bStartMatchmakingSuccess, &bStartMatchmakingError](const FAccelByteModelsMatchmakingResponse& Result)
		{
			bStartMatchmakingSuccess = true;
			if (Result.Code != "0")
			{
				bStartMatchmakingError = true;
			}
		}));

	FRegistry::Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	FAllianceRule AllianceRule;
	AllianceRule.min_number = 1;
	AllianceRule.max_number = 1;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	bool bCreateMatchmakingChannelSuccess = false;
	Matchmaking_Create_Matchmaking_Channel(ChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
		{
			bCreateMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteDSMTest, Log, TEXT("Create Matchmaking Channel Success..!"));
		}), DSMTestErrorHandler);

	WaitUntil(bCreateMatchmakingChannelSuccess, "Create Matchmaking channel...");

	if (!bGetInfoPartyError)
	{
		FRegistry::Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	FRegistry::Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	FRegistry::Lobby.SendStartMatchmaking(ChannelName, "", "", { TPair<FString,float>("ap-southeast-1", 20) });

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil(bMatchmakingNotifSuccess, "Waiting for Matchmaking Notification...");

	FRegistry::Lobby.SendReadyConsentRequest(matchmakingNotifResponse.MatchId);

	WaitUntil(bReadyConsentNotifSuccess, "Waiting for Ready Consent Notification...");
	AB_TEST_FALSE(bReadyConsentNotifError);

	WaitUntil(bDsNotifSuccess, "Waiting for DS Notification...");
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	Matchmaking_Delete_Matchmaking_Channel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteDSMTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), DSMTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError);
	AB_TEST_FALSE(matchmakingNotifResponse.MatchId.IsEmpty());
	AB_TEST_TRUE(matchmakingNotifResponse.Status == EAccelByteMatchmakingStatus::Done);
	AB_TEST_TRUE(readyConsentNoticeResponse.MatchId == matchmakingNotifResponse.MatchId);

	FRegistry::Lobby.Disconnect();

	WaitUntil(bAutoShutdownSuccess, "Waiting DSM Auto Shutdown...", 20);
	AB_TEST_TRUE(bAutoShutdownSuccess);

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
	WaitUntil(bClientLoginSuccess, "Waiting for Login client...");
	AB_TEST_TRUE(bClientLoginSuccess);

	bool bLocalServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM("127.0.0.1", 7777, ServerName, FVoidHandler::CreateLambda([&bLocalServerRegisterSuccess]()
	{
		bLocalServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Local Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bLocalServerRegisterSuccess, "Waiting for register local server Url...");
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
	WaitUntil(bServerRegisterFailed, "Waiting for register server Url...");
	AB_TEST_TRUE(bServerRegisterFailed);

	bool bLocalServerShutdownSuccess = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM(ServerName, FVoidHandler::CreateLambda([&bLocalServerShutdownSuccess]()
	{
		bLocalServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Deregister Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bLocalServerShutdownSuccess, "Waiting for deregister server Url...");
	AB_TEST_TRUE(bLocalServerShutdownSuccess);

	bool bCloudServerRegisterSuccess = false;
	FRegistry::ServerDSM.RegisterServerToDSM(7777, FVoidHandler::CreateLambda([&bCloudServerRegisterSuccess]()
	{
		bCloudServerRegisterSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Register Cloud Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bCloudServerRegisterSuccess, "Waiting for register server Url...");
	AB_TEST_TRUE(bCloudServerRegisterSuccess);

	bool bCloudServerShutdownSuccess = false;
	FRegistry::ServerDSM.SendShutdownToDSM(false, "", FVoidHandler::CreateLambda([&bCloudServerShutdownSuccess]()
	{
		bCloudServerShutdownSuccess = true;
		UE_LOG(LogAccelByteDSMTest, Log, TEXT("Shutdown Success..!"));
	}), DSMTestErrorHandler);
	WaitUntil(bCloudServerShutdownSuccess, "Waiting for shutdown server Url...");
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