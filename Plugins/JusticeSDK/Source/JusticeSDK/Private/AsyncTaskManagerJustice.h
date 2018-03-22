// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "HAL/PlatformTime.h"
#include "HAL/PlatformProcess.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "Misc/SingleThreadRunnable.h"

#define POLLING_INTERVAL_MS 1000

DECLARE_DELEGATE_TwoParams(FOnScheduleTickDelegate, struct FDateTime, int32);

class FJusticeAsyncTask 
{
public:
	FJusticeAsyncTask(FDateTime nextUpdate):
		Done(false), 
		NextUpdate(nextUpdate)	{}

	virtual bool IsDone()
	{
		return Done;
	}

	virtual bool WasSuccessful()
	{
		return Done;
	}

	virtual FString ToString() const { return TEXT("FJusticeAsyncTask"); }

	virtual void Tick() = 0;
	FDateTime GetNextUpdate() { return NextUpdate; };
	void SetAsDone() { Done = true; }

private:
	FDateTime NextUpdate;
	bool Done;
};


/**
 *	Justice version of the async task manager to register the various Justice callbacks with the engine
 */
class FAsyncTaskManagerJustice : public FRunnable, FSingleThreadRunnable
{
public:

	FAsyncTaskManagerJustice():		
		WorkEvent(nullptr),
		PollingInterval(POLLING_INTERVAL_MS),
		bRequestingExit(false),
		OnlineThreadId(0) {}

	~FAsyncTaskManagerJustice()	{}

	bool Init(void);
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	virtual void Tick() {};
	virtual void OnlineTick() ;
	void AddToRefreshQueue(FJusticeAsyncTask* NewTask);
	void ClearRefreshQueue();

private:	
	volatile uint32 OnlineThreadId;
	static int32 InvocationCount;
	FEvent* WorkEvent;
	uint32 PollingInterval;
	FThreadSafeBool bRequestingExit;
	TArray<FJusticeAsyncTask*> InJusticeQueue;
	FCriticalSection InJusticeQueueLock;
};
