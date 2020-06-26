// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteLeaderboardApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLeaderboardTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLeaderboardTest);

const int32 AutomationFlagMaskLeaderboard = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto LeaderboardTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteLeaderboardTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const FString TestStatCodePrefix = "atestleaderboardstatcode";
const int TestStatCodeCount = 2;
TArray<FString> TestStatCodes;
TArray<FString> TestLeaderboardCodes;

struct LeaderboardTestUserInfo
{
	const FString EmailPrefix = "Leaderboard_UE4Test";
	const FString EmailSuffix = "@example.com";
	const FString Password = "Password+123";
	const FString Country = "US";
	const FString DisplayName = "LeaderboadMan";
	const int UserCount = 4;
};
const LeaderboardTestUserInfo LeaderboardTestUserInfo_;

TArray<FString> TestUsersID;

const TArray<EAccelByteLeaderboardTimeFrame> AllLeaderboardTimeFrame
{
	EAccelByteLeaderboardTimeFrame::ALL_TIME,
	EAccelByteLeaderboardTimeFrame::CURRENT_SEASON,
	EAccelByteLeaderboardTimeFrame::CURRENT_MONTH,
	EAccelByteLeaderboardTimeFrame::CURRENT_WEEK,
	EAccelByteLeaderboardTimeFrame::TODAY
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LeaderboardSetup, "AccelByte.Tests.Leaderboard.A.Setup", AutomationFlagMaskLeaderboard);
bool LeaderboardSetup::RunTest(const FString& Parameters)
{
	TArray<bool> UsersCreationSuccess;
	UsersCreationSuccess.Init(false, LeaderboardTestUserInfo_.UserCount);

	TArray<bool> UsersLoginSuccess;
	UsersLoginSuccess.Init(false, LeaderboardTestUserInfo_.UserCount);

	TArray<FString> TestEmails;
	TestEmails.Init("", LeaderboardTestUserInfo_.UserCount);

	TArray<bool> StatCodeInitSuccess;
	StatCodeInitSuccess.Init(false, TestStatCodeCount);

	TArray<bool> LeaderboardConfigSuccess;
	LeaderboardConfigSuccess.Init(false, TestStatCodeCount);

	TArray<bool> UserStatCodeInitSuccess;
	UserStatCodeInitSuccess.Init(false, LeaderboardTestUserInfo_.UserCount);

	bool bServerClientLoginSuccess = false;

	TestStatCodes.Reset();
	// STAT CODE CREATION
	for (size_t i = 0; i < TestStatCodeCount; i++)
	{
		UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("STAT CODE CREATION...%d out of %d"), i + 1, TestStatCodeCount);

		// (a-z) lower case and (0-9) only
		FString StatisticStatCode = FString::Printf(TEXT("%s%d"), *TestStatCodePrefix, i);
		TestStatCodes.Add(StatisticStatCode);

		bool bStatIsExist = false;
		bool bGetStatDone = false;

		Statistic_Get_Stat_By_StatCode(StatisticStatCode, THandler<FAccelByteModelsStatInfo>::CreateLambda([&i, &StatCodeInitSuccess, &bStatIsExist, &bGetStatDone](const FAccelByteModelsStatInfo& Result)
			{
				StatCodeInitSuccess[i] = true;
				bStatIsExist = true;
				bGetStatDone = true;
			}), FErrorHandler::CreateLambda([&bGetStatDone](int32 ErrorCode, FString ErrorMessage)
				{
					if ((ErrorCodes)ErrorCode != ErrorCodes::StatisticConfigNotFoundException)
					{
						UE_LOG(LogAccelByteLeaderboardTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					}
					else
					{
						UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
			createStat.description = "StatCode for Leaderboard UE4 SDK Test purpose";
			createStat.incrementOnly = true;
			createStat.maximum = 999999;
			createStat.minimum = 0;
			createStat.name = StatisticStatCode;
			createStat.setAsGlobal = true;
			createStat.setBy = EAccelByteStatisticSetBy::SERVER;
			createStat.statCode = StatisticStatCode;
			createStat.tags = { StatisticStatCode };

			Statistic_Create_Stat(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&i, &StatCodeInitSuccess, &CreateStatResult, &bCreateStatDone](FAccelByteModelsStatInfo Result)
				{
					StatCodeInitSuccess[i] = true;
					CreateStatResult = Result;
					bCreateStatDone = true;
				}), LeaderboardTestErrorHandler);
			FlushHttpRequests();
			Waiting(bCreateStatDone, "Waiting for stat created...");
		}
	}

	// LEADERBOARD CONFIG CREATION
	for (size_t i = 0; i < TestStatCodeCount; i++)
	{
		UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("LEADERBOARD CONFIG CREATION...%d out of %d"), i + 1, TestStatCodeCount);
		FLeaderboardConfigRequest createRequest;
		createRequest.leaderboardCode = TestStatCodes[i];
		createRequest.name = TestStatCodes[i];
		createRequest.statCode = TestStatCodes[i];
		createRequest.seasonPeriod = 31 + 5; // minimum is 31 day

		//Intentional: start time supposed to be in the future. 10 seconds added. It will takes effect in 10 seconds.
		createRequest.startTime = (FDateTime::UtcNow() + FTimespan::FromSeconds(10)).ToIso8601();

		TestLeaderboardCodes.Add(createRequest.leaderboardCode);

		bool bCreateLeaderboardConfigDone = false;
		Leaderboard_Create_Leaderboard(createRequest, THandler<FLeaderboardConfigRequest>::CreateLambda([&](FLeaderboardConfigRequest result)
			{
				LeaderboardConfigSuccess[i] = true;
				bCreateLeaderboardConfigDone = true;
			}), FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
				{
					if ((ErrorCodes)ErrorCode == ErrorCodes::LeaderboardConfigAlreadyExist)
					{
						LeaderboardConfigSuccess[i] = true;
						bCreateLeaderboardConfigDone = true;
					}
					else
					{
						bCreateLeaderboardConfigDone = true;
						UE_LOG(LogAccelByteLeaderboardTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					}
				}));
		FlushHttpRequests();
		Waiting(bCreateLeaderboardConfigDone, "Waiting for config leaderboard...");
	}

	// SERVER LOGIN
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bServerClientLoginSuccess]()
		{
			bServerClientLoginSuccess = true;
			UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("\t\tClient Successfully Login."));
		}), LeaderboardTestErrorHandler);
	FlushHttpRequests();
	Waiting(bServerClientLoginSuccess, "Waiting for Client Login...");

	TArray<FString> LoginUserIDs;
	for (int i = 0; i < LeaderboardTestUserInfo_.UserCount; i++)
	{
		// USER CREATION
		UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("USER CREATION...%d out of %d"), i + 1, LeaderboardTestUserInfo_.UserCount);

		TestEmails[i] = FString::Printf(TEXT("%s+%d%s"), *LeaderboardTestUserInfo_.EmailPrefix, i, *LeaderboardTestUserInfo_.EmailSuffix);
		TestEmails[i].ToLowerInline();

		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7));
		const FString DoB_String = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		bool bUsersCreationDone = false;
		FRegistry::User.Register(TestEmails[i], LeaderboardTestUserInfo_.Password, LeaderboardTestUserInfo_.DisplayName, LeaderboardTestUserInfo_.Country, DoB_String, THandler<FRegisterResponse>::CreateLambda([&i, &UsersCreationSuccess, &bUsersCreationDone](const FRegisterResponse& Response)
		{
			UsersCreationSuccess[i] = true;
			UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Test User is successfuly created."));
			bUsersCreationDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Test User is already created."));
			}
			else
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Test User can't be created"));
			}
			bUsersCreationDone = true;
		}));
		FlushHttpRequests();
		Waiting(bUsersCreationDone, "Waiting for user created...");

		// USER LOG IN
		UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("LOG USER IN...%d out of %d"), i + 1, LeaderboardTestUserInfo_.UserCount);

		FRegistry::User.LoginWithUsername(
			TestEmails[i],
			LeaderboardTestUserInfo_.Password,
			FVoidHandler::CreateLambda([&i, &UsersLoginSuccess, &LoginUserIDs]()
		{
			LoginUserIDs.Add(FRegistry::Credentials.GetUserId());
			UsersLoginSuccess[i] = true;
			UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("\t\tSuccessfully Login."));
		}), LeaderboardTestErrorHandler);
		FlushHttpRequests();
		Waiting(UsersLoginSuccess[i], "Waiting for login with user name...");
		UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

		// USER's STAT ITEM CREATION
		UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("CREATE USER STAT ITEM...%d out of %d USER..."), i + 1, LeaderboardTestUserInfo_.UserCount);

		bool bCreateStatSuccess = false;
		FRegistry::ServerStatistic.CreateUserStatItems(FRegistry::Credentials.GetUserId(), TestStatCodes, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&i, &UserStatCodeInitSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("User stat items are created."));
				UserStatCodeInitSuccess[i] = true;
			}), LeaderboardTestErrorHandler);
		FlushHttpRequests();
		Waiting(UserStatCodeInitSuccess[i], "Waiting for create user stat items...");
	}
	TestUsersID = LoginUserIDs;

	// ASSERTION
	for (int i = 0; i < LeaderboardTestUserInfo_.UserCount; i++)
	{
		check(UsersCreationSuccess[i]);
		check(UsersLoginSuccess[i]);
		check(UserStatCodeInitSuccess[i]);
	}
	check(bServerClientLoginSuccess);
	for (size_t i = 0; i < TestStatCodeCount; i++)
	{
		check(StatCodeInitSuccess[i]);
		check(LeaderboardConfigSuccess[i]);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LeaderboardTearDown, "AccelByte.Tests.Leaderboard.Z.Teardown", AutomationFlagMaskLeaderboard);
bool LeaderboardTearDown::RunTest(const FString& Parameters)
{
	// DELETE LEADERBOARD CODE
	for (auto& leaderboardCode : TestLeaderboardCodes)
	{
		bool bDeleteLeaderboard = false;

		Leaderboard_Delete_Leaderboard(leaderboardCode, FSimpleDelegate::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Success"));
				bDeleteLeaderboard = true;
			}), LeaderboardTestErrorHandler);
		FlushHttpRequests();
		Waiting(bDeleteLeaderboard, "Waiting for leaderboard code deletion...");
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LeaderboardGetRankings, "AccelByte.Tests.Leaderboard.B.GetRankings", AutomationFlagMaskLeaderboard);
bool LeaderboardGetRankings::RunTest(const FString& Parameters)
{
	//ARRANGE

	/* create an expected learboard results */
	const int USER_ID_WINNER_INDEX = 0;
	const float WINNER_SCORE = 3.0f;
	const float COMMONER_SCORE = 1.f;

	TArray<FAccelByteModelsBulkUserStatItemInc> request;
	
	for (auto& statCode : TestStatCodes)
	{
		for (int i = 0; i < LeaderboardTestUserInfo_.UserCount; i++)
		{
			float increment = (i == USER_ID_WINNER_INDEX) ? WINNER_SCORE : COMMONER_SCORE;
			request.Add({ increment, TestUsersID[i], statCode });
		}
	}

	bool bIncrementManyUsersStatItemsSuccess = false;
	FRegistry::ServerStatistic.IncrementManyUsersStatItems(
		request,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("IncrementManyUsersStatItems SUCCESS!"));
				bIncrementManyUsersStatItemsSuccess = true;
			}),
		LeaderboardTestErrorHandler);
	FlushHttpRequests();
	Waiting(bIncrementManyUsersStatItemsSuccess, "Waiting for IncrementManyUsersStatItems...");


	//ACT
	TArray<TArray<TTuple<FString, FAccelByteModelsLeaderboardRankingResult>>> AllLeaderboardTestResults;

	for (auto timeFrame : AllLeaderboardTimeFrame)
	{
		TArray<TTuple<FString, FAccelByteModelsLeaderboardRankingResult>> ThisTimeFrameResults;

		for (auto& leaderboardCode : TestLeaderboardCodes)
		{
			bool bGetRankingDone = false;
			THandler<FAccelByteModelsLeaderboardRankingResult> leaderboardGetRankingsHandler = THandler<FAccelByteModelsLeaderboardRankingResult>::CreateLambda([&](const FAccelByteModelsLeaderboardRankingResult& result)
			{
				if (result.data.Num() != 0)
				{
					ThisTimeFrameResults.Add(TTuple<FString, FAccelByteModelsLeaderboardRankingResult>{leaderboardCode, result });
					FRegistry::Leaderboard.GetRankings(leaderboardCode, timeFrame, result.data.Num(), 9999, leaderboardGetRankingsHandler, LeaderboardTestErrorHandler);
				}
				else
				{
					bGetRankingDone = true;
				}
			});
			FRegistry::Leaderboard.GetRankings(leaderboardCode, timeFrame, 0, 9999, leaderboardGetRankingsHandler, LeaderboardTestErrorHandler);
			FlushHttpRequests();
			Waiting(bGetRankingDone, "Waiting for get rankings...");
		}

		AllLeaderboardTestResults.Add(ThisTimeFrameResults);
	}

	//ASSERT
	/// TestUsersID[USER_ID_WINNER_INDEX] should be in the first place for all time frame and for all leaderboard code

	check(AllLeaderboardTestResults.Num() == AllLeaderboardTimeFrame.Num())
	for (auto& resultInATimeFrame : AllLeaderboardTestResults)
	{
		check(resultInATimeFrame.Num() == TestLeaderboardCodes.Num())
		for (auto& resultForEachLeaderboardCode : resultInATimeFrame)
		{
			check(TestUsersID.Num() <= resultForEachLeaderboardCode.Value.data.Num())

			TArray<FString> commonerUserIDs;
			for (auto& testUser : TestUsersID)
			{
				if (testUser != TestUsersID[USER_ID_WINNER_INDEX])
				{
					commonerUserIDs.Add(testUser);
				}
			}
			
			bool bWinnerCorrect = false;
			TArray<FString> commonerFoundResults;
			for (auto& player : resultForEachLeaderboardCode.Value.data)
			{
				if (player.userId == TestUsersID[USER_ID_WINNER_INDEX])
				{
					bWinnerCorrect = true;
				}
				else
				{
					commonerFoundResults.Add(player.userId);
				}
			}

			check(bWinnerCorrect)
			for (auto& userId : commonerUserIDs)
			{
				check(commonerFoundResults.Contains(userId))
			}
		}
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LeaderboardGetUserRanking, "AccelByte.Tests.Leaderboard.C.GetUserRankings", AutomationFlagMaskLeaderboard);
bool LeaderboardGetUserRanking::RunTest(const FString& Parameters)
{
	//ARRANGE

	/* create an expected learboard results */
	const int USER_ID_WINNER_INDEX = 0;
	const float WINNER_SCORE = 3.0f;
	const float COMMONER_SCORE = 1.f;

	TArray<FAccelByteModelsBulkUserStatItemInc> request;

	for (auto& statCode : TestStatCodes)
	{
		for (auto& userId : TestUsersID)
		{
			float increment = (userId == TestUsersID[USER_ID_WINNER_INDEX]) ? WINNER_SCORE : COMMONER_SCORE;
			request.Add({ increment, userId, statCode });
		}
	}

	bool bIncrementManyUsersStatItemsSuccess = false;
	FRegistry::ServerStatistic.IncrementManyUsersStatItems(
		request,
		THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("IncrementManyUsersStatItems SUCCESS!"));
				bIncrementManyUsersStatItemsSuccess = true;
			}),
		LeaderboardTestErrorHandler);
	FlushHttpRequests();
	Waiting(bIncrementManyUsersStatItemsSuccess, "Waiting for IncrementManyUsersStatItems...");

	//ACT
	/// Currently the logged in user is the last user in the TestUsersID
	/// It will test wether it can GET his/her user ranking and GET other user's ranking

	TArray<TArray<FAccelByteModelsUserRankingData>> allGetUserRankingResult;
	for (auto& userId : TestUsersID)
	{
		TArray<FAccelByteModelsUserRankingData> currentUserRanking;
		for (auto& leaderboardCode : TestLeaderboardCodes)
		{
			bool bGetUserRankingDone = false;
			FRegistry::Leaderboard.GetUserRanking(userId, leaderboardCode, THandler<FAccelByteModelsUserRankingData>::CreateLambda([&](const FAccelByteModelsUserRankingData& Result)
				{
					bGetUserRankingDone = true;
					currentUserRanking.Add(Result);
				}), LeaderboardTestErrorHandler);
			FlushHttpRequests();
			Waiting(bGetUserRankingDone, "Waiting for GetUserRanking...");
		}
		allGetUserRankingResult.Add(currentUserRanking);
	}

	//ASSERT
	for (auto& allLeaderboardCodeRankingResult : allGetUserRankingResult)
	{
		for (auto& userRankingResult : allLeaderboardCodeRankingResult)
		{
			if (userRankingResult.userId == TestUsersID[USER_ID_WINNER_INDEX])
			{
				check(userRankingResult.allTime.rank == 1)
				check(userRankingResult.current.rank == 1)
			}
			else
			{
				check(userRankingResult.allTime.rank != 1)
				check(userRankingResult.current.rank != 1)
			}
		}
	}
	return true;
}
