// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "Private/Models/OAuthTokenJustice.h"
#include "Private/Models/UserProfileInfo.h"
#include "AsyncTaskManagerJustice.h"

class FJusticeSDKModule : public IModuleInterface
{
public:
	static inline FJusticeSDKModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FJusticeSDKModule >("JusticeSDK");
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
	OAuthTokenJustice*	GameClientToken;
	OAuthTokenJustice* UserToken;
	UserProfileInfo* UserProfile;

	FAsyncTaskManagerJustice* AsyncTaskManager;

private:
	FCriticalSection GameClientCritical;
};