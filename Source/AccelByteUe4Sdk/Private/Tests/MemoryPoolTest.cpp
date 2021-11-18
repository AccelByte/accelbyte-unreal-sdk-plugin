// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Core/AccelByteMemoryPool.h"
#include "Core/AccelByteHttpCache.h"
#include "Runtime/Online/HTTP/Public/HttpManager.h"
#include "TestUtilities.h"
#include "Engine/World.h"
#include "Core/AccelByteHttpClient.h"
#include "HttpClientTest.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using namespace AccelByte::Core;

DECLARE_LOG_CATEGORY_EXTERN(LogMemoryPoolTest, Log, All);
DEFINE_LOG_CATEGORY(LogMemoryPoolTest);

struct DummyStruct
{
	int Integer = 4;
	FString String = "ABCDEFG";
	TArray<float> FloatArray = { 0.0f, 0.5f, 1.0f };
	TArray<FString> StringArray = { "AB", "CD", "EF" };
};

MemoryConstructionParameter DefaultParam = { MemoryMethod::Dynamic, 10 * 1024 * 1024, 100 };

const int32 AutomationFlagMaskMemoryPool = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

BEGIN_DEFINE_SPEC(FMemoryPoolTestSpec, "AccelByte.Tests.Core.MemoryPool", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext)

END_DEFINE_SPEC(FMemoryPoolTestSpec)
void FMemoryPoolTestSpec::Define()
{
	Describe("Dynamic", [this]()
	{
		Describe("Given FString Data", [this]()
		{
			auto Memory = FAccelByteMemoryFactory<FString>::Create(DefaultParam);
			FString Content = "TEST";
			const FName Key = "FString";
			It("1.Insert to the Memory", [=]()
			{
				FString Content_ = Content;
				auto Result = Memory->Insert(Content_, Key);
				AB_TEST_EQUAL(*Result->Data, Content);
				AB_TEST_TRUE(Result->Key == Key);
				AB_TEST_TRUE(Result->Length <= sizeof(Content));
				AB_TEST_TRUE(Result->Pool == Memory.Get());
				return true;
			});
			
			It("2.Get from the Memory", [=]()
			{
				auto Result = Memory->Get(Key);
				AB_TEST_EQUAL(*Result, Content);
				AB_TEST_TRUE(sizeof(*Result) == sizeof(Content));
				return true;
			});

			It("3.Remove from the Memory", [=]()
			{
				Memory->Remove(Key);
				auto Result = Memory->Get(Key);
				AB_TEST_TRUE(Result == nullptr);
				return true;
			});
		});

		Describe("Given Array<FString> Data", [this]()
		{
			FName Key = "ArrayOfString";
			TArray<FString> ArrayContent;
			for (size_t i = 0; i < DefaultParam.ChunkCount; i++)
			{
				ArrayContent.Add(FString::Printf(TEXT("%d"), i));
			}
			auto Memory = FAccelByteMemoryFactory<TArray<FString>>::Create(DefaultParam);

			It("1.Insert to the Memory", [=]()
			{
				TArray<FString> ArrayContent_ = ArrayContent;
				const FChunkInfo<TArray<FString>> * Result;
				{
					Result = Memory->Insert(ArrayContent_, Key);
				}

				// After the Array of FString is copied by the memory pool, somehow the size is a bit smaller than the original array.
				AB_TEST_TRUE(Result->Data.GetAllocatedSize() <= ArrayContent.GetAllocatedSize());
				AB_TEST_EQUAL(Result->Pool, Memory.Get());
				return true;
			});

			It("2.Get from the Memory", [=]()
			{
				auto Result = Memory->Get(Key);

				AB_TEST_TRUE(Result->GetAllocatedSize() <= ArrayContent.GetAllocatedSize());
				for (size_t i = 0; i < DefaultParam.ChunkCount; i++)
				{
					FString Actual = Result->operator[](i);
					AB_TEST_EQUAL(Actual, ArrayContent[i]);
				}
				return true;
			});

			It("3.Remove from the Memory", [=]()
			{
				Memory->Remove(Key);
				auto Result = Memory->Get(Key);

				AB_TEST_TRUE(Result == nullptr);
				return true;
			});
		});

		Describe("Given Struct Data", [this]()
		{
			auto Memory = FAccelByteMemoryFactory<DummyStruct>::Create(DefaultParam);
			const FName& Key = "StructKey";
			const DummyStruct DefaultContent;

			It("1. When Insert to the Memory", [=]()
			{
				const FChunkInfo<DummyStruct> * Result;
				{
					DummyStruct OriginalContent;
					Result = Memory->Insert(OriginalContent, Key);
				}

				AB_TEST_EQUAL(Result->Data.Integer, DefaultContent.Integer);
				AB_TEST_EQUAL(Result->Data.String, DefaultContent.String);
				AB_TEST_EQUAL(Result->Data.FloatArray, DefaultContent.FloatArray);
				AB_TEST_EQUAL(Result->Data.StringArray, DefaultContent.StringArray);
				AB_TEST_TRUE(Result->Key == Key);
				AB_TEST_TRUE(Result->Length == sizeof(DefaultContent));
				AB_TEST_TRUE(Result->Pool == Memory.Get());
				return true;
			});

			It("2. Get from the Memory", [=]()
			{
				auto Result = Memory->Get(Key);
				AB_TEST_TRUE(Result != nullptr);
				AB_TEST_EQUAL(Result->Integer, DefaultContent.Integer);
				AB_TEST_EQUAL(Result->String, DefaultContent.String);
				AB_TEST_EQUAL(Result->FloatArray, DefaultContent.FloatArray);
				AB_TEST_EQUAL(Result->StringArray, DefaultContent.StringArray);
				return true;
			});

			It("3. Remove from the Memory", [=]()
			{
				Memory->Remove(Key);
				auto Result = Memory->Get(Key);
				AB_TEST_TRUE(Result == nullptr);
				return true;
			});
		});

		Describe("Given UStruct Data", [this]()
		{
			auto Memory = FAccelByteMemoryFactory<FHttpClientTestAnythingResponse>::Create(DefaultParam);
			const FName& Key = "UStruct";
			FString ExpectedMethod = "POST";
			FString ExpectedUrl = "https://httpbin.org/anything";
			FString ExpectedArgKey1 = "arg1"; FString ExpectedArgValue1 = "A";
			FString ExpectedArgKey2 = "argB"; FString ExpectedArgValue2 = "B";

			It("1. Insert to the Memory", [=]()
			{
				const FChunkInfo<FHttpClientTestAnythingResponse> * Result;
				{
					FHttpClientTestAnythingResponse UStruct;
					UStruct.Method = ExpectedMethod;
					UStruct.Url = ExpectedUrl;
					UStruct.Args.Add(ExpectedArgKey1, ExpectedArgValue1);
					UStruct.Args.Add(ExpectedArgKey2, ExpectedArgValue2);
					Result = Memory->Insert(UStruct, Key);
				}

				AB_TEST_EQUAL(Result->Data.Method, ExpectedMethod);
				AB_TEST_EQUAL(Result->Data.Url, ExpectedUrl);
				AB_TEST_EQUAL(Result->Data.Args[ExpectedArgKey1], ExpectedArgValue1);
				AB_TEST_EQUAL(*Result->Data.Args.FindKey(ExpectedArgValue2), ExpectedArgKey2);
				AB_TEST_TRUE(Result->Pool == Memory.Get());
				return true;
			});

			It("2. Get from the Memory", [=]()
			{
				auto Result = Memory->Get(Key);
				AB_TEST_EQUAL(Result->Method, ExpectedMethod);
				AB_TEST_EQUAL(Result->Url, ExpectedUrl);
				AB_TEST_EQUAL(Result->Args[ExpectedArgKey1], ExpectedArgValue1);
				AB_TEST_EQUAL(*Result->Args.FindKey(ExpectedArgValue2), ExpectedArgKey2);
				return true;
			});

			It("3. Remove from the Memory", [=]()
			{
				Memory->Remove(Key);
				auto Result = Memory->Get(Key);
				AB_TEST_TRUE(Result == nullptr);
				return true;
			});
		});

		Describe("Given TSharedRef Data", [this]()
		{
			auto Memory = FAccelByteMemoryFactory<TSharedRef<FHttpClientTestAnythingResponse>>::Create(DefaultParam);
			const FName& Key = "TSharedRef";
			FString ExpectedMethod = "POST";
			FString ExpectedUrl = "https://httpbin.org/anything";
			FString ExpectedArgKey1 = "arg1"; FString ExpectedArgValue1 = "A";
			FString ExpectedArgKey2 = "argB"; FString ExpectedArgValue2 = "B";

			It("1. Insert to the Memory", [=]()
			{
				const FChunkInfo<TSharedRef<FHttpClientTestAnythingResponse>> * Result;
				{
					FHttpClientTestAnythingResponse UStruct;
					UStruct.Method = ExpectedMethod;
					UStruct.Url = ExpectedUrl;
					UStruct.Args.Add(ExpectedArgKey1, ExpectedArgValue1);
					UStruct.Args.Add(ExpectedArgKey2, ExpectedArgValue2);
					TSharedRef<FHttpClientTestAnythingResponse> SharedRef = MakeShared<FHttpClientTestAnythingResponse>(UStruct);
					Result = Memory->Insert(SharedRef, Key);
				}

				AB_TEST_EQUAL(Result->Data->Method, ExpectedMethod);
				AB_TEST_EQUAL(Result->Data->Url, ExpectedUrl);
				AB_TEST_EQUAL(Result->Data->Args[ExpectedArgKey1], ExpectedArgValue1);
				AB_TEST_EQUAL(*Result->Data->Args.FindKey(ExpectedArgValue2), ExpectedArgKey2);
				AB_TEST_TRUE(Result->Pool == Memory.Get());
				return true;
			});
			
			It("2. Get from the Memory", [=]()
			{
				auto Result = Memory->Get(Key);
				AB_TEST_EQUAL(Result->Get().Method, ExpectedMethod);
				AB_TEST_EQUAL(Result->Get().Url, ExpectedUrl);
				AB_TEST_EQUAL(Result->Get().Args[ExpectedArgKey1], ExpectedArgValue1);
				AB_TEST_EQUAL(*Result->Get().Args.FindKey(ExpectedArgValue2), ExpectedArgKey2);
				return true;
			});

			It("3. Remove from the Memory", [=]()
			{
				Memory->Remove(Key);
				auto Result = Memory->Get(Key);
				AB_TEST_TRUE(Result == nullptr);
				return true;
			});
		});

		Describe("RespondedHttp", [this]()
		{
			TArray<TTuple<FString, FString>> Headers = { TTuple<FString, FString>("Authorization", "Bearer 01223456789"), TTuple<FString, FString>("Content-Type", "application/x-www-form-urlencoded") };
			const FString Content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut pharetra mi nunc, at porta enim condimentum non. Ut in volutpat urna. Morbi a fringilla leo, eget ornare eros. Proin sem ligula, aliquam id porta eu, consequat sed velit.";
			FName Key = "HttpRequest";
			const FString URL = "https://httpbin.org/anything";
			auto Memory = FAccelByteMemoryFactory<FHttpRequestPtr>::Create(DefaultParam);

			It("1. Insert to the Memory", [=]()
			{
				const FChunkInfo<FHttpRequestPtr> * InsertResult;

				{
					FHttpRequestPtr HttpRequestPtr = FHttpModule::Get().CreateRequest();
					InsertResult = Memory->Insert(HttpRequestPtr, Key);
					HttpRequestPtr->SetURL(URL);
					HttpRequestPtr->SetContentAsString(Content);
					HttpRequestPtr->SetHeader(Headers[0].Key, Headers[0].Value);
					HttpRequestPtr->SetHeader(Headers[1].Key, Headers[1].Value);
					HttpRequestPtr->SetVerb(FString("POST"));

					bool IsRequestComplete = false;

					FRegistry::HttpRetryScheduler.ProcessRequest(
						HttpRequestPtr,
						FHttpRequestCompleteDelegate::CreateLambda(
							[&](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
							{
								IsRequestComplete = true;
							}),
						FPlatformTime::Seconds());

					WaitUntil(TFunction<bool()>([&]()
						{
							HttpRequestPtr->Tick(0.1f);
							return IsRequestComplete;
						}), "Waiting");
				}

				auto ResultRequestContent = InsertResult->Data->GetContent();
				FString ResultRequestContentAsString = "";
				for (auto a : ResultRequestContent)
				{
					ResultRequestContentAsString += static_cast<char>(a);
				}

				auto Response = InsertResult->Data->GetResponse();
				auto ResultRequestHeaders = InsertResult->Data->GetAllHeaders();

				AB_TEST_EQUAL(InsertResult->Data->GetURL(), URL);
				AB_TEST_EQUAL(ResultRequestHeaders[0], FString("") + Headers[0].Key + ": " + Headers[0].Value);
				AB_TEST_EQUAL(ResultRequestHeaders[1], FString("") + Headers[1].Key + ": " + Headers[1].Value);
				AB_TEST_EQUAL(ResultRequestContentAsString, Content);
				AB_TEST_EQUAL(InsertResult->Pool, Memory.Get());

				AB_TEST_TRUE(Response->GetResponseCode() >= 200);
				AB_TEST_TRUE(Response->GetContentAsString().IsEmpty() == false);

				FHttpClientTestAnythingResponse JsonObject;
				FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &JsonObject, 0, 0);
				AB_TEST_EQUAL(*JsonObject.Form.FindKey(""), Content);

				return true;
			});
			
			It("2. Get from the Memory", [=]()
			{
				auto Result = Memory->Get(Key);

				auto ResultRequestContent = Result->Get()->GetContent();
				FString ResultRequestContentAsString = "";
				for (auto a : ResultRequestContent)
				{
					ResultRequestContentAsString += static_cast<char>(a);
				}

				auto Response = Result->Get()->GetResponse();
				auto ResultRequestHeaders = Result->Get()->GetAllHeaders();

				AB_TEST_EQUAL(Result->Get()->GetURL(), URL);
				AB_TEST_EQUAL(ResultRequestHeaders[0], FString("") + Headers[0].Key + ": " + Headers[0].Value);
				AB_TEST_EQUAL(ResultRequestHeaders[1], FString("") + Headers[1].Key + ": " + Headers[1].Value);
				AB_TEST_EQUAL(ResultRequestContentAsString, Content);

				AB_TEST_TRUE(Response->GetResponseCode() >= 200);
				AB_TEST_TRUE(Response->GetContentAsString().IsEmpty() == false);

				FHttpClientTestAnythingResponse JsonObject;
				FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &JsonObject, 0, 0);
				AB_TEST_EQUAL(*JsonObject.Form.FindKey(""), Content);

				return true;
			});
			
			It("3. Remove from the Memory", [=]()
			{
				Memory->Remove(Key);
				auto Result = Memory->Get(Key);
				AB_TEST_TRUE(Result == nullptr);
				return true;
			});
		});

		Describe("Misc", [&]()
		{
			It("Would not store more than the maximum chunk count", [&]()
			{
				auto Memory = FAccelByteMemoryFactory<FString>::Create(DefaultParam);
				for (size_t i = 0; i < DefaultParam.ChunkCount; i++)
				{
					FString Key = FString::Printf(TEXT("%d"), i);
					FName KeyName = FName(Key);
					FString Value = "value";
					Memory->Insert(Value, KeyName);
				}

				FName Key100 = "100";
				FString Value = "100";
				auto InsertResult = Memory->Insert(Value, Key100);
				auto GetResult = Memory->Get(Key100);

				AB_TEST_TRUE(InsertResult == nullptr);
				AB_TEST_TRUE(GetResult == nullptr);
				return true;
			});

			It("Get a non-existent key from memory pool", [&]()
			{
				auto Memory = FAccelByteMemoryFactory<FString>::Create(DefaultParam);
				FName Key = "INVALID";
				auto GetResult = Memory->Get(Key);
				AB_TEST_TRUE(GetResult == nullptr);
				return true;
			});

			It("Remove a non-existent key from memory pool", [&]()
			{
				auto Memory = FAccelByteMemoryFactory<FString>::Create(DefaultParam);
				FName Key = "INVALID";
				Memory->Remove(Key);
				return true;
			});
		});
	});
}
