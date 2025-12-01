// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"

// STL
#include <atomic>

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
	FORCENOINLINE bool IsCancelRequested() { return State.load(std::memory_order_relaxed) == EState::CancelRequested; }

	/**
	 * @brief Return true if the state is Cancelled.
	 */
	FORCENOINLINE bool IsCancelled() { return State.load(std::memory_order_relaxed) == EState::Cancelled; }

	/**
	 * @brief Return true if the state is Done.
	 */
	FORCENOINLINE bool IsTaskDone() { return State.load(std::memory_order_relaxed) == EState::Done; }

	/**
	 * @brief Request cancellation.
	 */
	FORCENOINLINE void Cancel()
	{
		switch (State.load(std::memory_order_relaxed))
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
		switch (State.load(std::memory_order_relaxed))
		{
		case EState::Running: break;
		case EState::CancelRequested: State.store(EState::Cancelled, std::memory_order_relaxed);
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
		switch (State.load(std::memory_order_relaxed))
		{
		case EState::Running: State.store(EState::Done, std::memory_order_relaxed);
		case EState::CancelRequested:
		case EState::Cancelled:
		case EState::Done:
		default: break;
		}
	}

private:
	enum class EState : int32
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

	std::atomic<EState> State;
};

using FAccelByteCancellationTokenPtr = TSharedPtr<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe>;
using FAccelByteCancellationTokenRef = TSharedRef<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe>;
using FAccelByteCancellationTokenWeakPtr = TWeakPtr<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe>;

class ACCELBYTEUE4SDK_API FAccelByteCancellationToken
{
private:
	FAccelByteCancellationTokenWeakPtr Token;

public:
	FAccelByteCancellationToken(const FAccelByteCancellationTokenWeakPtr& Token)
		: Token{ Token }
	{}

	~FAccelByteCancellationToken() = default;

	/**
	 * @brief Request cancellation
	 */
	FORCENOINLINE void Cancel()
	{ 
		auto PinnedToken = Token.Pin();
		if(PinnedToken.IsValid())
		{
			PinnedToken->Cancel();
		}
		else
		{
			// Do nothing
		}
	}

	/**
	 * @brief Will return true when task is on cancellation or already cancelled.
	 */
	FORCENOINLINE bool IsCancelled()
	{
		auto PinnedToken = Token.Pin();
		if(PinnedToken.IsValid())
		{
			return PinnedToken->IsCancelled() || PinnedToken->IsCancelRequested();
		}
		else
		{
			return false;
		}
	}
};

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
	 * @brief Pause the Task in the middle of the process.
	 */
	virtual EAccelByteTaskState Pause() { return EAccelByteTaskState::Paused; }

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
		double TaskTimeBuf = TaskTime.load(std::memory_order_acquire);
		TaskTimeBuf += Time;
		TaskTime.store(TaskTimeBuf, std::memory_order_release);
	}

	/**
	 * @brief Finish the Task.
	 */
	virtual bool Finish() 
	{
		Token->MarkDone();
		bIsFinished.store(true, std::memory_order_release);
		return true; 
	}

	/**
	 * @brief Current State of the Task.
	 */
	virtual EAccelByteTaskState State() const { return TaskState; }

	/**
	 * @brief Get current Task Time.
	 */
	virtual double Time() const { return TaskTime.load(std::memory_order_acquire); }

	/**
	 * @brief Create Cancellation Token handler.
	 */
	FAccelByteCancellationToken GetCancellationToken()
	{
		return FAccelByteCancellationToken(Token);
	}

protected:
	std::atomic<double> TaskTime{0.0};
	std::atomic<bool> bIsFinished{false};
	FAccelByteCancellationTokenRef Token = MakeShared<FAccelByteCancellationTokenSource, ESPMode::ThreadSafe>();

protected:
	void SetTaskState(EAccelByteTaskState NewState) noexcept
	{
		FWriteScopeLock Lock(TaskStateMtx);
		TaskState = NewState;
	}

	EAccelByteTaskState GetTaskState() const noexcept
	{
		FReadScopeLock Lock(TaskStateMtx);
		return TaskState;
	}

private:
	mutable FRWLock TaskStateMtx;
	EAccelByteTaskState TaskState = EAccelByteTaskState::Pending;
};

using FAccelByteTaskPtr = TSharedPtr<FAccelByteTask, ESPMode::ThreadSafe>;
using FAccelByteTaskRef = TSharedRef<FAccelByteTask, ESPMode::ThreadSafe>;
using FAccelByteTaskWPtr = TWeakPtr<FAccelByteTask, ESPMode::ThreadSafe>;

FORCEINLINE FAccelByteTask::FAccelByteTask()
{

}

FORCEINLINE FAccelByteTask::~FAccelByteTask()
{
	if (!bIsFinished.load(std::memory_order_acquire))
	{
		Finish();
	}
}
}