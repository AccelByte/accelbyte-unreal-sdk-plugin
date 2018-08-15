// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Models/TelemetryEvent.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "RetryTaskManagerJustice.h"
#include "JusticeSDK.h"

class JUSTICESDK_API JusticeTelemetry 
{
public:
	static void SendTelemetryEvent(FTelemetryEvent TelemetryEvent, FDefaultCompleteDelegate OnComplete = nullptr);
	
private:
	static void OnSendTelemetryEventResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
};