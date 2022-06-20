// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>

#include "Delegates/DelegateCombinations.h"
#include "HttpManager.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteTask.h"
#include "Core/AccelByteHttpCache.h"
#include "Core/AccelByteDefines.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpRetry, Log, All);

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FHttpRetryScheduler
{
public:
	DECLARE_DELEGATE(FBearerAuthRejected);
	DECLARE_MULTICAST_DELEGATE_OneParam(FBearerAuthRejectedRefresh, FString const&);

	/**
	 * @brief A delegate to handle specified HTTP status code when the HTTP request succeeded
	 *			and then will return a task state as the next command for the scheduler to do
	 *			various things on the task that track the HTTP request
	 *			- Completed, Cancelled and Failed state will finish the task
	 *			- Running, Pending and Retrying state will retry the task
	 *			- Paused state will pause the task
	 *
	 * @param StatusCode The HTTP status code that will be handled
	 * @return EAccelByteTaskState The next state
	 */
	DECLARE_DELEGATE_RetVal_OneParam(EAccelByteTaskState, FHttpResponseCodeHandler, int32 /* StatusCode */);
	
	static int InitialDelay;
	static int MaximumDelay;
	static int TotalTimeout;
	static int PauseTimeout;

	FHttpRetryScheduler();
	virtual ~FHttpRetryScheduler();

	FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);

	void SetBearerAuthRejectedDelegate(FBearerAuthRejected BearerAuthRejected);
	void BearerAuthRejected();
	void PauseBearerAuthRequest();
	void ResumeBearerAuthRequest(const FString& AccessToken);

	virtual void Startup();
	virtual void Shutdown();
	bool PollRetry(double Time);

	static void SetHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode, const FHttpResponseCodeHandler& Handler);
	static bool RemoveHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode);
protected:
	static TMap<EHttpResponseCodes::Type, FHttpResponseCodeHandler> ResponseCodeDelegates;

	TQueue<FAccelByteTaskPtr, EQueueMode::Mpsc> TaskQueue{};
	FDelegateHandleAlias PollRetryHandle{};

	Core::FAccelByteHttpCache HttpCache{};

	FBearerAuthRejected BearerAuthRejectedDelegate{};
	FBearerAuthRejectedRefresh BearerAuthRejectedRefresh{};


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