// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>
#include <ctime>

#include "Runtime/Core/Public/Containers/Ticker.h"

#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteTask.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FHttpRetryScheduler
{
public:
	static const int InitialDelay;
	static const int MaximumDelay;
	static const int TotalTimeout;

	FAccelByteTaskRef ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);

	void Startup();
	void Shutdown();
	bool PollRetry(double Time);

protected:
	TQueue<FAccelByteTaskPtr, EQueueMode::Mpsc> TaskQueue;
	FDelegateHandle PollRetryHandle;

	enum class EHttpRetrySchedulerState
	{
		UNINITIALIZED,
		INITIALIZED,
		SHUTTING_DOWN
	};
	EHttpRetrySchedulerState State = EHttpRetrySchedulerState::UNINITIALIZED;
};

}