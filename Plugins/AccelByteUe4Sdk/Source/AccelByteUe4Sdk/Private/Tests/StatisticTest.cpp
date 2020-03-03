// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteStatisticApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteStatisticTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteStatisticTest);

const int32 AutomationFlagMaskStatistic = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto StatisticTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const FString StatisticStatCode = "sdktest";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticSetup, "AccelByte.Tests.Statistic.A.Setup", AutomationFlagMaskStatistic);
bool StatisticSetup::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("LOGIN WITH CLIENT CREDENTIALS..."));
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess = false;
	bool UsersLoginSuccess = false;

	FString Email = FString::Printf(TEXT("Statistic_UE4Test@example.com"));
	Email.ToLowerInline();
	FString Password = FString::Printf(TEXT("123Password123"), 0);
	FString DisplayName = FString::Printf(TEXT("StatisticUE4Test"), 0);
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bUsersCreationSuccess = false;
	bool bUsersCreationDone = false;
	FRegistry::User.Register(Email, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bUsersCreationSuccess, &bUsersCreationDone](const FRegisterResponse& Response)
	{
		bUsersCreationSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test User is successfuly created."));
		bUsersCreationDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
		{
			bUsersCreationSuccess = true;
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test User can't be created"));
		}
		bUsersCreationDone = true;
	}));
	FlushHttpRequests();
	Waiting(bUsersCreationDone, "Waiting for user created...");

	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("LOGIN WITH USERNAME..."));
	bool bUsersLoginSuccess = false;
	FRegistry::User.LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&bUsersLoginSuccess]()
	{
		bUsersLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tSuccessfully Login."));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bUsersLoginSuccess, "Waiting for login with user name...");
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tClient Successfully Login."));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

	check(bClientLoginSuccess);
	check(bUsersLoginSuccess);
	bool bStatIsExist = false;
	bool bGetStatDone = false;
	FAccelByteModelsStatInfo GetStatResult;
	Statistic_Get_Stat_By_StatCode(StatisticStatCode, THandler<FAccelByteModelsStatInfo>::CreateLambda([&GetStatResult, &bStatIsExist, &bGetStatDone](const FAccelByteModelsStatInfo& Result)
	{
		GetStatResult = Result;
		bStatIsExist = true;
		bGetStatDone = true;
	}), FErrorHandler::CreateLambda([&bGetStatDone](int32 ErrorCode, FString ErrorMessage)
	{
		if ((ErrorCodes)ErrorCode != ErrorCodes::StatisticNotFoundException)
		{
			UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		}
		else
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		}
		bGetStatDone = true;
	}));
	FlushHttpRequests();
	Waiting(bGetStatDone, "Waiting for get stat...");

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
		createStat.name = StatisticStatCode;
		createStat.setAsGlobal = true;
		createStat.setBy = EAccelByteStatisticSetBy::CLIENT;
		createStat.statCode = StatisticStatCode;
		createStat.tags = { StatisticStatCode };

		Statistic_Create_Stat(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&CreateStatResult, &bStatIsExist, &bCreateStatDone](FAccelByteModelsStatInfo Result)
		{
			CreateStatResult = Result;
			bCreateStatDone = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bCreateStatDone, "Waiting for stat created...");
	}

	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("CREATE USER STAT ITEM..."));
	TArray<FString> StatCodes;
	StatCodes.Add(StatisticStatCode);
	bool bCreateStatSuccess = false;
	FRegistry::Statistic.CreateUserStatItems(StatCodes, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Stat Codes are created."));
		bCreateStatSuccess = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bCreateStatSuccess, "Waiting for create stat codes...");
	check(bCreateStatSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticTearDown, "AccelByte.Tests.Statistic.Z.Teardown", AutomationFlagMaskStatistic);
bool StatisticTearDown::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("DeleteUserById..."));
	bool bDeleteUsersSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteUsersSuccess]()
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Success"));
		bDeleteUsersSuccess = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteUsersSuccess, "Waiting for user deletion...");
	check(bDeleteUsersSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetAllUserStatItems, "AccelByte.Tests.Statistic.B.0GetAllUserStatItems_Success", AutomationFlagMaskStatistic);
bool StatisticGetAllUserStatItems::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER ALL STATITEMS..."));
	bool bGetAllUserStatItemsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::Statistic.GetAllUserStatItems(THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&GetResult, &bGetAllUserStatItemsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET ALL User STATITEMS SUCCESS!"));
		bGetAllUserStatItemsSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetAllUserStatItemsSuccess, "Waiting for get all stat items...");
	check(bGetAllUserStatItemsSuccess);
	check(GetResult.Data.Num() > 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetUserStatItemsByStatCodes, "AccelByte.Tests.Statistic.B.0GetUserStatItemsByStatCodes_Success", AutomationFlagMaskStatistic);
bool StatisticGetUserStatItemsByStatCodes::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
	bool bGetUserStatItemsByStatCodesSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::Statistic.GetUserStatItems({ StatisticStatCode }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");
	check(bGetUserStatItemsByStatCodesSuccess);
	check(GetResult.Data.Num() > 0);
	check(GetResult.Data[0].StatCode == StatisticStatCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetUserStatItemsByTags, "AccelByte.Tests.Statistic.B.0GetUserStatItemsByTags_Success", AutomationFlagMaskStatistic);
bool StatisticGetUserStatItemsByTags::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER STATITEMS BY TAGS..."));
	bool bGetUserStatItemsByTagsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::Statistic.GetUserStatItems({}, { StatisticStatCode }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&GetResult, &bGetUserStatItemsByTagsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByTagsSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemsByTagsSuccess, "Waiting for get stat items...");
	check(bGetUserStatItemsByTagsSuccess);
	check(GetResult.Data.Num() > 0);
	check(GetResult.Data[0].Tags[0] == StatisticStatCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticBulkAddUserStatItemValue, "AccelByte.Tests.Statistic.B.BulkAddUserStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticBulkAddUserStatItemValue::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEM VALUE..."));
	bool bBulkAddUserStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> BulkAddUserStatItemResult;
	FAccelByteModelsBulkStatItemInc MVP;
	MVP.inc = 1;
	MVP.statCode = StatisticStatCode;
	FRegistry::Statistic.IncrementUserStatItems({ MVP }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&BulkAddUserStatItemResult, &bBulkAddUserStatItemSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEMS SUCCESS!"));
		bBulkAddUserStatItemSuccess = true;
		BulkAddUserStatItemResult = Result;
		for (FAccelByteModelsBulkStatItemOperationResult data : Result)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *data.StatCode, data.Success);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bBulkAddUserStatItemSuccess, "Waiting for bulk add User statitem...");
	check(bBulkAddUserStatItemSuccess);
	check(BulkAddUserStatItemResult[0].Success);
	check(BulkAddUserStatItemResult[0].StatCode == MVP.statCode);
	return true;
}