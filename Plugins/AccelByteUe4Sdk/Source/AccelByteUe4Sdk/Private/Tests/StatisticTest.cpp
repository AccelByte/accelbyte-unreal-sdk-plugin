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
#include "Api/AccelByteGameProfileApi.h"
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

const FString StatisticStatCode = "SDKTEST";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticSetup, "AccelByte.Tests.Statistic.A.Setup", AutomationFlagMaskStatistic);
bool StatisticSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess = false;
	bool UsersLoginSuccess = false;

	FRegistry::User.LoginWithClientCredentials(FSimpleDelegate::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Client Login Success"));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bClientLoginSuccess, "Waiting for Login...");


	FString Email = FString::Printf(TEXT("Statistic_UE4Test@example.com"));
	Email.ToLowerInline();
	FString Password = FString::Printf(TEXT("123Password123"), 0);
	FString DisplayName = FString::Printf(TEXT("StatisticUE4Test"), 0);
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	FRegistry::User.Register(Email, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Response)
	{
		UsersCreationSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test User is successfuly created."));
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if (Code == EHttpResponseCodes::Conflict)
		{
			UsersCreationSuccess = true;
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Test User can't be created"));
		}
	}));
	FlushHttpRequests();
	Waiting(UsersCreationSuccess, "Waiting for user created...");

	FRegistry::User.LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
	{
		UsersLoginSuccess = true;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tSuccessfully Login."));
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(UsersLoginSuccess, "Waiting for Login...");
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	check(bClientLoginSuccess);
	check(UsersLoginSuccess);

	bool bStatIsExist = false;
	bool bGetStatDone = false;
	FAccelByteModelsStatInfo GetStatResult;
	Statistic_Get_Stat_By_StatCode(StatisticStatCode, THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bGetStatDone, &GetStatResult](const FAccelByteModelsStatInfo& Result)
	{
		GetStatResult = Result;
		bStatIsExist = true;
		bGetStatDone = true;
	}), FErrorHandler::CreateLambda([&bStatIsExist, &bGetStatDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bGetStatDone = true;
	}));
	FlushHttpRequests();
	Waiting(bStatIsExist, "Waiting for get stat...");

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

		Statistic_Create_Stat(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bCreateStatDone, &CreateStatResult](FAccelByteModelsStatInfo Result)
		{
			CreateStatResult = Result;
			bCreateStatDone = true;
			bStatIsExist = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bCreateStatDone, "Waiting for stat created...");
		check(bCreateStatDone);
	}
	check(bStatIsExist);

	bool bUserStatItemIsExist = false;
	bool bGetUserStatItemDone = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetUserStatItemResult;
	FRegistry::Statistic.GetUserStatItemsByStatCodes({ StatisticStatCode }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&bGetUserStatItemDone, &bUserStatItemIsExist, &GetUserStatItemResult](FAccelByteModelsUserStatItemPagingSlicedResult Result)
	{
		GetUserStatItemResult = Result;
		bGetUserStatItemDone = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemDone, "Waiting for get statitem...");
	if (GetUserStatItemResult.Data.Num() == 0)
	{
		bool bCreateStatItemDone = false;
		TArray<FAccelByteModelsBulkStatItemIncResult> CreateStatItemResult;
		Statistic_Bulk_Create_User_StatItem(FRegistry::Credentials.GetUserId(), { StatisticStatCode }, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([&bUserStatItemIsExist, &bCreateStatItemDone, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemIncResult> Result)
		{
			CreateStatItemResult = Result;
			bUserStatItemIsExist = true;
			bCreateStatItemDone = true;
		}), StatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bCreateStatItemDone, "Waiting for statitem created...");
		check(bCreateStatItemDone);
	}
	else
	{
		bUserStatItemIsExist = true;
	}
	check(bUserStatItemIsExist);
	check(bGetStatDone);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticTearDown, "AccelByte.Tests.Statistic.Z.Teardown", AutomationFlagMaskStatistic);
bool StatisticTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteUsersSuccessful = false;

	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("DeleteUserById..."));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Success"));
		bDeleteUsersSuccessful = true;
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteUsersSuccessful, "Waiting for user deletion...");


	check(bDeleteUsersSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticGetAllUserStatItems, "AccelByte.Tests.Statistic.B.0GetAllUserStatItems_Success", AutomationFlagMaskStatistic);
bool StatisticGetAllUserStatItems::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER ALL STATITEMS..."));
	bool bGetAllUserStatItemsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::Statistic.GetAllUserStatItems(THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetAllUserStatItemsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
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
	FRegistry::Statistic.GetUserStatItemsByStatCodes({ StatisticStatCode }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
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
	FRegistry::Statistic.GetUserStatItemsByTags({ StatisticStatCode }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByTagsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticBulkAddStatItemValue, "AccelByte.Tests.Statistic.B.BulkAddStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticBulkAddStatItemValue::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD STATITEM VALUE..."));
	bool bBulkAddStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemIncResult> BulkAddStatItemResult;
	FAccelByteModelsBulkUserStatItemInc MVP;
	MVP.inc = 1;
	MVP.userId = FRegistry::Credentials.GetUserId();
	MVP.statCode = StatisticStatCode;
	FRegistry::Statistic.BulkAddStatItemValue({ MVP }, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([&bBulkAddStatItemSuccess, &BulkAddStatItemResult](TArray<FAccelByteModelsBulkStatItemIncResult> Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD STATITEMS SUCCESS!"));
		bBulkAddStatItemSuccess = true;
		BulkAddStatItemResult = Result;
		for (FAccelByteModelsBulkStatItemIncResult data : Result)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *data.StatCode, data.Success);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bBulkAddStatItemSuccess, "Waiting for bulk add statitem...");
	check(bBulkAddStatItemSuccess);
	check(BulkAddStatItemResult[0].Success);
	check(BulkAddStatItemResult[0].StatCode == MVP.statCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticBulkAddUserStatItemValue, "AccelByte.Tests.Statistic.B.BulkAddUserStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticBulkAddUserStatItemValue::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEM VALUE..."));
	bool bBulkAddUserStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemIncResult> BulkAddUserStatItemResult;
	FAccelByteModelsBulkStatItemInc MVP;
	MVP.inc = 1;
	MVP.statCode = StatisticStatCode;
	FRegistry::Statistic.BulkAddUserStatItemValue({ MVP }, THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>::CreateLambda([&bBulkAddUserStatItemSuccess, &BulkAddUserStatItemResult](TArray<FAccelByteModelsBulkStatItemIncResult> Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEMS SUCCESS!"));
		bBulkAddUserStatItemSuccess = true;
		BulkAddUserStatItemResult = Result;
		for (FAccelByteModelsBulkStatItemIncResult data : Result)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatisticAddUserStatItemValue, "AccelByte.Tests.Statistic.B.AddUserStatItemValue_Success", AutomationFlagMaskStatistic);
bool StatisticAddUserStatItemValue::RunTest(const FString& Parameters)
{
	const FString statCode = StatisticStatCode;
	bool bGetUserStatItemSuccess = false;
	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
	bool bGetUserStatItemsByStatCodesSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::Statistic.GetUserStatItemsByStatCodes({ statCode }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %f"), *data.StatCode, data.Value);
		}
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");
	check(bGetUserStatItemsByStatCodesSuccess);

	UE_LOG(LogAccelByteStatisticTest, Log, TEXT("ADD USER STATITEM VALUE..."));
	const float ValueBefore = GetResult.Data[0].Value;
	bool bAddUserStatItemSuccess = false;
	FAccelByteModelsStatItemIncResult AddUserStatItemResult;
	const float IncreasedValue = 1;
	FRegistry::Statistic.AddUserStatItemValue(statCode, IncreasedValue, THandler<FAccelByteModelsStatItemIncResult>::CreateLambda([&bAddUserStatItemSuccess, &AddUserStatItemResult](FAccelByteModelsStatItemIncResult Result)
	{
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("ADD USER STATITEMS SUCCESS!"));
		bAddUserStatItemSuccess = true;
		AddUserStatItemResult = Result;
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Current Value: %f"), Result.currentValue);
	}), StatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bAddUserStatItemSuccess, "Waiting for add User statitem...");
	check(bAddUserStatItemSuccess);
	check(AddUserStatItemResult.currentValue == ValueBefore + IncreasedValue);
	return true;
}