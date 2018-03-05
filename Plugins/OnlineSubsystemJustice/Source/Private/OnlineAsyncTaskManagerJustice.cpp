// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

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
	
	for (TPair<int, FSchedule>& CheckRegisteredDelegate : RegisteredUpdatedDelegate)
	{		
		if (FDateTime::UtcNow() >= CheckRegisteredDelegate.Value.NextUpdate) {
			CheckRegisteredDelegate.Value.Delegate.Execute(LastTokenRefreshCheckUtc, RefreshTokenCheckInterval, JusticeSubsystem);
			FDateTime NextTick_Update = FDateTime::UtcNow() + FTimespan::FromSeconds(CheckRegisteredDelegate.Value.Schedule.GetSeconds());

			UpdateDelegateSchedule(CheckRegisteredDelegate.Key, CheckRegisteredDelegate.Value.Schedule, NextTick_Update, CheckRegisteredDelegate.Value.Delegate);
			UE_LOG_ONLINE(VeryVerbose, TEXT("Rescheduling RegisteredUpdateDelegate"));
		}	
	}
}
void FOnlineAsyncTaskManagerJustice::UpdateDelegateSchedule(int name, FTimespan schedule, FDateTime NextTick, FOnScheduleTickDelegate delegate)
 {
	FSchedule UpdateSchedule;
	UpdateSchedule.Schedule = schedule;
	UpdateSchedule.NextUpdate = NextTick;
	UpdateSchedule.Delegate = delegate;
	RegisteredUpdatedDelegate.Add(name, UpdateSchedule);
}
void FOnlineAsyncTaskManagerJustice::UnregisterDelegate(int name) 
{
	RegisteredUpdatedDelegate.Remove(name);	
}