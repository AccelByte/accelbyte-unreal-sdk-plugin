// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteSessionApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"
#include "Api/AccelByteQos.h"
#include "Net/VoiceDataCommon.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using Api::User;
using Api::Session;

const int32 AutomationFlagMaskParty = EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteGameSessionTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteGameSessionTest);

const auto GameSessionErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteGameSessionTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

FAccelByteModelsV2GameSession GameSession;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionSetup, "AccelByte.Tests.GameSession.A.Setup", AutomationFlagMaskParty);
bool GameSessionSetup::RunTest(const FString& Parameters)
{
	bool bLoginDone = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]
	{
		bLoginDone = true;
	}), GameSessionErrorHandler);	
	WaitUntil(bLoginDone, "Waiting for LoginWithDeviceId...");

	AB_TEST_TRUE(bLoginDone);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionCreate, "AccelByte.Tests.GameSession.B.Create", AutomationFlagMaskParty);
bool GameSessionCreate::RunTest(const FString& Parameters)
{
	FAccelByteModelsV2GameSessionCreateRequest Request;

	// These should be the required fields for creation
	Request.JoinType = Api::SessionJoinType::Closed;
	Request.DSRequest.MinPlayer = 1;
	Request.DSRequest.RequestedRegions.Add({});
	Request.DSRequest.GameMode =
		Request.DSRequest.ServerName =
		Request.DSRequest.ClientVersion =
		Request.DSRequest.Deployment = "test";
	
	bool bSessionCreateSuccess = false;
	FAccelByteModelsV2GameSession GameSessionResponse;
	FRegistry::Session.CreateGameSession(Request, THandler<FAccelByteModelsV2GameSession>::CreateLambda([&bSessionCreateSuccess, &GameSessionResponse](FAccelByteModelsV2GameSession Response)
	{
		bSessionCreateSuccess = true;
		GameSessionResponse = Response;
	}), GameSessionErrorHandler);
	WaitUntil(bSessionCreateSuccess, "Waiting for game session creation...");

	GameSession = GameSessionResponse;
	
	AB_TEST_TRUE(bSessionCreateSuccess);
	AB_TEST_EQUAL(GameSession.JoinType, Request.JoinType);
	// TODO: Test other fields?
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionGetDetails, "AccelByte.Tests.GameSession.C.GetDetails", AutomationFlagMaskParty);
bool GameSessionGetDetails::RunTest(const FString& Parameters)
{	
	bool bSessionGetSuccess = false;
	FAccelByteModelsV2GameSession GameSessionResponse;
	FRegistry::Session.GetGameSessionDetails(GameSession.ID, THandler<FAccelByteModelsV2GameSession>::CreateLambda([&bSessionGetSuccess, &GameSessionResponse](FAccelByteModelsV2GameSession Response)
	{
		bSessionGetSuccess = true;
		GameSessionResponse = Response;
	}), GameSessionErrorHandler);
	WaitUntil(bSessionGetSuccess, "Waiting for game session get details...");
	
	AB_TEST_TRUE(bSessionGetSuccess);
	AB_TEST_EQUAL(GameSessionResponse.ID, GameSession.ID);
	// TODO: Test other fields?
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionUpdate, "AccelByte.Tests.GameSession.C.Update", AutomationFlagMaskParty);
bool GameSessionUpdate::RunTest(const FString& Parameters)
{
	FAccelByteModelsV2GameSessionUpdateRequest Request;

	// These should be the required fields for creation
	Request.JoinType = Api::SessionJoinType::Open;
	
	bool bSessionUpdateSuccess = false;
	FAccelByteModelsV2GameSession GameSessionResponse;
	FRegistry::Session.UpdateGameSession(GameSession.ID, Request, THandler<FAccelByteModelsV2GameSession>::CreateLambda([&bSessionUpdateSuccess, &GameSessionResponse](FAccelByteModelsV2GameSession Response)
	{
		bSessionUpdateSuccess = true;
		GameSessionResponse = Response;
	}), GameSessionErrorHandler);
	WaitUntil(bSessionUpdateSuccess, "Waiting for game session update...");

	GameSession = GameSessionResponse;
	
	AB_TEST_TRUE(bSessionUpdateSuccess);
	AB_TEST_EQUAL(GameSession.JoinType, Request.JoinType);
	// TODO: Test other fields?
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionJoin, "AccelByte.Tests.GameSession.D.Join", AutomationFlagMaskParty);
bool GameSessionJoin::RunTest(const FString& Parameters)
{
	bool bSessionJoinSuccess = false;
	FAccelByteModelsV2GameSession GameSessionResponse;
	FRegistry::Session.JoinGameSession(GameSession.ID, THandler<FAccelByteModelsV2GameSession>::CreateLambda([&bSessionJoinSuccess, &GameSessionResponse](FAccelByteModelsV2GameSession Response)
	{
		bSessionJoinSuccess = true;
		GameSessionResponse = Response;
	}), GameSessionErrorHandler);
	WaitUntil(bSessionJoinSuccess, "Waiting for game session join...");

	AB_TEST_TRUE(bSessionJoinSuccess);

	GameSession = GameSessionResponse;

	bool bMemberFound = false;
	for(auto& Member : GameSession.Members)
	{
		if(Member.ID.Equals(FRegistry::Credentials.GetUserId()))
		{
			bMemberFound = true;
			break;
		}
	}

	AB_TEST_TRUE(bMemberFound);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionQuery, "AccelByte.Tests.GameSession.D.Query", AutomationFlagMaskParty);
bool GameSessionQuery::RunTest(const FString& Parameters)
{
	FAccelByteModelsV2SessionQueryRequest Request;
	Request.MemberID = FRegistry::Credentials.GetUserId();
	
	bool bSessionQuerySuccess = false;
	FAccelByteModelsV2PaginatedGameSessionQueryResult GameSessions;
	FRegistry::Session.QueryGameSessions(Request, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult>::CreateLambda([&bSessionQuerySuccess, &GameSessions](FAccelByteModelsV2PaginatedGameSessionQueryResult Response)
	{
		bSessionQuerySuccess = true;
		GameSessions = Response;
	}), GameSessionErrorHandler);
	WaitUntil(bSessionQuerySuccess, "Waiting for game session query...");

	AB_TEST_TRUE(bSessionQuerySuccess);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameSessionDelete, "AccelByte.Tests.GameSession.Z.Delete", AutomationFlagMaskParty);
bool GameSessionDelete::RunTest(const FString& Parameters)
{	
	bool bSessionDeleteSuccess = false;
	FRegistry::Session.DeleteGameSession(GameSession.ID, FVoidHandler::CreateLambda([&bSessionDeleteSuccess]
	{
		bSessionDeleteSuccess = true;
	}), GameSessionErrorHandler);
	WaitUntil(bSessionDeleteSuccess, "Waiting for game session delete...");
	
	AB_TEST_TRUE(bSessionDeleteSuccess);
	
	return true;
}
