// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpCache.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Core/AccelByteLRUCacheFile.h"
#include "Core/AccelByteLRUCacheMemory.h"

#include "Core/AccelByteUtilities.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpCache, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteHttpCache);

namespace AccelByte
{
namespace Core
{
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
} // namespace ControlDirective
} // namespace Cache

namespace Verb
{
const FString Delete = TEXT("DELETE");
} // namespace Verb
} // namespace HTTPHeader

int FAccelByteHttpCache::MaxAgeCacheLowerThreshold = 100;
FAccelByteHttpCache::FAccelByteHttpCache()
	: CachedItemsInternal{nullptr}
{
}

FAccelByteHttpCache::~FAccelByteHttpCache()
{
	// empty
};

void FAccelByteHttpCache::Initialize()
{
	FWriteScopeLock WriteLock(CachedItemsInternalMtx);
	if (CachedItemsInternal.IsValid())
	{
		return;
	}
	switch (CacheType)
	{
	case EHttpCacheType::MEMORY:
		{
			CachedItemsInternal = MakeShareable<FAccelByteLRUCacheMemory<FAccelByteHttpCacheItem>>(new FAccelByteLRUCacheMemory<FAccelByteHttpCacheItem>());
			return;
		}
	case EHttpCacheType::STORAGE:
	default:
		{
			CachedItemsInternal = MakeShareable<FAccelByteLRUCacheFile<FAccelByteHttpCacheItem>>(new FAccelByteLRUCacheFile<FAccelByteHttpCacheItem>());
			return;
		}
	}
}

TSharedPtr<FAccelByteHttpCacheItem> FAccelByteHttpCache::GetCachedItems(const FName& Key)
{
	Initialize();

	FWriteScopeLock WriteLock(CachedItemsInternalMtx);
	return CachedItemsInternal->Find(Key);
}

TSharedPtr<FAccelByteHttpCacheItem> FAccelByteHttpCache::GetSerializedHttpCache(const FHttpRequestPtr& Request)
{
	Initialize();

	FWriteScopeLock WriteLock(CachedItemsInternalMtx);
	auto CachedItem = CachedItemsInternal->Find(ConstructKey(Request));
	if (CachedItem.IsValid())
	{
		return CachedItem;
	}
	else
	{
		return nullptr;
	}
}

bool FAccelByteHttpCache::IsResponseCacheable(const FHttpRequestPtr& CompletedRequest)
{
	if (CompletedRequest->GetVerb() == HTTPHeader::Verb::Delete)
	{
		return false;
	}

	const FHttpResponsePtr ResponsePtr = CompletedRequest->GetResponse();
	if (!ResponsePtr.IsValid())
	{
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Request has not receive response yet"));
		return false;
	}

	//If http 304 or between http 200 - 204
	auto ResponseCode = ResponsePtr->GetResponseCode();
	if(ResponseCode != EHttpResponseCodes::NotModified && (ResponseCode < EHttpResponseCodes::Ok || ResponseCode > EHttpResponseCodes::NoContent))
	{
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Request has an invalid response code"));
		return false;
	}

	const FString CacheControlHeader = ResponsePtr->GetHeader(HTTPHeader::Cache::Control);
	if(CacheControlHeader.IsEmpty())
	{
		//@TODO atm default behavior is to cache, even without CacheControl directives, might be better to
		// follow the convention only caching with valid cache control directive. so we dont need to handle valid verb manually
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response has empty CacheControlHeader"));
		return false; 
	}
	UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response has valid CacheControlHeader: %s"),*CacheControlHeader);
	FString MaxAge = ExtractControlDirective(CacheControlHeader, HTTPHeader::Cache::ControlDirective::MaxAge);
	if(CacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::NoStore))
	{
		return false;
	}
	else if(!MaxAge.IsEmpty())
	{
		const int MaxAgeValue = FCString::Atoi(*MaxAge);
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Max Age %d; Lower Threshold %d; Should cache? %d"), MaxAgeValue, MaxAgeCacheLowerThreshold, MaxAgeValue > MaxAgeCacheLowerThreshold);
		return MaxAgeValue > MaxAgeCacheLowerThreshold;
	}
	else // HTTPHeader::Cache::ControlDirective::Immutable and others
	{
		return true;
	}
}

void FAccelByteHttpCache::SetCacheType(const EHttpCacheType InCacheType)
{
	CacheType = InCacheType;
}

bool FAccelByteHttpCache::TryRetrieving(const FHttpRequestPtr& Request, FHttpResponsePtr& OutCachedResponse)
{
	Initialize();

	if(!Request.IsValid())
	{
		return false;
	}
	const FName Key = ConstructKey(Request);
	const auto CachedItem = GetCachedItems(Key);
	if(!CachedItem.IsValid())
	{
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("No Valid cached response found for request to: %s; Will continue sending the request."), *Request->GetURL());
		return false;
	}
	
	FAccelByteHttpResponseConstructable Response;
	Response.SetPayload(CachedItem->SerializableRequestAndResponse.ResponsePayload);
	Response.SetResponseCode(CachedItem->SerializableRequestAndResponse.ResponseCode);
	Response.SetHeaders(CachedItem->SerializableRequestAndResponse.ResponseHeaders);
	Response.SetURL(CachedItem->SerializableRequestAndResponse.RequestURL);

	EHttpCacheFreshness FreshnessState = CheckCachedItemFreshness(CachedItem);
	if (FreshnessState == EHttpCacheFreshness::FRESH)
	{
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Valid cached response found, will return that instead of sending request"));
		OutCachedResponse = MakeShared<FAccelByteHttpResponseConstructable, ESPMode::ThreadSafe>(Response);
		return true;
	}
	else if (FreshnessState == EHttpCacheFreshness::WAITING_REFRESH)
	{
		FString ETagValue = Response.GetHeader(HTTPHeader::Cache::ETag);
		if (ETagValue.IsEmpty())
		{
			UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Cached response to: %s doesn't have ETag; Will continue sending the request."), *Request->GetURL());
			return false;
		}
		else
		{
			OutCachedResponse = MakeShared<FAccelByteHttpResponseConstructable, ESPMode::ThreadSafe>(Response);
			return true;
		}
	}
	else // EHttpCacheFreshness::STALE
	{
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Stale cached response found for request to: %s; Will delete the cached entry and continue sending the request."), *Request->GetURL());
		FWriteScopeLock WriteLock(CachedItemsInternalMtx);
		CachedItemsInternal->Remove(Key);
		return false;
	}
}

bool FAccelByteHttpCache::TryStoring(const FHttpRequestPtr& Request)
{
	Initialize();

	if(!Request.IsValid())
	{
		return false;
	}

	const FHttpResponsePtr Response = Request->GetResponse();
	if (!Response.IsValid() || !IsResponseCacheable(Request))
	{
		return false;
	}

	int TimeInProxyCache = 0;
	const FString CacheAge = Response->GetHeader(HTTPHeader::Cache::Age);
	if (!CacheAge.IsEmpty())
	{
		TimeInProxyCache = FCString::Atoi(*CacheAge);
	}

	double ResponseAgeThreshold = 0;
	FString CacheControlHeader = Response->GetHeader(HTTPHeader::Cache::Control);
	if (!CacheControlHeader.IsEmpty())
	{
		FString MaxAge = ExtractControlDirective(CacheControlHeader, HTTPHeader::Cache::ControlDirective::MaxAge);
		if (!MaxAge.IsEmpty())
		{
			int ResponseAgeThresholdInt = FCString::Atoi(*MaxAge);
			ResponseAgeThreshold = static_cast<double>(ResponseAgeThresholdInt);
		}
	}

	FAccelByteHttpCacheItem NewCacheItem;
	const double TimeNow = FPlatformTime::Seconds();
	NewCacheItem.ExpireTime = TimeNow + ResponseAgeThreshold - TimeInProxyCache;
	NewCacheItem.SerializableRequestAndResponse.Serialize(Request, NewCacheItem.ExpireTime);

	const FName Key = ConstructKey(Request);
	auto const CachedItem = GetCachedItems(Key);

	// IF the response from the online storage service return 304
	// THEN we can reuse the old cache and extend the usage because the response should be same
	if (Response->GetResponseCode() == EHttpResponseCodes::NotModified && CachedItem.IsValid())
	{
		TArray<FString> TransferredHeaders = {
			HTTPHeader::Cache::Control,
			HTTPHeader::Cache::ContentLocation,
			HTTPHeader::Cache::Date,
			HTTPHeader::Cache::ETag,
			HTTPHeader::Cache::Expires
		};

		if (!CacheControlHeader.IsEmpty())
		{
			for (const auto& Header : TransferredHeaders)
			{
				FString Value = ExtractControlDirective(CacheControlHeader, Header);
				if (!Value.IsEmpty())
				{
					CachedItem->SerializableRequestAndResponse.RequestHeaders.AddUnique(FString::Printf(TEXT("%s: %s"), *Header, *Value));
				}
			}
		}
		CachedItem->ExpireTime = NewCacheItem.ExpireTime;
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response for request [%s] is now extended using the same cached response"), *Response->GetURL());
		return true;
	}
	else
	{
		FWriteScopeLock WriteLock(CachedItemsInternalMtx);
		CachedItemsInternal->Emplace(Key, NewCacheItem);
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Response for request [%s] is now cached"),*Response->GetURL());
		return true;
	}
}

void FAccelByteHttpCache::ClearCache()
{
	FWriteScopeLock WriteLock(CachedItemsInternalMtx);
	if (CachedItemsInternal.IsValid())
	{
		CachedItemsInternal->Empty();
	}
}

bool FAccelByteHttpCache::RetrieveAndLoadCacheFileInfo()
{		
	FWriteScopeLock WriteLock(CachedItemsInternalMtx);
	if (!CachedItemsInternal.IsValid())
	{
		return false;
	}
	return CachedItemsInternal->RetrieveAndLoadCacheFileInfo();
}

FAccelByteHttpCache::EHttpCacheFreshness FAccelByteHttpCache::CheckCachedItemFreshness(const TSharedPtr<FAccelByteHttpCacheItem>& CachedItem)
{
	if(!CachedItem.IsValid())
	{
		return EHttpCacheFreshness::STALE;
	}

	// check cached response "cache-control" directive and "ETag"
	FString ResponseCacheControlHeader = TEXT("");
	FString ETag = TEXT("");
	for (int i = 0 ; i < CachedItem->SerializableRequestAndResponse.ResponseHeaders.Num() ; i ++)
	{
		const FString& Header = CachedItem->SerializableRequestAndResponse.ResponseHeaders[i];
		if (Header.Contains(HTTPHeader::Cache::Control))
		{
			FString HeaderKey, Value;
			Header.Split(":", &HeaderKey, &Value);
			Value = Value.TrimStartAndEnd();

			ResponseCacheControlHeader = Value;
		}
		else if(Header.Contains(HTTPHeader::Cache::ETag))
		{
			FString HeaderKey, Value;
			Header.Split(":", &HeaderKey, &Value);
			Value = Value.TrimStartAndEnd();

			ETag = Value;
		}
		// If both found, exit immediately
		if(!ResponseCacheControlHeader.IsEmpty() && !ETag.IsEmpty())
		{
			break;
		}
	}
	
	const double TimeNow = FPlatformTime::Seconds();
	if (ResponseCacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::Immutable))
	{
		return EHttpCacheFreshness::FRESH;
	}
	else if(ResponseCacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::NoCache) //@TODO this needs to check for validation, for now we are dropping this as stale
		|| ResponseCacheControlHeader.Contains(HTTPHeader::Cache::ControlDirective::MustRevalidate) //When using the "must-revalidate" directive, the cache must verify the status of stale resources before using them
		|| (CachedItem->ExpireTime < TimeNow)) // Expired
	{
		//The "public" directive indicates that the response may be cached by any cache. 
		// This can be useful if pages with HTTP authentication or response status codes that aren't normally cacheable should now be cached.
		//On the other hand, "private" indicates that the response is intended for a single user only
		//and must not be stored by a shared cache.A private browser cache may store the response in this case.
		return EHttpCacheFreshness::STALE;
	}
	else if (!ETag.IsEmpty())
	{
		UE_LOG(LogAccelByteHttpCache, VeryVerbose, TEXT("Cached item not removed yet, waiting for refresh"));
		return EHttpCacheFreshness::WAITING_REFRESH;
	}
	else
	{
		return EHttpCacheFreshness::FRESH;
	}
};

FName FAccelByteHttpCache::ConstructKey(const FHttpRequestPtr& Request)
{
	const FString KeyString = FAccelByteUtilities::GenerateHashString(FString::Printf(TEXT("%s-%s")
		, *Request->GetVerb()
		, *Request->GetURL()
	));
	return FName(*KeyString);
}

FString FAccelByteHttpCache::ExtractControlDirective(const FString& CacheControlHeader, const FString& ControlDirectiveToSearch)
{
	FString Result = TEXT("");
	TArray<FString> Directives;
	CacheControlHeader.ParseIntoArray(Directives, TEXT(","));

	for (const auto& Directive : Directives)
	{
		FString Key, Value;
		UGameplayStatics::GetKeyValue(Directive, Key, Value);

		if (Key == ControlDirectiveToSearch)
		{
			Result = Value;
			break;
		}
	}
	return Result;
}
} // namespace Core

} // namespace AccelByte

