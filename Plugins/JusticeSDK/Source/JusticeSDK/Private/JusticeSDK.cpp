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
    if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("RedirectURI"), RedirectURI, GEngineIni))
    {
        UE_LOG(LogJustice, Error, TEXT("Missing RedirectURI = in [JusticeSDK] of DefaultEngine.ini"));
        IsInitialized = false;
    }
	if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("AppId"), AppID, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing AppId = in [JusticeSDK] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("JusticeSDK"), TEXT("PublisherNamespace"), PublisherNamespace, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing PublisherNamespace = in [JusticeSDK] of DefaultEngine.ini"));
		IsInitialized = false;
	}

    if (!GConfig->GetArray(TEXT("Platform"), TEXT("SupportedPlatform"), SupportedPlatform, GEngineIni))
    {
        UE_LOG(LogJustice, Error, TEXT("Missing SupportedPlatform = in [Platform] of DefaultEngine.ini"));
        IsInitialized = false;
    }
	if (!GConfig->GetString(TEXT("Platform"), TEXT("GooglePlatformURL"), GooglePlatformURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing GooglePlatform= in [Platform] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("Platform"), TEXT("GoogleRedirectURL"), GoogleRedirectURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing GoogleRedirectURL= in [Platform] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("Platform"), TEXT("FacebookPlatformURL"), FacebookPlatformURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing FacebookPlatform= in [Platform] of DefaultEngine.ini"));
		IsInitialized = false;
	}
	if (!GConfig->GetString(TEXT("Platform"), TEXT("FacebookRedirectURL"), FacebookRedirectURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing FacebookRedirectURL= in [Platform] of DefaultEngine.ini"));
		IsInitialized = false;
	}
  
	GameClientToken = MakeShared<FOAuthTokenJustice>();
	UserToken       = MakeShared<FOAuthTokenJustice>();
	UserProfile     = MakeShared<FUserProfileInfo>();

	AsyncTaskManager = TSharedPtr<FAsyncTaskManagerJustice>(new FAsyncTaskManagerJustice);
	RetryTaskManager = TSharedPtr<FRetryTaskManagerJustice>(new FRetryTaskManagerJustice);

	OnlineAsyncTaskThread = FRunnableThread::Create(AsyncTaskManager.Get(), *FString::Printf(TEXT("AsyncTaskManagerJustice")), 128 * 1024, TPri_Normal);
	UE_LOG(LogJustice, Log, TEXT("Justice AsyncTaskManagerCreated thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID());

	RetryAsyncTaskThread = FRunnableThread::Create(RetryTaskManager.Get(), *FString::Printf(TEXT("AsyncTaskManagerJustice")), 128 * 1024, TPri_Normal);
	UE_LOG(LogJustice, Log, TEXT("Justice RetryAsyncTaskThread thread (ID:%d)."), RetryAsyncTaskThread->GetThreadID());

	check(OnlineAsyncTaskThread != nullptr)
	UE_LOG(LogJustice, Log, TEXT("Justice AsyncTaskManagerCreated thread (ID:%d)."), OnlineAsyncTaskThread->GetThreadID())
}

void FJusticeSDKModule::ShutdownModule()
{
    delete OnlineAsyncTaskThread;
    delete RetryAsyncTaskThread;

}
bool FJusticeSDKModule::ParseClientToken(FString json)
{
	FScopeLock lock(&GameClientCritical);
	if (!GameClientToken.IsValid())
	{
		GameClientToken = MakeShared<FOAuthTokenJustice>();
	}

	if (GameClientToken->FromJson(json))
	{
		return true;
	}
	return false;
}

bool FJusticeSDKModule::ParseUserToken(FString json)
{
	if (!UserToken.IsValid())
	{
		UserToken = MakeShared<FOAuthTokenJustice>();
	}

	if (UserToken->FromJson(json))
	{
		return true;
	}
	return false;
}

bool FJusticeSDKModule::ParsePublisherUserToken(FString json)
{
	if (!PublisherUserToken.IsValid())
	{
		PublisherUserToken = MakeShared<FOAuthTokenJustice>();
	}

	if (PublisherUserToken->FromJson(json))
	{
		return true;
	}
	return false;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJusticeSDKModule, JusticeSDK)