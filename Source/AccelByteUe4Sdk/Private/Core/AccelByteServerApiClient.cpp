// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerApiClient.h"

namespace AccelByte
{
	FServerApiClient::FServerApiClient()
		: bUseSharedCredentials(false)
		, ServerCredentialsRef(MakeShared<AccelByte::ServerCredentials, ESPMode::ThreadSafe>())
		, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
	{
		HttpRef->Startup();
		ServerCredentialsRef->Startup();
	}

	FServerApiClient::FServerApiClient(AccelByte::ServerCredentials& Credentials, AccelByte::FHttpRetryScheduler& Http)
		: bUseSharedCredentials(true)
		, ServerCredentialsRef(MakeShareable<AccelByte::ServerCredentials>(&Credentials, [](AccelByte::ServerCredentials*) {}))
		, HttpRef(MakeShareable<AccelByte::FHttpRetryScheduler>(&Http, [](AccelByte::FHttpRetryScheduler*) {}))
	{

	}

	FServerApiClient::~FServerApiClient()
	{
		if (!bUseSharedCredentials)
		{
			ServerCredentialsRef->Shutdown();
			HttpRef->Shutdown();
		}
	}
}