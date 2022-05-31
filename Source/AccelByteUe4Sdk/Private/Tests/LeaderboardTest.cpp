// Copyright (c) 2020-2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "HAL/FileManager.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteLeaderboardApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "LeaderboardTestAdmin.h"
#include "StatisticTestAdmin.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLeaderboardTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLeaderboardTest);

const int32 AutomationFlagMaskLeaderboard = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto LeaderboardTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteLeaderboardTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

BEGIN_DEFINE_SPEC(FLeaderboardTestSpec, "AccelByte.Tests.Leaderboard", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	const FString TestStatCodePrefix = "atestleaderboardstatcode";
	const int TestStatCodeCount = 2;
	FDateTime LeaderboardStartTime;
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
END_DEFINE_SPEC(FLeaderboardTestSpec)

void FLeaderboardTestSpec::Define()
{
	Describe("A_Setup", [this]()
	{
		It("should create leaderboard stat code", [this]()
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

				AdminGetStatisticByStatCode(StatisticStatCode, THandler<FAccelByteModelsStatInfo>::CreateLambda([&i, &StatCodeInitSuccess, &bStatIsExist, &bGetStatDone](const FAccelByteModelsStatInfo& Result)
				{
					StatCodeInitSuccess[i] = true;
					bStatIsExist = true;
					bGetStatDone = true;
				}),
				FErrorHandler::CreateLambda([&bGetStatDone](int32 ErrorCode, FString ErrorMessage)
				{
					if ((ErrorCodes)ErrorCode != ErrorCodes::StatisticConfigNotFoundException)
					{
						UE_LOG(LogAccelByteLeaderboardTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					}
					else
					{
						UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
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
					createStat.description = "StatCode for Leaderboard UE4 SDK Test purpose";
					createStat.incrementOnly = true;
					createStat.maximum = 999999;
					createStat.minimum = 0;
					createStat.name = StatisticStatCode;
					createStat.setAsGlobal = true;
					createStat.setBy = EAccelByteStatisticSetBy::SERVER;
					createStat.statCode = StatisticStatCode;
					createStat.tags = { StatisticStatCode };

					AdminCreateStatistic(createStat, THandler<FAccelByteModelsStatInfo>::CreateLambda([&i, &StatCodeInitSuccess, &CreateStatResult, &bCreateStatDone](FAccelByteModelsStatInfo Result)
					{
						StatCodeInitSuccess[i] = true;
						CreateStatResult = Result;
						bCreateStatDone = true;
					}), LeaderboardTestErrorHandler);
					WaitUntil(bCreateStatDone, "Waiting for stat created...");
				}
			}

			// LEADERBOARD CONFIG CREATION
			double DelayedTime = 10.0f;
			for (size_t i = 0; i < TestStatCodeCount; i++)
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("LEADERBOARD CONFIG CREATION...%d out of %d"), i + 1, TestStatCodeCount);
				FLeaderboardConfigRequest createRequest;
				createRequest.leaderboardCode = TestStatCodes[i];
				createRequest.name = TestStatCodes[i];
				createRequest.statCode = TestStatCodes[i];
				createRequest.seasonPeriod = 31 + 5; // minimum is 31 day

				//Intentional: start time supposed to be in the future. 10 seconds added. It will takes effect in 10 seconds.
				LeaderboardStartTime = FDateTime::UtcNow() + FTimespan::FromSeconds(DelayedTime);
				createRequest.startTime = LeaderboardStartTime.ToIso8601();

				TestLeaderboardCodes.Add(createRequest.leaderboardCode);

				bool bCreateLeaderboardConfigDone = false;
				AdminCreateLeaderboard(createRequest, THandler<FLeaderboardConfigRequest>::CreateLambda([&](FLeaderboardConfigRequest result)
				{
					LeaderboardConfigSuccess[i] = true;
					bCreateLeaderboardConfigDone = true;
				}), 
				FErrorHandler::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
				{
					if ((ErrorCodes)ErrorCode == ErrorCodes::LeaderboardConfigAlreadyExist)
					{
						LeaderboardConfigSuccess[i] = true;
						bCreateLeaderboardConfigDone = true;
					}
					else
					{
						bCreateLeaderboardConfigDone = true;
						UE_LOG(LogAccelByteLeaderboardTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					}
				}));
				WaitUntil(bCreateLeaderboardConfigDone, "Waiting for config leaderboard...");
			}
			DelaySeconds(DelayedTime);

			// SERVER LOGIN
			FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bServerClientLoginSuccess]()
			{
				bServerClientLoginSuccess = true;
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("\t\tClient Successfully Login."));
			}), LeaderboardTestErrorHandler);
			WaitUntil(bServerClientLoginSuccess, "Waiting for Client Login...");

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
				}),
				FErrorHandler::CreateLambda([&](int32 Code, FString Message)
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
				WaitUntil(bUsersCreationDone, "Waiting for user created...");

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
					}), FCustomErrorHandler::CreateLambda([](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
					{
						UE_LOG(LogAccelByteLeaderboardTest, Error, TEXT("failed to login..! Error: %d | Message: %s"), Code, *Message);
					}));
				WaitUntil(UsersLoginSuccess[i], "Waiting for login with user name...");
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

				// USER's STAT ITEM CREATION
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("CREATE USER STAT ITEM...%d out of %d USER..."), i + 1, LeaderboardTestUserInfo_.UserCount);

				bool bCreateStatSuccess = false;
				FRegistry::ServerStatistic.CreateUserStatItems(FRegistry::Credentials.GetUserId(), TestStatCodes, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&i, &UserStatCodeInitSuccess](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
				{
					UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("User stat items are created."));
					UserStatCodeInitSuccess[i] = true;
				}), LeaderboardTestErrorHandler);
				WaitUntil(UserStatCodeInitSuccess[i], "Waiting for create user stat items...");
			}
			TestUsersID = LoginUserIDs;

			// ASSERTION
			for (int i = 0; i < LeaderboardTestUserInfo_.UserCount; i++)
			{
				AB_TEST_TRUE(UsersCreationSuccess[i]);
				AB_TEST_TRUE(UsersLoginSuccess[i]);
				AB_TEST_TRUE(UserStatCodeInitSuccess[i]);
			}
			AB_TEST_TRUE(bServerClientLoginSuccess);
			for (size_t i = 0; i < TestStatCodeCount; i++)
			{
				AB_TEST_TRUE(StatCodeInitSuccess[i]);
				AB_TEST_TRUE(LeaderboardConfigSuccess[i]);
			}

			return true;
		});
	});

	Describe("GetRankings", [this]()
	{
		It("should return Rankings data", [this]()
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

			UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Waiting leaderboard start for %f seconds"), (LeaderboardStartTime - FDateTime::UtcNow()).GetTotalSeconds());
			while (FDateTime::UtcNow() < LeaderboardStartTime)
			{
				FPlatformProcess::Sleep(1.0f);
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
			WaitUntil(bIncrementManyUsersStatItemsSuccess, "Waiting for IncrementManyUsersStatItems...");

			//ACT
			TArray<TArray<TTuple<FString, FAccelByteModelsLeaderboardRankingResult>>> AllLeaderboardTestResults;

			for (auto timeFrame : AllLeaderboardTimeFrame)
			{
				TArray<TTuple<FString, FAccelByteModelsLeaderboardRankingResult>> ThisTimeFrameResults;

				for (auto& leaderboardCode : TestLeaderboardCodes)
				{
					bool bGetRankingDone = false;
					bool bGetRankingFound = false;
					THandler<FAccelByteModelsLeaderboardRankingResult> leaderboardGetRankingsHandler = THandler<FAccelByteModelsLeaderboardRankingResult>::CreateLambda([&](const FAccelByteModelsLeaderboardRankingResult& result)
					{
						bGetRankingDone = true;
						if (result.data.Num() != 0)
						{
							ThisTimeFrameResults.Add(TTuple<FString, FAccelByteModelsLeaderboardRankingResult>{leaderboardCode, result });
							bGetRankingFound = true;
						}
					});

					const auto GetLeaderboardErrorHandler = FErrorHandler::CreateLambda([&bGetRankingDone](int32 ErrorCode, FString ErrorMessage)
					{
						// set done to retry (continue iteration)
						bGetRankingDone = true;
						if (ErrorCode != static_cast<int32>(ErrorCodes::LeaderboardRankingNotFound))
						{
							UE_LOG(LogAccelByteLeaderboardTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
						}
					});

					// retry 60 times
					for (int i = 0; i < 60; i++)
					{
						FRegistry::Leaderboard.GetRankings(leaderboardCode, timeFrame, 0, 9999, leaderboardGetRankingsHandler, GetLeaderboardErrorHandler);

						WaitUntil(bGetRankingDone, FString::Printf(TEXT("Waiting for get rankings [%d]..."), i));

						if (bGetRankingFound)
						{
							break;
						}

						FPlatformProcess::Sleep(1.0f);
						bGetRankingDone = false;
					}
					AB_TEST_TRUE(bGetRankingFound);
				}

				AllLeaderboardTestResults.Add(ThisTimeFrameResults);
			}

			//ASSERT
			/// TestUsersID[USER_ID_WINNER_INDEX] should be in the first place for all time frame and for all leaderboard code

			AB_TEST_EQUAL(AllLeaderboardTestResults.Num(), AllLeaderboardTimeFrame.Num());
			for (auto& resultInATimeFrame : AllLeaderboardTestResults)
			{
				AB_TEST_EQUAL(resultInATimeFrame.Num(), TestLeaderboardCodes.Num());
				for (auto& resultForEachLeaderboardCode : resultInATimeFrame)
				{
					AB_TEST_TRUE(TestUsersID.Num() <= resultForEachLeaderboardCode.Value.data.Num())

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

					AB_TEST_TRUE(bWinnerCorrect)
					for (auto& userId : commonerUserIDs)
					{
						AB_TEST_TRUE(commonerFoundResults.Contains(userId))
					}
				}
			}
			return true;
		});
	});

	Describe("GetUserRankings", [this]()
	{
		It("should return user rankings data", [this]() 
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

			UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Waiting leaderboard start for %f seconds"), (LeaderboardStartTime - FDateTime::UtcNow()).GetTotalSeconds());
			while (FDateTime::UtcNow() < LeaderboardStartTime)
			{
				FPlatformProcess::Sleep(1.0f);
			}

			bool bIncrementManyUsersStatItemsSuccess = false;
			FRegistry::ServerStatistic.IncrementManyUsersStatItems(request,
			THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>::CreateLambda([&](const TArray<FAccelByteModelsBulkStatItemOperationResult>& Result)
			{
				UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("IncrementManyUsersStatItems SUCCESS!"));
				bIncrementManyUsersStatItemsSuccess = true;
			}),
			LeaderboardTestErrorHandler);
			WaitUntil(bIncrementManyUsersStatItemsSuccess, "Waiting for IncrementManyUsersStatItems...");

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
					bool bGetUserRankingFound = false;
					THandler<FAccelByteModelsUserRankingData> leaderboardGetRankingsHandler = THandler<FAccelByteModelsUserRankingData>::CreateLambda([&](const FAccelByteModelsUserRankingData& Result)
					{
						bGetUserRankingDone = true;
						currentUserRanking.Add(Result);
						bGetUserRankingFound = true;
					});

					const auto GetLeaderboardErrorHandler = FErrorHandler::CreateLambda([&bGetUserRankingDone](int32 ErrorCode, FString ErrorMessage)
					{
						if (ErrorCode == static_cast<int32>(ErrorCodes::LeaderboardRankingNotFound))
						{
							// retry
							bGetUserRankingDone = true;
						}
						else
						{
							UE_LOG(LogAccelByteLeaderboardTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
						}
					});

					// retry 60 times
					for (int i = 0; i < 60; i++)
					{
						FRegistry::Leaderboard.GetUserRanking(userId, leaderboardCode, leaderboardGetRankingsHandler, GetLeaderboardErrorHandler);

						WaitUntil(bGetUserRankingDone, FString::Printf(TEXT("Waiting for GetUserRanking [%d]..."), i));

						if (bGetUserRankingFound)
						{
							break;
						}

						FPlatformProcess::Sleep(1.0f);
						bGetUserRankingDone = false;
					}
					AB_TEST_TRUE(bGetUserRankingFound);
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
						AB_TEST_EQUAL(userRankingResult.daily.rank, 1);
					}
					else
					{
						AB_TEST_NOT_EQUAL(userRankingResult.daily.rank, 1);
					}
				}
			}
			return true;
		});
	});

	Describe("Teardown", [this]()
	{
		It("should clean up leaderboard stat code", [this]()
		{
			// DELETE LEADERBOARD CODE
			for (auto& leaderboardCode : TestLeaderboardCodes)
			{
				bool bDeleteLeaderboard = false;

				AdminDeleteLeaderboard(leaderboardCode, FSimpleDelegate::CreateLambda([&]()
				{
					UE_LOG(LogAccelByteLeaderboardTest, Log, TEXT("Success"));
					bDeleteLeaderboard = true;
				}), LeaderboardTestErrorHandler);
				WaitUntil(bDeleteLeaderboard, "Waiting for leaderboard code deletion...");
			}
			return true;
		});
	});

}
