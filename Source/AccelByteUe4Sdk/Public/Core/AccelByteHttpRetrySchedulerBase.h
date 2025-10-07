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
class ACCELBYTEUE4SDK_API FHttpRetrySchedulerBase : public TSharedFromThis<FHttpRetrySchedulerBase, ESPMode::ThreadSafe> 
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

	virtual ~FHttpRetrySchedulerBase();

	virtual void InitializeRateLimit() = 0;

	FAccelByteTaskPtr virtual ProcessRequest(FHttpRequestPtr Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime) = 0;

	virtual FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr Request, TSharedPtr<FJsonObject> Content, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, bool bOmitBlankValues = false) = 0;

	virtual void SetBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected) = 0;
	virtual FDelegateHandle AddBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected) = 0;
	virtual bool RemoveBearerAuthRejectedDelegate(FDelegateHandle const& BearerAuthRejectedHandle) = 0;
	virtual void BearerAuthRejected() = 0;
	virtual void PauseBearerAuthRequest() = 0;
	virtual void ResumeBearerAuthRequest(const FString& AccessToken) = 0;

	virtual FDelegateHandle AddBearerAuthRefreshedDelegate(FBearerAuthRefreshed const& BearerAuthRefreshed) = 0;
	virtual bool RemoveBearerAuthRefreshedDelegate(FDelegateHandle const& BearerAuthRefreshedHandle) = 0;

	virtual void Startup() = 0;
	virtual void Shutdown() = 0;
	virtual bool PollRetry(double Time) = 0;

	static void SetHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode, const FHttpResponseCodeHandler& Handler);
	static bool RemoveHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode);

	static void SetHeaderNamespace(const FString& Value) { HeaderNamespace = Value; }
	static void SetHeaderSDKVersion(const FString& Value) { HeaderSDKVersion = Value; }
	static void SetHeaderOSSVersion(const FString& Value) { HeaderOSSVersion = Value; }
	static void SetHeaderGameClientVersion(const FString& Value) { HeaderGameClientVersion = Value; }

	virtual Core::FAccelByteHttpCache &GetHttpCache() = 0;

protected:
	static TMap<EHttpResponseCodes::Type, FHttpResponseCodeHandler> ResponseCodeDelegates;

	//Custom Metadata Header
	static FString HeaderNamespace;
	static FString HeaderSDKVersion;
	static FString HeaderOSSVersion;
	static FString HeaderGameClientVersion;
	static int32 RateLimit;
};

using FHttpRetrySchedulerRef = TSharedRef<FHttpRetrySchedulerBase, ESPMode::ThreadSafe>;
using FHttpRetrySchedulerPtr = TSharedPtr<FHttpRetrySchedulerBase, ESPMode::ThreadSafe>;
using FHttpRetrySchedulerWPtr = TWeakPtr<FHttpRetrySchedulerBase, ESPMode::ThreadSafe>;

}