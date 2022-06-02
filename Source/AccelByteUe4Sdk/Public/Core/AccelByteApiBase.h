// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpClient.h"
#include "Core/AccelByteHttpRetryScheduler.h"

using namespace AccelByte;

namespace AccelByte
{
	
class ACCELBYTEUE4SDK_API FApiBase
{
public:
	FApiBase(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef);
	
protected:
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
	FHttpRetryScheduler& HttpRef;
	FHttpClient HttpClient;
};

}
