// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "HttpClientTest.h"

#include "Misc/AutomationTest.h"

#include "TestUtilities.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpClient.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpClientTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpClientTest);

static const int32 AutomationFlagMaskCustomization = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

TArray<TSharedPtr<FTestUser>> HttpClientTestUsers;
TArray<TSharedPtr<Credentials>> HttpClientTestCredentials;

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
	AB_TEST_TRUE(SetupTestUsers("deadbeef", 1, HttpClientTestUsers, HttpClientTestCredentials));

	FHttpClientTestAnythingResponse Result;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	FHttpClientTestAnythingRequest Content;

	Content.Name = "John Doe";
	Content.Age = 24;

	FHttpClient HttpClient(*HttpClientTestCredentials[0], FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

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
	AB_TEST_TRUE(Result.Url.Contains(HttpClientTestCredentials[0]->GetNamespace() + "/" + HttpClientTestCredentials[0]->GetUserId()));
	AB_TEST_EQUAL(Result.Method, Method);
	AB_TEST_EQUAL(Result.Args["q"], Query);
	AB_TEST_EQUAL(Result.Headers[TEXT("Authorization")], FString::Printf(TEXT("Bearer %s"), *HttpClientTestCredentials[0]->GetAccessToken()));
	if (!Method.ToUpper().Equals(TEXT("GET")))
	{
		AB_TEST_EQUAL(Result.Json.Name, Content.Name);
		AB_TEST_EQUAL(Result.Json.Age, Content.Age);
	}

	AB_TEST_TRUE(TearDownTestUsers(HttpClientTestCredentials));

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
	AB_TEST_TRUE(SetupTestUsers("deadbeef", 1, HttpClientTestUsers, HttpClientTestCredentials));

	FHttpClientTestAnythingResponse Result;

	bool bIsDone = false;
	bool bIsSuccess = false;

	const FString Query = TEXT("=123");

	const TMap<FString, FString> Content = {
		{"name", "John Doe"},
		{"age", "24"},
	};

	FHttpClient HttpClient(*HttpClientTestCredentials[0], FRegistry::Settings, FRegistry::HttpRetryScheduler); // For testing purposes only

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
	AB_TEST_TRUE(Result.Url.Contains(HttpClientTestCredentials[0]->GetNamespace() + "/" + HttpClientTestCredentials[0]->GetUserId()));
	AB_TEST_EQUAL(Result.Args["q"], Query);
	AB_TEST_EQUAL(Result.Headers[TEXT("Authorization")], FString::Printf(TEXT("Bearer %s"), *HttpClientTestCredentials[0]->GetAccessToken()));
	if (!Method.ToUpper().Equals(TEXT("GET")))
	{
		AB_TEST_EQUAL(Result.Form[TEXT("name")], TEXT("John Doe"));
		AB_TEST_EQUAL(Result.Form[TEXT("age")], TEXT("24"));
	}

	AB_TEST_TRUE(TearDownTestUsers(HttpClientTestCredentials));

	return bIsSuccess;
}
