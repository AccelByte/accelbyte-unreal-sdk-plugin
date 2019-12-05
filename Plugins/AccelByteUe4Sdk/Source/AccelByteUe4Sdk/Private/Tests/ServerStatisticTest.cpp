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

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerStatisticTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerStatisticTest);

const int32 AutomationFlagMaskServerStatistic = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto ServerStatisticTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteServerStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const TArray<FString> StatisticStatCodes = { "sdktest" , "sdktest2"};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticSetup, "AccelByte.Tests.ServerStatistic.A.Setup", AutomationFlagMaskServerStatistic);
bool ServerStatisticSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess = false;
	bool UsersLoginSuccess = false;

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
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Test User is successfuly created."));
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if ((ErrorCodes) Code == ErrorCodes::UserEmailAlreadyUsedException)
		{
			UsersCreationSuccess = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Test User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Test User can't be created"));
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
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("\t\tSuccessfully Login."));
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(UsersLoginSuccess, "Waiting for Login...");
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("\t\tClient Successfully Login."));
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

	check(bClientLoginSuccess);
	check(UsersLoginSuccess);

	for (int i = 0; i < StatisticStatCodes.Num(); i++)
	{
		bool bStatIsExist = false;
		bool bGetStatDone = false;
		FAccelByteModelsStatInfo GetStatResult;
		Statistic_Get_Stat_By_StatCode(StatisticStatCodes[i], THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bGetStatDone, &GetStatResult](const FAccelByteModelsStatInfo& Result)
		{
			GetStatResult = Result;
			bStatIsExist = true;
			bGetStatDone = true;
		}), FErrorHandler::CreateLambda([&bStatIsExist, &bGetStatDone](int32 ErrorCode, FString ErrorMessage)
		{
			if ((ErrorCodes)ErrorCode != ErrorCodes::StatisticNotFoundException)
			{
				UE_LOG(LogAccelByteServerStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			}
			else
			{
				UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
			createStat.name = StatisticStatCodes[i];
			createStat.setAsGlobal = true;
			createStat.setBy = EAccelByteStatisticSetBy::CLIENT;
			createStat.statCode = StatisticStatCodes[i];
			createStat.tags = { StatisticStatCodes[i] };

			Statistic_Create_Stat(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&bStatIsExist, &bCreateStatDone, &CreateStatResult](FAccelByteModelsStatInfo Result)
			{
				CreateStatResult = Result;
				bCreateStatDone = true;
				bStatIsExist = true;
			}), ServerStatisticTestErrorHandler);
			FlushHttpRequests();
			Waiting(bCreateStatDone, "Waiting for stat created...");
			check(bCreateStatDone);
		}
		check(bStatIsExist);
		check(bGetStatDone);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticTearDown, "AccelByte.Tests.ServerStatistic.Z.Teardown", AutomationFlagMaskServerStatistic);
bool ServerStatisticTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteUsersSuccessful = false;

	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("DeleteUserById..."));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
		bDeleteUsersSuccessful = true;
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteUsersSuccessful, "Waiting for user deletion...");
	check(bDeleteUsersSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticCreateStatItem, "AccelByte.Tests.ServerStatistic.B.CreateStatItem_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticCreateStatItem::RunTest(const FString& Parameters)
{
	bool bUserStatItemIsExist = false;
	bool bGetUserStatItemDone = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetUserStatItemResult;
	FRegistry::ServerStatistic.GetUserStatItemsByStatCodes(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0] }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&bGetUserStatItemDone, &bUserStatItemIsExist, &GetUserStatItemResult](FAccelByteModelsUserStatItemPagingSlicedResult Result)
	{
		GetUserStatItemResult = Result;
		bGetUserStatItemDone = true;
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemDone, "Waiting for get statitem...");

	for (int i = 0; i < GetUserStatItemResult.Data.Num(); i++)
	{
		bool bDeleteUserStatSuccess = false;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete StatItem"));
		Statistic_Delete_StatItem(FRegistry::Credentials.GetUserId(), GetUserStatItemResult.Data[i].StatCode, FSimpleDelegate::CreateLambda([&bDeleteUserStatSuccess]()
		{
			bDeleteUserStatSuccess = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success..!"));
		}), ServerStatisticTestErrorHandler);
		FlushHttpRequests();
		Waiting(bDeleteUserStatSuccess, "Waiting for user statitem deletion...");
		check(bDeleteUserStatSuccess);
	}

	bool bCreateStatItemDone = false;
	bool bCreateStatItemSuccess = false;
	FAccelByteModelsUserStatItemInfo CreateStatItemResult;
	FRegistry::ServerStatistic.CreateUserStatItem(FRegistry::Credentials.GetUserId(), StatisticStatCodes[0], THandler<FAccelByteModelsUserStatItemInfo>::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess, &CreateStatItemResult](FAccelByteModelsUserStatItemInfo Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		bCreateStatItemSuccess = true;
	}), FErrorHandler::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess](int32 ErrorCode, FString ErrorMessage)
	{
		if ((ErrorCodes)ErrorCode != ErrorCodes::UserStatAlreadyExistException)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		}
		else
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			bCreateStatItemSuccess = true;
		}
		bCreateStatItemDone = true;
	}));
	FlushHttpRequests();
	Waiting(bCreateStatItemDone, "Waiting for statitem created...");
	check(bCreateStatItemDone);
	check(bCreateStatItemSuccess);

	bool bStatItemDeleteSuccess = false;
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete User StatItem"));
	Statistic_Delete_StatItem(FRegistry::Credentials.GetUserId(), StatisticStatCodes[0], FSimpleDelegate::CreateLambda([&bStatItemDeleteSuccess]()
	{
		bStatItemDeleteSuccess = true;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
	}), FErrorHandler::CreateLambda([&bStatItemDeleteSuccess](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		if ((ErrorCodes)ErrorCode == ErrorCodes::UserStatsNotFoundException)
		{
			bStatItemDeleteSuccess = true;
		}
	}));
	FlushHttpRequests();
	Waiting(bStatItemDeleteSuccess, "Waiting for statitem deleted...");
	check(bStatItemDeleteSuccess);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticBulkCreateStatItem, "AccelByte.Tests.ServerStatistic.B.BulkCreateStatItem_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticBulkCreateStatItem::RunTest(const FString& Parameters)
{
	bool bUserStatItemIsExist = false;
	bool bGetUserStatItemDone = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetUserStatItemResult;
	FRegistry::ServerStatistic.GetUserStatItemsByStatCodes(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0], StatisticStatCodes[1] }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&bGetUserStatItemDone, &bUserStatItemIsExist, &GetUserStatItemResult](FAccelByteModelsUserStatItemPagingSlicedResult Result)
	{
		GetUserStatItemResult = Result;
		bGetUserStatItemDone = true;
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemDone, "Waiting for get statitem...");

	for (int i = 0; i < GetUserStatItemResult.Data.Num(); i++)
	{
		bool bDeleteUserStatSuccess = false;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete StatItem"));
		Statistic_Delete_StatItem(FRegistry::Credentials.GetUserId(), GetUserStatItemResult.Data[i].StatCode, FSimpleDelegate::CreateLambda([&bDeleteUserStatSuccess]()
		{
			bDeleteUserStatSuccess = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success..!"));
		}), FErrorHandler::CreateLambda([&bDeleteUserStatSuccess](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			if ((ErrorCodes)ErrorCode == ErrorCodes::UserStatsNotFoundException)
			{
				bDeleteUserStatSuccess = true;
			}
		}));
		FlushHttpRequests();
		Waiting(bDeleteUserStatSuccess, "Waiting for user statitem deletion...");
		check(bDeleteUserStatSuccess);
	}

	bool bCreateStatItemDone = false;
	bool bCreateStatItemSuccess = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> CreateStatItemResult;
	FRegistry::ServerStatistic.BulkCreateUserStatItems(FRegistry::Credentials.GetUserId(), { {StatisticStatCodes[0]}, {StatisticStatCodes[1]} }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		bCreateStatItemSuccess = true;
		for (FAccelByteModelsBulkStatItemOperationResult& result : Result)
		{
			if (!result.Success)
			{
				if ((ErrorCodes)FCString::Atoi(*result.Details["errorCode"]) != ErrorCodes::UserStatAlreadyExistException)
				{
					bCreateStatItemSuccess = false;
					break;
				}
			}
		}
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bCreateStatItemDone, "Waiting for statitem created...");
	check(bCreateStatItemDone);
	check(bCreateStatItemSuccess);

	for (int i = 0; i < StatisticStatCodes.Num(); i++)
	{
		bool bStatItemDeleteSuccess = false;
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Delete User StatItem"));
		Statistic_Delete_StatItem(FRegistry::Credentials.GetUserId(), StatisticStatCodes[i], FSimpleDelegate::CreateLambda([&bStatItemDeleteSuccess]()
		{
			bStatItemDeleteSuccess = true;
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Success"));
		}), FErrorHandler::CreateLambda([&bStatItemDeleteSuccess](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			if ((ErrorCodes)ErrorCode == ErrorCodes::UserStatsNotFoundException)
			{
				bStatItemDeleteSuccess = true;
			}
		}));
		FlushHttpRequests();
		Waiting(bStatItemDeleteSuccess, "Waiting for statitem deleted...");
		check(bStatItemDeleteSuccess);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticCreateStatItemInvalid, "AccelByte.Tests.ServerStatistic.B.CreateStatItem_Invalid", AutomationFlagMaskServerStatistic);
bool ServerStatisticCreateStatItemInvalid::RunTest(const FString& Parameters)
{
	const FString InvalidStatCode = "invalid";
	bool bCreateStatItemDone = false;
	bool bCreateStatItemSuccess = false;
	bool bStatItemInvalid = false;
	FAccelByteModelsUserStatItemInfo CreateStatItemResult;
	FRegistry::ServerStatistic.CreateUserStatItem(FRegistry::Credentials.GetUserId(), InvalidStatCode, THandler<FAccelByteModelsUserStatItemInfo>::CreateLambda([&bCreateStatItemDone, &bCreateStatItemSuccess, &CreateStatItemResult](FAccelByteModelsUserStatItemInfo Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		bCreateStatItemSuccess = true;
	}), FErrorHandler::CreateLambda([&bCreateStatItemDone, &bStatItemInvalid](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bCreateStatItemDone = true;
		if ((ErrorCodes)ErrorCode == ErrorCodes::StatisticNotFoundException)
		{
			bStatItemInvalid = true;
		}
	}));
	FlushHttpRequests();
	Waiting(bCreateStatItemDone, "Waiting for statitem created...");
	check(bCreateStatItemDone);
	check(bStatItemInvalid);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticBulkCreateStatItemInvalid, "AccelByte.Tests.ServerStatistic.B.BulkCreateStatItem_Invalid", AutomationFlagMaskServerStatistic);
bool ServerStatisticBulkCreateStatItemInvalid::RunTest(const FString& Parameters)
{
	const FString InvalidStatCode = "invalid";
	bool bCreateStatItemDone = false;
	bool bStatItemInvalid = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> CreateStatItemResult;
	FRegistry::ServerStatistic.BulkCreateUserStatItems(FRegistry::Credentials.GetUserId(), { {InvalidStatCode} }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatItemDone, &bStatItemInvalid, &CreateStatItemResult](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
	{
		CreateStatItemResult = Result;
		bCreateStatItemDone = true;
		for (FAccelByteModelsBulkStatItemOperationResult& result : Result)
		{
			if (!result.Success)
			{
				if ((ErrorCodes)FCString::Atoi(*result.Details["errorCode"]) == ErrorCodes::StatisticNotFoundException)
				{
					bStatItemInvalid = true;
					break;
				}
			}
		}
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bCreateStatItemDone, "Waiting for statitem created...");
	check(bCreateStatItemDone);
	check(bStatItemInvalid);

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
	FlushHttpRequests();
	Waiting(bGetAllUserStatItemsSuccess, "Waiting for get all stat items...");
	check(bGetAllUserStatItemsSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticGetUserStatItemsByStatCodes, "AccelByte.Tests.ServerStatistic.B.GetUserStatItemsByStatCodes_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticGetUserStatItemsByStatCodes::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
	bool bGetUserStatItemsByStatCodesSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetUserStatItemsByStatCodes(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0] }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByStatCodesSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");
	check(bGetUserStatItemsByStatCodesSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerStatisticGetUserStatItemsByTags, "AccelByte.Tests.ServerStatistic.B.GetUserStatItemsByTags_Success", AutomationFlagMaskServerStatistic);
bool ServerStatisticGetUserStatItemsByTags::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GETTING USER STATITEMS BY TAGS..."));
	bool bGetUserStatItemsByTagsSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
	FRegistry::ServerStatistic.GetUserStatItemsByTags(FRegistry::Credentials.GetUserId(), { StatisticStatCodes[0] }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([this, &GetResult, &bGetUserStatItemsByTagsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
		bGetUserStatItemsByTagsSuccess = true;
		GetResult = Result;
		for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
		{
			UE_LOG(LogAccelByteServerStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
		}
	}), ServerStatisticTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetUserStatItemsByTagsSuccess, "Waiting for get stat items...");
	check(bGetUserStatItemsByTagsSuccess);
	return true;
}
