// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "JsonUtilities.h"

namespace AccelByte
{
	struct ACCELBYTEUE4SDK_API FPlatformUser
	{
		FString ID{};
		FString Name{};
		FString Presence{};
	};

	struct ACCELBYTEUE4SDK_API FPlatformProduct
	{
		FString Sku{};
		FString Name{};
		FString Description{};
		FString Price{};
	};

	typedef TSharedPtr<FPlatformProduct, ESPMode::ThreadSafe> FPlatformProductPtr;

	struct ACCELBYTEUE4SDK_API FPlatformPurchase
	{
		FString Sku{};
		FString TransactionId{};
		FJsonObjectWrapper Metadata{};
	};

	typedef TSharedPtr<FPlatformPurchase, ESPMode::ThreadSafe> FPlatformPurchasePtr;

	struct ACCELBYTEUE4SDK_API FPlatformConsumeItemResponse
	{
		bool bSuccess{false};
	};

	typedef TSharedPtr<FPlatformConsumeItemResponse, ESPMode::ThreadSafe> FPlatformConsumeItemResponsePtr;

	struct ACCELBYTEUE4SDK_API FPlatformHandlerError
	{
		FString ErrorCode{};
		FString ErrorMessage{};
	};
}
