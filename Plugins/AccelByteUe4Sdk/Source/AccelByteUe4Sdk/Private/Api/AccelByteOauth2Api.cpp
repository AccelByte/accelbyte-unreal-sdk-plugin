// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "AccelByteSettings.h"
#include "Base64.h"

namespace AccelByte
{
namespace Api
{
// WARNING: THIS DOESN'T ACTUALLY WORK!!!
void Oauth2::GetAccessTokenWithAuthorizationCodeGrant(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const FGetAccessTokenWithAuthorizationCodeGrantSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/oauth/token"), *Settings::IamServerUrl);
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

void Oauth2::GetAccessTokenWithPasswordGrant(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const FGetAccessTokenWithPasswordGrantSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/oauth/token"), *Settings::IamServerUrl);
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

void Oauth2::GetAccessTokenWithClientCredentialsGrant(const FString& ClientId, const FString& ClientSecret, const FGetAccessTokenWithClientCredentialsGrantSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/oauth/token"), *Settings::IamServerUrl);
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

void Oauth2::GetAccessTokenWithRefreshTokenGrant(const FString& ClientId, const FString& ClientSecret, const FString& RefreshToken, const FGetAccessTokenWithRefreshTokenGrantSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/oauth/token"), *Settings::IamServerUrl);
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

void Oauth2::GetAccessTokenWithDeviceGrant(const FString& ClientId, const FString& ClientSecret, const FGetAccessTokenWithDeviceGrantSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString DeviceId = FGenericPlatformMisc::GetDeviceId();

	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/oauth/platforms/device/token"), *Settings::IamServerUrl);;
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

void Oauth2::GetAccessTokenWithPlatformGrant(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const FGetAccessTokenWithPlatformGrantSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
    FString Url = FString::Printf(TEXT("%s/oauth/platforms/%s/token"), *Settings::IamServerUrl, *PlatformId);
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

void Oauth2::GetAccessTokenWithAuthorizationCodeGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, FErrorHandler OnError)
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

void Oauth2::GetAccessTokenWithPasswordGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithPasswordGrantSuccess OnSuccess, FErrorHandler OnError)
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

void Oauth2::GetAccessTokenWithDeviceGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, FErrorHandler OnError)
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

void Oauth2::GetAccessTokenWithPlatformGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, FErrorHandler OnError)
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

void Oauth2::GetAccessTokenWithRefreshTokenGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, FErrorHandler OnError)
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

void Oauth2::GetAccessTokenWithClientCredentialsGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, FErrorHandler OnError)
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