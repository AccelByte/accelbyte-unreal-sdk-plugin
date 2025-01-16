// Copyright (c) 2020-2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/ServerTime/AccelByteTimeManager.h"

#include "Core/AccelByteHttpRetryTask.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteServerSettings.h"
#include "Misc/Timespan.h"

namespace AccelByte
{
	
FAccelByteTimeManager::FAccelByteTimeManager(AccelByte::FHttpRetryScheduler& Http)
	: bUseSharedResources(true)
	, HttpRef(Http.AsShared())
{
	ReferenceCount.Increment();
}

FAccelByteTimeManager::FAccelByteTimeManager()
	: HttpRef(MakeShared<FHttpRetryScheduler, ESPMode::ThreadSafe>())
{
	HttpRef->Startup();
};

FAccelByteTimeManager::~FAccelByteTimeManager()
{
	ReferenceCount.Decrement();
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

	if (AccelByteGetServerTimeTaskWPtr.IsValid())
	{
		auto TaskPtr = AccelByteGetServerTimeTaskWPtr.Pin();
		if (TaskPtr.IsValid())
		{
			TaskPtr->Cancel();
		}
	}

	if (ReferenceCount.GetValue() == 0)
	{
		Reset();
	}
}

void FAccelByteTimeManager::Reset()
{
	CachedServerTime = FDateTime::MinValue();
	CurrentServerTime = FDateTime::MinValue();
	ServerTimeLastUpdated = FDateTime::MinValue();
}

FAccelByteTaskWPtr FAccelByteTimeManager::GetServerTime(THandler<FTime> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bForceSync)
{
	FReport::Log(FString(__FUNCTION__));

	bool bShouldSync{ bForceSync };
	FDateTime MinimumDateTime{ FDateTime::MinValue() };

	if (CachedServerTime == MinimumDateTime)
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
			auto TaskPtr = AccelByteGetServerTimeTaskWPtr.Pin();
			if (TaskPtr.IsValid())
			{
				auto RetryTaskPtr = StaticCastSharedPtr<FHttpRetryTask>(TaskPtr);
				if (RetryTaskPtr.IsValid())
				{
					FScopeLock ScopeLock(&ServerTimeLock);
					ServerTimeLastUpdated = RetryTaskPtr->GetResponseTime();
					CachedServerTime = ServerTime.CurrentTime;
				}
			}
			OnSuccess.ExecuteIfBound(ServerTime);
		});

		return AccelByteGetServerTimeTaskWPtr = FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(SuccessDelegate, OnError), FPlatformTime::Seconds());
	}
	else
	{
		FTime ServerTime{};
		ServerTime.CurrentTime = CurrentServerTime;
		OnSuccess.ExecuteIfBound(ServerTime);
		return nullptr;
	}
}

FDateTime FAccelByteTimeManager::GetCachedServerTime() const
{
	return CachedServerTime;
}

FDateTime FAccelByteTimeManager::GetCurrentServerTime()
{
	BackCalculateServerTime();
	return CurrentServerTime;
}

bool FAccelByteTimeManager::IsInSync()
{
	return GetCurrentServerTime() != FDateTime::MinValue();
}

FTimespan FAccelByteTimeManager::BackCalculateServerTime()
{
	FTimespan DeltaTime{ -1 };
	if (CachedServerTime != FDateTime::MinValue())
	{
		FDateTime CurrentTime = FDateTime::UtcNow();
		DeltaTime = CurrentTime - ServerTimeLastUpdated;
		CurrentServerTime = CachedServerTime + DeltaTime;
	}
	return DeltaTime;
}

}