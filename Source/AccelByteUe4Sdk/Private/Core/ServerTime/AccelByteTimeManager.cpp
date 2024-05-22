// Copyright (c) 2020-2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/ServerTime/AccelByteTimeManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteServerSettings.h"
#include "Misc/Timespan.h"

namespace AccelByte
{

static FDateTime SERVER_TIME_CACHED{ FDateTime::MinValue() };
static FDateTime SERVER_TIME_CURRENT{ FDateTime::MinValue() };
static FDateTime LAST_UPDATED_SERVER_TIME{ FDateTime::MinValue() };

FAccelByteTimeManager::FAccelByteTimeManager()
	: bUseSharedResources(false)
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
{
	HttpRef->Startup();
}

FAccelByteTimeManager::FAccelByteTimeManager(AccelByte::FHttpRetryScheduler& Http)
	: bUseSharedResources(true)
	, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http,
		[](AccelByte::FHttpRetryScheduler*) {}))
{
}

FAccelByteTimeManager::~FAccelByteTimeManager()
{
	if (!bUseSharedResources)
	{
		const bool bShouldExecuteImmediately = IsInGameThread() || !FTaskGraphInterface::IsRunning();
		if (bShouldExecuteImmediately)
		{
			HttpRef->Shutdown();
		}
		else
		{
			FFunctionGraphTask::CreateAndDispatchWhenReady(
				[HttpScheduler = HttpRef]()
				{
					HttpScheduler->Shutdown();
				}
				, TStatId()
					, nullptr
					, ENamedThreads::GameThread);
		}
	}
	Reset();
}

void FAccelByteTimeManager::Reset()
{
	SERVER_TIME_CACHED = FDateTime::MinValue();
	SERVER_TIME_CURRENT = FDateTime::MinValue();
	LAST_UPDATED_SERVER_TIME = FDateTime::MinValue();
}

FAccelByteTaskWPtr FAccelByteTimeManager::GetServerTime(THandler<FTime> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bForceSync)
{
	FReport::Log(FString(__FUNCTION__));

	bool bShouldSync{ bForceSync };
	FDateTime MinimumDateTime{ FDateTime::MinValue() };
	FDateTime CurrentServerTime{ 0 };

	if (SERVER_TIME_CACHED == MinimumDateTime)
	{
		bShouldSync = true;
	}
	else
	{
		FTimespan DeltaTime = BackCalculateServerTime();

		// #NOTE: Should sync ServerTime after an hour span from the cached ServerTime.
		if (DeltaTime.GetTotalSeconds() <= 0.0f || DeltaTime.GetTotalSeconds() >= 3600.0f)
		{
			bShouldSync = true;
		}
	}

	if (bShouldSync)
	{
		FString BasicServerUrl;

		if (IsRunningDedicatedServer())
		{
			BasicServerUrl = FRegistry::ServerSettings.BasicServerUrl;
		}
		else
		{
			BasicServerUrl = FRegistry::Settings.BasicServerUrl;
		}

		const FString Url = FString::Printf(TEXT("%s/v1/public/misc/time"), *BasicServerUrl);
		const FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

		Request->SetVerb(TEXT("GET"));
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

		auto SuccessDelegate = THandler<FTime>::CreateLambda([OnSuccess, this](FTime const& ServerTime)
			{
				{
					FScopeLock ScopeLock(&ServerTimeLock);
					LAST_UPDATED_SERVER_TIME = FDateTime::UtcNow();
					SERVER_TIME_CACHED = ServerTime.CurrentTime;
				}
				OnSuccess.ExecuteIfBound(ServerTime);
			});

		return FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(SuccessDelegate, OnError), FPlatformTime::Seconds());
	}
	else
	{
		FTime ServerTime{};
		ServerTime.CurrentTime = SERVER_TIME_CURRENT;
		OnSuccess.ExecuteIfBound(ServerTime);
		return nullptr;
	}
}

FDateTime FAccelByteTimeManager::GetCachedServerTime() const
{
	return SERVER_TIME_CACHED;
}

FDateTime FAccelByteTimeManager::GetCurrentServerTime() const
{
	BackCalculateServerTime();
	return SERVER_TIME_CURRENT;
}

bool FAccelByteTimeManager::IsInSync()
{
	return GetCurrentServerTime() != FDateTime::MinValue();
}

FTimespan FAccelByteTimeManager::BackCalculateServerTime() const
{
	FTimespan DeltaTime{ -1 };
	if (SERVER_TIME_CACHED != FDateTime::MinValue())
	{
		FDateTime CurrentTime = FDateTime::UtcNow();
		DeltaTime = CurrentTime - LAST_UPDATED_SERVER_TIME;
		SERVER_TIME_CURRENT = SERVER_TIME_CACHED + DeltaTime;
	}
	return DeltaTime;
}

}