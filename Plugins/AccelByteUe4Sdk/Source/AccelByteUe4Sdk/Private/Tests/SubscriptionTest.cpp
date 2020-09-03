// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
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

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteSubscriptionTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteSubscriptionTest);

const int32 AutomationFlagMaskSubscription = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

const auto SubscriptionErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteSubscriptionTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});


IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedAppId, "AccelByte.Tests.Subscription.CheckUserEligibleOwnedAppId", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedAppId::RunTest(const FString& Parameters)
{
	FString oriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = "abshooterid123123";

	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername("fauzan+canada5@accelbyte.net", "fauzan123$", FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = false;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result) 
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	check(bCheckEligibleResult);
	FRegistry::Settings.AppId = oriAppId;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedSku, "AccelByte.Tests.Subscription.CheckUserEligibleOwnedSku", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedSku::RunTest(const FString& Parameters)
{
	FString oriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = "abshooterid123123";

	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername("fauzan+canada3@accelbyte.net", "fauzan123$", FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = false;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	check(bCheckEligibleResult);
	FRegistry::Settings.AppId = oriAppId;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedNone, "AccelByte.Tests.Subscription.CheckUserEligibleOwnedNone", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedNone::RunTest(const FString& Parameters)
{
	FString oriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = "abshooterid123123";

	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername("fauzan+canada4@accelbyte.net", "fauzan123$", FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = true;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	check(!bCheckEligibleResult);
	FRegistry::Settings.AppId = oriAppId;
	return true;
}

