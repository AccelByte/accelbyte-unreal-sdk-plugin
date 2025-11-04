// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteIdValidator.h"
#include "Core/AGS/AccelBytePlatform.h"

class FAccelByteInstance;

namespace AccelByte
{

// forward declaration
class FApiClient;
	
class ACCELBYTEUE4SDK_API FApiBase
{
public:
	FApiBase(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient);

	FApiBase(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	void SetApiClient(TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient);
	
protected:
	template<typename T>
	bool ValidateAccelByteId(FString const& Id, EAccelByteIdHypensRule HypenRule, FString const& ErrorMessage, T const& OnError)
	{
		if (!FAccelByteIdValidator::IsAccelByteIdValid(Id, HypenRule))
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), ErrorMessage);
			return false;
		}
		return true;
	}

	TSharedRef<Credentials const, ESPMode::ThreadSafe> CredentialsRef;
	Settings const& SettingsRef;
	FHttpRetrySchedulerBase& HttpRef;
	FHttpClient HttpClient;
	FAccelBytePlatformPtr AccelBytePlatformPtr;
};

}
