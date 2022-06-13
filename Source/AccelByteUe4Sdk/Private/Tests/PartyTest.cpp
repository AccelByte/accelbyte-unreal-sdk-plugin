// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteSessionApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"
#include "Api/AccelByteLobbyApi.h"

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

static bool TestPartyMembership(const FAccelByteModelsV2PartySession& TestParty, const FString& MemberID, const EAccelByteV2SessionMemberStatus& Status=EAccelByteV2SessionMemberStatus::ACTIVE)
{
	for(auto& Member : TestParty.Members)
	{
		if(Member.ID.Equals(MemberID))
		{
			return Member.Status == Status;
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
	Request.JoinType = EAccelByteV2SessionJoinability::INVITE_ONLY;
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyUpdate, "AccelByte.Tests.Party.D.Update", AutomationFlagMaskParty);
bool PartyUpdate::RunTest(const FString& Parameters)
{
	bool bUpdateSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FAccelByteModelsV2PartyUpdateRequest Request;

	Request.JoinType = EAccelByteV2SessionJoinability::OPEN;
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
	AB_TEST_EQUAL(Request.JoinType, EAccelByteV2SessionJoinability::OPEN);

	Party = Response;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyInviteJoinFlow, "AccelByte.Tests.Party.E.InviteJoinFlow", AutomationFlagMaskParty);
bool PartyInviteJoinFlow::RunTest(const FString& Parameters)
{	
	Api::Lobby Lobby(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	Lobby.Connect();
	WaitUntil([&Lobby] { return Lobby.IsConnected(); }, "", 5);

	FAccelByteModelsV2PartyInvitedEvent InvitePayload;
	
	bool bGetInviteSuccess = false;
	const auto NotifDelegate = Api::Lobby::FV2PartyInvited::CreateLambda([&bGetInviteSuccess, &InvitePayload](FAccelByteModelsV2PartyInvitedEvent Payload)
	{
		InvitePayload = Payload;
		bGetInviteSuccess = true;
	});

	Lobby.SetV2PartyInvitedDelegate(NotifDelegate);
	
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyLeave, "AccelByte.Tests.Party.F.Leave", AutomationFlagMaskParty);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyKickPlayer, "AccelByte.Tests.Party.G.KickPlayer", AutomationFlagMaskParty);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyInviteReject, "AccelByte.Tests.Party.H.InviteReject", AutomationFlagMaskParty);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyQuery, "AccelByte.Tests.Party.I.Query", AutomationFlagMaskParty);
bool PartyQuery::RunTest(const FString& Parameters)
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

	bool bQueryPartiesSuccess = false;

	FAccelByteModelsV2SessionQueryRequest Query1;
	Query1.MemberID = InviteeUserID;

	FAccelByteModelsV2PaginatedPartyQueryResult Response;
	FRegistry::Session.QueryParties(Query1, THandler<FAccelByteModelsV2PaginatedPartyQueryResult>::CreateLambda([&bQueryPartiesSuccess, &Response](FAccelByteModelsV2PaginatedPartyQueryResult const QueryResponse)
	{
		bQueryPartiesSuccess = true;
		Response = QueryResponse;
	}), PartyErrorHandler);
	WaitUntil(bQueryPartiesSuccess, "Waiting for party query...");

	AB_TEST_TRUE(bQueryPartiesSuccess);
	AB_TEST_TRUE(Response.Data.Num() > 0);
	for(auto& PartyResponse : Response.Data)
	{
		AB_TEST_EQUAL(PartyResponse.LeaderID, FRegistry::Credentials.GetUserId());
	}

	bQueryPartiesSuccess = false;

	FAccelByteModelsV2SessionQueryRequest Query2;
	Query2.JoinType = EAccelByteV2SessionJoinability::CLOSED;
	FRegistry::Session.QueryParties(Query2, THandler<FAccelByteModelsV2PaginatedPartyQueryResult>::CreateLambda([&bQueryPartiesSuccess, &Response](FAccelByteModelsV2PaginatedPartyQueryResult const QueryResponse)
	{
		bQueryPartiesSuccess = true;
		Response = QueryResponse;
	}), PartyErrorHandler);
	WaitUntil(bQueryPartiesSuccess, "Waiting for party query...");

	AB_TEST_TRUE(bQueryPartiesSuccess);
	AB_TEST_EQUAL(Response.Data.Num(), 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyGetMyParties, "AccelByte.Tests.Party.J.GetMyParties", AutomationFlagMaskParty);
bool PartyGetMyParties::RunTest(const FString& Parameters)
{
	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);

	bool bGetPartiesSuccess = false;

	FAccelByteModelsV2PaginatedPartyQueryResult Response;
	InviteeSession.GetMyParties(THandler<FAccelByteModelsV2PaginatedPartyQueryResult>::CreateLambda([&bGetPartiesSuccess, &Response](FAccelByteModelsV2PaginatedPartyQueryResult const QueryResponse)
	{
		bGetPartiesSuccess = true;
		Response = QueryResponse;
	}), PartyErrorHandler, EAccelByteV2SessionMemberStatus::ACTIVE);
	WaitUntil(bGetPartiesSuccess, "Waiting for parties get...");

	AB_TEST_TRUE(bGetPartiesSuccess);
	AB_TEST_TRUE(Response.Data.Num() > 0);
	for(auto& PartyResponse : Response.Data)
	{
		AB_TEST_TRUE(TestPartyMembership(PartyResponse, InviteeUserID));
	}

	return true;
}

// This will trigger a party delete
IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyCleanup, "AccelByte.Tests.Party.Z.Cleanup", AutomationFlagMaskParty);
bool PartyCleanup::RunTest(const FString& Parameters)
{
	bool bLeaveSuccess = false;
	FRegistry::Session.LeaveParty(Party.ID, FVoidHandler::CreateLambda([&bLeaveSuccess]
	{
		bLeaveSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bLeaveSuccess, "Waiting for party leave...");

	AB_TEST_TRUE(bLeaveSuccess);

	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);

	bLeaveSuccess = false;
	InviteeSession.LeaveParty(Party.ID, FVoidHandler::CreateLambda([&bLeaveSuccess]
	{
		bLeaveSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bLeaveSuccess, "Waiting for party leave...");

	AB_TEST_TRUE(bLeaveSuccess);

	return true;
}
