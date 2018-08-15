// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeSDK.h"
#include "Engine.h"
#include "JusticeLog.h"
#include "Utilities/AsyncTaskManagerJustice.h"

#define LOCTEXT_NAMESPACE "FJusticeSDKModule"

void FJusticeSDKModule::StartupModule()
{
	IsInitialized = true;
	if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing BaseURL= in [JusticeSDK] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing Namespace= in [JusticeSDK] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing ClientId= in [JusticeSDK] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing ClientSecret= in [JusticeSDK] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("PlatformURL"), TEXT("GooglePlatformURL"), GooglePlatformURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing GooglePlatformURL= in [PlatformURL] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("PlatformURL"), TEXT("GoogleRedirectURL"), GoogleRedirectURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing GoogleRedirectURL= in [PlatformURL] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("PlatformURL"), TEXT("FacebookPlatformURL"), FacebookPlatformURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing FacebookPlatformURL= in [PlatformURL] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("PlatformURL"), TEXT("FacebookRedirectURL"), FacebookRedirectURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing FacebookRedirectURL= in [PlatformURL] of DefaultEngine.ini"));
		IsInitialized = false;
	}

	GameClientToken = new FOAuthTokenJustice;
	UserToken = new FOAuthTokenJustice;
	UserProfile = new FUserProfileInfo;

	AsyncTaskManager = new FAsyncTaskManagerJustice();
	RetryTaskManager = new FRetryTaskManagerJustice();

	OnlineAsyncTaskThread = FRunnableThread::Create(AsyncTaskManager, *FString::Printf(TEXT("AsyncTaskManagerJustice")), 128 * 1024, TPri_Normal);
	UE_LOG(LogJustice, Log, TEXT("Justice AsyncTaskManagerCreated thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID());

	RetryAsyncTaskThread = FRunnableThread::Create(RetryTaskManager, *FString::Printf(TEXT("AsyncTaskManagerJustice")), 128 * 1024, TPri_Normal);
	UE_LOG(LogJustice, Log, TEXT("Justice RetryAsyncTaskThread thread (ID:%d)."), RetryAsyncTaskThread->GetThreadID());

	check(OnlineAsyncTaskThread != nullptr)
	UE_LOG(LogJustice, Log, TEXT("Justice AsyncTaskManagerCreated thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID())
}

void FJusticeSDKModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
bool FJusticeSDKModule::ParseClientToken(FString json)
{
	FScopeLock lock(&GameClientCritical);
	if (GameClientToken->FromJson(json))
	{
		return true;
	}
	return false;
}

bool FJusticeSDKModule::ParseUserToken(FString json)
{
	if (FJusticeSDKModule::Get().UserToken->FromJson(json))
	{
		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJusticeSDKModule, JusticeSDK)