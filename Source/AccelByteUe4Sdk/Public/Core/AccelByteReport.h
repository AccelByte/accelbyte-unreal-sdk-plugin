// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByte, Log, All);

namespace AccelByte 
{

class ACCELBYTEUE4SDK_API FReport
{
public:
	static void LogHttpRequest(const FHttpRequestPtr& Request);
	static void LogHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
	static void Log(FString Message);
	static void LogDeprecated(FString FunctionName, FString Message = TEXT(""));
};
}