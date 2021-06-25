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

DECLARE_LOG_CATEGORY_EXTERN(AccelByteMultiRegistryTestLog, Log, All);
DEFINE_LOG_CATEGORY(AccelByteMultiRegistryTestLog);

const int32 AccelByteMultiRegistryTestAutomationFlagMask = (
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::CommandletContext |
	EAutomationTestFlags::ClientContext);

const auto AccelByteMultiRegistryTestFatalErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MultiRegistry, "AccelByte.Tests.MultiRegistry.Statistic", AccelByteMultiRegistryTestAutomationFlagMask);
bool MultiRegistry::RunTest(const FString& Parameters)
{
	const FString StatisticStatCode = "sdktest";

	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7)); // 21 years old
	const FString DateOfBirthIso8601 = FString::Printf(TEXT("%04d-%02d-%02d"),
		DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	struct TestUser
	{
		FString Email;
		FString Password;
		FString DisplayName;
		FString Country;
		FString DateOfBirth;
	};

	const TestUser Users[] = {
		{
			TEXT("justice-ue4-sdk-multiregistrytest-user1@example.com"), // XXX Lower case
			TEXT("Password123"),
			TEXT("Justice UE4 SDK MultiRegistryTest User 1"),
			TEXT("US"),
			DateOfBirthIso8601
		},
		{
			TEXT("justice-ue4-sdk-multiregistrytest-user2@example.com"), // XXX Lower case
			TEXT("Password123"),
			TEXT("Justice UE4 SDK MultiRegistryTest User 2"),
			TEXT("US"),
			DateOfBirthIso8601
		}
	};

	TArray<FString> StatCodes;

	StatCodes.Add(StatisticStatCode);

	FStatCreateRequest CreateStatRequest;

	CreateStatRequest.defaultValue = 0;
	CreateStatRequest.description = "StatCode for SDK Test purpose";
	CreateStatRequest.incrementOnly = true;
	CreateStatRequest.maximum = 999999;
	CreateStatRequest.minimum = 0;
	CreateStatRequest.name = StatisticStatCode;
	CreateStatRequest.setAsGlobal = true;
	CreateStatRequest.setBy = EAccelByteStatisticSetBy::CLIENT;
	CreateStatRequest.statCode = StatisticStatCode;
	CreateStatRequest.tags = { StatisticStatCode };

#pragma region Lambda methods definition

	const auto Register = [](const TestUser& InUser, bool& OutIsRegisterDone, bool& OutIsUserExist)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Registering user ..."));
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(InUser.Email); // Using email as credentials key
		ApiClient->User.Register(InUser.Email, InUser.Password, InUser.DisplayName, InUser.Country, InUser.DateOfBirth,
			THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Response)
				{
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
					OutIsUserExist = true;
					OutIsRegisterDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
				{
					if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
					{
						UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("User is already created"));
						OutIsUserExist = true;
					}
					else
					{
						UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Cannot create user"));
					}
					OutIsRegisterDone = true;
				}));
		Waiting(OutIsRegisterDone, "Waiting ...");
	};

	const auto LoginUser = [](const TestUser& InUser, bool& OutIsUserLoginDone)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Logging in user ..."));
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(InUser.Email); // Using email as credentials key
		ApiClient->User.LoginWithUsername(InUser.Email, InUser.Password,
			FVoidHandler::CreateLambda([&]()
				{
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
					OutIsUserLoginDone = true;
				}), AccelByteMultiRegistryTestFatalErrorHandler);
		Waiting(OutIsUserLoginDone, "Waiting ...");
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("User creds: %s"), *ApiClient->Credentials.GetUserId());
	};

	const auto LoginClient = [](bool& OutIsClientLoginDone)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Logging in client ..."));
		FRegistry::ServerOauth2.LoginWithClientCredentials(
			FVoidHandler::CreateLambda([&]()
				{
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
					OutIsClientLoginDone = true;
				}), AccelByteMultiRegistryTestFatalErrorHandler);
		Waiting(OutIsClientLoginDone, "Waiting ...");
	};

	const auto CheckExistingStat = [](const FString& InStatisticStatCode, FAccelByteModelsStatInfo& OutGetStatResult, bool& OutIsCheckingStatDone, bool& OutIsStatExist)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Checking existing stat for testing ..."));
		Statistic_Get_Stat_By_StatCode(InStatisticStatCode,
			THandler<FAccelByteModelsStatInfo>::CreateLambda([&](const FAccelByteModelsStatInfo& Result)
				{
					OutGetStatResult = Result;
					OutIsStatExist = true;

					OutIsCheckingStatDone = true;
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
				}),
			FErrorHandler::CreateLambda([&OutIsCheckingStatDone](int32 ErrorCode, FString ErrorMessage)
				{
					if ((ErrorCodes)ErrorCode != ErrorCodes::StatisticConfigNotFoundException)
					{
						UE_LOG(AccelByteMultiRegistryTestLog, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					}
					else
					{
						UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					}
					OutIsCheckingStatDone = true;
				}));
		Waiting(OutIsCheckingStatDone, "Waiting ...");
	};

	const auto CreateStat = [](const FStatCreateRequest& InCreateStatRequest, FAccelByteModelsStatInfo& OutCreateStatResult, bool& OutIsCreateStatDone)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Creating stat for testing ..."));
		Statistic_Create_Stat(InCreateStatRequest,
			THandler<FAccelByteModelsStatInfo>::CreateLambda([&](FAccelByteModelsStatInfo Result)
				{
					OutCreateStatResult = Result;
					OutIsCreateStatDone = true;
				}),
			AccelByteMultiRegistryTestFatalErrorHandler);
		Waiting(OutIsCreateStatDone, "Waiting ...");
	};

	const auto CreateUserStatItems = [](const TestUser& InUser, const TArray<FString>& InStatCodes, bool& OutIsCreateUserStatItemsDone)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Creating user stat items ..."));
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(InUser.Email); // Using email as credentials key
		ApiClient->Statistic.CreateUserStatItems(InStatCodes,
			THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
				{
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
					OutIsCreateUserStatItemsDone = true;
				}),
			AccelByteMultiRegistryTestFatalErrorHandler);
		Waiting(OutIsCreateUserStatItemsDone, "Waiting ...");
	};

	const auto TryGetAllUserStatItems = [](const TestUser& InUser, FAccelByteModelsUserStatItemPagingSlicedResult& OutGetResult, bool& OutIsGetAllUserStatItemsDone)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Getting all user stat items ..."));
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(InUser.Email); // Using email as credentials key
		ApiClient->Statistic.GetAllUserStatItems(
			THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
				{
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
					OutGetResult = Result;
					OutIsGetAllUserStatItemsDone = true;

					//for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
					//{
					//	UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
					//}
				}),
			AccelByteMultiRegistryTestFatalErrorHandler);
		Waiting(OutIsGetAllUserStatItemsDone, "Waiting ...");
	};

	const auto DeleteUser = [](const TestUser& InUser, bool& OutIsDeleteUserDone)
	{
		UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("Deleting user ..."));
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(InUser.Email); // Using email as credentials key
		DeleteUserById(ApiClient->Credentials.GetUserId(),
			FSimpleDelegate::CreateLambda([&]()
				{
					UE_LOG(AccelByteMultiRegistryTestLog, Log, TEXT("OK"));
					OutIsDeleteUserDone = true;
				}),
			AccelByteMultiRegistryTestFatalErrorHandler);
		Waiting(OutIsDeleteUserDone, "Waiting ...");
	};

#pragma endregion

	// Register user

	for (const TestUser& User : Users)
	{
		bool bIsRegisterDone = false;
		bool bIsUserExists = false;

		Register(User, bIsRegisterDone, bIsUserExists);

		AB_TEST_TRUE(bIsRegisterDone);
		AB_TEST_TRUE(bIsUserExists);
	}

	// Login user

	for (const TestUser& User : Users)
	{
		bool bIsUserLoginDone = false;

		LoginUser(User, bIsUserLoginDone);

		AB_TEST_TRUE(bIsUserLoginDone);
	}

	// Check user credentials

	for (const TestUser& User : Users)
	{
		TSharedPtr<FApiClient> ApiClient = FMultiRegistry::GetApiClient(User.Email); // Using email as credentials key

		AB_TEST_FALSE(ApiClient->Credentials.GetUserId().IsEmpty());
	}

	// Login client

	for (const TestUser& User : Users)
	{
		bool bIsClientLoginDone = false;

		LoginClient(bIsClientLoginDone);

		AB_TEST_TRUE(bIsClientLoginDone);
	}

	// Make sure stat for testing is available

	for (const TestUser& User : Users)
	{
		// Check existing stat for testing

		bool bIsCheckingStatDone = false;
		bool bIsStatExist = false;
		FAccelByteModelsStatInfo GetStatResult;

		CheckExistingStat(StatisticStatCode, GetStatResult, bIsCheckingStatDone, bIsStatExist);

		AB_TEST_TRUE(bIsCheckingStatDone);

		// Create stat for testing if it does not exist

		if (!bIsStatExist)
		{
			bool IsCreateStatDone = false;
			FAccelByteModelsStatInfo CreateStatResult;

			CreateStat(CreateStatRequest, CreateStatResult, IsCreateStatDone);

			AB_TEST_TRUE(IsCreateStatDone);
		}
	}

	// Create user stat items

	for (const TestUser& User : Users)
	{
		bool bIsCreateUserStatItemsDone = false;

		CreateUserStatItems(User, StatCodes, bIsCreateUserStatItemsDone);

		AB_TEST_TRUE(bIsCreateUserStatItemsDone);
	}

	// Try get all user stat items

	for (const TestUser& User : Users)
	{
		bool bIsGetAllUserStatItemsDone = false;
		FAccelByteModelsUserStatItemPagingSlicedResult GetResult;

		TryGetAllUserStatItems(User, GetResult, bIsGetAllUserStatItemsDone);

		AB_TEST_TRUE(bIsGetAllUserStatItemsDone);
		AB_TEST_TRUE(GetResult.Data.Num() > 0);
	}

	// Delete user

	for (const TestUser& User : Users)
	{
		bool bIsDeleteUserDone = false;

		DeleteUser(User, bIsDeleteUserDone);

		AB_TEST_TRUE(bIsDeleteUserDone);
	}

	return true;
}
