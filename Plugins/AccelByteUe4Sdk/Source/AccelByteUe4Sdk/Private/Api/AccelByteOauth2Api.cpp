// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Runtime/Core/Public/Misc/Base64.h"

namespace AccelByte
{
namespace Api
{
void Oauth2::GetSessionIdWithAuthorizationCodeGrant(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const THandler<FOauth2Session>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString url;
	url                     = FRegistry::Settings.IamServerUrl;
	if (FRegistry::Settings.IamServerUrl.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}
	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url             = FString::Printf(TEXT("%s/v1/login/code"), *url);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthorizationCode, *RedirectUri);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetSessionIdWithPasswordGrant(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Session>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString url;
	url                     = FRegistry::Settings.IamServerUrl;
	if (FRegistry::Settings.IamServerUrl.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}
	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url             = FString::Printf(TEXT("%s/v1/login/password"), *url);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(Username), *FGenericPlatformHttp::UrlEncode(Password));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetAccessTokenWithClientCredentialsGrant(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url             = FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

//
// Custom grant types 
//

void Oauth2::GetSessionIdWithDeviceGrant(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Session>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	const TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	FString MacAddressString;
	for (TArray<uint8>::TConstIterator it(MacAddr);it;++it)
	{
		MacAddressString += FString::Printf(TEXT("%02x"),*it);
	}
	FString DeviceId        = FMD5::HashAnsiString(*MacAddressString);
	FString url;
	url = FRegistry::Settings.IamServerUrl;
	if (FRegistry::Settings.IamServerUrl.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}

	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url             = FString::Printf(TEXT("%s/v3/login/platforms/device"), *url);;
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("device_id=%s"), *FGenericPlatformHttp::UrlEncode(*DeviceId));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetSessionIdWithPlatformGrant(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const THandler<FOauth2Session>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString url;
	url                     = FRegistry::Settings.IamServerUrl;
	if (FRegistry::Settings.IamServerUrl.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}

    FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
    FString Url             = FString::Printf(TEXT("%s/v3/login/platforms/%s"), *url, *PlatformId);
    FString Verb            = TEXT("POST");
    FString ContentType     = TEXT("application/x-www-form-urlencoded");
    FString Accept          = TEXT("application/json");
    FString Content         = FString::Printf(TEXT("platform_token=%s"), *FGenericPlatformHttp::UrlEncode(PlatformToken));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetSessionIdWithRefreshId(const FString& ClientId, const FString& ClientSecret, const FString& RefreshId, const THandler<FOauth2Session>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString url;
	url = FRegistry::Settings.IamServerUrl;
	if (FRegistry::Settings.IamServerUrl.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}

	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/v1/sessions/refresh"), *url);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("refresh_id=%s"), *RefreshId);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::Logout(const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString url;
	url                     = FRegistry::Settings.IamServerUrl;
	if (FRegistry::Settings.IamServerUrl.Contains("/iam"))
	{
		url.RemoveFromEnd("/iam");
	}

	FString Authorization   = TEXT("Bearer " + AccessToken);
	FString Url             = FString::Printf(TEXT("%s/v1/logout"), *url);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda([OnSuccess]() { 
		FRegistry::Credentials.ForgetAll();
		OnSuccess.ExecuteIfBound();
	}), OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte