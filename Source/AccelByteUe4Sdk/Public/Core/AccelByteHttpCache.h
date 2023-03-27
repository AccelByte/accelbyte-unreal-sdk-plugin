// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HttpManager.h"
#include "Misc/ScopeLock.h"
#include "Misc/ScopeTryLock.h"
#include "Core/AccelByteLRUCache.h"
#include "Models/AccelByteGeneralModels.h"

namespace AccelByte
{

	namespace Core
	{
		class ACCELBYTEUE4SDK_API FAccelByteHttpCache
		{
		public:

			FAccelByteHttpCache();
			virtual ~FAccelByteHttpCache();

			/// <summary>
			/// Obtain the value from configuration to determine which caching method is chosen.
			/// We need to avoid access configuration (AccelByte::FRegistry.Settings) from a class constructor
			/// Because FRegistry.Settings load the value when triggered by startup module phase
			/// </summary>
			void InitializeFromConfig();

			bool TryRetrieving(FHttpRequestPtr& Out, FHttpResponsePtr& OutCachedResponse);
			bool TryStoring(const FHttpRequestPtr& Request);

			/// <summary>
			/// Should not be called from destructor at all.
			/// Call this from module shutdown only.
			/// </summary>
			void ClearCache();

			/**
			 * @brief To be called by CreateHttpResultHandler to obtain the CacheItem
			 *
			 * @param The CompletedRequest
			 * @return Return pointer to the cache , else return nullpointer if invalid
			*/
			FAccelByteHttpCacheItem* GetSerializedHttpCache(const FHttpRequestPtr& Request);

		protected:

			static int MaxAgeCacheThreshold;

			TSharedPtr<FAccelByteLRUCache<FAccelByteHttpCacheItem>> CachedItems;

			static FName ConstructKey(const FHttpRequestPtr& Request);

			enum class EHttpCacheFreshness : uint8
			{
				STALE = 0,
				FRESH,
				WAITING_REFRESH //if the cached item has an ETag value and it still has a chance to get 304 response from the cloud storage provider
			};

			FCriticalSection CacheCritSection;

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


