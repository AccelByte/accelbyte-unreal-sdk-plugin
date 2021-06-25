// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteServerCredentials.h"
#include "TestUtilities.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::ServerCredentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::GameServerApi::ServerGameTelemetry;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerGameTelemetryTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerGameTelemetryTest);

const int32 AutomationFlagMaskServerGameTelemetry = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto ServerTelemetryErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteServerGameTelemetryTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerGameTelemetryTestSendProtectedEvent, "AccelByte.Tests.ServerGameTelemetry.Send_BatchTelemetryEvent_ReturnsOK", AutomationFlagMaskServerGameTelemetry);
bool ServerGameTelemetryTestSendProtectedEvent::RunTest(const FString& Parameters)
{
	bool bLoginSuccessful = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccessful]()
		{
			bLoginSuccessful = true;
			UE_LOG(LogAccelByteServerGameTelemetryTest, Log, TEXT("\t\tClient Successfully Login."));
		}), ServerTelemetryErrorHandler);
	Waiting(bLoginSuccessful, "Waiting for Client Login...");
	
	FRegistry::ServerGameTelemetry.SetBatchFrequency(FTimespan::FromSeconds(5.0f));
	FRegistry::ServerGameTelemetry.SetImmediateEventList({});
	const int EVENT_COUNT = 10;
	int SuccessResultCount = 0;
	
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		FJsonObject Payload;
		Payload.SetStringField("someString", "someString");
		Payload.SetNumberField("someInt", i);
		Payload.SetBoolField("someBool", true);
		FAccelByteModelsTelemetryBody TelemetryBody;
		TelemetryBody.EventName = "ServerGameTelemetry.Send_BatchTelemetryEvent_ReturnsOK";
		TelemetryBody.EventNamespace = "SDKTestUE4";
		TelemetryBody.Payload = MakeShared<FJsonObject>(Payload);

		FRegistry::ServerGameTelemetry.Send(
			TelemetryBody,
			FVoidHandler::CreateLambda([&SuccessResultCount]()
			{
				SuccessResultCount++;
				UE_LOG(LogAccelByteServerGameTelemetryTest, Log, TEXT("Received %d result count"), SuccessResultCount);
			}), ServerTelemetryErrorHandler);
	}

	
	WaitUntil([&SuccessResultCount, EVENT_COUNT]() {return SuccessResultCount == EVENT_COUNT; }, 60, "batch telemetry event sent successfully");

	FRegistry::ServerCredentials.ForgetAll();
	
	AB_TEST_EQUAL(SuccessResultCount, EVENT_COUNT);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerGameTelemetryTestSendMultipleProtectedEvents, "AccelByte.Tests.ServerGameTelemetry.Send_ImmediateEvent_ReturnsOK", AutomationFlagMaskServerGameTelemetry);
bool ServerGameTelemetryTestSendMultipleProtectedEvents::RunTest(const FString& Parameters)
{
	bool bLoginSuccessful = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccessful]()
		{
			bLoginSuccessful = true;
			UE_LOG(LogAccelByteServerGameTelemetryTest, Log, TEXT("\t\tClient Successfully Login."));
		}), ServerTelemetryErrorHandler);
	Waiting(bLoginSuccessful, "Waiting for Client Login...");

	FString CurrentImmediateEventName = "SDK_UE4_Immediate_Event";
	FRegistry::ServerGameTelemetry.SetBatchFrequency(FTimespan::FromSeconds(5.0f));
	FRegistry::ServerGameTelemetry.SetImmediateEventList({ CurrentImmediateEventName });
	const int EVENT_COUNT = 3;
	int SuccessResultCount = 0;
	bool allEventDone = false;
	for (int i = 0; i < EVENT_COUNT; i++)
	{
		FJsonObject Payload;
		Payload.SetStringField("someString", "someString");
		Payload.SetNumberField("someInt", i);
		Payload.SetBoolField("someBool", true);
		FAccelByteModelsTelemetryBody TelemetryBody;
		TelemetryBody.EventName = CurrentImmediateEventName;
		TelemetryBody.EventNamespace = "SDKTestUE4";
		TelemetryBody.Payload = MakeShared<FJsonObject>(Payload);

		FRegistry::ServerGameTelemetry.Send(
			TelemetryBody,
			FVoidHandler::CreateLambda([&]()
				{
					SuccessResultCount++;
					if (SuccessResultCount == EVENT_COUNT)
					{
						allEventDone = true;
					}
				}), ServerTelemetryErrorHandler);
	}

	Waiting(allEventDone, "Sending multiple immediate telemetry events");

	FRegistry::ServerCredentials.ForgetAll();

	AB_TEST_EQUAL(SuccessResultCount, EVENT_COUNT);

	return true;
}
