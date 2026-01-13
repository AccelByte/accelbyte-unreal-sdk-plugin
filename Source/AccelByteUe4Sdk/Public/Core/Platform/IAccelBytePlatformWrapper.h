// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelBytePlatformHandleModels.h"

namespace AccelByte
{
	/**
	 * @brief Platform Wrapper Interface to implement specific functionalities of given Native Platform.
	 */
	class ACCELBYTEUE4SDK_API IAccelBytePlatformWrapper
	{
	public:
		virtual ~IAccelBytePlatformWrapper() = default;

		/**
		 * @brief Retrieve App Id of the game on specific Native Platform.
		 * 
		 * @return AppId of the game.
		 */
		virtual FString GetAppId() = 0;

		/**
		 * @brief Retrieve User Id that was playing the game on the Native Platform.
		 * 
		 * @param ExtraAttributes Key-Value pairs to determine which player info that were trying to retrieve e.g. LocalUserIndex. (optional)
		 * @return Specific Platform User Id.
		 */
		virtual FString GetPlatformUserId(TMap<FString, FString> const& ExtraAttributes = {}) = 0;

		/**
		 * @brief Fetch Platform Token or Auth Code on specific Native Platform.
		 * 
		 * @param Delegate A callback delegate that will be triggered to return the Platform Token value whether success or not.
		 * @param ExtraAttributes Key-Value pairs for additional information needed to get the Platform Token e.g. service identity in Steam. (optional)
		 */
		virtual void FetchPlatformToken(TDelegate<void(FString const&)> Delegate, TMap<FString, FString> const& ExtraAttributes = {}) = 0;

		/**
		 * @brief Retrieve native platform friends.
		 * 
		 * @param OnSuccess A callback delegate that will be triggered to return native platform friends.
		 * @param OnError A callback delegate that will be triggered if the request failed.
		 */
		virtual void FetchPlatformFriends(const TDelegate<void(const TArray<FPlatformUser>&)> & OnSuccess
			, const TDelegate<void(const FPlatformHandlerError&)>& OnError) = 0;
		
		/**
		 * @brief Get products from native platform store by SKUs.
		 *
		 * @param Skus Array of product SKU.
		 * @param OnSuccess A callback delegate that will be triggered to return array of products.
		 * @param OnError A callback delegate that will be triggered if the request failed.
		 */
		virtual void GetProductsBySku(const TArray<FString>& Skus
			, const TDelegate<void(const TArray<FPlatformProductPtr>&)>& OnSuccess
			, const TDelegate<void(const FPlatformHandlerError&)>& OnError) = 0;

		/**
		 * @brief Get all purchased products from native platform store for current user.
		 *
		 * @param OnSuccess A callback delegate that will be triggered to return array of purchases.
		 * @param OnError A callback delegate that will be triggered if the request failed.
		 */
		virtual void GetPurchasedProducts(const TDelegate<void(const TArray<FPlatformPurchasePtr>&)>& OnSuccess
			, const TDelegate<void(const FPlatformHandlerError&)>& OnError) = 0;

		/**
		 * @brief Launch native platform checkout flow.
		 *
		 * @param Sku The Sku of product.
		 * @param OnSuccess A callback delegate that will be triggered to return purchase details.
		 * @param OnError A callback delegate that will be triggered if the request failed.
		 */
		virtual void CheckoutProduct(const FString& Sku, const TDelegate<void(const FPlatformPurchasePtr&)>& OnSuccess
			, const TDelegate<void(const FPlatformHandlerError&)>& OnError) = 0;

		/**
		 * @brief Consume purchased product.
		 *
		 * @param Sku The Sku of product.
		 * @param OnSuccess A callback delegate that will be triggered to return true if product consumed successfully.
		 * @param OnError A callback delegate that will be triggered if the request failed.
		 */
		virtual void ConsumeProduct(const FString& Sku, const TDelegate<void(const FPlatformConsumeItemResponsePtr&)>& OnSuccess
			, const TDelegate<void(const FPlatformHandlerError&)>& OnError) = 0;
	};

	typedef TSharedPtr<IAccelBytePlatformWrapper, ESPMode::ThreadSafe> AccelBytePlatformWrapperPtr;
	typedef TWeakPtr<IAccelBytePlatformWrapper, ESPMode::ThreadSafe> AccelBytePlatformWrapperWPtr;
}
