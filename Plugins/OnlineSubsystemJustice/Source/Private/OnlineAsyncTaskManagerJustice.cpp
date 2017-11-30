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
		}
	}
}
