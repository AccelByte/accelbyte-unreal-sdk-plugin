// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteEnvironment.h"

namespace AccelByte 
{
	FString Environment::GetEnvironmentVariable(const FString EnvironmentVariableName, const int32 EnvironmentLenght) 
	{
		FString TempEnvironment;
		TempEnvironment = FPlatformMisc::GetEnvironmentVariable(*EnvironmentVariableName);
		return TempEnvironment;
	}
}