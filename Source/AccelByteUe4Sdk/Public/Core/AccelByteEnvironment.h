// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteEnvironment.generated.h"

UENUM(BlueprintType)
enum class ESettingsEnvironment : uint8
{
	/** Dev environment settings */
	Development		UMETA(DisplayName = "Development"),
	/** Cert environment settings */
	Certification	UMETA(DisplayName = "Certification"),
	/** Prod environment settings */
	Production		UMETA(DisplayName = "Production"),
	/** Default environment settings */
	Default			UMETA(DisplayName = "Default")
};

namespace AccelByte 
{	
class ACCELBYTEUE4SDK_API Environment
{
public:
	/**
	 * @brief Get environment variable from the platform.
	 * 
	 * @param EnvironmentVariableName The name of an environment variable from the platform.
	 * @param EnvironmentLenght The length of an environment variable. It needed to handle UE4 version below 4.21.
	 */
	static FString GetEnvironmentVariable(const FString EnvironmentVariableName, const int32 EnvironmentLenght);

};
}