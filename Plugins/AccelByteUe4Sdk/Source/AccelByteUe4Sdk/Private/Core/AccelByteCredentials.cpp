// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCredentials.h"
#include "AccelByteOauth2Models.h"

namespace AccelByte
{

Credentials::Credentials()
{
}

Credentials::~Credentials()
{
}

Credentials& Credentials::Get()
{
    // Deferred/lazy initialization
    // Thread-safe in C++11
    static Credentials Instance;
    return Instance;
}

void Credentials::ForgetAll()
{
	UserAccessToken = FString();
	UserRefreshToken = FString();
	UserAccessTokenExpirationUtc = FDateTime();
	UserNamespace = FString();
	UserId = FString();
	UserDisplayName = FString();
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

void Credentials::SetClientToken(const FString& AccessToken, const FDateTime& ExpirationUtc, const FString& Namespace)
{
	UserAccessToken = AccessToken;
	UserAccessTokenExpirationUtc = ExpirationUtc;
	UserNamespace = Namespace;
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

FString Credentials::GetUserId() const
{
	return UserId;
}

FString Credentials::GetUserDisplayName() const
{
	return UserDisplayName;
}

} // Namespace AccelByte

using AccelByte::Credentials;

FString UAccelByteBlueprintsCredentials::GetUserAccessToken()
{
	return Credentials::Get().GetUserAccessToken();
}

FString UAccelByteBlueprintsCredentials::GetUserRefreshToken()
{
	return Credentials::Get().GetUserRefreshToken();
}

FDateTime UAccelByteBlueprintsCredentials::GetUserAccessTokenExpirationUtc()
{
	return Credentials::Get().GetUserAccessTokenExpirationUtc();
}

FString UAccelByteBlueprintsCredentials::GetUserId()
{
	return Credentials::Get().GetUserId();
}

FString UAccelByteBlueprintsCredentials::GetUserDisplayName()
{
	return Credentials::Get().GetUserDisplayName();
}

FString UAccelByteBlueprintsCredentials::GetUserNamespace()
{
	return Credentials::Get().GetUserNamespace();
}
