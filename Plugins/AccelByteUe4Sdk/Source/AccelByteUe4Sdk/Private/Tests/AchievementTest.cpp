// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteAchievementApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerAchievementApi.h"
#include "Api/AccelByteStatisticApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Achievement;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteAchievementTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteAchievementTest);

static const int32 AutomationFlagMaskAchievement = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

static const auto AchievementTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
	});

static FAchievementRequest Achievement1;
static FAchievementRequest Achievement2;
static FAchievementRequest AchievementIncrementalClient;
static FAchievementRequest AchievementIncrementalServer;
static FStatCreateRequest AchievementStatisticClientRequest;
static FStatCreateRequest AchievementStatisticServerRequest;
static TArray<FStatCreateRequest> AllStatisticRequests;
static TArray<FAchievementRequest> AllAchievementRequests;

const auto AchievementTestQueryAchievements = [](const FString& InLanguage, const EAccelByteAchievementListSortBy& InSortBy, FAccelByteModelsPaginatedPublicAchievement& OutPaginatedPublicAchievement, const int32& InOffset = 0, const int32& InLimit = 20) // XXX InOffset and InLimit defaults must match with QueryAchievements defaults
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("%s"), TEXT("Querying achievements"));
	FRegistry::Achievement.QueryAchievements(InLanguage, InSortBy,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&](FAccelByteModelsPaginatedPublicAchievement Result)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("OK"));
				OutPaginatedPublicAchievement = Result;
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
			{
				//AchievementTestErrorHandler.ExecuteIfBound(ErrorCode, ErrorMessage);
				bIsDone = true;
			}), InOffset, InLimit);
	Waiting(bIsDone, "Waiting ...");
	return bIsOk;
};

const auto AchievementTestQueryUserAchievements = [](const EAccelByteAchievementListSortBy& InSortBy, FAccelByteModelsPaginatedUserAchievement& OutPaginatedUserAchievement,
	const int32& InOffset = 0, const int32& InLimit = 20, const bool InPreferUnlocked = true) // XXX InOffset, InLimit, and InPreferUnlocked defaults must match with QueryUserAchievements defaults
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("%s"), TEXT("Querying user achievements"));
	FRegistry::Achievement.QueryUserAchievements(InSortBy,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&](FAccelByteModelsPaginatedUserAchievement Result)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("OK"));
				OutPaginatedUserAchievement = Result;
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
			{
				//AchievementTestErrorHandler.ExecuteIfBound(ErrorCode, ErrorMessage);
				bIsDone = true;
			}), InOffset, InLimit, InPreferUnlocked);
	Waiting(bIsDone, "Waiting ...");
	return bIsOk;
};

const auto AchievementTestGetAchievement = [](const FString& AchievementCode, FAccelByteModelsMultiLanguageAchievement& OutMultiLanguageAchievement)
{
	bool bIsOk = false;
	bool bIsDone = false;
	FRegistry::Achievement.GetAchievement(AchievementCode,
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&](FAccelByteModelsMultiLanguageAchievement Result)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("OK"));
				OutMultiLanguageAchievement = Result;
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
			{
				//AchievementTestErrorHandler.ExecuteIfBound(ErrorCode, ErrorMessage);
				bIsDone = true;
			}));
	Waiting(bIsDone, "Waiting ...");
	return bIsOk;
};

const auto AchievementTestServerUnlockAchievement = [](const FString& UserId, const FString& AchievementCode)
{
	bool bIsOk = false;
	bool bIsDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(UserId, AchievementCode,
		FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("OK"));
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
			{
				//AchievementTestErrorHandler.ExecuteIfBound(ErrorCode, ErrorMessage);
				bIsDone = true;
			}));
	Waiting(bIsDone, "Waiting ...");
	return bIsOk;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestSetup, "AccelByte.Tests.Achievement.A.Setup", AutomationFlagMaskAchievement);
bool FAchievementTestSetup::RunTest(const FString& Parameters)
{
	// statistic client
	{
		AchievementStatisticClientRequest.defaultValue = 0;
		AchievementStatisticClientRequest.description = "StatCode for SDK Test purpose";
		AchievementStatisticClientRequest.incrementOnly = true;
		AchievementStatisticClientRequest.maximum = 999999;
		AchievementStatisticClientRequest.minimum = 0;
		AchievementStatisticClientRequest.name = "Achievement Incremental Testing Client";
		AchievementStatisticClientRequest.setAsGlobal = false;
		AchievementStatisticClientRequest.setBy = EAccelByteStatisticSetBy::CLIENT;
		AchievementStatisticClientRequest.statCode = "incremental-testing-client";
		AchievementStatisticClientRequest.tags = { "nothing" };
	}
	// statistic server
	{
		AchievementStatisticServerRequest.defaultValue = 0;
		AchievementStatisticServerRequest.description = "StatCode for SDK Test purpose";
		AchievementStatisticServerRequest.incrementOnly = true;
		AchievementStatisticServerRequest.maximum = 999999;
		AchievementStatisticServerRequest.minimum = 0;
		AchievementStatisticServerRequest.name = "Achievement Incremental Testing Server";
		AchievementStatisticServerRequest.setAsGlobal = false;
		AchievementStatisticServerRequest.setBy = EAccelByteStatisticSetBy::SERVER;
		AchievementStatisticServerRequest.statCode = "incremental-testing-server";
		AchievementStatisticServerRequest.tags = { "nothing" };
	}

	// achievement1
	{
		Achievement1.AchievementCode = TEXT("UE4AchievementCode1");
		Achievement1.DefaultLanguage = TEXT("en");
		Achievement1.Description.Add(TEXT("en"), TEXT("This is achievement 1 en description"));
		Achievement1.GoalValue = 7;
		Achievement1.Hidden = true;
		Achievement1.Incremental = false;
		Achievement1.LockedIcons.Add({ TEXT("This is locked icon 1 url"), TEXT("This is locked icon 1 slug") });
		Achievement1.LockedIcons.Add({ TEXT("This is locked icon 2 url"), TEXT("This is locked icon 2 slug") });
		Achievement1.Name.Add(TEXT("en"), TEXT("This is achievement 1 en description"));
		Achievement1.StatCode = TEXT("This is achievement 1 stat code");
		Achievement1.Tags.Add(TEXT("Tags1"));
		Achievement1.Tags.Add(TEXT("Tags2"));
		Achievement1.UnlockedIcons.Add({ TEXT("This is unlocked icon 1 url"), TEXT("This is unlocked icon 1 slug") });
		Achievement1.UnlockedIcons.Add({ TEXT("This is unlocked icon 2 url"), TEXT("This is unlocked icon 2 slug") });
	}
	// achievement2
	{
		Achievement2.AchievementCode = TEXT("UE4AchievementCode2");
		Achievement2.DefaultLanguage = TEXT("id");
		Achievement2.Description.Add(TEXT("en"), TEXT("This is achievement 2 en description"));
		Achievement2.Description.Add(TEXT("id"), TEXT("This is achievement 2 id description"));
		Achievement2.GoalValue = 7;
		Achievement2.Hidden = true;
		Achievement2.Incremental = false;
		Achievement2.LockedIcons.Add({ TEXT("This is locked icon 1 url"), TEXT("This is locked icon 1 slug") });
		Achievement2.LockedIcons.Add({ TEXT("This is locked icon 2 url") , TEXT("This is locked icon 2 slug") });
		Achievement2.Name.Add(TEXT("en"), TEXT("This is achievement 2 en name"));
		Achievement2.Name.Add(TEXT("id"), TEXT("This is achievement 2 id name"));
		Achievement2.StatCode = TEXT("This is achievement 2 stat code");
		Achievement2.Tags.Add(TEXT("Tags1"));
		Achievement2.Tags.Add(TEXT("Tags2"));
		Achievement2.UnlockedIcons.Add({ TEXT("This is unlocked icon 1 url"), TEXT("This is unlocked icon 1 slug") });
		Achievement2.UnlockedIcons.Add({ TEXT("This is unlocked icon 2 url"), TEXT("This is unlocked icon 2 slug") });
	}
	// achievementStatisticClientRequest
	{
		AchievementIncrementalClient.AchievementCode = TEXT("UE4AchievementCode4");
		AchievementIncrementalClient.DefaultLanguage = TEXT("en");
		AchievementIncrementalClient.Description.Add(TEXT("en"), TEXT("This is achievement 4 en description"));
		AchievementIncrementalClient.GoalValue = 100;
		AchievementIncrementalClient.Hidden = true;
		AchievementIncrementalClient.Incremental = true;
		AchievementIncrementalClient.LockedIcons.Add({ TEXT("This is locked icon 4 url"), TEXT("This is locked icon 4 slug") });
		AchievementIncrementalClient.Name.Add(TEXT("en"), TEXT("This is achievement 4 en description"));
		AchievementIncrementalClient.Tags.Add(TEXT("Tags4"));
		AchievementIncrementalClient.UnlockedIcons.Add({ TEXT("This is unlocked icon 4 url"), TEXT("This is unlocked icon 4 slug") });
		AchievementIncrementalClient.StatCode = AchievementStatisticClientRequest.statCode;
	}
	// achievementStatisticServerRequest
	{
		AchievementIncrementalServer.AchievementCode = TEXT("UE4AchievementCode3");
		AchievementIncrementalServer.DefaultLanguage = TEXT("en");
		AchievementIncrementalServer.Description.Add(TEXT("en"), TEXT("This is achievement 3 en description"));
		AchievementIncrementalServer.GoalValue = 100;
		AchievementIncrementalServer.Hidden = true;
		AchievementIncrementalServer.Incremental = true;
		AchievementIncrementalServer.LockedIcons.Add({ TEXT("This is locked icon 3 url"), TEXT("This is locked icon 3 slug") });
		AchievementIncrementalServer.Name.Add(TEXT("en"), TEXT("This is achievement 3 en description"));
		AchievementIncrementalServer.Tags.Add(TEXT("Tags3"));
		AchievementIncrementalServer.UnlockedIcons.Add({ TEXT("This is unlocked icon 3 url"), TEXT("This is unlocked icon 3 slug") });
		AchievementIncrementalServer.StatCode = AchievementStatisticServerRequest.statCode;
	}

	AllStatisticRequests = { AchievementStatisticClientRequest, AchievementStatisticServerRequest };
	AllAchievementRequests = { Achievement1, Achievement2, AchievementIncrementalClient, AchievementIncrementalServer };

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			UE_LOG(LogAccelByteAchievementTest, Log, TEXT("User Login Success"));
			bUserLoginSuccess = true;
		}), AchievementTestErrorHandler);
	Waiting(bUserLoginSuccess, "Waiting for Login ...");

	bool bDeleteUserSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteUserSuccess]()
		{
			UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete user by id success"));
			bDeleteUserSuccess = true;
		}), AchievementTestErrorHandler);
	Waiting(bDeleteUserSuccess, "Waiting for user deletion ...");

	// Login again
	bool bNewUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bNewUserLoginSuccess]()
		{
			UE_LOG(LogAccelByteAchievementTest, Log, TEXT("User Login Success"));
			bNewUserLoginSuccess = true;
		}), AchievementTestErrorHandler);
	Waiting(bNewUserLoginSuccess, "Waiting for Login new user ...");

	// Client Login
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Client login Success"));
			bClientLoginSuccess = true;
		}), AchievementTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");
	AB_TEST_TRUE(bClientLoginSuccess);

	for (const auto& StatisticRequest : AllStatisticRequests)
	{
		bool bCreateStatDone = false;

		// Even though it fails, as long as it exists in the namespace should not be a problem.
		Statistic_Create_Stat(StatisticRequest, THandler<FAccelByteModelsStatInfo>::CreateLambda([&](FAccelByteModelsStatInfo Result)
			{
				bCreateStatDone = true;
			}), FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage) { bCreateStatDone = true; }));
		Waiting(bCreateStatDone, "Waiting for Create statistic code...");

		bool bCreateUserStatItemDone = false;
		FRegistry::Statistic.CreateUserStatItems({ StatisticRequest.statCode }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				bCreateUserStatItemDone = true;
			}), AchievementTestErrorHandler);
		Waiting(bCreateUserStatItemDone, "Waiting for create user stat item...");
	}

	for (const auto& AchievementRequest : AllAchievementRequests)
	{
		bool bDeleteAchievementDone = false;
		Achievement_Delete(AchievementRequest.AchievementCode, FSimpleDelegate::CreateLambda([&bDeleteAchievementDone]()
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete achievement success"));
				bDeleteAchievementDone = true;
			}), FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
				{
					UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Can not eelete achievement. Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					bDeleteAchievementDone = true;
				}));
		Waiting(bDeleteAchievementDone, "Waiting for deleting achievement ...");

		bool bCreateAchievementSuccess = false;
		Achievement_Create(AchievementRequest, THandler<FAchievementResponse>::CreateLambda([&](FAchievementResponse achievement)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Create achievement  success"));
				bCreateAchievementSuccess = true;
			}), AchievementTestErrorHandler);
		Waiting(bCreateAchievementSuccess, "Waiting for creating achievement ...");
		AB_TEST_TRUE(bCreateAchievementSuccess);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryAchievementsDefaultLanguage, "AccelByte.Tests.Achievement.B.QueryAchievementsDefaultLanguage", AutomationFlagMaskAchievement);
bool FAchievementTestQueryAchievementsDefaultLanguage::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::NONE, paginatedAchievements, 0, 100));

	bool bEnAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement1.AchievementCode && paginatedAchievements.Data[i].Name == Achievement1.Name[Achievement1.DefaultLanguage])
		{
			bEnAchievement1Found = true;
			break;
		}
	}
	bool bEnAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement2.AchievementCode && paginatedAchievements.Data[i].Name == Achievement2.Name[Achievement2.DefaultLanguage])
		{
			bEnAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bEnAchievement1Found);
	AB_TEST_TRUE(bEnAchievement2Found);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryAchievementsSpecificLanguage, "AccelByte.Tests.Achievement.B.QueryAchievementsSpecificLanguage", AutomationFlagMaskAchievement);
bool FAchievementTestQueryAchievementsSpecificLanguage::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedEnAchievements;

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT("en"), EAccelByteAchievementListSortBy::NONE, paginatedEnAchievements, 0, 100));

	bool bEnAchievement1Found = false;
	for (int i = 0; i < paginatedEnAchievements.Data.Num(); i++)
	{
		if (paginatedEnAchievements.Data[i].AchievementCode == Achievement1.AchievementCode && paginatedEnAchievements.Data[i].Name == Achievement1.Name[TEXT("en")])
		{
			bEnAchievement1Found = true;
			break;
		}
	}

	bool bEnAchievement2Found = false;
	for (int i = 0; i < paginatedEnAchievements.Data.Num(); i++)
	{
		if (paginatedEnAchievements.Data[i].AchievementCode == Achievement2.AchievementCode && paginatedEnAchievements.Data[i].Name == Achievement2.Name[TEXT("en")])
		{
			bEnAchievement2Found = true;
			break;
		}
	}

	FAccelByteModelsPaginatedPublicAchievement paginatedIdAchievements;

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT("id"), EAccelByteAchievementListSortBy::NONE, paginatedIdAchievements, 0, 100))

		bool bIdAchievement1Found = false;
	for (int i = 0; i < paginatedIdAchievements.Data.Num(); i++)
	{
		if (paginatedIdAchievements.Data[i].AchievementCode == Achievement1.AchievementCode && paginatedIdAchievements.Data[i].Name == Achievement1.Name[Achievement1.DefaultLanguage])
		{
			bIdAchievement1Found = true;
			break;
		}
	}

	bool bIdAchievement2Found = false;
	for (int i = 0; i < paginatedIdAchievements.Data.Num(); i++)
	{
		if (paginatedIdAchievements.Data[i].AchievementCode == Achievement2.AchievementCode && paginatedIdAchievements.Data[i].Name == Achievement2.Name[TEXT("id")])
		{
			bIdAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bEnAchievement1Found);
	AB_TEST_TRUE(bEnAchievement2Found);
	AB_TEST_TRUE(bIdAchievement1Found);
	AB_TEST_TRUE(bIdAchievement2Found);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryAchievementsCreatedAsc, "AccelByte.Tests.Achievement.B.QueryAchievementsCreatedAsc", AutomationFlagMaskAchievement);
bool FAchievementTestQueryAchievementsCreatedAsc::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC, paginatedAchievements, 0, 100));

	int achievement1Order = 0;
	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement1.AchievementCode)
		{
			achievement1Order = i;
			bAchievement1Found = true;
			break;
		}
	}

	int achievement2Order = 0;
	bool bAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement2.AchievementCode)
		{
			achievement2Order = i;
			bAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement2Found);
	AB_TEST_TRUE(achievement1Order < achievement2Order);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryAchievementsCreatedDesc, "AccelByte.Tests.Achievement.B.QueryAchievementsCreatedDesc", AutomationFlagMaskAchievement);
bool FAchievementTestQueryAchievementsCreatedDesc::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_DESC, paginatedAchievements, 0, 100));

	int achievement1Order = 0;
	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement1.AchievementCode)
		{
			achievement1Order = i;
			bAchievement1Found = true;
			break;
		}
	}

	int achievement2Order = 0;
	bool bAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement2.AchievementCode)
		{
			achievement2Order = i;
			bAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement2Found);
	AB_TEST_TRUE(achievement1Order > achievement2Order);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryAchievementsLimit1, "AccelByte.Tests.Achievement.B.QueryAchievementsLimit1", AutomationFlagMaskAchievement);
bool FAchievementTestQueryAchievementsLimit1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::NONE, paginatedAchievements, 0, 1));

	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryAchievementsOffset1, "AccelByte.Tests.Achievement.B.QueryAchievementsOffset1", AutomationFlagMaskAchievement);
bool FAchievementTestQueryAchievementsOffset1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;

	AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC, paginatedAchievements, 1);

	FAccelByteModelsPaginatedPublicAchievement paginatedAchievementsNoOffset;

	AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC, paginatedAchievementsNoOffset);

	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, paginatedAchievementsNoOffset.Data[1].AchievementCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestGetAchievement, "AccelByte.Tests.Achievement.B.GetAchievement", AutomationFlagMaskAchievement);
bool FAchievementTestGetAchievement::RunTest(const FString& Parameters)
{
	FAccelByteModelsMultiLanguageAchievement getAchievement1;

	AB_TEST_TRUE(AchievementTestGetAchievement(Achievement1.AchievementCode, getAchievement1));

	FAccelByteModelsMultiLanguageAchievement getAchievement2;

	AB_TEST_TRUE(AchievementTestGetAchievement(Achievement2.AchievementCode, getAchievement2));

	AB_TEST_EQUAL(getAchievement1.AchievementCode, Achievement1.AchievementCode);
	AB_TEST_EQUAL(getAchievement2.AchievementCode, Achievement2.AchievementCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestGetAchievementInvalidId, "AccelByte.Tests.Achievement.B.GetAchievementInvalidId", AutomationFlagMaskAchievement);
bool FAchievementTestGetAchievementInvalidId::RunTest(const FString& Parameters)
{
	FAccelByteModelsMultiLanguageAchievement getAchievement;

	AB_TEST_FALSE(AchievementTestGetAchievement(TEXT("1fdcacd5-eb94-4e18-a3af-7d8c007a0d89"), getAchievement));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestGetAchievementEmptyId, "AccelByte.Tests.Achievement.B.GetAchievementEmptyId", AutomationFlagMaskAchievement);
bool FAchievementTestGetAchievementEmptyId::RunTest(const FString& Parameters)
{
	FAccelByteModelsMultiLanguageAchievement getAchievement;

	AB_TEST_FALSE(AchievementTestGetAchievement(TEXT(""), getAchievement));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsEmptyData, "AccelByte.Tests.Achievement.B.QueryUserAchievementsEmptyData", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsEmptyData::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::NONE, paginatedAchievements));

	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestUnlockAchievement, "AccelByte.Tests.Achievement.C.UnlockAchievement", AutomationFlagMaskAchievement);
bool FAchievementTestUnlockAchievement::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	FRegistry::Achievement.UnlockAchievement(Achievement1.AchievementCode,
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess]()
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock achievement 1 success"));
				bUnlockAchievementSuccess = true;
			}), AchievementTestErrorHandler);
	Waiting(bUnlockAchievementSuccess, "Waiting for unlocking achievement 1 ...");

	AB_TEST_TRUE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievements, "AccelByte.Tests.Achievement.D.QueryUserAchievements", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievements::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::NONE, paginatedAchievements));

	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_TRUE(paginatedAchievements.Data[0].AchievementCode == Achievement1.AchievementCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievement, "AccelByte.Tests.Achievement.E.ServerUnlockAchievement", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievement::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(AchievementTestServerUnlockAchievement(FRegistry::Credentials.GetUserId(), Achievement2.AchievementCode));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievementInvalidUserId, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementInvalidUserId", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievementInvalidUserId::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = AchievementTestServerUnlockAchievement(TEXT("INVALID"), Achievement2.AchievementCode);

	// TODO: Uncomment below if the user id validation is fixed on the backend.
	//AB_TEST_FALSE(bUnlockAchievementSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievementEmptyUserId, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementEmptyUserId", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievementEmptyUserId::RunTest(const FString& Parameters)
{
	AB_TEST_FALSE(AchievementTestServerUnlockAchievement(TEXT(""), Achievement2.AchievementCode));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievementInvalidCode, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementInvalidCode", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievementInvalidCode::RunTest(const FString& Parameters)
{
	AB_TEST_FALSE(AchievementTestServerUnlockAchievement(FRegistry::Credentials.GetUserId(), TEXT("INVALID")));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockEmptyAchievementCode, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementEmptyCode", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockEmptyAchievementCode::RunTest(const FString& Parameters)
{
	AB_TEST_FALSE(AchievementTestServerUnlockAchievement(FRegistry::Credentials.GetUserId(), TEXT("")));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsUnlockedAll, "AccelByte.Tests.Achievement.F.QueryUserAchievementsUnlockedAll", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsUnlockedAll::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::NONE, paginatedAchievements));

	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement1.AchievementCode)
		{
			bAchievement1Found = true;
			break;
		}
	}

	bool bAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == Achievement2.AchievementCode)
		{
			bAchievement2Found = true;
			break;
		}
	}

	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 2);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement2Found);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsOffset1, "AccelByte.Tests.Achievement.F.QueryUserAchievementsOffset1", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsOffset1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::CREATED_AT_ASC, paginatedAchievements, 1));

	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, Achievement2.AchievementCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsLimit1, "AccelByte.Tests.Achievement.F.QueryUserAchievementsLimit1", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsLimit1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::CREATED_AT_ASC, paginatedAchievements, 0, 1));

	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, Achievement1.AchievementCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestUnlockIncrementalAchievementClientUpdateStat, "AccelByte.Tests.Achievement.G.UnlockIncrementalAchievementClientUpdateStat", AutomationFlagMaskAchievement);
bool FAchievementTestUnlockIncrementalAchievementClientUpdateStat::RunTest(const FString& Parameters)
{
	// Arrange
	FString currentStatCode = AchievementIncrementalClient.StatCode;
	FAchievementRequest currentAchievementRequest = AchievementIncrementalClient;
	bool bGetUserStatItemSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult getUserStatItemResult;
	FRegistry::Statistic.GetUserStatItems({ currentStatCode }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
		{
			bGetUserStatItemSuccess = true;
			getUserStatItemResult = Result;
		}), AchievementTestErrorHandler);
	Waiting(bGetUserStatItemSuccess, "Waiting for get user stat item achievement ...");
	for (auto entry : getUserStatItemResult.Data)
	{
		if (entry.StatCode == currentStatCode)
		{
			AB_TEST_EQUAL(entry.Value, 0.0f);
		}
	}

	// Act
	FAccelByteModelsBulkStatItemInc UpdateStat;
	UpdateStat.inc = currentAchievementRequest.GoalValue;
	UpdateStat.statCode = currentStatCode;
	bool bIncrementUserStatDone = false;
	FRegistry::Statistic.IncrementUserStatItems({ UpdateStat }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](TArray<FAccelByteModelsBulkStatItemOperationResult> Result)
		{
			bIncrementUserStatDone = true;
		}), AchievementTestErrorHandler);
	Waiting(bIncrementUserStatDone, "Waiting for increment user stat item ...");

	WaitUntil([]() { return false; }, 10.0f, "Waiting for KAFKA");

	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::NONE, paginatedAchievements));

	// Assert
	bool bIncrementalAchievementUnlocked = false;
	for (auto userAchievement : paginatedAchievements.Data)
	{
		if (userAchievement.AchievementCode == currentAchievementRequest.AchievementCode)
		{
			bIncrementalAchievementUnlocked = true;
		}
	}

	AB_TEST_TRUE(bIncrementalAchievementUnlocked);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestUnlockIncrementalAchievementServerUpdateStat, "AccelByte.Tests.Achievement.G.UnlockIncrementalAchievementServerUpdateStat", AutomationFlagMaskAchievement);
bool FAchievementTestUnlockIncrementalAchievementServerUpdateStat::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
		{
			bClientLoginSuccess = true;
		}), AchievementTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

	// Arrange
	FString currentStatCode = AchievementIncrementalServer.StatCode;
	FAchievementRequest currentAchievementRequest = AchievementIncrementalServer;
	bool bGetUserStatItemSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult getUserStatItemResult;
	FRegistry::Statistic.GetUserStatItems({ currentStatCode }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
		{
			bGetUserStatItemSuccess = true;
			getUserStatItemResult = Result;
		}), AchievementTestErrorHandler);
	Waiting(bGetUserStatItemSuccess, "Waiting for get user stat item achievement ...");
	for (auto entry : getUserStatItemResult.Data)
	{
		if (entry.StatCode == currentStatCode)
		{
			AB_TEST_EQUAL(entry.Value, 0.0f);
		}
	}

	// Act
	bool bIncrementStatDone = false;
	TArray<FAccelByteModelsBulkStatItemOperationResult> IncrementResult;
	FRegistry::ServerStatistic.IncrementUserStatItems(
		FRegistry::Credentials.GetUserId(),
		{ { static_cast<float>(currentAchievementRequest.GoalValue), currentStatCode } },
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				bIncrementStatDone = true;
			}),
		AchievementTestErrorHandler);
	Waiting(bIncrementStatDone, "Waiting for increment user stat items...");

	WaitUntil([]() { return false; }, 10.0f, "Waiting for KAFKA");

	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;

	AB_TEST_TRUE(AchievementTestQueryUserAchievements(EAccelByteAchievementListSortBy::NONE, paginatedAchievements));

	// Assert
	bool bIncrementalAchievementUnlocked = false;
	for (auto userAchievement : paginatedAchievements.Data)
	{
		if (userAchievement.AchievementCode == currentAchievementRequest.AchievementCode)
		{
			bIncrementalAchievementUnlocked = true;
		}
	}

	AB_TEST_TRUE(bIncrementalAchievementUnlocked);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestAchievementTearDown, "AccelByte.Tests.Achievement.Z.TearDown", AutomationFlagMaskAchievement);
bool FAchievementTestAchievementTearDown::RunTest(const FString& Parameters)
{
	TArray<bool> deleteAchievementResults;
	for (auto achievementRequest : AllAchievementRequests)
	{
		bool bDeleteAchievementSuccess = false;
		Achievement_Delete(achievementRequest.AchievementCode, FSimpleDelegate::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete achievement success"));
				bDeleteAchievementSuccess = true;
				deleteAchievementResults.Add(true);
			}), AchievementTestErrorHandler);
		Waiting(bDeleteAchievementSuccess, "Waiting for deleting achievement ...");
	}

	bool bDeleteSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteSuccess]()
		{
			UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete user by id success"));
			bDeleteSuccess = true;
		}), AchievementTestErrorHandler);
	Waiting(bDeleteSuccess, "Waiting for user deletion...");

	for (auto bDeleteAchievementSuccess : deleteAchievementResults)
	{
		AB_TEST_TRUE(bDeleteAchievementSuccess);
	}

	AB_TEST_TRUE(bDeleteSuccess);

	return true;
}
