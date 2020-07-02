// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteEnvironment.h"

namespace AccelByte 
{
	FString Environment::GetEnvironmentVariable(const FString EnvironmentVariableName, const int32 EnvironmentLenght) 
	{
		FString TempEnvironment;
#if ENGINE_MINOR_VERSION < 21
		TCHAR* TempTCharEnvironment = new TCHAR[EnvironmentLenght];
		#if PLATFORM_WINDOWS
			FWindowsPlatformMisc::GetEnvironmentVariable((TEXT("%s"), *EnvironmentVariableName), TempTCharEnvironment, EnvironmentLenght);
		#elif PLATFORM_LINUX
			FLinuxPlatformMisc::GetEnvironmentVariable((TEXT("%s"), *EnvironmentVariableName), TempTCharEnvironment, EnvironmentLenght);
		#elif PLATFORM_MAC
			FApplePlatformMisc::GetEnvironmentVariable((TEXT("%s"), *EnvironmentVariableName), TempTCharEnvironment, EnvironmentLenght);
		#endif
		TempEnvironment = TempTCharEnvironment;
#else
		#if PLATFORM_WINDOWS
			TempEnvironment = FWindowsPlatformMisc::GetEnvironmentVariable(*EnvironmentVariableName);
		#elif PLATFORM_LINUX
			TempEnvironment = FLinuxPlatformMisc::GetEnvironmentVariable(*EnvironmentVariableName);
		#elif PLATFORM_MAC
			TempEnvironment = FApplePlatformMisc::GetEnvironmentVariable(*EnvironmentVariableName);
		#endif
#endif
		return TempEnvironment;
	}
}