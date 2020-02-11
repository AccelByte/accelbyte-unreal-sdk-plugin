// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Api/AccelByteQos.h"
#include "Core/AccelByteRegistry.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteQosTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteQosTest);

const int32 AutomationFlagMaskQos = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto QosTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteQosTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QosGetServerLatencies, "AccelByte.Tests.Qos.A.GetServerLatencies", AutomationFlagMaskQos);
bool QosGetServerLatencies::RunTest(const FString& Parameters)
{
	bool bGetServerLatenciesSuccess = false;
	TArray<TPair<FString, float>> Latencies;
	FRegistry::Qos.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda([&bGetServerLatenciesSuccess, &Latencies](const TArray<TPair<FString, float>>& Result)
	{
		UE_LOG(LogAccelByteQosTest, Log, TEXT("Get Server Latencies Success! Count: %d"), Result.Num());
		bGetServerLatenciesSuccess = true;
		Latencies = Result;
	}), QosTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetServerLatenciesSuccess, "Waiting for get server latencies...");
	check(bGetServerLatenciesSuccess);
	check(Latencies.Num() > 0);
	return true;
}
