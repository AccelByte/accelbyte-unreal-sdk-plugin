// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiBase.h"

namespace AccelByte
{
	
FApiBase::FApiBase(
	Credentials const& InCredentialsRef,
	Settings const& InSettingsRef,
	FHttpRetrySchedulerBase& InHttpRef,
	TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: CredentialsRef{InCredentialsRef.AsShared()}
	, SettingsRef{InSettingsRef}
	, HttpRef{InHttpRef}
	, HttpClient(InCredentialsRef, InSettingsRef, InHttpRef)
	, ApiClient(InApiClient)
{}

void FApiBase::SetApiClient(TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
{
	ApiClient = InApiClient;
}
}
