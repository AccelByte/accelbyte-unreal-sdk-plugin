// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteIdValidator.h"

namespace AccelByte
{

// forward declaration
class FServerApiClient;

class ACCELBYTEUE4SDK_API FServerApiBase
{
public:
	FServerApiBase(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRe
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient);

	void SetServerApiClient(TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient);
	
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
	TSharedRef<ServerCredentials const, ESPMode::ThreadSafe> ServerCredentialsRef;
	ServerSettings const& ServerSettingsRef;
	FHttpRetryScheduler& HttpRef;
	FHttpClient HttpClient;
	TWeakPtr<FServerApiClient, ESPMode::ThreadSafe> ServerApiClient;
};

}
