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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameTelemetryTestSendProtectedEvent, "AccelByte.Tests.GameTelemetry.SendProtectedEvent", AutomationFlagMaskGameTelemetry);
bool GameTelemetryTestSendProtectedEvent::RunTest(const FString& Parameters)
{
	bool bLoginSuccessful = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]() { bLoginSuccessful = true; }), GameTelemetryErrorHandler);
	
	Waiting(bLoginSuccessful, "LoginWithDeviceId");
	
	FJsonObject Payload;
	Payload.SetStringField("someString", "someString");
	Payload.SetNumberField("someInt", 7.77);
	Payload.SetBoolField("someBool", true);

	bool bTelemetryEventSent = false;

	FRegistry::GameTelemetry.SendProtectedEvent(
		"someeventname", 
		std::move(Payload),
		FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteGameTelemetryTest, Log, TEXT("Single protected event sent"));
			bTelemetryEventSent = true;
		}), 
		GameTelemetryErrorHandler);;

	Waiting(bTelemetryEventSent, "Sending single protected event");

	FRegistry::User.ForgetAllCredentials();
	
	check(bTelemetryEventSent);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameTelemetryTestSendMultipleProtectedEvents, "AccelByte.Tests.GameTelemetry.SendMultipleProtectedEvents", AutomationFlagMaskGameTelemetry);
bool GameTelemetryTestSendMultipleProtectedEvents::RunTest(const FString& Parameters)
{
	bool bLoginSuccessful = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]() { bLoginSuccessful = true; }), GameTelemetryErrorHandler);

	Waiting(bLoginSuccessful, "LoginWithDeviceId");

	TMap<FString, TSharedPtr<FJsonObject>> Events;
	auto Payload1 = MakeShared<FJsonObject>();
	Payload1->SetStringField("someString", "someString");
	Payload1->SetNumberField("someInt", 7.77);
	Payload1->SetBoolField("someBool", true);

	Events.Add("someeventname", Payload1);

	auto Payload2 = MakeShared<FJsonObject>();
	Payload2->SetStringField("anotherString", "anotherString");
	Payload2->SetBoolField("someBool", true);

	Events.Add("othereventname", Payload1);

	bool bTelemetryEventSent = false;
	FRegistry::GameTelemetry.SendProtectedEvents(
		std::move(Events),
		FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteGameTelemetryTest, Log, TEXT("Multiple protected events sent"));
			bTelemetryEventSent = true;
		}),
		GameTelemetryErrorHandler);;

	Waiting(bTelemetryEventSent, "Sending multiple protected events");

	FRegistry::User.ForgetAllCredentials();

	check(bTelemetryEventSent);

	return true;
}
