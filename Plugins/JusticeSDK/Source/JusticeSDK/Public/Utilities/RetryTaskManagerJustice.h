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
#include "HTTPJustice.h"
#include "JusticeLog.h"

#define POLLING_INTERVAL_MS 1000

class FJusticeRetryTask 
{
public:
	FJusticeRetryTask(int32 Wait, int32 ElapsedWaitValue):
		bDone(false), 
		LastWait(Wait),
		ElapsedWait(ElapsedWaitValue)
	{
		NextRetry = FDateTime::UtcNow() + FTimespan::FromSeconds(Wait);
	}

	virtual ~FJusticeRetryTask() {}

	virtual bool IsDone()
	{
		return bDone;
	}

	virtual bool WasSuccessful()
	{
		return bDone;
	}

	virtual FString ToString() const { return TEXT("FJusticeRetryTask"); }

	virtual void Tick() = 0;
	FDateTime GetNextRetry() { return NextRetry; };
	int32 GetTotalElapsedWait() { return ElapsedWait + LastWait; }
	int32 GetElapsedWait() { return ElapsedWait; }

	void SetAsDone() { bDone = true; }

private:
	bool bDone;
	FDateTime NextRetry;
	int32 LastWait;
	int32 ElapsedWait;

};

class FWebRequestTask : public FJusticeRetryTask
{
public:
	FWebRequestTask(TSharedPtr<JusticeHttpRequest> HttpRequest, int32 WaitTime, FWebRequestResponseDelegate ReponseDelegate)
		: FJusticeRetryTask(WaitTime, 0),
		  Request(HttpRequest),
		  OnReponseDelegate(ReponseDelegate)
	{}

	FString GetTaskName() { return TEXT("ClientLogin"); }


	virtual void Tick()
	{
		UE_LOG(LogJustice, Log, TEXT("Executing FWebRequestTask URL: %s"), *Request->URL);
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		FJusticeHTTP::CreateRequest(Request, OnReponseDelegate);
	}
private:
    TSharedPtr<JusticeHttpRequest> Request;
	FWebRequestResponseDelegate OnReponseDelegate;
};



/**
 *	Justice version of the async task manager to register the various retry Justice callbacks
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
	void AddQueue(TSharedPtr<JusticeHttpRequest> Request, int32 WaitTime, FWebRequestResponseDelegate ReponseDelegate);
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