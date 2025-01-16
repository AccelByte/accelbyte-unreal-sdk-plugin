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
		, ServerSettings(MakeShareable(&FRegistry::ServerSettings))
	{
		HttpRef->Startup();
		ServerCredentialsRef->Startup();
	}

	FServerApiClient::FServerApiClient(ServerSettingsPtr InServerSettings)
		: bUseSharedCredentials(false)
		, ServerCredentialsRef(MakeShared<AccelByte::ServerCredentials, ESPMode::ThreadSafe>())
		, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
		, ServerSettings(InServerSettings)
	{
		HttpRef->Startup();
		ServerCredentialsRef->Startup();
	}

	FServerApiClient::FServerApiClient(AccelByte::ServerCredentials& Credentials, AccelByte::FHttpRetryScheduler& Http)
		: bUseSharedCredentials(true)
		, ServerCredentialsRef(Credentials.AsShared())
		, HttpRef(Http.AsShared())
		, ServerSettings(MakeShareable(&FRegistry::ServerSettings))
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