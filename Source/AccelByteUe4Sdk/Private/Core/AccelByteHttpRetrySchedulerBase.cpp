// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpRetrySchedulerBase.h"

DEFINE_LOG_CATEGORY(LogAccelByteHttpRetry);

using namespace std;

namespace AccelByte
{

int FHttpRetrySchedulerBase::InitialDelay = 1;
int FHttpRetrySchedulerBase::MaximumDelay = 30;
int FHttpRetrySchedulerBase::TotalTimeout = 10;
int FHttpRetrySchedulerBase::TotalTimeoutIncludingRetries = 60;
int FHttpRetrySchedulerBase::PauseTimeout = 60;
int FHttpRetrySchedulerBase::RateLimit =
    FHttpRetrySchedulerBase::DefaultRateLimit;

FString FHttpRetrySchedulerBase::HeaderNamespace = TEXT("");
FString FHttpRetrySchedulerBase::HeaderSDKVersion = TEXT("");
FString FHttpRetrySchedulerBase::HeaderOSSVersion = TEXT("");
FString FHttpRetrySchedulerBase::HeaderGameClientVersion = TEXT("");

TMap<EHttpResponseCodes::Type, FHttpRetrySchedulerBase::FHttpResponseCodeHandler> FHttpRetrySchedulerBase::ResponseCodeDelegates{};


FHttpRetrySchedulerBase::~FHttpRetrySchedulerBase() {}

void FHttpRetrySchedulerBase::SetHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode, FHttpResponseCodeHandler const& Handler)
{
	FHttpRetrySchedulerBase::ResponseCodeDelegates.Emplace(StatusCode, Handler);
}

bool FHttpRetrySchedulerBase::RemoveHttpResponseCodeHandlerDelegate(EHttpResponseCodes::Type StatusCode)
{
	bool bResult = false;

	if (FHttpRetrySchedulerBase::ResponseCodeDelegates.Contains(StatusCode))
	{
		FHttpRetrySchedulerBase::ResponseCodeDelegates.Remove(StatusCode);
		bResult = true;
	}
	return bResult;
}

}
