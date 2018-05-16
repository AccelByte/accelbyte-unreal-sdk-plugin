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

//DECLARE_DELEGATE_TwoParams(FOnScheduleTickDelegate, struct FDateTime, int32);

class FJusticeRetryTask 
{
public:
	FJusticeRetryTask(int wait, int totalElapsedWait):
		Done(false), 
		LastWait(wait),
		TotalElapsedWait(TotalElapsedWait)
	{
		NextRetry = FDateTime::UtcNow() + FTimespan::FromSeconds(wait);
	}

	virtual ~FJusticeRetryTask() {}

	virtual bool IsDone()
	{
		return Done;
	}

	virtual bool WasSuccessful()
	{
		return Done;
	}

	virtual FString ToString() const { return TEXT("FJusticeRetryTask"); }

	virtual void Tick() = 0;
	FDateTime GetNextRetry() { return NextRetry; };
	int GetTotalElapsedWait() { return TotalElapsedWait; }
	void SetAsDone() { Done = true; }
	int GetLastWait() {return LastWait;	}

private:
	bool Done;
	FDateTime NextRetry;
	int LastWait;
	int TotalElapsedWait;

};


/**
 *	Justice version of the async task manager to register the various Justice callbacks with the engine
 */
class FRetryTaskManagerJustice : public FRunnable, FSingleThreadRunnable
{
public:

	FRetryTaskManagerJustice():
		OnlineThreadId(0),
		WorkEvent(nullptr),
		PollingInterval(POLLING_INTERVAL_MS),
		bRequestingExit(false) {}

	~FRetryTaskManagerJustice()	{}

	bool Init(void);
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	virtual void Tick() {};
	virtual void OnlineTick() ;
	void AddToRetryQueue(FJusticeRetryTask* NewTask);
	void ClearRetryQueue();

private:	
	volatile uint32 OnlineThreadId;
	static int32 InvocationCount;
	FEvent* WorkEvent;
	uint32 PollingInterval;
	FThreadSafeBool bRequestingExit;
	TArray<FJusticeRetryTask*> InJusticeQueue;
	FCriticalSection InJusticeQueueLock;
};
