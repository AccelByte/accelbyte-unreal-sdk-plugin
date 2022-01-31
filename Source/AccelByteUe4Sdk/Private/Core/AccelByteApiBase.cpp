// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiBase.h"

namespace AccelByte
{
	
FApiBase::FApiBase(
	Credentials const& CredentialsRef,
	Settings const& SettingsRef,
	FHttpRetryScheduler& HttpRef)
	:
	CredentialsRef{ CredentialsRef },
	SettingsRef{ SettingsRef },
	HttpRef{ HttpRef },
	HttpClient(CredentialsRef, SettingsRef, HttpRef)
{
}

}
