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
	void SetUserToken(const FString& AccessToken, const FString& RefreshToken, const FDateTime& ExpirationUtc, const FString& Id, const FString& DisplayName, const FString& Namespace);
	void SetClientToken(const FString& AccessToken, const FDateTime& ExpirationUtc, const FString& Namespace);
	Credentials operator=(const Credentials& Other);
	FString GetUserAccessToken() const;
	FString GetUserRefreshToken() const;
	FDateTime GetUserAccessTokenExpirationUtc() const;
	FString GetUserId() const;
	FString GetUserDisplayName() const;
	FString GetUserNamespace() const;
	FString GetClientAccessToken() const;
	FDateTime GetClientAccessTokenExpirationUtc() const;
	FString GetClientNamespace() const;
private:
	FString UserAccessToken;
	FString UserRefreshToken;
	FDateTime UserAccessTokenExpirationUtc;
	FString UserNamespace;
	FString UserId;
	FString UserDisplayName;
	FString ClientAccessToken;
	FDateTime ClientAccessTokenExpirationUtc;
	FString ClientNamespace;
};

extern Credentials UserCredentials;

} // Namespace AccelByte
