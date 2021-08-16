// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HttpClientTest.generated.h"

USTRUCT()
struct ACCELBYTEUE4SDK_API FHttpClientTestAnythingRequest
{
	GENERATED_BODY()
	UPROPERTY()
		FString Name;
	UPROPERTY()
		int32 Age;
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FHttpClientTestAnythingResponse
{
	GENERATED_BODY()
	UPROPERTY()
		FString Method;
	UPROPERTY()
		FString Url;
	UPROPERTY()
		TMap<FString, FString> Args;
	UPROPERTY()
		TMap<FString, FString> Headers;
	UPROPERTY()
		TMap<FString, FString> Form;
	UPROPERTY()
		FHttpClientTestAnythingRequest Json;
};
