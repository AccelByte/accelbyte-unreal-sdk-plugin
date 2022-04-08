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

const auto QosTestErrorHandler = FErrorHandler::CreateLambda([](const int32 ErrorCode, const FString ErrorMessage)
{
	UE_LOG(LogAccelByteQosTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

/**
 * @brief Sets generic logs, passed from err fail via code+msg.
 * @param ErrorCode Pass from err
 * @param ErrorMessage Pass from err
 */
void QosTestErrorHandler2(const int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteQosTest, Error, TEXT("[QosTest] Error code: %d\nError message: %s"), ErrorCode, *ErrorMessage);
}

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
const auto QosTestLoginApiClient = [](const FTestUser& User)
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
				QosTestErrorHandler2(ErrorCode, ErrorMessage); // Curry to general err handler
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
bool InitTeardown(TArray<FTestUser>& QosTestUsers)
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
#pragma endregion /Auth


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
const auto QosTestGetServerLatenciesThenPing = [](
	const FApiClientPtr& QosTestApiUserPtr,
	const bool bPingRegionsOnSuccess,
	TArray<TPair<FString, float>>& OutServerLatencies)
{
	const FString FuncName = "QosTestGetServerLatencies";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteQosTest, Display, TEXT("%s"), *FuncName);
	
	QosTestApiUserPtr->Qos.CallGetQosServers(
		bPingRegionsOnSuccess,
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
const auto QosTestGetActiveNamespaceServerLatenciesThenPing = [](
	const FApiClientPtr& QosTestApiUserPtr,
	TArray<TPair<FString, float>>& OutServerLatencies)
{
	const FString FuncName = "QosTestGetActiveNamespaceServerLatencies";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteQosTest, Display, TEXT("%s"), *FuncName);
	
	QosTestApiUserPtr->Qos.GetActiveServerLatencies(
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

/** @brief Logs + removes tickers from FRegistry and TestUser */
void QosTestRemoveLatencyPollers(const FApiClientPtr& ApiUserPtr)
{
	UE_LOG(LogAccelByteQosTest, Log, TEXT("QosTestRemoveLatencyPollers"));
	FRegistry::Qos.RemoveLatencyPollers(); // Auto-triggers @ login success
	ApiUserPtr->Qos.RemoveLatencyPollers(); // Triggers manually
}

/**
 * @brief Start Server (target regions) Latencies scheduler to run every x seconds, caching QosServers.
 *   then wait for x ticks before determining success.
 *   - DIRECTLY accesses the scheduler - skips login.
 */
const auto QosTestInitGetServerLatenciesSchedulerDirectly = [](
	const FApiClientPtr& QosTestApiUserPtr,
	const int32 NumTimesToTest)
{
	const FString FuncName = "QosTestInitGetServerLatenciesScheduler";
	bool bIsOk = false;
	bool bIsDone = false;
	int32 Counter{0};
	UE_LOG(LogAccelByteQosTest, Display, TEXT("%s"), *FuncName);

	QosTestApiUserPtr->Qos.InitGetServerLatenciesScheduler(
	THandler<TArray<TPair<FString, float>>>::CreateLambda(
		[&](const TArray<TPair<FString, float>>& OnTickResult)
	{
		UE_LOG(LogAccelByteQosTest, Log, TEXT("%s Success (%d/%d)!"),
			*FuncName, Counter + 1, NumTimesToTest);
			
		bIsOk = true;
		Counter++;
		bIsDone = Counter >= NumTimesToTest; // Stops the func immediately, when true
	}),
	FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
	{
		QosAutoTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
		bIsDone = true; // Finish the entire func - stop early instead of wait for full timeout
	}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("Waiting for %s ...)"), *FuncName));
	return bIsOk;
};

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
	TArray<TPair<FString, float>> QosTestLatencies;
END_DEFINE_SPEC(FQosTestSpec)

void FQosTestSpec::Define()
{
	Describe("Latency2 (Login + Set Poll Settings)", [this]()
	{
		LatentBeforeEach([this](const FDoneDelegate& Done)
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
		
		LatentIt("01: Should get server latency regions only (no ping, no polling)",
			[this](const FDoneDelegate& Done)
		{
			constexpr bool bPingRegionsOnSuccess = false;
			const bool bIsSuccess = QosTestGetServerLatenciesThenPing(
				QosTestApiUserPtrArr[0],
				bPingRegionsOnSuccess,
				QosTestLatencies);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("02: Should Should set Settings::QosServerLatencyPollIntervalSecs > 0; init the 'get latencies scheduler' (uses cached server regions); "
			"if Settings::QosServerLatencyPollIntervalSecs < 0, nothing will happen (success)",
			[this](const FDoneDelegate& Done)
		{
			// Logged in: Call the scheduler directly with test intervals, caching AccelByteQos::QosServers (latency region targets).
			SetQosSchedulerSettings(
				OrigQosLatencyPollIntervalSecs,
				OrigQosServerLatencyPollIntervalSecs);

			// To catch if successful or not, we re-init the scheduler:
			constexpr bool bPingRegionsOnSuccess = true;
			const bool bIsSuccess = QosTestGetServerLatenciesThenPing(
				QosTestApiUserPtrArr[0],
				bPingRegionsOnSuccess,
				QosTestLatencies);

			Done.Execute();
			return bIsSuccess;
		});

		/* Commented this test because feature still not in release pipeline yet
		* 
		LatentIt("03: Should set Settings::QosServerLatencyPollIntervalSecs > 0; init the 'get latencies scheduler' (uses cached server regions); "
			"if Settings::QosServerLatencyPollIntervalSecs < 0, nothing will happen (success) for activated region in namespace",
			[this](const FDoneDelegate& Done)
		{
			// Logged in: Call the scheduler directly with test intervals, caching AccelByteQos::QosServers (latency region targets).
			SetQosSchedulerSettings(
				1,
				1);

			// To catch if successful or not, we re-init the scheduler:
			const bool bIsSuccess = QosTestGetActiveNamespaceServerLatenciesThenPing(
				QosTestApiUserPtrArr[0],
				QosTestLatencies);


			AB_TEST_EQUAL(QosTestLatencies, QosTestApiUserPtrArr[0]->Qos.GetCachedLatencies());

			Done.Execute();
			return bIsSuccess;
		});
		*/		
		// ===================================================================		

		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			// Restore original settings
			const bool bStopScheduler = OrigQosLatencyPollIntervalSecs == 0;
			if (bStopScheduler)
				QosTestRemoveLatencyPollers(QosTestApiUserPtrArr[0]);
				
			SetQosSchedulerSettings(OrigQosLatencyPollIntervalSecs, OrigQosServerLatencyPollIntervalSecs);

			const bool bIsSuccess = InitTeardown(QosTestUsers); // Logout
			QosTestUsers.Reset();
			QosTestApiUserPtrArr.Empty();

			Done.Execute();
			return bIsSuccess;
		});
	});
}
#pragma endregion /Automation Spec


#pragma region Deprecated Test Style (!AutomationSpec)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(QosGetServerLatencies, "AccelByte.Tests.Qos.A.GetServerLatencies", AutomationFlagMaskQos);
bool QosGetServerLatencies::RunTest(const FString& Parameters)
{
	bool bGetServerLatenciesSuccess = false;
	TArray<TPair<FString, float>> Latencies;

	FRegistry::Qos.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda([&bGetServerLatenciesSuccess, &Latencies](const TArray<TPair<FString, float>>& Result)
	{
		UE_LOG(LogAccelByteQosTest, Log, TEXT("Get Server Latencies Success! Count: %d"), Result.Num());
		for (auto Latency : Result)
		{
			UE_LOG(LogAccelByteQosTest, Log, TEXT("Latency: Region %s | Ping: %.2f"), *Latency.Key, Latency.Value);
		}
		bGetServerLatenciesSuccess = true;
		Latencies = Result;
	}), QosTestErrorHandler);
	WaitUntil([&]()
	{
		return bGetServerLatenciesSuccess;
	}, "Waiting for get server latencies...", 60);
	AB_TEST_TRUE(bGetServerLatenciesSuccess);
	AB_TEST_TRUE(Latencies.Num() > 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ServerQosGetServerLatencies, "AccelByte.Tests.Qos.A.ServerGetServerLatencies", AutomationFlagMaskQos);
bool ServerQosGetServerLatencies::RunTest(const FString& Parameters)
{
	bool bGetServerLatenciesSuccess = false;
	TArray<TPair<FString, float>> Latencies;

	FRegistry::ServerQosManager.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda([&bGetServerLatenciesSuccess, &Latencies](const TArray<TPair<FString, float>>& Result)
	{
		UE_LOG(LogAccelByteQosTest, Log, TEXT("Get Server Latencies Success! Count: %d"), Result.Num());
		for (auto Latency : Result)
		{
			UE_LOG(LogAccelByteQosTest, Log, TEXT("Latency: Region %s | Ping: %.2f"), *Latency.Key, Latency.Value);
		}
		bGetServerLatenciesSuccess = true;
		Latencies = Result;
	}), QosTestErrorHandler);
	WaitUntil([&]()
	{
		return bGetServerLatenciesSuccess;
	}, "Waiting for get server latencies...", 60);
	AB_TEST_TRUE(bGetServerLatenciesSuccess);
	AB_TEST_TRUE(Latencies.Num() > 0);
	return true;
}
#pragma endregion /Deprecated Test Style (!AutomationSpec)
