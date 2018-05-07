// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
#include "Private/Models/FUserCreateResponse.h"
//static FOnScheduleTickDelegate onRefreshDelegate;
FCriticalSection Mutex;

class FRefreshTokenAsyncTask : public FJusticeAsyncTask
{
public:
	FRefreshTokenAsyncTask(FDateTime nextUpdate)
		:FJusticeAsyncTask(nextUpdate)
	{}

	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		JusticeIdentity::RefreshToken(nullptr);
		SetAsDone();
	}
};


void JusticeIdentity::Login(FString LoginId, FString Password, FGrantTypeJustice GrantType, FUserLoginCompleteDelegate OnComplete)
{
	Mutex.Lock();
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	FJusticeSDKModule::Get().LoginId = LoginId;

	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = "";
	if (GrantType == FGrantTypeJustice::PasswordGrant)
	{		
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));
 		Grant = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(LoginId), *FGenericPlatformHttp::UrlEncode(Password));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLoginComplete, RequestTrace, OnComplete);
	}
	else if (GrantType == FGrantTypeJustice::RefreshGrant)
	{		
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));
		FString RefreshToken = FJusticeSDKModule::Get().UserToken->RefreshToken;
		Grant = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"),*FGenericPlatformHttp::UrlEncode(RefreshToken));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnRefreshComplete, RequestTrace, OnComplete);
	}
	else if (GrantType == FGrantTypeJustice::ClientCredentialGrant)
	{
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));		
		Grant = FString::Printf(TEXT("grant_type=client_credentials"));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientCredentialComplete, RequestTrace);			
	}
	else if (GrantType == FGrantTypeJustice::Anonymous)
	{		
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));
		Grant = FString::Printf(TEXT("grant_type=anonymous"));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLoginComplete, RequestTrace, OnComplete);
	}

	Request->SetContentAsString(Grant);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
	Mutex.Unlock();
}

void JusticeIdentity::OnLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete)
{
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

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (FJusticeSDKModule::Get().UserParseJson(JsonObject))
				{
						FRefreshTokenAsyncTask* NewTask = new FRefreshTokenAsyncTask(FJusticeSDKModule::Get().UserToken->NextTokenRefreshUtc);
						FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);
						UOAuthTokenJustice* newToken = NewObject<UOAuthTokenJustice>();
						newToken->FromParent(FJusticeSDKModule::Get().UserToken);
						OnComplete.ExecuteIfBound(true, TEXT(""), newToken);
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d  Response=%s"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Log, TEXT("Token password grant failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}

	UE_LOG(LogJustice, Log, TEXT("Token password grant successful. UserId=%s %s %s ReqTime=%.3f"),
		*FJusticeSDKModule::Get().UserToken->UserId, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
}

void JusticeIdentity::OnRefreshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete)
{
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

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (FJusticeSDKModule::Get().UserParseJson(JsonObject))
				{
					FRefreshTokenAsyncTask* NewTask = new FRefreshTokenAsyncTask(FJusticeSDKModule::Get().UserToken->NextTokenRefreshUtc);
					FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);

					UOAuthTokenJustice* newToken = NewObject<UOAuthTokenJustice>();
					newToken->FromParent(FJusticeSDKModule::Get().UserToken);
					OnComplete.ExecuteIfBound(true, TEXT(""), newToken);
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d  Response=%s"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Log, TEXT("Refresh grant failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}

	UE_LOG(LogJustice, Log, TEXT("Refresh grant successful. UserId=%s %s %s ReqTime=%.3f"),
		*FJusticeSDKModule::Get().UserToken->UserId, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
}

void JusticeIdentity::OnClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
{
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

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if(!FJusticeSDKModule::Get().GameClientParseJson(JsonObject))
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("Game Client Credential failed. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}
	UE_LOG(LogJustice, Log, TEXT("Game Client Credential successful. ReqTime=%.3f"), Request->GetElapsedTime());
}

void JusticeIdentity::ForgotPassword(FString LoginId, FForgotPasswordCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(FString::Printf(TEXT("{	\"LoginID\": \"%s\"}"), *LoginId));
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnForgotPasswordComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ForgotPassword Sucessful, XRayID= %s"), *RequestTrace->ToString());
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ForgotPassword failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::OnForgotPasswordComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FForgotPasswordCompleteDelegate OnComplete)
{
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
		{
			UE_LOG(LogJustice, Log, TEXT("OnForgotPasswordComplete receive success response "));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		}
		case EHttpResponseCodes::BadRequest:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Invalid Request. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::Forbidden:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Forbidden. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::NotFound:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Data not found. Code=%d"), Response->GetResponseCode());
			break;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}				
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnForgotPasswordComplete Error=%s ReqTime=%.3f"),*ErrorStr, Request->GetElapsedTime());		
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ResetPassword(FString UserId, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	ResetPasswordRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginID = UserId;
	ResetPasswordRequest.NewPassword = NewPassword;

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(ResetPasswordRequest.ToJson());
	UE_LOG(LogJustice, Log, TEXT("Attemp to call JusticeIdentity::ResetPassword: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnResetPasswordComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ResetPassword Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ResetPassword failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::OnResetPasswordComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FResetPasswordCompleteDelegate OnComplete)
{
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
		{
			UE_LOG(LogJustice, Log, TEXT("OnResetPasswordComplete receive success response "));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		}
		case EHttpResponseCodes::BadRequest:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Invalid Request. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::Forbidden:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Forbidden. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::NotFound:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Data not found. Code=%d"), Response->GetResponseCode());
			break;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnResetPasswordComplete Error=%s ReqTime=%.3f"), *ErrorStr, Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::Login(LoginId, Password, FGrantTypeJustice::PasswordGrant, OnComplete);
}

void JusticeIdentity::UserLogout(FUserLogoutCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());	
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeSDKModule::Get().UserToken->AccessToken));

	Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *BaseURL));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLogoutComplete, RequestTrace, OnComplete);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogout failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::AnonymousLogin(FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::Anonymous, OnComplete);
}

void JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::RefreshGrant, OnComplete);
}

void JusticeIdentity::OnLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLogoutCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		if (Response->GetResponseCode() == EHttpResponseCodes::Ok)
		{
			UE_LOG(LogJustice, Log, TEXT("Token logout receive success response "));
		}
		else
		{
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Token logout failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}
	OnComplete.ExecuteIfBound(true, TEXT(""));
}

void JusticeIdentity::ClientLogout()
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeSDKModule::Get().GameClientToken->AccessToken));

	Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *BaseURL));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientLogoutComplete, RequestTrace);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::ClientLogout failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}


void JusticeIdentity::SetRefreshToken(FString RefreshToken)
{
	FJusticeSDKModule::Get().UserToken->RefreshToken = RefreshToken;
}

void JusticeIdentity::OnClientLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		if (Response->GetResponseCode() == EHttpResponseCodes::Ok)
		{
			UE_LOG(LogJustice, Log, TEXT("Client logout receive success response "));
			FJusticeSDKModule::Get().GameClientToken = new OAuthTokenJustice;
			FJusticeSDKModule::Get().AsyncTaskManager->ClearRefreshQueue();		
		}
		else
		{
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Client  logout failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}
}

void JusticeIdentity::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = UserId;
	NewUserRequest.AuthType = AuthType;// EMAILPASSWD or PHONEPASSWD

	Request->SetContentAsString(NewUserRequest.ToJson());
	Request->OnProcessRequestComplete().BindStatic(JusticeIdentity::OnRegisterNewPlayerComplete, RequestTrace, OnComplete);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::RegisterNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::OnRegisterNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FRegisterPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Created:
		{
			UE_LOG(LogJustice, Log, TEXT("OnRegisterNewPlayerComplete : Entity Created"));
			//parse json
			FString ResponseStr = Response->GetContentAsString();
			FUserCreateResponse UserCreateResponse;
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (UserCreateResponse.FromJson(JsonObject))
				{
					UUserCreateResponse* pUserCreateResponse = NewObject<UUserCreateResponse>();
					pUserCreateResponse->LoadFromStruct(UserCreateResponse);
					ReissueVerificationCode(pUserCreateResponse->UserId, pUserCreateResponse->LoginId, FVerifyNewPlayerCompleteDelegate::CreateLambda([OnComplete, pUserCreateResponse](bool IsSuccess, FString ErrorStr) {
						if (IsSuccess)
						{
							OnComplete.ExecuteIfBound(true, TEXT(""), pUserCreateResponse);
						}
						else
						{
							OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
						}					
					}));
				}
				else
				{
					OnComplete.ExecuteIfBound(false, TEXT("Cannot serialize to UserCreateResponse"), nullptr);
				}
			}
			else
			{
				OnComplete.ExecuteIfBound(false, TEXT("Invalid json response"), nullptr);
			}
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
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnRegisterNewPlayerComplete. Error Message: %s XRay: %s ReqTime: %.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());

		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString Payload = FString::Printf(TEXT("{ \"Code\": \"%s\"}"), *VerificationCode);

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verification"), *BaseURL, *Namespace, *UserId));	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());	
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnVerifyNewPlayerComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("VerifyNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}


void JusticeIdentity::OnVerifyNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FVerifyNewPlayerCompleteDelegate OnComplete)
{
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
			UE_LOG(LogJustice, VeryVerbose, TEXT("TokenVerifyNewPlayerComplete : Player Activated"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("VerifyNewPlayerCompleteDelegate. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ReissueVerificationCode(FString UserId, FString LoginId, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString Payload = FString::Printf(TEXT("{ \"LoginID\": \"%s\"}"), *LoginId);

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *BaseURL, *Namespace, *UserId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnReissueVerificationCodeComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("ReissueVerificationCode failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}


void JusticeIdentity::OnReissueVerificationCodeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FVerifyNewPlayerCompleteDelegate OnComplete)
{
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
			UE_LOG(LogJustice, VeryVerbose, TEXT("OnReissueVerificationCodeComplete : Operation succeeded"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnReissueVerificationCodeComplete. Error:%s XRay: %s ReqTime: %.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ClientLogin()
{
	Login(TEXT(""), TEXT(""), FGrantTypeJustice::ClientCredentialGrant, nullptr);
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