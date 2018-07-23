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

	FString BaseURL;
	FString Namespace;
	FString ClientID;
	FString ClientSecret;
	class FRunnableThread* OnlineAsyncTaskThread;
	class FRunnableThread* RetryAsyncTaskThread;
	FString GooglePlatformURL;
	FString GoogleRedirectURL;
	FString FacebookPlatformURL;
	FString FacebookRedirectURL;

	OAuthTokenJustice*	GameClientToken;
	OAuthTokenJustice* UserToken;
	FString LoginID;
	bool bHeadlessAccount;
	FString Password;
	UserProfileInfo* UserProfile;
	FAsyncTaskManagerJustice* AsyncTaskManager;
	FRetryTaskManagerJustice* RetryTaskManager;

private:
	FCriticalSection GameClientCritical;
};

#define FJusticeRetryManager FJusticeSDKModule::Get().RetryTaskManager
#define FJusticeRefreshManager FJusticeSDKModule::Get().AsyncTaskManager
#define FJusticeBaseURL FJusticeSDKModule::Get().BaseURL
#define FJusticeNamespace FJusticeSDKModule::Get().Namespace
#define FJusticeUserToken FJusticeSDKModule::Get().UserToken
#define FJusticeGameClientToken FJusticeSDKModule::Get().GameClientToken
#define FJusticeUserID FJusticeSDKModule::Get().UserToken->UserID

#define FGooglePlatformURL FJusticeSDKModule::Get().GooglePlatformURL
#define FGoogleRedirectURL FJusticeSDKModule::Get().GoogleRedirectURL
#define FFacebookPlatformURL FJusticeSDKModule::Get().FacebookPlatformURL
#define FFacebookRedirectURL FJusticeSDKModule::Get().FacebookRedirectURL

#define INITIAL_WAIT 1
#define INITIAL_ELAPSED_WAIT 0
#define MAX_ELAPSED_WAIT 60