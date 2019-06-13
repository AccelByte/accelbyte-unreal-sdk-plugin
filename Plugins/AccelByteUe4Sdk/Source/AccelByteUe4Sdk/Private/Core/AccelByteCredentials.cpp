// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCredentials.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteOauth2Models.h"

using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCredentials);


namespace AccelByte
{

Credentials::Credentials()
	: ClientAccessToken(TEXT(""))
	, ClientNamespace(TEXT(""))
	, UserSessionId(TEXT(""))
	, UserSessionExpire(0)
	, UserNamespace(TEXT(""))
	, UserId(TEXT(""))
	, UserDisplayName(TEXT(""))
	, UserSessionState(ESessionState::Invalid)
{
}

void Credentials::ForgetAll()
{
	UserSessionId = FString();
	UserSessionExpire = 0;
	UserNamespace = FString();
	UserId = FString();
	UserDisplayName = FString();

	UserSessionState = ESessionState::Invalid;
}

void Credentials::SetClientCredentials(const FString& ClientId, const FString& ClientSecret)
{
	this->ClientId = ClientId;
	this->ClientSecret = ClientSecret;
}

void Credentials::SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace)
{
	ClientAccessToken = AccessToken;
	ClientNamespace = Namespace;
}

void Credentials::SetUserSession(const FString& SessionId, double ExpiredTime)
{
	UserSessionId = SessionId;
	UserSessionExpire = ExpiredTime;

	UserSessionState = ESessionState::Valid;
}

void Credentials::SetUserLogin(const FString& Id, const FString& DisplayName, const FString& Namespace)
{
	UserId = Id;
	UserDisplayName = DisplayName;
	UserNamespace = Namespace;
}

const FString& Credentials::GetUserSessionId() const
{
	return UserSessionId;
}

const FString& Credentials::GetUserNamespace() const
{
	return UserNamespace;
}

Credentials::ESessionState Credentials::GetSessionState() const
{
	return UserSessionState;
}

const FString& Credentials::GetUserId() const
{
	return UserId;
}

const FString& Credentials::GetUserDisplayName() const
{
	return UserDisplayName;
}

const FString& Credentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

const FString& Credentials::GetClientNamespace() const
{
	return ClientNamespace;
}

} // Namespace AccelByte

#include "AccelByteRegistry.h"

FString UAccelByteBlueprintsCredentials::GetUserSessionId()
{
	return FRegistry::Credentials.GetUserSessionId();
}

FString UAccelByteBlueprintsCredentials::GetUserId()
{
	return FRegistry::Credentials.GetUserId();
}

FString UAccelByteBlueprintsCredentials::GetUserDisplayName()
{
	return FRegistry::Credentials.GetUserDisplayName();
}

FString UAccelByteBlueprintsCredentials::GetUserNamespace()
{
	return FRegistry::Credentials.GetUserNamespace();
}
