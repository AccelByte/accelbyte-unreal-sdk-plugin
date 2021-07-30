// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

namespace AccelByte 
{

enum class EAccelByteTaskState
{
	/** Task has completed execution. */
	Completed,

	/** Task execution is still pending. */
	Pending,

	/** Task is executing. */
	Running,

	/** Task execution failed. */
	Failed,

	/** Task has been canceled. */
	Cancelled,
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
	virtual bool Cancel() { return true; }

	/**
	 * @brief Task Execution ticks.
	 *
	 * @param Time Timer for the ticks
	 */
	virtual void Tick(double Time) { TaskTime += Time; }

	/**
	 * @brief Finish the Task.
	 */
	virtual bool Finish() { return bIsFinished = true; }

	/**
	 * @brief Current State of the Task.
	 */
	virtual EAccelByteTaskState State() const { return TaskState; }

	/**
	 * @brief Get current Task Time.
	 */
	virtual double Time() const { return TaskTime; }
protected:
	double TaskTime;
	EAccelByteTaskState TaskState;
	bool bIsFinished;
};

FORCEINLINE FAccelByteTask::FAccelByteTask()
	: TaskTime(0.0f)
	, TaskState(EAccelByteTaskState::Pending)
	, bIsFinished(false)
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