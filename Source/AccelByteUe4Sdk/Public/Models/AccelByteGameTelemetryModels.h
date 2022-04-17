// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

struct ACCELBYTEUE4SDK_API FAccelByteModelsTelemetryBody
{
	/** @brief Namespace of the event and it only accepts input with valid characters. Allowed characters: Aa-Zz0-9 */
	FString EventNamespace{};

	/** @brief Name of the event and it only accepts input with valid characters. Allowed characters: Aa-Zz0-9 */
	FString EventName{};

	/** @brief Event payload that will be sent as telemetry data */
	TSharedPtr<FJsonObject> Payload{};

	/** @brief Timestamp when the event is registered */
	FDateTime EventTimestamp {0};
};