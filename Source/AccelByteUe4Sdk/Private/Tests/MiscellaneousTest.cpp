// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteMiscellaneousApi.h"
#include "Core/AccelByteRegistry.h"
#include "TestUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteMiscellaneousTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteMiscellaneousTest);

const int32 AutomationFlagMaskMiscellaneous = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto MiscellaneousTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteMiscellaneousTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetServerCurrentTime, "AccelByte.Tests.Miscellaneous.A.GetServerCurrentTime", AutomationFlagMaskMiscellaneous);
bool GetServerCurrentTime::RunTest(const FString& Parameters)
{
	bool bGetServerCurrentTimeSuccess = false;
	FTime CurrentServerTime;
	FRegistry::Miscellaneous.GetServerCurrentTime(THandler<FTime>::CreateLambda([&bGetServerCurrentTimeSuccess, &CurrentServerTime](const FTime& Result)
	{
		CurrentServerTime = Result;
		bGetServerCurrentTimeSuccess = true;
	}), MiscellaneousTestErrorHandler);
	WaitUntil(bGetServerCurrentTimeSuccess, "Waiting for Server Current Time...");
	AB_TEST_TRUE(bGetServerCurrentTimeSuccess);
	return true;
}