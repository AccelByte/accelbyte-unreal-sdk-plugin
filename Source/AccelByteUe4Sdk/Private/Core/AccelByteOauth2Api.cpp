// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteOauth2Api.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{

Oauth2::Oauth2(FHttpRetryScheduler& InHttpRef)
: HttpRef{InHttpRef}
{}

Oauth2::Oauth2(FHttpRetryScheduler& InHttpRef, FString const& InIamServerUrl)
	: HttpRef(InHttpRef)
	, IamServerUrl(InIamServerUrl)
{}

FHttpRequestPtr Oauth2::ConstructTokenRequest(FString const& Url
	, FString const& ClientId
	, FString const& ClientSecret
	, TMap<FString, FString> const& AdditionalHeaders)
{
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(Url);
	if (!ClientId.IsEmpty())
	{
		Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	}
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("x-flight-id"), FAccelByteUtilities::GetFlightId());

	for (auto& KVP : AdditionalHeaders)
	{
		Request->SetHeader(KVP.Key, KVP.Value);
	}

	return Request;
}

FString Oauth2::ConstructAdditionalData()
{
	TSharedPtr<FJsonObject> AdditionalDataJsonObj = MakeShared<FJsonObject>();
	AdditionalDataJsonObj->SetStringField(TEXT("flightId"), FAccelByteUtilities::GetFlightId());

	FString AdditionalDataString{};
	const auto Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&AdditionalDataString);
	FJsonSerializer::Serialize(AdditionalDataJsonObj.ToSharedRef(), Writer);
	return AdditionalDataString;
}

FAccelByteTaskWPtr Oauth2::GetTokenWithAuthorizationCode(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AuthorizationCode
	, FString const& RedirectUri
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	return GetTokenWithAuthorizationCode(ClientId
		, ClientSecret
		, AuthorizationCode
		, RedirectUri
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl
		, AdditionalHeaders);
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithAuthorizationCode(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AuthorizationCode
	, FString const& RedirectUri
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__)
		, TEXT("The API might be removed wihtout notice, so please use GetTokenWithAuthorizationCodeV3 instead!!"));

	const FString Url = FString::Printf(TEXT("%s/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("grant_type"), TEXT("authorization_code") },
		{ TEXT("code"), AuthorizationCode },
		{ TEXT("redirect_uri"), RedirectUri },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithPasswordCredentials(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	return GetTokenWithPasswordCredentials(ClientId
		, ClientSecret
		, Username
		, Password
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl
		, AdditionalHeaders
		, DeviceId);
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithPasswordCredentials(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__)
		, TEXT("The API might be removed without notice, please use GetTokenWithPasswordCredentialsV3 instead!!"));
	
	const FString Url = FString::Printf(TEXT("%s/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("grant_type"), TEXT("password") },
		{ TEXT("username"), FGenericPlatformHttp::UrlEncode(Username) },
		{ TEXT("password"), FGenericPlatformHttp::UrlEncode(Password) },
		{ TEXT("device_id"), FGenericPlatformHttp::UrlEncode(DeviceId) },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithClientCredentials(FString const& ClientId
	, FString const& ClientSecret
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("grant_type"), TEXT("client_credentials") },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithDeviceId(FString const& ClientId
	, FString const& ClientSecret
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, bool bCreateHeadless
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	return GetTokenWithDeviceId(ClientId
		, ClientSecret
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl
		, bCreateHeadless
		, AdditionalHeaders
		, DeviceId);
}

FAccelByteTaskWPtr Oauth2::GetTokenWithDeviceId(FString const& ClientId
	, FString const& ClientSecret 
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, bool bCreateHeadless
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/platforms/device/token?createHeadless=%s")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl, bCreateHeadless ? TEXT("true") : TEXT("false"));
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("device_id"), *FGenericPlatformHttp::UrlEncode(*DeviceId) },
		{ TEXT("createHeadless"), bCreateHeadless ? TEXT("true") : TEXT("false") },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithOtherPlatformToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& PlatformId
	, FString const& PlatformToken
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams
	, FString const& EncodedMacAddress
	, FString const& DeviceId)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    	
	return GetTokenWithOtherPlatformToken(ClientId
		, ClientSecret
		, PlatformId
		, PlatformToken
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, true
		, IamUrl
		, AdditionalHeaders
		, OptionalParams
		, EncodedMacAddress
		, DeviceId);
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithOtherPlatformToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& PlatformId
	, FString const& PlatformToken
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams
	, FString const& EncodedMacAddress
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token?createHeadless=%s")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl
		, *PlatformId
		, bCreateHeadless ? TEXT("true") : TEXT("false"));
	
	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("platform_token"), FGenericPlatformHttp::UrlEncode(PlatformToken)},
		{TEXT("createHeadless"), bCreateHeadless ? TEXT("true") : TEXT("false")},
		{TEXT("skipSetCookie"), OptionalParams.bSkipSetCookie ? TEXT("true") : TEXT("false")},
		{TEXT("macAddress"), FGenericPlatformHttp::UrlEncode(EncodedMacAddress) },
		{TEXT("additionalData"), ConstructAdditionalData()},
		{TEXT("serviceLabel"), OptionalParams.ServiceLabelForPSN},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithRefreshToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& RefreshToken
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	return GetTokenWithRefreshToken(ClientId
		, ClientSecret
		, RefreshToken
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl
		, AdditionalHeaders);
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithRefreshToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& RefreshToken
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
			, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("refresh_token")},
		{TEXT("refresh_token"), RefreshToken},
		{TEXT("additionalData"), ConstructAdditionalData()},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::RefreshPlatformToken(FString const& ClientID
	, FString const& ClientSecret
	, FString const& PlatformID
	, FString const& PlatformToken
	, THandler<FPlatformTokenRefreshResponse> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/platforms/%s/token/verify")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl,
		*PlatformID);

	FString Content = FString::Printf(TEXT("platform_token=%s"), *PlatformToken);
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientID, ClientSecret, AdditionalHeaders);
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::RevokeToken(FString const& AccessToken
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("This method uses wrong auth type, so please use RevokeToken with ClientId and ClientSecret instead!"));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/revoke")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	AdditionalHeaders.Emplace(TEXT("Authorization"), TEXT("Bearer " + AccessToken));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, TEXT(""), TEXT(""), AdditionalHeaders);

	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::RevokeToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AccessToken
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	return RevokeToken(ClientId
		, ClientSecret
		, AccessToken
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl
		, AdditionalHeaders);
}
	
FAccelByteTaskWPtr Oauth2::RevokeToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AccessToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/revoke")
				, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithAuthorizationCodeV3(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AuthorizationCode
	, FString const& RedirectUri
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	return GetTokenWithAuthorizationCodeV3(ClientId
		, ClientSecret
		, AuthorizationCode
		, RedirectUri
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, IamUrl
		, AdditionalHeaders);
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithAuthorizationCodeV3(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AuthorizationCode
	, FString const& RedirectUri
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("authorization_code") },
		{TEXT("code"), AuthorizationCode },
		{TEXT("redirect_uri"), RedirectUri },
		{TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithPasswordCredentialsV3(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, THandler<FOauth2Token> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bRememberMe
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));
    
	return GetTokenWithPasswordCredentialsV3(ClientId
		, ClientSecret
		, Username
		, Password
		, OnSuccess
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, bRememberMe
		, IamUrl
		, AdditionalHeaders
		, DeviceId);
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithPasswordCredentialsV3(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberMe
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/token")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	AdditionalHeaders.Emplace(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password")},
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("extend_exp"), bRememberMe ? TEXT("true") : TEXT("false")},
		{TEXT("additionalData"), ConstructAdditionalData()},
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
FAccelByteTaskWPtr Oauth2::GetTokenWithSimultaneousPlatformToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& NativePlatformName
	, FString const& NativePlatformToken
	, FString const& SecondaryPlatformName
	, FString const& SecondaryPlatformToken
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/simultaneousLogin")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	AdditionalHeaders.Emplace(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("nativePlatform"), NativePlatformName },
		{ TEXT("nativePlatformTicket"), FGenericPlatformHttp::UrlEncode(NativePlatformToken) },
		{ TEXT("simultaneousPlatform"), FGenericPlatformHttp::UrlEncode(SecondaryPlatformName) },
		{ TEXT("simultaneousTicket"), FGenericPlatformHttp::UrlEncode(SecondaryPlatformToken) },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::VerifyAndRememberNewDevice(FString const& ClientId
	, FString const& ClientSecret
	, FString const& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, FString const& Code 
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberDevice
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/mfa/verify")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("mfaToken"), MfaToken },
		{ TEXT("factor"), Factor },
		{ TEXT("code"), Code },
		{ TEXT("rememberDevice"), bRememberDevice ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
FAccelByteTaskWPtr Oauth2::CreateHeadlessAccountAndResponseToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& LinkingToken
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/headless/token")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("linkingToken"), LinkingToken },
		{ TEXT("client_id"), ClientId },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}
	
FAccelByteTaskWPtr Oauth2::AuthenticationWithPlatformLink(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, FString const& LinkingToken
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/authenticateWithLink")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("username"), FGenericPlatformHttp::UrlEncode(Username) },
		{ TEXT("password"), FGenericPlatformHttp::UrlEncode(Password) },
		{ TEXT("linkingToken"), LinkingToken },
		{ TEXT("client_id"), ClientId },
	}, TEXT(""));
	Request->SetContentAsString(Content);
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
FAccelByteTaskWPtr Oauth2::VerifyToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Token
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/verify")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *Token));

	return HttpRef.ProcessRequest(Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda([OnSuccess, Token](FJsonObject const& JsonObject)
			{
				FString CheckedToken;
				JsonObject.TryGetStringField(TEXT("access_token"), CheckedToken);
				if (Token.Equals(CheckedToken))
				{
					OnSuccess.ExecuteIfBound();
				}
			}),
			OnError),
		FPlatformTime::Seconds());
}
	
FAccelByteTaskWPtr Oauth2::GenerateOneTimeCode(FString const& AccessToken
	, FString const& PlatformId
	, THandler<FGeneratedOneTimeCode> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/link/code/request")
					, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);
	
	AdditionalHeaders.Emplace(TEXT("Authorization"), TEXT("Bearer " + AccessToken));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, TEXT(""), TEXT(""), AdditionalHeaders);
	Request->SetContentAsString(FString::Printf(TEXT("platformId=%s"), *PlatformId));
	
	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}
	
FAccelByteTaskWPtr Oauth2::GenerateGameToken(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Code
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/token/exchange")
							, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	
	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("code"), Code },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}

FAccelByteTaskWPtr Oauth2::GenerateCodeForPublisherTokenExchange(FString const& AccessToken
	, FString const& PublisherNamespace
	, FString const& PublisherClientID
	, THandler<FCodeForTokenExchangeResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));
	const FString Url = FString::Printf(TEXT("%s/v3/namespace/%s/token/request")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl
		, *PublisherNamespace);

	AdditionalHeaders.Emplace(TEXT("Authorization"), TEXT("Bearer " + AccessToken));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, TEXT(""), TEXT(""), AdditionalHeaders);
	Request->SetContentAsString(FString::Printf(TEXT("client_id=%s"), *PublisherClientID));

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::RetrieveUserThirdPartyPlatformToken(FString const& Namespace
	, FString const& UserId
	, FString const& PlatformId
	, FString const& Authorization
	, THandler<FThirdPartyPlatformTokenData> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/oauth/namespaces/%s/users/%s/platforms/%s/platformToken")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl
		, *Namespace 
		, *UserId
		, *PlatformId);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithPasswordCredentialsV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberMe
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	AdditionalHeaders.Emplace(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("password") },
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username) },
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password) },
		{TEXT("extend_exp"), bRememberMe ? TEXT("true") : TEXT("false") },
		{TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithDeviceIdV4(FString const& ClientId
	, FString const& ClientSecret
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, bool bCreateHeadless
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/platforms/device/token?createHeadless=%s")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl, bCreateHeadless ? TEXT("true") : TEXT("false"));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("device_id"), *FGenericPlatformHttp::UrlEncode(*DeviceId) },
		{TEXT("createHeadless"), bCreateHeadless ? TEXT("true") : TEXT("false") },
		{TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithAuthorizationCodeV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& AuthorizationCode
	, FString const& RedirectUri
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("authorization_code") },
		{TEXT("code"), AuthorizationCode },
		{TEXT("redirect_uri"), RedirectUri },
		{TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithOtherPlatformTokenV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& PlatformId
	, FString const& PlatformToken
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams
	, FString const& DeviceId
	, FString const& EncodedMacAddress)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/platforms/%s/token?createHeadless=%s")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl
		, *PlatformId
		, bCreateHeadless ? TEXT("true") : TEXT("false"));

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("platform_token"), FGenericPlatformHttp::UrlEncode(PlatformToken)},
		{TEXT("createHeadless"), bCreateHeadless ? TEXT("true") : TEXT("false")},
		{TEXT("skipSetCookie"), OptionalParams.bSkipSetCookie ? TEXT("true") : TEXT("false")},
		{TEXT("macAddress"), FGenericPlatformHttp::UrlEncode(EncodedMacAddress) },
		{TEXT("additionalData"), ConstructAdditionalData()},
		{TEXT("serviceLabel"), OptionalParams.ServiceLabelForPSN},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::VerifyAndRememberNewDeviceV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, FString const& Code
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberDevice
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/mfa/verify")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("mfaToken"), MfaToken},
		{TEXT("factor"), Factor},
		{TEXT("code"), Code},
		{TEXT("rememberDevice"), bRememberDevice ? TEXT("true") : TEXT("false") },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GenerateGameTokenV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Code
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/token/exchange")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("code"), Code },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::AuthenticationWithPlatformLinkV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& Username
	, FString const& Password
	, FString const& LinkingToken
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/authenticateWithLink")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("username"), FGenericPlatformHttp::UrlEncode(Username)},
		{TEXT("password"), FGenericPlatformHttp::UrlEncode(Password)},
		{TEXT("linkingToken"), LinkingToken},
		{TEXT("client_id"), ClientId},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::CreateHeadlessAccountAndResponseTokenV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& LinkingToken
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/headless/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("linkingToken"), LinkingToken },
		{ TEXT("client_id"), ClientId },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithSimultaneousPlatformTokenV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& NativePlatformName
	, FString const& NativePlatformToken
	, FString const& SecondaryPlatformName
	, FString const& SecondaryPlatformToken
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/simultaneousLogin")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	AdditionalHeaders.Emplace(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("nativePlatform"), NativePlatformName },
		{ TEXT("nativePlatformTicket"), FGenericPlatformHttp::UrlEncode(NativePlatformToken) },
		{ TEXT("simultaneousPlatform"), FGenericPlatformHttp::UrlEncode(SecondaryPlatformName) },
		{ TEXT("simultaneousTicket"), FGenericPlatformHttp::UrlEncode(SecondaryPlatformToken) },
		{ TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithRefreshTokenV4(FString const& ClientId
	, FString const& ClientSecret
	, FString const& RefreshToken
	, THandler<FOauth2TokenV4> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("refresh_token")},
		{TEXT("refresh_token"), RefreshToken},
		{TEXT("additionalData"), ConstructAdditionalData()},
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FAccelByteTaskWPtr Oauth2::GetTokenWithLoginTicket(FString const& ClientId
	, FString const& ClientSecret
	, FString const& LoginTicket
	, THandler<FOauth2Token> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& IamUrl
	, TMap<FString, FString> AdditionalHeaders
	, FString const& DeviceId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/oauth/token")
		, IamUrl.IsEmpty() ? *IamServerUrl : *IamUrl);

	AdditionalHeaders.Emplace(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(DeviceId)));
	AdditionalHeaders.Emplace(TEXT("Auth-Trust-Id"), FAccelByteUtilities::GetAuthTrustId());

	FHttpRequestPtr Request = ConstructTokenRequest(Url, ClientId, ClientSecret, AdditionalHeaders);
	FString Content = FAccelByteUtilities::CreateQueryParams({
		{TEXT("grant_type"), TEXT("urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Alogin_queue_ticket") },
		{TEXT("login_queue_ticket"), LoginTicket },
		{TEXT("additionalData"), ConstructAdditionalData() },
	}, TEXT(""));
	Request->SetContentAsString(Content);

	return HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
