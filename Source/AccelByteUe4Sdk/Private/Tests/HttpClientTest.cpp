// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "HttpClientTest.h"

#include "Misc/AutomationTest.h"

#include "TestUtilities.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpClient.h"
#include "UserTestAdmin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpClientTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpClientTest);

static const int32 AutomationFlagMaskCustomization = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

TArray<FTestUser> HttpClientTestUsers;

class FThreadCancellationTestWorker : public FRunnable
{
public:
	FThreadCancellationTestWorker(const FString& ThreadName, const TSharedPtr<FAccelByteCancellationToken>& TokenHandler, bool bIsCanceller = false)
		: TokenHandler(TokenHandler)
		, bIsCanceller(bIsCanceller)
	{
		Thread = FRunnableThread::Create(this, *ThreadName);
	}

	~FThreadCancellationTestWorker()
	{
		delete Thread;
		Thread = nullptr;
	}

	virtual uint32 Run() override
	{
		uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
		FString ThreadName = FThreadManager::Get().GetThreadName(ThreadId);
		UE_LOG(LogAccelByteHttpClientTest, Log, TEXT("[Async] Start Cancellation Thread=%s"), *ThreadName);
		if (bIsCanceller)
		{
			TokenHandler->Cancel();
		}
		return 0;
	}

	void EnsureCompletion()
	{
		Stop();
		Thread->WaitForCompletion();
	}

	bool IsCancelled() { return TokenHandler->IsCancelled(); }

private:
	TSharedPtr<FAccelByteCancellationToken> TokenHandler;
	bool bIsCanceller;
	FRunnableThread* Thread;
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(DoUStructTest, "AccelByte.Tests.Core.HttpClient.DoUStruct", AutomationFlagMaskCustomization)
void DoUStructTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("GET");
	OutTestCommands.Add("GET");
	OutBeautifiedNames.Add("POST");
	OutTestCommands.Add("POST");
	OutBeautifiedNames.Add("PUT");
	OutTestCommands.Add("PUT");
	OutBeautifiedNames.Add("DELETE");
	OutTestCommands.Add("DELETE");
}

bool DoUStructTest::RunTest(const FString& Method)
{
	AB_TEST_TRUE(SetupTestUsers(1, HttpClientTestUsers));

	FHttpClientTestAnythingResponse Result;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	FHttpClientTestAnythingRequest Content;

	Content.Name = "John Doe";
	Content.Age = 24;

	FHttpClient HttpClient(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

	const FString Url = "https://httpbin.org/anything/{namespace}/{userId}"; // {namespace} and {userId} is going to be replaced by actual value in credentials
	
	HttpClient.ApiRequest(Method, Url, { { "q", Query }}, Content,
		THandler<FHttpClientTestAnythingResponse>::CreateLambda([&](const FHttpClientTestAnythingResponse& Response)
			{
				Result = Response;
				bIsSuccess = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
			{
				bIsDone = true;
			}));

	WaitUntil(bIsDone, "Waiting ...");

	AB_TEST_TRUE(bIsSuccess);
	AB_TEST_TRUE(Result.Url.Contains(HttpClientTestUsers[0].Credentials.GetNamespace() + "/" + HttpClientTestUsers[0].Credentials.GetUserId()));
	AB_TEST_EQUAL(Result.Method, Method);
	AB_TEST_EQUAL(Result.Args["q"], Query);
	AB_TEST_EQUAL(Result.Headers[TEXT("Authorization")], FString::Printf(TEXT("Bearer %s"), *HttpClientTestUsers[0].Credentials.GetAccessToken()));
	if (!Method.ToUpper().Equals(TEXT("GET")))
	{
		AB_TEST_EQUAL(Result.Json.Name, Content.Name);
		AB_TEST_EQUAL(Result.Json.Age, Content.Age);
	}

	AB_TEST_TRUE(TeardownTestUsers(HttpClientTestUsers));

	return bIsSuccess;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(DoFormTest, "AccelByte.Tests.Core.HttpClient.DoForm", AutomationFlagMaskCustomization)
void DoFormTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("GET");
	OutTestCommands.Add("GET");
	OutBeautifiedNames.Add("POST");
	OutTestCommands.Add("POST");
	OutBeautifiedNames.Add("PUT");
	OutTestCommands.Add("PUT");
	OutBeautifiedNames.Add("DELETE");
	OutTestCommands.Add("DELETE");
}

bool DoFormTest::RunTest(const FString& Method)
{
	AB_TEST_TRUE(SetupTestUsers(1, HttpClientTestUsers));

	FHttpClientTestAnythingResponse Result;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	const TMap<FString, FString> Content = {
		{"name", "John Doe"},
		{"age", "24"},
	};

	FHttpClient HttpClient(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

	const FString Url = "https://httpbin.org/anything/{namespace}/{userId}"; // {namespace} and {userId} is going to be replaced by actual value in credentials
	
	HttpClient.ApiRequest(Method, Url, { { "q", Query } }, Content,
		THandler<FHttpClientTestAnythingResponse>::CreateLambda([&](const FHttpClientTestAnythingResponse& Response)
			{
				Result = Response;
				bIsSuccess = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
			{
				bIsDone = true;
			}));

	WaitUntil(bIsDone, "Waiting ...");

	AB_TEST_TRUE(bIsSuccess);
	AB_TEST_EQUAL(Result.Method, Method);
	AB_TEST_TRUE(Result.Url.Contains(HttpClientTestUsers[0].Credentials.GetNamespace() + "/" + HttpClientTestUsers[0].Credentials.GetUserId()));
	AB_TEST_EQUAL(Result.Args["q"], Query);
	AB_TEST_EQUAL(Result.Headers[TEXT("Authorization")], FString::Printf(TEXT("Bearer %s"), *HttpClientTestUsers[0].Credentials.GetAccessToken()));
	if (!Method.ToUpper().Equals(TEXT("GET")))
	{
		AB_TEST_EQUAL(Result.Form[TEXT("name")], TEXT("John Doe"));
		AB_TEST_EQUAL(Result.Form[TEXT("age")], TEXT("24"));
	}

	AB_TEST_TRUE(TeardownTestUsers(HttpClientTestUsers));

	return bIsSuccess;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(DoCancelAfterDoneTest, "AccelByte.Tests.Core.HttpClient.DoCancelAfterDone", AutomationFlagMaskCustomization)
void DoCancelAfterDoneTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("GET");
	OutTestCommands.Add("GET");
	OutBeautifiedNames.Add("POST");
	OutTestCommands.Add("POST");
	OutBeautifiedNames.Add("PUT");
	OutTestCommands.Add("PUT");
	OutBeautifiedNames.Add("DELETE");
	OutTestCommands.Add("DELETE");
}

bool DoCancelAfterDoneTest::RunTest(const FString& Method)
{
	AB_TEST_TRUE(SetupTestUsers(1, HttpClientTestUsers));

	FHttpClientTestAnythingResponse Result;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	const TMap<FString, FString> Content = {
		{"name", "John Doe"},
		{"age", "24"},
	};

	FHttpClient HttpClient(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

	const FString Url = "https://httpbin.org/anything/{namespace}/{userId}"; // {namespace} and {userId} is going to be replaced by actual value in credentials

	FAccelByteTaskPtr HttpTask = HttpClient.ApiRequest(Method, Url, { { "q", Query } }, Content,
		THandler<FHttpClientTestAnythingResponse>::CreateLambda([&](const FHttpClientTestAnythingResponse& Response)
			{
				Result = Response;
				bIsSuccess = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
			{
				bIsDone = true;
			}));

	WaitUntil(bIsDone, "Waiting ...");
	FAccelByteCancellationToken CancellationToken = HttpTask->GetCancellationToken();
	CancellationToken.Cancel();

	AB_TEST_TRUE(bIsDone);
	AB_TEST_FALSE(CancellationToken.IsCancelled());

	AB_TEST_TRUE(TeardownTestUsers(HttpClientTestUsers));

	return bIsDone;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(DoCancelAfterRequestTest, "AccelByte.Tests.Core.HttpClient.DoCancelAfterRequest", AutomationFlagMaskCustomization)
void DoCancelAfterRequestTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("GET");
	OutTestCommands.Add("GET");
	OutBeautifiedNames.Add("POST");
	OutTestCommands.Add("POST");
	OutBeautifiedNames.Add("PUT");
	OutTestCommands.Add("PUT");
	OutBeautifiedNames.Add("DELETE");
	OutTestCommands.Add("DELETE");
}

bool DoCancelAfterRequestTest::RunTest(const FString& Method)
{
	AB_TEST_TRUE(SetupTestUsers(1, HttpClientTestUsers));

	FHttpClientTestAnythingResponse Result;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	const TMap<FString, FString> Content = {
		{"name", "John Doe"},
		{"age", "24"},
	};

	FHttpClient HttpClient(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

	const FString Url = "https://httpbin.org/anything/{namespace}/{userId}"; // {namespace} and {userId} is going to be replaced by actual value in credentials

	FAccelByteTaskPtr HttpTask = HttpClient.ApiRequest(Method, Url, { { "q", Query } }, Content,
		THandler<FHttpClientTestAnythingResponse>::CreateLambda([&](const FHttpClientTestAnythingResponse& Response)
			{
				Result = Response;
				bIsSuccess = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
			{
				bIsDone = true;
			}));

	FAccelByteCancellationToken CancellationToken = HttpTask->GetCancellationToken();
	CancellationToken.Cancel();

	WaitUntil(bIsDone, "Waiting ...");

	AB_TEST_TRUE(bIsDone);
	AB_TEST_TRUE(CancellationToken.IsCancelled());

	AB_TEST_TRUE(TeardownTestUsers(HttpClientTestUsers));

	return bIsDone;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(DoCancelOneofManyThreadRequestTest, "AccelByte.Tests.Core.HttpClient.DoCancelOneofManyThreadRequest", AutomationFlagMaskCustomization)
void DoCancelOneofManyThreadRequestTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("GET");
	OutTestCommands.Add("GET");
	OutBeautifiedNames.Add("POST");
	OutTestCommands.Add("POST");
	OutBeautifiedNames.Add("PUT");
	OutTestCommands.Add("PUT");
	OutBeautifiedNames.Add("DELETE");
	OutTestCommands.Add("DELETE");
}

bool DoCancelOneofManyThreadRequestTest::RunTest(const FString& Method)
{
	AB_TEST_TRUE(SetupTestUsers(1, HttpClientTestUsers));

	const int ThreadCount = 10;
	TArray<TSharedPtr<FThreadCancellationTestWorker>> Tasks;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	const TMap<FString, FString> Content = {
		{"name", "John Doe"},
		{"age", "24"},
	};

	FHttpClient HttpClient(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

	const FString Url = "https://httpbin.org/anything/{namespace}/{userId}"; // {namespace} and {userId} is going to be replaced by actual value in credentials

	FAccelByteCancellationToken CancellationToken = HttpClient.ApiRequest(Method, Url, { { "q", Query } }, Content,
		THandler<FHttpClientTestAnythingResponse>::CreateLambda([&](const FHttpClientTestAnythingResponse& Response)
			{
				bIsSuccess = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
			{
				bIsDone = true;
			}))->GetCancellationToken();

	const int Canceller = FMath::RandRange(0, (ThreadCount-1));
	for (int i = 0; i < ThreadCount; i++)
	{
		FString ThreadName = FString::Printf(TEXT("Concurrency_%03d"), i);
		TSharedPtr<FThreadCancellationTestWorker> Task = MakeShared<FThreadCancellationTestWorker>(ThreadName, MakeShared<FAccelByteCancellationToken>(CancellationToken), i == Canceller ? true : false);
		Tasks.Add(Task);
	}

	WaitUntil(bIsDone, "Waiting ...");

	AB_TEST_TRUE(bIsDone);
	for (auto& Task : Tasks)
	{
		AB_TEST_TRUE(Task->IsCancelled());
	}

	for (auto& Task : Tasks)
	{
		Task->EnsureCompletion();
	}

	AB_TEST_TRUE(TeardownTestUsers(HttpClientTestUsers));

	return bIsDone;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(DoCancelOneofManyRequestsTest, "AccelByte.Tests.Core.HttpClient.DoCancelOneofManyRequests", AutomationFlagMaskCustomization)
void DoCancelOneofManyRequestsTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("GET");
	OutTestCommands.Add("GET");
	OutBeautifiedNames.Add("POST");
	OutTestCommands.Add("POST");
	OutBeautifiedNames.Add("PUT");
	OutTestCommands.Add("PUT");
	OutBeautifiedNames.Add("DELETE");
	OutTestCommands.Add("DELETE");
}

bool DoCancelOneofManyRequestsTest::RunTest(const FString& Method)
{
	AB_TEST_TRUE(SetupTestUsers(1, HttpClientTestUsers));

	const int RequestCount = 10;
	TArray<TSharedPtr<FAccelByteCancellationToken>> CancellationTokens;
	int CancelledCount = 0;

	int DoneRequestCount = 0;
	int SuccessRequestCount = 0;
	bool bAllIsDone = false;

	const FString Query = TEXT("=123");

	const TMap<FString, FString> Content = {
		{"name", "John Doe"},
		{"age", "24"},
	};

	FHttpClient HttpClient(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

	const FString Url = "https://httpbin.org/anything/{namespace}/{userId}"; // {namespace} and {userId} is going to be replaced by actual value in credentials

	const int Canceller = FMath::RandRange(0, (RequestCount-1));
	for (int i = 0; i < RequestCount; i++)
	{
		FAccelByteCancellationToken CancellationToken = HttpClient.ApiRequest(Method, Url, { { "q", Query } }, Content,
		THandler<FHttpClientTestAnythingResponse>::CreateLambda([&](const FHttpClientTestAnythingResponse& Response)
			{
				SuccessRequestCount++;
				DoneRequestCount++;
				if (DoneRequestCount == RequestCount)
				{
					bAllIsDone = true;
				}
			}),
		FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
			{
				DoneRequestCount++;
				CancelledCount++;
				if (DoneRequestCount == RequestCount)
				{
					bAllIsDone = true;
				}
			}))->GetCancellationToken();
		
		if (i == Canceller)
		{
			CancellationToken.Cancel();
		}
		CancellationTokens.Add(MakeShared<FAccelByteCancellationToken>(CancellationToken));
	}

	WaitUntil(bAllIsDone, "Waiting ...");

	AB_TEST_TRUE(bAllIsDone);

	AB_TEST_EQUAL(CancelledCount, 1);
	AB_TEST_TRUE(TeardownTestUsers(HttpClientTestUsers));

	return bAllIsDone;
}