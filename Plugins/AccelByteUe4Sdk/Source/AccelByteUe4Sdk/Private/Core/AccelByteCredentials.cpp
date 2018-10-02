// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelbyteCredentials.h"
#include "AccelByteModelsIdentity.h"

namespace AccelByte
{
Credentials::Credentials()
{
}

Credentials::~Credentials()
{
}

FAccelByteModelsOAuthToken Credentials::GetClientAccessToken()
{
	return ClientAccessToken; // the return value is constructed before local variables get destroyed
}

void Credentials::SetClientAccessToken(const FAccelByteModelsOAuthToken& New)
{
	ClientAccessToken = New;
}

FAccelByteModelsOAuthToken Credentials::GetUserAccessToken()
{
	return UserAccessToken;
}

void Credentials::SetUserAccessToken(const FAccelByteModelsOAuthToken& New)
{
	UserAccessToken = New;
}

} // Namespace AccelByte
