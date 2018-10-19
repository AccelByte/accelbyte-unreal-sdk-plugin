// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "Base64.h"
#include "AccelByteHttpRetrySystem.h"

namespace AccelByte
{
namespace Api
{
#if 0
// WARNING: THIS DOESN'T ACTUALLY WORK!!!
void Oauth2::GetAccessTokenWithAuthorizationCodeGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString AuthorizationCode, FString RedirectUri, FGetAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	// Missing state parameter. Do not use this!!! You (user) will be susceptible to Cross-Site Request Forgery attack!!!
	FString Content = FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthorizationCode, *RedirectUri);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetAccessTokenWithAuthorizationCodeGrantResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}
#endif

void Oauth2::GetAccessTokenWithPasswordGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Username, FString Password, FGetAccessTokenWithPasswordGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(Username), *FGenericPlatformHttp::UrlEncode(Password));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetAccessTokenWithPasswordGrantResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Oauth2::GetAccessTokenWithClientCredentialsGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=client_credentials"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetAccessTokenWithClientCredentialsGrantResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Oauth2::GetAccessTokenWithRefreshTokenGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RefreshToken, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *FGenericPlatformHttp::UrlEncode(RefreshToken));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetAccessTokenWithRefreshTokenGrantResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

//
// Custom grant types 
//

void Oauth2::GetAccessTokenWithDeviceGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	FString DeviceId = FGenericPlatformMisc::GetDeviceId();

	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/platforms/device/token"), *ServerBaseUrl);;
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("device_id=%s"), *DeviceId);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetAccessTokenWithDeviceGrantResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Oauth2::GetAccessTokenWithPlatformGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString PlatformId, FString PlatformToken, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
    FString Url = FString::Printf(TEXT("%s/iam/oauth/platforms/%s/token"), *ServerBaseUrl, *PlatformId);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/x-www-form-urlencoded");
    FString Accept = TEXT("application/json");
    FString Content = FString::Printf(TEXT("platform_token=%s"), *PlatformToken);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetAccessTokenWithPlatformGrantResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

#if 0
void Oauth2::GetAccessTokenWithAuthorizationCodeGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOauth2Token Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}
#endif

void Oauth2::GetAccessTokenWithPasswordGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithPasswordGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOauth2Token Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Oauth2::GetAccessTokenWithDeviceGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOauth2Token Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Oauth2::GetAccessTokenWithPlatformGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOauth2Token Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Oauth2::GetAccessTokenWithRefreshTokenGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOauth2Token Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void Oauth2::GetAccessTokenWithClientCredentialsGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOauth2Token Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte