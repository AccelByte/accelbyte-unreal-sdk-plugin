// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Core/AccelByteMultiRegistry.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteMultiRegistryTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteMultiRegistryTest);

const int32 AutomationFlagMaskMultiRegistry = (	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter |	EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto MultiRegistryTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteMultiRegistryTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

BEGIN_DEFINE_SPEC(FMultiRegistryTest, "AccelByte.Tests.MultiRegistry", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	TArray<FTestUser> MultiRegistryTestUsers;
	TArray<FCredentialsPtr> MultiRegistryTestCredentials;
END_DEFINE_SPEC(FMultiRegistryTest)

void FMultiRegistryTest::Define()
{
	const auto setupOnce = [this]()
	{
		AB_TEST_TRUE(SetupTestUsers(2, MultiRegistryTestUsers));
		
		return true;
	};

	BeforeEach(setupOnce);

	const auto teardown = [this]()
	{		
		AB_TEST_TRUE(TeardownTestUsers(MultiRegistryTestUsers));

		return true;
	};

	AfterEach(teardown);

	Describe("User", [this]()
	{
		It("Should perform multi user login using FMultiRegistry and FApiClient then return true", [this]()
		{
			// Multi user login
			for (const auto& User : MultiRegistryTestUsers)
			{
				bool bIsDone = false;
				bool bIsOk = false;
				UE_LOG(LogAccelByteMultiRegistryTest, Log, TEXT("%s: %s"), TEXT("Logging in"), *User.Email);
				FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(User.Email); // Using email as key
				ApiClient->User.LoginWithUsername(User.Email, User.Password,
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
				WaitUntil(bIsDone, "Waiting ...");
				AB_TEST_TRUE(bIsOk);
			}

			// Check and load multi user credentials to an array (for testing purposes only)

			MultiRegistryTestCredentials.Empty();

			TSet<FString> UniqueUserIds;

			for (const auto& User : MultiRegistryTestUsers)
			{
				FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(User.Email);
				FString UserId = ApiClient->CredentialsRef->GetUserId();
				AB_TEST_TRUE(!UserId.IsEmpty());
				UniqueUserIds.Add(UserId);
				MultiRegistryTestCredentials.Add(ApiClient->CredentialsRef); 
			}

			AB_TEST_EQUAL(MultiRegistryTestUsers.Num(), UniqueUserIds.Num());
			AB_TEST_EQUAL(MultiRegistryTestUsers.Num(), MultiRegistryTestCredentials.Num());

			// Multi user API GetData using FMultiRegistry and FApiClient

			for (int i = 0; i < MultiRegistryTestUsers.Num(); i++)
			{
				FAccountUserData AccountUserData;
				bool bIsDone = false;
				bool bIsOk = false;
				UE_LOG(LogAccelByteMultiRegistryTest, Log, TEXT("%s: %s"), TEXT("Getting user data"), *MultiRegistryTestUsers[i].Email);
				FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(MultiRegistryTestUsers[i].Email);
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
				WaitUntil(bIsDone, TEXT("Waiting ..."));
				AB_TEST_TRUE(bIsOk);
				AB_TEST_EQUAL(AccountUserData.UserId, MultiRegistryTestCredentials[i]->GetUserId());
			}
			
			return true;
		});
	});
}