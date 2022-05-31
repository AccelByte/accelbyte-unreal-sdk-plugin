// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"

using namespace AccelByte;

namespace AccelByte
{
	
class ACCELBYTEUE4SDK_API FServerApiBase
{
public:
	FServerApiBase(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRe
		, FHttpRetryScheduler& InHttpRef);
	
protected:
	ServerCredentials const& ServerCredentialsRef;
	ServerSettings const& ServerSettingsRef;
	FHttpRetryScheduler& HttpRef;
};

}
