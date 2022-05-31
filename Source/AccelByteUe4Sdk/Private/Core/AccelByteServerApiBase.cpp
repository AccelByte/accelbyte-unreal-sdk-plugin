// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
	
FServerApiBase::FServerApiBase( ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: ServerCredentialsRef{InCredentialsRef}
	, ServerSettingsRef{InSettingsRef}
	, HttpRef{InHttpRef}
{
}

}
