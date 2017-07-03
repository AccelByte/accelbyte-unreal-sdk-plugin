// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"

/**
 *	Justice version of the async task manager to register the various Justice callbacks with the engine
 */
class FOnlineAsyncTaskManagerJustice : public FOnlineAsyncTaskManager
{
protected:

	/** Cached reference to the main online subsystem */
	class FOnlineSubsystemJustice* JusticeSubsystem;

public:

	FOnlineAsyncTaskManagerJustice(class FOnlineSubsystemJustice* InOnlineSubsystem)
		: JusticeSubsystem(InOnlineSubsystem)
	{
	}

	~FOnlineAsyncTaskManagerJustice() 
	{
	}

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override;
};
