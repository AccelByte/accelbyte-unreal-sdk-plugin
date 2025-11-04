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
	class FHttpRetryTask 
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

		bool FinishFromCached(FHttpResponsePtr const& Response);

		FHttpRequestPtr GetHttpRequest() const { return Request; };

		void SetResponseTime(FDateTime InResponseTime) { ResponseTime = InResponseTime; };

		FDateTime GetResponseTime() const { return ResponseTime; };

		virtual TMap<FString, FString> GetResponseHeader();

		bool IsResponseFromCache() { return bIsResponseFromCache; };

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
		FHttpRequestPtr Request{ nullptr };
		FHttpResponsePtr Response{ nullptr };
		const FHttpRequestCompleteDelegate CompleteDelegate{};
		const double RequestTime{ 0.0f };
		double PauseTime{ 0.0f };
		double PauseDuration{ 0.0f };
		double NextRetryTime{ 0.0f };
		double NextDelay{ 0.0f };
		FHttpRetrySchedulerWPtr HttpRetrySchedulerWPtr{ nullptr };
		FDelegateHandle BearerAuthRejectedHandle{};
		FDelegateHandle BearerAuthRefreshedHandle{};
		bool bIsBeenRunFromPause{ false };
		TMap<int32, FHttpRetryScheduler::FHttpResponseCodeHandler> ResponseCodeDelegates{};
		FDateTime ResponseTime{ 0 };
		bool bIsResponseFromCache{ false };
	};

	typedef TSharedPtr<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskPtr;
	typedef TSharedRef<FHttpRetryTask, ESPMode::ThreadSafe> FAccelByteHttpRetryTaskRef;
} // namespace AccelByte