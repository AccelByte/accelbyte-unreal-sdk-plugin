// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteTask.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteError.h"

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FHttpRetryTask
	: public FAccelByteTask, public TSharedFromThis<FHttpRetryTask, ESPMode::ThreadSafe>
{
public:
	FHttpRetryTask(
		FHttpRequestPtr& InRequest,
		FHttpRequestCompleteDelegate const& InCompleteDelegate,
		double InRequestTime,
		double InNextDelay,
		FHttpRetrySchedulerWPtr const& InHttpRetrySchedulerWPtr,
		TMap<EHttpResponseCodes::Type, FHttpRetryScheduler::FHttpResponseCodeHandler> HandlerDelegates = {});
	virtual ~FHttpRetryTask() override;

	virtual bool Start() override;
	virtual bool Cancel() override;
	virtual void Tick(double CurrentTime) override;
	virtual bool Finish() override;
	virtual EAccelByteTaskState Pause() override;

	virtual TMap<FString, FString> GetResponseHeader();

	bool FinishFromCached(FHttpResponsePtr const& Response);

	FHttpRequestPtr GetHttpRequest() const { return Request; };

	void SetResponseTime(FDateTime InResponseTime);

	FDateTime GetResponseTime() const;

	bool IsResponseFromCache();

private:
	void InitializeDefaultDelegates();
	EAccelByteTaskState TriggerBearerAuthRejected();
	void OnBearerAuthRejected();
	void OnBearerAuthRefreshed(FString const& AccessToken);
	EAccelByteTaskState HandleDefaultRetry(int32 StatusCode);
	bool CheckRetry(EAccelByteTaskState& Out);
	EAccelByteTaskState Retry();
	EAccelByteTaskState ScheduleNextRetry();
	bool IsFinished();
	bool IsRefreshable();
	bool IsTimedOut();
	void OnProcessRequestComplete(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bConnectedSuccessfully);

private:
	// Never changed after class construction, mutexes are not needed for these
	TMap<int32, FHttpRetryScheduler::FHttpResponseCodeHandler> ResponseCodeDelegates{};
	const FHttpRequestCompleteDelegate CompleteDelegate{};
	FHttpRetrySchedulerWPtr HttpRetrySchedulerWPtr{ nullptr };
	FHttpRequestPtr Request{ nullptr };

	FRWLock ResponseMtx;
	FHttpResponsePtr Response{ nullptr };

	FRWLock BearerAuthRejectedHandleMtx;
	FDelegateHandle BearerAuthRejectedHandle{};

	FRWLock BearerAuthRefreshedHandleMtx;
	FDelegateHandle BearerAuthRefreshedHandle{};

	mutable FRWLock ResponseTimeMtx;
	FDateTime ResponseTime{ 0 };

	const double RequestTime{ 0.0f };
	std::atomic<double> PauseTime{ 0.0f };
	std::atomic<double> PauseDuration{ 0.0f };
	std::atomic<double> NextRetryTime{ 0.0f };
	std::atomic<double> NextDelay{ 0.0f };
	std::atomic<bool> bIsResponseFromCache{ false };
	std::atomic<bool> bIsBeenRunFromPause{ false };
};

typedef TSharedPtr<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskPtr;
typedef TSharedRef<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskRef;
} // namespace AccelByte
