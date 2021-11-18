// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "HttpCacheTest.h"
#include "Misc/AutomationTest.h"

#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteHttpCache.h"
#include "UserTestAdmin.h"
#include "TestUtilities.h"
#include "Sockets.h"
#include "Networking.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::Core::FAccelByteHttpCache;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpCacheTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpCacheTest);

BEGIN_DEFINE_SPEC(FHttpCacheTestSpec, "AccelByte.Tests.Core.HttpCache",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask
)

TArray<FTestUser> HttpClientTestUsers;
TArray<TSharedPtr<AccelByte::FHttpClient>> HttpClients;

FHttpCacheTestResponse Result;

const int ResponseWaitLimit = 10;
bool bIsDone = false;
bool bIsSuccess = false;
bool bOriginalValueIsCacheEnabled;

const double NetworkRoundTripTimeThreshold = .002f; // min network ping at 1ms, roundtrip means 2ms
double TimePreRequest;
double TimePostResponse;

const THandler<FHttpCacheTestResponse> OnSuccess = THandler<FHttpCacheTestResponse>::CreateLambda([&](const FHttpCacheTestResponse& Response)
	{
		Result = Response;
		bIsSuccess = true;
		bIsDone = true;
		UE_LOG(LogAccelByteHttpCacheTest, VeryVerbose, TEXT("Success Response"));
	});

const FErrorHandler OnError = FErrorHandler::CreateLambda([&](const int32 Code, const FString& Message)
	{
		bIsDone = true;
		UE_LOG(LogAccelByteHttpCacheTest, VeryVerbose, TEXT("Error Response with code: [%d] -> [%s]"), Code, *Message);
	});

// Base Http Request Helper
FAccelByteHttpRetryTaskPtr TestRequest(
	const FString& Url
	, const THandler<FHttpCacheTestResponse> SuccessHandler
	, bool bShouldWaitForResponse = true
	, const FJsonSerializableKeyValueMap& Params = {}
)
{
	bIsDone = false;
	if (HttpClients.Num() > 0)
	{
		const FString Method = TEXT("GET");
		const FHttpCacheTestRequest EmptyContent{};
		FAccelByteTaskPtr TaskPtr = HttpClients[0]->ApiRequest(Method, Url, Params, EmptyContent, SuccessHandler, OnError);

		if (bShouldWaitForResponse)
		{
			WaitUntil([&]() { return bIsDone; }, "Waiting for response...", ResponseWaitLimit);
		}

		FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(TaskPtr));

		return HttpRetryTaskPtr;
	}

	return nullptr;
}

// Base Http Request Helper with default OnSuccess handler
FAccelByteHttpRetryTaskPtr TestRequest(
	const FString& Url
	, bool bShouldWaitForResponse = true
	, const FJsonSerializableKeyValueMap& Params = {}
)
{
	return TestRequest(Url,OnSuccess, bShouldWaitForResponse, Params);
}

// Http Request helper with custom cache control directive
FAccelByteHttpRetryTaskPtr TestCacheControlRequest(const FString& CacheControlDirectives, const THandler<FHttpCacheTestResponse> SuccessHandler)
{
	const FJsonSerializableKeyValueMap Params{ { "cache-control", CacheControlDirectives } };
	return TestRequest("https://httpbin.org/response-headers", SuccessHandler, true, Params);
}

// Http Request helper with custom cache control directive and default OnSuccess handler
FAccelByteHttpRetryTaskPtr TestCacheControlRequest(const FString& CacheControlDirectives)
{
	return TestCacheControlRequest(CacheControlDirectives, OnSuccess);
}

void StartCachedDetectionTimer()
{
	TimePreRequest = FPlatformTime::Seconds();
}

// we detect if a callback is from cached, by measuring the duration it took from request to response
// there is a big difference between cpu process time and network process time
bool FinishCachedDetectionTimer()
{
	TimePostResponse = FPlatformTime::Seconds(); // Microseconds resolution in windows
	const double ProcessTimeDuration = TimePostResponse - TimePreRequest;
	const bool bClientSideLocalCachedDuration = ProcessTimeDuration < NetworkRoundTripTimeThreshold;
	UE_LOG(LogAccelByteHttpCacheTest, VeryVerbose, TEXT("CachedDuration [%s] = ProcessDuration [%f] < NetworkRoundTripThreshold [%f]. {Time: PreRequest [%f], PostResponse [%f]}")
	, bClientSideLocalCachedDuration?TEXT("true"): TEXT("false"), ProcessTimeDuration, NetworkRoundTripTimeThreshold, TimePreRequest, TimePostResponse);

	return bClientSideLocalCachedDuration;
}

END_DEFINE_SPEC(FHttpCacheTestSpec)

#define TEST_REQUEST_RESPONSE_CODE(Code)\
	FAccelByteHttpRetryTaskPtr Task_##Code = TestRequest("https://httpbin.org/status/"#Code);\
	const FHttpRequestPtr Req_##Code = Task_##Code->GetHttpRequest();\
	AB_TEST_FALSE(FAccelByteHttpCache::IsResponseCacheable(Req_##Code));\

#define REQUEST_RESPONSE_CACHE_CONTROL(VarName,CacheControlDirective)\
	UE_LOG(LogAccelByteHttpCacheTest, VeryVerbose, TEXT(#VarName));\
	const FAccelByteHttpRetryTaskPtr Task_##VarName = TestCacheControlRequest(CacheControlDirective);\
	const FHttpRequestPtr Req_##VarName = Task_##VarName->GetHttpRequest();\

#define TEST_REQUEST_RESPONSE_CACHE_CONTROL(VarName,CacheControlDirective,Cacheable)\
		REQUEST_RESPONSE_CACHE_CONTROL(VarName,CacheControlDirective)\
		AB_TEST_##Cacheable(FAccelByteHttpCache::IsResponseCacheable(Req_##VarName));\

void FHttpCacheTestSpec::Define()
{

	BeforeEach([this]()
		{
			SetupTestUsers(1, HttpClientTestUsers);
			HttpClients.Add(MakeShared<AccelByte::FHttpClient>(HttpClientTestUsers[0].Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler));  // For testing purposes only

			bOriginalValueIsCacheEnabled = UAccelByteBlueprintsSettings::IsHttpCacheEnabled();
			UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(true);
		});

	AfterEach([this]()
		{
			TeardownTestUsers(HttpClientTestUsers);
			HttpClients.Empty();

			UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(bOriginalValueIsCacheEnabled);
		});

	Describe("Unit Test", [&]()
		{

			It("Should be able to determine whether a response is cacheable", [&]()
				{

					{ // test request with pending response
						const FAccelByteHttpRetryTaskPtr Task_PendingResponse = TestRequest("https://httpbin.org/anything", false);
						const FHttpRequestPtr Req_PendingResponse = Task_PendingResponse->GetHttpRequest();
						AB_TEST_FALSE(FAccelByteHttpCache::IsResponseCacheable(Req_PendingResponse));
						WaitUntil([&]() { return bIsDone; }, "Waiting for response...");
					}

					{	// test non 200 response code
						//TEST_REQUEST_RESPONSE_CODE(100); // will time out
						TEST_REQUEST_RESPONSE_CODE(204);
						TEST_REQUEST_RESPONSE_CODE(300);
						TEST_REQUEST_RESPONSE_CODE(400);
						//TEST_REQUEST_RESPONSE_CODE(500); //will trigger retries
					}

					{	// Single
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(MaxAge0		, "max-age=0"	, FALSE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(MaxAge900	, "max-age=900" , TRUE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(NoCache		, "no-cache"	, TRUE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(NoStore		, "no-store"	, FALSE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(Private		, "private"		, TRUE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(Public		, "public"		, TRUE);
						
						//Need to expand macro for this specific case, because it cannot pass any test/assertion
						//TEST_REQUEST_RESPONSE_CACHE_CONTROL(Immutable	, "immutable"	, FALSE); 
						UE_LOG(LogAccelByteHttpCacheTest, VeryVerbose, TEXT("Immutable"));
						const FAccelByteHttpRetryTaskPtr Task_Immutable = TestCacheControlRequest("Immutable");
						const FHttpRequestPtr Req_Immutable = Task_Immutable->GetHttpRequest();
						AB_TEST_TRUE(FAccelByteHttpCache::IsResponseCacheable(Req_Immutable));

					}

					{	// Multiple directives
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(PrivateMaxAge900		, "private,max-age=900"			, TRUE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(PrivateMaxAge900NoStore	, "private,max-age=900,no-store", FALSE);
						TEST_REQUEST_RESPONSE_CACHE_CONTROL(ImmutableMaxAge900		, "immutable,max-age=900"		, TRUE);
					}

					return true;
				});

			It("Should be able to enable/disable Http Cache", [&]()
				{
					//Initial request for caching
					REQUEST_RESPONSE_CACHE_CONTROL(InitialRequestForCache, "immutable");

					// Cache enable 1
					UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(true);
					StartCachedDetectionTimer();
					REQUEST_RESPONSE_CACHE_CONTROL(TestForCached, "immutable");
					UTEST_TRUE("TestForCached", FinishCachedDetectionTimer());

					// Cache disable 1
					UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(false);
					StartCachedDetectionTimer();
					REQUEST_RESPONSE_CACHE_CONTROL(TestForNotCached, "immutable");
					UTEST_FALSE("TestForNotCached", FinishCachedDetectionTimer());

					// Cache enable 2
					UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(true);
					StartCachedDetectionTimer();
					REQUEST_RESPONSE_CACHE_CONTROL(TestForCached2, "immutable");
					UTEST_TRUE("TestForCached2", FinishCachedDetectionTimer());

					// Cache disable 2
					UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(false);
					StartCachedDetectionTimer();
					REQUEST_RESPONSE_CACHE_CONTROL(TestForNotCached2, "immutable");
					UTEST_FALSE("TestForNotCached2", FinishCachedDetectionTimer());

					return true;
				});

			It("Should not be caching DELETE response", [&]()
				{
					bIsDone = false;
					if (HttpClients.Num() > 0)
					{
						const FString Method = TEXT("DELETE");
						const FHttpCacheTestRequest EmptyContent{};
						FAccelByteTaskPtr TaskPtr = HttpClients[0]->ApiRequest(Method, "https://httpbin.org/delete", FJsonSerializableKeyValueMap(), EmptyContent, OnSuccess, OnError);

						WaitUntil([&]() { return bIsDone; }, "Waiting for response...", ResponseWaitLimit);
						
						FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(TaskPtr));
						;
						AB_TEST_FALSE(FAccelByteHttpCache::IsResponseCacheable(HttpRetryTaskPtr->GetHttpRequest()));
					}

					return true;
				});

		});


	Describe("Integration Test", [&]()
		{
			It("Should be able to return cached response with correct callback delegate", [&]()
				{
					REQUEST_RESPONSE_CACHE_CONTROL(InitialCacheTrigger, "immutable"); // First Call to trigger caching

					const int RequestsLimit = 10;
					int UniqueResponseIdentifier = 0;
					for (int i = 0; i < RequestsLimit; i++)
					{
						TestCacheControlRequest("immutable",
							THandler<FHttpCacheTestResponse>::CreateLambda([&, i](const FHttpCacheTestResponse& Response)
								{
									Result = Response;
									bIsSuccess = true;
									bIsDone = true;
									UniqueResponseIdentifier = i;
									UE_LOG(LogAccelByteHttpCacheTest, VeryVerbose, TEXT("Unique response identifier %d"), i);
								}
						));
						UTEST_TRUE("Each cached response can trigger correct callback", UniqueResponseIdentifier == i);
					}

					return true;
				});
		});

	Describe("Integration Test with Local Http Server", [&]()
		{
			FString ExpectedCachedResponse = "Simulate fetch a resource from online storage success.\r\n";
			FString OriginalETag = "0123456789";

			auto CreateHttpListener = ([](FString LocalUrlAndPort)
			{
				FIPv4Endpoint EndPoint;
				FIPv4Endpoint::Parse((TEXT("%s"), *LocalUrlAndPort), EndPoint);
				FTcpListener* ListenerSocket = new FTcpListener(EndPoint);
				ListenerSocket->Init();
				return ListenerSocket;
			});

			auto CreateHttpResponse_200 = ([](int MaxAge, FString Body, FString ETag){
				return [&, Body, ETag](FSocket* Socket, const FIPv4Endpoint& Endpoint)
				{
					FString ResponseBody = Body;
					FTCHARToUTF8 ResponseBodyUtf8(*ResponseBody);
					FString ResponseHeader = FString::Printf(
						TEXT(
							"HTTP/1.1 200 OK\r\n"
							"Connection: keep-alive\r\n"
							"Content-Type: text/html; charset=UTF-8\r\n"
							"Content-Length: %d\r\n"
							"ETag: %s\r\n"
							"Cache-Control: max-age=%d\r\n"
							"\r\n"
						),
						ResponseBodyUtf8.Length(),
						*ETag,
						MaxAge);
					FString Response = ResponseHeader.Append(ResponseBody);

					FTCHARToUTF8 ResponseUtf8(*Response);
					int32 SentBytes = 0;

					Socket->Send((const uint8_t*)ResponseUtf8.Get(), ResponseUtf8.Length(), SentBytes);
					Socket->Close();
					return true;
				};
			});

			auto CreateHttpResponse_304 = ([](int MaxAge, FString ETag)
				{
					return [&, ETag](FSocket* Socket, const FIPv4Endpoint& Endpoint)
					{
						FString ResponseHeader = FString::Printf(
							TEXT(
								"HTTP/1.1 304 Not Modified\r\n"
								"Connection: keep-alive\r\n"
								"Content-Type: text/html; charset=UTF-8\r\n"
								"Content-Length: 0\r\n"
								"ETag: %s\r\n"
								"Cache-Control: max-age=%d\r\n\r\n"
							),
							*ETag,
							MaxAge
						);

						FTCHARToUTF8 ResponseUtf8(*ResponseHeader);
						int32 SentBytes = 0;

						Socket->Send((const uint8_t*)ResponseUtf8.Get(), ResponseUtf8.Length(), SentBytes);
						Socket->Close();
						return true;
					};
				});

			It("1. Call online storage service twice and reuse the cache when got 304", [=]()
				{
					FString LocalUrlAndPort = "127.0.0.1:7890";
					auto ListenerSocket = CreateHttpListener(LocalUrlAndPort);

					//FIRST CALL 200
					ListenerSocket->OnConnectionAccepted().BindLambda(CreateHttpResponse_200(999, ExpectedCachedResponse, OriginalETag));

					const FString Method = TEXT("GET");
					const FHttpCacheTestRequest EmptyContent{};
					bool bIsDoneLocal = false;
					auto OnLocalHttpCallSuccess = THandler<FString>::CreateLambda([&](const FString& Payload)
					{
						UE_LOG(LogAccelByteHttpCacheTest, Log, TEXT("Response from local HTTP server:\n%s"), *Payload);
						bIsDoneLocal = true;
					});
					FAccelByteTaskPtr TaskPtr = HttpClients[0]->ApiRequest(Method, LocalUrlAndPort, FJsonSerializableKeyValueMap(), EmptyContent, OnLocalHttpCallSuccess, OnError);

					WaitUntil([&]() { return bIsDoneLocal; }, "Waiting for response...", ResponseWaitLimit);

					FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(TaskPtr));

					AB_TEST_TRUE(FAccelByteHttpCache::IsResponseCacheable(HttpRetryTaskPtr->GetHttpRequest()));

					//SECOND CALL 304 still not modified
					ListenerSocket->OnConnectionAccepted().BindLambda(CreateHttpResponse_304(999, OriginalETag));

					bIsDoneLocal = false;
					FString ActualResponse;
					OnLocalHttpCallSuccess = THandler<FString>::CreateLambda([&](const FString& Payload)
					{
						UE_LOG(LogAccelByteHttpCacheTest, Log, TEXT("Expecting same response from local HTTP server:\n%s"), *Payload);
						ActualResponse = Payload;
						bIsDoneLocal = true;
					});
					TaskPtr = HttpClients[0]->ApiRequest(Method, LocalUrlAndPort, FJsonSerializableKeyValueMap(), EmptyContent, OnLocalHttpCallSuccess, OnError);
					WaitUntil([&]() { return bIsDoneLocal; }, "Waiting for response...", ResponseWaitLimit);

					AB_TEST_FALSE(FAccelByteHttpCache::IsResponseCacheable(StaticCastSharedPtr< FHttpRetryTask >(TaskPtr)->GetHttpRequest()));
					AB_TEST_TRUE(ExpectedCachedResponse == ActualResponse);

					ListenerSocket->Stop();
					delete ListenerSocket;
					return true;
				});

			It("2. Call online storage service twice but the second one is already updated or modified", [=]()
				{
					FString LocalUrlAndPort = "127.0.0.1:7891";
					auto ListenerSocket = CreateHttpListener(LocalUrlAndPort);

					int AgeOfFirstResponse = 101;

					//FIRST CALL 200
					ListenerSocket->OnConnectionAccepted().BindLambda(CreateHttpResponse_200(AgeOfFirstResponse, ExpectedCachedResponse, OriginalETag));

					const FString Method = TEXT("GET");
					const FHttpCacheTestRequest EmptyContent{};
					bool bIsDoneLocal = false;
					auto OnLocalHttpCallSuccess = THandler<FString>::CreateLambda([&](const FString& Payload)
					{
						UE_LOG(LogAccelByteHttpCacheTest, Log, TEXT("Response from local HTTP server:\n%s"), *Payload);
						bIsDoneLocal = true;
					});
					FAccelByteTaskPtr TaskPtr = HttpClients[0]->ApiRequest(Method, LocalUrlAndPort, FJsonSerializableKeyValueMap(), EmptyContent, OnLocalHttpCallSuccess, OnError);

					WaitUntil([&]() { return bIsDoneLocal; }, "Waiting for response...", ResponseWaitLimit);

					FAccelByteHttpRetryTaskPtr HttpRetryTaskPtr(StaticCastSharedPtr< FHttpRetryTask >(TaskPtr));

					AB_TEST_TRUE(FAccelByteHttpCache::IsResponseCacheable(HttpRetryTaskPtr->GetHttpRequest()));

					FString ModifiedETag = "abcdefgh";
					FString NewExpectedResponse = "The content is already modified!";
					
					//Waiting until the first response stale
					DelaySeconds(AgeOfFirstResponse + 10, "");

					//SECOND CALL 200/modified
					ListenerSocket->OnConnectionAccepted().BindLambda(CreateHttpResponse_200(999, NewExpectedResponse, ModifiedETag));

					bIsDoneLocal = false;
					FString ActualResponse;
					OnLocalHttpCallSuccess = THandler<FString>::CreateLambda([&](const FString& Payload)
					{
						UE_LOG(LogAccelByteHttpCacheTest, Log, TEXT("Response from local HTTP server:\n%s"), *Payload);
						ActualResponse = Payload;
						bIsDoneLocal = true;
					});
					TaskPtr = HttpClients[0]->ApiRequest(Method, LocalUrlAndPort, FJsonSerializableKeyValueMap(), EmptyContent, OnLocalHttpCallSuccess, OnError);
					WaitUntil([&]() { return bIsDoneLocal; }, "Waiting for response...", ResponseWaitLimit);

					AB_TEST_TRUE(FAccelByteHttpCache::IsResponseCacheable(StaticCastSharedPtr< FHttpRetryTask >(TaskPtr)->GetHttpRequest()));
					AB_TEST_TRUE(NewExpectedResponse == ActualResponse);

					ListenerSocket->Stop();
					delete ListenerSocket;
					return true;
				});
		});
}
#undef REQUEST_RESPONSE_CACHE_CONTROL
#undef TEST_REQUEST_RESPONSE_CACHE_CONTROL
#undef TEST_REQUEST_RESPONSE_CODE