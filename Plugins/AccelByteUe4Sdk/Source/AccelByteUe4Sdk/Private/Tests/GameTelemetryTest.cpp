// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::GameTelemetry;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteGameTelemetryTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteGameTelemetryTest);

const int32 AutomationFlagMaskGameTelemetry = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

const auto GameTelemetryErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteGameTelemetryTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameTelemetryTestSendProtectedEvent, "AccelByte.Tests.GameTelemetry.Send_BatchTelemetryEvent_ReturnsOK", AutomationFlagMaskGameTelemetry);
bool GameTelemetryTestSendProtectedEvent::RunTest(const FString& Parameters)
{
	bool bLoginSuccessful = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]() { bLoginSuccessful = true; }), GameTelemetryErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginSuccessful, "LoginWithDeviceId");
	
	FRegistry::GameTelemetry.SetBatchFrequency(FTimespan::FromSeconds(5.0f));
	FRegistry::GameTelemetry.SetImmediateEventList({});
	const int EVENT_COUNT = 999;
	int SuccessResultCount = 0;
	bool allEventDone = false;
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

		bool bTelemetryEventSent = false;

		FRegistry::GameTelemetry.Send(
			TelemetryBody,
			FVoidHandler::CreateLambda([&]()
				{
					SuccessResultCount++;
					if (SuccessResultCount == EVENT_COUNT)
					{
						allEventDone = true;
					}
				}), GameTelemetryErrorHandler);
		FlushHttpRequests();
	}

	WaitUntil([&allEventDone]()
	{
		if (!allEventDone)
		{
			FlushHttpRequests();
		}
		return allEventDone;
	}, 100, "Sending batch telemetry event");

	FRegistry::User.ForgetAllCredentials();

	check(SuccessResultCount == EVENT_COUNT);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameTelemetryTestSendMultipleProtectedEvents, "AccelByte.Tests.GameTelemetry.Send_ImmediateEvent_ReturnsOK", AutomationFlagMaskGameTelemetry);
bool GameTelemetryTestSendMultipleProtectedEvents::RunTest(const FString& Parameters)
{
	bool bLoginSuccessful = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]() { bLoginSuccessful = true; }), GameTelemetryErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginSuccessful, "LoginWithDeviceId");

	FString CurrentImmediateEventName = "SDK_UE4_Immediate_Event";
	FRegistry::GameTelemetry.SetBatchFrequency(FTimespan::FromSeconds(5.0f));
	FRegistry::GameTelemetry.SetImmediateEventList({ CurrentImmediateEventName });
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

		bool bTelemetryEventSent = false;

		FRegistry::GameTelemetry.Send(
			TelemetryBody,
			FVoidHandler::CreateLambda([&]()
				{
					SuccessResultCount++;
					if (SuccessResultCount == EVENT_COUNT)
					{
						allEventDone = true;
					}
				}), GameTelemetryErrorHandler);
		FlushHttpRequests();
	}

	Waiting(allEventDone, "Sending multiple immediate telemetry events");

	FRegistry::User.ForgetAllCredentials();

	check(SuccessResultCount == EVENT_COUNT);

	return true;
}
