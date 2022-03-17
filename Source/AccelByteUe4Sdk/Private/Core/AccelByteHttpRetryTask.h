// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteTask.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
	typedef FHttpRetryScheduler::FBearerAuthRejectedRefresh FBearerAuthRejectedRefresh;

	class FHttpRetryTask : public FAccelByteTask
	{
	public:
		FHttpRetryTask(
			FHttpRequestPtr& HttpRequest,
			const FHttpRequestCompleteDelegate& CompleteDelegate,
			double RequestTime,
			double NextDelay,
			const FVoidHandler& OnBearerAuthRejectDelegate,
			FBearerAuthRejectedRefresh& BearerAuthRejectedRefresh,
			TMap<EHttpResponseCodes::Type, FHttpRetryScheduler::FHttpResponseCodeHandler> HandlerDelegates = {});
		virtual ~FHttpRetryTask() override;

		virtual bool Start() override;
		virtual bool Cancel() override;
		virtual void Tick(double CurrentTime) override;
		virtual bool Finish() override;
		bool FinishFromCached(const FHttpResponsePtr& Response);

		EAccelByteTaskState Pause();

		FHttpRequestPtr GetHttpRequest() const { return Request; };

	private:
		FHttpRequestPtr Request{};
		const FHttpRequestCompleteDelegate CompleteDelegate{};
		const double RequestTime{};
		double PauseTime{};
		double PauseDuration{};
		double NextRetryTime{};
		double NextDelay{};
		const FVoidHandler OnBearerAuthRejectDelegate{};
		FBearerAuthRejectedRefresh& BearerAuthRejectedRefresh;
		FDelegateHandle BearerAuthRejectedRefreshHandle{};
		bool bIsBeenRunFromPause{};
		TMap<int32, FHttpRetryScheduler::FHttpResponseCodeHandler> ResponseCodeDelegates{};

		void InitializeDefaultDelegates();
		void BearerAuthUpdated(const FString& AccessToken);
		EAccelByteTaskState HandleDefaultRetry(int32 StatusCode);
		bool CheckRetry(EAccelByteTaskState& Out);
		EAccelByteTaskState Retry();
		EAccelByteTaskState ScheduleNextRetry();
		bool IsFinished();
		bool IsRefreshable();
		bool IsTimedOut();
	};

	typedef TSharedPtr<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskPtr;
	typedef TSharedRef<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskRef;

}