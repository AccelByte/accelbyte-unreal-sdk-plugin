// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteIdValidator.h"

namespace AccelByte
{

// forward declaration
class FApiClient;
	
class ACCELBYTEUE4SDK_API FApiBase
{
public:
	FApiBase(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient);

	void SetApiClient(TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient);
	
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
	FHttpRetryScheduler& HttpRef;
	FHttpClient HttpClient;
	TWeakPtr<FApiClient, ESPMode::ThreadSafe> ApiClient;
};

}
