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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerGameTelemetryTestSendProtectedEvent, "AccelByte.Tests.ServerGameTelemetry.SendProtectedEvent", AutomationFlagMaskServerGameTelemetry);
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
	
	FJsonObject Payload;
	Payload.SetStringField("someString", "someString");
	Payload.SetNumberField("someInt", 7.77);
	Payload.SetBoolField("someBool", true);

	bool bTelemetryEventSent = false;

	FRegistry::ServerGameTelemetry.SendProtectedEvent(
		"someeventname", 
		std::move(Payload),
		FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteServerGameTelemetryTest, Log, TEXT("Single protected event sent"));
			bTelemetryEventSent = true;
		}), 
		ServerTelemetryErrorHandler);;

	Waiting(bTelemetryEventSent, "Sending single protected event");

	FRegistry::ServerCredentials.ForgetAll();
	
	check(bTelemetryEventSent);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerGameTelemetryTestSendMultipleProtectedEvents, "AccelByte.Tests.ServerGameTelemetry.SendMultipleProtectedEvents", AutomationFlagMaskServerGameTelemetry);
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
	FRegistry::ServerGameTelemetry.SendProtectedEvents(
		std::move(Events),
		FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteServerGameTelemetryTest, Log, TEXT("Multiple protected events sent"));
			bTelemetryEventSent = true;
		}),
		ServerTelemetryErrorHandler);;

	Waiting(bTelemetryEventSent, "Sending multiple protected events");

	FRegistry::ServerCredentials.ForgetAll();

	check(bTelemetryEventSent);

	return true;
}
