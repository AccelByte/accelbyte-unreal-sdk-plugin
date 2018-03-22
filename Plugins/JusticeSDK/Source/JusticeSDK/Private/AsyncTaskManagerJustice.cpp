// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AsyncTaskManagerJustice.h"
#include "HAL/PlatformProcess.h"
#include "JusticeLog.h"
#include "JusticeIdentity.h"

int32 FAsyncTaskManagerJustice::InvocationCount = 0;

bool FAsyncTaskManagerJustice::Init(void)
{
	WorkEvent = FPlatformProcess::GetSynchEventFromPool();
	int32 PollingConfig = POLLING_INTERVAL_MS;
	if (GConfig->GetInt(TEXT("OnlineSubsystem"), TEXT("PollingIntervalInMs"), PollingConfig, GEngineIni))
	{
		PollingInterval = (uint32)PollingConfig;
	}
	return WorkEvent != nullptr;
}

void FAsyncTaskManagerJustice::OnlineTick()
{
	check(FPlatformTLS::GetCurrentThreadId() == OnlineThreadId || !FPlatformProcess::SupportsMultithreading());
	int32 QueueSize = 0;
	{
		FScopeLock LockInQueue(&InJusticeQueueLock);
		QueueSize = InJusticeQueue.Num();
	}
	if (QueueSize > 0)
	{
		FJusticeAsyncTask* Task = nullptr;
		{
			FScopeLock LockInQueue(&InJusticeQueueLock);
			Task = InJusticeQueue[0];
		}

		if (FDateTime::UtcNow() >= Task->GetNextUpdate()) {
			UE_LOG(LogJustice, Log, TEXT("Executing Refresh Token Task"));
			Task->Tick();
			{
				FScopeLock LockInQueue(&InJusticeQueueLock);
				InJusticeQueue.RemoveAt(0);
			}
		}
	}
}

void FAsyncTaskManagerJustice::AddToRefreshQueue(FJusticeAsyncTask * NewTask)
{
	FScopeLock Lock(&InJusticeQueueLock);
	InJusticeQueue.Add(NewTask);
}

void FAsyncTaskManagerJustice::ClearRefreshQueue()
{
	FScopeLock Lock(&InJusticeQueueLock);
	InJusticeQueue.Empty();
}

uint32 FAsyncTaskManagerJustice::Run()
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

void FAsyncTaskManagerJustice::Stop()
{
	UE_LOG(LogJustice, Display, TEXT("FOnlineAsyncTaskManager::Stop() "));
	bRequestingExit = true;
	WorkEvent->Trigger();
}

void FAsyncTaskManagerJustice::Exit()
{
	UE_LOG(LogJustice, Log, TEXT("FOnlineAsyncTaskManager::Exit() started"));
	FPlatformProcess::ReturnSynchEventToPool(WorkEvent);
	WorkEvent = nullptr;
	OnlineThreadId = 0;
	InvocationCount--;
	UE_LOG(LogJustice, Log, TEXT("FOnlineAsyncTaskManager::Exit() finished"));
}

