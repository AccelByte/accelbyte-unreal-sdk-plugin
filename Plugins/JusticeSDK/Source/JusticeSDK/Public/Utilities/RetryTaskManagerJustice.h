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
#include "HttpJustice.h"

#define POLLING_INTERVAL_MS 1000

class FJusticeRetryTask 
{
public:
	FJusticeRetryTask(int wait, int elapsedWait):
		Done(false), 
		LastWait(wait),
		ElapsedWait(elapsedWait)
	{
		NextRetry = FDateTime::UtcNow() + FTimespan::FromSeconds(wait) + FTimespan::FromSeconds(FMath::RandRange(1, 60));
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
	int GetTotalElapsedWait() { return ElapsedWait + LastWait; }
	int GetElapsedWait() { return ElapsedWait; }

	void SetAsDone() { Done = true; }
	int GetNextWait() {return LastWait * 2;	}

private:
	bool Done;
	FDateTime NextRetry;
	int LastWait;
	int ElapsedWait;

};

class FWebRequestTask : public FJusticeRetryTask
{
public:
	FWebRequestTask(FJusticeHttpRequestPtr request, int waitTime, FWebRequestResponseDelegate reponseDelegate)
		: FJusticeRetryTask(waitTime, 0),
		  Request(request),
		  OnReponseDelegate(reponseDelegate)
	{}

	FString GetTaskName() { return TEXT("ClientLogin"); }


	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		FJusticeHTTP::CreateRequest(Request, OnReponseDelegate);
	}
private:
	FJusticeHttpRequestPtr Request;
	FWebRequestResponseDelegate OnReponseDelegate;
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
	void AddQueue(FJusticeRetryTask* NewTask);

	void AddQueue(FJusticeHttpRequestPtr Request, int WaitTime, FWebRequestResponseDelegate reponseDelegate)
	{
		FWebRequestTask* newTask = new FWebRequestTask(Request, WaitTime, reponseDelegate);
		check(newTask);
		AddQueue(newTask);
	}

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