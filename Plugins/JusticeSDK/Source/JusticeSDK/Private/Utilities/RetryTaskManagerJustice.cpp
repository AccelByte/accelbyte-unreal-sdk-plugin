// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "RetryTaskManagerJustice.h"
#include "HAL/PlatformProcess.h"
#include "JusticeLog.h"
#include "JusticeIdentity.h"

int32 FRetryTaskManagerJustice::InvocationCount = 0;

bool FRetryTaskManagerJustice::Init(void)
{
	WorkEvent = FPlatformProcess::GetSynchEventFromPool();
	int32 PollingConfig = POLLING_INTERVAL_MS;
	if (GConfig->GetInt(TEXT("OnlineSubsystem"), TEXT("PollingIntervalInMs"), PollingConfig, GEngineIni))
	{
		PollingInterval = (uint32)PollingConfig;
	}
	return WorkEvent != nullptr;
}

void FRetryTaskManagerJustice::OnlineTick()
{
	check(FPlatformTLS::GetCurrentThreadId() == OnlineThreadId || !FPlatformProcess::SupportsMultithreading());
	int32 QueueSize = 0;
	{
		FScopeLock LockInQueue(&InJusticeQueueLock);
		QueueSize = InJusticeQueue.Num();
	}
	if (QueueSize > 0)
	{
		FJusticeRetryTask* Task = nullptr;
		{
			FScopeLock LockInQueue(&InJusticeQueueLock);
			Task = InJusticeQueue[0];
		}

		if (FDateTime::UtcNow() >= Task->GetNextRetry()) {
			UE_LOG(LogJustice, Log, TEXT("Executing Retry Task"));
			Task->Tick();
			{
				FScopeLock LockInQueue(&InJusticeQueueLock);
				InJusticeQueue.RemoveAt(0);
			}
		}
	}
}

void FRetryTaskManagerJustice::AddToRetryQueue(FJusticeRetryTask * NewTask)
{
	FScopeLock Lock(&InJusticeQueueLock);
	InJusticeQueue.Add(NewTask);
}

void FRetryTaskManagerJustice::ClearRetryQueue()
{
	FScopeLock Lock(&InJusticeQueueLock);
	InJusticeQueue.Empty();
}

uint32 FRetryTaskManagerJustice::Run()
{
	InvocationCount++;
	check(OnlineThreadId == 0);
	FPlatformAtomics::InterlockedExchange((volatile int32*)&OnlineThreadId, FPlatformTLS::GetCurrentThreadId());
	do
	{
		// Wait for a trigger event to start work
		WorkEvent->Wait(PollingInterval);
		if (!bRequestingExit)
		{
			OnlineTick();
		}
	} while (!bRequestingExit);

	return 0;
}

void FRetryTaskManagerJustice::Stop()
{
	UE_LOG(LogJustice, Display, TEXT("FRetryTaskManagerJustice::Stop() "));
	bRequestingExit = true;
	WorkEvent->Trigger();
}

void FRetryTaskManagerJustice::Exit()
{
	UE_LOG(LogJustice, Log, TEXT("FRetryTaskManagerJustice::Exit() started"));
	FPlatformProcess::ReturnSynchEventToPool(WorkEvent);
	WorkEvent = nullptr;
	OnlineThreadId = 0;
	InvocationCount--;
	UE_LOG(LogJustice, Log, TEXT("FRetryTaskManagerJustice::Exit() finished"));
}
