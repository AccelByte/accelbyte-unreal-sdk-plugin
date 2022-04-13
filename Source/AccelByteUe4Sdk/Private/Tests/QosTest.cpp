// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUe4SdkModule.h"
#include "Misc/AutomationTest.h"
#include "Api/AccelByteQos.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "Core/AccelByteRegistry.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteMultiRegistry.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteQosTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteQosTest);

constexpr int32 AutomationFlagMaskQos =
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::CommandletContext |
	EAutomationTestFlags::ClientContext;

/**
 * @brief Used in Test Automation Spec.
 * - Sets generic logs, passed from err fail via code+msg.
 * @param ErrorCode Pass from err
 * @param ErrorMessage Pass from err
 */
void QosAutoTestErrorHandler(const int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteQosTest, Error, TEXT("Error code: %d\nError message: %s"), ErrorCode, *ErrorMessage);
}

#pragma region Auth
/** @brief LoginWithUsername with TestUser */
bool QosTestLoginApiClient(const FTestUser& User)
{
	UE_LOG(LogAccelByteQosTest, Display, TEXT("[QosTest] QosTestLoginApiClient: %s"), *User.Email);
	bool bIsOk = false;
	bool bIsDone = false;

	const FApiClientPtr& ApiClient = FMultiRegistry::GetApiClient(User.Email); // Using email as key
	ApiClient->User.LoginWithUsername(
		User.Email,
		User.Password,
		FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteQosTest, Display, TEXT("[QosTest] LoginWithUsername Success"));
				bIsOk = true;
				bIsDone = true;
			}),
			FCustomErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorJson)
			{
				QosAutoTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[QosTest] Waiting for Client (%s) to login ..."), *User.Email));
	return bIsOk;
};

bool QosTestInitSetupTestUsers(
	const int32 NumTestUsers,
	TArray<FTestUser>& OutQosTestUsers,
	TArray<FApiClientPtr>& OutQosTestApiUserPtrArr)
{
	// Create, register and login test user(s) with random details to avoid test collision
	const bool bSetupUsers = SetupTestUsers(NumTestUsers, OutQosTestUsers);

	// Get ref to the client pointers since this is used more often than raw Users
	OutQosTestApiUserPtrArr.Reset();
	for (const FTestUser& User : OutQosTestUsers)
	{
		OutQosTestApiUserPtrArr.Emplace(FMultiRegistry::GetApiClient(User.Email));
	}
			
	// Multi-user login
	for (const auto& User : OutQosTestUsers)
	{
		if (!QosTestLoginApiClient(User))
			return false;
	}
	
	return bSetupUsers;
}

/**
 * @brief Logout test users
 * @param QosTestUsers
 * @return 
 */
bool QosTestInitTeardown(TArray<FTestUser>& QosTestUsers)
{
	// 1: Should teardown test users
	// Teardown: Always delete test users after it is done
	if (QosTestUsers.Num() < 1)
		return true; // Already toredown
		
	const bool bTeardownUsersSuccess = TeardownTestUsers(QosTestUsers);
	if (!bTeardownUsersSuccess)
		return false; // Err

	// Successful teardown
	return true;
}
#pragma endregion


#pragma region Utils
/**
 * @brief If >0 && <10m, we'll set to interval to 10m.
 * @param NewQosLatencyPollIntervalSecs 
 * @param NewQosServerLatencyPollIntervalSecs 
 */
void SetQosSchedulerSettings(const float NewQosLatencyPollIntervalSecs, const float NewQosServerLatencyPollIntervalSecs)
{
	FRegistry::Settings.QosLatencyPollIntervalSecs = NewQosLatencyPollIntervalSecs;
	FRegistry::Settings.QosServerLatencyPollIntervalSecs = NewQosServerLatencyPollIntervalSecs;
}

void QosTestLogLatencies(const TArray<TPair<FString, float>>& LatenciesResult)
{
	for (TTuple<FString, float> Latency : LatenciesResult)
	{
		UE_LOG(LogAccelByteQosTest, Log, TEXT("LatenciesResult: Region %s | Ping: %.2f"),
			*Latency.Key, Latency.Value);
	}
}

/**
 * @brief Be sure you set Settings::QosServerLatencyPollIntervalSecs > 0 to have any effect.
 * - Gets server regions.
 *   - Pings latencies on success, if bPingRegionsOnSuccess
 */
bool QosTestGetServerLatenciesThenPing(
	AccelByte::Api::Qos& Qos,
	TArray<TPair<FString, float>>& OutServerLatencies)
{
	const FString FuncName = "QosTestGetServerLatencies";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteQosTest, Display, TEXT("%s"), *FuncName);
	
	Qos.GetServerLatencies(
		THandler<TArray<TPair<FString, float>>>::CreateLambda(
		[&FuncName, &OutServerLatencies, &bIsOk, &bIsDone](const TArray<TPair<FString, float>>& Result)
		{
			UE_LOG(LogAccelByteQosTest, Log, TEXT("%s Success! Count: %d"), *FuncName, Result.Num());
			QosTestLogLatencies(Result);
			
			OutServerLatencies = Result;
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			QosAutoTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true; // Stop early instead of wait for full timeout
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("Waiting for %s ...)"), *FuncName));
	return bIsOk;
};

// Same as above, just with region that is activated in game's namespace in Admin Portal
bool QosTestGetActiveNamespaceServerLatenciesThenPing(
	AccelByte::Api::Qos& Qos,
	TArray<TPair<FString, float>>& OutServerLatencies)
{
	const FString FuncName = "QosTestGetActiveNamespaceServerLatencies";
	bool bIsOk = false;
	bool bIsDone = false;

	UE_LOG(LogAccelByteQosTest, Display, TEXT("%s"), *FuncName);
	
	Qos.GetActiveServerLatencies(
		THandler<TArray<TPair<FString, float>>>::CreateLambda(
		[&FuncName, &OutServerLatencies, &bIsOk, &bIsDone](const TArray<TPair<FString, float>>& Result)
		{
			UE_LOG(LogAccelByteQosTest, Log, TEXT("%s Success! Count: %d"), *FuncName, Result.Num());
			QosTestLogLatencies(Result);
			
			OutServerLatencies = Result;
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			QosAutoTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true; // Stop early instead of wait for full timeout
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("Waiting for %s ...)"), *FuncName));
	return bIsOk;
};

/** @brief Logs + start latency pollers */
void QosTestStartLatencyPollers(AccelByte::Api::Qos& Qos)
{
	UE_LOG(LogAccelByteQosTest, Log, TEXT("%s"), *FString(__FUNCTION__));
	Qos.StartLatencyPollers();
}

/** @brief Logs + stop latency pollers */
void QosTestStopLatencyPollers(AccelByte::Api::Qos& Qos)
{
	UE_LOG(LogAccelByteQosTest, Log, TEXT("%s"), *FString(__FUNCTION__));
	Qos.StopLatencyPollers();
}

/**
 * @brief After logging in (pre-req), wait x seconds.
 * - Used to test init qos scheduler to ensure logs show ticks.
 */
const auto QosTestWaitXSeconds = [](const float NumSecsToSleep)
{
	const FString FuncName = "QosTestWaitXSeconds";
	UE_LOG(LogAccelByteQosTest, Display, TEXT("%s (%f secs)"), *FuncName, NumSecsToSleep);

	if (NumSecsToSleep > 0)
		FPlatformProcess::Sleep(NumSecsToSleep);

	// Look at logs to determine if okay.
	return true;
};
#pragma endregion /Utils


#pragma region Automation Spec
BEGIN_DEFINE_SPEC(FQosTestSpec, "AccelByte.Tests.QosNew", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	float OrigQosLatencyPollIntervalSecs;
	float OrigQosServerLatencyPollIntervalSecs;
	TArray<FTestUser> QosTestUsers;
	TArray<FApiClientPtr> QosTestApiUserPtrArr;
END_DEFINE_SPEC(FQosTestSpec)

void FQosTestSpec::Define()
{
	Describe("while trying to get latencies data", 
		[this]()
		{
			BeforeEach(
				[this]()
				{
					QosTestStopLatencyPollers(FRegistry::Qos);
				});

			It("game client should get latencies data from available regions", 
				[this]()->bool
				{
					TArray<TPair<FString, float>> Latencies;
					bool bIsSuccess = QosTestGetServerLatenciesThenPing(FRegistry::Qos, Latencies);

					AB_TEST_TRUE(bIsSuccess);
					AB_TEST_TRUE(Latencies.Num() > 0);

					return true;
				});

			// Temporary disabled because the feature still not in release pipeline yet
			// TODO: enable this test when the feature is added
			xIt("game client should get latencies data from activated region in the namespace",
				[this]()->bool
				{
					TArray<TPair<FString, float>> Latencies;
					const bool bIsSuccess = QosTestGetActiveNamespaceServerLatenciesThenPing(FRegistry::Qos, Latencies);

					AB_TEST_TRUE(bIsSuccess);
					AB_TEST_TRUE(Latencies.Num() > 0);
					AB_TEST_EQUAL(Latencies, QosTestApiUserPtrArr[0]->Qos.GetCachedLatencies());

					return bIsSuccess;
				});

			// Disabled permanently
			xIt("game server should get latencies data from available regions", 
				[this]()->bool
				{
					bool bGetServerLatenciesSuccess = false;
					TArray<TPair<FString, float>> Latencies;

					FRegistry::ServerQosManager.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda(
						[&bGetServerLatenciesSuccess, &Latencies](const TArray<TPair<FString, float>>& Result)
					{
						UE_LOG(LogAccelByteQosTest, Log, TEXT("Get Server Latencies Success! Count: %d"), Result.Num());
						for (auto Latency : Result)
						{
							UE_LOG(LogAccelByteQosTest, Log, TEXT("Latency: Region %s | Ping: %.2f"), *Latency.Key, Latency.Value);
						}
						bGetServerLatenciesSuccess = true;
						Latencies = Result;
					}), FErrorHandler::CreateStatic(QosAutoTestErrorHandler));

					WaitUntil([&]()
						{
							return bGetServerLatenciesSuccess;
						}, "Waiting for get server latencies...", 60);

					AB_TEST_TRUE(bGetServerLatenciesSuccess);
					AB_TEST_TRUE(Latencies.Num() > 0);

					return true;
				});
		});

	Describe("while poller interval is equal to 0 (zero)", 
		[this]()
		{
			BeforeEach(
				[this]()
				{
					QosTestStopLatencyPollers(FRegistry::Qos);
					OrigQosLatencyPollIntervalSecs = FRegistry::Settings.QosLatencyPollIntervalSecs;
					OrigQosServerLatencyPollIntervalSecs = FRegistry::Settings.QosServerLatencyPollIntervalSecs;

					SetQosSchedulerSettings(0.0f, 0.0f);
				});

			It("pollers should be inactive when pollers are started directly", 
				[this]()->bool
				{
					QosTestStopLatencyPollers(FRegistry::Qos);
					QosTestStartLatencyPollers(FRegistry::Qos);

					bool bArePollersActive = FRegistry::Qos.AreLatencyPollersActive();
					TArray<TPair<FString, float>> Latencies = FRegistry::Qos.GetCachedLatencies();

					AB_TEST_FALSE(bArePollersActive);
					AB_TEST_EQUAL(Latencies.Num(), 0);

					return true;
				});

			It("game client should get latencies data from available regions when trying to get latencies", 
				[this]()->bool
				{
					TArray<TPair<FString, float>> Latencies;
					bool bIsSuccess = QosTestGetServerLatenciesThenPing(FRegistry::Qos, Latencies);
					bool bArePollersActive = FRegistry::Qos.AreLatencyPollersActive();

					AB_TEST_TRUE(bIsSuccess);
					AB_TEST_TRUE(Latencies.Num() > 0);
					AB_TEST_FALSE(bArePollersActive);

					return true;
				});

			AfterEach(
				[this]()
				{
					SetQosSchedulerSettings(OrigQosLatencyPollIntervalSecs, OrigQosServerLatencyPollIntervalSecs);
					QosTestStopLatencyPollers(FRegistry::Qos);
				});
		});

	Describe("while poller interval is greater than 0 (zero)", 
		[this]()
		{
			BeforeEach(
				[this]()->bool
				{
					QosTestStopLatencyPollers(FRegistry::Qos);
					OrigQosLatencyPollIntervalSecs = FRegistry::Settings.QosLatencyPollIntervalSecs;
					OrigQosServerLatencyPollIntervalSecs = FRegistry::Settings.QosServerLatencyPollIntervalSecs;

					SetQosSchedulerSettings(60.0f, 60.0f);

					return true;
				});

			It("pollers should be active when pollers are started directly", 
				[this]()->bool
				{
					QosTestStopLatencyPollers(FRegistry::Qos);
					QosTestStartLatencyPollers(FRegistry::Qos);

					bool bArePollersActive = FRegistry::Qos.AreLatencyPollersActive();
					TArray<TPair<FString, float>> Latencies = FRegistry::Qos.GetCachedLatencies();

					AB_TEST_TRUE(bArePollersActive);
					AB_TEST_EQUAL(Latencies.Num(), 0);

					return true;
				});

			It("game client should get latencies data from available regions and pollers are active when trying to get server latencies", 
				[this]()->bool
				{
					TArray<TPair<FString, float>> Latencies;
					bool bIsSuccess = QosTestGetServerLatenciesThenPing(FRegistry::Qos, Latencies);
					bool bArePollersActive = FRegistry::Qos.AreLatencyPollersActive();

					AB_TEST_TRUE(bIsSuccess);
					AB_TEST_TRUE(Latencies.Num() > 0);
					AB_TEST_TRUE(bArePollersActive);

					return true;
				});
		});

	Describe("while user logging in and poller interval is greater than 0 (zero)", 
		[this]()
		{
			LatentBeforeEach(
				[this](const FDoneDelegate& Done)
				{
					// Cache orig Settings for Qos*LatencyPollIntervalSecs so we can restore them later
					OrigQosLatencyPollIntervalSecs = FRegistry::Settings.QosLatencyPollIntervalSecs;
					OrigQosServerLatencyPollIntervalSecs = FRegistry::Settings.QosServerLatencyPollIntervalSecs;

					// Set Settings Qos*LatencyPollIntervalSecs 
					constexpr float SettingsIntervalSecs = 2;
					SetQosSchedulerSettings(SettingsIntervalSecs, SettingsIntervalSecs + 1);

					// -------------------
					// Login
					constexpr int32 NumTestUsers = 1;
					const bool bSetupUsers = QosTestInitSetupTestUsers(NumTestUsers, QosTestUsers, QosTestApiUserPtrArr);

					Done.Execute();
					return bSetupUsers;
				});

			// ===================================================================
			LatentIt("should get latencies data from cache",
				[this](const FDoneDelegate& Done)
				{
					bool bArePollersActive = QosTestApiUserPtrArr[0]->Qos.AreLatencyPollersActive();
					TArray<TPair<FString, float>> CachedLatencies = QosTestApiUserPtrArr[0]->Qos.GetCachedLatencies();
					TArray<TPair<FString, float>> Latencies;
					bool bIsSuccess = QosTestGetServerLatenciesThenPing(QosTestApiUserPtrArr[0]->Qos, Latencies);

					Done.Execute();

					AB_TEST_TRUE(Latencies.Num() > 0);
					AB_TEST_TRUE(bArePollersActive);
					AB_TEST_NOT_EQUAL(Latencies, CachedLatencies);

					return bIsSuccess;
				});

			LatentIt("should get latencies data from available regions",
				[this](const FDoneDelegate& Done)
				{
					TArray<TPair<FString, float>> Latencies;
					bool bIsSuccess = QosTestGetServerLatenciesThenPing(QosTestApiUserPtrArr[0]->Qos, Latencies);
					bool bArePollersActive = QosTestApiUserPtrArr[0]->Qos.AreLatencyPollersActive();

					Done.Execute();

					AB_TEST_TRUE(bIsSuccess);
					AB_TEST_TRUE(Latencies.Num() > 0);
					AB_TEST_TRUE(bArePollersActive);

					return bIsSuccess;
				});
			
			// ===================================================================		

			LatentAfterEach(
				[this](const FDoneDelegate& Done)
				{
					// Restore original settings
					const bool bStopScheduler = OrigQosLatencyPollIntervalSecs == 0;
					if (bStopScheduler)
					{
						QosTestStopLatencyPollers(QosTestApiUserPtrArr[0]->Qos);
					}

					SetQosSchedulerSettings(OrigQosLatencyPollIntervalSecs, OrigQosServerLatencyPollIntervalSecs);

					const bool bIsSuccess = QosTestInitTeardown(QosTestUsers); // Logout
					QosTestUsers.Reset();
					QosTestApiUserPtrArr.Empty();

					Done.Execute();
					return bIsSuccess;
				});
		});
}
#pragma endregion
