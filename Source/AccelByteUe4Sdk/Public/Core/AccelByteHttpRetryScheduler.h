// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>

#include "AccelByteSettings.h"
#include "AccelByteHttpRetrySchedulerBase.h"
#include "Delegates/DelegateCombinations.h"
#include "HttpManager.h"
#include "Core/AccelByteTask.h"
#include "Core/AccelByteHttpCache.h"
#include "Core/AccelByteDefines.h"
#include "Dom/JsonObject.h"

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FHttpRetryScheduler : public FHttpRetrySchedulerBase
{
public:

	FHttpRetryScheduler();
	explicit FHttpRetryScheduler(const SettingsWPtr InSettingsWeak);
	virtual ~FHttpRetryScheduler() override;

	virtual void InitializeRateLimit() override;

	virtual FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime) override;

	virtual FAccelByteTaskPtr ProcessRequest(FHttpRequestPtr Request, TSharedPtr<FJsonObject> Content, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, bool bOmitBlankValues = false) override;

	virtual void SetBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected) override;
	virtual FDelegateHandle AddBearerAuthRejectedDelegate(FBearerAuthRejected const& BearerAuthRejected) override;
	virtual bool RemoveBearerAuthRejectedDelegate(FDelegateHandle const& BearerAuthRejectedHandle) override;
	virtual void BearerAuthRejected() override;
	virtual void PauseBearerAuthRequest() override;
	virtual void ResumeBearerAuthRequest(const FString& AccessToken) override;

	virtual FDelegateHandle AddBearerAuthRefreshedDelegate(FBearerAuthRefreshed const& BearerAuthRefreshed) override;
	virtual bool RemoveBearerAuthRefreshedDelegate(FDelegateHandle const& BearerAuthRefreshedHandle) override;

	virtual void Startup() override;
	virtual void Shutdown() override;
	virtual bool PollRetry(double Time) override;

	virtual Core::FAccelByteHttpCache &GetHttpCache() override { return HttpCache; } 

protected:

	FString ParseUStructToJsonString(const TSharedPtr<FJsonObject>& JsonObject, bool bOmitBlankValues = false);

	bool Tick(float DeltaTime);

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
};
}