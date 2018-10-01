// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsTelemetry.h"
#include "Http.h"

namespace AccelByte
{
namespace Services
{

class ACCELBYTEUE4SDK_API Telemetry 
{
	DECLARE_DELEGATE(FDefaultCompleteDelegate);
public:
	static void SendTelemetryEvent(FAccelByteModelsTelemetryEvent TelemetryEvent, FDefaultCompleteDelegate OnComplete);
	
private:
	static void SendTelemetryEventResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
};

} // Namespace Services
} // Namespace AccelByte
