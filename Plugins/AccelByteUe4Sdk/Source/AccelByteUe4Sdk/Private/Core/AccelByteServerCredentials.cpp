// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerCredentials.h"
#include "Api/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"

using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerCredentials);


namespace AccelByte
{

ServerCredentials::ServerCredentials()
	: ClientAccessToken(TEXT(""))
	, ClientNamespace(TEXT(""))
	, ClientSessionState(ESessionState::Invalid)
{
}

void ServerCredentials::ForgetAll()
{
	ClientAccessToken = FString();

	ClientSessionState = ESessionState::Invalid;
}

void ServerCredentials::SetClientCredentials(const FString& ClientId_, const FString& ClientSecret_)
{
	ClientId = ClientId_;
	ClientSecret = ClientSecret_;
}

void ServerCredentials::SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace)
{
	ClientAccessToken = AccessToken;
	ClientNamespace = Namespace;

	ClientSessionState = ESessionState::Valid;
}

void ServerCredentials::SetMatchId(const FString& GivenMatchId)
{
	MatchId = GivenMatchId;
}

ServerCredentials::ESessionState ServerCredentials::GetSessionState() const
{
	return ClientSessionState;
}

const FString& ServerCredentials::GetClientAccessToken() const
{
	return ClientAccessToken;
}

const FString& ServerCredentials::GetClientNamespace() const
{
	return ClientNamespace;
}

const FString& ServerCredentials::GetMatchId() const
{
	return MatchId;
}

} // Namespace AccelByte

#include "Core/AccelByteRegistry.h"

FString UAccelByteBlueprintsServerCredentials::GetClientAccessToken()
{
	return FRegistry::ServerCredentials.GetClientAccessToken();
}

FString UAccelByteBlueprintsServerCredentials::GetClientNamespace()
{
	return FRegistry::ServerCredentials.GetClientNamespace();
}

FString UAccelByteBlueprintsServerCredentials::GetMatchId()
{
	return FRegistry::ServerCredentials.GetMatchId();
}