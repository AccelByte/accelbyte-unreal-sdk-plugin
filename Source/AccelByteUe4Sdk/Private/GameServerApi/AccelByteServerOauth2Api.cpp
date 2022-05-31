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

void ServerOauth2::GetAccessTokenWithClientCredentialsGrant(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url             = FString::Printf(TEXT("%s/v3/oauth/token"), *SettingsRef.IamServerUrl);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("grant_type=client_credentials"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerOauth2::LoginWithClientCredentials(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	GetAccessTokenWithClientCredentialsGrant(
		CredentialsRef.GetOAuthClientId(),
		CredentialsRef.GetOAuthClientSecret(),
		THandler<FOauth2Token>::CreateLambda([this, OnSuccess](const FOauth2Token& Result)
		{
			OnLoginSuccess(OnSuccess, Result);
		}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
}

void ServerOauth2::GetJwks(THandler<FJwkSet> const& OnSuccess, FErrorHandler const& OnError) const
{
	FReport::Log(FString(__FUNCTION__));

	FString Url             = FString::Printf(TEXT("%s/v3/oauth/jwks"), *SettingsRef.IamServerUrl);
	FString Verb            = TEXT("GET");
	FString Accept          = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerOauth2::ForgetAllCredentials()
{
	FReport::Log(FString(__FUNCTION__));

	CredentialsRef.ForgetAll();
}

void ServerOauth2::OnLoginSuccess(const FVoidHandler& OnSuccess, const FOauth2Token& Response) const
{
	CredentialsRef.SetClientToken(Response.Access_token, Response.Expires_in, Response.Namespace);
	OnSuccess.ExecuteIfBound();
}

ServerOauth2::ServerOauth2(ServerCredentials& InCredentialsRef
	, ServerSettings& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
	, HttpRef{InHttpRef}
{}

ServerOauth2::~ServerOauth2()
{}

} // Namespace Api
} // Namespace AccelByte
