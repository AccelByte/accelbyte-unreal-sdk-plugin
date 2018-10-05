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

void Credentials::SetUserToken(const FString& AccessToken, const FString& RefreshToken, const FDateTime& ExpirationUtc, const FString& Id, const FString& DisplayName, const FString& Namespace)
{
	UserAccessToken = AccessToken;
	UserRefreshToken = RefreshToken;
	UserAccessTokenExpirationUtc = ExpirationUtc;
	UserId = Id;
	UserDisplayName = DisplayName;
	UserNamespace = Namespace;
}

void Credentials::SetClientToken(const FString & AccessToken, const FDateTime & ExpirationUtc, const FString & Namespace)
{
	ClientAccessToken = AccessToken;
	ClientAccessTokenExpirationUtc = ExpirationUtc;
	ClientNamespace = Namespace;
}

Credentials Credentials::operator=(const Credentials& Other)
{
	return *this;
}

FString Credentials::GetUserAccessToken() const
{
	return UserAccessToken;
}

FString Credentials::GetUserRefreshToken() const
{
	return UserRefreshToken;
}

FDateTime Credentials::GetUserAccessTokenExpirationUtc() const
{
	return UserAccessTokenExpirationUtc;
}

FString Credentials::GetUserNamespace() const
{
	return UserNamespace;
}

FString Credentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

FDateTime Credentials::GetClientAccessTokenExpirationUtc() const
{
	return ClientAccessTokenExpirationUtc;
}

FString Credentials::GetClientNamespace() const
{
	return ClientNamespace;
}

FString Credentials::GetUserId() const
{
	return UserId;
}

FString Credentials::GetUserDisplayName() const
{
	return UserDisplayName;
}


} // Namespace AccelByte
