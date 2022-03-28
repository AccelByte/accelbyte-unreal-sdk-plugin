// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Core/AccelByteMemoryPool.h"
#include "Core/AccelByteLRUCache.h"
#include "TestUtilities.h"
#include "Engine/World.h"
#include "HttpManager.h"
#include "Core/AccelByteHttpClient.h"
#include "HttpClientTest.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using namespace AccelByte::Core;

DECLARE_LOG_CATEGORY_EXTERN(LogLRUCacheTest, Log, All);
DEFINE_LOG_CATEGORY(LogLRUCacheTest);

BEGIN_DEFINE_SPEC(FLRUCacheTestSpec, "AccelByte.Tests.Core.LRUCache", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext)

END_DEFINE_SPEC(FLRUCacheTestSpec)
void FLRUCacheTestSpec::Define()
{
	Describe("Behaviour", [this]()
	{
		TFunction<FString*(TSharedPtr<FAccelByteLRUCache<FString>> LRU, int i, bool bIsPeekOnly)> LRUAccessor =
		[](TSharedPtr<FAccelByteLRUCache<FString>> LRU,
			int i,
			bool bIsPeekOnly)
		{
			FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
			FName Key = FName(KeyString);
			if (bIsPeekOnly)
			{
				return LRU->Peek(Key);
			}
			else
			{
				return LRU->Find(Key);
			}
		};

		Describe("Emplacement", [LRUAccessor, this]()
		{
			int Count = 5;
			MemoryConstructionParameter Param = { MemoryMethod::Dynamic, 1 * 1024, Count };
			TSharedPtr<FAccelByteLRUCache<FString>> LRU = MakeShareable(new FAccelByteLRUCache<FString>(Param));

			It("1. Emplace initial data equal to capacity", [=]()
			{
				for (int i = 0; i < Count; i++)
				{
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
					FName Key = FName(KeyString);
					FString Value = FString::Printf(TEXT("%d"), i);
					AB_TEST_TRUE(LRU->Emplace(Key, Value));
				}

				int Order[] = { 4, 3, 2, 1, 0 };
				auto Node = LRU->GetHead();
				for (int i = 0 ; i < 5 ; i++)
				{
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), Order[i]);
					FName Key = FName(KeyString);
					FString Expected = FString::Printf(TEXT("%d"), Order[i]);

					AB_TEST_TRUE(Node->GetValue().Key == Key);
					Node = Node->GetNextNode();
					AB_TEST_TRUE(*LRU->Peek(Key) == Expected);
				}
				return true;
			});

			It("2. Emplace new data more than capacity", [=]()
			{
				int Overcapacity = Count + 2;
				for (int i = Count; i < Overcapacity; i++)
				{
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
					FName Key = FName(KeyString);
					FString Value = FString::Printf(TEXT("%d"), i);
					AB_TEST_TRUE(LRU->Emplace(Key, Value));
				}

				int Order[] = { 6, 5, 4, 3, 2 };
				auto Node = LRU->GetHead();
				for (int i = 0 ; i < 5 ; i++)
				{
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), Order[i]);
					FName Key = FName(KeyString);
					FString Expected = FString::Printf(TEXT("%d"), Order[i]);

					AB_TEST_TRUE(Node->GetValue().Key == Key);
					Node = Node->GetNextNode();
					AB_TEST_TRUE(*LRU->Peek(Key) == Expected);
				}
				return true;
			});

			It("3. Access the LRU cache and re-order the linked list after an access attempt", [=]()
			{
				// access Empty key
				for (int i = 0; i < 2; i++)
				{
					FString* Actual = LRUAccessor(LRU, i, false);
					AB_TEST_TRUE(Actual == nullptr);
				}

				// access the tail
				LRUAccessor(LRU, 2, false);
				LRUAccessor(LRU, 3, false);

				auto Node = LRU->GetHead();
				int Order[] = { 3, 2, 6, 5, 4 };
				for (int i = 0 ; i < 5; i++)
				{
					FString ExpectedValue = FString::Printf(TEXT("%d"), Order[i]);
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), Order[i]);
					FName Key = FName(KeyString);

					FString* Actual = LRU->Peek(Key);
					AB_TEST_TRUE(*Actual == ExpectedValue);
					AB_TEST_TRUE(Node->GetValue().Key == Key);
					Node = Node->GetNextNode();
				}
				return true;
			});
		});
		
		Describe("Removal", [LRUAccessor, this]()
		{
			int Count = 10;
			MemoryConstructionParameter Param = { MemoryMethod::Dynamic, 10 * 1024 * 1024, Count };
			TSharedPtr<FAccelByteLRUCache<FString>> LRU = MakeShareable(new FAccelByteLRUCache<FString>(Param));

			for (int i = 0; i < Count; i++)
			{
				FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				FString Value = FString::Printf(TEXT("%d"), i);
				AB_TEST_TRUE(LRU->Emplace(Key, Value));
			}

			It("1. Remove the tail from the LRU cache ", [=]()
			{
				FString KeyString = "Key0";
				FName Key = FName(KeyString);
				AB_TEST_TRUE(LRU->Remove(Key));

				FString* Actual = LRUAccessor(LRU, 0, true);
				AB_TEST_TRUE(Actual == nullptr);
				for (int i = 1; i < Count; i++)
				{
					AB_TEST_TRUE(*LRUAccessor(LRU, i, true) == FString::Printf(TEXT("%d"), i));
				}
				return true;
			});

			It("2. Remove a node from the middle of LRU cache ", [=]()
			{
				FString RemovedKeyString = "Key3";
				FName RemovedKey = FName(RemovedKeyString);
				AB_TEST_TRUE(LRU->Remove(RemovedKey));

				int Order[] = { 9, 8, 7, 6, 5, 4, 2, 1 }; // 3 should be removed, 0 is already removed too
				auto Node = LRU->GetHead();
				for (int i = 0; i < 8; i++)
				{
					AB_TEST_TRUE(*LRUAccessor(LRU, Order[i], true) == FString::Printf(TEXT("%d"), Order[i]));
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), Order[i]);
					FName Key = FName(KeyString);
					AB_TEST_TRUE(Node->GetValue().Key == Key);
					Node = Node->GetNextNode();
				}
				return true;
			});

			It("3. Remove the head of LRU cache ", [=]()
			{
				FString RemovedKeyString = "Key9";
				FName RemovedKey = FName(RemovedKeyString);
				AB_TEST_TRUE(LRU->Remove(RemovedKey));

				int Order[] = { 8, 7, 6, 5, 4, 2, 1 }; // 9 should be removed
				auto Node = LRU->GetHead();
				for (int i = 0; i < 7; i++)
				{
					AB_TEST_TRUE(*LRUAccessor(LRU, Order[i], true) == FString::Printf(TEXT("%d"), Order[i]));
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), Order[i]);
					FName Key = FName(KeyString);
					AB_TEST_TRUE(Node->GetValue().Key == Key);
					Node = Node->GetNextNode();
				}
				return true;
			});
			
			return true;
		});

		It("Contains", [this]()
		{
			int Count = 10;
			MemoryConstructionParameter Param = { MemoryMethod::Dynamic, 10 * 1024 * 1024, Count };
			TSharedPtr<FAccelByteLRUCache<FString>> LRU = MakeShareable(new FAccelByteLRUCache<FString>(Param));

			for (int i = 0; i < Count; i++)
			{
				FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				FString Value = FString::Printf(TEXT("%d"), i);
				AB_TEST_TRUE(LRU->Emplace(Key, Value));
			}

			for (int i = 0; i < Count; i++)
			{
				FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				AB_TEST_TRUE(LRU->Contains(Key));
			}
			return true;
		});

		It("Empty", [this]()
		{
			int Count = 10;
			MemoryConstructionParameter Param = { MemoryMethod::Dynamic, 10 * 1024 * 1024, Count };
			TSharedPtr<FAccelByteLRUCache<FString>> LRU = MakeShareable(new FAccelByteLRUCache<FString>(Param));

			for (int i = 0; i < Count; i++)
			{
				FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				FString Value = FString::Printf(TEXT("%d"), i);
				AB_TEST_TRUE(LRU->Emplace(Key, Value));
			}
			LRU->Empty();

			AB_TEST_TRUE(LRU->GetHead() == nullptr);
			for (int i = 0; i < Count; i++)
			{
				FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				AB_TEST_FALSE(LRU->Contains(Key));
			}
			return true;
		});
		
		Describe("The memory pool size is not enough to add more item", [this]()
		{
			It("Expect to remove the tail(s)", [this]()
			{
				int Count = 6;
				int ExpectedAmountOfItemInsideCache = 4;
				int TextLength = 256;
				size_t SizeOfEachCharInString = 2;
				size_t FStringOverhead = 2;
				size_t PoolSize = (((TextLength * SizeOfEachCharInString) + FStringOverhead) * ExpectedAmountOfItemInsideCache);
				MemoryConstructionParameter Param = { MemoryMethod::Dynamic, PoolSize, Count };
				TSharedPtr<FAccelByteLRUCache<FString>> LRU = MakeShareable(new FAccelByteLRUCache<FString>(Param));

				FString DefaultValue;
				for (int i = 0; i < TextLength; i++)
				{
					DefaultValue += "a";
				}

				for (int i = 0; i < Count; i++)
				{
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
					FName Key = FName(KeyString);
					AB_TEST_TRUE(LRU->Emplace(Key, DefaultValue));
				}

				for (int i = 0; i < Count; i++)
				{
					FString KeyString = "Key" + FString::Printf(TEXT("%d"), i);
					FName Key = FName(KeyString);
					if (i < Count - ExpectedAmountOfItemInsideCache)
					{
						AB_TEST_FALSE(LRU->Contains(Key));
					}
					else
					{
						AB_TEST_TRUE(LRU->Contains(Key));
					}
				}

				return true;
			});
		});

		Describe("MemoryPool is not enough to accept large data", [this]()
		{
			It("Expect it won't be inserted to the pool", [this]()
			{
				int TextLength = 256;
				size_t SizeOfEachCharInString = 2;
				size_t FStringOverhead = 2;
				
				size_t ItemSizeSize = TextLength * SizeOfEachCharInString + FStringOverhead;
				size_t HalfSizeMemoryPool = ItemSizeSize / 2;

				FString DefaultValue;
				for (int i = 0; i < TextLength; i++)
				{
					DefaultValue += "a";
				}

				MemoryConstructionParameter Param = { MemoryMethod::Dynamic, HalfSizeMemoryPool, 5 };
				TSharedPtr<FAccelByteLRUCache<FString>> LRU = MakeShareable(new FAccelByteLRUCache<FString>(Param));

				FName Key = "Key";
				bool bIsSuccess = LRU->Emplace(Key, DefaultValue);
				AB_TEST_FALSE(bIsSuccess);
				return true;
			});
		});
	});

	Describe("Various Data Type", [&]()
	{
		const int Capacity = 4;
		const MemoryConstructionParameter Param = { MemoryMethod::Dynamic, 10 * 1024 * 1024, Capacity };

		// Scenario for each
		// * Emplace until full (Count)
		// * Overwrite
		// * Remove
		// * Find
		// * Contains

		It("HTTP", [=]()
		{
			TSharedPtr<FAccelByteLRUCache<FHttpRequestPtr>> LRU = MakeShareable(new FAccelByteLRUCache<FHttpRequestPtr>(Param));
			TArray<TTuple<FString, FString>> Headers = { TTuple<FString, FString>("Authorization", "Bearer 01223456789"), TTuple<FString, FString>("Content-Type", "application/x-www-form-urlencoded") };
			const FString Content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut pharetra mi nunc, at porta enim condimentum non. Ut in volutpat urna. Morbi a fringilla leo, eget ornare eros. Proin sem ligula, aliquam id porta eu, consequat sed velit.";
			const FString BaseURL = "https://httpbin.org/anything";

			int OverwriteCount = 2;
			// 3,2,1,0 first emplacement
			// Then an overwrite changes it to 5,4,3,2
			for (int i = 0; i < Capacity + OverwriteCount; i++)
			{
				FString KeyString = "HttpRequest" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				FString URL = BaseURL + FString::Printf(TEXT("/post%d"), i);
				bool IsRequestComplete = false;

				FHttpRequestPtr HttpRequestPtr = FHttpModule::Get().CreateRequest();
				HttpRequestPtr->SetURL(URL);
				HttpRequestPtr->SetContentAsString(Content);
				HttpRequestPtr->SetHeader(Headers[0].Key, Headers[0].Value);
				HttpRequestPtr->SetHeader(Headers[1].Key, Headers[1].Value);
				HttpRequestPtr->SetVerb(FString("POST"));

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

				AB_TEST_TRUE(LRU->Emplace(Key, HttpRequestPtr));
			}
			
			auto Node = LRU->GetHead();
			int ExpectedOrder[] = { 5, 4, 3, 2 };
			for (int i = 0; i < Capacity; i++)
			{
				FString ContainURL = FString::Printf(TEXT("post%d"), ExpectedOrder[i]);
				AB_TEST_TRUE(Node->GetValue().Data->GetURL().Contains(ContainURL));
				Node = Node->GetNextNode();
			}

			FName RemovedKey = "HttpRequest3";
			AB_TEST_TRUE(LRU->Remove(RemovedKey));
			Node = LRU->GetHead();
			int ExpectedOrder2[] = { 5, 4, 2 };
			for (int i = 0; i < Capacity - 1 ; i++)
			{
				FString ExpectedKeyString = FString::Printf(TEXT("HttpRequest%d"), ExpectedOrder2[i]);
				FName ExpectedKey = FName(ExpectedKeyString);
				FString ContainURL = FString::Printf(TEXT("post%d"), ExpectedOrder2[i]);
				auto ActualResult = LRU->Peek(ExpectedKey);
				AB_TEST_TRUE(Node->GetValue().Data->GetURL().Contains(ContainURL));
				AB_TEST_TRUE(ActualResult->Get()->GetURL().Contains(ContainURL));
				AB_TEST_TRUE(LRU->Contains(ExpectedKey));
				Node = Node->GetNextNode();
			}

			return true;
		});
		
		It("UObject", [=]()
		{
			TSharedPtr<FAccelByteLRUCache<FHttpClientTestAnythingResponse>> LRU = MakeShareable(new FAccelByteLRUCache<FHttpClientTestAnythingResponse>(Param));
			const FString BaseURL = "https://httpbin.org/anything";

			int OverwriteCount = 2;
			for (int i = 0; i < Capacity + OverwriteCount; i++)
			{
				FString KeyString = "UObject" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				FString URL = BaseURL + FString::Printf(TEXT("/post%d"), i);
				FHttpClientTestAnythingResponse UStruct;
				UStruct.Url = URL;
				AB_TEST_TRUE(LRU->Emplace(Key, UStruct));
			}
			
			auto Node = LRU->GetHead();
			int ExpectedOrder[] = { 5, 4, 3, 2 };
			for (int i = 0; i < Capacity; i++)
			{
				FString ContainURL = FString::Printf(TEXT("post%d"), ExpectedOrder[i]);
				AB_TEST_TRUE(Node->GetValue().Data.Url.Contains(ContainURL));
				Node = Node->GetNextNode();
			}

			FName RemovedKey = "UObject3";
			AB_TEST_TRUE(LRU->Remove(RemovedKey));
			Node = LRU->GetHead();
			int ExpectedOrder2[] = { 5, 4, 2 };
			for (int i = 0; i < Capacity - 1 ; i++)
			{
				FString ExpectedKeyString = FString::Printf(TEXT("UObject%d"), ExpectedOrder2[i]);
				FName ExpectedKey = FName(ExpectedKeyString);
				FString ContainURL = FString::Printf(TEXT("post%d"), ExpectedOrder2[i]);
				auto ActualResult = LRU->Peek(ExpectedKey);
				AB_TEST_TRUE(Node->GetValue().Data.Url.Contains(ContainURL));
				AB_TEST_TRUE(ActualResult->Url.Contains(ContainURL));
				AB_TEST_TRUE(LRU->Contains(ExpectedKey));
				Node = Node->GetNextNode();
			}

			return true;
		});
		
		It("TSharedRef", [=]()
		{
			TSharedPtr<FAccelByteLRUCache<TSharedRef<FHttpClientTestAnythingResponse>>> LRU = MakeShareable(new FAccelByteLRUCache<TSharedRef<FHttpClientTestAnythingResponse>>(Param));
			const FString BaseURL = "https://httpbin.org/anything";

			int OverwriteCount = 2;
			for (int i = 0; i < Capacity + OverwriteCount; i++)
			{
				FString KeyString = "TSharedRef" + FString::Printf(TEXT("%d"), i);
				FName Key = FName(KeyString);
				FString URL = BaseURL + FString::Printf(TEXT("/post%d"), i);
				FHttpClientTestAnythingResponse UStruct;
				UStruct.Url = URL;
				TSharedRef<FHttpClientTestAnythingResponse> SharedRef = MakeShared<FHttpClientTestAnythingResponse>(UStruct);
				AB_TEST_TRUE(LRU->Emplace(Key, SharedRef));
			}
			
			auto Node = LRU->GetHead();
			int ExpectedOrder[] = { 5, 4, 3, 2 };
			for (int i = 0; i < Capacity; i++)
			{
				FString ContainURL = FString::Printf(TEXT("post%d"), ExpectedOrder[i]);
				AB_TEST_TRUE(Node->GetValue().Data->Url.Contains(ContainURL));
				Node = Node->GetNextNode();
			}

			FName RemovedKey = "TSharedRef3";
			AB_TEST_TRUE(LRU->Remove(RemovedKey));
			Node = LRU->GetHead();
			int ExpectedOrder2[] = { 5, 4, 2 };
			for (int i = 0; i < Capacity - 1 ; i++)
			{
				FString ExpectedKeyString = FString::Printf(TEXT("TSharedRef%d"), ExpectedOrder2[i]);
				FName ExpectedKey = FName(ExpectedKeyString);
				FString ContainURL = FString::Printf(TEXT("post%d"), ExpectedOrder2[i]);
				auto ActualResult = LRU->Peek(ExpectedKey);
				AB_TEST_TRUE(Node->GetValue().Data->Url.Contains(ContainURL));
				AB_TEST_TRUE(ActualResult->Get().Url.Contains(ContainURL));
				AB_TEST_TRUE(LRU->Contains(ExpectedKey));
				Node = Node->GetNextNode();
			}

			return true;
		});
	});
}
