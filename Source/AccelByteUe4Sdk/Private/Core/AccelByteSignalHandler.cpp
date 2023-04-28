// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSignalHandler.h"

#if PLATFORM_LINUX
extern volatile sig_atomic_t GEnteredSignalHandler;
#endif

FOnSigtermReceived FAccelByteSignalHandler::OnSigtermReceivedDelegate;

void FAccelByteSignalHandler::Initialize()
{
#if PLATFORM_LINUX
	struct sigaction Action;
	FMemory::Memzero(&Action, sizeof(struct sigaction));
	Action.sa_sigaction = FAccelByteSignalHandler::OnSignalReceived;
	sigfillset(&Action.sa_mask);
	Action.sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK;
	sigaction(SIGTERM, &Action, nullptr);
}

void FAccelByteSignalHandler::OnSignalReceived(int32 Signal, siginfo_t* Info, void* Context)
{
	GEnteredSignalHandler = 1;
	// do not flush logs at this point; this can result in a deadlock if the signal was received while we were holding lock in the malloc (flushing allocates memory)
	if (!IsEngineExitRequested())
	{
		OnSigtermReceivedDelegate.ExecuteIfBound();
		FPlatformMisc::RequestExitWithStatus(false, 128 + Signal);	// Keeping the established shell practice of returning 128 + signal for terminations by signal. Allows to distinguish SIGINT/SIGTERM/SIGHUP.
	}
	else
	{
		FPlatformMisc::RequestExit(true);
	}

	GEnteredSignalHandler = 0;
#endif
}

void FAccelByteSignalHandler::SetOnSigtermReceived(const FOnSigtermReceived& OnSigtermReceived)
{
	OnSigtermReceivedDelegate = OnSigtermReceived;
}