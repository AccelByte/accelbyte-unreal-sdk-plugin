// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
FCriticalSection Mutex;

void JusticeIdentity::ForgotPassword(FString LoginId, FForgotPasswordCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("{\"LoginID\": \"%s\"}"), *LoginId);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnForgotPasswordResponse, OnComplete));
}

void JusticeIdentity::OnForgotPasswordResponse(FJusticeHttpResponsePtr Response, FForgotPasswordCompleteDelegate OnComplete)
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
			FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (IsSuccess)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"));
						return;
					}
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnForgotPasswordResponse, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				return;
			}
		}));
		break;
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"));
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

void JusticeIdentity::ResetPassword(FString UserId, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDelegate OnComplete)
{
	ResetPasswordRequest resetPasswordRequest;
	resetPasswordRequest.Code = VerificationCode;
	resetPasswordRequest.LoginID = UserId;
	resetPasswordRequest.NewPassword = NewPassword;

	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= resetPasswordRequest.ToJson();

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnResetPasswordResponse, OnComplete));
}

void JusticeIdentity::OnResetPasswordResponse(FJusticeHttpResponsePtr Response, FResetPasswordCompleteDelegate OnComplete)
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
		JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (IsSuccess)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"));
						return;
					}
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnResetPasswordResponse, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				return;
			}
		}));

	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
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

void JusticeIdentity::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(LoginId), *FGenericPlatformHttp::UrlEncode(Password));;

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
		bool result = FJusticeSDKModule::Get().UserParseJson(Response->Content);
		check(result);
		FJusticeUserToken->SetLastRefreshTimeToNow();
		FJusticeUserToken->ScheduleNormalRefresh();		
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUtc);
		OnComplete.ExecuteIfBound(true, TEXT(""), FJusticeUserToken);
		break;
	}

	case EHttpResponseCodes::Denied:
		ErrorStr = FString::Printf(TEXT("User Login Failed, Response: %s. XRay: %s"), *Response->Content, *Response->AmazonTraceID);
		break;
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), nullptr);
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

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/namespaces/%s/platforms/device/token"), *FJusticeBaseURL, *FJusticeNamespace);;	
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("device_id=%s"), *deviceID);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLoginResponse, OnComplete));
}

void JusticeIdentity::UserLogout(FUserLogoutCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *FJusticeBaseURL);	
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeUserToken->AccessToken));

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnUserLogoutResponse, OnComplete));
}
void JusticeIdentity::OnUserLogoutResponse(FJusticeHttpResponsePtr Response, FUserLogoutCompleteDelegate OnComplete)
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
	FString RefreshToken = FJusticeSDKModule::GetModule().UserToken->UserRefreshToken;

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeSDKModule::GetModule().BaseURL);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *FGenericPlatformHttp::UrlEncode(RefreshToken));

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
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUtc);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		bool result = FJusticeSDKModule::Get().UserParseJson(Response->Content);
		check(result);
		FJusticeUserToken->SetLastRefreshTimeToNow();
		FJusticeUserToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUtc);
		OnComplete.ExecuteIfBound(true, TEXT(""), FJusticeUserToken);
		break;
	}		
	default:
		ErrorStr = FString::Printf(TEXT("User Refresh Unexpected Response. Code: %d, Response:%s, Start Backoff refresh !"), Response->Code, *Response->Content);
		FJusticeUserToken->ScheduleBackoffRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(JusticeIdentity::UserRefresh), FJusticeUserToken->NextTokenRefreshUtc);
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

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeSDKModule::GetModule().BaseURL);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("grant_type=client_credentials"));

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
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUtc);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FJusticeSDKModule::GetModule().GameClientParseJson(Response->Content);
		FJusticeGameClientToken->SetLastRefreshTimeToNow();
		FJusticeGameClientToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUtc);
		UE_LOG(LogJustice, Log, TEXT("Refresh Game Client Credential Success. XRay: %s"), *Response->AmazonTraceID);
		break;
	}
	default:
		UE_LOG(LogJustice, Error, TEXT("Refresh Game Client Credential Failed. Backoff refresh. Reponse Code: %d. Error Message: %s. XRay: %s"), Response->Code,  *Response->ErrorString, *Response->AmazonTraceID);
		FJusticeGameClientToken->ScheduleBackoffRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUtc);
		break;
	}
}

void JusticeIdentity::SetRefreshToken(FString UserRefreshToken)
{
	FJusticeUserToken->UserRefreshToken = UserRefreshToken;
}

void JusticeIdentity::ClientLogout()
{
	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeGameClientToken->AccessToken));

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
		FJusticeGameClientToken = new OAuthTokenJustice;
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

void JusticeIdentity::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
	FUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = UserId;
	NewUserRequest.AuthType = (AuthType == Email) ? TEXT("EMAILPASSWD"): TEXT("PHONEPASSWD");

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= NewUserRequest.ToJson();

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
		UserCreateResponse* userCreate = new UserCreateResponse();
		userCreate->FromJson(Response->Content);
		check(userCreate);

		ReissueVerificationCode(userCreate->UserId, 
			userCreate->LoginId, 
			FVerifyNewPlayerCompleteDelegate::CreateLambda([OnComplete, userCreate](bool IsSuccess, FString ErrorStr) {
			if (IsSuccess)
			{
				OnComplete.ExecuteIfBound(true, TEXT(""), userCreate);
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
			FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (IsSuccess)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), nullptr);
						return;
					}
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnRegisterNewPlayerResponse, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				return;					
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), nullptr);
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

void JusticeIdentity::VerifyNewPlayer(FString UserId, FString VerificationCode, FUserAuthTypeJustice AuthType, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ContactType = (AuthType == Email) ? TEXT("email") : TEXT("phone");

	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *FJusticeBaseURL, *FJusticeNamespace);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnVerifyNewPlayerResponse, OnComplete));
}

void JusticeIdentity::OnVerifyNewPlayerResponse(FJusticeHttpResponsePtr Response, FVerifyNewPlayerCompleteDelegate OnComplete)
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
			FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (IsSuccess)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					OnComplete.ExecuteIfBound(false, ErrorStr);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"));
						return;
					}
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnVerifyNewPlayerResponse, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("You token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"));
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

void JusticeIdentity::ReissueVerificationCode(FString UserId, FString LoginId, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeGameClientToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *FJusticeBaseURL, *FJusticeNamespace, *UserId);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("{ \"LoginID\": \"%s\"}"), *LoginId);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeIdentity::OnReissueVerificationCodeResponse, OnComplete));
}
void JusticeIdentity::OnReissueVerificationCodeResponse(FJusticeHttpResponsePtr Response, FVerifyNewPlayerCompleteDelegate OnComplete)
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
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		ErrorStr = FString::Printf(TEXT("You have to wait 15 minutes before request another ReissueVerificationCode. Response Code=%d"), Response->Code);
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ClientLogin(FUserLoginCompleteDelegate OnComplete)
{
	FScopeLock Lock(&Mutex);

	FString Authorization	= FJusticeHTTP::BasicAuth();
	FString URL				= FString::Printf(TEXT("%s/iam/oauth/token"), *FJusticeBaseURL);
	FString Verb			= POST;
	FString ContentType		= TYPE_FORM;
	FString Accept			= TYPE_JSON;
	FString Payload			= FString::Printf(TEXT("grant_type=client_credentials"));

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
		bool result = FJusticeSDKModule::Get().GameClientParseJson(Response->Content);
		check(result);
		FJusticeGameClientToken->SetLastRefreshTimeToNow();
		FJusticeGameClientToken->ScheduleNormalRefresh();
		FJusticeRefreshManager->AddQueue(FOnJusticeTickDelegate::CreateStatic(ClientRefreshToken), FJusticeGameClientToken->NextTokenRefreshUtc);
		UE_LOG(LogJustice, Log, TEXT("Game Client Credential Success. XRay: %s"), *Response->AmazonTraceID);
		OnComplete.ExecuteIfBound(true, TEXT(""), FJusticeGameClientToken);
		break;
	}
	case EHttpResponseCodes::Denied:
		ErrorStr = FString::Printf(TEXT("Client authentication failed, Response: %s"), *Response->Content);
		break;
	case EHttpResponseCodes::RequestTimeout:
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, TEXT("Timeout, too many retries"), nullptr);
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
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserId = GetUserId();

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms"), *BaseURL, *Namespace, *UserId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnGetLinkedPlatformComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("GetLinkedPlatform failed. Error=%s XrayID=%s ReqTime=%.3f"));
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<LinkedPlatform>());
	}
}

void JusticeIdentity::OnGetLinkedPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetLinkedPlatformCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid()) 
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();
			UE_LOG(LogJustice, Log, TEXT("OnGetLinkedPlatformComplete : %s"), *ResponseStr);

			TSharedPtr<FJsonValue> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			TArray<LinkedPlatform> Result;

			if (ResponseStr.Contains("null")) {
				OnComplete.ExecuteIfBound(true, ErrorStr, TArray<LinkedPlatform>());
			}
			else
			if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
			{
				if (JsonObject.IsValid()) 
				{
					TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
					for (int platform = 0; platform != JsonArray.Num(); platform++)
					{
						LinkedPlatform linkedPlatform;
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
				ErrorStr = TEXT("unable to deserialize response from server");
			}
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("On Get User Linked Platforms Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<LinkedPlatform>());
		return;
	}
}

void JusticeIdentity::LinkPlatform(FString PlatformId, FString Ticket, FLinkPlatformCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserId = GetUserId();
	FString TicketForm = FString::Printf(TEXT("ticket=%s"), *Ticket);
	
	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/link"), *BaseURL, *Namespace, *UserId, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(TicketForm);
	
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLinkPlatformComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("LinkPlatform failed. Error=%s XrayID=%s ReqTime%.3f"));
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::OnLinkPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FLinkPlatformCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else 
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
			UE_LOG(LogJustice, VeryVerbose, TEXT("OnLinkPlatformComplete : Operation succeeded"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		default:
			ErrorStr = FString::Printf(TEXT("unexpected response code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("On Link Platform Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticeIdentity::UnlinkPlatform(FString PlatformId, FUnlinkPlatformCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserId = GetUserId();

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/unlink"), *BaseURL, *Namespace, *UserId, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=UTF-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnUnlinkPlatformComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UnlinkPlatform failed. Error=%s XrayID=%s ReqTime%.3f"));
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::OnUnlinkPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUnlinkPlatformCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
			UE_LOG(LogJustice, VeryVerbose, TEXT("OnUnlinkPlatformComplete : Operation succeeded"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		default:
			ErrorStr = FString::Printf(TEXT("unexpected response code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("On Unlink Platform Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticeIdentity::ClientLogin()
{
	Login(TEXT(""), TEXT(""), FGrantTypeJustice::ClientCredentialGrant, OnComplete);
}

OAuthTokenJustice * JusticeIdentity::GetUserToken()
{
	return FJusticeSDKModule::Get().UserToken;
}

OAuthTokenJustice * JusticeIdentity::GetClientToken()
{
	return FJusticeSDKModule::Get().GameClientToken;
}

FString JusticeIdentity::GetUserId()
{
	return FJusticeSDKModule::Get().UserToken->UserId;
}