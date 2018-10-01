// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Http.h"
#include "HttpRetrySystem.h"
#include "Ticker.h"

namespace AccelByte
{

class FRetrySystemTicker : public FTickerObjectBase
{
public:
	// Samuel's note: transient faults
	FHttpRetrySystem::FRetryLimitCountSetting RetryLimitCount{ 5 };
	FHttpRetrySystem::FRetryTimeoutRelativeSecondsSetting RetryTimeoutRelativeSeconds{ 10.0 };
	FHttpRetrySystem::FRetryResponseCodes RetryResponseCodes
	{ // Samuel's note: transient faults
		EHttpResponseCodes::ServerError,
		EHttpResponseCodes::BadGateway,
		EHttpResponseCodes::ServiceUnavail,
		EHttpResponseCodes::GatewayTimeout
	};
	FHttpRetrySystem::FManager Manager{ RetryLimitCount, RetryTimeoutRelativeSeconds };

	bool Tick(float DeltaTime) override;
};

static FRetrySystemTicker RetrySystem;

}
