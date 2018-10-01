// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsIdentity.h"

namespace AccelByte
{

/**
 * @brief Class for storing user's credentials.
 */
class Credentials
{
public:
	Credentials(); 
	~Credentials();
	FAccelByteModelsOAuthToken GetClientAccessToken();
	void SetClientAccessToken(const FAccelByteModelsOAuthToken& New);
	FAccelByteModelsOAuthToken GetUserAccessToken();
	void SetUserAccessToken(const FAccelByteModelsOAuthToken& New);
private:
	FAccelByteModelsOAuthToken ClientAccessToken;
	FAccelByteModelsOAuthToken UserAccessToken;
#if 0
	FString UserAccessToken;
	FString UserRefreshToken;
	FDateTime UserAccessTokenExpiration;
	FString ClientAccesstoken;
	FString ClientRefeshtoken;
	FDateTime ClientAccessTokenExpiration;
	FString UserId;
#endif
};

static Credentials UserCredentials;

} // Namespace AccelByte
