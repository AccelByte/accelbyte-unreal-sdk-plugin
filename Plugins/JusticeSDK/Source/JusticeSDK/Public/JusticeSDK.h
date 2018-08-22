// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "Utilities/AsyncTaskManagerJustice.h"
#include "Utilities/RetryTaskManagerJustice.h"
#include "Models/OAuthTokenJustice.h"
#include "Models/UserProfileInfo.h"

DECLARE_DELEGATE_TwoParams(FDefaultCompleteDelegate, bool, FString);

class FJusticeSDKModule : public IModuleInterface
{
public:
	bool IsInitialized;
	static inline FJusticeSDKModule& Get()
	{
		FJusticeSDKModule& moduleInstance = FModuleManager::LoadModuleChecked< FJusticeSDKModule >("JusticeSDK");
		check(moduleInstance.IsInitialized);
		return moduleInstance;
	}
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("JusticeSDK");
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool ParseClientToken(FString json);
	bool ParseUserToken(FString json);
	bool ParsePublisherUserToken(FString json);

	FString                                 BaseURL;
	FString                                 Namespace;
	FString                                 ClientID;
	FString                                 ClientSecret;
	FString                                 AppID;
	FString                                 PublisherNamespace;
	class FRunnableThread*                  OnlineAsyncTaskThread;
	class FRunnableThread*                  RetryAsyncTaskThread;
	FString                                 GooglePlatformURL;
	FString                                 GoogleRedirectURL;
	FString                                 FacebookPlatformURL;
	FString                                 FacebookRedirectURL;
	FString                                 RedirectURI;
	TArray<FString>                         SupportedPlatform;
	TSharedPtr<FOAuthTokenJustice>          GameClientToken;
	TSharedPtr<FOAuthTokenJustice>          UserToken;
	TSharedPtr<FOAuthTokenJustice>          PublisherUserToken;
	FString                                 LoginID;
	bool                                    bHeadlessAccount;
	FString                                 Password;
	TSharedPtr<FUserProfileInfo>            UserProfile;
	TSharedPtr<FAsyncTaskManagerJustice>    AsyncTaskManager;
	TSharedPtr<FRetryTaskManagerJustice>    RetryTaskManager;

    // Steam Specific
    FString                                 SteamAuthTicket;
    FString                                 SteamNickName;
    UTexture2D*                             Avatar;
	bool                                    bClientCredentialReady;

private:
	FCriticalSection                        GameClientCritical;
};

#define FJusticeRetryManager FJusticeSDKModule::Get().RetryTaskManager
#define FJusticeRefreshManager FJusticeSDKModule::Get().AsyncTaskManager
#define FJusticeBaseURL FJusticeSDKModule::Get().BaseURL
#define JusticeGameNamespace FJusticeSDKModule::Get().Namespace
#define FJusticeUserToken FJusticeSDKModule::Get().UserToken
#define FJusticeGameClientToken FJusticeSDKModule::Get().GameClientToken
#define FJusticeUserID FJusticeSDKModule::Get().UserToken->UserID
#define FJusticePublisherUserID FJusticeSDKModule::Get().PublisherUserToken->UserID
#define FJusticeAppID FJusticeSDKModule::Get().AppID
#define JusticePublisherNamespace FJusticeSDKModule::Get().PublisherNamespace

#define FGooglePlatformURL FJusticeSDKModule::Get().GooglePlatformURL
#define FGoogleRedirectURL FJusticeSDKModule::Get().GoogleRedirectURL
#define FFacebookPlatformURL FJusticeSDKModule::Get().FacebookPlatformURL
#define FFacebookRedirectURL FJusticeSDKModule::Get().FacebookRedirectURL

#define INITIAL_WAIT 1
#define INITIAL_ELAPSED_WAIT 0
#define MAX_ELAPSED_WAIT 60