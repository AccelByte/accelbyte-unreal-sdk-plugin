// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteStatisticApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "StatisticTestAdmin.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerStatisticTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerStatisticTest);

const int32 AutomationFlagMaskServerStatistic = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto ServerStatisticTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteServerStatisticTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const TArray<FString> StatisticStatCodes = { "sdktest3" , "sdktest4"};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticSetup, "AccelByte.Tests.ServerStatistic.A.Setup", AutomationFlagMaskServerStatistic);
bool ServerStatisticSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool bUserCreated = false;
	bool bUserLoggedIn = false;

	FString Email = FString::Printf(TEXT("Statistic_UE4Test@example.com"));
	Email.ToLowerInline();
	FString Password = TEXT("123Password123");
	FString DisplayName = TEXT("StatisticUE4Test");
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	FRegistry::User.Register(Email, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Response)
	{
		bUserCreated = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Test User is successfuly created."));
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException)
		{
			bUserCreated = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Test User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Test User can't be created"));
		}
	}));
	WaitUntil(bUserCreated, "Waiting for user created...");

	FRegistry::User.LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		bUserLoggedIn = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("\t\tSuccessfully Login."));
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bUserLoggedIn, "Waiting for Login...");
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("\t\tClient Successfully Login."));
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	AB_TEST_TRUE(bClientLoginSuccess);
	AB_TEST_TRUE(bUserLoggedIn);

	for (int i = 0; i < StatisticStatCodes.Num(); i++)
	{
		bool bStatIsExist = false;
		bool bGetStatDone = false;
		FAccelByteModelsStatInfo GetStatResult;
		AdminGetStatisticByStatCode(StatisticStatCodes[i], THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bGetStatDone, &GetStatResult](const FAccelByteModelsStatInfo& Result)
		{
			GetStatResult = Result;
			bStatIsExist = true;
			bGetStatDone = true;
		}), FErrorHandler::CreateLambda([&bGetStatDone](int32 ErrorCode, FString ErrorMessage)
		{
			if (ErrorCode != 70131 && ErrorCode != 12241)
			{
				UE_LOG(LogAccelByteServerStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			}
			else
			{
				UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			}
			bGetStatDone = true;
		}));
		WaitUntil(bGetStatDone, "Waiting for get stat...");

		if (!bStatIsExist)
		{
			bool bCreateStatDone = false;
			FAccelByteModelsStatInfo CreateStatResult;
			FStatCreateRequest createStat;
			createStat.defaultValue = 0;
			createStat.description = "StatCode for SDK Test purpose";
			createStat.incrementOnly = true;
			createStat.maximum = 999999;
			createStat.minimum = 0;
			createStat.name = StatisticStatCodes[i];
			createStat.setAsGlobal = true;
			createStat.setBy = EAccelByteStatisticSetBy::SERVER;
			createStat.statCode = StatisticStatCodes[i];
			createStat.tags = { StatisticStatCodes[i] };

			AdminCreateStatistic(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bCreateStatDone, &CreateStatResult](FAccelByteModelsStatInfo Result)
			{
				CreateStatResult = Result;
				bCreateStatDone = true;
				bStatIsExist = true;
			}), ServerStatisticTestErrorHandler);
			WaitUntil(bCreateStatDone, "Waiting for stat created...");
			AB_TEST_TRUE(bCreateStatDone);
		}
		AB_TEST_TRUE(bStatIsExist);
		AB_TEST_TRUE(bGetStatDone);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticTearDown, "AccelByte.Tests.ServerStatistic.Z.Teardown", AutomationFlagMaskServerStatistic);
bool ServerStatisticTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteUsersSuccessful = false;

	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("DeleteUserById..."));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
		bDeleteUsersSuccessful = true;
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bDeleteUsersSuccessful, "Waiting for user deletion...");
	AB_TEST_TRUE(bDeleteUsersSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticBulkCreateStatItem, "AccelByte.Tests.ServerStatistic.B.CreateUserStatItems_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticBulkCreateStatItem::RunTest(const FString& Parameters)
{
	bool bGetUserStatItemDone = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetUserStatItemResult;
	FRegistry::ServerStatistic.GetUserStatItems(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0], StatisticStatCodes[1] }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&bGetUserStatItemDone, &GetUserStatItemResult](FAccelByteModelsUserStatItemPagingSlicedResult Result)
	{
		GetUserStatItemResult = Result;
		bGetUserStatItemDone = true;
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bGetUserStatItemDone, "Waiting for get statitem...");

	for (int i = 0; i < GetUserStatItemResult.Data.Num(); i++)
	{
		bool bDeleteUserStatSuccess = false;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete StatItem"));
		AdminDeleteStatisticItem(FRegistry::Credentials.GetUserId(), GetUserStatItemResult.Data[i].StatCode, FSimpleDelegate::CreateLambda([&bDeleteUserStatSuccess]()
		{
			bDeleteUserStatSuccess = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success..!"));
		}), FErrorHandler::CreateLambda([&bDeleteUserStatSuccess](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			if (static_cast<ErrorCodes>(ErrorCode) == ErrorCodes::UserStatsNotFoundException)
			{
				bDeleteUserStatSuccess = true;
			}
		}));
		WaitUntil(bDeleteUserStatSuccess, "Waiting for user statitem deletion...");
		AB_TEST_TRUE(bDeleteUserStatSuccess);
	}

	bool bCreateStatItemDone = false;
	bool bCreateStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> CreateStatItemResult;
	FRegistry::ServerStatistic.CreateUserStatItems(FRegistry::Credentials.GetUserId(), { {StatisticStatCodes[0]}, {StatisticStatCodes[1]} }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		bCreateStatItemSuccess = true;
		for (FAccelByteModelsBulkStatItemOperationResult& result : Result)
		{
			if (!result.Success)
			{
				if (static_cast<ErrorCodes>(FCString::Atoi(*result.Details["errorCode"])) != ErrorCodes::UserStatAlreadyExistException)
				{
					bCreateStatItemSuccess = false;
					break;
				}
			}
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bCreateStatItemDone, "Waiting for statitem created...");
	AB_TEST_TRUE(bCreateStatItemDone);
	AB_TEST_TRUE(bCreateStatItemSuccess);

	for (int i = 0; i < StatisticStatCodes.Num(); i++)
	{
		bool bStatItemDeleteSuccess = false;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete User StatItem"));
		AdminDeleteStatisticItem(FRegistry::Credentials.GetUserId(), StatisticStatCodes[i], FSimpleDelegate::CreateLambda([&bStatItemDeleteSuccess]()
		{
			bStatItemDeleteSuccess = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
		}), FErrorHandler::CreateLambda([&bStatItemDeleteSuccess](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			if (static_cast<ErrorCodes>(ErrorCode) == ErrorCodes::UserStatsNotFoundException)
			{
				bStatItemDeleteSuccess = true;
			}
		}));
		WaitUntil(bStatItemDeleteSuccess, "Waiting for statitem deleted...");
		AB_TEST_TRUE(bStatItemDeleteSuccess);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticBulkCreateStatItemInvalid, "AccelByte.Tests.ServerStatistic.B.CreateUserStatItems_Invalid", AutomationFlagMaskServerStatistic);
bool ServerStatisticBulkCreateStatItemInvalid::RunTest(const FString& Parameters)
{
	const FString InvalidStatCode = "invalid";
	bool bCreateStatItemDone = false;
	bool bStatItemInvalid = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> CreateStatItemResult;
	FRegistry::ServerStatistic.CreateUserStatItems(FRegistry::Credentials.GetUserId(), { {InvalidStatCode} }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatItemDone, &bStatItemInvalid, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		for (FAccelByteModelsBulkStatItemOperationResult& result : Result)
		{
			if (!result.Success)
			{
				if (static_cast<ErrorCodes>(FCString::Atoi(*result.Details["errorCode"])) == ErrorCodes::StatisticNotFoundException)
				{
					bStatItemInvalid = true;
					break;
				}
			}
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bCreateStatItemDone, "Waiting for statitem created...");
	AB_TEST_TRUE(bCreateStatItemDone);
	AB_TEST_TRUE(bStatItemInvalid);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticGetAllUserStatItems, "AccelByte.Tests.ServerStatistic.B.GetAllUserStatItems_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticGetAllUserStatItems::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER ALL STATITEMS..."));
	bool bGetAllUserStatItemsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetAllUserStatItems(FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetAllUserStatItemsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET ALL User STATITEMS SUCCESS!"));
		bGetAllUserStatItemsSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bGetAllUserStatItemsSuccess, "Waiting for get all stat items...");
	AB_TEST_TRUE(bGetAllUserStatItemsSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticGetUserStatItemsByStatCodes, "AccelByte.Tests.ServerStatistic.B.GetUserStatItemsByStatCodes_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticGetUserStatItemsByStatCodes::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
	bool bGetUserStatItemsByStatCodesSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetUserStatItems(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0] }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");
	AB_TEST_TRUE(bGetUserStatItemsByStatCodesSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticGetUserStatItemsByTags, "AccelByte.Tests.ServerStatistic.B.GetUserStatItemsByTags_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticGetUserStatItemsByTags::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER STATITEMS BY TAGS..."));
	bool bGetUserStatItemsByTagsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetUserStatItems(FRegistry::Credentials.GetUserId(), {}, { StatisticStatCodes[0] }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByTagsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByTagsSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bGetUserStatItemsByTagsSuccess, "Waiting for get stat items...");
	AB_TEST_TRUE(bGetUserStatItemsByTagsSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerstatisticIncrementUserStatItems, "AccelByte.Tests.ServerStatistic.B.IncrementUserStatItems_Success", AutomationFlagMaskServerStatistic);
bool ServerstatisticIncrementUserStatItems::RunTest(const FString& Parameters)
{
	bool bCreateStatItemDone = false;
	bool bCreateStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> CreateStatItemResult;
	FRegistry::ServerStatistic.CreateUserStatItems(FRegistry::Credentials.GetUserId(), { {StatisticStatCodes[0]}, {StatisticStatCodes[1]} }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		bCreateStatItemSuccess = true;
		for (FAccelByteModelsBulkStatItemOperationResult& result : Result)
		{
			if (!result.Success)
			{
				if (static_cast<ErrorCodes>(FCString::Atoi(*result.Details["errorCode"])) != ErrorCodes::UserStatAlreadyExistException)
				{
					bCreateStatItemSuccess = false;
					break;
				}
			}
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bCreateStatItemDone, "Waiting for statitem created...");

	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("INCREMENT USER STATITEMS BY 7..."));
	bool bIncrementManyUsersStatItemsSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> IncrementResult;
	FRegistry::ServerStatistic.IncrementUserStatItems(
		FRegistry::Credentials.GetUserId(), 
		{ { 7, StatisticStatCodes[0] } }, 
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([this, &IncrementResult, &bIncrementManyUsersStatItemsSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
			bIncrementManyUsersStatItemsSuccess = true;
			IncrementResult = Result;
			for (FAccelByteModelsBulkStatItemOperationResult Data : IncrementResult)
			{
				UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *Data.StatCode, Data.Success);
			}
		}), 
		ServerStatisticTestErrorHandler);
	WaitUntil(bIncrementManyUsersStatItemsSuccess, "Waiting for increment user stat items...");
	
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
	bool bGetUserStatItemsByStatCodesSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetUserStatItems(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0] }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");

	bool bStatItemDeleteSuccess = false;
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete User StatItem"));
	AdminDeleteStatisticItem(FRegistry::Credentials.GetUserId(), StatisticStatCodes[0], FSimpleDelegate::CreateLambda([&bStatItemDeleteSuccess]()
	{
		bStatItemDeleteSuccess = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
	}), FErrorHandler::CreateLambda([&bStatItemDeleteSuccess](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		if (static_cast<ErrorCodes>(ErrorCode) == ErrorCodes::UserStatsNotFoundException)
		{
			bStatItemDeleteSuccess = true;
		}
	}));
	WaitUntil(bStatItemDeleteSuccess, "Waiting for statitem deleted...");

	AB_TEST_TRUE(bIncrementManyUsersStatItemsSuccess);
	AB_TEST_TRUE(bGetUserStatItemsByStatCodesSuccess);
	AB_TEST_TRUE(IncrementResult[0].Success);
	AB_TEST_EQUAL(GetResult.Data[0].Value, 7.0f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerstatisticIncrementManyUsersStatItems, "AccelByte.Tests.ServerStatistic.B.IncrementManyUsersStatItems_Success", AutomationFlagMaskServerStatistic);
bool ServerstatisticIncrementManyUsersStatItems::RunTest(const FString& Parameters)
{
	bool bCreateStatItemDone = false;
	bool bCreateStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> CreateStatItemResult;
	FRegistry::ServerStatistic.CreateUserStatItems(FRegistry::Credentials.GetUserId(), { {StatisticStatCodes[0]}, {StatisticStatCodes[1]} }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		bCreateStatItemSuccess = true;
		for (FAccelByteModelsBulkStatItemOperationResult& result : Result)
		{
			if (!result.Success)
			{
				if (static_cast<ErrorCodes>(FCString::Atoi(*result.Details["errorCode"])) != ErrorCodes::UserStatAlreadyExistException)
				{
					bCreateStatItemSuccess = false;
					break;
				}
			}
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bCreateStatItemDone, "Waiting for statitem created...");

	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("INCREMENT USER STATITEMS BY 7..."));
	bool bIncrementManyUsersStatItemsSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> IncrementResult;
	FRegistry::ServerStatistic.IncrementManyUsersStatItems(
		{ { 7, FRegistry::Credentials.GetUserId(), StatisticStatCodes[1] } }, 
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([this, &IncrementResult, &bIncrementManyUsersStatItemsSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
			bIncrementManyUsersStatItemsSuccess = true;
			IncrementResult = Result;
			for (FAccelByteModelsBulkStatItemOperationResult Data : IncrementResult)
			{
				UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *Data.StatCode, Data.Success);
			}
		}), 
		ServerStatisticTestErrorHandler);
	WaitUntil(bIncrementManyUsersStatItemsSuccess, "Waiting for increment user stat items...");
	
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
	bool bGetUserStatItemsByStatCodesSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetUserStatItems(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[1] }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	WaitUntil(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");

	bool bStatItemDeleteSuccess = false;
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete User StatItem"));
	AdminDeleteStatisticItem(FRegistry::Credentials.GetUserId(), StatisticStatCodes[1], FSimpleDelegate::CreateLambda([&bStatItemDeleteSuccess]()
	{
		bStatItemDeleteSuccess = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
	}), FErrorHandler::CreateLambda([&bStatItemDeleteSuccess](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		if (static_cast<ErrorCodes>(ErrorCode) == ErrorCodes::UserStatsNotFoundException)
		{
			bStatItemDeleteSuccess = true;
		}
	}));
	WaitUntil(bStatItemDeleteSuccess, "Waiting for statitem deleted...");

	AB_TEST_TRUE(bIncrementManyUsersStatItemsSuccess);
	AB_TEST_TRUE(IncrementResult[0].Success);
	AB_TEST_EQUAL(GetResult.Data[0].Value, 7.0f);
	return true;
}