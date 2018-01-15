// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "OnlineAsyncTaskManagerJustice.h"
#include "OnlineIdentityJustice.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemJustice.h"
#include "OnlineSubsystemJusticeTypes.h"
#include "OnlineSubsystemUtils.h"

void FOnlineAsyncTaskManagerJustice::OnlineTick()
{
	check(JusticeSubsystem);
	check(FPlatformTLS::GetCurrentThreadId() == OnlineThreadId || !FPlatformProcess::SupportsMultithreading());
	
	for (auto& CheckRegisteredDelegate : RegisteredUpdatedDelegate)
	{
		UE_LOG_ONLINE(Log, TEXT("NextTick Schedule= %s, UtcNow= %s"), *CheckRegisteredDelegate.Value.NextUpdate.ToIso8601(), *FDateTime::UtcNow().ToIso8601());
		if (FDateTime::UtcNow() >= CheckRegisteredDelegate.Value.NextUpdate) {
			CheckRegisteredDelegate.Value.Delegate.Execute(LastTokenRefreshCheckUtc, RefreshTokenCheckInterval, JusticeSubsystem);
			FDateTime NextTick_Update = FDateTime::UtcNow() + FTimespan::FromSeconds(CheckRegisteredDelegate.Value.Schedule.GetSeconds());

			UpdateDelegateSchedule(CheckRegisteredDelegate.Key, CheckRegisteredDelegate.Value.Schedule, NextTick_Update, CheckRegisteredDelegate.Value.Delegate);
			UE_LOG_ONLINE(Warning, TEXT("Rescheduling RegisteredUpdateDelegate"));
		}	
	}
}
void FOnlineAsyncTaskManagerJustice::UpdateDelegateSchedule(FString name, FTimespan schedule, FDateTime NextTick, FOnScheduleTickDelegate delegate)
 {
	FSchedule HashValue;
	HashValue.Schedule = schedule;
	HashValue.NextUpdate = NextTick;
	HashValue.Delegate = delegate;
	UE_LOG(LogOnline, Log, TEXT("Hashing parameter into a table with updating delegate concepts"));
	RegisteredUpdatedDelegate.Add(name, HashValue);
}
void FOnlineAsyncTaskManagerJustice::UnregisterDelegate(FString name) 
{
	RegisteredUpdatedDelegate.Remove(name);	
}