// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSingleton.h"
#include "JusticeLog.h"


void JusticeIdentity::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = UJusticeSingleton::Instance()->BaseURL;
	FString Namespace = UJusticeSingleton::Instance()->Namespace;
	FString ClientID = UJusticeSingleton::Instance()->ClientID;
	FString ClientSecret = UJusticeSingleton::Instance()->ClientSecret;

	Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"),
		*FGenericPlatformHttp::UrlEncode(LoginId), *FGenericPlatformHttp::UrlEncode(Password));

	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace) {
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
					if (UJusticeSingleton::Instance()->UserToken->FromJson(JsonObject))
					{
						TArray<TSharedPtr<FJsonValue>> PermissionArray = JsonObject->GetArrayField(TEXT("permissions"));
						for (TSharedPtr<FJsonValue> Permission : PermissionArray)
						{
							TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
							FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
							int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
							FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
							UJusticeSingleton::Instance()->UserToken->Permissions.Add(PermissionObject);
							//UserAccountPtr->SetUserAttribute(Resource, FString::FromInt(Action));
						}
						UJusticeSingleton::Instance()->UserToken->SetLastRefreshTimeToNow();
						UJusticeSingleton::Instance()->UserToken->ScheduleNormalRefresh();
						//UserAccountPtr->SetUserId(UserAccountPtr->Token->UserId);


						//OnlineAsyncTaskManagerJustice->UpdateDelegateSchedule(FTaskTypeJustice::IdentityRefresh,
						//	FTimespan::FromSeconds((UserAccountPtr->Token->ExpiresIn + 1) * 0.8),
						//	UserAccountPtr->Token->NextTokenRefreshUtc,
						//	OnRefreshTokenLogDelegate);

						OnComplete.Execute(true, TEXT(""), UJusticeSingleton::Instance()->UserToken);

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
			{
				FString ResponseStr = Response->GetContentAsString();
				ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d  Response=%s"), Response->GetResponseCode(), *ResponseStr);

			}
			}
		}

		if (!ErrorStr.IsEmpty())
		{
			UE_LOG(LogJustice, Warning, TEXT("Token password grant failed. User=%s Error=%s %s %s ReqTime=%.3f"),
				*UJusticeSingleton::Instance()->UserToken->UserId, *ErrorStr, *UJusticeSingleton::Instance()->UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
			//UserAccountPtr->Token = nullptr;//CreateDefaultSubobject<UOAuthTokenJustice>(TEXT("Token"));
			//TriggerOnLoginCompleteDelegates(LocalUserNum, false, *UserAccountPtr->GetUserId(), *ErrorStr);
			//TriggerOnLoginChangedDelegates(LocalUserNum);
			return;
		}

		UE_LOG(LogJustice, Log, TEXT("Token password grant successful. UserId=%s %s %s ReqTime=%.3f"),
			*UJusticeSingleton::Instance()->UserToken->UserId, *UJusticeSingleton::Instance()->UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		//TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserAccountPtr->GetUserId(), *ErrorStr);
		//TriggerOnLoginChangedDelegates(LocalUserNum);

	}, RequestTrace);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::UserLogout(FUserLogoutCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	
	FString BaseURL = UJusticeSingleton::Instance()->BaseURL;
	FString Namespace = UJusticeSingleton::Instance()->Namespace;
	FString ClientID = UJusticeSingleton::Instance()->ClientID;
	FString ClientSecret = UJusticeSingleton::Instance()->ClientSecret;


	Request->SetURL(BaseURL + TEXT("/iam/oauth/revoke"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(UJusticeSingleton::Instance()->UserToken->AccessToken));

	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace) {
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
				*UJusticeSingleton::Instance()->UserToken->UserId, *ErrorStr, *UJusticeSingleton::Instance()->UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());

			return;
		}

		OnComplete.Execute(true, TEXT(""));

	}, RequestTrace);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr);
	}
}

void JusticeIdentity::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = UJusticeSingleton::Instance()->BaseURL;
	FString Namespace = UJusticeSingleton::Instance()->Namespace;

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(UJusticeSingleton::Instance()->GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FUserCreateRequest newUser;
	newUser.DisplayName = UserId;
	newUser.Password = Password;
	newUser.LoginId = UserId;
	newUser.AuthType = AuthType;// EMAILPASSWD or PHONEPASSWD

	Request->SetContentAsString(newUser.ToJson());
	//Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenRegisterPlayerComplete, RequestTrace);
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace) {
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
	}, RequestTrace);

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

void JusticeIdentity::VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = UJusticeSingleton::Instance()->BaseURL;
	FString Namespace = UJusticeSingleton::Instance()->Namespace;

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/") + UserId + TEXT("/verification"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(UJusticeSingleton::Instance()->GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Body = FString::Printf(TEXT("{ \"Code\": \"%s\"}"), *VerificationCode);
	Request->SetContentAsString(Body);
	//Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenVerifyNewPlayerComplete, UserId, RequestTrace);
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, FString UserId, TSharedRef<FAWSXRayJustice> RequestTrace) {
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
	}, UserId, RequestTrace);
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
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(UJusticeSingleton::Instance()->GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("loginId=%s"), *LoginId);
	Request->SetContentAsString(Grant);
	//Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::ForgotPasswordStep1Complete, RequestTrace);
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

	FString BaseURL = UJusticeSingleton::Instance()->BaseURL;
	FString Namespace = UJusticeSingleton::Instance()->Namespace;
	FString ClientID = UJusticeSingleton::Instance()->ClientID;
	FString ClientSecret = UJusticeSingleton::Instance()->ClientSecret;

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/resetpasswordstub"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(UJusticeSingleton::Instance()->GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("UserId=%s&Verification=%s&NewPassword=%s"), *UserId, *VerificationCode, *NewPassword);
	Request->SetContentAsString(Grant);
	//Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::ForgotPasswordStep2Complete, RequestTrace);
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
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	UJusticeSingleton* test = UJusticeSingleton::Instance();

	FString BaseURL = UJusticeSingleton::Instance()->BaseURL;
	FString Namespace = UJusticeSingleton::Instance()->Namespace;
	FString ClientID = UJusticeSingleton::Instance()->ClientID;
	FString ClientSecret = UJusticeSingleton::Instance()->ClientSecret;

	Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("grant_type=client_credentials"));
	Request->SetContentAsString(Grant);
	//Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenGameClientCredentialComplete, RequestTrace);
	Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace) {
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
					if (UJusticeSingleton::Instance()->GameClientToken->FromJson(JsonObject))
					{
						TArray<TSharedPtr<FJsonValue>> PermissionArray = JsonObject->GetArrayField(TEXT("permissions"));
						for (TSharedPtr<FJsonValue> Permission : PermissionArray)
						{
							TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
							FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
							int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
							FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
							UJusticeSingleton::Instance()->GameClientToken->Permissions.Add(PermissionObject);

						}
						UJusticeSingleton::Instance()->GameClientToken->SetLastRefreshTimeToNow();
						UJusticeSingleton::Instance()->GameClientToken->ScheduleNormalRefresh();
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
	}, RequestTrace);


	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ClientLogin Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ClientLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

UOAuthTokenJustice * JusticeIdentity::GetUserToken()
{
	return UJusticeSingleton::Instance()->UserToken;
}

UOAuthTokenJustice * JusticeIdentity::GetClientToken()
{
	return UJusticeSingleton::Instance()->GameClientToken;
}

FString JusticeIdentity::GetUserId()
{
	return UJusticeSingleton::Instance()->UserToken->UserId;
}




