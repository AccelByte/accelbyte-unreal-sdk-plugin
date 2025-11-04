// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"

namespace AccelByte
{
	
FApiBase::FApiBase(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: CredentialsRef{InCredentialsRef.AsShared()}
	, SettingsRef{InSettingsRef}
	, HttpRef{InHttpRef}
	, HttpClient(InCredentialsRef, InSettingsRef, InHttpRef)
{
	SetApiClient(InApiClient);
}

FApiBase::FApiBase(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase & InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: CredentialsRef{ InCredentialsRef.AsShared() }
	, SettingsRef{ InSettingsRef }
	, HttpRef{ InHttpRef }
	, HttpClient(InCredentialsRef, InSettingsRef, InHttpRef)
	, AccelBytePlatformPtr(InAccelBytePlatform)
{
}

void FApiBase::SetApiClient(TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
{
	if (InApiClient.IsValid())
	{
		auto AccelByteInstance = InApiClient->GetAccelByteInstance().Pin();
		if (AccelByteInstance.IsValid())
		{
			AccelBytePlatformPtr = AccelByteInstance->GetAccelBytePlatform();
		}
	}
}

}
