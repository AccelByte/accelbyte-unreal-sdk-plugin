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
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("JusticeSDK");
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool GameClientParseJson(TSharedPtr<FJsonObject> jsonObject);
	bool UserParseJson(TSharedPtr<FJsonObject> jsonObject);

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