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
	

	if ((FDateTime::UtcNow() - LastTokenRefreshCheckUtc) > FTimespan::FromSeconds(RefreshTokenCheckInterval))
	{
<<<<<<< Updated upstream
		LastTokenRefreshCheckUtc = FDateTime::UtcNow();

		IOnlineIdentityPtr IdentityInterface = JusticeSubsystem->GetIdentityInterface();
		for (TSharedPtr<FUserOnlineAccount> UserAccountPtr : IdentityInterface->GetAllUserAccounts())
		{
			FString LocalUserNum;
			UserAccountPtr->GetAuthAttribute(FString("LocalUserNum"), LocalUserNum);
				
			FOnlineAccountCredentials Credentials;
			Credentials.Id = UserAccountPtr->GetUserId()->ToString();

			if (!IdentityInterface->Login(FCString::Atoi(*LocalUserNum), Credentials))
			{
				UE_LOG_ONLINE(Warning, TEXT("Failed to refresh token. User=%s"), *UserAccountPtr->GetUserId()->ToString());
			}
=======
		UE_LOG(LogOnline, Log, TEXT("NextTick Schedule= %s, UtcNow= %s"), *CheckRegisteredDelegate.Value.NextTick.ToIso8601(), *FDateTime::UtcNow().ToIso8601());
		if (FDateTime::UtcNow() >= CheckRegisteredDelegate.Value.NextTick) {
			mutex.Lock();
			CheckRegisteredDelegate.Value.delegate.Execute(LastTokenRefreshCheckUtc, RefreshTokenCheckInterval, JusticeSubsystem);
			FDateTime NextTick_Update = FDateTime::UtcNow() + FTimespan::FromSeconds(CheckRegisteredDelegate.Value.schedule);
			UpdateDelegateSchedule(CheckRegisteredDelegate.Key, CheckRegisteredDelegate.Value.schedule, NextTick_Update, CheckRegisteredDelegate.Value.delegate);
			UE_LOG_ONLINE(Warning, TEXT("Rescheduling RegisteredUpdateDelegate"));
			mutex.Unlock();
>>>>>>> Stashed changes
		}
	}
}

<<<<<<< Updated upstream
=======


void FOnlineAsyncTaskManagerJustice::UpdateDelegateSchedule(FString name, float schedule, FDateTime NextTick, FOnScheduleTickDelegate delegate)
{
	FHashValue HashValue;
	HashValue.schedule = schedule;
	HashValue.NextTick = NextTick;
	HashValue.delegate = delegate;
	UE_LOG(LogOnline, Log, TEXT("Hashing parameter into a table with updating delegate concepts"));
	RegisteredUpdatedDelegate.Add(name, HashValue);
}

void FOnlineAsyncTaskManagerJustice::UnregisterDelegate(FString name) {
	RegisteredUpdatedDelegate.Remove(name);
}
>>>>>>> Stashed changes
