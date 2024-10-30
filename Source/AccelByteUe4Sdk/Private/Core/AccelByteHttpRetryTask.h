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
	typedef FHttpRetryScheduler::FBearerAuthRejectedRefresh FBearerAuthRejectedRefresh;

	class FHttpRetryTask : public FAccelByteTask
	{
	public:
		FHttpRetryTask(
			FHttpRequestPtr& InRequest,
			const FHttpRequestCompleteDelegate& InCompleteDelegate,
			double InRequestTime,
			double InNextDelay,
			const FVoidHandler& InOnBearerAuthRejectDelegate,
			FBearerAuthRejectedRefresh& InBearerAuthRejectedRefresh,
			TMap<EHttpResponseCodes::Type, FHttpRetryScheduler::FHttpResponseCodeHandler> HandlerDelegates = {});
		virtual ~FHttpRetryTask() override;

		virtual bool Start() override;
		virtual bool Cancel() override;
		virtual void Tick(double CurrentTime) override;
		virtual bool Finish() override;
		bool FinishFromCached(const FHttpResponsePtr& Response);

		virtual EAccelByteTaskState Pause() override;

		FHttpRequestPtr GetHttpRequest() const { return Request; };

		void SetResponseTime(FDateTime InResponseTime) { ResponseTime = InResponseTime; };
		FDateTime GetResponseTime() const { return ResponseTime; };

		virtual TMap<FString, FString> GetResponseHeader();

		bool IsResponseFromCache() { return bIsResponseFromCache; };

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
		FDateTime ResponseTime{0};
		bool bIsResponseFromCache{false};

		void InitializeDefaultDelegates();
		void BearerAuthUpdated(const FString& AccessToken);
		EAccelByteTaskState HandleDefaultRetry(int32 StatusCode);
		bool CheckRetry(EAccelByteTaskState& Out);
		EAccelByteTaskState Retry();
		EAccelByteTaskState ScheduleNextRetry();
		bool IsFinished();
		bool IsRefreshable();
		bool IsTimedOut();

		void OnProcessRequestComplete(FHttpRequestPtr InRequest, FHttpResponsePtr InResponse, bool bConnectedSuccessfully);
	};

	typedef TSharedPtr<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskPtr;
	typedef TSharedRef<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskRef;

}