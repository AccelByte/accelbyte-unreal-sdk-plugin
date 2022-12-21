// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{
	
void Oauth2::GetTokenWithAuthorizationCode(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithAuthorizationCode(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, AuthorizationCode, RedirectUri, OnSuccess, OnError);
}

void Oauth2::GetTokenWithAuthorizationCode(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("will be deprecated soon, please use GetTokenWithAuthorizationCodeV3 instead!!"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("authorization_code")},
		{TEXT("code"), AuthorizationCode },
		{TEXT("redirect_uri"), RedirectUri },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithPasswordCredentials(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, Username, Password, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("will be deprecated soon, please use GetTokenWithPasswordCredentialsV3 instead!!"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username) },
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password) },
		{TEXT("device_id"),  FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())		 },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	GetTokenWithPasswordCredentials(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, Username, Password, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("will be deprecated soon, please use GetTokenWithPasswordCredentialsV3 instead!!"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username) },
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password) },
		{TEXT("device_id"),  FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())		 },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithClientCredentials(const FString& ClientId
	, const FString& ClientSecret
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithClientCredentials(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithClientCredentials(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=client_credentials")));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithDeviceId(const FString& ClientId
	, const FString& ClientSecret
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithDeviceId(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithDeviceId(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/device/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	Request->SetContentAsString(FString::Printf(TEXT("device_id=%s"), *FGenericPlatformHttp::UrlEncode(*FAccelByteUtilities::GetDeviceId())));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithOtherPlatformToken(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, PlatformId, PlatformToken, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithOtherPlatformToken(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA is enabled then this method should be changed to the method using FCustomErrorHandler"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token"), *IamUrl, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("platform_token=%s"), *FGenericPlatformHttp::UrlEncode(PlatformToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError
	, bool bCreateHeadless)
{
	GetTokenWithOtherPlatformToken(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, PlatformId, PlatformToken, OnSuccess, OnError, bCreateHeadless);
}
	
void Oauth2::GetTokenWithOtherPlatformToken(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError
	, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token"), *IamUrl, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("platform_token"), FGenericPlatformHttp::UrlEncode(PlatformToken)},
		{TEXT("createHeadless"), bCreateHeadless ? TEXT("true") : TEXT("false")},
		{TEXT("macAddress"), FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetMacAddress()) }
	}, TEXT(""));
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithRefreshToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& RefreshToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithRefreshToken(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, RefreshToken, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithRefreshToken(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& RefreshToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("refresh_token")},
		{TEXT("refresh_token"), RefreshToken},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeToken(const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	RevokeToken(FRegistry::Settings.IamServerUrl, AccessToken, OnSuccess, OnError);
}

void Oauth2::RevokeToken(const FString& IamUrl
	, const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("This method uses wrong auth type and will be deprecated. Use RevokeUserToken instead!"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/revoke"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda(
			[OnSuccess]()
			{ 
				OnSuccess.ExecuteIfBound();
			})
		, OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeUserToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	RevokeUserToken(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, AccessToken, OnSuccess, OnError);
}
	
void Oauth2::RevokeUserToken(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& AccessToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/revoke"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda(
			[OnSuccess]()
			{ 
				OnSuccess.ExecuteIfBound();
			})
		, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithAuthorizationCodeV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTokenWithAuthorizationCodeV3(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, AuthorizationCode, RedirectUri, OnSuccess, OnError);
}
	
void Oauth2::GetTokenWithAuthorizationCodeV3(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& AuthorizationCode
	, const FString& RedirectUri
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("authorization_code") },
		{TEXT("code"), AuthorizationCode },
		{TEXT("redirect_uri"), RedirectUri },
	}, TEXT(""));

	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, bool bRememberMe)
{
	GetTokenWithPasswordCredentialsV3(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, Username, Password, OnSuccess, OnError, bRememberMe);
}
	
void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FErrorHandler& OnError
	, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA is enabled then this method should be changed to the method using FCustomErrorHandler"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("extend_exp"), bRememberMe ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError
	, bool bRememberMe)
{
	GetTokenWithPasswordCredentialsV3(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, Username, Password, OnSuccess, OnError, bRememberMe);
}
	
void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError
	, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetHeader(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("extend_exp"), bRememberMe ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::VerifyAndRememberNewDevice(const FString& ClientId
	, const FString& ClientSecret
	, const FString& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, const FString& Code 
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError
	, bool bRememberDevice)
{
	VerifyAndRememberNewDevice(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, MfaToken, AuthFactorType, Code, OnSuccess, OnError, bRememberDevice);
}
	
void Oauth2::VerifyAndRememberNewDevice(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, const FString& Code 
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError
	, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/mfa/verify"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("mfaToken"), MfaToken},
		{TEXT("factor"), Factor},
		{TEXT("code"), Code},
		{TEXT("rememberDevice"), bRememberDevice ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::CreateHeadlessAccountAndResponseToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& LinkingToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	CreateHeadlessAccountAndResponseToken(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, LinkingToken, OnSuccess, OnError);
}
	
void Oauth2::CreateHeadlessAccountAndResponseToken(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& LinkingToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST")); 
	Request->SetURL(FString::Printf(TEXT("%s/v3/headless/token"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("linkingToken=%s&client_id=%s"), *LinkingToken, *ClientId));
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}

void Oauth2::AuthenticationWithPlatformLink(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const FString& LinkingToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	AuthenticationWithPlatformLink(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, Username, Password, LinkingToken, OnSuccess, OnError);
}
	
void Oauth2::AuthenticationWithPlatformLink(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& Username
	, const FString& Password
	, const FString& LinkingToken
	, const THandler<FOauth2Token>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/authenticateWithLink"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("linkingToken"), LinkingToken},
		{TEXT("client_id"), ClientId},
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::VerifyToken(const FString& ClientId
	, const FString& ClientSecret
	, const FString& Token
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	VerifyToken(FRegistry::Settings.IamServerUrl, ClientId, ClientSecret, Token, OnSuccess, OnError);
}
	
void Oauth2::VerifyToken(const FString& IamUrl
	, const FString& ClientId
	, const FString& ClientSecret
	, const FString& Token
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/verify"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *Token));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda([OnSuccess, Token](FJsonObject const& JsonObject)
			{
				FString CheckedToken;
				JsonObject.TryGetStringField("access_token", CheckedToken);
				if (Token.Equals(CheckedToken))
				{
					OnSuccess.ExecuteIfBound();
				}
			}),
			OnError),
		FPlatformTime::Seconds());
}

void Oauth2::GenerateOneTimeCode(const FString& AccessToken
	, const FString& PlatformId
	, const THandler<FGeneratedOneTimeCode>& OnSuccess
	, const FErrorHandler& OnError)
{
	GenerateOneTimeCode(FRegistry::Settings.IamServerUrl, AccessToken, PlatformId, OnSuccess, OnError);
}
	
void Oauth2::GenerateOneTimeCode(const FString& IamUrl
	, const FString& AccessToken
	, const FString& PlatformId
	, const THandler<FGeneratedOneTimeCode>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST")); 
	Request->SetURL(FString::Printf(TEXT("%s/v3/link/code/request"), *IamUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("platformId=%s"), *PlatformId));
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}

} // Namespace Api
} // Namespace AccelByte
