// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
	
FServerApiBase::FServerApiBase(
	ServerCredentials const& ServerCredentialsRef,
	ServerSettings const& ServerSettingsRef,
	FHttpRetryScheduler& HttpRef)
	:
	ServerCredentialsRef{ ServerCredentialsRef },
	ServerSettingsRef{ ServerSettingsRef },
	HttpRef{ HttpRef }
{
}

}
