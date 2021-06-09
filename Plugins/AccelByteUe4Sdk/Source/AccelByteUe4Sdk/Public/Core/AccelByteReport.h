// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "HttpManager.h"
#include "CoreMinimal.h"

#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "Http.h"

DECLARE_LOG_CATEGORY_EXTERN(AccelByteReportLog, Log, All);

using namespace AccelByte;

namespace AccelByte 
{

class ACCELBYTEUE4SDK_API Report
{
public:
	void GetHttpRequest(const FHttpRequestPtr& Request);
	void GetHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response);
	void GetFunctionLog(FString FunctionMacroName);
private:
	FString GetStandardTime();
};
}