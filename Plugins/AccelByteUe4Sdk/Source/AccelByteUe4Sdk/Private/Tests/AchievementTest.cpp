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

const int32 AutomationFlagMaskAchievement = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

FAchievementRequest achievement1;
FAchievementRequest achievement2;
FAchievementRequest achievementIncrementalClient;
FAchievementRequest achievementIncrementalServer;
FStatCreateRequest achievementStatisticClientRequest;
FStatCreateRequest achievementStatisticServerRequest;
TArray<FStatCreateRequest> allStatisticRequest;
TArray<FAchievementRequest> allAchievementRequests;

const auto AchievementErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteAchievementTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AchievementSetup, "AccelByte.Tests.Achievement.A.Setup", AutomationFlagMaskAchievement);
bool AchievementSetup::RunTest(const FString& Parameters)
{
	// statistic client
	{
		achievementStatisticClientRequest.defaultValue = 0;
		achievementStatisticClientRequest.description = "StatCode for SDK Test purpose";
		achievementStatisticClientRequest.incrementOnly = true;
		achievementStatisticClientRequest.maximum = 999999;
		achievementStatisticClientRequest.minimum = 0;
		achievementStatisticClientRequest.name = "Achievement Incremental Testing Client";
		achievementStatisticClientRequest.setAsGlobal = false;
		achievementStatisticClientRequest.setBy = EAccelByteStatisticSetBy::CLIENT;
		achievementStatisticClientRequest.statCode = "incremental-testing-client";
		achievementStatisticClientRequest.tags = { "nothing" };
	}
	// statistic server
	{
		achievementStatisticServerRequest.defaultValue = 0;
		achievementStatisticServerRequest.description = "StatCode for SDK Test purpose";
		achievementStatisticServerRequest.incrementOnly = true;
		achievementStatisticServerRequest.maximum = 999999;
		achievementStatisticServerRequest.minimum = 0;
		achievementStatisticServerRequest.name = "Achievement Incremental Testing Server";
		achievementStatisticServerRequest.setAsGlobal = false;
		achievementStatisticServerRequest.setBy = EAccelByteStatisticSetBy::SERVER;
		achievementStatisticServerRequest.statCode = "incremental-testing-server";
		achievementStatisticServerRequest.tags = { "nothing" };
	}

	// achievement1
	{
		achievement1.AchievementCode = TEXT("UE4AchievementCode1");
		achievement1.DefaultLanguage = TEXT("en");
		achievement1.Description.Add(TEXT("en"), TEXT("This is achievement 1 en description"));
		achievement1.GoalValue = 7;
		achievement1.Hidden = true;
		achievement1.Incremental = false;
		achievement1.LockedIcons.Add({ TEXT("This is locked icon 1 url"), TEXT("This is locked icon 1 slug")});
		achievement1.LockedIcons.Add({ TEXT("This is locked icon 2 url"), TEXT("This is locked icon 2 slug")});
		achievement1.Name.Add(TEXT("en"), TEXT("This is achievement 1 en description"));
		achievement1.StatCode = TEXT("This is achievement 1 stat code");
		achievement1.Tags.Add(TEXT("Tags1"));
		achievement1.Tags.Add(TEXT("Tags2"));
		achievement1.UnlockedIcons.Add({ TEXT("This is unlocked icon 1 url"), TEXT("This is unlocked icon 1 slug")});
		achievement1.UnlockedIcons.Add({ TEXT("This is unlocked icon 2 url"), TEXT("This is unlocked icon 2 slug")});
	}
	// achievement2
	{
		achievement2.AchievementCode = TEXT("UE4AchievementCode2");
		achievement2.DefaultLanguage = TEXT("id");
		achievement2.Description.Add(TEXT("en"), TEXT("This is achievement 2 en description"));
		achievement2.Description.Add(TEXT("id"), TEXT("This is achievement 2 id description"));
		achievement2.GoalValue = 7;
		achievement2.Hidden = true;
		achievement2.Incremental = false;
		achievement2.LockedIcons.Add({ TEXT("This is locked icon 1 url"), TEXT("This is locked icon 1 slug") });
		achievement2.LockedIcons.Add({ TEXT("This is locked icon 2 url") , TEXT("This is locked icon 2 slug") });
		achievement2.Name.Add(TEXT("en"), TEXT("This is achievement 2 en name"));
		achievement2.Name.Add(TEXT("id"), TEXT("This is achievement 2 id name"));
		achievement2.StatCode = TEXT("This is achievement 2 stat code");
		achievement2.Tags.Add(TEXT("Tags1"));
		achievement2.Tags.Add(TEXT("Tags2"));
		achievement2.UnlockedIcons.Add({ TEXT("This is unlocked icon 1 url"), TEXT("This is unlocked icon 1 slug") });
		achievement2.UnlockedIcons.Add({ TEXT("This is unlocked icon 2 url"), TEXT("This is unlocked icon 2 slug") });
	}
	// achievementStatisticClientRequest
	{
		achievementIncrementalClient.AchievementCode = TEXT("UE4AchievementCode4");
		achievementIncrementalClient.DefaultLanguage = TEXT("en");
		achievementIncrementalClient.Description.Add(TEXT("en"), TEXT("This is achievement 4 en description"));
		achievementIncrementalClient.GoalValue = 100;
		achievementIncrementalClient.Hidden = true;
		achievementIncrementalClient.Incremental = true;
		achievementIncrementalClient.LockedIcons.Add({ TEXT("This is locked icon 4 url"), TEXT("This is locked icon 4 slug") });
		achievementIncrementalClient.Name.Add(TEXT("en"), TEXT("This is achievement 4 en description"));
		achievementIncrementalClient.Tags.Add(TEXT("Tags4"));
		achievementIncrementalClient.UnlockedIcons.Add({ TEXT("This is unlocked icon 4 url"), TEXT("This is unlocked icon 4 slug") });
		achievementIncrementalClient.StatCode = achievementStatisticClientRequest.statCode;
	}
	// achievementStatisticServerRequest
	{
		achievementIncrementalServer.AchievementCode = TEXT("UE4AchievementCode3");
		achievementIncrementalServer.DefaultLanguage = TEXT("en");
		achievementIncrementalServer.Description.Add(TEXT("en"), TEXT("This is achievement 3 en description"));
		achievementIncrementalServer.GoalValue = 100;
		achievementIncrementalServer.Hidden = true;
		achievementIncrementalServer.Incremental = true;
		achievementIncrementalServer.LockedIcons.Add({ TEXT("This is locked icon 3 url"), TEXT("This is locked icon 3 slug") });
		achievementIncrementalServer.Name.Add(TEXT("en"), TEXT("This is achievement 3 en description"));
		achievementIncrementalServer.Tags.Add(TEXT("Tags3"));
		achievementIncrementalServer.UnlockedIcons.Add({ TEXT("This is unlocked icon 3 url"), TEXT("This is unlocked icon 3 slug") });
		achievementIncrementalServer.StatCode = achievementStatisticServerRequest.statCode;
	}

	allStatisticRequest = { achievementStatisticClientRequest, achievementStatisticServerRequest };
	allAchievementRequests = { achievement1, achievement2, achievementIncrementalClient, achievementIncrementalServer };

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("User Login Success"));
		bUserLoginSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bUserLoginSuccess, "Waiting for Login ...");
	
	bool bDeleteUserSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteUserSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete user by id success"));
		bDeleteUserSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bDeleteUserSuccess, "Waiting for user deletion ...");

	// Login again
	bool bNewUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bNewUserLoginSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("User Login Success"));
		bNewUserLoginSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bNewUserLoginSuccess, "Waiting for Login new user ...");

	// Client Login
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Client login Success"));
			bClientLoginSuccess = true;
		}), AchievementErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");
	AB_TEST_TRUE(bClientLoginSuccess);

	for (auto statisticRequest : allStatisticRequest)
	{
		bool bCreateStatDone = false;

		// Even though it fails, as long as it exists in the namespace should not be a problem.
		Statistic_Create_Stat(statisticRequest, THandler<FAccelByteModelsStatInfo>::CreateLambda([&](FAccelByteModelsStatInfo Result)
		{
			bCreateStatDone = true;
		}), FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage) { bCreateStatDone = true; }));
		Waiting(bCreateStatDone, "Waiting for Create statistic code...");

		bool bCreateUserStatItemDone = false;
		FRegistry::Statistic.CreateUserStatItems({ statisticRequest.statCode }, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				bCreateUserStatItemDone = true;
			}), AchievementErrorHandler);
		Waiting(bCreateUserStatItemDone, "Waiting for create user stat item...");
	}

	for (auto achievementRequest : allAchievementRequests)
	{
		bool bDeleteAchievementDone = false;
		Achievement_Delete(achievementRequest.AchievementCode, FSimpleDelegate::CreateLambda([&bDeleteAchievementDone]()
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
		Achievement_Create(achievementRequest, THandler<FAchievementResponse>::CreateLambda([&](FAchievementResponse achievement)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Create achievement  success"));
				bCreateAchievementSuccess = true;
			}), AchievementErrorHandler);
		Waiting(bCreateAchievementSuccess, "Waiting for creating achievement ...");
		AB_TEST_TRUE(bCreateAchievementSuccess);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryNamespaceAchievementsDefaultLanguageCreatedAsc, "AccelByte.Tests.Achievement.B.QueryNamespaceAchievementsDefaultLanguage", AutomationFlagMaskAchievement);
bool QueryNamespaceAchievementsDefaultLanguageCreatedAsc::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 100);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying achievements ...");

	bool bEnAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == achievement1.AchievementCode && paginatedAchievements.Data[i].Name == achievement1.Name[achievement1.DefaultLanguage])
		{
			bEnAchievement1Found = true;
			break;
		}
	}
	bool bEnAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == achievement2.AchievementCode && paginatedAchievements.Data[i].Name == achievement2.Name[achievement2.DefaultLanguage])
		{
			bEnAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_TRUE(bEnAchievement1Found);
	AB_TEST_TRUE(bEnAchievement2Found);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryNamespaceAchievementsSpecificLanguage, "AccelByte.Tests.Achievement.B.QueryNamespaceAchievementsSpecificLanguage", AutomationFlagMaskAchievement);
bool QueryNamespaceAchievementsSpecificLanguage::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedEnAchievements;
	bool bQueryEnAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT("en"), EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryEnAchievementsSuccess, &paginatedEnAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query en achievements success"));
		paginatedEnAchievements = achievements;
		bQueryEnAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 100);
	Waiting(bQueryEnAchievementsSuccess, "Waiting for querying en achievements ...");

	bool bEnAchievement1Found = false;
	for (int i = 0; i < paginatedEnAchievements.Data.Num(); i++)
	{
		if (paginatedEnAchievements.Data[i].AchievementCode == achievement1.AchievementCode && paginatedEnAchievements.Data[i].Name == achievement1.Name[TEXT("en")])
		{
			bEnAchievement1Found = true;
			break;
		}
	}

	bool bEnAchievement2Found = false;
	for (int i = 0; i < paginatedEnAchievements.Data.Num(); i++)
	{
		if (paginatedEnAchievements.Data[i].AchievementCode == achievement2.AchievementCode && paginatedEnAchievements.Data[i].Name == achievement2.Name[TEXT("en")])
		{
			bEnAchievement2Found = true;
			break;
		}
	}

	FAccelByteModelsPaginatedPublicAchievement paginatedIdAchievements;
	bool bQueryIdAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT("id"), EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryIdAchievementsSuccess, &paginatedIdAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query id achievements success"));
		paginatedIdAchievements = achievements;
		bQueryIdAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 100);
	Waiting(bQueryIdAchievementsSuccess, "Waiting for querying id achievements ...");

	bool bIdAchievement1Found = false;
	for (int i = 0; i < paginatedIdAchievements.Data.Num(); i++)
	{
		if (paginatedIdAchievements.Data[i].AchievementCode == achievement1.AchievementCode && paginatedIdAchievements.Data[i].Name == achievement1.Name[achievement1.DefaultLanguage])
		{
			bIdAchievement1Found = true;
				break;
		}
	}

	bool bIdAchievement2Found = false;
	for (int i = 0; i < paginatedIdAchievements.Data.Num(); i++)
	{
		if (paginatedIdAchievements.Data[i].AchievementCode == achievement2.AchievementCode && paginatedIdAchievements.Data[i].Name == achievement2.Name[TEXT("id")])
		{
			bIdAchievement2Found = true;
				break;
		}
	}

	AB_TEST_TRUE(bQueryEnAchievementsSuccess);
	AB_TEST_TRUE(bEnAchievement1Found);
	AB_TEST_TRUE(bEnAchievement2Found);
	AB_TEST_TRUE(bQueryIdAchievementsSuccess);
	AB_TEST_TRUE(bIdAchievement1Found);
	AB_TEST_TRUE(bIdAchievement2Found);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryNamespaceAchievementsCreatedAsc, "AccelByte.Tests.Achievement.B.QueryNamespaceAchievementsCreatedAsc", AutomationFlagMaskAchievement);
bool QueryNamespaceAchievementsCreatedAsc::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 100);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying achievements ...");

	int achievement1Order = 0;
	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == achievement1.AchievementCode)
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
		if (paginatedAchievements.Data[i].AchievementCode == achievement2.AchievementCode)
		{
			achievement2Order = i;
			bAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement2Found);
	AB_TEST_TRUE(achievement1Order < achievement2Order);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryNamespaceAchievementsCreatedDesc, "AccelByte.Tests.Achievement.B.QueryNamespaceAchievementsCreatedDesc", AutomationFlagMaskAchievement);
bool QueryNamespaceAchievementsCreatedDesc::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_DESC,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 100);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying achievements ...");

	int achievement1Order = 0;
	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == achievement1.AchievementCode)
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
		if (paginatedAchievements.Data[i].AchievementCode == achievement2.AchievementCode)
		{
			achievement2Order = i;
			bAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement2Found);
	AB_TEST_TRUE(achievement1Order > achievement2Order);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryNamespaceAchievementsLimit1, "AccelByte.Tests.Achievement.B.QueryNamespaceAchievementsLimit1", AutomationFlagMaskAchievement);
bool QueryNamespaceAchievementsLimit1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 1);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryNamespaceAchievementsOffset1, "AccelByte.Tests.Achievement.B.QueryNamespaceAchievementsOffset1", AutomationFlagMaskAchievement);
bool QueryNamespaceAchievementsOffset1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedPublicAchievement paginatedAchievementsNoOffset;
	bool bQueryAchievementsNoOffsetSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryAchievementsNoOffsetSuccess, &paginatedAchievementsNoOffset](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query achievements no offest success"));
		paginatedAchievementsNoOffset = achievements;
		bQueryAchievementsNoOffsetSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bQueryAchievementsNoOffsetSuccess, "Waiting for querying achievements no offset ...");

	FAccelByteModelsPaginatedPublicAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedPublicAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 1);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying achievements ...");

	bool bAchievementSame = false;
	if (paginatedAchievements.Data[0].AchievementCode == paginatedAchievementsNoOffset.Data[1].AchievementCode)
	{
		bAchievementSame = true;
	}

	AB_TEST_TRUE(bQueryAchievementsNoOffsetSuccess);
	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_TRUE(bAchievementSame);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetAchievement, "AccelByte.Tests.Achievement.B.GetAchievement", AutomationFlagMaskAchievement);
bool GetAchievement::RunTest(const FString& Parameters)
{
	FAccelByteModelsMultiLanguageAchievement getAchievement1;
	bool bGetAchievement1Success = false;
	FRegistry::Achievement.GetAchievement(achievement1.AchievementCode,
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&bGetAchievement1Success, &getAchievement1](FAccelByteModelsMultiLanguageAchievement achievement)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Get achievement 1 success"));
		getAchievement1 = achievement;
		bGetAchievement1Success = true;
	}), AchievementErrorHandler);
	Waiting(bGetAchievement1Success, "Waiting for getting achievement 1 ...");

	FAccelByteModelsMultiLanguageAchievement getAchievement2;
	bool bGetAchievement2Success = false;
	FRegistry::Achievement.GetAchievement(achievement2.AchievementCode,
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&bGetAchievement2Success, &getAchievement2](FAccelByteModelsMultiLanguageAchievement achievement)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Get achievement 2 success"));
		getAchievement2 = achievement;
		bGetAchievement2Success = true;
	}), AchievementErrorHandler);
	Waiting(bGetAchievement2Success, "Waiting for getting achievement 2 ...");

	AB_TEST_TRUE(bGetAchievement1Success);
	AB_TEST_EQUAL(getAchievement1.AchievementCode, achievement1.AchievementCode);
	AB_TEST_TRUE(bGetAchievement2Success);
	AB_TEST_EQUAL(getAchievement2.AchievementCode, achievement2.AchievementCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetInvalidAchievementId, "AccelByte.Tests.Achievement.B.GetInvalidAchievementId", AutomationFlagMaskAchievement);
bool GetInvalidAchievementId::RunTest(const FString& Parameters)
{
	FAccelByteModelsMultiLanguageAchievement getAchievement;
	bool bGetAchievementSuccess = false;
	bool bGetAchievementDone = false;
	FRegistry::Achievement.GetAchievement(TEXT("1fdcacd5-eb94-4e18-a3af-7d8c007a0d89"),
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&bGetAchievementSuccess, &bGetAchievementDone, &getAchievement](FAccelByteModelsMultiLanguageAchievement achievement)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Get invalid achievement success"));
		getAchievement = achievement;
		bGetAchievementSuccess = true;
		bGetAchievementDone = true;
	}), FErrorHandler::CreateLambda([&bGetAchievementDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bGetAchievementDone = true;
	}));
	Waiting(bGetAchievementDone, "Waiting for getting invalid achievement ...");

	AB_TEST_FALSE(bGetAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetEmptyAchievementId, "AccelByte.Tests.Achievement.B.GetEmptyAchievementId", AutomationFlagMaskAchievement);
bool GetEmptyAchievementId::RunTest(const FString& Parameters)
{
	bool bGetAchievementSuccess = false;
	bool bGetAchievementDone= false;
	FRegistry::Achievement.GetAchievement(TEXT(""),
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&bGetAchievementSuccess, &bGetAchievementDone](FAccelByteModelsMultiLanguageAchievement achievement)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Get achievement success"));
		bGetAchievementSuccess = true;
		bGetAchievementDone = true;
	}), FErrorHandler::CreateLambda([&bGetAchievementDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bGetAchievementDone = true;
	}));
	Waiting(bGetAchievementDone, "Waiting for getting empty achievement id ...");

	AB_TEST_FALSE(bGetAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryUserAchievementsEmptyData, "AccelByte.Tests.Achievement.B.QueryUserAchievementsEmptyData", AutomationFlagMaskAchievement);
bool QueryUserAchievementsEmptyData::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UnlockAchievement, "AccelByte.Tests.Achievement.C.UnlockAchievement", AutomationFlagMaskAchievement);
bool UnlockAchievement::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	FRegistry::Achievement.UnlockAchievement(achievement1.AchievementCode,
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock achievement 1 success"));
		bUnlockAchievementSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bUnlockAchievementSuccess, "Waiting for unlocking achievement 1 ...");
	
	AB_TEST_TRUE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryUserAchievements, "AccelByte.Tests.Achievement.D.QueryUserAchievements", AutomationFlagMaskAchievement);
bool QueryUserAchievements::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	bool bAchievement1Found = false;
	if (paginatedAchievements.Data[0].AchievementCode == achievement1.AchievementCode)
	{
		bAchievement1Found = true;
	}

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_TRUE(bAchievement1Found);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerUnlockAchievement, "AccelByte.Tests.Achievement.E.ServerUnlockAchievement", AutomationFlagMaskAchievement);
bool ServerUnlockAchievement::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	FRegistry::ServerAchievement.UnlockAchievement(FRegistry::Credentials.GetUserId(), achievement2.AchievementCode,
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock achievement 1 success"));
		bUnlockAchievementSuccess = true;
	}), AchievementErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUnlockAchievementSuccess, "Waiting for unlocking achievement 1 ...");

	AB_TEST_TRUE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerUnlockAchievementInvalidUserId, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementInvalidUserId", AutomationFlagMaskAchievement);
bool ServerUnlockAchievementInvalidUserId::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	bool bUnlockAchievementDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(TEXT("Invalid"), achievement2.AchievementCode,
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess, &bUnlockAchievementDone]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock invalid achievement success"));
		bUnlockAchievementSuccess = true;
		bUnlockAchievementDone = true;
	}), FErrorHandler::CreateLambda([&bUnlockAchievementDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bUnlockAchievementDone = true;
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUnlockAchievementDone, "Waiting for invalid achievement ...");

	// TODO: Uncomment below if the user id validation is fixed on the backend.
	//AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerUnlockAchievementEmptyUserId, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementEmptyUserId", AutomationFlagMaskAchievement);
bool ServerUnlockAchievementEmptyUserId::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	bool bUnlockAchievementDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(TEXT(""), achievement2.AchievementCode,
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess, &bUnlockAchievementDone]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock empty user id achievement success"));
		bUnlockAchievementSuccess = true;
		bUnlockAchievementDone = true;
	}), FErrorHandler::CreateLambda([&bUnlockAchievementDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bUnlockAchievementDone = true;
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUnlockAchievementDone, "Waiting for empty user id achievement ...");

	AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerUnlockInvalidAchievementCode, "AccelByte.Tests.Achievement.E.ServerUnlockInvalidAchievementCode", AutomationFlagMaskAchievement);
bool ServerUnlockInvalidAchievementCode::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	bool bUnlockAchievementDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(FRegistry::Credentials.GetUserId(), TEXT("Invalid"),
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess, &bUnlockAchievementDone]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock invalid achievement success"));
		bUnlockAchievementSuccess = true;
		bUnlockAchievementDone = true;
	}), FErrorHandler::CreateLambda([&bUnlockAchievementDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bUnlockAchievementDone = true;
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUnlockAchievementDone, "Waiting for invalid achievement ...");

	AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerUnlockEmptyAchievementCode, "AccelByte.Tests.Achievement.E.ServerUnlockEmptyAchievementCode", AutomationFlagMaskAchievement);
bool ServerUnlockEmptyAchievementCode::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	bool bUnlockAchievementDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(FRegistry::Credentials.GetUserId(), TEXT(""),
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess, &bUnlockAchievementDone]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock invalid achievement success"));
		bUnlockAchievementSuccess = true;
		bUnlockAchievementDone = true;
	}), FErrorHandler::CreateLambda([&bUnlockAchievementDone](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
		bUnlockAchievementDone = true;
	}));
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUnlockAchievementDone, "Waiting for invalid achievement ...");

	AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryUserAchievementsUnlockedAll, "AccelByte.Tests.Achievement.F.QueryUserAchievementsUnlockedAll", AutomationFlagMaskAchievement);
bool QueryUserAchievementsUnlockedAll::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == achievement1.AchievementCode)
		{
			bAchievement1Found = true;
			break;
		}
	}

	bool bAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == achievement2.AchievementCode)
		{
			bAchievement2Found = true;
			break;
		}
	}

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 2);
	AB_TEST_TRUE(bAchievement1Found);
	AB_TEST_TRUE(bAchievement2Found);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryUserAchievementsOffset1, "AccelByte.Tests.Achievement.F.QueryUserAchievementsOffset1", AutomationFlagMaskAchievement);
bool QueryUserAchievementsOffset1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::CREATED_AT_ASC,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 1);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, achievement2.AchievementCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(QueryUserAchievementsLimit1, "AccelByte.Tests.Achievement.F.QueryUserAchievementsLimit1", AutomationFlagMaskAchievement);
bool QueryUserAchievementsLimit1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementErrorHandler, 0, 1);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, achievement1.AchievementCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UnlockIncrementalAchievement_ClientUpdateStat, "AccelByte.Tests.Achievement.G.UnlockIncrementalAchievement_ClientUpdateStat", AutomationFlagMaskAchievement);
bool UnlockIncrementalAchievement_ClientUpdateStat::RunTest(const FString& Parameters)
{
	// Arrange
	FString currentStatCode = achievementIncrementalClient.StatCode;
	FAchievementRequest currentAchievementRequest = achievementIncrementalClient;
	bool bGetUserStatItemSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult getUserStatItemResult;
	FRegistry::Statistic.GetUserStatItems({ currentStatCode }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
		{
			bGetUserStatItemSuccess = true;
			getUserStatItemResult = Result;
		}), AchievementErrorHandler);
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
		}), AchievementErrorHandler);
	Waiting(bIncrementUserStatDone, "Waiting for increment user stat item ...");

	WaitUntil([]() { return false; }, 10.0f, "Waiting for KAFKA");

	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
				paginatedAchievements = achievements;
				bQueryAchievementsSuccess = true;
			}), AchievementErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UnlockIncrementalAchievement_ServerUpdateStat, "AccelByte.Tests.Achievement.G.UnlockIncrementalAchievement_ServerUpdateStat", AutomationFlagMaskAchievement);
bool UnlockIncrementalAchievement_ServerUpdateStat::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
		{
			bClientLoginSuccess = true;
		}), AchievementErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

	// Arrange
	FString currentStatCode = achievementIncrementalServer.StatCode;
	FAchievementRequest currentAchievementRequest = achievementIncrementalServer;
	bool bGetUserStatItemSuccess = false;
	FAccelByteModelsUserStatItemPagingSlicedResult getUserStatItemResult;
	FRegistry::Statistic.GetUserStatItems({ currentStatCode }, {}, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
		{
			bGetUserStatItemSuccess = true;
			getUserStatItemResult = Result;
		}), AchievementErrorHandler);
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
		AchievementErrorHandler);
	Waiting(bIncrementStatDone, "Waiting for increment user stat items...");

	WaitUntil([]() { return false; }, 10.0f, "Waiting for KAFKA");

	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
				paginatedAchievements = achievements;
				bQueryAchievementsSuccess = true;
			}), AchievementErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AchievementTearDown, "AccelByte.Tests.Achievement.Z.TearDown", AutomationFlagMaskAchievement);
bool AchievementTearDown::RunTest(const FString& Parameters)
{
	TArray<bool> deleteAchievementResults;
	for (auto achievementRequest : allAchievementRequests)
	{
		bool bDeleteAchievementSuccess = false;
		Achievement_Delete(achievementRequest.AchievementCode, FSimpleDelegate::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete achievement success"));
				bDeleteAchievementSuccess = true;
				deleteAchievementResults.Add(true);
			}), AchievementErrorHandler);
		Waiting(bDeleteAchievementSuccess, "Waiting for deleting achievement ...");
	}

	bool bDeleteSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Delete user by id success"));
		bDeleteSuccess = true;
	}), AchievementErrorHandler);
	Waiting(bDeleteSuccess, "Waiting for user deletion...");

	for(auto bDeleteAchievementSuccess : deleteAchievementResults)
	{
		AB_TEST_TRUE(bDeleteAchievementSuccess);
	}
	AB_TEST_TRUE(bDeleteSuccess);
	return true;
}
