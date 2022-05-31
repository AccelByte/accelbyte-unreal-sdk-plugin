// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HttpManager.h"
#include "Core/AccelByteLRUCache.h"

namespace AccelByte
{

	namespace Core
	{
		struct FAccelByteHttpCacheItem
		{
			// Platform time until cached response is stale (in seconds)
			double ExpireTime{0};

			// Completed request with valid response
			FHttpRequestPtr Request;
		};

		class ACCELBYTEUE4SDK_API FAccelByteHttpCache
		{
		public:

			virtual ~FAccelByteHttpCache();

			bool TryRetrieving(FHttpRequestPtr& Out, FHttpResponsePtr& OutCachedResponse);
			bool TryStoring(const FHttpRequestPtr& Request);
			void ClearCache();

		protected:

			static int MaxAgeCacheThreshold;

			FAccelByteLRUCache<FAccelByteHttpCacheItem> CachedItems;

			static FName ConstructKey(const FHttpRequestPtr& Request);

			enum class EHttpCacheFreshness : uint8
			{
				STALE = 0,
				FRESH,
				WAITING_REFRESH //if the cached item has an ETag value and it still has a chance to get 304 response from the cloud storage provider
			};

		public:

			/**
			 * @brief Check whether a response fullfil the caching prerequisites from it's cache-control directives in header
			 *
			 * @param CompletedRequest - Request with completed response
			 * @return true if cacheable (cache-directive, response code and request verb OK), false otherwise
			*/
			static bool IsResponseCacheable(const FHttpRequestPtr& CompletedRequest);

		private:

			/**
			 * @brief Check whether the cached response is not stale nor invalid
			 *
			 * @param Key - Key for a HttpCache content
			 * @return EHttpCacheFreshness 
			*/
			FAccelByteHttpCache::EHttpCacheFreshness CheckCachedItemFreshness(const FName& Key);

			/**
			 * @brief Extract specific control directive value from a Cache-Control header
			 * 
			 * @param CacheControlHeader
			 * @param ControlDirective
			 * @return FString - Value of the control directive. Empty if not found
			*/
			static FString ExtractControlDirective(const FString& CacheControlHeader, const FString& ControlDirective);
		};

	} // namespace Core

} // namespace AccelByte


