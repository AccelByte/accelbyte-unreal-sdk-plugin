// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HttpCacheTest.generated.h"

USTRUCT()
struct ACCELBYTEUE4SDK_API FHttpCacheTestRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString Dummy;

};

USTRUCT()
struct ACCELBYTEUE4SDK_API FHttpCacheTestResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString Url;
};
