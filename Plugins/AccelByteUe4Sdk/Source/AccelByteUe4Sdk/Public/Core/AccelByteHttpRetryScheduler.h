// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>
#include <ctime>

#include "HttpRetrySystem.h"
#include "Runtime/Core/Public/Containers/Ticker.h"

#include "Runtime/Core/Public/Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteUserApi.h"
#include "Models/AccelByteUserProfileModels.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "Runtime/Core/Public/HAL/FileManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include <vector>

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::UserProfile;
using namespace std;

namespace AccelByte
{

namespace HttpRequest
{
	bool IsFinished(const FHttpRequestPtr& Request);
}

class ACCELBYTEUE4SDK_API FHttpRetryScheduler
{
public:
	static const int InitialDelay;
	static const int MaximumDelay;
	static const int TotalTimeout;

	bool ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);
	bool PollRetry(double CurrentTime, Credentials& UserCredentials);

	void Startup();
	void Shutdown();

private:
	class FHttpRetryTask
	{
	public:
		const FHttpRequestPtr Request;
		const FHttpRequestCompleteDelegate CompleteDelegate;
		const double RequestTime;
		float NextDelay;
		double NextRetryTime;
		bool ScheduledRetry;

		FHttpRetryTask(const FHttpRequestPtr& HttpRequest, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay);
		void ScheduleNextRetry(double CurrentTime);
	};

private:
	TArray<TSharedRef<FHttpRetryTask>> RetryList;
	FDelegateHandle PollRetryHandle;
};

}