// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h" 

struct ACCELBYTEUE4SDK_API FAccelByteModelsTelemetryBody
{
	FString EventNamespace;
	FString EventName;
	TSharedPtr<FJsonObject> Payload;
};