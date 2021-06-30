// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Core/AccelByteMultiRegistry.h"
#include "TestUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteMultiRegistryTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteMultiRegistryTest);

static const int32 AutomationFlagMaskMultiRegistry = (
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::CommandletContext |
	EAutomationTestFlags::ClientContext);

static const auto MultiRegistryTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteMultiRegistryTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	});

static TArray<TSharedPtr<FTestUser>> TestUsers;
static TArray<TSharedPtr<Credentials>> TestCredentials;
static const FString TestUID = TEXT("25479f26"); // Arbitrary UID for this test

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiRegistryTestLogin, "AccelByte.Tests.MultiRegistry.User", AutomationFlagMaskMultiRegistry);
bool FMultiRegistryTestLogin::RunTest(const FString& Parameters)
{
	//const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7)); // 21 years old
	//const FString DateOfBirthIso8601 = FString::Printf(TEXT("%04d-%02d-%02d"),
	//	DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	// Setup

	AB_TEST_TRUE(SetupTestUsers(TestUID, 2, TestUsers)); // Register test users only (no login)

	// Multi user login using FMultiRegistry and FApiClient

	for (const TSharedPtr<FTestUser> User : TestUsers)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteMultiRegistryTest, Log, TEXT("%s: %s"), TEXT("Logging in"), *User->Email);
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(User->Email); // Using email as key
		ApiClient->User.LoginWithUsername(User->Email, User->Password,
			FVoidHandler::CreateLambda([&]()
				{
					UE_LOG(LogAccelByteMultiRegistryTest, Log, TEXT("OK"));
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
				{
					MultiRegistryTestErrorHandler.ExecuteIfBound(ErrorCode, ErrorMessage);
					bIsDone = true;
				}));
		Waiting(bIsDone, "Waiting ...");
		AB_TEST_TRUE(bIsOk);
	}

	// Check and load multi user credentials to an array (for testing purposes only)

	TestCredentials.Empty();

	TSet<FString> UniqueUserIds;

	for (const TSharedPtr<FTestUser> User : TestUsers)
	{
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(User->Email);
		FString UserId = ApiClient->Credentials.GetUserId();
		AB_TEST_TRUE(!UserId.IsEmpty());
		UniqueUserIds.Add(UserId);
		TestCredentials.Add(MakeShared<Credentials>(ApiClient->Credentials)); 
	}

	AB_TEST_EQUAL(TestUsers.Num(), UniqueUserIds.Num());
	AB_TEST_EQUAL(TestUsers.Num(), TestCredentials.Num());

	// Multi user API GetData using FMultiRegistry and FApiClient

	for (int i = 0; i < TestUsers.Num(); i++)
	{
		FAccountUserData AccountUserData;
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteMultiRegistryTest, Log, TEXT("%s: %s"), TEXT("Getting user data"), *TestUsers[i]->Email);
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(TestUsers[i]->Email);
		ApiClient->User.GetData(
			THandler<FAccountUserData>::CreateLambda([&](const FAccountUserData& Result)
				{
					UE_LOG(LogAccelByteMultiRegistryTest, Log, TEXT("OK"));
					AccountUserData = Result;
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
				{
					MultiRegistryTestErrorHandler.ExecuteIfBound(ErrorCode, ErrorMessage);
					bIsDone = true;
				}));
		Waiting(bIsDone, TEXT("Waiting ..."));
		AB_TEST_TRUE(bIsOk);
		AB_TEST_EQUAL(AccountUserData.UserId, TestCredentials[i]->GetUserId());
	}

	// Tear down

	AB_TEST_TRUE(TearDownTestUsers(TestCredentials));

	return true;
}
