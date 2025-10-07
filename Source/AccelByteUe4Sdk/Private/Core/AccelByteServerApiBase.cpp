// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
	
FServerApiBase::FServerApiBase( ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: ServerCredentialsRef{InCredentialsRef.AsShared()}
	, ServerSettingsRef{InSettingsRef}
	, HttpRef{InHttpRef}
	, HttpClient(InCredentialsRef, InSettingsRef, InHttpRef)
	, ServerApiClient(InServerApiClient)
{
}

void FServerApiBase::SetServerApiClient(TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
{
	ServerApiClient = InServerApiClient;
}
}
