// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

namespace AccelByte 
{


class ACCELBYTEUE4SDK_API FAccelByteCancellationTokenSource
{
public:
	FAccelByteCancellationTokenSource()
		: State{ EState::Running } 
	{}
	~FAccelByteCancellationTokenSource() {}

	/**
	 * @brief Return true if the state is CancelRequested.
	 */
	FORCENOINLINE bool IsCancelRequested() { return State == EState::CancelRequested; }

	/**
	 * @brief Return true if the state is Cancelled.
	 */
	FORCENOINLINE bool IsCancelled() { return State == EState::Cancelled; }

	/**
	 * @brief Return true if the state is Done.
	 */
	FORCENOINLINE bool IsTaskDone() { return State == EState::Done; }

	/**
	 * @brief Request cancellation.
	 */
	FORCENOINLINE void Cancel()
	{
		switch (State)
		{
		case EState::Running: State = EState::CancelRequested;
		case EState::CancelRequested:
		case EState::Cancelled:
		case EState::Done:
		default: break;
		}
	}

	/**
	 * @brief Mark the task as cancelled successfully.
	 */
	FORCENOINLINE void MarkCancelled()
	{
		switch (State)
		{
		case EState::Running: break;
		case EState::CancelRequested: State = EState::Cancelled;
		case EState::Cancelled:
		case EState::Done:
		default: break;
		}
	}

	/**
	 * @brief Mark the task as done, cannot be cancelled.
	 */
	FORCENOINLINE void MarkDone()
	{
		switch (State)
		{
		case EState::Running: State = EState::Done;
		case EState::CancelRequested:
		case EState::Cancelled:
		case EState::Done:
		default: break;
		}
	}

private:
	enum class EState
	{
		/** Task running normally. */
		Running,

		/** Cancellation requested. */
		CancelRequested,

		/** Task Succesfully cancelled. */
		Cancelled,

		/** Task is finished already, cannot be cancelled. */
		Done
	};

	EState State;
	
};

typedef TSharedPtr<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe> FAccelByteCancellationTokenPtr;
typedef TSharedRef<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe> FAccelByteCancellationTokenRef;
typedef TWeakPtr<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe> FAccelByteCancellationTokenWeakPtr;

class ACCELBYTEUE4SDK_API FAccelByteCancellationToken
{
private:
	FAccelByteCancellationTokenWeakPtr Token;
public:
	FAccelByteCancellationToken(FAccelByteCancellationTokenWeakPtr Token = MakeShared<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe>())
		: Token{ Token }
	{}
	~FAccelByteCancellationToken() {}

	/**
	 * @brief Request cancellation
	 */
	FORCENOINLINE void Cancel()
	{ 
		FAccelByteCancellationTokenPtr TokenPtr = Token.Pin();
		if (Token.IsValid())
		{
			TokenPtr->Cancel();
		}
	}

	/**
	 * @brief Will return true when task is on cancellation or already cancelled.
	 */
	FORCENOINLINE bool IsCancelled()
	{ 
		FAccelByteCancellationTokenPtr TokenPtr = Token.Pin();
		if (Token.IsValid())
		{
			return TokenPtr->IsCancelled() || TokenPtr->IsCancelRequested();
		}
		return true;
	}
};

enum class EAccelByteTaskState
{
	/** Task has completed execution. */
	Completed,

	/** Task execution is still pending. */
	Pending,

	/** Task execution is paused because Bearer Auth Rejected. */
	Paused,

	/** Task is executing. */
	Running,

	/** Task execution failed. */
	Failed,

	/** Task has been canceled. */
	Cancelled,

	/** Task has been canceled. */
	Retrying,
};

class FAccelByteTask;

typedef TSharedPtr<FAccelByteTask, ESPMode::ThreadSafe> FAccelByteTaskPtr;
typedef TSharedRef<FAccelByteTask, ESPMode::ThreadSafe> FAccelByteTaskRef;

/**
 * Base class for AccelByte Task that can be used in one of the schedulers
 */
class ACCELBYTEUE4SDK_API FAccelByteTask
{
public:
	FAccelByteTask();
	virtual ~FAccelByteTask();

	/**
	 * @brief Start the Task.
	 */
	virtual bool Start() { return true; }

	/**
	 * @brief Stop the Task in the middle of the process.
	 */
	virtual bool Cancel() 
	{ 
		Token->MarkCancelled();
		return true; 
	}

	/**
	 * @brief Task Execution ticks.
	 *
	 * @param Time Timer for the ticks
	 */
	virtual void Tick(double Time) 
	{ 
		if (Token->IsCancelRequested())
		{
			Cancel();
		}
		TaskTime += Time; 
	}

	/**
	 * @brief Finish the Task.
	 */
	virtual bool Finish() 
	{
		Token->MarkDone();
		return bIsFinished = true; 
	}

	/**
	 * @brief Current State of the Task.
	 */
	virtual EAccelByteTaskState State() const { return TaskState; }

	/**
	 * @brief Get current Task Time.
	 */
	virtual double Time() const { return TaskTime; }

	/**
	 * @brief Create Cancellation Token handler.
	 */
	FAccelByteCancellationToken GetCancellationToken()
	{
		return FAccelByteCancellationToken(Token);
	}

protected:
	double TaskTime;
	EAccelByteTaskState TaskState;
	bool bIsFinished;
	FAccelByteCancellationTokenRef Token;
};

FORCEINLINE FAccelByteTask::FAccelByteTask()
	: TaskTime{ 0.0f }
	, TaskState{ EAccelByteTaskState::Pending }
	, bIsFinished{ false }
	, Token{ MakeShared<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe>() }
{

}

FORCEINLINE FAccelByteTask::~FAccelByteTask()
{
	if (!bIsFinished)
	{
		Finish();
	}
}
}