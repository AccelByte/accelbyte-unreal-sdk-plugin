// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#ifndef JUSTICE_SUBSYSTEM
#define JUSTICE_SUBSYSTEM FName(TEXT("JUSTICE"))
#endif

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemJusticePackage.h"
#include "HAL/ThreadSafeCounter.h"

class FOnlineAchievementsJustice;
class FOnlineIdentityJustice;
class FOnlineLeaderboardsJustice;
class FOnlineSessionJustice;
class FOnlineVoiceImpl;
class FOnlineAsyncTaskManagerJustice;

/** Forward declarations of all interface classes */
typedef TSharedPtr<class FOnlineSessionJustice, ESPMode::ThreadSafe> FOnlineSessionJusticePtr;
typedef TSharedPtr<class FOnlineProfileJustice, ESPMode::ThreadSafe> FOnlineProfileJusticePtr;
typedef TSharedPtr<class FOnlineFriendsJustice, ESPMode::ThreadSafe> FOnlineFriendsJusticePtr;
typedef TSharedPtr<class FOnlineUserCloudJustice, ESPMode::ThreadSafe> FOnlineUserCloudJusticePtr;
typedef TSharedPtr<class FOnlineLeaderboardsJustice, ESPMode::ThreadSafe> FOnlineLeaderboardsJusticePtr;
typedef TSharedPtr<class FOnlineVoiceImpl, ESPMode::ThreadSafe> FOnlineVoiceImplPtr;
typedef TSharedPtr<class FOnlineExternalUIJustice, ESPMode::ThreadSafe> FOnlineExternalUIJusticePtr;
typedef TSharedPtr<class FOnlineIdentityJustice, ESPMode::ThreadSafe> FOnlineIdentityJusticePtr;
typedef TSharedPtr<class FOnlineAchievementsJustice, ESPMode::ThreadSafe> FOnlineAchievementsJusticePtr;

/**
 *	OnlineSubsystemJustice - Implementation of the online subsystem for Justice services
 */
class ONLINESUBSYSTEMJUSTICE_API FOnlineSubsystemJustice :
	public FOnlineSubsystemImpl
{

public:

	virtual ~FOnlineSubsystemJustice()
	{
	}

	// IOnlineSubsystem

	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual IOnlinePartyPtr GetPartyInterface() const override;
	virtual IOnlineGroupsPtr GetGroupsInterface() const override;
	virtual IOnlineSharedCloudPtr GetSharedCloudInterface() const override;
	virtual IOnlineUserCloudPtr GetUserCloudInterface() const override;
	virtual IOnlineEntitlementsPtr GetEntitlementsInterface() const override;
	virtual IOnlineLeaderboardsPtr GetLeaderboardsInterface() const override;
	virtual IOnlineVoicePtr GetVoiceInterface() const override;
	virtual IOnlineExternalUIPtr GetExternalUIInterface() const override;	
	virtual IOnlineTimePtr GetTimeInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineTitleFilePtr GetTitleFileInterface() const override;
	virtual IOnlineStorePtr GetStoreInterface() const override;
	virtual IOnlineStoreV2Ptr GetStoreV2Interface() const override { return nullptr; }
	virtual IOnlinePurchasePtr GetPurchaseInterface() const override { return nullptr; }
	virtual IOnlineEventsPtr GetEventsInterface() const override;
	virtual IOnlineAchievementsPtr GetAchievementsInterface() const override;
	virtual IOnlineSharingPtr GetSharingInterface() const override;
	virtual IOnlineUserPtr GetUserInterface() const override;
	virtual IOnlineMessagePtr GetMessageInterface() const override;
	virtual IOnlinePresencePtr GetPresenceInterface() const override;
	virtual IOnlineChatPtr GetChatInterface() const override;
    virtual IOnlineTurnBasedPtr GetTurnBasedInterface() const override;
	virtual FText GetOnlineServiceName() const override;

	
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual FString GetAppId() const override;
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	FOnlineAsyncTaskManagerJustice* GetOnlineAsyncTaskManager() { return OnlineAsyncTaskThreadRunnable; }

	// FTickerObjectBase
	
	virtual bool Tick(float DeltaTime) override;

	// FOnlineSubsystemJustice

	/**
	 * Is the Justice API available for use
	 * @return true if Justice functionality is available, false otherwise
	 */
	bool IsEnabled();

PACKAGE_SCOPE:

	/** Only the factory makes instances */
	FOnlineSubsystemJustice(FName InInstanceName) :
		FOnlineSubsystemImpl(JUSTICE_SUBSYSTEM, InInstanceName),
		SessionInterface(nullptr),
		VoiceInterface(nullptr),
		bVoiceInterfaceInitialized(false),
		LeaderboardsInterface(nullptr),
		IdentityInterface(nullptr),
		AchievementsInterface(nullptr),
		OnlineAsyncTaskThreadRunnable(nullptr),
		OnlineAsyncTaskThread(nullptr)
	{}

	FOnlineSubsystemJustice() :
		SessionInterface(nullptr),
		VoiceInterface(nullptr),
		bVoiceInterfaceInitialized(false),
		LeaderboardsInterface(nullptr),
		IdentityInterface(nullptr),
		AchievementsInterface(nullptr),
		OnlineAsyncTaskThreadRunnable(nullptr),
		OnlineAsyncTaskThread(nullptr)
    {}

private:

	/** Interface to the session services */
	FOnlineSessionJusticePtr SessionInterface;

	/** Interface for voice communication */
	mutable FOnlineVoiceImplPtr VoiceInterface;

	/** Interface for voice communication */
	mutable bool bVoiceInterfaceInitialized;

	/** Interface to the leaderboard services */
	FOnlineLeaderboardsJusticePtr LeaderboardsInterface;

	/** Interface to the identity registration/auth services */
	FOnlineIdentityJusticePtr IdentityInterface;

	/** Interface for achievements */
	FOnlineAchievementsJusticePtr AchievementsInterface;

	/** Online async task runnable */
	class FOnlineAsyncTaskManagerJustice* OnlineAsyncTaskThreadRunnable;

	/** Online async task thread */
	class FRunnableThread* OnlineAsyncTaskThread;

	// task counter, used to generate unique thread names for each task
	static FThreadSafeCounter TaskCounter;
    
    /** API Url */
    FString BaseURL;
};

typedef TSharedPtr<FOnlineSubsystemJustice, ESPMode::ThreadSafe> FOnlineSubsystemJusticePtr;

