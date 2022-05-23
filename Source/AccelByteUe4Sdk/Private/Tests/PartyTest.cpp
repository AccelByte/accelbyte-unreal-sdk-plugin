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
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyCreate, "AccelByte.Tests.Party.B.Create", AutomationFlagMaskParty);
bool PartyCreate::RunTest(const FString& Parameters)
{
	bool bCreatePartySuccess = false;
	FAccelByteModelsPartyCreateRequest Request;
	Request.JoinType = FString(TEXT("open"));
	FAccelByteModelsBaseSession Response;
	FRegistry::Session.CreateParty(Request, THandler<FAccelByteModelsBaseSession>::CreateLambda([&bCreatePartySuccess, &Response](FAccelByteModelsBaseSession const Party)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Create party success"));
		bCreatePartySuccess = true;
		Response = Party;
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
	FAccelByteModelsBaseSession Response;
	FRegistry::Session.GetPartyDetails(PartyID, THandler<FAccelByteModelsBaseSession>::CreateLambda([&bGetDetailsSuccess, &Response](FAccelByteModelsBaseSession const Party)
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
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone] { bLoginDone = true; }), PartyErrorHandler);	
	WaitUntil(bLoginDone, "Waiting for LoginWithDeviceId...");
	
	Api::Lobby& Lobby = FRegistry::Lobby;
	Lobby.Connect();
	WaitUntil([&Lobby] { return Lobby.IsConnected(); }, "", 5);

	const FString UserID = FRegistry::Credentials.GetUserId();

	bool bGetInviteSuccess = false;
	FAccelByteModelsPartyGetInvitedNotice InvitedToPartyResponse;
	const auto InvitedToPartyDelegate = Api::Lobby::FPartyGetInvitedNotif::CreateLambda([&InvitedToPartyResponse, &bGetInviteSuccess](FAccelByteModelsPartyGetInvitedNotice result)
	{
		InvitedToPartyResponse = result;
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Got party invitation!"));
		if (!result.PartyId.IsEmpty())
		{
			bGetInviteSuccess = true;
		}
	});

	Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	// TODO: This may fail due to the user sending the invite being in the party already?
	bool bSendInviteSuccess = false;
	FRegistry::Session.SendPartyInvite(PartyID, UserID, FVoidHandler::CreateLambda([&bSendInviteSuccess]
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Send party invite success"));
		bSendInviteSuccess = true;
	}), PartyErrorHandler);
	WaitUntil(bSendInviteSuccess, "Waiting for party invite send...");

	WaitUntil(bGetInviteSuccess, "Waiting for party invite notif...");

	AB_TEST_TRUE(InvitedToPartyResponse.PartyId.Equals(PartyID));
	
	bool bJoinPartySuccess = false;
	FAccelByteModelsBaseSession JoinedParty;
	FRegistry::Session.JoinParty(PartyID, THandler<FAccelByteModelsBaseSession>::CreateLambda([&bJoinPartySuccess, &JoinedParty](FAccelByteModelsBaseSession const Party)
	{
		UE_LOG(LogAccelBytePartyTest, Log, TEXT("Join party success"));
		bJoinPartySuccess = true;
		JoinedParty = Party;
	}), PartyErrorHandler);

	// Ensure that the new user is an active party member for the joined party
	bool bInParty = false;
	for(auto& Member : JoinedParty.Members)
	{
		if(Member.ID.Equals(UserID))
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
