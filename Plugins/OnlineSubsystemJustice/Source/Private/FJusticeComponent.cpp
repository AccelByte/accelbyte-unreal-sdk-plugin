// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "FJusticeComponent.h"
#include "OnlineIdentityJustice.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"

#include "Misc/ConfigCacheIni.h"

UFJusticeComponent* UFJusticeComponent::Instance = nullptr;

void UFJusticeComponent::RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate onComplete)
{
	const IOnlineSubsystem* OnlineSub;
	IOnlineIdentityPtr Identity;
	FString BaseURL;
	FString JusticeUserId;
	FString JusticeUserNamespace;
	FString ErrorStr;
	OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	Identity = OnlineSub->GetIdentityInterface();	
	FOnlineIdentityJustice* JusticeIdentity = (FOnlineIdentityJustice*)Identity.Get();
	FString token = JusticeIdentity->GetAuthToken(0); // 0 = localUserNum
	PlayerProfileCompleteDelegate = onComplete;

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	
	TSharedPtr<const FUniqueNetId> UserId = JusticeIdentity->GetUniquePlayerId(0);
	if (UserId.IsValid())
	{
		TSharedPtr<FUserOnlineAccount> UserAccount = JusticeIdentity->GetUserAccount(*UserId);
		FUserOnlineAccountJustice* justiceUserAccount = (FUserOnlineAccountJustice*)UserAccount.Get();
	
		JusticeUserId = justiceUserAccount->Token.UserId;
		JusticeUserNamespace = justiceUserAccount->Token.Namespace;	
	}
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	Request->SetURL(BaseURL + TEXT("/platform/api/namespace/") + JusticeUserNamespace + TEXT("/players/") + JusticeUserId);
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(token));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	
	UE_LOG(LogOnline, VeryVerbose, TEXT("Attemp to call Player Profile: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenPlayerProfileComplete, JusticeUserId, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::RequestCurrentPlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		PlayerProfileCompleteDelegate.Execute(false, ErrorStr, NewObject<UUserProfileJustice>());
	}
}

void UFJusticeComponent::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType, FRegisterPlayerCompleteDelegate onComplete)
{	
	FString ErrorStr;
	FString BaseURL;
	FString Namespace;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	RegisterPlayerCompleteDelegate = onComplete;
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace +  TEXT("/users"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(GameClientToken.AccessToken));
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
	Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenRegisterPlayerComplete, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::RegisterNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		RegisterPlayerCompleteDelegate.Execute(false, ErrorStr, NewObject<UUserCreateResponse>());
	}
}

void UFJusticeComponent::VerifyNewPlayer(FString UserId, FString VerificationCode, FVerifyNewPlayerCompleteDelegate onComplete)
{
	FString ErrorStr;
	FString BaseURL;
	FString Namespace;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	VerifyNewPlayerCompleteDelegate = onComplete;
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/") + UserId + TEXT("/verification"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(GameClientToken.AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Body = FString::Printf(TEXT("{ \"Code\": \"%s\"}"), *VerificationCode);
	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenVerifyNewPlayerComplete, UserId, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("VerifyNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		VerifyNewPlayerCompleteDelegate.Execute(false, ErrorStr);
	}
}

void UFJusticeComponent::ClientCredentialLogin()
{
	FString ErrorStr;
	FString ClientID;
	FString ClientSecret;
	FString BaseURL;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("grant_type=client_credentials"));
	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenGameClientCredentialComplete, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG_ONLINE(VeryVerbose, TEXT("UFJusticeComponent::ClientCredentialLogin Sucessful, XRayID= %s"),  *RequestTrace->ToString());
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::ClientCredentialLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());	
	}
}

void UFJusticeComponent::ForgotPasswordStep1(FString LoginId)
{
	FString ErrorStr;
	FString ClientID;
	FString ClientSecret;
	FString BaseURL;
	FString Namespace;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/forgotpassword"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(GameClientToken.AccessToken));
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
	UE_LOG_ONLINE(VeryVerbose, TEXT("UFJusticeComponent::ForgotPasswordStep1 Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::ForgotPasswordStep1 failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void UFJusticeComponent::ForgotPasswordStep2(FString UserId, FString VerificationCode, FString NewPassword)
{
	FString ErrorStr;
	FString ClientID;
	FString ClientSecret;
	FString BaseURL;
	FString Namespace;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/namespaces/") + Namespace + TEXT("/users/resetpassword"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(GameClientToken.AccessToken));
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
	UE_LOG_ONLINE(VeryVerbose, TEXT("UFJusticeComponent::ForgotPasswordStep2 Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::ForgotPasswordStep2 failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void UFJusticeComponent::TokenGameClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
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
				if (GameClientToken.FromJson(JsonObject))
				{
					TArray<TSharedPtr<FJsonValue>> PermissionArray = JsonObject->GetArrayField(TEXT("permissions"));
					for (TSharedPtr<FJsonValue> Permission : PermissionArray)
					{
						TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
						FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
						int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
						FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
						GameClientToken.Permissions.Add(PermissionObject);
						
					}
					GameClientToken.SetLastRefreshTimeToNow();
					GameClientToken.ScheduleNormalRefresh();
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
		UE_LOG_ONLINE(Warning, TEXT("Game Client Credential failed. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}

	UE_LOG_ONLINE(Log, TEXT("Game Client Credential successful. ReqTime=%.3f"),Request->GetElapsedTime());
	
}

void UFJusticeComponent::TokenRegisterPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
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
			UE_LOG_ONLINE(VeryVerbose, TEXT("TokenRegisterPlayerComplete : Entity Created"));
			//parse json
			FString ResponseStr = Response->GetContentAsString();
			UUserCreateResponse* pUserCreateResponse = NewObject<UUserCreateResponse>();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (pUserCreateResponse->FromJson(JsonObject))
				{
					RegisterPlayerCompleteDelegate.Execute(true, TEXT(""), pUserCreateResponse);
				}
				else
				{
					RegisterPlayerCompleteDelegate.Execute(false, TEXT("Cannot serialize to UserCreateResponse"), NewObject<UUserCreateResponse>());
				}
			}
			else
			{
				RegisterPlayerCompleteDelegate.Execute(false, TEXT("Invalid json response"), NewObject<UUserCreateResponse>());
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
		UE_LOG_ONLINE(Error, TEXT("TokenRegisterPlayerComplete. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		
		RegisterPlayerCompleteDelegate.Execute(false, ErrorStr, NewObject<UUserCreateResponse>());
	}


}

void UFJusticeComponent::TokenPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, FString UserId, TSharedRef<FAWSXRayJustice> RequestTrace)
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
			UUserProfileJustice* pUserProfileJustice = NewObject<UUserProfileJustice>();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (!pUserProfileJustice->FromJson(JsonObject))
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
				PlayerProfileCompleteDelegate.Execute(true, ErrorStr, pUserProfileJustice);
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
		}
		break;
	
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogOnline, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		PlayerProfileCompleteDelegate.Execute(false, ErrorStr, NewObject<UUserProfileJustice>());
		return;
	}

}

void UFJusticeComponent::TokenVerifyNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, FString UserId, TSharedRef<FAWSXRayJustice> RequestTrace)
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
			UE_LOG_ONLINE(VeryVerbose, TEXT("TokenVerifyNewPlayerComplete : Player Activated"));
			VerifyNewPlayerCompleteDelegate.Execute(true, TEXT(""));
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
		UE_LOG_ONLINE(Error, TEXT("VerifyNewPlayerCompleteDelegate. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());

		VerifyNewPlayerCompleteDelegate.Execute(false, ErrorStr);
	}
}
