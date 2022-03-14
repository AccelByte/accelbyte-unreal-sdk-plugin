// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpCache.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpCache, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpCache);

namespace AccelByte
{

	namespace Core
	{

		int FAccelByteHttpCache::MaxAgeCacheThreshold = 100;

		namespace HTTPHeader
		{

			namespace Cache
			{
				// The time, in seconds, that the object has been in a proxy cache
				const FString Age = TEXT("Age");

				// Directives for caching mechanisms in both requests and responses
				const FString Control = TEXT("Cache-Control");

				// Directives to identify unique version of a resource
				const FString ETag = TEXT("ETag");

				const FString ContentLocation = TEXT("Content-Location");
				const FString Date = TEXT("Date");
				const FString Expires = TEXT("Expires");

				// Directives to makes the request conditional
				const FString IfNoneMatch = TEXT("If-None-Match");

				namespace ControlDirective
				{
					
					// indicates that the response remains fresh until N seconds after the response is generated
					const FString MaxAge = TEXT("max-age");

					// allows caches to store a response, but requires them to revalidate it before reuse
					const FString NoCache = TEXT("no-cache");

					// The no-store response directive indicates that any caches of any kind (private or shared) should not store this response.
					const FString NoStore = TEXT("no-store");

					// The private response directive indicates that the response can be stored only in a private cache (e.g. local caches in browsers).
					const FString Private = TEXT("private");

					// Enables responses for requests with Authorization header fields to be stored in a shared cache.
					const FString Public = TEXT("public");

					// Static resources that are never modified
					const FString Immutable = TEXT("immutable");

					// the response can be stored in caches and can be reused while fresh. Once it becomes stale, it must be validated with the origin server before reuse
					const FString MustRevalidate = TEXT("must-revalidate");
				}

			}

			namespace Verb
			{
				const FString Delete = TEXT("DELETE");
			}

		}

		FAccelByteHttpCache::~FAccelByteHttpCache()
		{
			// empty
		};


		bool FAccelByteHttpCache::IsResponseCacheable(const FHttpRequestPtr& CompletedRequest)
		{
			if (CompletedRequest->GetVerb() == HTTPHeader::Verb::Delete)
			{
				return false;
			}

			const FHttpResponsePtr ResponsePtr = CompletedRequest->GetResponse();
			if (ResponsePtr == nullptr)
			{
				UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Request has not receive response yet"));
				return false;
			}

			//IF 304 or between 200-204
			auto ResponseCode = ResponsePtr->GetResponseCode();
			if(ResponseCode != EHttpResponseCodes::NotModified && (ResponseCode < EHttpResponseCodes::Ok || ResponseCode > EHttpResponseCodes::NoContent))
			{
				UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Request has an invalid response code"));
				return false;
			}

			bool bCacheable = true;
			const FString CacheControlHeader = ResponsePtr->GetHeader(HTTPHeader::Cache::Control);
			if (!CacheControlHeader.IsEmpty())
			{
				// check hard prevention (no-store)
				bCacheable = !CacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::NoStore);
				// re-check the requirement (immutable)
				if (CacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::Immutable))
				{
					bCacheable = true;
				}
				if (bCacheable)
				{
					FString MaxAge = ExtractControlDirective(CacheControlHeader, HTTPHeader::Cache::ControlDirective::MaxAge);

					if (MaxAge.IsEmpty() == false)
					{
						const int MaxAgeValue = FCString::Atoi(*MaxAge);
						bCacheable = bCacheable && MaxAgeValue > MaxAgeCacheThreshold;
						UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Max Age %d > Threshold %d"), MaxAgeValue, MaxAgeCacheThreshold);
					}
				}
				UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response has valid CacheControlHeader: %s"),*CacheControlHeader);
			}
			else 
			{
				//@TODO atm default behavior is to cache, even without CacheControl directives, might be better to
				// follow the convention only caching with valid cache control directive. so we dont need to handle valid verb manually
				bCacheable = false;
				UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response has empty CacheControlHeader"));
			}
		
			return bCacheable;
		}

		bool FAccelByteHttpCache::TryRetrieving(FHttpRequestPtr& Out, FHttpResponsePtr& OutCachedResponse)
		{
			bool bRetrieved = false;

			const FName Key = ConstructKey(Out);
			if (CachedItems.Contains(Key))
			{
				FAccelByteHttpCacheItem* CachedItem = CachedItems.Find(Key);
				if (CheckCachedItemFreshness(Key) == EHttpCacheFreshness::FRESH)
				{
					Out = CachedItem->Request;
					OutCachedResponse = CachedItem->Request->GetResponse();

					bRetrieved = true;
					UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Valid cached response found, will return that instead of sending request"));
				}
				else if (CheckCachedItemFreshness(Key) == EHttpCacheFreshness::WAITING_REFRESH)
				{
					if (CachedItem == nullptr) return bRetrieved;
					if (CachedItem->Request.IsValid() == false) return bRetrieved;
					if (CachedItem->Request->GetResponse().IsValid() == false) return bRetrieved;

					auto ETagValue = CachedItem->Request->GetResponse()->GetHeader(HTTPHeader::Cache::ETag);
					if (ETagValue.IsEmpty()) return bRetrieved;

					Out->AppendToHeader(HTTPHeader::Cache::IfNoneMatch, ETagValue);
				}
	
			}

			return bRetrieved;
		}

		bool FAccelByteHttpCache::TryStoring(const FHttpRequestPtr& Request)
		{
			const FHttpResponsePtr Response = Request.Get()->GetResponse();
			if (Response != nullptr && IsResponseCacheable(Request))
			{
				const FName Key = ConstructKey(Request);

				int TimeInProxyCache = 0;
				const FString CacheAge = Response->GetHeader(HTTPHeader::Cache::Age);
				if (!CacheAge.IsEmpty())
				{
					TimeInProxyCache = FCString::Atoi(*CacheAge);
				}

				double ResponseAgeThreshold = 0;
				FString CacheControlHeader = Response->GetHeader(HTTPHeader::Cache::Control);
				if (CacheControlHeader.IsEmpty() == false)
				{
					FString MaxAge = ExtractControlDirective(CacheControlHeader, HTTPHeader::Cache::ControlDirective::MaxAge);
					if (MaxAge.IsEmpty() == false)
					{
						int ResponseAgeThresholdInt = FCString::Atoi(*MaxAge);
						ResponseAgeThreshold = (double)ResponseAgeThresholdInt;
					}
				}

				FAccelByteHttpCacheItem NewCacheItem;
				const double TimeNow = FPlatformTime::Seconds();
				NewCacheItem.ExpireTime = TimeNow + ResponseAgeThreshold - TimeInProxyCache;

				// IF the response from the online storage service return 304
				// THEN we can reuse the old cache and extend the usage because the response should be same
				if (Response->GetResponseCode() == EHttpResponseCodes::NotModified &&
					CachedItems.Contains(Key))
				{
					auto CurrentCachedItem = CachedItems.Find(Key);
					
					TArray<FString> TransferredHeaders = {
						HTTPHeader::Cache::Control,
						HTTPHeader::Cache::ContentLocation,
						HTTPHeader::Cache::Date,
						HTTPHeader::Cache::ETag,
						HTTPHeader::Cache::Expires
					};

					if (CacheControlHeader.IsEmpty() == false)
					{
						for (auto Header : TransferredHeaders)
						{
							FString Value = ExtractControlDirective(CacheControlHeader, Header);
							if (Value.IsEmpty() == false)
							{
								CurrentCachedItem->Request->AppendToHeader(Header, Value);
							}
						}
					}
					
					CurrentCachedItem->ExpireTime = NewCacheItem.ExpireTime;
					UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response for request [%s] is now extended using the same cached response"), *Response->GetURL());
				}
				else
				{
					NewCacheItem.Request = Request;
					CachedItems.Emplace(Key, NewCacheItem);
					UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response for request [%s] is now cached"),*Response->GetURL());
				}

			}

			return false;
		}

		void FAccelByteHttpCache::ClearCache()
		{
			CachedItems.Empty();
		}

		FAccelByteHttpCache::EHttpCacheFreshness FAccelByteHttpCache::CheckCachedItemFreshness(const FName& Key)
		{
			check(CachedItems.Contains(Key));
			const FAccelByteHttpCacheItem CachedItem = CachedItems[Key];
			const double TimeNow = FPlatformTime::Seconds();
			bool bIsStaleResponse = false;
			EHttpCacheFreshness Freshness = EHttpCacheFreshness::FRESH;

			// check RESPONSE cache-control directive
			const FHttpResponsePtr CachedResponse = CachedItem.Request->GetResponse();
			const FString ResponseCacheControlHeader = CachedResponse->GetHeader(HTTPHeader::Cache::Control);

			if (ResponseCacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::Immutable))
			{
				bIsStaleResponse = false;
			} 
			else if (CachedItem.ExpireTime < TimeNow) //Expired
			{
				bIsStaleResponse = true;
			}
			else // Fresh
			{
				if (!ResponseCacheControlHeader.IsEmpty())
				{
					//@TODO this needs to check for validation, for now we are dropping this as stale
					if (ResponseCacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::NoCache))
					{
						bIsStaleResponse = true;
						// no-cache 
						//A cache will send the request to the origin server for validation before releasing a cached copy.

					}
					else if (ResponseCacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::MustRevalidate))
					{
						// must-revalidate
						//When using the "must-revalidate" directive, the cache must verify the status of stale resources before using them

						//TODO
						//Revalidate(Key);
						bIsStaleResponse = true;
					}

					//The "public" directive indicates that the response may be cached by any cache. 
					// This can be useful if pages with HTTP authentication or response status codes that aren't normally cacheable should now be cached.
					//On the other hand, "private" indicates that the response is intended for a single user only
					//and must not be stored by a shared cache.A private browser cache may store the response in this case.
				}
			}

			if (bIsStaleResponse)
			{
				// Don't remove the cached item for this KEY yet, it has an ETag an might be refreshed by the following request
				if (CachedItem.Request->GetResponse().IsValid() &&
					CachedItem.Request->GetResponse()->GetHeader(HTTPHeader::Cache::ETag).IsEmpty() == false)
				{
					Freshness = EHttpCacheFreshness::WAITING_REFRESH;
					UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Cached item [%s] not removed yet, waiting for refresh"), *Key.ToString());
				}
				else
				{
					Freshness = EHttpCacheFreshness::STALE;
					CachedItems.Remove(Key);
					UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Removed stale cached item [%s]"), *Key.ToString());
				}
			}

			return Freshness;
		};

		FName FAccelByteHttpCache::ConstructKey(const FHttpRequestPtr& Request)
		{
			const FString KeyString = FString::Printf(TEXT("%s-%s")
				, *Request->GetVerb()
				, *Request->GetURL()
			);

			const FName Key = FName(*KeyString);

			return Key;
		}

		FString FAccelByteHttpCache::ExtractControlDirective(const FString& CacheControlHeader, const FString& ControlDirective)
		{
			FString Result = "";
			TArray<FString> Array;
			CacheControlHeader.ParseIntoArray(Array, TEXT(","));
			for (const FString& Directive : Array)
			{
				FString Key, Value;
				UGameplayStatics::GetKeyValue(CacheControlHeader, Key, Value);

				if (Key == ControlDirective)
				{
					Result = Value;
					break;
				}

			}

			return Result;
		}
	} // namespace Core

} // namespace AccelByte

