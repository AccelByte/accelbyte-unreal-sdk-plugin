// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemJustice.h"
#include "HAL/RunnableThread.h"
#include "OnlineAsyncTaskManagerJustice.h"

#include "OnlineSessionInterfaceJustice.h"
#include "OnlineLeaderboardInterfaceJustice.h"
#include "OnlineIdentityJustice.h"
#include "VoiceInterfaceImpl.h"
#include "OnlineAchievementsInterfaceJustice.h"
#include "IamClient.h"

FThreadSafeCounter FOnlineSubsystemJustice::TaskCounter;

IOnlineSessionPtr FOnlineSubsystemJustice::GetSessionInterface() const
{
	return SessionInterface;
}

IOnlineFriendsPtr FOnlineSubsystemJustice::GetFriendsInterface() const
{
	return nullptr;
}

IOnlinePartyPtr FOnlineSubsystemJustice::GetPartyInterface() const
{
	return nullptr;
}

IOnlineGroupsPtr FOnlineSubsystemJustice::GetGroupsInterface() const
{
	return nullptr;
}

IOnlineSharedCloudPtr FOnlineSubsystemJustice::GetSharedCloudInterface() const
{
	return nullptr;
}

IOnlineUserCloudPtr FOnlineSubsystemJustice::GetUserCloudInterface() const
{
	return nullptr;
}

IOnlineEntitlementsPtr FOnlineSubsystemJustice::GetEntitlementsInterface() const
{
	return nullptr;
};

IOnlineLeaderboardsPtr FOnlineSubsystemJustice::GetLeaderboardsInterface() const
{
	return LeaderboardsInterface;
}

IOnlineVoicePtr FOnlineSubsystemJustice::GetVoiceInterface() const
{
	if (VoiceInterface.IsValid() && !bVoiceInterfaceInitialized)
	{	
		if (!VoiceInterface->Init())
		{
			VoiceInterface = nullptr;
		}

		bVoiceInterfaceInitialized = true;
	}

	return VoiceInterface;
}

IOnlineExternalUIPtr FOnlineSubsystemJustice::GetExternalUIInterface() const
{
	return nullptr;
}

IOnlineTimePtr FOnlineSubsystemJustice::GetTimeInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemJustice::GetIdentityInterface() const
{
	return IdentityInterface;
}

IOnlineTitleFilePtr FOnlineSubsystemJustice::GetTitleFileInterface() const
{
	return nullptr;
}

IOnlineStorePtr FOnlineSubsystemJustice::GetStoreInterface() const
{
	return nullptr;
}

IOnlineEventsPtr FOnlineSubsystemJustice::GetEventsInterface() const
{
	return nullptr;
}

IOnlineAchievementsPtr FOnlineSubsystemJustice::GetAchievementsInterface() const
{
	return AchievementsInterface;
}

IOnlineSharingPtr FOnlineSubsystemJustice::GetSharingInterface() const
{
	return nullptr;
}

IOnlineUserPtr FOnlineSubsystemJustice::GetUserInterface() const
{
	return nullptr;
}

IOnlineMessagePtr FOnlineSubsystemJustice::GetMessageInterface() const
{
	return nullptr;
}

IOnlinePresencePtr FOnlineSubsystemJustice::GetPresenceInterface() const
{
	return nullptr;
}

IOnlineChatPtr FOnlineSubsystemJustice::GetChatInterface() const
{
	return nullptr;
}

IOnlineTurnBasedPtr FOnlineSubsystemJustice::GetTurnBasedInterface() const
{
    return nullptr;
}

bool FOnlineSubsystemJustice::Tick(float DeltaTime)
{
	if (!FOnlineSubsystemImpl::Tick(DeltaTime))
	{
		return false;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		OnlineAsyncTaskThreadRunnable->GameTick();
	}

 	if (SessionInterface.IsValid())
 	{
 		SessionInterface->Tick(DeltaTime);
 	}

	if (VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		VoiceInterface->Tick(DeltaTime);
	}

	return true;
}

bool FOnlineSubsystemJustice::Init()
{
	const bool bJusticeInit = true;
    bool bIsOk = true;

	if (bJusticeInit)
	{
		// Create the online async task thread
		OnlineAsyncTaskThreadRunnable = new FOnlineAsyncTaskManagerJustice(this);
		check(OnlineAsyncTaskThreadRunnable);
		OnlineAsyncTaskThread = FRunnableThread::Create(OnlineAsyncTaskThreadRunnable, *FString::Printf(TEXT("OnlineAsyncTaskThreadJustice %s(%d)"), *InstanceName.ToString(), TaskCounter.Increment()), 128 * 1024, TPri_Normal);
		check(OnlineAsyncTaskThread);
		UE_LOG_ONLINE(Verbose, TEXT("Created thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID());

 		SessionInterface = MakeShareable(new FOnlineSessionJustice(this));
		LeaderboardsInterface = MakeShareable(new FOnlineLeaderboardsJustice(this));
		IdentityInterface = MakeShareable(new FOnlineIdentityJustice(this));
		AchievementsInterface = MakeShareable(new FOnlineAchievementsJustice(this));
		VoiceInterface = MakeShareable(new FOnlineVoiceImpl(this));
        
        if (GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ApiUrl"), ApiUrl, GEngineIni))
        {
            UE_LOG_ONLINE(Display, TEXT("Justice API URL: %s"), *ApiUrl);
        }
        else
        {
            UE_LOG_ONLINE(Error, TEXT("Required Justice API URL config ApiUrl is unset."));
            bIsOk = false;
        }

        UE_LOG_ONLINE(Display,TEXT("Loaded OnlineSubsystem%s()"), TEXT("Justice"));

        FIamClient* iam = new FIamClient();
        iam->ClientLogin(ApiUrl, TEXT(""), TEXT(""));

    }
    else
	{
		Shutdown();
	}

	return bIsOk;
}

bool FOnlineSubsystemJustice::Shutdown()
{
	UE_LOG_ONLINE(Display, TEXT("FOnlineSubsystemJustice::Shutdown()"));

	FOnlineSubsystemImpl::Shutdown();

	if (OnlineAsyncTaskThread)
	{
		// Destroy the online async task thread
		delete OnlineAsyncTaskThread;
		OnlineAsyncTaskThread = nullptr;
	}

	if (OnlineAsyncTaskThreadRunnable)
	{
		delete OnlineAsyncTaskThreadRunnable;
		OnlineAsyncTaskThreadRunnable = nullptr;
	}

	if (VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		VoiceInterface->Shutdown();
	}
	
#define DESTRUCT_INTERFACE(Interface) \
 	if (Interface.IsValid()) \
 	{ \
 		ensure(Interface.IsUnique()); \
 		Interface = nullptr; \
 	}
 
 	// Destruct the interfaces
	DESTRUCT_INTERFACE(VoiceInterface);
	DESTRUCT_INTERFACE(AchievementsInterface);
	DESTRUCT_INTERFACE(IdentityInterface);
	DESTRUCT_INTERFACE(LeaderboardsInterface);
 	DESTRUCT_INTERFACE(SessionInterface);
	
#undef DESTRUCT_INTERFACE
	
	return true;
}

FString FOnlineSubsystemJustice::GetAppId() const
{
    return GConfig->GetStr(TEXT("OnlineSubsystemJustice"), TEXT("AppId"), GEngineIni);
}

bool FOnlineSubsystemJustice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FOnlineSubsystemImpl::Exec(InWorld, Cmd, Ar))
	{
		return true;
	}
	return false;
}

bool FOnlineSubsystemJustice::IsEnabled()
{
    bool bEnableJustice = true;
    GConfig->GetBool(TEXT("OnlineSubsystemJustice"), TEXT("bEnabled"), bEnableJustice, GEngineIni);
    return bEnableJustice;
}
