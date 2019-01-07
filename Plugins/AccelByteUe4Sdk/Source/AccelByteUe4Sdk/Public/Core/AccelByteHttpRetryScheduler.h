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
#include "AccelByteUserManagementApi.h"
#include "AccelByteUserProfileModels.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "FileManager.h"
#include "AccelByteUserAuthenticationApi.h"
#include "AccelByteUserProfileApi.h"
#include <vector>

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::Settings;
using AccelByte::HandleHttpError;
using AccelByte::Api::UserAuthentication;
using AccelByte::Api::UserManagement;
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
	bool ProcessRequest(const FHttpRequestPtr& Request, const FHttpRequestCompleteDelegate& CompleteDelegate, double RequestTime);
	bool PollRetry(double CurrentTime, Credentials& UserCredentials);

	double GetInitialDelay() const { return InitialDelay; }
	double GetMaximumDelay() const { return MaximumDelay; }
	double GetTotalTimeout() const { return TotalTimeout; }

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
	};

private:
	void ScheduleNextRetry(TSharedPtr<FHttpRetryTask> Task, double CurrentTime);

private:
	const double InitialDelay = 1.0;
	const double MaximumDelay = 30.0;
	const double TotalTimeout = 60.0;

	vector<TSharedRef<FHttpRetryTask>> RetryList;
};

}