// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteMiscellaneousApi.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Miscellaneous::Miscellaneous(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase & InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
	, TimeManager(InHttpRef, InSettingsRef.BasicServerUrl)
{}

Miscellaneous::~Miscellaneous()
{}

FAccelByteTaskWPtr Miscellaneous::GetServerCurrentTime(THandler<FTime> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	return TimeManager.GetServerTime(OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
