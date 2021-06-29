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

const auto AchievementTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteAchievementTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

FAchievementRequest AchievementTestAchievement1;
FAchievementRequest AchievementTestAchievement2;
FAchievementRequest AchievementTestAchievementIncrementalClient;
FAchievementRequest AchievementTestAchievementIncrementalServer;
FStatCreateRequest AchievementTestAchievementStatisticClientRequest;
FStatCreateRequest AchievementTestAchievementStatisticServerRequest;
TArray<FStatCreateRequest> AchievementTestAllStatisticRequests;
TArray<FAchievementRequest> AchievementTestAllAchievementRequests;

const auto AchievementTestQueryAchievements = [](const FString& InLanguage, const EAccelByteAchievementListSortBy& InSortBy, FAccelByteModelsPaginatedPublicAchievement& PaginatedPublicAchievement, const int32& InOffset = 0, const int32& InLimit = 20) // XXX InOffset and InLimit defaults must match with QueryAchievements defaults
{
	bool bIsOk = false;
	UE_LOG(LogAccelByteTest, Log, TEXT("%s"), TEXT("Querying achievements"));
	FRegistry::Achievement.QueryAchievements(InLanguage, InSortBy,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([&](FAccelByteModelsPaginatedPublicAchievement Result)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("OK"));
				PaginatedPublicAchievement = Result;
				bIsOk = true;
			}),
		AchievementTestErrorHandler, InOffset, InLimit);
	Waiting(bIsOk, "Waiting ...");
	return bIsOk;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestSetup, "AccelByte.Tests.Achievement.A.Setup", AutomationFlagMaskAchievement);
bool FAchievementTestSetup::RunTest(const FString& Parameters)
{
	// statistic client
	{
		AchievementTestAchievementStatisticClientRequest.defaultValue = 0;
		AchievementTestAchievementStatisticClientRequest.description = "StatCode for SDK Test purpose";
		AchievementTestAchievementStatisticClientRequest.incrementOnly = true;
		AchievementTestAchievementStatisticClientRequest.maximum = 999999;
		AchievementTestAchievementStatisticClientRequest.minimum = 0;
		AchievementTestAchievementStatisticClientRequest.name = "Achievement Incremental Testing Client";
		AchievementTestAchievementStatisticClientRequest.setAsGlobal = false;
		AchievementTestAchievementStatisticClientRequest.setBy = EAccelByteStatisticSetBy::CLIENT;
		AchievementTestAchievementStatisticClientRequest.statCode = "incremental-testing-client";
		AchievementTestAchievementStatisticClientRequest.tags = { "nothing" };
	}
	// statistic server
	{
		AchievementTestAchievementStatisticServerRequest.defaultValue = 0;
		AchievementTestAchievementStatisticServerRequest.description = "StatCode for SDK Test purpose";
		AchievementTestAchievementStatisticServerRequest.incrementOnly = true;
		AchievementTestAchievementStatisticServerRequest.maximum = 999999;
		AchievementTestAchievementStatisticServerRequest.minimum = 0;
		AchievementTestAchievementStatisticServerRequest.name = "Achievement Incremental Testing Server";
		AchievementTestAchievementStatisticServerRequest.setAsGlobal = false;
		AchievementTestAchievementStatisticServerRequest.setBy = EAccelByteStatisticSetBy::SERVER;
		AchievementTestAchievementStatisticServerRequest.statCode = "incremental-testing-server";
		AchievementTestAchievementStatisticServerRequest.tags = { "nothing" };
	}

	// achievement1
	{
		AchievementTestAchievement1.AchievementCode = TEXT("UE4AchievementCode1");
		AchievementTestAchievement1.DefaultLanguage = TEXT("en");
		AchievementTestAchievement1.Description.Add(TEXT("en"), TEXT("This is achievement 1 en description"));
		AchievementTestAchievement1.GoalValue = 7;
		AchievementTestAchievement1.Hidden = true;
		AchievementTestAchievement1.Incremental = false;
		AchievementTestAchievement1.LockedIcons.Add({ TEXT("This is locked icon 1 url"), TEXT("This is locked icon 1 slug")});
		AchievementTestAchievement1.LockedIcons.Add({ TEXT("This is locked icon 2 url"), TEXT("This is locked icon 2 slug")});
		AchievementTestAchievement1.Name.Add(TEXT("en"), TEXT("This is achievement 1 en description"));
		AchievementTestAchievement1.StatCode = TEXT("This is achievement 1 stat code");
		AchievementTestAchievement1.Tags.Add(TEXT("Tags1"));
		AchievementTestAchievement1.Tags.Add(TEXT("Tags2"));
		AchievementTestAchievement1.UnlockedIcons.Add({ TEXT("This is unlocked icon 1 url"), TEXT("This is unlocked icon 1 slug")});
		AchievementTestAchievement1.UnlockedIcons.Add({ TEXT("This is unlocked icon 2 url"), TEXT("This is unlocked icon 2 slug")});
	}
	// achievement2
	{
		AchievementTestAchievement2.AchievementCode = TEXT("UE4AchievementCode2");
		AchievementTestAchievement2.DefaultLanguage = TEXT("id");
		AchievementTestAchievement2.Description.Add(TEXT("en"), TEXT("This is achievement 2 en description"));
		AchievementTestAchievement2.Description.Add(TEXT("id"), TEXT("This is achievement 2 id description"));
		AchievementTestAchievement2.GoalValue = 7;
		AchievementTestAchievement2.Hidden = true;
		AchievementTestAchievement2.Incremental = false;
		AchievementTestAchievement2.LockedIcons.Add({ TEXT("This is locked icon 1 url"), TEXT("This is locked icon 1 slug") });
		AchievementTestAchievement2.LockedIcons.Add({ TEXT("This is locked icon 2 url") , TEXT("This is locked icon 2 slug") });
		AchievementTestAchievement2.Name.Add(TEXT("en"), TEXT("This is achievement 2 en name"));
		AchievementTestAchievement2.Name.Add(TEXT("id"), TEXT("This is achievement 2 id name"));
		AchievementTestAchievement2.StatCode = TEXT("This is achievement 2 stat code");
		AchievementTestAchievement2.Tags.Add(TEXT("Tags1"));
		AchievementTestAchievement2.Tags.Add(TEXT("Tags2"));
		AchievementTestAchievement2.UnlockedIcons.Add({ TEXT("This is unlocked icon 1 url"), TEXT("This is unlocked icon 1 slug") });
		AchievementTestAchievement2.UnlockedIcons.Add({ TEXT("This is unlocked icon 2 url"), TEXT("This is unlocked icon 2 slug") });
	}
	// achievementStatisticClientRequest
	{
		AchievementTestAchievementIncrementalClient.AchievementCode = TEXT("UE4AchievementCode4");
		AchievementTestAchievementIncrementalClient.DefaultLanguage = TEXT("en");
		AchievementTestAchievementIncrementalClient.Description.Add(TEXT("en"), TEXT("This is achievement 4 en description"));
		AchievementTestAchievementIncrementalClient.GoalValue = 100;
		AchievementTestAchievementIncrementalClient.Hidden = true;
		AchievementTestAchievementIncrementalClient.Incremental = true;
		AchievementTestAchievementIncrementalClient.LockedIcons.Add({ TEXT("This is locked icon 4 url"), TEXT("This is locked icon 4 slug") });
		AchievementTestAchievementIncrementalClient.Name.Add(TEXT("en"), TEXT("This is achievement 4 en description"));
		AchievementTestAchievementIncrementalClient.Tags.Add(TEXT("Tags4"));
		AchievementTestAchievementIncrementalClient.UnlockedIcons.Add({ TEXT("This is unlocked icon 4 url"), TEXT("This is unlocked icon 4 slug") });
		AchievementTestAchievementIncrementalClient.StatCode = AchievementTestAchievementStatisticClientRequest.statCode;
	}
	// achievementStatisticServerRequest
	{
		AchievementTestAchievementIncrementalServer.AchievementCode = TEXT("UE4AchievementCode3");
		AchievementTestAchievementIncrementalServer.DefaultLanguage = TEXT("en");
		AchievementTestAchievementIncrementalServer.Description.Add(TEXT("en"), TEXT("This is achievement 3 en description"));
		AchievementTestAchievementIncrementalServer.GoalValue = 100;
		AchievementTestAchievementIncrementalServer.Hidden = true;
		AchievementTestAchievementIncrementalServer.Incremental = true;
		AchievementTestAchievementIncrementalServer.LockedIcons.Add({ TEXT("This is locked icon 3 url"), TEXT("This is locked icon 3 slug") });
		AchievementTestAchievementIncrementalServer.Name.Add(TEXT("en"), TEXT("This is achievement 3 en description"));
		AchievementTestAchievementIncrementalServer.Tags.Add(TEXT("Tags3"));
		AchievementTestAchievementIncrementalServer.UnlockedIcons.Add({ TEXT("This is unlocked icon 3 url"), TEXT("This is unlocked icon 3 slug") });
		AchievementTestAchievementIncrementalServer.StatCode = AchievementTestAchievementStatisticServerRequest.statCode;
	}

	AchievementTestAllStatisticRequests = { AchievementTestAchievementStatisticClientRequest, AchievementTestAchievementStatisticServerRequest };
	AchievementTestAllAchievementRequests = { AchievementTestAchievement1, AchievementTestAchievement2, AchievementTestAchievementIncrementalClient, AchievementTestAchievementIncrementalServer };

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

	for (auto statisticRequest : AchievementTestAllStatisticRequests)
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
			}), AchievementTestErrorHandler);
		Waiting(bCreateUserStatItemDone, "Waiting for create user stat item...");
	}

	for (auto achievementRequest : AchievementTestAllAchievementRequests)
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

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::NONE, paginatedAchievements, 0, 100))

	bool bEnAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement1.AchievementCode && paginatedAchievements.Data[i].Name == AchievementTestAchievement1.Name[AchievementTestAchievement1.DefaultLanguage])
		{
			bEnAchievement1Found = true;
			break;
		}
	}
	bool bEnAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement2.AchievementCode && paginatedAchievements.Data[i].Name == AchievementTestAchievement2.Name[AchievementTestAchievement2.DefaultLanguage])
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

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT("en"), EAccelByteAchievementListSortBy::NONE, paginatedEnAchievements, 0, 100))

	bool bEnAchievement1Found = false;
	for (int i = 0; i < paginatedEnAchievements.Data.Num(); i++)
	{
		if (paginatedEnAchievements.Data[i].AchievementCode == AchievementTestAchievement1.AchievementCode && paginatedEnAchievements.Data[i].Name == AchievementTestAchievement1.Name[TEXT("en")])
		{
			bEnAchievement1Found = true;
			break;
		}
	}

	bool bEnAchievement2Found = false;
	for (int i = 0; i < paginatedEnAchievements.Data.Num(); i++)
	{
		if (paginatedEnAchievements.Data[i].AchievementCode == AchievementTestAchievement2.AchievementCode && paginatedEnAchievements.Data[i].Name == AchievementTestAchievement2.Name[TEXT("en")])
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
		if (paginatedIdAchievements.Data[i].AchievementCode == AchievementTestAchievement1.AchievementCode && paginatedIdAchievements.Data[i].Name == AchievementTestAchievement1.Name[AchievementTestAchievement1.DefaultLanguage])
		{
			bIdAchievement1Found = true;
				break;
		}
	}

	bool bIdAchievement2Found = false;
	for (int i = 0; i < paginatedIdAchievements.Data.Num(); i++)
	{
		if (paginatedIdAchievements.Data[i].AchievementCode == AchievementTestAchievement2.AchievementCode && paginatedIdAchievements.Data[i].Name == AchievementTestAchievement2.Name[TEXT("id")])
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

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_ASC, paginatedAchievements, 0, 100))

	int achievement1Order = 0;
	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement1.AchievementCode)
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
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement2.AchievementCode)
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

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::CREATED_AT_DESC, paginatedAchievements, 0, 100))

	int achievement1Order = 0;
	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement1.AchievementCode)
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
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement2.AchievementCode)
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

	AB_TEST_TRUE(AchievementTestQueryAchievements(TEXT(""), EAccelByteAchievementListSortBy::NONE, paginatedAchievements, 0, 1))

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
	bool bGetAchievement1Success = false;
	FRegistry::Achievement.GetAchievement(AchievementTestAchievement1.AchievementCode,
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&bGetAchievement1Success, &getAchievement1](FAccelByteModelsMultiLanguageAchievement achievement)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Get achievement 1 success"));
		getAchievement1 = achievement;
		bGetAchievement1Success = true;
	}), AchievementTestErrorHandler);
	Waiting(bGetAchievement1Success, "Waiting for getting achievement 1 ...");

	FAccelByteModelsMultiLanguageAchievement getAchievement2;
	bool bGetAchievement2Success = false;
	FRegistry::Achievement.GetAchievement(AchievementTestAchievement2.AchievementCode,
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([&bGetAchievement2Success, &getAchievement2](FAccelByteModelsMultiLanguageAchievement achievement)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Get achievement 2 success"));
		getAchievement2 = achievement;
		bGetAchievement2Success = true;
	}), AchievementTestErrorHandler);
	Waiting(bGetAchievement2Success, "Waiting for getting achievement 2 ...");

	AB_TEST_TRUE(bGetAchievement1Success);
	AB_TEST_EQUAL(getAchievement1.AchievementCode, AchievementTestAchievement1.AchievementCode);
	AB_TEST_TRUE(bGetAchievement2Success);
	AB_TEST_EQUAL(getAchievement2.AchievementCode, AchievementTestAchievement2.AchievementCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestGetAchievementInvalidId, "AccelByte.Tests.Achievement.B.GetAchievementInvalidId", AutomationFlagMaskAchievement);
bool FAchievementTestGetAchievementInvalidId::RunTest(const FString& Parameters)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestGetAchievementEmptyId, "AccelByte.Tests.Achievement.B.GetAchievementEmptyId", AutomationFlagMaskAchievement);
bool FAchievementTestGetAchievementEmptyId::RunTest(const FString& Parameters)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsEmptyData, "AccelByte.Tests.Achievement.B.QueryUserAchievementsEmptyData", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsEmptyData::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementTestErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestUnlockAchievement, "AccelByte.Tests.Achievement.C.UnlockAchievement", AutomationFlagMaskAchievement);
bool FAchievementTestUnlockAchievement::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	FRegistry::Achievement.UnlockAchievement(AchievementTestAchievement1.AchievementCode,
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
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementTestErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	bool bAchievement1Found = false;
	if (paginatedAchievements.Data[0].AchievementCode == AchievementTestAchievement1.AchievementCode)
	{
		bAchievement1Found = true;
	}

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_TRUE(bAchievement1Found);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievement, "AccelByte.Tests.Achievement.E.ServerUnlockAchievement", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievement::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	FRegistry::ServerAchievement.UnlockAchievement(FRegistry::Credentials.GetUserId(), AchievementTestAchievement2.AchievementCode,
		FVoidHandler::CreateLambda([&bUnlockAchievementSuccess]()
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Unlock achievement 1 success"));
		bUnlockAchievementSuccess = true;
	}), AchievementTestErrorHandler);
	Waiting(bUnlockAchievementSuccess, "Waiting for unlocking achievement 1 ...");

	AB_TEST_TRUE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievementInvalidUserId, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementInvalidUserId", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievementInvalidUserId::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	bool bUnlockAchievementDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(TEXT("Invalid"), AchievementTestAchievement2.AchievementCode,
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
	Waiting(bUnlockAchievementDone, "Waiting for invalid achievement ...");

	// TODO: Uncomment below if the user id validation is fixed on the backend.
	//AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievementEmptyUserId, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementEmptyUserId", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievementEmptyUserId::RunTest(const FString& Parameters)
{
	bool bUnlockAchievementSuccess = false;
	bool bUnlockAchievementDone = false;
	FRegistry::ServerAchievement.UnlockAchievement(TEXT(""), AchievementTestAchievement2.AchievementCode,
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
	Waiting(bUnlockAchievementDone, "Waiting for empty user id achievement ...");

	AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockAchievementInvalidCode, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementInvalidCode", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockAchievementInvalidCode::RunTest(const FString& Parameters)
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
	Waiting(bUnlockAchievementDone, "Waiting for invalid achievement ...");

	AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestServerUnlockEmptyAchievementCode, "AccelByte.Tests.Achievement.E.ServerUnlockAchievementEmptyCode", AutomationFlagMaskAchievement);
bool FAchievementTestServerUnlockEmptyAchievementCode::RunTest(const FString& Parameters)
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
	Waiting(bUnlockAchievementDone, "Waiting for invalid achievement ...");

	AB_TEST_FALSE(bUnlockAchievementSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsUnlockedAll, "AccelByte.Tests.Achievement.F.QueryUserAchievementsUnlockedAll", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsUnlockedAll::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementTestErrorHandler);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	bool bAchievement1Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement1.AchievementCode)
		{
			bAchievement1Found = true;
			break;
		}
	}

	bool bAchievement2Found = false;
	for (int i = 0; i < paginatedAchievements.Data.Num(); i++)
	{
		if (paginatedAchievements.Data[i].AchievementCode == AchievementTestAchievement2.AchievementCode)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsOffset1, "AccelByte.Tests.Achievement.F.QueryUserAchievementsOffset1", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsOffset1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::CREATED_AT_ASC,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementTestErrorHandler, 1);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, AchievementTestAchievement2.AchievementCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestQueryUserAchievementsLimit1, "AccelByte.Tests.Achievement.F.QueryUserAchievementsLimit1", AutomationFlagMaskAchievement);
bool FAchievementTestQueryUserAchievementsLimit1::RunTest(const FString& Parameters)
{
	FAccelByteModelsPaginatedUserAchievement paginatedAchievements;
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
	{
		UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
		paginatedAchievements = achievements;
		bQueryAchievementsSuccess = true;
	}), AchievementTestErrorHandler, 0, 1);
	Waiting(bQueryAchievementsSuccess, "Waiting for querying user achievements ...");

	AB_TEST_TRUE(bQueryAchievementsSuccess);
	AB_TEST_EQUAL(paginatedAchievements.Data.Num(), 1);
	AB_TEST_EQUAL(paginatedAchievements.Data[0].AchievementCode, AchievementTestAchievement1.AchievementCode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestUnlockIncrementalAchievementClientUpdateStat, "AccelByte.Tests.Achievement.G.UnlockIncrementalAchievementClientUpdateStat", AutomationFlagMaskAchievement);
bool FAchievementTestUnlockIncrementalAchievementClientUpdateStat::RunTest(const FString& Parameters)
{
	// Arrange
	FString currentStatCode = AchievementTestAchievementIncrementalClient.StatCode;
	FAchievementRequest currentAchievementRequest = AchievementTestAchievementIncrementalClient;
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
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
				paginatedAchievements = achievements;
				bQueryAchievementsSuccess = true;
			}), AchievementTestErrorHandler);
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
	FString currentStatCode = AchievementTestAchievementIncrementalServer.StatCode;
	FAchievementRequest currentAchievementRequest = AchievementTestAchievementIncrementalServer;
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
	bool bQueryAchievementsSuccess = false;
	FRegistry::Achievement.QueryUserAchievements(EAccelByteAchievementListSortBy::NONE,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([&bQueryAchievementsSuccess, &paginatedAchievements](FAccelByteModelsPaginatedUserAchievement achievements)
			{
				UE_LOG(LogAccelByteAchievementTest, Log, TEXT("Query user achievements success"));
				paginatedAchievements = achievements;
				bQueryAchievementsSuccess = true;
			}), AchievementTestErrorHandler);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAchievementTestAchievementTearDown, "AccelByte.Tests.Achievement.Z.TearDown", AutomationFlagMaskAchievement);
bool FAchievementTestAchievementTearDown::RunTest(const FString& Parameters)
{
	TArray<bool> deleteAchievementResults;
	for (auto achievementRequest : AchievementTestAllAchievementRequests)
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

	for(auto bDeleteAchievementSuccess : deleteAchievementResults)
	{
		AB_TEST_TRUE(bDeleteAchievementSuccess);
	}
	AB_TEST_TRUE(bDeleteSuccess);
	return true;
}
