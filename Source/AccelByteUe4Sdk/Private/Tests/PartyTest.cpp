// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteSessionApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "UserTestAdmin.h"
#include "TestUtilities.h"
#include "NVIDIA/CUDA/cuda.h"
#include "Api/AccelByteQos.h"
#include "Core/AccelByteReport.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using Api::User;
using Api::Session;

FAccelByteModelsV2PartySession Party;
FString InviteeUserID;
Credentials InviteeCredentials;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelBytePartyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelBytePartyTest);

const int32 AutomationFlagMaskParty = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext;

const auto PartyErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelBytePartyTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

static bool TestPartyMembership(const FAccelByteModelsV2PartySession& TestParty, const FString& MemberID, const FString& Status=TEXT("active"))
{
	for(auto& Member : TestParty.Members)
	{
		if(Member.ID.Equals(MemberID))
		{
			return Member.Status.Equals(Status);
		}
	}

	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySetup, "AccelByte.Tests.Party.A.Setup", AutomationFlagMaskParty);
bool PartySetup::RunTest(const FString& Parameters)
{
	bool bLoginDone = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone] { bLoginDone = true; }), PartyErrorHandler);	
	WaitUntil(bLoginDone, "Waiting for LoginWithDeviceId...");

	User Invitee(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);

	InviteeCredentials.SetClientCredentials(FRegistry::Credentials.GetOAuthClientId(), FRegistry::Credentials.GetOAuthClientSecret());

	// Create test user and log in
	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	const FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime BirthDateTime = FDateTime::Now() - FTimespan::FromDays(365 * 25);
	const FString DateOfBirth = FString::Printf(TEXT("%04d-%02d-%02d"), BirthDateTime.GetYear(), BirthDateTime.GetMonth(), BirthDateTime.GetDay());

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelBytePartyTest, Log, TEXT("Creating test user..."));
	Invitee.Register(EmailAddress, Password, DisplayName, Country, DateOfBirth,
		THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse&)
		{
			UE_LOG(LogAccelBytePartyTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}),
		FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
		{
			UE_LOG(LogAccelBytePartyTest, Warning, TEXT("	Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
			bRegisterDone = true;
		}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for registration...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bLoginDone = false;
	UE_LOG(LogAccelBytePartyTest, Log, TEXT("Logging in test user..."));
	Invitee.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
	{
		InviteeUserID = InviteeCredentials.GetUserId();
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("	Success"));
		bLoginDone = true;
	}),
	FCustomErrorHandler::CreateLambda([](int Code, const FString& Message, const FJsonObject&)
	{
		UE_LOG(LogAccelBytePartyTest, Warning, TEXT("	Error. Code: %d, Reason: %s"), Code, *Message);
	}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for login...");
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyCreate, "AccelByte.Tests.Party.B.Create", AutomationFlagMaskParty);
bool PartyCreate::RunTest(const FString& Parameters)
{
	bool bCreatePartySuccess = false;
	FAccelByteModelsV2PartySession Response;
	FAccelByteModelsV2PartyCreateRequest Request;
	Request.JoinType = TEXT("inviteOnly");
	FRegistry::Session.CreateParty(Request, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bCreatePartySuccess, &Response](FAccelByteModelsV2PartySession const PartyResponse)
	{
		bCreatePartySuccess = true;
		Response = PartyResponse;
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Create party success"));
	}), PartyErrorHandler);
	WaitUntil(bCreatePartySuccess, "Waiting for party creation...");

	Party = Response;
	
	AB_TEST_TRUE(bCreatePartySuccess);
	AB_TEST_EQUAL(Party.JoinType, Request.JoinType);
	// TODO: Test other fields
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyGetPartyDetails, "AccelByte.Tests.Party.C.GetPartyDetails", AutomationFlagMaskParty);
bool PartyGetPartyDetails::RunTest(const FString& Parameters)
{
	bool bGetDetailsSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FRegistry::Session.GetPartyDetails(Party.ID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bGetDetailsSuccess, &Response](FAccelByteModelsV2PartySession const PartyResponse)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Get party details success"));
		bGetDetailsSuccess = true;
		Response = PartyResponse;
	}), PartyErrorHandler);
	WaitUntil(bGetDetailsSuccess, "Waiting for get party details...");
	
	AB_TEST_TRUE(bGetDetailsSuccess);
	AB_TEST_EQUAL(Response.ID, Party.ID);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyUpdate, "AccelByte.Tests.Party.C.Update", AutomationFlagMaskParty);
bool PartyUpdate::RunTest(const FString& Parameters)
{
	bool bUpdateSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FAccelByteModelsV2PartyUpdateRequest Request;

	Request.JoinType = TEXT("open");
	Request.Version = Party.Version;
	
	FRegistry::Session.UpdateParty(Party.ID, Request, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bUpdateSuccess, &Response](FAccelByteModelsV2PartySession const PartyResponse)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Party update success"));
		bUpdateSuccess = true;
		Response = PartyResponse;
	}), PartyErrorHandler);
	WaitUntil(bUpdateSuccess, "Waiting for party update...");
	
	AB_TEST_TRUE(bUpdateSuccess);
	AB_TEST_EQUAL(Response.ID, Party.ID);
	AB_TEST_EQUAL(Request.JoinType, TEXT("open"));

	Party = Response;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyInviteJoinFlow, "AccelByte.Tests.Party.C.InviteJoinFlow", AutomationFlagMaskParty);
bool PartyInviteJoinFlow::RunTest(const FString& Parameters)
{	
	Api::Lobby Lobby(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	Lobby.Connect();
	WaitUntil([&Lobby] { return Lobby.IsConnected(); }, "", 5);

	FAccelByteModelsV2PartyUserInvitedEvent InvitePayload;
	
	// TODO: probably need to have a delegate for v2 party invite. backend will need to be reworked to not use freeform notifs
	bool bGetInviteSuccess = false;
	const auto MessageNotifDelegate = Api::Lobby::FMessageNotif::CreateLambda([&bGetInviteSuccess, &InvitePayload](FAccelByteModelsNotificationMessage Message)
	{
		if(Message.Topic.Equals(Api::SessionTopic::UserInvited))
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(Message.Payload, &InvitePayload, 0, 0);
			bGetInviteSuccess = true;
		}
	});

	Lobby.SetMessageNotifDelegate(MessageNotifDelegate);
	
	bool bSendInviteSuccess = false;
	FRegistry::Session.SendPartyInvite(Party.ID, InviteeUserID, FVoidHandler::CreateLambda([&bSendInviteSuccess]
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Send party invite success"));
		bSendInviteSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bGetInviteSuccess, "Waiting for party invite notif...", 30);
	
	AB_TEST_TRUE(InvitePayload.PartyID.Equals(Party.ID));
	AB_TEST_TRUE(InvitePayload.SenderID.Equals(FRegistry::Credentials.GetUserId()));

	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	
	bool bJoinPartySuccess = false;
	FAccelByteModelsV2PartySession JoinedParty;
	InviteeSession.JoinParty(Party.ID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bJoinPartySuccess, &JoinedParty](FAccelByteModelsV2PartySession const PartyResponse)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Join party success"));
		bJoinPartySuccess = true;
		JoinedParty = PartyResponse;
	}), PartyErrorHandler);
	WaitUntil(bJoinPartySuccess, "Waiting for party join...");

	Lobby.Disconnect();
	WaitUntil([&Lobby] { return !Lobby.IsConnected(); }, "", 5);
	
	Party = JoinedParty;
	
	AB_TEST_TRUE(bSendInviteSuccess);
	AB_TEST_TRUE(bGetInviteSuccess);
	AB_TEST_TRUE(bJoinPartySuccess);
	AB_TEST_TRUE(TestPartyMembership(Party, InviteeUserID));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyLeave, "AccelByte.Tests.Party.D.Leave", AutomationFlagMaskParty);
bool PartyLeave::RunTest(const FString& Parameters)
{
	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	
	bool bLeaveSuccess = false;
	InviteeSession.LeaveParty(Party.ID, FVoidHandler::CreateLambda([&bLeaveSuccess]
	{
		bLeaveSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bLeaveSuccess, "Waiting for party leave...");
	
	bool bGetDetailsSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FRegistry::Session.GetPartyDetails(Party.ID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bGetDetailsSuccess, &Response](FAccelByteModelsV2PartySession const PartyResponse)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Get party details success"));
		bGetDetailsSuccess = true;
		Response = PartyResponse;
	}), PartyErrorHandler);
	WaitUntil(bGetDetailsSuccess, "Waiting for get party details...");

	Party = Response;
	
	AB_TEST_TRUE(bLeaveSuccess);
	AB_TEST_TRUE(!TestPartyMembership(Party, InviteeUserID));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyKickPlayer, "AccelByte.Tests.Party.E.KickPlayer", AutomationFlagMaskParty);
bool PartyKickPlayer::RunTest(const FString& Parameters)
{
	bool bInviteSuccess = false;
	FRegistry::Session.SendPartyInvite(Party.ID, InviteeUserID, FVoidHandler::CreateLambda([&bInviteSuccess]
	{
		bInviteSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bInviteSuccess, "Waiting for party invite...");
	
	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);

	// rejoin the party after leaving in the last test
	bool bJoinPartySuccess = false;
	InviteeSession.JoinParty(Party.ID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bJoinPartySuccess](FAccelByteModelsV2PartySession const)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Join party success"));
		bJoinPartySuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bJoinPartySuccess, "Waiting for party join...");
	
	bool bKickSuccess = false;
	FRegistry::Session.KickUserFromParty(Party.ID, InviteeUserID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bKickSuccess](FAccelByteModelsV2PartySession const)
	{
		bKickSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bKickSuccess, "Waiting for party kick...");

	bool bGetDetailsSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FRegistry::Session.GetPartyDetails(Party.ID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bGetDetailsSuccess, &Response](FAccelByteModelsV2PartySession const PartyResponse)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Get party details success"));
		bGetDetailsSuccess = true;
		Response = PartyResponse;
	}), PartyErrorHandler);
	WaitUntil(bGetDetailsSuccess, "Waiting for get party details...");

	Party = Response;
	
	AB_TEST_TRUE(bKickSuccess);
	AB_TEST_TRUE(!TestPartyMembership(Party, InviteeUserID));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyInviteReject, "AccelByte.Tests.Party.F.InviteReject", AutomationFlagMaskParty);
bool PartyInviteReject::RunTest(const FString& Parameters)
{
	bool bSendInviteSuccess = false;
	FRegistry::Session.SendPartyInvite(Party.ID, InviteeUserID, FVoidHandler::CreateLambda([&bSendInviteSuccess]
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Send party invite success"));
		bSendInviteSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bSendInviteSuccess, "Waiting for party invite success...", 30);

	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	
	bool bRejectInviteSuccess = false;
	FAccelByteModelsV2PartySession JoinedParty;
	InviteeSession.RejectPartyInvite(Party.ID, FVoidHandler::CreateLambda([&bRejectInviteSuccess]
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Reject invite success"));
		bRejectInviteSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bRejectInviteSuccess, "Waiting for invite rejection...");

	bool bGetDetailsSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FRegistry::Session.GetPartyDetails(Party.ID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bGetDetailsSuccess, &Response](FAccelByteModelsV2PartySession const PartyResponse)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Get party details success"));
		bGetDetailsSuccess = true;
		Response = PartyResponse;
	}), PartyErrorHandler);
	WaitUntil(bGetDetailsSuccess, "Waiting for get party details...");

	Party = Response;
	
	AB_TEST_TRUE(bSendInviteSuccess);
	AB_TEST_TRUE(bRejectInviteSuccess);
	AB_TEST_TRUE(!TestPartyMembership(Party, InviteeUserID));
	
	return true;
}
