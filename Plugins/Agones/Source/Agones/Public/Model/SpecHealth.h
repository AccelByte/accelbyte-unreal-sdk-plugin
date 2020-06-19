// Copyright 2020 Google LLC All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Modifications copyright (C) 2020 < AccelByte Inc. / jonathan@accelbyte.net >
//  * JsonObject->TryGetNumberField has different function signature on UE4.18
//  * Runtime/Launch/Resources/Version.h added to get the UE4 minor version

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Runtime/Launch/Resources/Version.h"
#include "SpecHealth.generated.h"

USTRUCT()
struct AGONES_API FHealth
{
	GENERATED_BODY()

	UPROPERTY()
	bool bDisabled = false;

	UPROPERTY()
	int32 PeriodSeconds = 0;

	UPROPERTY()
	int32 FailureThreshold = 0;

	UPROPERTY()
	int32 InitialDelaySeconds = 0;

	/** Default constructor, no initialization */
	FHealth()
	{}

	FHealth(TSharedPtr<FJsonObject> JsonObject)
	{
		JsonObject->TryGetBoolField(TEXT("disabled"), bDisabled);
#if ENGINE_MINOR_VERSION > 20
		JsonObject->TryGetNumberField(TEXT("period_seconds"), PeriodSeconds);
		JsonObject->TryGetNumberField(TEXT("failure_threshold"), FailureThreshold);
		JsonObject->TryGetNumberField(TEXT("initial_delay_seconds"), InitialDelaySeconds);

#else
		double outPeriodSeconds, outFailureThreshold, outInitialDelaySeconds = 0.f;
		JsonObject->TryGetNumberField(TEXT("period_seconds"), outPeriodSeconds);
		PeriodSeconds = outPeriodSeconds;
		JsonObject->TryGetNumberField(TEXT("failure_threshold"), outFailureThreshold);
		FailureThreshold = outFailureThreshold;
		JsonObject->TryGetNumberField(TEXT("initial_delay_seconds"), outInitialDelaySeconds);
		InitialDelaySeconds = outInitialDelaySeconds;
#endif
	}
};
