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
#include "Core/AccelByteError.h"
#include "Core/AccelByteTask.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FHttpRetryScheduler
{

public:

	DECLARE_DELEGATE(FBearerAuthRejected);
	DECLARE_MULTICAST_DELEGATE_OneParam(FBearerAuthRejectedRefresh, const FString&);

	static const int InitialDelay;
	static const int MaximumDelay;
	static const int TotalTimeout;

	FHttpRetryScheduler();
	~FHttpRetryScheduler();

	FAccelByteTaskPtr ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);

	void SetBearerAuthRejectedDelegate(FBearerAuthRejected BearerAuthRejected);
	void BearerAuthRejected();
	void PauseBearerAuthRequest();
	void ResumeBearerAuthRequest(const FString& AccessToken);

	void Startup();
	void Shutdown();
	bool PollRetry(double Time);

protected:
	TQueue<FAccelByteTaskPtr, EQueueMode::Mpsc> TaskQueue;
	FDelegateHandle PollRetryHandle;

	FBearerAuthRejected BearerAuthRejectedDelegate;
	FBearerAuthRejectedRefresh BearerAuthRejectedRefresh;

	enum class EState
	{
		Uninitialized,
		Initialized,
		Paused,
		ShuttingDown
	};

	EState State{EState::Uninitialized};
};

typedef TSharedRef<FHttpRetryScheduler, ESPMode::ThreadSafe> FHttpRetrySchedulerRef;	
typedef TSharedPtr<FHttpRetryScheduler, ESPMode::ThreadSafe> FHttpRetrySchedulerPtr;

}