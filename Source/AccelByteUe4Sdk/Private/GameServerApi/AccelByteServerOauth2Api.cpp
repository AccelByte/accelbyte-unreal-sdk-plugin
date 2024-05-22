// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerCredentials.h"
#include "JsonUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace GameServerApi
{

FAccelByteTaskWPtr ServerOauth2::GetAccessTokenWithClientCredentialsGrant(FString const& ClientId
	, FString const& ClientSecret
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	const FString Url       = FString::Printf(TEXT("%s/v3/oauth/token"), *ServerSettingsRef.IamServerUrl);
	FString Content         = FString::Printf(TEXT("grant_type=client_credentials"));

	const TMap<FString, FString> Headers = {
		{TEXT("Authorization"), Authorization},
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerOauth2::LoginWithClientCredentials(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	return GetAccessTokenWithClientCredentialsGrant(ServerCredentialsRef->GetOAuthClientId()
		, ServerCredentialsRef->GetOAuthClientSecret()
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess](FOauth2Token const& Result)
			{
				OnLoginSuccess(OnSuccess, Result);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

FAccelByteTaskWPtr ServerOauth2::GetJwks(THandler<FJwkSet> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/jwks")
		, *ServerSettingsRef.IamServerUrl);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("GET"), Url, FString(), Headers, OnSuccess, OnError);
}

void ServerOauth2::ForgetAllCredentials()
{
	FReport::Log(FString(__FUNCTION__));

	ServerCredentialsRef->ForgetAll();
}

void ServerOauth2::OnLoginSuccess(const FVoidHandler& OnSuccess
	, const FOauth2Token& Response) const
{
	ServerCredentialsRef->SetClientToken(Response.Access_token, Response.Expires_in, Response.Namespace);
	FHttpRetryScheduler::SetHeaderNamespace(Response.Namespace);
	OnSuccess.ExecuteIfBound();
}

ServerOauth2::ServerOauth2(ServerCredentials& InCredentialsRef
	, ServerSettings& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, ServerCredentialsRef{InCredentialsRef.AsShared()}
{}

ServerOauth2::~ServerOauth2()
{}

} // Namespace GameServerApi
} // Namespace AccelByte
