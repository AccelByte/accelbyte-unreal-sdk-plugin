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

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using Api::User;
using Api::Session;

FString PartyID;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelBytePartyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelBytePartyTest);

const int32 AutomationFlagMaskParty = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext;

const auto PartyErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelBytePartyTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySetup, "AccelByte.Tests.Party.A.Setup", AutomationFlagMaskParty);
bool PartySetup::RunTest(const FString& Parameters)
{
    bool bLoginDone = false;
    FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone] { bLoginDone = true; }), PartyErrorHandler);	
    WaitUntil(bLoginDone, "Waiting for LoginWithDeviceId...");

	AB_TEST_TRUE(bLoginDone);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyCreate, "AccelByte.Tests.Party.B.Create", AutomationFlagMaskParty);
bool PartyCreate::RunTest(const FString& Parameters)
{
	bool bCreatePartySuccess = false;
	FAccelByteModelsV2PartyCreateRequest Request;
	Request.JoinType = FString(TEXT("open"));
	FAccelByteModelsV2PartySession Response;
	FRegistry::Session.CreateParty(Request, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bCreatePartySuccess, &Response](FAccelByteModelsV2PartySession const Party)
	{
		bCreatePartySuccess = true;
		Response = Party;
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Create party success"));
	}), PartyErrorHandler);
	WaitUntil(bCreatePartySuccess, "Waiting for party creation...");

	AB_TEST_TRUE(bCreatePartySuccess);
	AB_TEST_EQUAL(Response.JoinType, Request.JoinType);
	// TODO: Test other fields

	PartyID = Response.ID;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyGetPartyDetails, "AccelByte.Tests.Party.C.GetPartyDetails", AutomationFlagMaskParty);
bool PartyGetPartyDetails::RunTest(const FString& Parameters)
{
	bool bGetDetailsSuccess = false;
	FAccelByteModelsV2PartySession Response;
	FRegistry::Session.GetPartyDetails(PartyID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bGetDetailsSuccess, &Response](FAccelByteModelsV2PartySession const Party)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Get party details success"));
		bGetDetailsSuccess = true;
		Response = Party;
	}), PartyErrorHandler);
	WaitUntil(bGetDetailsSuccess, "Waiting for get party details...");
	
	AB_TEST_TRUE(bGetDetailsSuccess);
	AB_TEST_EQUAL(Response.ID, PartyID);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySendInvite, "AccelByte.Tests.Party.C.SendInvite", AutomationFlagMaskParty);
bool PartySendInvite::RunTest(const FString& Parameters)
{
	Credentials InviteeCredentials;
	User Invitee(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	FString InviteeUserID;
	
	// Create test user and log in
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
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
			UE_LOG(LogAccelBytePartyTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
			bRegisterDone = true;
		}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for registration...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelBytePartyTest, Log, TEXT("Log in test user..."));
	Invitee.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
	{
		InviteeUserID = InviteeCredentials.GetUserId();
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("    Success"));
		bLoginSuccessful = true;
	}),
	FCustomErrorHandler::CreateLambda([](int Code, const FString& Message, const FJsonObject&)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
	}));

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for login...");
	
	Api::Lobby Lobby(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
    Lobby.Connect();
    WaitUntil([&Lobby] { return Lobby.IsConnected(); }, "", 5);

	FAccelByteModelsV2PartyUserInvitedEvent InvitePayload;
	
	// TODO: probably need to have a delegate for v2 party invite.  backend will need to be reworked to not use freeform notifs
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
	FRegistry::Session.SendPartyInvite(PartyID, InviteeUserID, FVoidHandler::CreateLambda([&bSendInviteSuccess]
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Send party invite success"));
		bSendInviteSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bGetInviteSuccess, "Waiting for party invite notif...", 30);
	
	AB_TEST_TRUE(InvitePayload.PartyID.Equals(PartyID));
	AB_TEST_TRUE(InvitePayload.SenderID.Equals(FRegistry::Credentials.GetUserId()));

	Session InviteeSession(InviteeCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	
	bool bJoinPartySuccess = false;
	FAccelByteModelsV2PartySession JoinedParty;
	InviteeSession.JoinParty(PartyID, THandler<FAccelByteModelsV2PartySession>::CreateLambda([&bJoinPartySuccess, &JoinedParty](FAccelByteModelsV2PartySession const Party)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Join party success"));
		bJoinPartySuccess = true;
		JoinedParty = Party;
	}), PartyErrorHandler);
	WaitUntil(bJoinPartySuccess, "Waiting for party join...");
	
	// Ensure that the new user is an active party member for the joined party
	bool bInParty = false;
	for(auto& Member : JoinedParty.Members)
	{
		if(Member.ID.Equals(InviteeUserID))
		{
			AB_TEST_TRUE(Member.Status.Equals(FString(TEXT("active"))));
			bInParty = true;
			break;
		}
	}

	AB_TEST_TRUE(bSendInviteSuccess);
	AB_TEST_TRUE(bGetInviteSuccess);
	AB_TEST_TRUE(bJoinPartySuccess);
	AB_TEST_TRUE(bInParty);
	
	return true;
}
