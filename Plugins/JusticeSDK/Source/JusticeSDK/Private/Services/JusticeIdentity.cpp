// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
FCriticalSection Mutex;

void JusticeIdentity::ForgotPassword(FString LoginID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
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

void JusticeIdentity::OnForgotPasswordResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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

void JusticeIdentity::ResetPassword(FString UserID, FString VerificationCode, FString NewPassword, FDefaultCompleteDelegate OnComplete)
{
	FResetPasswordRequest resetPasswordRequest;
	resetPasswordRequest.Code = VerificationCode;
	resetPasswordRequest.LoginID = UserID;
	resetPasswordRequest.NewPassword = NewPassword;

	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
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

void JusticeIdentity::OnResetPasswordResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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


void JusticeIdentity::AuthCodeLogin(FString AuthCode, FString RedirectURI, FUserLoginCompleteDelegate OnComplete)
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


void JusticeIdentity::UserLogin(FString LoginID, FString Password, FUserLoginCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(LoginID), *FGenericPlatformHttp::UrlEncode(Password));;

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLoginResponse, OnComplete));
}

void JusticeIdentity::OnUserLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete)
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
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLoginResponse, OnComplete));
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
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/namespaces/%s/platforms/device/token"), *FJusticeBaseURL, *FJusticeNamespace);;	
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

void JusticeIdentity::OnDeviceLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete)
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
	FString Payload         = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeUserToken->AccessToken));

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLogoutResponse, OnComplete));
}
void JusticeIdentity::OnUserLogoutResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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

void JusticeIdentity::OnUserRefreshResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete)
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

void JusticeIdentity::OnClientRefreshResponse(FJusticeHttpResponsePtr Response)
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

void JusticeIdentity::ClientLogout()
{
	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeGameClientToken->AccessToken));

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLogoutResponse));
}

void JusticeIdentity::OnClientLogoutResponse(FJusticeHttpResponsePtr Response)
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

void JusticeIdentity::RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
	FUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginID = UserID;
	NewUserRequest.AuthType = (AuthType == Email) ? TEXT("EMAILPASSWD"): TEXT("PHONEPASSWD");

	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *FJusticeBaseURL, *FJusticeNamespace);
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

void JusticeIdentity::OnRegisterNewPlayerResponse(FJusticeHttpResponsePtr Response, FRegisterPlayerCompleteDelegate OnComplete)
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
		FUserCreateResponse userCreate;
		userCreate.FromJson(Response->Content);

		ReissueVerificationCode(userCreate.UserID, 
			userCreate.LoginID, 
			FDefaultCompleteDelegate::CreateLambda([OnComplete, &userCreate](bool bSuccessful, FString ErrorStr) {
			if (bSuccessful)
			{
				OnComplete.ExecuteIfBound(true, TEXT(""), &userCreate);
			}
			else
			{
				OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			}
		}));
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
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, FOAuthTokenJustice* Token) {
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

void JusticeIdentity::VerifyNewPlayer(FString UserID, FString VerificationCode, FUserAuthTypeJustice AuthType, FDefaultCompleteDelegate OnComplete)
{
	FString ContactType = (AuthType == Email) ? TEXT("email") : TEXT("phone");

	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verification"), *FJusticeBaseURL, *FJusticeNamespace, *UserID);
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

void JusticeIdentity::OnVerifyNewPlayerResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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

void JusticeIdentity::ReissueVerificationCode(FString UserID, FString LoginID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *FJusticeBaseURL, *FJusticeNamespace, *UserID);
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
void JusticeIdentity::OnReissueVerificationCodeResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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

void JusticeIdentity::ClientLogin(FUserLoginCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization   = FJusticeHTTP::BasicAuth();
	FString URL             = FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb            = POST;
	FString ContentType     = TYPE_FORM;
	FString Accept          = TYPE_JSON;
	FString Payload         = FString::Printf(TEXT("grant_type=client_credentials"));

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLoginResponse, OnComplete) );
}

void JusticeIdentity::OnClientLoginResponse(FJusticeHttpResponsePtr Response, FUserLoginCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Game Client Credential Failed. Error Message: %s. XRay: %s"), *Response->ErrorString, *Response->AmazonTraceID);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, nullptr);
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
		OnComplete.ExecuteIfBound(true, TEXT(""), FJusticeGameClientToken.Get());
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
			OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest, 
			Response->NextWait, 
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnClientLoginResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
		break;
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Game Client Credential Failed. Error Message: %s. XRay: %s"), *ErrorStr, *Response->AmazonTraceID);
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}
}

void JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete) 
{
	FString Authorization     = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);;
	FString URL               = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID);
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

void JusticeIdentity::OnGetLinkedPlatformResponse(FJusticeHttpResponsePtr Response, FGetLinkedPlatformCompleteDelegate OnComplete)
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

void JusticeIdentity::LinkPlatform(FString PlatformID, FString Ticket, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization  = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL            = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/link"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID, *PlatformID);
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

void JusticeIdentity::OnLinkPlatformResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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

void JusticeIdentity::UnlinkPlatform(FString PlatformID, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/unlink"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID, *PlatformID);
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

void JusticeIdentity::OnUnlinkPlatformResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
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

void JusticeIdentity::UpgradeHeadlessAccount(FString Email, FString Password, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization   = FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL             = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/upgradeHeadlessAccount"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID);
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
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUpgradeHeadlessAccountResponse, OnComplete, Email));
}

void JusticeIdentity::OnUpgradeHeadlessAccountResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete, FString LoginID)
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
		UE_LOG(LogJustice, Log, TEXT("Upgrade Headless Account : Send Verification Code"));
		JusticeIdentity::ReissueVerificationCode(
			*FJusticeUserID, 
			LoginID, 
			FDefaultCompleteDelegate::CreateLambda([=](bool bSuccessful, FString ErrorStr) {
				if (bSuccessful)
				{
					UE_LOG(LogJustice, Log, TEXT("Upgrade Headless Account : Verification Code Success"));
					OnComplete.ExecuteIfBound(true, TEXT(""));
				}
				else
				{
					OnComplete.ExecuteIfBound(false, ErrorStr);
				}
			}));		
		return;
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
			FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUpgradeHeadlessAccountResponse, OnComplete, LoginID));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected response Code:%d Content:%s"), Response->Code, *Response->Content);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Upgrade Headless Account Error: %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}