// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "OnlineAsyncTaskManager.h"

#define DEFAULT_REFRESH_TOKEN_CHECK_INTERVAL_SECONDS 60
DECLARE_DELEGATE_ThreeParams(FOnScheduleTickDelegate, struct FDateTime, int32, class FOnlineSubsystemJustice*);

/**
 *	Justice version of the async task manager to register the various Justice callbacks with the engine
 */
class FOnlineAsyncTaskManagerJustice : public FOnlineAsyncTaskManager
{
protected:

	/** Cached reference to the main online subsystem */
	class FOnlineSubsystemJustice* JusticeSubsystem;

	// Last token refresh run
	FDateTime LastTokenRefreshCheckUtc;
	
	// How often to check if tokens need to be refreshed
	int32 RefreshTokenCheckInterval;

public:

	FOnlineAsyncTaskManagerJustice(class FOnlineSubsystemJustice* InOnlineSubsystem)
		:   JusticeSubsystem(InOnlineSubsystem),
			LastTokenRefreshCheckUtc(0),
			RefreshTokenCheckInterval(DEFAULT_REFRESH_TOKEN_CHECK_INTERVAL_SECONDS)
	{
	}

	~FOnlineAsyncTaskManagerJustice() 
	{
	}

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;
	void UpdateDelegateSchedule(int name, FTimespan schedule, FDateTime NextTick, FOnScheduleTickDelegate delegate);
	void UnregisterDelegate(int name);
	struct FSchedule 
	{		
		FTimespan Schedule;
		FDateTime NextUpdate;
		FOnScheduleTickDelegate Delegate;
	};
	TMap<int, FSchedule> RegisteredUpdatedDelegate;
};
