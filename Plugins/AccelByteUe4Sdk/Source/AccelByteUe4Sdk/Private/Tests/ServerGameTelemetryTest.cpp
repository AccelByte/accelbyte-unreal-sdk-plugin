// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteServerCredentials.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::ServerCredentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::GameServerApi::ServerGameTelemetry;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerGameTelemetryTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerGameTelemetryTest);

const int32 AutomationFlagMaskServerGameTelemetry = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

const auto ServerTelemetryErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteServerGameTelemetryTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Client Login...");
	
	FRegistry::ServerGameTelemetry.SetBatchFrequency(FTimespan::FromSeconds(5.0f));
	FRegistry::ServerGameTelemetry.SetImmediateEventList({});
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
		TelemetryBody.EventNamespace = "SDK Test UE4";
		TelemetryBody.Payload = MakeShared<FJsonObject>(Payload);

		bool bTelemetryEventSent = false;
	
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

	Waiting(allEventDone, "Sending batch telemetry event");

	FRegistry::ServerCredentials.ForgetAll();
	
	check(SuccessResultCount == EVENT_COUNT);

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
	FlushHttpRequests();
	Waiting(bLoginSuccessful, "Waiting for Client Login...");

	FString CurrentImmediateEventName = "SDK UE4 Immediate Event";
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
		TelemetryBody.EventNamespace = "SDK Test UE4";
		TelemetryBody.Payload = MakeShared<FJsonObject>(Payload);

		bool bTelemetryEventSent = false;

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

	check(SuccessResultCount == EVENT_COUNT);

	return true;
}
