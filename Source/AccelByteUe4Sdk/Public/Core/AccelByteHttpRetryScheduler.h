// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>

#include "AccelByteSettings.h"
#include "Delegates/DelegateCombinations.h"
#include "HttpManager.h"
#include "Core/AccelByteTask.h"
#include "Core/AccelByteHttpCache.h"
#include "Core/AccelByteDefines.h"
#include "Dom/JsonObject.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteHttpRetry, Log, All);

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FHttpRetryScheduler
	: public TSharedFromThis<FHttpRetryScheduler, ESPMode::ThreadSafe>
{
public:
	DECLARE_DELEGATE(FBearerAuthRejected);
	DECLARE_MULTICAST_DELEGATE(FBearerAuthRejectedMulticast);
	DECLARE_DELEGATE_OneParam(FBearerAuthRefreshed, FString const&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FBearerAuthRefreshedMulticast, FString const&);

	/**
	 * @brief A delegate to handle specified HTTP status code when the HTTP request succeeded
	 *			and then will return a task state as the next command for the scheduler to do
	 *			various things on the task that track the HTTP request
	 *			- Completed, CancelledUAccelByteBlueprintsCredentials and Failed state will finish the task
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
	static int TotalTimeoutIncludingRetries;
	static int PauseTimeout;
	static constexpr uint32 DefaultRateLimit = 6;

	FHttpRetryScheduler();
	explicit FHttpRetryScheduler(const SettingsWPtr InSettingsWeak);
	virtual ~FHttpRetryScheduler();

	void InitializeRateLimit();

	FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);

	FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr Request, TSharedPtr<FJsonObject> Content, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, bool bOmitBlankValues = false);

	void SetBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected);
	FDelegateHandle AddBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected);
	bool RemoveBearerAuthRejectedDelegate(FDelegateHandle const& BearerAuthRejectedHandle);
	void BearerAuthRejected();
	void PauseBearerAuthRequest();
	void ResumeBearerAuthRequest(const FString& AccessToken);

	FDelegateHandle AddBearerAuthRefreshedDelegate(FBearerAuthRefreshed const& BearerAuthRefreshed);
	bool RemoveBearerAuthRefreshedDelegate(FDelegateHandle const& BearerAuthRefreshedHandle);

	virtual void Startup();
	virtual void Shutdown();
	bool PollRetry(double Time);

	static void SetHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode, const FHttpResponseCodeHandler& Handler);
	static bool RemoveHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode);

	static void SetHeaderNamespace(const FString& Value) { HeaderNamespace = Value; }
	static void SetHeaderSDKVersion(const FString& Value) { HeaderSDKVersion = Value; }
	static void SetHeaderOSSVersion(const FString& Value) { HeaderOSSVersion = Value; }
	static void SetHeaderGameClientVersion(const FString& Value) { HeaderGameClientVersion = Value; }

	Core::FAccelByteHttpCache& GetHttpCache() { return HttpCache; }

protected:

	FString ParseUStructToJsonString(const TSharedPtr<FJsonObject>& JsonObject, bool bOmitBlankValues = false);

	bool Tick(float DeltaTime);

	static TMap<EHttpResponseCodes::Type, FHttpResponseCodeHandler> ResponseCodeDelegates;
	TMap<FString /*Endpoint*/, FRequestBucket> RequestsBucket;
	mutable FCriticalSection RequestBucketLock;

	TQueue<FAccelByteTaskPtr, EQueueMode::Mpsc> TaskQueue{};
	FDelegateHandleAlias PollRetryHandle{};

	Core::FAccelByteHttpCache HttpCache{};

	SettingsWPtr SettingsWeak;

	FBearerAuthRejectedMulticast BearerAuthRejectedMulticast{};
	FBearerAuthRefreshedMulticast BearerAuthRefreshedMulticast{};

	//for mutex lock
	FCriticalSection LockBearerAuthRejected;
	FCriticalSection LockBearerAuthRefreshed;

	enum class EState
	{
		Uninitialized,
		Initialized,
		Paused,
		ShuttingDown
	};

	EState State{EState::Uninitialized};

	//Custom Metadata Header
	static FString HeaderNamespace;
	static FString HeaderSDKVersion;
	static FString HeaderOSSVersion;
	static FString HeaderGameClientVersion;
	static int32 RateLimit;
};

typedef TSharedRef<FHttpRetryScheduler, ESPMode::ThreadSafe> FHttpRetrySchedulerRef;
typedef TSharedPtr<FHttpRetryScheduler, ESPMode::ThreadSafe> FHttpRetrySchedulerPtr;
typedef TWeakPtr<FHttpRetryScheduler, ESPMode::ThreadSafe> FHttpRetrySchedulerWPtr;

}