// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"
#if PLATFORM_LINUX
#include <signal.h>
#endif

DECLARE_DELEGATE(FOnSigtermReceived);

class ACCELBYTEUE4SDK_API FAccelByteSignalHandler
{
public:
	static void Initialize();
	static void SetOnSigtermReceived(const FOnSigtermReceived& OnSigtermReceived);
#if PLATFORM_LINUX
	static void OnSignalReceived(int32 Signal, siginfo_t* Info, void* Context);
#endif

private:
	static FOnSigtermReceived OnSigtermReceivedDelegate;
};