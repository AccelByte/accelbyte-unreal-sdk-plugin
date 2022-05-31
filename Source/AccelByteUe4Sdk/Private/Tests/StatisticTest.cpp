// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteMultiRegistry.h"
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

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteStatisticTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteStatisticTest);

const int32 AutomationFlagMaskStatistic = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto StatisticTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteStatisticTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

BEGIN_DEFINE_SPEC(FStatisticTestSpec, "AccelByte.Tests.Statistic", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	const FString StatisticStatCode = "sdktest";
	FTestUser TestUser;
END_DEFINE_SPEC(FStatisticTestSpec)

void FStatisticTestSpec::Define()
{
	const auto setupOnce = [this]()
	{
		AB_TEST_TRUE(SetupTestUser(TestUser));

		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("LOGIN WITH CLIENT CREDENTIALS..."));
		bool bClientLoginSuccess = false;
		FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			bClientLoginSuccess = true;
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("\t\tClient Successfully Login."));
		}), StatisticTestErrorHandler);

		WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");
		AB_TEST_TRUE(bClientLoginSuccess);
		
		bool bStatIsExist = false;
		bool bGetStatDone = false;
		FAccelByteModelsStatInfo GetStatResult;
		AdminGetStatisticByStatCode(StatisticStatCode, THandler<FAccelByteModelsStatInfo>::CreateLambda([&GetStatResult, &bStatIsExist, &bGetStatDone](const FAccelByteModelsStatInfo& Result)
		{
			GetStatResult = Result;
			bStatIsExist = true;
			bGetStatDone = true;
		}), FErrorHandler::CreateLambda([&bGetStatDone](int32 ErrorCode, FString ErrorMessage)
		{
			if (static_cast<ErrorCodes>(ErrorCode) != ErrorCodes::StatisticConfigNotFoundException)
			{
				UE_LOG(LogAccelByteStatisticTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
			}
			else
			{
				UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
			createStat.name = StatisticStatCode;
			createStat.setAsGlobal = true;
			createStat.setBy = EAccelByteStatisticSetBy::CLIENT;
			createStat.statCode = StatisticStatCode;
			createStat.tags = { StatisticStatCode };

			AdminCreateStatistic(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&CreateStatResult, &bCreateStatDone](FAccelByteModelsStatInfo Result)
			{
				CreateStatResult = Result;
				bCreateStatDone = true;
			}), StatisticTestErrorHandler);
			WaitUntil(bCreateStatDone, "Waiting for stat created...");
		}

		FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
		
		UE_LOG(LogAccelByteStatisticTest, Log, TEXT("CREATE USER STAT ITEM..."));
		TArray<FString> StatCodes;
		StatCodes.Add(StatisticStatCode);
		bool bCreateStatSuccess = false;
		ApiClient->Statistic.CreateUserStatItems(StatCodes, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&bCreateStatSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("Stat Codes are created."));
			bCreateStatSuccess = true;
		}), StatisticTestErrorHandler);

		WaitUntil(bCreateStatSuccess, "Waiting for create stat codes...");
		AB_TEST_TRUE(bCreateStatSuccess);
		
		return true;
	};

	BeforeEach(setupOnce);

	const auto teardown = [this]()
	{		
		AB_TEST_TRUE(DeleteTestUser(TestUser));

		return true;
	};

	AfterEach(teardown);

	Describe("GetAllUserStatItems_Success", [this]()
	{
		It("Should get all user stat item", [this]()
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER ALL STATITEMS..."));
			
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
			bool bGetAllUserStatItemsSuccess = false;
			FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
			ApiClient->Statistic.GetAllUserStatItems(THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&GetResult, &bGetAllUserStatItemsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
			{
				UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET ALL User STATITEMS SUCCESS!"));
				bGetAllUserStatItemsSuccess = true;
				GetResult = Result;
				for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
				{
					UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
				}
			}), StatisticTestErrorHandler);

			WaitUntil(bGetAllUserStatItemsSuccess, "Waiting for get all stat items...");
			AB_TEST_TRUE(bGetAllUserStatItemsSuccess);
			AB_TEST_TRUE(GetResult.Data.Num() > 0);

			return true;
		});
	});

	Describe("GetUserStatItemsByStatCodes_Success", [this]()
	{
		It("Should get user stat item by stat code", [this]()
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER STATITEMS BY STATCODES..."));
			
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
			bool bGetUserStatItemsByStatCodesSuccess = false;
			FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
			ApiClient->Statistic.GetUserStatItems({ StatisticStatCode }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&GetResult, &bGetUserStatItemsByStatCodesSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
			{
				UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
				bGetUserStatItemsByStatCodesSuccess = true;
				GetResult = Result;
				for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
				{
					UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
				}
			}), StatisticTestErrorHandler);

			WaitUntil(bGetUserStatItemsByStatCodesSuccess, "Waiting for get stat items...");
			AB_TEST_TRUE(bGetUserStatItemsByStatCodesSuccess);
			AB_TEST_TRUE(GetResult.Data.Num() > 0);
			AB_TEST_EQUAL(GetResult.Data[0].StatCode, StatisticStatCode);
			
			return true;
		});
	});

	Describe("GetUserStatItemsByTags_Success", [this]()
	{
		It("Should get user stat item by tags", [this]()
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GETTING USER STATITEMS BY TAGS..."));
			
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
			bool bGetUserStatItemsByTagsSuccess = false;
			FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
			ApiClient->Statistic.GetUserStatItems({}, { StatisticStatCode }, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&GetResult, &bGetUserStatItemsByTagsSuccess](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
			{
				UE_LOG(LogAccelByteStatisticTest, Log, TEXT("GET User STATITEMS SUCCESS!"));
				bGetUserStatItemsByTagsSuccess = true;
				GetResult = Result;
				for (FAccelByteModelsUserStatItemInfo data : GetResult.Data)
				{
					UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Value: %d"), *data.StatCode, data.Value);
				}
			}), StatisticTestErrorHandler);

			WaitUntil(bGetUserStatItemsByTagsSuccess, "Waiting for get stat items...");
			AB_TEST_TRUE(bGetUserStatItemsByTagsSuccess);
			AB_TEST_TRUE(GetResult.Data.Num() > 0);
			AB_TEST_EQUAL(GetResult.Data[0].Tags[0], StatisticStatCode);
			
			return true;
		});
	});

	Describe("BulkAddUserStatItemValue_Success", [this]()
	{
		It("Should add User Stat item in bulk", [this]()
		{
			UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEM VALUE..."));
			
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
			bool bBulkAddUserStatItemSuccess = false;
			TArray<FAccelByteModelsBulkStatItemOperationResult> BulkAddUserStatItemResult;
			FAccelByteModelsBulkStatItemInc MVP;
			MVP.inc = 1;
			MVP.statCode = StatisticStatCode;
			ApiClient->Statistic.IncrementUserStatItems({ MVP }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&BulkAddUserStatItemResult, &bBulkAddUserStatItemSuccess](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
			{
				UE_LOG(LogAccelByteStatisticTest, Log, TEXT("BULK ADD USER STATITEMS SUCCESS!"));
				bBulkAddUserStatItemSuccess = true;
				BulkAddUserStatItemResult = Result;
				for (FAccelByteModelsBulkStatItemOperationResult data : Result)
				{
					UE_LOG(LogAccelByteStatisticTest, Log, TEXT("StatCode: %s | Success: %d"), *data.StatCode, data.Success);
				}
			}), StatisticTestErrorHandler);

			WaitUntil(bBulkAddUserStatItemSuccess, "Waiting for bulk add User stat item...");
			AB_TEST_TRUE(bBulkAddUserStatItemSuccess);
			AB_TEST_TRUE(BulkAddUserStatItemResult[0].Success);
			AB_TEST_EQUAL(BulkAddUserStatItemResult[0].StatCode, MVP.statCode);
			
			return true;
		});
	});
}