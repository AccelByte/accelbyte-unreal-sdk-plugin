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
		FJusticeSDKModule& module = FModuleManager::LoadModuleChecked< FJusticeSDKModule >("JusticeSDK");
		check(module.IsInitialized);
		return module;
	}

	static inline FJusticeSDKModule& GetModule()
	{
		FJusticeSDKModule& module = FModuleManager::GetModuleChecked< FJusticeSDKModule >("JusticeSDK");
		check(module.IsInitialized);
		return module;
	}


	//LogModuleManager: Warning: ModuleManager: Attempting to load 'JusticeSDK' outside the main thread.  
	// This module was already loaded - so we didn't crash but this isn't safe.  
	// Please call LoadModule on the main/game thread only.  
	// You can use GetModule or GetModuleChecked instead, those are safe to call outside the game thread.



	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("JusticeSDK");
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool GameClientParseJson(FString json);
	bool UserParseJson(FString json);

	FString BaseURL;
	FString Namespace;
	FString ClientID;
	FString ClientSecret;
	class FRunnableThread* OnlineAsyncTaskThread;
	class FRunnableThread* RetryAsyncTaskThread;

	OAuthTokenJustice*	GameClientToken;
	OAuthTokenJustice* UserToken;
	FString LoginId;
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

#define FJusticeUserID FJusticeSDKModule::Get().UserToken->UserId


#define INITIAL_WAIT 1
#define INITIAL_ELAPSED_WAIT 0
#define MAX_ELAPSED_WAIT 60
