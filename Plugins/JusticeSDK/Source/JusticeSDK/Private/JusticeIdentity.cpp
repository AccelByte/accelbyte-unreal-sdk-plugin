// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
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
		JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::RefreshGrant, nullptr);
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


	///iam/oauth/namespaces/{namespace}/token
	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = "";
	if (GrantType == FGrantTypeJustice::PasswordGrant)
	{
		Request->SetURL(BaseURL + TEXT("/iam/oauth/namespaces/") + Namespace + TEXT("/token"));
		Grant = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(LoginId), *FGenericPlatformHttp::UrlEncode(Password));		
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLoginComplete, RequestTrace, OnComplete);
	}
	else if (GrantType == FGrantTypeJustice::RefreshGrant)
	{
		Request->SetURL(BaseURL + TEXT("/iam/oauth/namespaces/") + Namespace + TEXT("/token"));
		FString RefreshToken = FJusticeSDKModule::Get().UserToken->RefreshToken;
		Grant = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"),*FGenericPlatformHttp::UrlEncode(RefreshToken));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnRefreshComplete, RequestTrace);		
	}
	else if (GrantType == FGrantTypeJustice::ClientCredentialGrant)
	{
		Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
		Grant = FString::Printf(TEXT("grant_type=client_credentials"));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientCredentialComplete, RequestTrace);			
	}
	else if (GrantType == FGrantTypeJustice::Anonymous)
	{
		Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
		Grant = FString::Printf(TEXT("grant_type=anonymous"));
		//Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientCredentialComplete, RequestTrace);
	}

	Request->SetContentAsString(Grant);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr, nullptr);
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
						OnComplete.Execute(true, TEXT(""), newToken);
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
		OnComplete.Execute(false, ErrorStr, nullptr);
		return;
	}

	UE_LOG(LogJustice, Log, TEXT("Token password grant successful. UserId=%s %s %s ReqTime=%.3f"),
		*FJusticeSDKModule::Get().UserToken->UserId, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
}

void JusticeIdentity::OnRefreshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
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


void JusticeIdentity::OnRefreshToken(FDateTime time, int32 nextTick)
{
	JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::RefreshGrant, nullptr);
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


	Request->SetURL(BaseURL + TEXT("/iam/oauth/revoke/token"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeSDKModule::Get().UserToken->AccessToken));

	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLogoutComplete, RequestTrace, OnComplete);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogout failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr);
	}
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
	OnComplete.Execute(true, TEXT(""));
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

	Request->SetURL(BaseURL + TEXT("/iam/oauth/revoke/token"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeSDKModule::Get().GameClientToken->AccessToken));

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

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FUserCreateRequest newUser;
	newUser.DisplayName = DisplayName;
	newUser.Password = Password;
	newUser.LoginId = UserId;
	newUser.AuthType = AuthType;// EMAILPASSWD or PHONEPASSWD

	Request->SetContentAsString(newUser.ToJson());
	Request->OnProcessRequestComplete().BindStatic(JusticeIdentity::OnRegisterNewPlayerComplete, RequestTrace, OnComplete);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::RegisterNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr, nullptr);
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
			UE_LOG(LogJustice, VeryVerbose, TEXT("TokenRegisterPlayerComplete : Entity Created"));
			//parse json
			FString ResponseStr = Response->GetContentAsString();
			UUserCreateResponse* pUserCreateResponse = NewObject<UUserCreateResponse>();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (pUserCreateResponse->FromJson(JsonObject))
				{
					OnComplete.Execute(true, TEXT(""), pUserCreateResponse);
				}
				else
				{
					OnComplete.Execute(false, TEXT("Cannot serialize to UserCreateResponse"), nullptr);
				}
			}
			else
			{
				OnComplete.Execute(false, TEXT("Invalid json response"), nullptr);
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
		UE_LOG(LogJustice, Error, TEXT("TokenRegisterPlayerComplete. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());

		OnComplete.Execute(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/") + UserId + TEXT("/verification"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Body = FString::Printf(TEXT("{ \"Code\": \"%s\"}"), *VerificationCode);
	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnVerifyNewPlayerComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("VerifyNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr);
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
			OnComplete.Execute(true, TEXT(""));
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

		OnComplete.Execute(false, ErrorStr);
	}
}
void JusticeIdentity::ForgotPasswordStep1(FString LoginId)
{
	FString ErrorStr;
	FString ClientID;
	FString ClientSecret;
	FString BaseURL;
	FString Namespace;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG(LogJustice, Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/forgotpasswordstub"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("loginId=%s"), *LoginId);
	Request->SetContentAsString(Grant);
	//Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnForgotPasswordStep1Complete,RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ForgotPasswordStep1 Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ForgotPasswordStep1 failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::ForgotPasswordStep2(FString UserId, FString VerificationCode, FString NewPassword)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/resetpasswordstub"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("UserId=%s&Verification=%s&NewPassword=%s"), *UserId, *VerificationCode, *NewPassword);
	Request->SetContentAsString(Grant);
	//Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnForgotPasswordStep2Complete,RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ForgotPasswordStep2 Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ForgotPasswordStep2 failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::LinkSteam(FUserLoginCompleteDelegate OnComplete)
{
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