// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "JusticeHttpModule.h"
#include "Misc/ConfigCacheIni.h"
#include "Modules/ModuleManager.h"
#include "HttpManager.h"
#include "JusticeHttp.h"
#include "NullHttp.h"
#include "HttpTests.h"

DEFINE_LOG_CATEGORY(LogJusticeHttp);

// FJusticeHttpModule

IMPLEMENT_MODULE(FJusticeHttpModule, HTTP);

FJusticeHttpModule* FJusticeHttpModule::Singleton = NULL;

void FJusticeHttpModule::StartupModule()
{	
	Singleton = this;
	MaxReadBufferSize = 256 * 1024;

	FPlatformJusticeHttp::Init();

	HttpTimeout = 300.0f;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpTimeout"), HttpTimeout, GEngineIni);

	HttpConnectionTimeout = -1;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpConnectionTimeout"), HttpConnectionTimeout, GEngineIni);

	HttpReceiveTimeout = HttpConnectionTimeout;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpReceiveTimeout"), HttpReceiveTimeout, GEngineIni);

	HttpSendTimeout = HttpConnectionTimeout;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpSendTimeout"), HttpSendTimeout, GEngineIni);

	HttpMaxConnectionsPerServer = 16;
	GConfig->GetInt(TEXT("HTTP"), TEXT("HttpMaxConnectionsPerServer"), HttpMaxConnectionsPerServer, GEngineIni);
	
	bEnableHttp = true;
	GConfig->GetBool(TEXT("HTTP"), TEXT("bEnableHttp"), bEnableHttp, GEngineIni);

	bUseNullHttp = false;
	GConfig->GetBool(TEXT("HTTP"), TEXT("bUseNullHttp"), bUseNullHttp, GEngineIni);

	HttpDelayTime = 0;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpDelayTime"), HttpDelayTime, GEngineIni);

	HttpThreadActiveFrameTimeInSeconds = 1.0f / 200.0f; // 200Hz
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpThreadActiveFrameTimeInSeconds"), HttpThreadActiveFrameTimeInSeconds, GEngineIni);

	HttpThreadActiveMinimumSleepTimeInSeconds = 0.0f;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpThreadActiveMinimumSleepTimeInSeconds"), HttpThreadActiveMinimumSleepTimeInSeconds, GEngineIni);

	HttpThreadIdleFrameTimeInSeconds = 1.0f / 30.0f; // 30Hz
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpThreadIdleFrameTimeInSeconds"), HttpThreadIdleFrameTimeInSeconds, GEngineIni);

	HttpThreadIdleMinimumSleepTimeInSeconds = 0.0f;
	GConfig->GetFloat(TEXT("HTTP"), TEXT("HttpThreadIdleMinimumSleepTimeInSeconds"), HttpThreadIdleMinimumSleepTimeInSeconds, GEngineIni);

	HttpManager = FPlatformJusticeHttp::CreatePlatformHttpManager();
	if (NULL == HttpManager)
	{
		// platform does not provide specific HTTP manager, use generic one
		HttpManager = new FHttpManager();
	}
	HttpManager->Initialize();
}

void FJusticeHttpModule::PostLoadCallback()
{

}

void FJusticeHttpModule::PreUnloadCallback()
{
}

void FJusticeHttpModule::ShutdownModule()
{
	if (HttpManager != nullptr)
	{
		// block on any http requests that have already been queued up
		HttpManager->Flush(true);
	}

#if PLATFORM_WINDOWS

	extern bool bUseCurl;
	if (!bUseCurl)
	{
		// due to peculiarities of some platforms (notably Windows with WinInet implementation) we need to shutdown platform http first,
		// then delete the manager. It is more logical to have reverse order of their creation though. Proper fix
		// would be refactoring HTTP platform abstraction to make HttpManager a proper part of it.
		FPlatformJusticeHttp::Shutdown();

		delete HttpManager;	// can be passed NULLs
	}
	else
#endif	// PLATFORM_WINDOWS
	{
		// at least on Linux, the code in HTTP manager (e.g. request destructors) expects platform to be initialized yet
		delete HttpManager;	// can be passed NULLs

		FPlatformJusticeHttp::Shutdown();
	}

	HttpManager = nullptr;
	Singleton = nullptr;
}

bool FJusticeHttpModule::HandleHTTPCommand(const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("TEST")))
	{
		int32 Iterations=1;
		FString IterationsStr;
		FParse::Token(Cmd, IterationsStr, true);
		if (!IterationsStr.IsEmpty())
		{
			Iterations = FCString::Atoi(*IterationsStr);
		}		
		FString Url;
		FParse::Token(Cmd, Url, true);
		if (Url.IsEmpty())
		{
			Url = TEXT("http://www.google.com");
		}		
		FHttpTest* HttpTest = new FHttpTest(TEXT("GET"),TEXT(""),Url,Iterations);
		HttpTest->Run();
	}
	else if (FParse::Command(&Cmd, TEXT("DUMPREQ")))
	{
		GetHttpManager().DumpRequests(Ar);
	}
	return true;	
}

bool FJusticeHttpModule::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// Ignore any execs that don't start with HTTP
	if (FParse::Command(&Cmd, TEXT("HTTP")))
	{
		return HandleHTTPCommand( Cmd, Ar );
	}
	return false;
}

FJusticeHttpModule& FJusticeHttpModule::Get()
{
	if (Singleton == NULL)
	{
		check(IsInGameThread());
		FModuleManager::LoadModuleChecked<FJusticeHttpModule>("HTTP");
	}
	check(Singleton != NULL);
	return *Singleton;
}

TSharedRef<IJusticeHttpRequest> FJusticeHttpModule::CreateRequest()
{
	if (bUseNullHttp)
	{
		return TSharedRef<IJusticeHttpRequest>(new FNullHttpRequest());
	}
	else
	{
		// Create the platform specific Http request instance
		return TSharedRef<IJusticeHttpRequest>(FPlatformJusticeHttp::ConstructRequest());
	}
}

