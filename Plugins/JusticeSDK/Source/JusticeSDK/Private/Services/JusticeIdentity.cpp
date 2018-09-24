// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
#include "ModuleManager.h"
#include "HttpManager.h"
#include "HttpModule.h"

FCriticalSection Mutex;

void JusticeIdentity::ForgotPassword(FString Namespace, FString LoginID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *FJusticeBaseURL, *Namespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_JSON;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("{\"LoginID\": \"%s\"}"), *LoginID);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnForgotPasswordResponse, OnComplete));
}

void JusticeIdentity::OnForgotPasswordResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Forgot Password Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
	{
		UE_LOG(LogJustice, Log, TEXT("Forgot Password Success"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::BadRequest:
	{
		ErrorStr = FString::Printf(TEXT("Forgot Password Expected Error: Invalid Request. Code: %d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::Forbidden:
	{
		ErrorStr = FString::Printf(TEXT("Forgot Password Expected Error: Forbidden. Code: %d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::NotFound:
	{
		ErrorStr = FString::Printf(TEXT("Forgot Password Expected Error: Data not found. Code: %d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnForgotPasswordResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				return;
			}
		}));
		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnForgotPasswordResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Forgot Password Unexpected response, Code: %d, Content: %s"), Response->Code, *Response->Content);
		break;
	}					
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"),*ErrorStr);		
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ResetPassword(FString Namespace, FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete)
{
	FResetPasswordRequest resetPasswordRequest;
	resetPasswordRequest.Code = VerificationCode;
	resetPasswordRequest.LoginID = UserID;
	resetPasswordRequest.NewPassword = NewPassword;

	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *FJusticeBaseURL, *Namespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_JSON;
	FString Accept          = TYPE_JSON;
	FString Payload         = resetPasswordRequest.ToJson();

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnResetPasswordResponse, OnComplete));
}

void JusticeIdentity::OnResetPasswordResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Reset Password Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
	{
		UE_LOG(LogJustice, Log, TEXT("OnResetPasswordResponse receive success response "));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::BadRequest:
	{
		ErrorStr = FString::Printf(TEXT("Expected Error: Invalid Request. Code=%d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::Forbidden:
	{
		ErrorStr = FString::Printf(TEXT("Expected Error: Forbidden. Code=%d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::NotFound:
	{
		ErrorStr = FString::Printf(TEXT("Expected Error: Data not found. Code=%d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnResetPasswordResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				return;
			}
		}));

	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnResetPasswordResponse, OnComplete));
		return;
		break;
	}
	case EHttpResponseCodes::TooManyRequests:
	{
		ErrorStr = FString::Printf(TEXT("You have to wait 15 minutes before request another Reset Password. Response Code=%d"), Response->Code);
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
		break;
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr, *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate2 OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization = FJusticeHTTP::BasicAuth();
	FString URL           = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb          = POST;
	FString ContentType   = TYPE_FORM;
	FString Accept        = TYPE_JSON;
	FString Payload       = FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthCode, *RedirectURI);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLoginResponse, OnComplete));
}

void JusticeIdentity::UserLogin(FString Namespace, FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("grant_type=password&username=%s&password=%s&namespace=%s"), *FGenericPlatformHttp::UrlEncode(LoginID), *FGenericPlatformHttp::UrlEncode(Password), *Namespace);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLoginResponse, OnComplete));
}

void JusticeIdentity::UserLogin(FString  Namespace, FString  ClientId, FString  ClientSecret, FString LoginID, FString Password, FUserLoginCompleteDelegate2 OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization = FJusticeHTTP::BasicAuth(ClientId, ClientSecret);
	FString URL = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb = POST;
	FString ContentType = TYPE_FORM;
	FString Accept = TYPE_JSON;
	FString Payload = FString::Printf(TEXT("grant_type=password&username=%s&password=%s&namespace=%s"), *FGenericPlatformHttp::UrlEncode(LoginID), *FGenericPlatformHttp::UrlEncode(Password), *Namespace);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLoginResponse, OnComplete));
}

void JusticeIdentity::OnUserLoginResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate2 OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("User Login Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FJusticeSDKModule::Get().bHeadlessAccount = false;
		bool result = FJusticeSDKModule::Get().ParseUserToken(Response->Content);
		check(result);
		TSharedPtr<FOAuthTokenJustice> OAuthToken = MakeShared<FOAuthTokenJustice>();
		OAuthToken->FromJson(Response->Content);
		FJusticeUserToken->SetLastRefreshTimeToNow();
		FJusticeUserToken->ScheduleNormalRefresh();		
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUTC);
		OnComplete.ExecuteIfBound(true, TEXT(""), OAuthToken);
		break;
	}

	case EHttpResponseCodes::Denied:
		ErrorStr = FString::Printf(TEXT("User Login Failed, Response: %s. XRay: %s"), *Response->Content, *Response->AmazonTraceID);
		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLoginResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("User Login Unexpcted Result,Response Code: %d  Content:%s"), Response->Code, *Response->Content);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}


void JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString deviceID = FGenericPlatformMisc::GetDeviceId();
	check(!deviceID.IsEmpty() && "Cannot get Device ID");

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/namespaces/%s/platforms/device/token"), *FJusticeBaseURL, *JusticeGameNamespace);;	
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("device_id=%s"), *deviceID);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnDeviceLoginResponse, OnComplete));
}

void JusticeIdentity::OnDeviceLoginResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("User Login Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FJusticeSDKModule::Get().bHeadlessAccount = true;
		bool result = FJusticeSDKModule::Get().ParseUserToken(Response->Content);
		check(result);
		FJusticeUserToken->SetLastRefreshTimeToNow();
		FJusticeUserToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUTC);
		OnComplete.ExecuteIfBound(true, TEXT(""), FJusticeUserToken.Get());
		break;
	}

	case EHttpResponseCodes::Denied:
		ErrorStr = FString::Printf(TEXT("User Login Failed, Response: %s. XRay: %s"), *Response->Content, *Response->AmazonTraceID);
		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnDeviceLoginResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("User Login Unexpcted Result,Response Code: %d  Content:%s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}

void JusticeIdentity::UserLogout(FDefaultCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *FJusticeBaseURL);	
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;

	TArray<FString> Out;
	FJusticeUserToken->AccessToken.ParseIntoArray(Out, TEXT("."), true);

	if (Out.Num() == 3)
	{
		FString Payload = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(Out[2]));

		FJusticeHTTP::CreateRequest(
			Authorization,
			URL,
			Verb,
			ContentType,
			Accept,
			Payload,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLogoutResponse, OnComplete));
	}
	else
	{
		UE_LOG(LogJustice, Error, TEXT("UserLogout Error: Invalid Token"));
	}


}

void JusticeIdentity::OnUserLogoutResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("User Logout Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		UE_LOG(LogJustice, Log, TEXT("User Logout Success"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("User Logout Unexpcted Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}	
}

void JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate OnComplete)
{
	FString RefreshToken = FJusticeSDKModule::Get().UserToken->UserRefreshToken;

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeSDKModule::Get().BaseURL);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *FGenericPlatformHttp::UrlEncode(RefreshToken));

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserRefreshResponse, OnComplete));
}
void JusticeIdentity::UserRefresh()
{
	JusticeIdentity::UserRefreshToken(nullptr);
}

void JusticeIdentity::OnUserRefreshResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Refresh User Token Failed. Backoff refresh. Error Message: %s."), *Response->ErrorString);
		FJusticeUserToken->ScheduleBackoffRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUTC);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		bool result = FJusticeSDKModule::Get().ParseUserToken(Response->Content);
		check(result);
		FJusticeUserToken->SetLastRefreshTimeToNow();
		FJusticeUserToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUTC);
		OnComplete.ExecuteIfBound(true, TEXT(""), FJusticeUserToken.Get());
		break;
	}		
	default:
		ErrorStr = FString::Printf(TEXT("User Refresh Unexpected Response. Code: %d, Response:%s, Start Backoff refresh !"), Response->Code, *Response->Content);
		FJusticeUserToken->ScheduleBackoffRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUTC);
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}

void JusticeIdentity::ClientRefreshToken()
{
	FScopeLock Lock(&Mutex);

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeSDKModule::Get().BaseURL);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("grant_type=client_credentials"));

	FJusticeHTTP::CreateRequest(Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientRefreshResponse));
}

void JusticeIdentity::OnClientRefreshResponse(FJusticeResponsePtr Response)
{
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Refresh Game Client Credential Failed. Backoff refresh. Error Message: %s."), *Response->ErrorString);
		FJusticeGameClientToken->ScheduleBackoffRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUTC);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FJusticeSDKModule::Get().ParseClientToken(Response->Content);
		FJusticeGameClientToken->SetLastRefreshTimeToNow();
		FJusticeGameClientToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUTC);
		UE_LOG(LogJustice, Log, TEXT("Refresh Game Client Credential Success. XRay: %s"), *Response->AmazonTraceID);
		break;
	}
	default:
		UE_LOG(LogJustice, Error, TEXT("Refresh Game Client Credential Failed. Backoff refresh. Reponse Code: %d. Error Message: %s. XRay: %s"), Response->Code,  *Response->ErrorString, *Response->AmazonTraceID);
		FJusticeGameClientToken->ScheduleBackoffRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUTC);
		break;
	}
}

void JusticeIdentity::SetRefreshToken(FString UserRefreshToken)
{
	FJusticeUserToken->UserRefreshToken = UserRefreshToken;
}

void JusticeIdentity::LoginWithPlatform(FString Namespace, FString PlatformID, FString Token, FUserLoginCompleteDelegate2 OnComplete)
{
    FScopeLock Lock(&Mutex);

    FString Authorization  = FJusticeHTTP::BasicAuth();
    FString URL            = FString::Printf(TEXT("%s/iam/oauth/platforms/%s/token"), *FJusticeSDKModule::Get().BaseURL, *PlatformID);
    FString Verb           = POST;
    FString ContentType    = TYPE_FORM;
    FString Accept         = TYPE_JSON;
    FString Payload        = FString::Printf(TEXT("platform_token=%s&namespace=%s"), *Token, *Namespace);

    FJusticeHTTP::CreateRequest(Authorization,
        URL,
        Verb,
        ContentType,
        Accept,
        Payload,
        FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnPlatformLoginResponse, OnComplete));
}

void JusticeIdentity::OnPlatformLoginResponse(FJusticeResponsePtr Response, FUserLoginCompleteDelegate2 OnComplete)
{
    FString ErrorStr;
    if (!Response->ErrorString.IsEmpty())
    {
        UE_LOG(LogJustice, Error, TEXT("Platform Login Failed. Error Message: %s."), *Response->ErrorString);
        OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
        return;
    }
    switch (Response->Code)
    {
    case EHttpResponseCodes::Ok:
    {
        FJusticeSDKModule::Get().bHeadlessAccount = false;
        bool result = FJusticeSDKModule::Get().ParseUserToken(Response->Content);
        check(result);
		TSharedPtr<FOAuthTokenJustice> OAuthToken = MakeShared<FOAuthTokenJustice>();
		OAuthToken->FromJson(Response->Content);
        FJusticeUserToken->SetLastRefreshTimeToNow();
        FJusticeUserToken->ScheduleNormalRefresh();
        FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUTC);
        OnComplete.ExecuteIfBound(true, TEXT(""), OAuthToken);
        break;
    }

    case EHttpResponseCodes::Denied:
        ErrorStr = FString::Printf(TEXT("Platform Login Failed, Response: %s. XRay: %s"), *Response->Content, *Response->AmazonTraceID);
        break;
    case EHttpResponseCodes::RequestTimeout:
        ErrorStr = TEXT("Request Timeout");
        break;
    case EHttpResponseCodes::Forbidden:
        ErrorStr = TEXT("Forbidden");
        break;
    case EHttpResponseCodes::ServerError:
    case EHttpResponseCodes::ServiceUnavail:
    case EHttpResponseCodes::GatewayTimeout:
    {
        if (Response->TooManyRetries() || Response->TakesTooLong())
        {
            ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
            OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
            return;
        }
        Response->UpdateRequestForNextRetry();
        FJusticeRetryManager->AddQueue(Response->JusticeRequest,
            Response->NextWait,
            FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnPlatformLoginResponse, OnComplete));
        break;
    }
    default:
        ErrorStr = FString::Printf(TEXT("Platform Login Unexpcted Result,Response Code: %d  Content:%s"), Response->Code, *Response->Content);
    }

    if (!ErrorStr.IsEmpty())
    {
        UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
        OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
        return;
    }
}

void JusticeIdentity::GenerateAuthorizationCode(FOAuthTokenJustice OAuthToken, FString ClientID, FString RedirectURI, FAuthCodeCompleteDelegate OnComplete)
{
    FString Namespace = JusticeGameNamespace;
    FString Authorization = FJusticeHTTP::BearerAuth(OAuthToken.AccessToken);
    FString URL = FString::Printf(TEXT("%s/iam/oauth/authorize"), *FJusticeSDKModule::Get().BaseURL);
    FString Verb = POST;
    FString ContentType = TYPE_FORM;
    FString Accept = TYPE_JSON;
    FString Payload = FString::Printf(TEXT("client_id=%s&redirect_uri=%s&response_type=code"), *ClientID, *RedirectURI);

    FJusticeHTTP::CreateRequest(Authorization,
        URL,
        Verb,
        ContentType,
        Accept,
        Payload,
        FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnGenerateAuthorizationCodeResponse, OnComplete));
}

void JusticeIdentity::OnGenerateAuthorizationCodeResponse(FJusticeResponsePtr Response, FAuthCodeCompleteDelegate OnComplete)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnGenerateAuthorizationCodeResponse"));
    }

    FString ErrorStr;
    if (!Response->ErrorString.IsEmpty())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Generate AuthorizationCode Failed . Error Message"));
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Response->ErrorString);
        }
        UE_LOG(LogJustice, Error, TEXT("Generate AuthorizationCode Failed . Error Message: %s."), *Response->ErrorString);
        OnComplete.ExecuteIfBound(false, Response->ErrorString, TEXT(""));
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("JusticeIdentity::OnGenerateAuthorizationCodeResponse Code:%d"), Response->Code));
    }


    switch (Response->Code)
    {
    case EHttpResponseCodes::Redirect :
    {
        FString Location = Response->Response->GetHeader(TEXT("Location"));
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("JusticeIdentity::OnGenerateAuthorizationCodeResponse"));
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Location);
        }



        TMap<FString, FString> queryDict;
        TArray<FString> queries;
        Location.Mid(Location.Find(TEXT("?")) + 1).ParseIntoArray(queries, TEXT("&"), true);

        for (FString query : queries)
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("JusticeIdentity::OnGenerateAuthorizationCodeResponse query"));
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, query);
            }


            TArray<FString> keyAndValue;
            query.ParseIntoArray(keyAndValue, TEXT("="), true);
            queryDict.Add(keyAndValue[0], keyAndValue.Num() > 1 ? keyAndValue[1] : TEXT(""));
        }

        if (queryDict.Contains(TEXT("code")))
        {
            FString AuthCode = queryDict[TEXT("code")];
            OnComplete.ExecuteIfBound(true, TEXT(""), AuthCode);
            return;
        }
        else
        {
            FString ErrorDesc = queryDict[TEXT("error_description")];
            ErrorStr = FString::Printf(TEXT("Auth Code Failed, error_description: %s"), *ErrorDesc);
        }
            
        break;
    }

    case EHttpResponseCodes::Denied:
        ErrorStr = FString::Printf(TEXT("Platform Login Failed, Response: %s. XRay: %s"), *Response->Content, *Response->AmazonTraceID);
        break;
    case EHttpResponseCodes::RequestTimeout:
        ErrorStr = TEXT("Request Timeout");
        break;
    case EHttpResponseCodes::Forbidden:
        ErrorStr = TEXT("Forbidden");
        break;
    case EHttpResponseCodes::ServerError:
    case EHttpResponseCodes::ServiceUnavail:
    case EHttpResponseCodes::GatewayTimeout:
    {
        if (Response->TooManyRetries() || Response->TakesTooLong())
        {
            ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
            OnComplete.ExecuteIfBound(false, ErrorStr, TEXT(""));
            return;
        }
        Response->UpdateRequestForNextRetry();
        FJusticeRetryManager->AddQueue(Response->JusticeRequest,
            Response->NextWait,
            FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnGenerateAuthorizationCodeResponse, OnComplete));
        break;
    }
    default:
        ErrorStr = FString::Printf(TEXT("Platform Login Unexpcted Result,Response Code: %d  Content:%s"), Response->Code, *Response->Content);
    }

    if (!ErrorStr.IsEmpty())
    {
        UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
        OnComplete.ExecuteIfBound(false, ErrorStr, TEXT(""));
        return;
    }
}

void JusticeIdentity::SetSteamAuthTicket(FString ticket)
{
    FJusticeSDKModule::Get().SteamAuthTicket = ticket;
}

void JusticeIdentity::SetSteamNickName(FString nickname)
{
    FJusticeSDKModule::Get().SteamNickName = nickname;
}

void JusticeIdentity::SetAvatar(UTexture2D * avatar)
{
    FJusticeSDKModule::Get().Avatar = avatar;
}

FString JusticeIdentity::GetSteamAuthTicket()
{
    return FJusticeSDKModule::Get().SteamAuthTicket;
}

FString JusticeIdentity::GetSteamNickName()
{
    return FJusticeSDKModule::Get().SteamNickName;
}

UTexture2D * JusticeIdentity::GetAvatar()
{
    return FJusticeSDKModule::Get().Avatar;
}

void JusticeIdentity::ClientLogout()
{
	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *FJusticeBaseURL, *JusticeGameNamespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;

	TArray<FString> Out;
	FJusticeGameClientToken->AccessToken.ParseIntoArray(Out, TEXT("."), true);

	if (Out.Num() == 3)
	{
		FString Payload = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(Out[2]));
		FJusticeHTTP::CreateRequest(
			Authorization,
			URL,
			Verb,
			ContentType,
			Accept,
			Payload,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLogoutResponse));
	}
	else
	{
		UE_LOG(LogJustice, Error, TEXT("ClientLogout Error: Invalid Token"));
	}
}

void JusticeIdentity::OnClientLogoutResponse(FJusticeResponsePtr Response)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Failed. Error Message: %s."), *Response->ErrorString);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok: 
		UE_LOG(LogJustice, Log, TEXT("Client Logout Success"));
		FJusticeGameClientToken.Reset();
		FJusticeRefreshManager->ClearRefreshQueue();		
		break;
	default:		
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d. Content: %s"), Response->Code, *Response->Content);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Client Logout %s"), *ErrorStr);
		return;
	}
}

void JusticeIdentity::RegisterNewPlayer(FString Namespace, FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
	FUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginID = UserID;
	NewUserRequest.AuthType = (AuthType == Email) ? TEXT("EMAILPASSWD"): TEXT("PHONEPASSWD");

	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *FJusticeBaseURL, *Namespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_JSON;
	FString Accept          = TYPE_JSON;
	FString Payload         = NewUserRequest.ToJson();

	FJusticeHTTP::CreateRequest(Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnRegisterNewPlayerResponse, OnComplete));
}

void JusticeIdentity::OnRegisterNewPlayerResponse(TSharedPtr<JusticeHttpResponse, ESPMode::ThreadSafe> Response, FRegisterPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Register New Player Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Created:
	{
		UE_LOG(LogJustice, Log, TEXT(" Register New Player Entity Created"));
		TSharedPtr<FUserCreateResponse> UserCreate = MakeShared<FUserCreateResponse>();
		UserCreate->FromJson(Response->Content);
		OnComplete.ExecuteIfBound(true, TEXT(""), UserCreate);
		break;
	}
	case EHttpResponseCodes::BadRequest:
		ErrorStr = TEXT("Invalid Request");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::Conflict:
		ErrorStr = TEXT("Conflict");
		break;
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&, OnComplete](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnRegisterNewPlayerResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			return;
		}
		//retry 
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnRegisterNewPlayerResponse, OnComplete));
		break;

	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpcted Response Code: %d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::VerifyNewPlayer(FString Namespace, FString UserID, FString VerificationCode, FUserAuthTypeJustice AuthType, FDefaultCompleteDelegate OnComplete)
{
	FString ContactType = (AuthType == Email) ? TEXT("email") : TEXT("phone");

	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verification"), *FJusticeBaseURL, *Namespace, *UserID);
	FString Verb            = POST;
	FString ContentType     = TYPE_JSON;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnVerifyNewPlayerResponse, OnComplete));
}

void JusticeIdentity::OnVerifyNewPlayerResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Verify New Player Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	
	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
		UE_LOG(LogJustice, VeryVerbose, TEXT("TokenVerifyNewPlayerComplete : Player Activated"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	case EHttpResponseCodes::BadRequest:
		ErrorStr = TEXT("Invalid Request");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnVerifyNewPlayerResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnVerifyNewPlayerResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected response Code: %d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ReissueVerificationCode(FString Namespace, FString UserID, FString LoginID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *FJusticeBaseURL, *Namespace, *UserID);
	FString Verb            = POST;
	FString ContentType     = TYPE_JSON;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("{ \"LoginID\": \"%s\"}"), *LoginID);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnReissueVerificationCodeResponse, OnComplete));
}

void JusticeIdentity::ReissueVerificationCode(FOAuthTokenJustice Token, FString LoginID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization = FJusticeHTTP::BearerAuth(Token.AccessToken);
	FString URL = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *FJusticeBaseURL, *Token.Namespace, *Token.UserID);
	FString Verb = POST;
	FString ContentType = TYPE_JSON;
	FString Accept = TYPE_JSON;
	FString Payload = FString::Printf(TEXT("{ \"LoginID\": \"%s\"}"), *LoginID);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnReissueVerificationCodeResponse, OnComplete));
}

void JusticeIdentity::OnReissueVerificationCodeResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("ReissueVerificationCode Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
		UE_LOG(LogJustice, VeryVerbose, TEXT("OnReissueVerificationCodeResponse : Operation succeeded"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	case EHttpResponseCodes::BadRequest:
		ErrorStr = TEXT("Invalid Request");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::Denied:
	{
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnReissueVerificationCodeResponse, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr);
				return;
			}
		}));
		return;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnReissueVerificationCodeResponse, OnComplete));
		return;
		break;
	}
	case EHttpResponseCodes::TooManyRequests:
	{
		ErrorStr = FString::Printf(TEXT("You have to wait 15 minutes before request another ReissueVerificationCode. Response Code=%d"), Response->Code);
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpcted Response Code: %d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("ReissueVerificationCode Error: %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ClientLogin()
{
	FScopeLock Lock(&Mutex);

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("grant_type=client_credentials"));
	bool bClientCredentialReady = false;

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLoginResponse, &bClientCredentialReady) );


	// spin lock here
	double LastTime = FPlatformTime::Seconds();
	while (!bClientCredentialReady)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	UE_LOG(LogJustice, Log, TEXT("GameClient Login Complete"));


}

void JusticeIdentity::OnClientLoginResponse(FJusticeResponsePtr Response, bool *IsReady)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Game Client Credential Failed. Error Message: %s. XRay: %s"), *Response->ErrorString, *Response->AmazonTraceID);		
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		bool result = FJusticeSDKModule::Get().ParseClientToken(Response->Content);
		check(result);
		FJusticeGameClientToken->SetLastRefreshTimeToNow();
		FJusticeGameClientToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUTC);
		UE_LOG(LogJustice, Log, TEXT("Game Client Credential Success. XRay: %s"), *Response->AmazonTraceID);
		*IsReady = true;
		break;
	}
	case EHttpResponseCodes::Denied:
		ErrorStr = FString::Printf(TEXT("Client authentication failed, Response: %s"), *Response->Content);
		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			*IsReady = true;
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest, 
			Response->NextWait, 
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLoginResponse, IsReady));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
		break;
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Game Client Credential Failed. Error Message: %s. XRay: %s"), *ErrorStr, *Response->AmazonTraceID);		
		*IsReady = true;
		return;
	}
}

void JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete) 
{
	FString Authorization     = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);;
	FString URL               = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms"), *FJusticeBaseURL, *JusticeGameNamespace, *FJusticeUserID);
	FString Verb              = GET;
	FString ContentType       = TYPE_JSON;
	FString Accept            = TYPE_JSON;
	FString Payload           = TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnGetLinkedPlatformResponse, OnComplete));
}

void JusticeIdentity::OnGetLinkedPlatformResponse(FJusticeResponsePtr Response, FGetLinkedPlatformCompleteDelegate OnComplete)
{
	TArray<FLinkedPlatform> Result;
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Linked Platform Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, Result);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		TSharedPtr<FJsonValue> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->Content);
		
		if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
		{
			if (JsonObject.IsValid()) 
			{
				TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
				for (int32 platform = 0; platform != JsonArray.Num(); platform++)
				{
					FLinkedPlatform linkedPlatform;
					if (linkedPlatform.FromJson(JsonArray[platform]->AsObject()))
					{
						Result.Add(linkedPlatform);
					}
				}
				OnComplete.ExecuteIfBound(true, ErrorStr, Result);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(true, ErrorStr, Result);
		}
		break;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, Result);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnGetLinkedPlatformResponse, OnComplete));
		break;
	}


	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get User Linked Platforms Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<FLinkedPlatform>());
		return;
	}
}

void JusticeIdentity::LinkPlatform(FString Namespace, FString PlatformID, FString Ticket, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization  = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL            = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/link"), *FJusticeBaseURL, *Namespace, *FJusticeUserID, *PlatformID);
	FString Verb           = POST;
	FString ContentType    = TYPE_FORM;
	FString Accept         = TYPE_JSON;
	FString Payload        = FString::Printf(TEXT("ticket=%s"), *Ticket);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnLinkPlatformResponse, OnComplete));
}

void JusticeIdentity::OnLinkPlatformResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{

	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Link Platform Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
	{
		UE_LOG(LogJustice, VeryVerbose, TEXT("OnLinkPlatformComplete : Operation succeeded"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnLinkPlatformResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpected response Code=%d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Link Platform Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticeIdentity::UnlinkPlatform(FString Namespace, FString PlatformID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/unlink"), *FJusticeBaseURL, *Namespace, *FJusticeUserID, *PlatformID);
	FString Verb            = POST;
	FString ContentType     = TYPE_PLAIN;
	FString Accept          = TYPE_JSON;
	FString Payload         = TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUnlinkPlatformResponse, OnComplete));
}

void JusticeIdentity::OnUnlinkPlatformResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Unlink Platform Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
	{
		UE_LOG(LogJustice, VeryVerbose, TEXT("OnUnlinkPlatformComplete : Operation succeeded"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUnlinkPlatformResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpected response Code=%d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Unlink Platform Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticeIdentity::UpgradeHeadlessAccount(FString Namespace, FString UserID, FString Email, FString Password, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/upgradeHeadlessAccount"), *FJusticeBaseURL, *Namespace, *UserID);
	FString Verb            = POST;
	FString ContentType     = TYPE_JSON;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("{ \"LoginID\": \"%s\", \"Password\": \"%s\"}"), *Email, *Password);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUpgradeHeadlessAccountResponse, OnComplete));
}

void JusticeIdentity::OnUpgradeHeadlessAccountResponse(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Upgrade Headless Account Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		UE_LOG(LogJustice, Log, TEXT("Upgrade Headless Account : Operation succeeded"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUpgradeHeadlessAccountResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected response Code:%d Content:%s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Upgrade Headless Account Error: %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}