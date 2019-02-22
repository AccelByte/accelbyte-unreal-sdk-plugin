// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include <chrono>
#include <ctime>

#include "HttpRetrySystem.h"
#include "Runtime/Core/Public/Containers/Ticker.h"

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOrderApi.h"
#include "AccelByteUserApi.h"
#include "AccelByteUserProfileModels.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "FileManager.h"
#include "AccelByteUserApi.h"
#include "AccelByteUserProfileApi.h"
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

class FHttpRetryScheduler
{
public:
	static const int InitialDelay = 1;
	static const int MaximumDelay = 30;
	static const int TotalTimeout = 60;

	bool ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);
	bool PollRetry(double CurrentTime, Credentials& UserCredentials);

private:
	class FHttpRetryTask
	{
	public:
		const FHttpRequestPtr Request;
		const FHttpRequestCompleteDelegate CompleteDelegate;
		const double RequestTime;
		float NextDelay;
		double NextRetryTime;

		FHttpRetryTask(const FHttpRequestPtr& HttpRequest, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime, double NextDelay);
		void ScheduleNextRetry(double CurrentTime);
	};

private:
	TArray<TSharedRef<FHttpRetryTask>> RetryList;
};

}